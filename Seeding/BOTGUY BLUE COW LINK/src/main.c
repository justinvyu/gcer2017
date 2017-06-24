#include <kipr/botball.h>

#include "Controller.h"

#define LEFT_GRABBER 1
#define RIGHT_GRABBER 0
#define RIGHT_GRABBER_UP 850
#define RIGHT_GRABBER_DOWN 2047
#define LEFT_GRABBER_UP 1250
#define LEFT_GRABBER_DOWN 100

#define TOUCH 4

#define BOTGUY_TOPHAT 1
#define BOTGUY_TOPHAT_THRESHOLD 730
#define LEFT_ET 4

#define RIGHT_ET 5
#define ET_COW_THRESHOLD 500
#define ET_PVC_THRESHOLD 450
#define ET_RIGHTPOLE_THRESHOLD 400
#define BOTGUY_MOTOR 2

void deploy_left_grabber() {
    controller.servo(LEFT_GRABBER, LEFT_GRABBER_DOWN);
    msleep(1000);
    controller.slow_servo(LEFT_GRABBER, LEFT_GRABBER_UP, 2);
}

void deploy_right_grabber() {
    controller.servo(RIGHT_GRABBER, RIGHT_GRABBER_DOWN);
    msleep(1000);
    controller.slow_servo(RIGHT_GRABBER, RIGHT_GRABBER_UP, 2);
}

void lower_claw() {
    controller.motor(BOTGUY_MOTOR, -30);
    msleep(1500);
    controller.motor_off(BOTGUY_MOTOR);
}
/*
void grab_botguy() {
    controller.motor(BOTGUY_MOTOR, 15);
    msleep(3500);
    controller.motor_off(BOTGUY_MOTOR);
    controller.motor(BOTGUY_MOTOR, 0.1);
}*/

void grab_botguy() {
    controller.motor(BOTGUY_MOTOR, 15);
    while(!digital(TOUCH));
    controller.motor_off(BOTGUY_MOTOR);
    controller.motor(BOTGUY_MOTOR, 0.1);
}

int right_sees_pole() { // when crossing the field
    return controller.analog10(RIGHT_ET) > ET_RIGHTPOLE_THRESHOLD ? 1 : 0; 
}

int right_sees_cow() {
    return controller.analog10(RIGHT_ET) > ET_COW_THRESHOLD ? 1 : 0; 
}

int right_sees_pvc() {
    return controller.analog10(RIGHT_ET) > ET_PVC_THRESHOLD ? 1 : 0;
}

int left_sees_cow() {
    return controller.analog10(LEFT_ET) > ET_COW_THRESHOLD ? 1 : 0; 
}

void right_sense_cow() {
    controller.motor(controller.motor_left, 25);
    controller.motor(controller.motor_right, 25);
    while(!right_sees_cow()) {
        msleep(1);
    }
    controller.motor_off(controller.motor_left);
    controller.motor_off(controller.motor_right);
    
    controller.motor(controller.motor_left, 25);
    controller.motor(controller.motor_right, 25);
    while(right_sees_cow()) {
        msleep(1);
    }
    controller.motor_off(controller.motor_left);
    controller.motor_off(controller.motor_right);
  
    controller.backward(1, 200);
}

void left_sense_cow() {
    controller.motor(controller.motor_left, -25);
    controller.motor(controller.motor_right, -25);
    while(!left_sees_cow()) {
        msleep(1);
    }
    controller.motor_off(controller.motor_left);
    controller.motor_off(controller.motor_right);
    
    controller.motor(controller.motor_left, -25);
    controller.motor(controller.motor_right, -25);
    while(left_sees_cow()) {
        msleep(1);
    }
    controller.motor_off(controller.motor_left);
    controller.motor_off(controller.motor_right);
 
    controller.forward(11,  250);
}

int main() {
    controller = new_controller(3, 0, 17, 5.5);
    
    grab_botguy();
    controller.servo(LEFT_GRABBER, LEFT_GRABBER_UP);
    controller.servo(RIGHT_GRABBER, RIGHT_GRABBER_UP);
    controller.enable_servos();
    
    controller.backward(15, 500);
  
    controller.left(45, 0, 80);
    controller.right(48, 0, 90);
  
    controller.backward(25, 500);
    controller.forward_with_speed(100, 82, 80);
  
    while(!right_sees_pole()) {
        controller.forward_with_speed(1, 82, 80);
        msleep(1);
    }
    
    controller.forward_with_speed(42, 82, 80);
    msleep(800);
    controller.right_in_place(82, 80);
    // controller.forward(24, 80);
    controller.backward(2, 500);
    left_sense_cow();
    msleep(800);
    deploy_left_grabber();
    
    controller.backward(16, 800);
    controller.right_in_place(88, 90);
  
    controller.forward_with_speed(60, 81, 80);
    controller.left_in_place(95, 90);
    controller.backward(35, 800);
  
    controller.right(86, 1, 90);
  
    controller.motor(controller.motor_left, 50);
    controller.motor(controller.motor_right, 50);
    while(!right_sees_pvc()) {
        msleep(1);
    }
    controller.motor_off(controller.motor_left);
    controller.motor_off(controller.motor_right);
  
    controller.right(88, 3, 90);
    controller.forward_with_speed(15, 80, 82);
    controller.forward_with_speed(35, 50, 50); // square up on the front
    controller.backward(10, 500);
    controller.right(88, 0, 90);
    controller.backward(30, 500);
    lower_claw();
    controller.forward_with_speed(15, 81, 80);
    msleep(2000);
  
	controller.motor(controller.motor_left, 50);
	controller.motor(controller.motor_right, 50);
    while(controller.analog10(BOTGUY_TOPHAT) < BOTGUY_TOPHAT_THRESHOLD);
    controller.motor_off(controller.motor_left);
    controller.motor_off(controller.motor_right);
  
    grab_botguy();
    controller.forward_with_speed(35, 82, 80);
    controller.left(45, 0, 90);
    controller.left_in_place(45, 90);
    controller.forward_with_speed(20, 81, 80);
  
    //open botguy claw more
    // turn right with radius
    // turn left with radius
    // keep going maybe linefollow
    // we're done
    /*
    while(1) {
        printf("%d\n", controller.analog10(BOTGUY_TOPHAT));
        msleep(500);
    } */
    return 0;
}