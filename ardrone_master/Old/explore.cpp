/*
GTRI ATAS
Explore
Created 7/7/17 :: 10:40 AM
Updated 7/7/17 :: 10:46 AM
*/

#include <ros/ros.h> 
#include <std_msgs/Empty.h>
#include <geometry_msgs/Twist.h>
#include <ardrone_autonomy/Navdata.h>
#include <unistd.h>
#include <math.h>

std_msgs::Empty emp_msg;
geometry_msgs::Twist explore_msg;

int main(int argc, char **argv)
    {

    ROS_INFO("Starting PARROT AR DRONE V1.0");
    ROS_INFO("Starting tag approach math node");
    ros::init(argc, argv,"ardrone_tagmath"); //NOTE: "ardrone_tagmath" is name of node
    ros::NodeHandle node;
    ros::Rate loop_rate(50);
 
    ros::Publisher pub_approach = node.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
    ros::Subscriber nav_sub = node.subscribe("/ardrone/navdata", 1, nav_callback);
		ros::Publisher pub_land = node.advertise<std_msgs::Empty>("/ardrone/land", 1);
    //ros::Subscriber pos_sub = node.subscribe("/*TOPIC*/", 1, pos_callback);

  
    // will take x, y, and z and find the magnitude
    double hyp = sqrt(((dcomponents[0]*dcomponents[0]) + (dcomponents[1]*dcomponents[1])));
    int  num = 0;
  
    while (ros::ok && hyp > 0)
        {
      	double spiralt=(double)ros::Time::now().toSec();
        while ((double)ros::Time::now().toSec()< spiralt+0.5)
        	{
         	//runs the velocities
        	ROS_INFO("Sending velocity message");
        	pub_approach.publish(totag_msg);
       		num++;
          ros::spinOnce();
          loop_rate.sleep();
        	}
    
        ros::spinOnce();
        loop_rate.sleep();
        }
