//
// Created by stevegao on 22-12-24.
//

#include <iostream>
#include <thread>
#include <ros/ros.h>
#include "rc_msgs/identify_results.h"
#include <string>

void identifyResultsCallback(const rc_msgs::identify_results::ConstPtr &msg) {
    rc_msgs::single_result analyzation;
    analyzation.result = "None";
    analyzation.score = 0.0;
    analyzation.x_coordinate = 0.0;
    analyzation.y_coordinate = 0.0;
    rc_msgs::identify_results identifyResults;
    identifyResults.identify_results = msg->identify_results;
    unsigned long length = identifyResults.identify_results.max_size();
    std::cout << "length: " << length << std::endl;
    for (int i = 0; i < length; i++) {
        analyzation.result = identifyResults.identify_results[i].result;
        if (i == 0) { std::cout << length << std::endl; }
        std::cout << "result: " << analyzation.result << std::endl;
        analyzation.score = identifyResults.identify_results[i].score;
        std::cout << "score: " << analyzation.score << std::endl;
        analyzation.x_coordinate = identifyResults.identify_results[i].x_coordinate;
        std::cout << "x_coordinate: " << analyzation.x_coordinate << std::endl;
        analyzation.y_coordinate = identifyResults.identify_results[i].y_coordinate;
        std::cout << "y_coordinate: " << analyzation.y_coordinate << std::endl;
    }
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "rc_msgs_analyzation");
    ros::NodeHandle n;
    ros::Subscriber resultsSub = n.subscribe("/identify_results", 1, &identifyResultsCallback);

    ros::Rate loop_rate(10);
    while (ros::ok()) {
        ros::spinOnce();
        loop_rate.sleep();
    }
}