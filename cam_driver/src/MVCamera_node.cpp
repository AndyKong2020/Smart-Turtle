#include<iostream>
#include<vector>
using namespace std;

#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <sstream>
#include <iostream>
#include "opencv2/opencv.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include "MVCamera.h"
#include <std_msgs/Bool.h>
#include <std_msgs/Int8.h>
#include <std_msgs/Int16.h>
#include <std_msgs/String.h>
#include "video_saver.h"
using namespace std;
using namespace cv;
MVCamera *mv_driver=NULL;

ros::Time imgTime;
ros::Time lastImgTime;
unsigned char lastMode=0x02;
string ns;


class MVCamNode
{
public:
    ros::NodeHandle node_;
    int false_idx=0;
    // shared image message
    Mat rawImg;
    sensor_msgs::ImagePtr msg;
    //image_transport::Publisher image_pub_;
    ros::Publisher image_pub_;
    ros::Subscriber imgSize_sub;

    int deviceID=0, resolution_idx, framerate_, exposure_=1200, dafu_exposure_=5000, brightness_, contrast_, saturation_, sharpness_, focus_,
    white_balance_, gain_, fps_mode=1;
    bool  is_record_=false, autofocus_, autoexposure_=false, auto_white_balance_;
    string rcd_path_;
    VideoSaver saver;
    int height, width, offset_H, offset_W;
    int dafu_height, dafu_width, dafu_offset_H, dafu_offset_W;

    //constructor
    MVCamNode():
        node_()
    {
        ns = ros::this_node::getNamespace();
        image_transport::ImageTransport it(node_);
        image_pub_ =node_.advertise<sensor_msgs::Image>("MVCamera/image_raw", 1);
        ros::param::get("deviceID", deviceID);
        ros::param::get("height", height);
        ros::param::get("width", width);
        ros::param::get("offset_H", offset_H);
        ros::param::get("offset_W", offset_W);
        ros::param::get("dafu_height", dafu_height);
        ros::param::get("dafu_width", dafu_width);
        ros::param::get("dafu_offset_H", dafu_offset_H);
        ros::param::get("dafu_offset_W", dafu_offset_W);
        node_.param("/resolution_idx", resolution_idx, 1);
        node_.param("/framerate", framerate_, 120);
        node_.getParam("/is_record", is_record_);
        node_.getParam("/rcd_path", rcd_path_);
        rcd_path_=rcd_path_+ns+"/";
        node_.getParam("/fps_mode", fps_mode);
        node_.getParam("/exp_time", exposure_);
        ROS_WARN("%d",exposure_);
        node_.getParam("/dafu_exp_time", dafu_exposure_);


        //init camera param
        mv_driver=new MVCamera;
        mv_driver->Init(deviceID);
        mv_driver->SetDefault();
        mv_driver->SetExposureTime(autoexposure_, 40000);
        mv_driver->SetResolution(0, 640, 480, 0, 0);
        mv_driver->SetFPS(fps_mode);
        mv_driver->Play();

        imgSize_sub=node_.subscribe<std_msgs::Int8>("/serial/read",1,&MVCamNode::Mode_cb,this);//从串口获取模式信息
        imgTime=ros::Time::now();
        lastImgTime=ros::Time::now();
    }

    void Mode_cb(const std_msgs::Int8ConstPtr &msg)
    {
        unsigned char mode_normal=0x02;
        unsigned char mode_s_windMill=0x03;
        unsigned char mode_top=0x05;
        unsigned char mode_b_windMill=0x04;

        ROS_INFO_STREAM("Read: " << msg->data);
        ROS_WARN("debug: mode changed!!");

        unsigned char mode=(unsigned char)msg->data;
        unsigned char  WKmode=mode & 0x0F; //工作模式

        if(WKmode!=lastMode)
        {
             if(WKmode==mode_normal || WKmode==mode_top)
            {
                mv_driver->SetResolution(resolution_idx, height, width, offset_H, offset_W);
                mv_driver->SetExposureTime(autoexposure_, exposure_);
            }

            else if(WKmode==mode_b_windMill || WKmode==mode_s_windMill) //dafu
            {
                mv_driver->SetResolution(2, dafu_height,  dafu_width, dafu_offset_H, dafu_offset_W);
                mv_driver->SetExposureTime(autoexposure_, dafu_exposure_);
            }
            lastMode=WKmode;
        }   
        lastImgTime=ros::Time::now();
    }
    

    ~MVCamNode()
    {
        mv_driver->Stop();
        mv_driver->Uninit();
    }
    ///
    /// \brief get_exp
    /// get exposure time
    /// \param exp_time
    ///
    void get_exp(const std_msgs::Int16ConstPtr &exp_time)
    {
        if(exposure_!=exp_time->data)
        {
            exposure_=exp_time->data;
            mv_driver->SetExposureTime(autoexposure_, exposure_);

        }
    }

    void get_is_rcd(const std_msgs::BoolConstPtr &is_rcd)
    {
        if(is_record_!=is_rcd->data)
        {
            is_record_=is_rcd->data;

        }
    }
    string num2str(double i)
    {
        stringstream ss;
        ss << i;
        return ss.str();
    }
    ///
    /// \brief take_and_send_image
    /// use camera API in MVCamera.cpp
    /// \return
    ///
    bool take_and_send_image()
    {
        // grab the image

        mv_driver->GetFrame(rawImg,1);
        // imgTime=ros::Time::now();
        if(rawImg.empty())
        {
            ROS_WARN("NO IMG GOT FROM MV");
            return false;
        }
        if(is_record_)
        {
            saver.write(rawImg,rcd_path_);
        }


        std_msgs::Header imgHead;
        // imgHead.stamp=imgTime;
        msg= cv_bridge::CvImage(imgHead, "bgr8", rawImg).toImageMsg();
        // publish the image
        image_pub_.publish(msg);
        return true;
    }

    bool spin()
    {
        ros::Rate loop_rate(this->framerate_);
        while (node_.ok())
        {
            if (!mv_driver->stopped) 
            {
                if (!take_and_send_image()) 
                {
                    ROS_WARN("MVcamera did not respond in time.");
                    ros::shutdown();
                }
                imgTime=ros::Time::now();
                float diff=(imgTime-lastImgTime).toNSec()/1e9;
                if(diff>0.5)
                {
                    ROS_WARN("MVcamera might drop, RECONNECT.");
                    ros::shutdown();
                    // if(!mv_driver->stopped)
                    // {
                    //     mv_driver->Stop();
                    //     mv_driver->Uninit();
                    // }
                    // mv_driver->Init(deviceID);
                    // mv_driver->SetExposureTime(autoexposure_, exposure_);
                    // mv_driver->SetResolution(resolution_idx, height, width, offset_H, offset_W);
                    // mv_driver->SetFPS(fps_mode);
                    // mv_driver->Play();
                }
                lastImgTime=ros::Time::now();
            }
            ros::spinOnce();
            loop_rate.sleep();

        }
        return true;
    }

};

int main(int argc, char **argv)
{
    ros::init(argc,argv,"MVcamera_node");

    MVCamNode mv_node;


    mv_node.spin();
    return EXIT_SUCCESS;


}
