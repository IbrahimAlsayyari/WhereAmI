#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>


// Define a global client that can request services
ros::ServiceClient client;

// This func tion calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    // TODO: Request a service and pass the velocities to it to drive the robot
    //std::vector<float>  velo = velocity_control(lin_x, ang_z);
  //ros::NodeHandle n;	
  //ros::ServiceServer service = n.advertiseService(client, velocity_control(lin_x, ang_z));
    ROS_INFO_STREAM("Driving the robot");

    // Request centered joint angles [1.57, 1.57]
    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = lin_x;
    srv.request.angular_z = ang_z;

    // Call the safe_move service and pass the requested joint angles
    if (!client.call(srv))
        ROS_ERROR("Failed to call service drive_bot");

}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{
  
    // TODO: Loop through each pixel in the image and check if there's a bright white one
    // Then, identify if this pixel falls in the left, mid, or right side of the image
    // Depending on the white ball position, call the drive_bot function and pass velocities to it
    // Request a stop when there's no white ball seen by the camera
    int white_pixel = 255;
    int x = -1;
  // 1  2  3  4  5  6
  // 7  8  9  10 11 12
  // 13 14 15 16 17 18
   //19 20 21 22 23 24
  
  
    // Loop through each pixel in the image and check if its equal to the first one
    for (int i = 0; i < img.height * img.step; i++) {
        if (img.data[i] == white_pixel) {
             ROS_INFO_STREAM("white_pixel detected i = "<< i);
          if ( i >= img.step){
           
            while (i >= img.step)
            i = i - img.step;
            x = i;

          }
                     
           ROS_INFO_STREAM(" x = " << x);
          if ( -1 <x &&  x < img.step/3 ){
             // drive left
            drive_robot(0,0.5); break;
          }
          else if ( img.step/3 <=x && x < 2*img.step/3){
            // drive forward
            drive_robot(1,0); break;
        }
          else if (x >= 2*img.step/3){
              // drive right
           drive_robot(0,-0.5); break;
    	}           
               
        }
       if(img.data[i] != white_pixel && i ==  img.height * img.step-1 && x== -1)
         	drive_robot(0,0.2);
    }

}

int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}
