/*GTRI ATAS
All -- Combined nodes
Created 6/27/17
Updated 7/7/17 :: 10:21 AM
*/

#include <ros/ros.h> 
#include <std_msgs/Empty.h>37
#include <geometry_msgs/Twist.h>
#include <ardrone_autonomy/Navdata.h>
#include <unistd.h>

double vx_, vy_, vz_;
int newa;
int tc;
int altitude;
double dcomponents[3] = {1.6, .7, -1.2}; //these are random variables until the tag detection node is finished

//set up the system we use to cmd x,y,z coords to drone (linear & angular)
geometry_msgs::Twist corner1_msg;
geometry_msgs::Twist corner2_msg;
geometry_msgs::Twist corner3_msg;
geometry_msgs::Twist loop1_msg;
geometry_msgs::Twist loop2_msg;
geometry_msgs::Twist loop3_msg;
geometry_msgs::Twist loop4_msg;
geometry_msgs::Twist explore_msg;
std_msgs::Empty takeoff_msg;
std_msgs::Empty land_msg;
geometry_msgs::Twist totag_msg;

void nav_callback(const ardrone_autonomy::Navdata& msg_in)
//this is called when a new message arrives on the nav topic.
	{
	ROS_INFO("Getting sensor reading");
  //Take in state of ardrone
  vx_=msg_in.vx*0.001;
	vy_=msg_in.vy*0.001;
	vz_=msg_in.vz*0.001;
  newa = msg_in.altd;
  tc = msg_in.tags_count;
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

//Function for finding the average altitude of the drone because the ultrasonic sensor is inaccurate 
double averageAltd ()
  { //function that can average the altitude b/c the ultrasonic sensor is booty 
  double sum = 0; //defines sum as altitude (say, 1.2m)
  
  for (double i = 0; i < 10; i++ )
    { //will loop 10 times (if you count 0)
    sum = sum + newa; //adds new altd measurement to existing sum variable
    ROS_INFO("function loop %i", i);
    }
  altitude = sum/10;
  return altitude ; //return altitude (in meters)
	}

int main(int argc, char **argv)
	{

	ROS_INFO("Starting PARROT AR DRONE V1.0");
  ROS_INFO("Starting drone movement node");
	ros::init(argc, argv,"ardrone_movement"); //NOTE: "ardrone_search" is name of node
 	ros::NodeHandle node;
	ros::Rate loop_rate(50);
  
	ROS_INFO("DECLARING FLIGHT VECTORS");
////////////////////////////////////////////////////////////////////////
  /*SEARCH VECTORS*/
////////////////////////////////////////////////////////////////////////
  /*Setup corner1: go forward*/
  	corner1_msg.linear.x=0.333; // forward
  	corner1_msg.linear.y=0.0; // to the side
  	corner1_msg.linear.z=0.0; // up
  	corner1_msg.angular.x=0.0; // not to use
  	corner1_msg.angular.y=0.0; // not to use
  	corner1_msg.angular.z=0.0; //spin
  /*Setup corner2: turn*/
  	corner2_msg.linear.x=0.0; // forward
    corner2_msg.linear.y=0.0; // to the side
    corner2_msg.linear.z=0.0; // up
    corner2_msg.angular.x=0.0; // not to use
    corner2_msg.angular.y=0.0; // not to use
    corner2_msg.angular.z=0.523; //spin
  /*Setup corner3: go to corner*/
  	corner3_msg.linear.x=0.167; // forward
    corner3_msg.linear.y=0.0; // to the side
    corner3_msg.linear.z=0.0; // up
    corner3_msg.angular.x=0.0; // not to use
    corner3_msg.angular.y=0.0; // not to use
    corner3_msg.angular.z=0.0; //spin
  /*Setup loop1: turn*/
  	loop1_msg.linear.x=0.0; // forward
    loop1_msg.linear.y=0.0; // to the side
    loop1_msg.linear.z=0.0; // up
    loop1_msg.angular.x=0.0; // not to use
    loop1_msg.angular.y=0.0; // not to use
    loop1_msg.angular.z=0.743; //spin
  /*Setup loop2: forward*/
  	loop2_msg.linear.x=0.653; // forward
    loop2_msg.linear.y=0.0; // to the side
    loop2_msg.linear.z=0.0; // up
    loop2_msg.angular.x=0.0; // not to use
    loop2_msg.angular.y=0.0; // not to use
    loop2_msg.angular.z=0.0; //spin
  /*Setup loop3: turn*/
  	loop3_msg.linear.x=0.0; // forward
    loop3_msg.linear.y=0.0; // to the side
    loop3_msg.linear.z=0.0; // up
    loop3_msg.angular.x=0.0; // not to use
    loop3_msg.angular.y=0.0; // not to use
    loop3_msg.angular.z=0.743; //spin
  /*Setup loop4: forward*/
  	loop4_msg.linear.x=0.333; // forward
    loop4_msg.linear.y=0.0; // to the side
    loop4_msg.linear.z=0.0; // up
    loop4_msg.angular.x=0.0; // not to use
    loop4_msg.angular.y=0.0; // not to use
    loop4_msg.angular.z=0.0; //spin
  /*Setup explore: forward and turn*/
		explore_msg.linear.x=0.333; // forward
    explore_msg.linear.y=0.0; // to the side
    explore_msg.linear.z=0.0; // up
    explore_msg.angular.x=0.0; // not to use
    explore_msg.angular.y=0.0; // not to use
    explore_msg.angular.z=0.5; //spin
//////////////////////////////////////////////////
  
  	ros::Subscriber nav_sub = node.subscribe("/ardrone/navdata", 1, nav_callback);
  	ros::Publisher pub_twist = node.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
  	ros::Publisher pub_takeoff = node.advertise<std_msgs::Empty>("/ardrone/takeoff", 1); //takeoff -- 10 was 1 
  	ros::Publisher pub_land = node.advertise<std_msgs::Empty>("/ardrone/land", 1);
		//ros::Subscriber pos_sub = node.subscribe("/*TOPIC*/", 1, pos_callback);
  
  	while (ros::ok)
    		{
    		loop_rate.sleep();
    		double time_start = (double)ros::Time::now().toSec();
        while ((double)ros::Time::now().toSec() < time_start+5.0 /*&& altitude < 150*/)
        	{
          ROS_INFO("Taking Off");
          pub_takeoff.publish(takeoff_msg); /* launches the drone */
          /*This is a test to see if you can refer to Navdata variables outside of the callback. Here, state should be 6, taking off*/
          ros::spinOnce();
          loop_rate.sleep();
      		ROS_INFO("Altitude is %i", altitude);
    			}
            
    		ROS_INFO("Beginning flight search pattern");
				while (tc < 1) // if the tag is spotted, will exit this loop
						{
      			ROS_INFO("Searching ...");
      			
      			double turnt = (double)ros::Time::now().toSec(); // Time for turns is set to run for 1 second
       		 	while ((double)ros::Time::now().toSec()< (turnt + 3) && tc < 1)
							{
							ROS_INFO("In turnt 1");
							pub_twist.publish(corner1_msg);
							ros::spinOnce();
        			loop_rate.sleep();
							}
						while ((double)ros::Time::now().toSec()< (turnt + 6) && tc < 1)
							{
							ROS_INFO("In turnt 2");
							pub_twist.publish(corner2_msg);
							ros::spinOnce();
        			loop_rate.sleep();
							}
						while ((double)ros::Time::now().toSec()< (turnt + 9) && tc < 1)
							{
							ROS_INFO("In turnt 3");
							pub_twist.publish(corner3_msg);
              ros::spinOnce();
        			loop_rate.sleep();
							}
						while (tc < 1) // search loop
							{
          		double stime = (double)ros::Time::now().toSec(); // Straights are set to run for 5 sec
            	while ((double)ros::Time::now().toSec()< stime+3.0 && tc < 1)//2
								{
								pub_twist.publish(loop1_msg);
								ros::spinOnce();
        				loop_rate.sleep();
                }
              while ((double)ros::Time::now().toSec()< stime+6.0 && tc < 1)
								{
								pub_twist.publish(loop2_msg);
								ros::spinOnce();
        				loop_rate.sleep();
								}
							while ((double)ros::Time::now().toSec()< stime+9.0 && tc < 1)
								{
								pub_twist.publish(loop3_msg);
								ros::spinOnce();
        				loop_rate.sleep();
								}
							while ((double)ros::Time::now().toSec()< stime+12.0 && tc < 1)
								{
                pub_twist.publish(loop4_msg);
								ros::spinOnce();
        				loop_rate.sleep();
								}
							}
            }
      	ROS_INFO("Tag detected. Approaching");
      
      	double modv = sqrt(((dcomponents[0]*dcomponents[0]) + (dcomponents[1]*dcomponents[1]) + (dcomponents[2]*dcomponents[2])));
    		int num = 0;
      
    		while (modv > 0)
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
        	while ((double)ros::Time::now().toSec()< begint+0.5) // half second run time, can be changed for run legth
        		{
         		//runs the velocities
        		ROS_INFO("Sending velocity message  %1", num);
        		pub_twist.publish(totag_msg);
       			num++;
            
            ros::spinOnce();
        		loop_rate.sleep();
        		}
    
        	ros::spinOnce();
        	loop_rate.sleep();
        	}
  
      	pub_land.publish(land_msg); //lands the drone
    		ros::spinOnce();
        loop_rate.sleep();
      
    		}
  pub_land.publish(land_msg); //lands the drone
  ros::spinOnce();
  loop_rate.sleep();
	}

