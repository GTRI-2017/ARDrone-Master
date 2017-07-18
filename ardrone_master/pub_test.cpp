/********************************************
GTRI ATAS
pubtest.cpp
Created: 7/12/17
Updated: 7/12/17 11:56 AM
********************************************/

#include <ros/ros.h>
#include <std_msgs/Empty.h>
#include <geometry_msgs/Twist.h>
#include <unistd.h>

geometry_msgs::Twist pos_msg;


int main(int argc, char **argv)
	{
	// initialize ROS and node
	ROS_INFO("Starting PARROT AR DRONE V1.0");
	ROS_INFO("Starting drone publisher node");
	ros::init(argc, argv,"ardrone_publisher"); //NOTE: "ardrone_publisher" is name of node
	ros::NodeHandle node;
	ros::Rate loop_rate(50); // 50 Hz

	ros::Publisher pub_pos = node.advertise<geometry_msgs::Twist>("/ardrone/tag_pos", 1);

	pos_msg.linear.x=.5; // left or right movement
	pos_msg.linear.y=.1; // forward or back movement
	pos_msg.linear.z=1; // tag seen, 0 or 1 //0 is false and 1 true
	pos_msg.angular.x=0.0; // can land?, 0 or 1 met if tag is within boundary 
	pos_msg.angular.y=0.0; // not to use
	pos_msg.angular.z=0.0; // not to use

	while (ros::ok()) 
		{
		pub_pos.publish(pos_msg);
		ros::spinOnce();
		loop_rate.sleep();
		}
	ros::spin();
	return 0;
	}
