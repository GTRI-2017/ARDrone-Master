/*--------------------------------------------
								GTRI ATAS
    Search flight for AR Tag detection
							Created: 6/27/17
          Updated: 7/10/17 10:57 AM
This file is the Full Functioned Progam
--------------------------------------------*/

#include <ros/ros.h>
#include <ros/ros.h> 
#include <std_msgs/Empty.h>
#include <geometry_msgs/Twist.h>
#include <ardrone_autonomy/Navdata.h>
#include <unistd.h>



//using std::cout;/////////////////////////////
//using std::endl;/////////////////////////////

class Master
	{
	public:
  	ros::NodeHandle node;
    ros::Publisher pub_approach;
    ros::Subscriber nav_sub;
    ros::Publisher pub_takeoff;
    ros::Publisher pub_land;
    //ros::Subscriber pos_sub;
  
  	double vx_, vy_, vz_;
		int newa;
		int tc;
		int altitude;
		double dcomponents[3] = {1.6, .7, -1.2}; //these are random variables until the tag detection node is finished; 
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
  
		void Master()
	  	{
    	// this constructor gets called once, establishes variables that do not change
    	// publishers
    		pub_approach = node.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
    		pub_takeoff = node.advertise<std_msgs::Empty>("/ardrone/takeoff", 1);
				pub_land = node.advertise<std_msgs::Empty>("/ardrone/land", 1);
      
   	 	// subscribers
    		nav_sub = node.subscribe("/ardrone/navdata", 1, &Master::nav_callback);
    		//pos_sub = node.subscribe("/*TOPIC*/", 1, &Master::pos_callback);
    	
      std_msgs::Empty takeoff_msg;
			std_msgs::Empty land_msg;
      ////////////////////////////////////////////////////////////////////////
    	/*SEARCH VECTORS*/
			/////////////////////////////////////////////////////////////////////
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
	  	}

    // callback functions
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

    // user-defined functions
      double hyp()
      	{
        return ( sqrt((lastdcomp[0]*lastdcomp[0]) + (lastdcomp[1]*lastdcomp[1])) );
      	}
  
  		void impdcomp()
      	{
        lastdcomp[0] = dcomponents[0];
        lastdcomp[1] = dcomponents[1];
        lastdcomp[2] = dcomponents[2];
      	}
  
  		void corner()
				{
        double time_start = (double)ros::Time::now().toSec();
        ROS_INFO("Going to corner");
          
        if (tc < 1) // if the tag is spotted, will exit this loop
          {
          double turnt = (double)ros::Time::now().toSec(); // Time for turns is set to run for 1 second
          while ((double)ros::Time::now().toSec()< (turnt + 3) && tc < 1)
            {
            ROS_INFO("In turnt 1");
            pub_twist.publish(corner1_msg);
            }
          while ((double)ros::Time::now().toSec()< (turnt + 6) && tc < 1)
            {
            ROS_INFO("In turnt 2");
            pub_twist.publish(corner2_msg);
            }
          while ((double)ros::Time::now().toSec()< (turnt + 9) && tc < 1)
            {
            ROS_INFO("In turnt 3");
            pub_twist.publish(corner3_msg);
            }
					}
				}

      void octagonthing()
				{
        ROS_INFO("Octagonal search");
        if (tc < 1)
          {
          double stime = (double)ros::Time::now().toSec(); // Straights are set to run for 5 sec
          while ((double)ros::Time::now().toSec()< stime+3.0 && tc < 1)//2
            {
            ROS_INFO("In loop 1");
            pub_twist.publish(loop1_msg);
            }
          while ((double)ros::Time::now().toSec()< stime+6.0 && tc < 1)
            {
            ROS_INFO("In loop 2");
            pub_twist.publish(loop2_msg);
            }
          while ((double)ros::Time::now().toSec()< stime+9.0 && tc < 1)
            {
            ROS_INFO("In loop 3");
            pub_twist.publish(loop3_msg);
            }
          while ((double)ros::Time::now().toSec()< stime+12.0 && tc < 1)
            {
            ROS_INFO("In loop 4");
            pub_twist.publish(loop4_msg);
          	}
          }
        }

    	void explore()
				{
  			if (tc < 1)
      		{
    			double time_start = (double)ros::Time::now().toSec();
					while (tc < 1 && ( (double)ros::Time::now().toSec() < (time_start+8) ) ) // theoretically, 4 loops
          	{
						pub_twist.publish(explore_msg);
            }
					}
				}

			void approach()
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
       				num++;
              
        			}
        		}
        	}
    		else
      		ROS_INFO("Oops I called approach function but can't find the tag!!!!");
				}
			
  		void center()
      	{
        impdcomp();
      	}
  
	
			void roomba()
				{
	
        }
  
    	void run()
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
          	octagonthing();
          while (hyp() > 0.1)
          	{
            	while (tc > 0 && hyp() > 0.1)
              	{
          			approach();
              	}
          		while (tc < 1)
          			{
            		explore();
            		octagonthing(); //roomba();
          			}
          	}
          //center();
          pub_land.publish(land_msg); //lands the drone
	       	}
        }

	//private:
    
  
	};

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
