#include <ros/ros.h>
// header files


using std::cout;
using std::endl;

class ClassName
{
public:
     ClassName()
	  {
	       // this constructor gets called once

	       // publish to topics of relevance
	       pub_ = n_.advertise<geometry_msgs::Twist>("cmd_vel", 1);

	       // subscribers
	       sub_ = n_.subscribe("topic_name", 1,
				   &ClassName::cb_fun, this);
	  }

     // callback functions
     void cb_fun()
	  {}

     // user-defined functions

     void run()
     	  {
	       ros::Rate loop_rate(10); // 10 Hz

	       while (ros::ok()) {

		    // main code

		    ros::spinOnce();
		    loop_rate.sleep();
	       }
     	  }

private:
     ros::NodeHandle n_;
     ros::Publisher pub_;
     ros::Subcriber sub_;
     // global variables
};

// -------------------------------------

int main(int argc, char **argv)
{
     // initialize ROS and node
     ros::init(argc, argv, "node_name");
     ClassName class_instance;
     class_instance.run();
     ros::spin();
     return 0;
}
