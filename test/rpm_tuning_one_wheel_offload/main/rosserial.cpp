#include "ros.h"
#include "auto_nav/ticks_msg.h"
#include "auto_nav/velocity_msg.h"
#include "rosserial.h"

ros::NodeHandle nh;

auto_nav::tuning_msg tuna;

void callback(const auto_nav::tuning_msg& msg){
  tuna.Kp=msg.Kp;
  tuna.Kd=msg.Kd;
  tuna.Ki=msg.Ki;
}
ros::Subscriber<auto_nav::tuning_msg> espSub("tuning", &callback);

void rosserial_setup(){  // Initialize ROS
  nh.initNode();
  nh.subscribe(espSub);
  tuna.Kp = 0;
  tuna.Kd = 0;
  tuna.Ki = 0;
}


void rosserial_subscribe(motor_t* motor){
  motor->Kp = tuna.Kp;
  motor->Kd = tuna.Kd;
  motor->Ki - tuna.ki;
  nh.spinOnce();
}

void rosserial_spinonce(){
  nh.spinOnce();
}
