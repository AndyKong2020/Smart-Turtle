#include <iostream>
#include <chrono>
#include <cmath>
#include <atomic>
#include <thread>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
//basic //ros
#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <dynamic_reconfigure/client.h>
#include <sensor_msgs/Image.h>
#include "rc_msgs/detection.h"
#include "rc_msgs/results.h"
#include "rc_msgs/point.h"
#include "std_msgs/Bool.h"
#include "rc_msgs/stepConfig.h"
//ros   //msgs
#include <sensor_msgs/Image.h>
#include <rc_msgs/stepConfig.h>
#include "rc_msgs/detection.h"
#include "rc_msgs/results.h"
#include "rc_msgs/point.h"
#include "std_msgs/Bool.h"
//msgs  //yolov7
#include "yolo/yolo.hpp"

using namespace std;

shared_ptr<Yolo::Infer> engine;
const auto using_mode = Yolo::Mode::FP16;   // 注意：修改此选项后必须删除原trtmodel才会生效
const int DEVICE = 0;
const int BATCH_SIZE = 1;                   // 注意：修改此选项后必须删除原trtmodel才会生效（不太需要动）
const float CONF_THRESH = .2;
const float NMS_THRESH = .4;

static const char *cocolabels[] = {
        "Tissues", "Toilet Rolls", "Soap", "Bottled Hand Sanitizer",
        "Porridge", "Chewing Gum", "Instant Noodles", "Potato Chips",
        "Canned Drinks", "Bottled Drinks", "Boxed Milk", "Bottled Water",
        "Apple", "Pear", "Banana", "Kiwi"
};

static std::tuple<uint8_t, uint8_t, uint8_t> hsv2bgr(float h, float s, float v) {
    const int h_i = static_cast<int>(h * 6);
    const float f = h * 6 - h_i;
    const float p = v * (1 - s);
    const float q = v * (1 - f * s);
    const float t = v * (1 - (1 - f) * s);
    float r, g, b;
    switch (h_i) {
        case 0:
            r = v;
            g = t;
            b = p;
            break;
        case 1:
            r = q;
            g = v;
            b = p;
            break;
        case 2:
            r = p;
            g = v;
            b = t;
            break;
        case 3:
            r = p;
            g = q;
            b = v;
            break;
        case 4:
            r = t;
            g = p;
            b = v;
            break;
        case 5:
            r = v;
            g = p;
            b = q;
            break;
        default:
            r = 1;
            g = 1;
            b = 1;
            break;
    }
    return make_tuple(static_cast<uint8_t>(b * 255), static_cast<uint8_t>(g * 255), static_cast<uint8_t>(r * 255));
}

static std::tuple<uint8_t, uint8_t, uint8_t> random_color(int id) {
    float h_plane = (float) ((((unsigned int) id << 2) ^ 0x937151) % 100) / 100.0f;
    float s_plane = (float) ((((unsigned int) id << 3) ^ 0x315793) % 100) / 100.0f;
    return hsv2bgr(h_plane, s_plane, 1);
}

inline long getModifyTime(char filePath[]) {
    struct stat buf{};
    auto fp = fopen(filePath, "r");
    int fd = fileno(fp);
    fstat(fd, &buf);
    fclose(fp);
    return buf.st_mtime;
}

inline long getNowTime() {
    timeval tv{};
    gettimeofday(&tv, nullptr);
    return tv.tv_sec;
}

cv::Mat inference(const cv::Mat &image) {
    assert(engine != nullptr);

    shared_future<Yolo::BoxArray> box;
    for (int i = 0; i < 10; ++i)
        box = engine->commit(image);

    auto bbox = box.get();
    for (auto &obj: bbox) {
        uint8_t b, g, r;
        tie(b, g, r) = random_color(obj.class_label);
        cv::rectangle(image, cv::Point(obj.left, obj.top), cv::Point(obj.right, obj.bottom), cv::Scalar(b, g, r), 5);

        auto name = cocolabels[obj.class_label];
        auto caption = cv::format("%s %.2f", name, obj.confidence);
        int width = cv::getTextSize(caption, 0, 1, 2, nullptr).width + 10;
        cv::rectangle(image, cv::Point(obj.left - 3, obj.top - 33), cv::Point(obj.left + width, obj.top),
                      cv::Scalar(b, g, r), -1);
        cv::putText(image, caption, cv::Point(obj.left, obj.top - 5), 0, 1, cv::Scalar::all(0), 2, 16);
    }
    return image;
}

std::atomic<bool> beatRun(true);
int step = 0;
ros::Publisher resPub;
ros::Publisher beatPub;

void imageCallback(const sensor_msgs::ImageConstPtr &msg) {

    rc_msgs::results Result;
    rc_msgs::detection tmp;
    cv::Mat img = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8)->image;
    Result.step = step;
    assert(engine != nullptr);

    shared_future<Yolo::BoxArray> box;
    for (int i = 0; i < 10; ++i)
        box = engine->commit(img);

    auto bbox = box.get();
    for (auto &obj: bbox) {
        rc_msgs::detection tmp;
        uint8_t b, g, r;
        tie(b, g, r) = random_color(obj.class_label);
        cv::rectangle(img, cv::Point(obj.left, obj.top), cv::Point(obj.right, obj.bottom), cv::Scalar(b, g, r), 2);
        tmp.x1 = obj.left;
        tmp.y1 = obj.top;
        tmp.x2 = obj.right;
        tmp.y2 = obj.bottom;
        auto name = cocolabels[obj.class_label];
        tmp.label = obj.class_label;
        tmp.score = obj.confidence;
        std::vector<rc_msgs::point> ps(4);
        ps[0].x = obj.left;
        ps[0].y = obj.top;
        ps[1].x = obj.right;
        ps[1].y = obj.top;
        ps[2].x = obj.right;
        ps[2].y = obj.bottom;
        ps[3].x = obj.left;
        ps[3].y = obj.bottom;
        tmp.contours = ps;
        auto caption = cv::format("%s %.2f", name, obj.confidence);
        int width = cv::getTextSize(caption, 0, 1, 1, nullptr).width + 10;
        cout << name << obj.confidence << endl;
        cv::rectangle(img, cv::Point(obj.left - 3, obj.top - 33), cv::Point(obj.left + width, obj.top),
                      cv::Scalar(b, g, r), -1);
        cv::putText(img, caption, cv::Point(obj.left, obj.top - 5), 0, 1, cv::Scalar::all(0), 1, 16);
        Result.results.emplace_back(tmp);
        std::cout << tmp.label << "   " << tmp.score << std::endl;
    }
    Result.color = *(cv_bridge::CvImage(std_msgs::Header(), "bgr8", img).toImageMsg());
    resPub.publish(Result);
}

void callback(const rc_msgs::stepConfig &config) {
    step = config.step;
}


void beatSend() {
    std::chrono::milliseconds duration(500);
    while (beatRun) {
        std_msgs::Bool beatMsg;
        beatMsg.data = true;
        beatPub.publish(beatMsg);
        std::this_thread::sleep_for(duration);
    }
}

//init for v7
shared_ptr<Yolo::Infer> init() {
    Yolo::set_device(DEVICE);

    if (access(ONNX_PATH, R_OK) != 0) {
        cerr << "ONNX File " << ONNX_PATH << " not exist!\n";
        exit(-1);
    }
    if (access(MODEL_PATH, R_OK) == 0) {
        long onnx_modify_time = getModifyTime(ONNX_PATH);
        long model_modify_time = getModifyTime(MODEL_PATH);
        long now_time = getNowTime();

        if (onnx_modify_time > now_time) {
            cerr << "ONNX File Modify Time is later than Now, Maybe Time Error!\n";
        }
        if (model_modify_time > now_time) {
            cerr << "Model File Modify Time is later than Now, Maybe Time Error!\n";
        }

        if (onnx_modify_time > model_modify_time) {
            cout << "Detected model file is older than onnx file, will rebuild engine!\n";
            Yolo::compile(
                    using_mode, Yolo::Type::V7,
                    BATCH_SIZE,
                    ONNX_PATH,
                    MODEL_PATH,
                    1 << 30,
                    "inference"
            );
        }
    } else {
        cout << "Model File " << MODEL_PATH << " not exist, will rebuild engine\n";
        Yolo::compile(
                using_mode, Yolo::Type::V7,
                BATCH_SIZE,
                ONNX_PATH,
                MODEL_PATH,
                1 << 30,
                "inference"
        );
    }

    return Yolo::create_infer(MODEL_PATH, Yolo::Type::V7, DEVICE, CONF_THRESH, NMS_THRESH);
}
//init for v7

int main(int argc, char **argv) {
    engine = init();
    ros::init(argc, argv, "yolo_identify");
    ros::NodeHandle n;

    ros::Subscriber imageSub = n.subscribe("/raw_img", 1, &imageCallback);
    resPub = n.advertise<rc_msgs::results>("/rcnn_results", 20);
    beatPub = n.advertise<std_msgs::Bool>("/nn_beat", 5);
    dynamic_reconfigure::Client<rc_msgs::stepConfig> client("/scheduler");

    client.setConfigurationCallback(&callback);

    std::thread beatThread = std::thread(&beatSend);

    ros::Rate loop_rate(10);
    while (ros::ok()) {
        ros::spinOnce();
        loop_rate.sleep();
    }
    beatRun = false;
    beatThread.join();
    engine.reset();
    return 0;
}


