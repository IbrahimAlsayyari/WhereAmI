#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "ball_chaser/DriveToTarget.h"

//TODO: Include the ball_chaser "DriveToTarget" header file

// ROS::Publisher motor commands;
ros::Publisher motor_command_publisher;

// TODO: Create a handle_drive_request callback function that executes whenever a drive_bot service is requested
// This function should publish the requested linear x and angular velocities to the robot wheel joints
// After publishing the requested velocities, a message feedback should be returned with the requested wheel velocities

    // TODO: Delete the loop, move the code to the inside of the callback function and make the necessary changes to publish the requested velocities instead of constant values

std::vector<float> velocity_control(float requested_j1, float requested_j2)
{
        geometry_msgs::Twist motor_command;
        // Set wheel velocities, forward [0.5, 0.0]
        motor_command.linear.x = requested_j1;
        motor_command.angular.z = requested_j2;
        // Publish angles to drive the robot
        motor_command_publisher.publish(motor_command);

      // Store clamped joint angles in a clamped_data vector
      std::vector<float> clamped_data = { requested_j1, requested_j1 };

    return clamped_data;
}

bool handle_safe_move_request(ball_chaser::DriveToTarget::Request& req,  ball_chaser::DriveToTarget::Response& res)
{

    ROS_INFO("DriveToTarget received - j1:%1.2f, j2:%1.2f", (float)req.linear_x, (float)req.angular_z);
	
    std::vector<float>  velo = velocity_control(req.linear_x, req.angular_z);
    //clamp_at_boundries ; 
        // Return a response message
    res.msg_feedback = "Joint angles set - j1: " + std::to_string(velo[0]) + " , j2: " + std::to_string(velo[1]);
  
    ROS_INFO_STREAM(res.msg_feedback);

    return true;
}

int main(int argc, char** argv)
{
    // Initialize a ROS node
    ros::init(argc, argv, "drive_bot");

    // Create a ROS NodeHandle object
    ros::NodeHandle n;

    // Inform ROS master that we will be publishing a message of type geometry_msgs::Twist on the robot actuation topic with a publishing queue size of 10
    motor_command_publisher = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

    // TODO: Define a drive /ball_chaser/command_robot service with a handle_drive_request callback function
	ros::ServiceServer service = n.advertiseService("/ball_chaser/command_robot", handle_safe_move_request);
    ROS_INFO("Ready to send joint commands");
    // TODO: Delete the loop, move the code to the inside of the callback function and make the necessary changes to publish the requested velocities instead of constant values
    
    // TODO: Handle ROS communication events
    ros::spin(); 

    return 0;
}
