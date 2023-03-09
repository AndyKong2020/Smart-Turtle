//
// Created by robin on 23-2-15.
//
#include <string>
#include <ros/ros.h>
#include <std_msgs/Int8.h>
#include "rc_msgs/identify_results.h"
#include "serialPro.h"
#include "robotComm.h"


robot::RobotSerial serial;

message_data identify {
    std::string over;
};

void identify_cb(const rc_msgs::identify_results::ConstPtr &msg) {
    rc_msgs::single_result analyzation;
    rc_msgs::identify_results identifyResults;
    static std::vector<std::string> score_cnt;

    identifyResults.identify_results = msg->identify_results;
    int length = identifyResults.identify_results.size();
    std::string cnt = "None";
    if (length != 1) {
        if (identifyResults.identify_results[0].x_coordinate < identifyResults.identify_results[1].x_coordinate) {
            cnt = identifyResults.identify_results[0].result + identifyResults.identify_results[1].result;
        } else {
            cnt = identifyResults.identify_results[1].result + identifyResults.identify_results[0].result;
        }
    } else {
        cnt = identifyResults.identify_results[0].result;
    }
    score_cnt.push_back(cnt);
    static int _check;
    for (int i = 0; i < score_cnt.size(); ++i)
    {
        if(score_cnt[i] == cnt){
            _check++;
        }
    }
    if(_check>=5){
        identify ide{
            .over = cnt
        };
        serial.write(0x88,ide);
        _check = 0;
    }
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "serial_robot");
    ros::NodeHandle nh("~");

    std::string serial_name;

    nh.param<std::string>("serial_name", serial_name, "/dev/ttyUSB0");
    serial = std::move(robot::RobotSerial(serial_name, 115200));


    ros::Subscriber resultsSub = nh.subscribe("/identify_results", 1, &identify_cb);
    serial.registerErrorHandle([](int label, const std::string &text) {  //日志记录
        robot::RobotSerial::error _label;
        _label = (robot::RobotSerial::error) label;
        std::stringstream _str;
        switch (_label) {
            case robot::RobotSerial::lengthNotMatch:
                ROS_ERROR_STREAM("lengthNotMatch");
            case robot::RobotSerial::rxLessThanLength:
                ROS_WARN_STREAM("rxLessThanLength");
                break;
            case robot::RobotSerial::crcError:
                ROS_ERROR_STREAM("crcError");
                break;
            default:
                return;
        }
        for (auto c: text) {
            _str << std::setw(2) << std::hex << (int) *(uint8_t *) &c << " ";
        }
        _str << std::endl;
        ROS_ERROR_STREAM(_str.str());
    });
    serial.spin(true);
    ros::spin();
}
