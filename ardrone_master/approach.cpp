/*
GTRI ATAS
Trajectory Calc
Created 7/5/17 :: 8:05 AM
Updated 7/7/17 :: 10:46 AM
*/

/*
The purpose of this code is to take information about the direction and distance of the tag from the camera and calculate the
velocity and time the drone needs to go. After that, this node will publish this information on a topic to be picked up by the movement node
and sent to the drone.
*/

//https://codeshare.io/G6AVAk
//https://codeshare.io/21qJEj


#include <ros/ros.h> 
#include <std_msgs/Empty.h>
#include <geometry_msgs/Twist.h>
#include <ardrone_autonomy/Navdata.h>
#include <unistd.h>
#include <math.h>

std_msgs::Empty emp_msg;
geometry_msgs::Twist totag_msg;

double dcomponents[3] = {1.5, 1.5, -3};

void nav_callback(const ardrone_autonomy::Navdata& msg_in)
//this is called when a new message arrives on the nav topic.
	{
  ROS_INFO("tagmath getting sensor reading");
  //Take in state of ardrone
  }
/*
void pos_callback(const ardrone_autonomy::TOP& msg_in)
//this is called when a new message arrives on the TOPIC topic.
    {
    ROS_INFO("tagmath getting position reading");
      //Take in state of ardrone
    dcomponents=msg_in.name;
    }
*/
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
    
        //calculates the velocities based on distances
        double t = 0.1;
      	double vx = (1/t)*dcomponents[0];
        double vy = (1/t)*dcomponents[1];
      	double vz = (1/t)*dcomponents[2];
    
        //imports velocities into the message to send
        totag_msg.linear.x=vx; // forward
        totag_msg.linear.y=vy; // to the side
        totag_msg.linear.z=vz; // up
        totag_msg.angular.x=0.0; // not to use
        totag_msg.angular.y=0.0; // not to use
        totag_msg.angular.z=0.0; // spin
    
      	double begint=(double)ros::Time::now().toSec();
        while ((double)ros::Time::now().toSec()< begint+0.5)
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
  
    pub_land.publish(emp_msg); //lands the droe
    ros::spinOnce();
    loop_rate.sleep();
    }
