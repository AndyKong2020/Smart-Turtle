/*************************************************************************
  > File Name: video_pub.cpp
  > Author: cyz
  > Mail:
  > Created Time: Sun 24 Feb 2019 05:08:34 PM
  >it situmalated a video file to normal video capture, thus you can use it to debug
 ************************************************************************/


#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <sstream>
#include <iostream>
#include "opencv2/opencv.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <std_msgs/Bool.h>
//#include "preprocess.h"

using namespace std;
using namespace cv;
Size large_size = Size(640, 512);
Size small_size = Size(640, 480);
//Size dist_size=large_size;
Size dist_size=small_size;
string video_source = "/home/andykong/Downloads/qq-files/243604572/file_recv/lane_test.mp4";
Mat img_show;



///
/// \brief get_is_large
/// if cam is 1280x1024, we resize it to 640 x512
/// \param is_large_resolution
///
void get_is_large(const std_msgs::BoolConstPtr &is_large_resolution)
{
  if(is_large_resolution->data==true)
  {
    dist_size=large_size;
  }else
  {
    dist_size=small_size;
  }
}

int main(int argc, char **argv)
{
  ros::init(argc,argv,"video_publisher");
  ros::NodeHandle nh;
  image_transport::ImageTransport it(nh);// useful when publish imgs
  image_transport::Publisher pub = it.advertise("camera/image_raw", 1);
  ros::Subscriber is_large_sub=nh.subscribe("mv_param/is_large",1,get_is_large);

  nh.getParam("/video_source",video_source);

  ros::Rate loop_rate(30);
  //cuda_proc processor(1024,1280);
  VideoCapture cap(video_source);//open video in the path
  if(!cap.isOpened())
  {
    std::cout<<"open video failed!"<<std::endl;
    return -1;
  }
  else
    std::cout<<"open video success!"<<std::endl;

  Mat frame,img_show;
  bool isSuccess = true;

  while(nh.ok())
  {
    isSuccess = cap.read(frame);
    if(!isSuccess)//if the video ends, then break
    {
      std::cout<<"video ends"<<std::endl;
      break;
    }
    /// preprocess: cuda version


    //        processor.proc_update(frame);
    //        if(!processor.compImg.empty())
    //            img_show=processor.compImg;
    //        else
    //            img_show=frame;


    ///preprocess: opencv version
    // Size src_size=Size(frame.cols,frame.rows);
    // if(src_size!=dist_size)   // resize to 640x512
    //   resize(frame, frame, dist_size);



    //        sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "mono8", img_show).toImageMsg();
    std_msgs::Header header;
    header.stamp=ros::Time::now();
    sensor_msgs::ImagePtr msg = cv_bridge::CvImage(header, "bgr8", frame).toImageMsg();
    pub.publish(msg);

    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}
