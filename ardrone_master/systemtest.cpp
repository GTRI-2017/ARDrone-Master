#include <cstdlib>
#include "ros/ros.h"

int main(int argc, char **argv)
{
	
using namespace std;
  
system( "rosservice  call /ardrone/setledanimation 5 2 5" );
//same prompt as put into the command line
}
