/*
GTRI ATAS
Search and Chase Tag
Created 6/27/17
Updated 6/28/17 :: 8:41 am
*/

#include <ros/ros.h> 
#include <std_msgs/Empty.h>
#include <geometry_msgs/Twist.h>
#include <ardrone_autonomy/Navdata.h>
#include <math.h> 

//double altitude = 0;
double tags_count = 0; //variable set since tags_count by itself is probably not the proper calling card
double vx_, vy_, vz_;

//set up the system we use to cmd x,y,z coords to drone (linear & angular)
geometry_msgs::Twist setup_msg;
geometry_msgs::Twist takeoff_msg;e
geometry_msgs::Twist setup_msg_neg;
std_msgs::Empty emp_msg;

void nav_callback(const ardrone_autonomy::Navdata& msg_in)
//this is called when a new message arrives on the nav topic.
	{
	ROS_INFO("Getting sensor reading");
  //Take in state of ardron
  vx_=msg_in.vx*0.001;
	vy_=msg_in.vy*0.001;
	vz_=msg_in.vz*0.001;
  int altitude = altd;
	}


int main(int argc, char **argv)
		{
  	printf("Starting search pattern node");
		ros::init(argc, argv,"ardrone_search"); //NOTE: "ardrone_search" is name of node
 		ros::NodeHandle node;
		ros::Rate loop_rate(50);
  
  	ros::Publisher pub_twist; // says twist may have to change to setup and takeoff specifically
  	ros::Subscriber nav_sub;
  
  	//Power for takeoff
  	/*Setup move*/
  		takeoff_msg.linear.x=0.0; // forward
    	takeoff_msg.linear.y=0.0; // to the side
    	takeoff_msg.linear.z=0.05; // up
  
    	takeoff_msg.angular.x=0.0; // not to use
    	takeoff_msg.angular.y=0.0; // not to use
    	takeoff_msg.angular.z=0.0; //spin
  
  	// Power for Search
  	/*Setup move*/
  		setup_msg.linear.x=0.5; // forward
    	setup_msg.linear.y=0.0; // to the side
    	setup_msg.linear.z=0.0; // up
  
    	setup_msg.angular.x=0.0; // not to use
    	setup_msg.angular.y=0.0; // not to use
    	setup_msg.angular.z=0.0; //spin
  
		/*Setup back move*/
    	setup_msg_neg.linear.x=-setup_msg.linear.x; 
    	setup_msg_neg.linear.y=-setup_msg.linear.y;
    	setup_msg_neg.linear.z=-setup_msg.linear.z;
   		setup_msg_neg.angular.x=-setup_msg.angular.x; 
    	setup_msg_neg.angular.y=-setup_msg.angular.y;
    	setup_msg_neg.angular.z=-setup_msg.angular.z;
  
  	nav_sub = node.subscribe("/ardrone/navdata", 1, nav_callback);
  	pub_twist = node.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
  
  	while (ros::ok )  
    	{
      	while (altitude < 150) // while for takeoff (should be 150, 50 for testing)
      		{
          	ROS_INFO("Taking Off");
  					pub_twist.publish(takeoff_msg);// Actual Flight Command
          	//altitude++;
          	ros::spinOnce();
          	loop_rate.sleep();
      		}
      
    		ROS_INFO("Beginning flight pattern");
 			
				while (tags_count < 1 && ros::ok) // while for 
					{
      			ROS_INFO("Searching");
       		 	pub_twist.publish(setup_msg);
       		 	//NOTE: In current state, the drone will continue to fly up and forward until node is manually ended by user.
						ros::spinOnce();
        		loop_rate.sleep();
          }

  			ros::spinOnce();
				loop_rate.sleep();
    	}
    }
