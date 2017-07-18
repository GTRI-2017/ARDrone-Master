/********************************************
								GTRI ATAS
    Search flight for AR Tag detection
            masterrunner2.cpp
							Created: 6/27/17
          Updated: 7/17/17 1:10 PM
This file is the Full Functioned Progam
********************************************/

/*
How to run

roscore
get driver up
rosrun ardrone_master masterrruner2
while in masterrunner2 is running: rosrun ardrone_master pub_test
should tell it there is a tag and thus change to center which should just fly in a new direction forever
*/

#include "ardrone_master/master.h"

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
/*Setup corner4: go to corner*/
    corner4_msg.linear.x=0.0; // forward
    corner4_msg.linear.y=0.0; // to the side
    corner4_msg.linear.z=0.0; // up
    corner4_msg.angular.x=0.0; // not to use
    corner4_msg.angular.y=0.0; // not to use
    corner4_msg.angular.z=0.523; //spin
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
		while ((double)ros::Time::now().toSec()< stopt + .4) //.75 sec
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
      while ((double)ros::Time::now().toSec()< (turnt + 1.6) && tc < 1) //.75 sec
        {
        pub_twist.publish(corner1_msg);

        }
	ROS_INFO("In turnt 2");
	stopf();
      while ((double)ros::Time::now().toSec()< (turnt + 3.45) && tc < 1) //.75
        {
        pub_twist.publish(corner2_msg);

        }
	ROS_INFO("In turnt 3");
	stopf();
      while ((double)ros::Time::now().toSec()< (turnt + 3.85) && tc < 1) //.5
        {
        pub_twist.publish(corner3_msg);

        }
	ROS_INFO("In turnt 4");
	stopf();
      while ((double)ros::Time::now().toSec()< (turnt + 3.85) && tc < 1) //.5
        {
        pub_twist.publish(corner4_msg);

        }
      }
		}

  void Master::octagonthing()
    {
    ROS_INFO("Octagonal search");
    if (tc < 1)
      {
      double stime = (double)ros::Time::now().toSec(); // Straights are set to run for 5 sec
	ROS_INFO("In loop 1");
	stopf();
      while ((double)ros::Time::now().toSec()< stime+2 && tc < 1)//2 //.75
        {
        pub_twist.publish(loop1_msg);
        }
        ROS_INFO("In loop 2");
	stopf();
      while ((double)ros::Time::now().toSec()< stime+2.75 && tc < 1)
        {
        pub_twist.publish(loop2_msg);
        }
        ROS_INFO("In loop 3");
	stopf();
      while ((double)ros::Time::now().toSec()< stime+4.75 && tc < 1)
        {
        pub_twist.publish(loop3_msg);
        }
        ROS_INFO("In loop 4");
	stopf();
      while ((double)ros::Time::now().toSec()< stime+5.5 && tc < 1)
        {
        pub_twist.publish(loop4_msg);
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
    else ROS_INFO ("Spiral search unnecessary");
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
        /*
      double lt=(double)ros::Time::now().toSec();
      while ((double)ros::Time::now().toSec() < lt+0.2)
        {*/
        //runs the velocities
        ROS_INFO("Sending velocity message %i", numl);
        pub_twist.publish(down_msg);
      ros::spinOnce();
      numl++;
        //}
      center();
      }
  	}

  void Master::roomba()
    {
	
    }

  /*void Master::gotaway()
    {
    	if (tc = 0)
    		{
    		ROS_INFO("Tag lost. Going to last detected location");
      	//calculates the velocities based on distances and the time we want it to take
        	double t = 0.1;
          double vx = (1/t)*lastdcomp[0];
          double vy = (1/t)*lastdcomp[1];
    
        //imports velocities into the message to send
        	gotaway_msg.linear.x=vx; // forward
          gotaway_msg.linear.y=vy; // to the side
          gotaway_msg.linear.z=0.0; // up
          gotaway_msg.angular.x=0.0; // not to use
          gotaway_msg.angular.y=0.0; // not to use
          gotaway_msg.angular.z=0.0; // spin
    
        double begint=(double)ros::Time::now().toSec();
        while ((double)ros::Time::now().toSec()< begint+0.5) // half second run time, can be changed for run length
          {
          //runs the velocities
          pub_twist.publish(gotaway_msg);
          }
        }
        else
          ROS_INFO("Tag not lost");
      }*/

  void Master::run()
    {
    ros::Rate loop_rate(50); // 50 Hz
    while (ros::ok())
      {
      loop_rate.sleep();
	ROS_INFO("Taking Off");
      double time_start = (double)ros::Time::now().toSec();
      while ((double)ros::Time::now().toSec() < time_start+5.0 )
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
			while (height > .3)//number
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
          //pub_land.publish(land_msg); //lands the drone
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
	ros::init(argc, argv,"ardrone_movement"); //NOTE: "ardrone_movement" is name of node
  Master class_instance1;
  class_instance1.run();
  ros::spin();
  return 0;
	}
