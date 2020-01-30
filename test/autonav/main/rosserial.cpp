#include "ros.h"
#include "auto_nav/ticks_msg.h"
#include "auto_nav/velocity_msg.h"
#include "auto_nav/base_params_msg.h"
#include "rosserial.h"

ros::NodeHandle nh;

auto_nav::velocity_msg velocity;
auto_nav::velocity_msg rpm;
auto_nav::ticks_msg ticks;
auto_nav::base_params_msg base_params;
ros::Publisher espPub_ticks("ticks", &ticks);
ros::Publisher espPub_base_params("base_params", &base_params);

void callback_teleop(const auto_nav::velocity_msg& msg){
  velocity.motor_F = msg.motor_F;
  velocity.motor_B = msg.motor_B;
  velocity.motor_L = msg.motor_L;
  velocity.motor_R = msg.motor_R;
  velocity.command = msg.command;
}
ros::Subscriber<auto_nav::velocity_msg> espSub_teleop("teleop", &callback_teleop);

void callback_rpm(const auto_nav::velocity_msg& msg){
  rpm.motor_L = msg.motor_L;
  rpm.motor_R = msg.motor_R;
}
ros::Subscriber<auto_nav::velocity_msg> espSub_rpm("rpm", &callback_rpm);

void rosserial_setup(){  // Initialize ROS
  nh.initNode();
  nh.advertise(espPub_ticks);
  nh.advertise(espPub_base_params);
  nh.subscribe(espSub_teleop);
  nh.subscribe(espSub_rpm);
  velocity.motor_F = 0;
  velocity.motor_B = 0;
  velocity.motor_L = 0;
  velocity.motor_R = 0;
  velocity.command = 0;
  rpm.motor_L = 0;
  rpm.motor_R = 0;
}

void rosserial_publish_ticks(volatile long int* ticks_L, volatile long int* ticks_R){
  ticks.motor_L = *ticks_L;
  ticks.motor_R = *ticks_R;
  ticks.motor_R *= -1;    //since the right motor moves forward in negative rpm, negative ticks are published
  espPub_ticks.publish(&ticks);  // publish the msg
}

void rosserial_publish_base_params(float* duty_cycle_L, float* duty_cycle_R, float* desr_rpm_L, float* desr_rpm_R, float* curr_rpm_L, float* curr_rpm_R)
{
  base_params.duty_cycle_L = *duty_cycle_L;
  base_params.duty_cycle_R = *duty_cycle_R;
  base_params.desr_rpm_L = *desr_rpm_L;
  base_params.desr_rpm_R = *desr_rpm_R;
  base_params.curr_rpm_L = *curr_rpm_L;
  base_params.curr_rpm_R = *curr_rpm_R;
  espPub_base_params.publish(&base_params);
}

void rosserial_subscribe_teleop(float* duty_F, float* duty_B, float* duty_L, float* duty_R){
  *duty_F = velocity.motor_F;
  *duty_B = velocity.motor_B;
  *duty_L = velocity.motor_L;
  *duty_R = velocity.motor_R;
  nh.spinOnce();
}

void rosserial_subscribe_command(int* command){
  *command = velocity.command;
  nh.spinOnce();
}

void rosserial_subscribe_rpm(float* rpm_L, float* rpm_R){    
  *rpm_L = rpm.motor_L;
  *rpm_R = rpm.motor_R;
  *rpm_R *= -1;   //since the right motor moves forward with negative rpm
  nh.spinOnce();
}
