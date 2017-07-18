
// GTRI ATAS Program for Camera Toggling
//This code send the camera's live feed to the pc on execution



#include <cstdlib>
#include "ros/ros.h"

int main(int argc, char **argv)
{
	
using namespace std;
  
system( "rosrun view_image view_image:/ardrone/image_raw" );
}
