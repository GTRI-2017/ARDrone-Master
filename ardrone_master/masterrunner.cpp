/********************************************
								GTRI ATAS
    Search flight for AR Tag detection
    masterrunner.cpp
							Created: 6/27/17
          Updated: 7/11/17 9:00 AM
This file is the Full Functioned Progam
********************************************/

#include "ardrone_master/master.h"

double vx_, vy_, vz_;
int tc;

// callback functions
/*  
void pos_callback(const ardrone_master:://code//& pos_msg)
  //this is called when a new message arrives on the /tag_pos topic.
  {
	ROS_INFO("Movement node getting position reading");
  //Take in state of ardrone
  dcomponents[0]=pos_msg.vx;
  dcomponents[1]=pos_msg.vy;
  dcomponents[2]=pos_msg.vz;
  }
*/

	void nav_callback(const ardrone_autonomy::Navdata& msg_in)
  //this is called when a new message arrives on the nav topic.
    {
    ROS_INFO("Getting sensor reading");
    //Take in state of ardrone
    vx_=msg_in.vx*0.001;
    vy_=msg_in.vy*0.001;
    vz_=msg_in.vz*0.001;
    tc = msg_in.tags_count; //this will eventually be in the callback function from the other node
  	}

Master::Master()
	{
  // this constructor gets called once, establishes variables that do not change
  // publishers
  	pub_twist = node.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
  	pub_takeoff = node.advertise<std_msgs::Empty>("/ardrone/takeoff", 1);
  	pub_land = node.advertise<std_msgs::Empty>("/ardrone/land", 1);
      
  // subscribers
  	nav_sub = node.subscribe("/ardrone/navdata", 1, nav_callback);
  	//pos_sub = node.subscribe("/tag_pos", 1, &Master::pos_callback);

  //dummy variables
	dcomponents[0] = 1.6;
	dcomponents[1] = .7;
	dcomponents[2] = -1.2;
  
  ///////////////////////////////////////////////////////////////////////
  /*SEARCH VECTORS*/
  /////////////////////////////////////////////////////////////////////
  /*Setup corner1: go forward*/
    corner1_msg.linear.x=0.1667; // forward
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
    corner2_msg.angular.z=0.2615; //spin
  /*Setup corner3: go to corner*/
    corner3_msg.linear.x=0.0835; // forward
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
    loop1_msg.angular.z=0.3715; //spin
  /*Setup loop2: forward*/
    loop2_msg.linear.x=0.3265; // forward
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
    loop3_msg.angular.z=0.3715; //spin
  /*Setup loop4: forward*/
    loop4_msg.linear.x=0.1667; // forward
    loop4_msg.linear.y=0.0; // to the side
    loop4_msg.linear.z=0.0; // up
    loop4_msg.angular.x=0.0; // not to use
    loop4_msg.angular.y=0.0; // not to use
    loop4_msg.angular.z=0.0; //spin
   /*Setup explore: forward and turn*/
    explore_msg.linear.x=0.1667; // forward
    explore_msg.linear.y=0.0; // to the side
    explore_msg.linear.z=0.0; // up
    explore_msg.angular.x=0.0; // not to use
    explore_msg.angular.y=0.0; // not to use
    explore_msg.angular.z=0.25; //spin
  }

// user-defined functions
  double Master::hyp()
    {
    return ( sqrt((lastdcomp[0]*lastdcomp[0]) + (lastdcomp[1]*lastdcomp[1])) );
    }
  
  void Master::impdcomp()
    {
    lastdcomp[0] = dcomponents[0];
    lastdcomp[1] = dcomponents[1];
    lastdcomp[2] = dcomponents[2];
    }
  
  void Master::corner()
    {
    double time_start = (double)ros::Time::now().toSec();
    ROS_INFO("Going to corner");
          
    if (tc < 1) // if the tag is spotted, will exit this loop
      {
      double turnt = (double)ros::Time::now().toSec(); // Time for turns is set to run for 1 second
      while ((double)ros::Time::now().toSec()< (turnt + 1) && tc < 1)
        {
        ROS_INFO("In turnt 1");
               pub_twist.publish(corner1_msg);
 pub_twist.publish(corner1_msg);
        }
      while ((double)ros::Time::now().toSec()< (turnt + 2) && tc < 1)
        {
        ROS_INFO("In turnt 2");
        pub_twist.publish(corner2_msg);
        }
      while ((double)ros::Time::now().toSec()< (turnt + 3) && tc < 1)
        {
        ROS_INFO("In turnt 3");
        pub_twist.publish(corner3_msg);
        }
      }
		}

  void Master::octagonthing()
    {
    ROS_INFO("Octagonal search");
    if (tc < 1)
      {
      double stime = (double)ros::Time::now().toSec(); // Straights are set to run for 5 sec
      while ((double)ros::Time::now().toSec()< stime+1 && tc < 1)//2
        {
        ROS_INFO("In loop 1");
        pub_twist.publish(loop1_msg);
        }
      while ((double)ros::Time::now().toSec()< stime+2 && tc < 1)
        {
        ROS_INFO("In loop 2");
        pub_twist.publish(loop2_msg);
        }
      while ((double)ros::Time::now().toSec()< stime+3 && tc < 1)
        {
        ROS_INFO("In loop 3");
        pub_twist.publish(loop3_msg);
        }
      while ((double)ros::Time::now().toSec()< stime+4 && tc < 1)
        {
        ROS_INFO("In loop 4");
        pub_twist.publish(loop4_msg);
        }
      }
    }

   void Master::explore()
    {
    if (tc = 0)
      {
      ROS_INFO ("Initiating spiral search");
      //calculates velocities for a spiral
      double st=(double)ros::Time::now().toSec();
      int  nume = 0;
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

  void Master::approach()
    {
    if (tc > 0)
      {
      ROS_INFO("Tag detected. Approaching");
      int num = 0;
      double t = 0.1;
      double vz = (1/t)*(dcomponents[2]-2); //this is supposed to make it change its height by whatever it needs to to end 2 meters above the tag.
      
      while (hyp() > 0.1)
        {
            
        //calculates the velocities based on distances and the time we want it to take
          double t = 0.1;
          double vx = (1/t)*dcomponents[0];
          double vy = (1/t)*dcomponents[1];
        	//double vz = (1/t)*dcomponents[2];
    
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
          ROS_INFO("Sending velocity message");
          pub_twist.publish(totag_msg);
          impdcomp();
          num++;
          }
        }
      }
    else
      ROS_INFO("Oops I called approach function but can't find the tag!!!!");
    }
			
  void Master::center()
    {
    impdcomp();
    }
  
  void Master::roomba()
    {
	
    }

  void Master::gotaway()
    {
    	if (tc = 0)
    		{
    		ROS_INFO("Tag got away! Going to last detected location");
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
        while ((double)ros::Time::now().toSec()< begint+0.5) // half second run time, can be changed for run legth
          {
          //runs the velocities
          ROS_INFO("Playing hide-and-go-seek with the tag");
          pub_twist.publish(gotaway_msg);
          }
        }
        else
          ROS_INFO("I thought the tag got away but it didn't");
      }

  void Master::run()
    {
    ros::Rate loop_rate(50); // 50 Hz
    while (ros::ok()) 
      {
      loop_rate.sleep();
      double time_start = (double)ros::Time::now().toSec();
      while ((double)ros::Time::now().toSec() < time_start+5.0 )
        {
        ROS_INFO("Taking Off");
        pub_takeoff.publish(takeoff_msg); /* launches the drone */
        ros::spinOnce();
        loop_rate.sleep();
        ROS_INFO("Takeoff done");
        }
      ROS_INFO("Beginning flight search pattern");
      corner();
      while (tc<1)
      	{
        octagonthing();
      	}
      while (hyp() > 0.1)
        {
        while (tc > 0 && hyp() > 0.1)
          {
          approach();
        	}
        while (tc = 0)
          {
          if (hyp() > 2)
            {
            gotaway();
          	}
          double timer=(double)ros::Time::now().toSec();
          while ((double)ros::Time::now().toSec() < timer + 8)
          	{
            explore();
          	}
          octagonthing(); //roomba();
          }
        }
      //center();
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
