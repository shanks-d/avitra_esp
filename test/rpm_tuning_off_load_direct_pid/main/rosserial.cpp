#include "ros.h"
#include "rosserial.h"
#include "auto_nav/tuning_msg.h"
#include "auto_nav/velocity_msg.h"
#include "auto_nav/pid_response.h"

ros::NodeHandle nh;

auto_nav::tuning_msg tuna;
auto_nav::pid_response pr;
ros::Publisher espPub("pid_response", &pr);

void callback(const auto_nav::tuning_msg& msg){
  tuna.Kp = msg.Kp;
  tuna.Kd = msg.Kd;
  tuna.Ki = msg.Ki;
}

ros::Subscriber<auto_nav::tuning_msg> espSub("tuning", &callback);

void rosserial_setup()
{
  nh.initNode();
  nh.advertise(espPub);
  nh.subscribe(espSub);
  tuna.Kp = 0;
  tuna.Kd = 0;
  tuna.Ki = 0;
}

void rosserial_publish(motor_t* motor_R)
{
  pr.curr_rpm = motor_R->encoder.curr_rpm;
  pr.prev_rpm  = motor_R->encoder.prev_rpm;
  pr.duty_cycle = motor_R->duty_cycle;
  pr.actual_duty_cycle = motor_R->actual_duty_cycle;
  pr.desr_rpm = motor_R->desr_rpm;
  pr.err = motor_R->err;
  pr.prev_err = motor_R->prev_err;
  pr.cum_err = motor_R->cum_err;
  pr.pTerm = motor_R->pTerm;
  pr.dTerm = motor_R->dTerm;
  pr.iTerm = motor_R->iTerm;
  pr.iTerm_limit = motor_R->iTerm_limit;
  pr.alpha = motor_R->alpha;
  espPub.publish(&pr);
}

void rosserial_subscribe(motor_t* motor_L, motor_t* motor_R){
  motor_L->Kp = tuna.Kp;
  motor_L->Kd = tuna.Kd;
  motor_L->Ki = tuna.Ki;
  motor_R->Kp = tuna.Kp;
  motor_R->Kd = tuna.Kd;
  motor_R->Ki = tuna.Ki;
  nh.spinOnce();
}

void rosserial_spinonce(){
  nh.spinOnce();
}
