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
#define BOTGUY_TOPHAT_THRESHOLD 780
#define LEFT_ET 4

#define BOTTOM_TOPHAT 0

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

void lower_claw(int ticks) {
    controller.motor(BOTGUY_MOTOR, -30);
    controller.cmpc(BOTGUY_MOTOR);
    while(digital(TOUCH));
    while(abs(controller.gmpc(BOTGUY_MOTOR)) < ticks);
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

int ask_botguy_pos() {
    printf("A: Botguy facing towards bot\nB: Botguy facing away from bot");
    while(1) {
      if(a_button()) return 1;
      if(b_button()) return 0;
    }
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

    controller.forward(11, 250);
}

/**
 *  line_follow(int distance_in_ticks) function by Justin Yu 3/14
 *  --------------------------------------------------------------
 *  This function uses a PID controller to follow a line for a
 *  specified distance, stopping when the motor position counter
 *  reaches the input `distance_in_ticks` parameter. The P controller calculates
 *  the error between the current sensor value and the desired threshold
 *  value. The I controller calculates a running total of error. The
 *  D controller calculates the change in error from iteration to iteration.
 *  These three, multiplied by experimentally defined constants and summed,
 *  create a turn variable that alters the speed of both motors (0 and 1).
 */
void line_follow(int distance_in_ticks) {
    float kp = 2.4,   // proportional constant
          ki = 0.1,   // integral constant
          kd = 8.2;   // derivative constant

    float integral_scalar = 7 / 8.;

    int last_error = 0;
    int speed = 1000;    // velocity of motors at error = 0
    int threshold = 3650; // experimentally determined white/black threshold
    int sensor_value, error = 0, integral = 0, derivative = 0;
    int turn = 0;

    controller.cmpc(controller.motor_left);
    controller.cmpc(controller.motor_right);
    while(controller.gmpc(controller.motor_left) < distance_in_ticks
          && controller.gmpc(controller.motor_right) < distance_in_ticks) {
        sensor_value = controller.analog(BOTTOM_TOPHAT);

        error = sensor_value - threshold;                  // P
        integral = integral_scalar * integral + error;     // I
        derivative = error - last_error;                   // D

        turn = (int)(kp * error + ki * integral + kd * derivative);
        controller.mav(controller.motor_left, speed + turn); // power left motor
        controller.mav(controller.motor_right, speed - turn); // power right motor
        last_error = error;
        msleep(1);
    }
}

int main() {
    controller = new_controller(3, 0, 17, 5.5);

    controller.cmpc(BOTGUY_MOTOR);
    //grab_botguy(); // MUST BE SET UP SUCH THAT CLAW IS FULLY OPEN AT THE BOTTOM!!
    int ticks_to_bottom = controller.gmpc(BOTGUY_MOTOR);
    //lower_claw(ticks_to_bottom);

    controller.servo(LEFT_GRABBER, LEFT_GRABBER_UP);
    controller.servo(RIGHT_GRABBER, RIGHT_GRABBER_UP);
    controller.enable_servos();
    int botguy_pos = 0; //ask_botguy_pos();
    //wait_for_light(2);
    /*controller.backward(15, 500);

    controller.left(45, 0, 80);
    controller.right(48, 0, 90);

    controller.backward(25, 500);
    controller.forward_with_speed(100, 82, 80);

    while(!right_sees_pole()) {
        controller.forward_with_speed(1, 82, 80);
        msleep(1);
    }

    controller.forward_with_speed(47, 82, 80);
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

    controller.right(83, 0, 90);

    controller.motor(controller.motor_left, 50);
    controller.motor(controller.motor_right, 50);
    while(!right_sees_pvc()) {
        msleep(1);
    }
    controller.motor_off(controller.motor_left);
    controller.motor_off(controller.motor_right);

    controller.forward_with_speed(3, 81, 80);
    controller.right(83, 4, 90);
    controller.forward_with_speed(15, 82, 80);
    controller.forward_with_speed(35, 50, 50); // square up on the front
    controller.backward(18, 500);
    controller.right(45, 0, 100);
    controller.right_in_place(35, 100);
    controller.backward(30, 500);*/

	//lower_claw(ticks_to_bottom);
    controller.forward_with_speed(15, 82, 80);
	controller.motor(controller.motor_left, 25);
	controller.motor(controller.motor_right, 25);
    while(controller.analog(BOTGUY_TOPHAT) < BOTGUY_TOPHAT_THRESHOLD);
    controller.motor_off(controller.motor_left);
    controller.motor_off(controller.motor_right);

    if(botguy_pos) {
        controller.forward_with_speed(7, 27, 25);
    }
    else {
        controller.forward_with_speed(6, 27, 25);
    }
    grab_botguy();
    controller.forward_with_speed(65,84,80);
    controller.backward(11, 500);
    controller.left(45, 0, 100);
    controller.backward(4, 500);
    controller.left_in_place(40, 100);
    controller.backward(18, 300);
    controller.forward_with_speed(35, 82, 80);
    /*controller.right(20, 6, 90);
    controller.left(10, 8, 100);

    line_follow(2600);
//    controller.forward_with_speed(38, 84, 80);

    controller.left(45, 0, 60);
    controller.left_in_place(45, 100);
    controller.backward(30, 500);
    controller.forward_with_speed(30, 81, 80);*/

    //open botguy claw more
    // turn right with radius
    // turn left with radius
    // keep going maybe linefollow
    // we're done
    /*
    while(1) {
        printf("%d\n", controller.analog(BOTTOM_TOPHAT));
        msleep(500);
    }*/
    return 0;
}
