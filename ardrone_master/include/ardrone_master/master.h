/********************************************
								GTRI ATAS
    					Header File master.h
							Created: 6/27/17
          Updated: 7/10/17 3:06 PM
********************************************/

#include <ros/ros.h>
#include <ros/ros.h>
#include <std_msgs/Empty.h>
#include <geometry_msgs/Twist.h>
#include <ardrone_autonomy/Navdata.h>
#include <unistd.h>


class Master
	{
	public:
  	ros::NodeHandle node;
    ros::Publisher pub_twist;
    ros::Subscriber nav_sub;
    ros::Publisher pub_takeoff;
    ros::Publisher pub_land;
    //ros::Subscriber pos_sub;
  
		double dcomponents[3]; //these are random variables until the tag detection node is finished; 
  	double lastdcomp[3];
  
  	//set up the system we use to cmd x,y,z coords to drone (linear & angular)
			geometry_msgs::Twist corner1_msg;
			geometry_msgs::Twist corner2_msg;
			geometry_msgs::Twist corner3_msg;
			geometry_msgs::Twist loop1_msg;
			geometry_msgs::Twist loop2_msg;
			geometry_msgs::Twist loop3_msg;
			geometry_msgs::Twist loop4_msg;
			geometry_msgs::Twist explore_msg;
			geometry_msgs::Twist totag_msg;
  		geometry_msgs::Twist gotaway_msg;
  		std_msgs::Empty takeoff_msg;
  		std_msgs::Empty land_msg;

    Master();
  	//void nav_callback(const ardrone_autonomy::Navdata& msg_in);
    double hyp();
    void impdcomp();
    void corner();
    void octagonthing();
    void explore();
    void approach();
    void center();
    void roomba();
    void gotaway();
    void run();
  		
  };
