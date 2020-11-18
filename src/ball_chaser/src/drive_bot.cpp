#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "ball_chaser/DriveToTarget.h"

// ROS::Publisher motor commands;
ros::Publisher motor_command_publisher;

//handle_drive_request executes when drive_bot service is requested, publishes linear and angular velocities
//to the robot wheel joints
bool handle_drive_request(ball_chaser::DriveToTarget::Request& req,
    ball_chaser::DriveToTarget::Response& res)
{
    ROS_INFO("DriveToTargetRequest received - linear:%1.2f, angular:%1.2f", (float)req.linear_x, (float)req.angular_z);
    //crate a motor_command object of type geometry_msgs::Twist
    geometry_msgs::Twist motor_command;
    //set requested linear and angular velocities
    motor_command.linear.x = req.linear_x;
    motor_command.angular.z = req.angular_z;
    //publish velocities to drive the robot
    motor_command_publisher.publish(motor_command);

    // Return a response message
    res.msg_feedback = "Velocity set - linear: " + std::to_string(motor_command.linear.x) + " , angular: " + std::to_string(motor_command.angular.z);
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

    ros::ServiceServer service = n.advertiseService("/ball_chaser/command_robot", handle_drive_request);

    // TODO: Handle ROS communication events
    ros::spin();

    return 0;
}