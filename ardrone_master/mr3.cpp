

/********************************************
GTRI ATAS Search flight for AR Tag detection
mr3.cpp
Created: 7/18/17
Updated: 7/24/17 9:51 PM
This file is the Functioned Program
*******************************************/
#include "ardrone_master/master3.h"
double vx_, vy_, vz_, height;
double xgo, ygo, tc, centered;

// callback functions
void pos_callback(const geometry_msgs::Twist& pos_msg)
//this is called when a new message arrives on the /tag_pos topic.
{
ROS_INFO("tc is %f, centered is %f", tc, centered);
ROS_INFO("xgo is %f, ygo is %f", xgo, ygo);
xgo=pos_msg.linear.x; //This tells the drone what direction to move in the x-axis to center itself.
ygo=pos_msg.linear.y; //This tells the drone what direction to move in the y-axis to center itself.
tc=pos_msg.linear.z; //This variable is 0 normally, and switches to a 1 if the vision node detects a tag.
centered=pos_msg.angular.x; //This variable is 0 normally, and switches to a 1 if the vision node detects a tag directly below the drone.
}

void nav_callback(const ardrone_autonomy::Navdata& msg_in)
//this is called when a new message arrives on the navdata topic.
{
vx_=msg_in.vx*0.001;
vy_=msg_in.vy*0.001;
vz_=msg_in.vz*0.001;
height=msg_in.altd;
}

Master::Master()
{
// this constructor gets called once, establishes variables that do not change
// publishers
pub_twist = node1.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
pub_takeoff = node1.advertise<std_msgs::Empty>("/ardrone/takeoff", 1);
pub_land = node1.advertise<std_msgs::Empty>("/ardrone/land", 1);

// subscribers
nav_sub = node1.subscribe("/ardrone/navdata", 1, nav_callback);
pos_sub = node1.subscribe("/ardrone/tag_pos", 1, pos_callback);
xgo=0;
ygo=0;
tc=0;
centered=0;
vx_=0;
vy_=0;
vz_=0;
height=0;

/////*MOVEMENT VECTORS*//////////////////////////////////////////////////////////////////////
/*Setup stop: stop*/
stop_msg.linear.x=0.0; // forward
stop_msg.linear.y=0.0; // to the side
stop_msg.linear.z=0.0; // up
stop_msg.angular.x=0.0; // not to use
stop_msg.angular.y=0.0; // not to use
stop_msg.angular.z=0.0; //spin
/*Setup back: go back*/
back_msg.linear.x=-0.1; // forward
back_msg.linear.y=0.0; // to the side
back_msg.linear.z=0.0; // up
back_msg.angular.x=0.0; // not to use
back_msg.angular.y=0.0; // not to use
back_msg.angular.z=0.0; //spin
/*Setup explore: forward and turn*/
explore_msg.linear.x=0.01; // forward
explore_msg.linear.y=0.0; // to the side
explore_msg.linear.z=0.0; // up
explore_msg.angular.x=0.0; // not to use
explore_msg.angular.y=0.0; // not to use
explore_msg.angular.z=0.7; //spin

} //End constructor

// user-defined functions
void Master::stopf()
//This function momentarily pauses the drone's movement, allowing momentum to be decreased.
{
double stopt = (double)ros::Time::now().toSec();
ROS_INFO("Paused");
while ( (double)ros::Time::now().toSec() < stopt + .3)
  {
  pub_twist.publish(stop_msg);
  }
}

void Master::explore()
//Flies in a spiral search pattern until tag is detected
  {
  ros::Rate loop_rate(50); // 50 H
  while (tc==0)
    {
    ROS_INFO ("Initiating spiral search");
    pub_twist.publish(explore_msg);
    ros::spinOnce();
    loop_rate.sleep();
    }
  }

void Master::center()
{
ROS_INFO("Tag detected. Centering");
int numc = 1;
ros::Rate loop_rate(50); // 50 Hz
double wtime = (double)ros::Time::now().toSec();
while (centered==0 && tc==1 && (double)ros::Time::now().toSec() < wtime+2.0) //to center it
  {
  double ct = .1;
  double cx = .03*xgo;
  double cy = .03*ygo;
  stopf();
  centering_msg.linear.x=cx; // forward
  centering_msg.linear.y=cy; // to the side
  centering_msg.linear.z=0.0; // up
  centering_msg.angular.x=0.0; // not to use
  centering_msg.angular.y=0.0; // not to use
  centering_msg.angular.z=0.0; // spin
  //runs the velocities
  ROS_INFO("Center: Sending velocity message. cx is %f, cy is %f", cx, cy);
  pub_twist.publish(centering_msg);
  ros::spinOnce();
  }
  ros::spinOnce();
  loop_rate.sleep();
  }

void Master::run()
{
ros::Rate loop_rate(50); // 50 Hz
if (ros::ok())
  {
  loop_rate.sleep();
  ROS_INFO("Taking Off");
  double time_start = (double)ros::Time::now().toSec();
  while ((double)ros::Time::now().toSec() < time_start+4.0 )
     {
     pub_takeoff.publish(takeoff_msg); /* launches the drone */
     ros::spinOnce();
     loop_rate.sleep();
     }
  stopf();
  ROS_INFO("Takeoff done");
  ROS_INFO("Beginning flight search pattern");
  while (tc==0)
     {
     explore();
     }
  ROS_INFO("Saw tag");
  double btime = (double)ros::Time::now().toSec();
  while ((double)ros::Time::now().toSec() < btime+2 )
     {
     pub_twist.publish(back_msg);
     ros::spinOnce();
     loop_rate.sleep();
     }
  stopf();
  center();
  ROS_INFO("centered");
  ros::spinOnce();
  loop_rate.sleep();
  pub_land.publish(land_msg);
  }
}

int main(int argc, char **argv)
  {
  // initialize ROS and node
  ROS_INFO("Starting PARROT AR DRONE V1.0");
  ROS_INFO("Starting drone movement node");
  ros::init(argc, argv,"ardrone_movement");
     //NOTE: "ardrone_search" is name of node
  Master class_instance1;
  class_instance1.run();
  ros::spin();
  return 0;
  }


