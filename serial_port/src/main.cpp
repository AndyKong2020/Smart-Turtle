//
// Created by bismarck on 12/8/22.
//

#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>

#include "serial_port/serialPort.h"
#include "serial_port/text.h"

serialPort serialHandle("/dev/ttyUSB0");

message_data Text {
    int x,y,num;
    char label;
    float confidence;
};

void textCallback(const serial_port::textConstPtr &msg){
    Text text{(int)msg->x,(int)msg->y,(int)msg->num,(char)msg->label,(float)msg->confidence} ;
    serialHandle.sendMsg(0x13, text);
}


void loopBack(Text msg) {
    ROS_INFO("x: %d, y: %d, num: %d, label: %c, confidence: %f", msg.x, msg.y, msg.num, msg.label, msg.confidence);
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "serial_port");
    ros::NodeHandle nh;
    ros::Subscriber pose_sub = nh.subscribe("/text", 1, textCallback);
    serialHandle.registerCallback<Text>(0x13, &loopBack);

    ros::spin();
}
