/*
GTRI ATAS

Flight and loop based on nav data state
*/

#include <ros/ros.h>
#include <std_msgs/Empty.h>
#include <geometry_msgs/Twist.h>
#include <ardrone_autonomy/Navdata.h>

int tags_count = 0;

geometry_msgs::Twist twist_msg;
geometry_msgs::Twist twist_msg_neg;

void nav_callback(const ardrone_autonomy::Navdata& msg_in)
//this is called when a new message arrives on the nav topic.
	{
	//Take in state of ardrone
	vx_=msg_in.vx*0.001; 
	vy_=msg_in.vy*0.001;
	vz_=msg_in.vz*0.001;
	ROS_INFO("Getting sensor reading");
	}

int main(int argc, char **argv)
		{
  	printf("Starting search pattern node");
		ros::init(argc, argv,"ARDrone_pattern"); //NOTE: "ARDrone_search" is name of node
 		ros::NodeHandle node;
		ros::Rate loop_rate(50);
  
  	ros::Publisher pub_twist;
  	ros::Subscriber nav_sub;
  
  	/*Setup move*/
  		twist_msg.linear.x=0.5; // forward
    	twist_msg.linear.y=0.0; // to the side
    	twist_msg.linear.z=0.0; // up
    	twist_msg.angular.x=0.0; // not to use
    	twist_msg.angular.y=0.0; // not to use
    	twist_msg.angular.z=0.0; 
		/*Setup back move*/
    	twist_msg_neg.linear.x=-twist_msg.linear.x; 
    	twist_msg_neg.linear.y=-twist_msg.linear.y;
    	twist_msg_neg.linear.z=-twist_msg.linear.z;
   		twist_msg_neg.angular.x=-twist_msg.angular.x; 
    	twist_msg_neg.angular.y=-twist_msg.angular.y;
    	twist_msg_neg.angular.z=-twist_msg.angular.z;
  
  	nav_sub = node.subscribe("/ardrone/navdata", 1, nav_callback);
  	pub_twist = node.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
  
  	ROS_INFO("Beginning flight pattern");
  	while (ros::ok)
    	{
 
			while (tags_count < 1)
				{
        ROS_INFO("Searching");
        pub_twist.publish(twist_msg);
				ros::spinOnce();
        loop_rate.sleep();
				}
  
    		}
  		ros::spinOnce();
			loop_rate.sleep();
    	}
    }
