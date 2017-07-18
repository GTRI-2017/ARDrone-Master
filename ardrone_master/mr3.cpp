/********************************************
								GTRI ATAS
    Search flight for AR Tag detection
            masterrunner2.cpp
							Created: 7/18/17
          Updated: 7/18/17 11:30 AM
This file is the Full Functioned Progam
********************************************/

#include "ardrone_master/master3.h"

double vx_, vy_, vz_, height;
double xgo, ygo, tc, centered;

// callback functions

void pos_callback(const geometry_msgs::Twist& pos_msg)
//this is called when a new message arrives on the /tag_pos topic.
  {
	ROS_INFO("Movement node getting position reading");
  //Take in state of ardrone
  xgo=pos_msg.linear.x;
  ygo=pos_msg.linear.y;
  tc=pos_msg.linear.z;
  centered=pos_msg.angular.x;
  }

void nav_callback(const ardrone_autonomy::Navdata& msg_in)
//this is called when a new message arrives on the navdata topic.
  {
  ROS_INFO("Getting sensor reading");
  //Take in state of ardrone
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
  	//pos_sub = node1.subscribe("/ardrone/tag_pos", 1, &Master::pos_callback);
  
  
  ///////////////////////////////////////////////////////////////////////
  /*SEARCH VECTORS*/
  /////////////////////////////////////////////////////////////////////
  /*Setup stop: stop*/
    stop_msg.linear.x=0.0; // forward
    stop_msg.linear.y=0.0; // to the side
    stop_msg.linear.z=0.0; // up
    stop_msg.angular.x=0.0; // not to use
  	stop_msg.angular.y=0.0; // not to use
    stop_msg.angular.z=0.0; //spin
  /*Setup forward: go forward*/
    forward_msg.linear.x=0.2; // forward
    forward_msg.linear.y=0.0; // to the side
    forward_msg.linear.z=0.0; // up
    forward_msg.angular.x=0.0; // not to use
  	forward_msg.angular.y=0.0; // not to use
    forward_msg.angular.z=0.0; //spin
  /*Setup turn: turn*/
    turn_msg.linear.x=0.0; // forward
    turn_msg.linear.y=0.0; // to the side
    turn_msg.linear.z=0.0; // up
    turn_msg.angular.x=0.0; // not to use
    turn_msg.angular.y=0.0; // not to use
    turn_msg.angular.z=0.7; //spin
   /*Setup explore: forward and turn*/
    explore_msg.linear.x=0.333; // forward
    explore_msg.linear.y=0.0; // to the side
    explore_msg.linear.z=0.0; // up
    explore_msg.angular.x=0.0; // not to use
    explore_msg.angular.y=0.0; // not to use
    explore_msg.angular.z=0.5; //spin
  /*Setup lower: go down*/
    down_msg.linear.x=0.0; // forward
    down_msg.linear.y=0.0; // to the side
    down_msg.linear.z=-0.1; // up
    down_msg.angular.x=0.0; // not to use
    down_msg.angular.y=0.0; // not to use
    down_msg.angular.z=0.0; // spin
  }

// user-defined functions
	void Master::stopf()
  	{
    double stopt = (double)ros::Time::now().toSec();
    ROS_INFO("Paused");
    while ( (double)ros::Time::now().toSec() < stopt + .5) //.5
    		{
			pub_twist.publish(stop_msg);
    		}
  	}

  void Master::corner()
    {
    double time_start = (double)ros::Time::now().toSec();
    ROS_INFO("Going to corner");
		ROS_INFO("tc is %d", tc);
    if (tc == 0) // if the tag is spotted, will exit this loop
      {
      double turnt = (double)ros::Time::now().toSec(); // Time for turns is set to run for 1 second
      ROS_INFO("In turnt 1");
      while ((double)ros::Time::now().toSec()< (turnt + 2.5) && tc < 1)  //15
        {
        pub_twist.publish(forward_msg);
        }
      stopf();
      ROS_INFO("In turnt 2");
      while ((double)ros::Time::now().toSec()< (turnt + 4.9) && tc < 1)  //3.1
        {
        pub_twist.publish(turn_msg);
        }
      stopf();
      ROS_INFO("In turnt 3");
      while ((double)ros::Time::now().toSec()< (turnt + 5.05) && tc < 1)  //15
        {
        pub_twist.publish(forward_msg);
        }
      stopf();
      }
		}

  void Master::octagonthing()
    {
    ROS_INFO("Octagonal search");
    if (tc < 1)
      {
      double stime = (double)ros::Time::now().toSec(); // Straights are set to run for 5 sec
      ROS_INFO("In loop 1");
      while ((double)ros::Time::now().toSec()< stime+2.4 && tc < 1)  //12.3
        {
        pub_twist.publish(turn_msg);
        }
      stopf();
      ROS_INFO("In loop 2");
      while ((double)ros::Time::now().toSec()< stime+4.4 && tc < 1)  //22.3
        {
        pub_twist.publish(forward_msg);
        }
      stopf();
      ROS_INFO("In loop 3");
      while ((double)ros::Time::now().toSec()< stime+6.8 && tc < 1)  //12.3
        {
        pub_twist.publish(turn_msg);
        }
      stopf();
      ROS_INFO("In loop 4");
      while ((double)ros::Time::now().toSec()< stime+8.8 && tc < 1)  //22.3
        {
        pub_twist.publish(forward_msg);
        }
      stopf();
      }
    }

   void Master::explore()
    {
    if (tc == 0)
      {
      ROS_INFO ("Initiating spiral search");
      //calculates velocities for a spiral
      double st=(double)ros::Time::now().toSec();
      int nume = 0;
      double b = (1/18.85); //spacing of spiral
      double sx = b*(st*st*sin(st));
      double sy = b*-1*(st*st*cos(st));
      
      double spiralt=(double)ros::Time::now().toSec();
      while ((double)ros::Time::now().toSec() < spiralt+0.2)
        {
        //runs the velocities
        ROS_INFO("Sending velocity message %i", nume);
        pub_twist.publish(explore_msg);
        nume++;
        }
      }
    else ROS_INFO ("Tried to spiral search but I still see the tag");
    }

  void Master::center()
    {
      ROS_INFO("Tag detected. Keeping center");
      int numc = 1;
      while (centered==0) //to center it
        {
        double ct = .1;
        double cx = (1/ct)*xgo;
        double cy = (1/ct)*ygo;
        
       	//centers over tag
    			centering_msg.linear.x=cx; // forward
    			centering_msg.linear.y=cy; // to the side
    			centering_msg.linear.z=0.0; // up
    			centering_msg.angular.x=0.0; // not to use
    			centering_msg.angular.y=0.0; // not to use
    			centering_msg.angular.z=0.0; // spin
        
        //runs the velocities
          ROS_INFO("Sending velocity message");
          pub_twist.publish(centering_msg);
        	ros::spinOnce();
          numc++;
        }
    }

	void Master::lower()	 //to lower it
  	{
    int numl=0;
    while (height > 0.3)
      {
      ROS_INFO("Sending velocity message %i", numl);
      pub_twist.publish(down_msg);
      ros::spinOnce();
      numl++;
      center();
      }
  	}

  void Master::roomba()
    {
	
    }

  void Master::run()
    {
    ros::Rate loop_rate(50); // 50 Hz
    while (ros::ok())
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

      corner();

      while (tc==0)
        {
        octagonthing();
        }
			while (height > .3) //number
      	{
      	while (centered==0)
      		{
      			while (tc > 0)
       				{
              center();
        			}
      		while (tc == 0)
          	{
          	double timer=(double)ros::Time::now().toSec();
         		while ((double)ros::Time::now().toSec() < timer + 8)
          		{
            	explore();
          		}
          	//roomba();
         		}
      		}
      	if ( !(centered==0) )
      		{
        	lower();
      		}
      	}
      pub_land.publish(land_msg); //lands the drone
      }
    }

// -------------------------------------

int main(int argc, char **argv)
	{
  // initialize ROS and node
  ROS_INFO("Starting PARROT AR DRONE V1.0");
  ROS_INFO("Starting drone movement node");
	ros::init(argc, argv,"ardrone_movement"); //NOTE: "ardrone_search" is name of node
  Master class_instance1;
  class_instance1.run();
  ros::spin();
  return 0;
	}
