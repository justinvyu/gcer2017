#include <kipr/botball.h>

#include "Controller.h"

#define BASE 3

#define WATERARM 0
#define WATERARM_LOWEST 500
#define WATERARM_DOWN 600
#define WATERARM_MID 1000
#define WATERARM_UP 2000
#define POMARM 1
#define POMARM_UP 500
#define POMARM_START 800
#define POMARM_MID 1400
#define POMARM_DOWN 1500


#define FULL_REVOLUTION 3200

int base_angle = 0;


/*** ================== DRIVING STUFFZ ================== ***/

void mav_forward(int power, int time) {
  	controller.mav(controller.motor_right, power);
	controller.mav(controller.motor_left, power);
  	msleep(time);
  	off(controller.motor_right);
  	off(controller.motor_left);
}

void turn_right_in_place(int power, int time) {
	controller.mav(controller.motor_right, -power);
	controller.mav(controller.motor_left, power);
  	msleep(time * 1.4);	//new wheels lol
  	off(controller.motor_right);
  	off(controller.motor_left);
}

void turn_left_in_place(int power, int time) {
	controller.mav(controller.motor_right, power);
	controller.mav(controller.motor_left, -power);
  	msleep(time * 1.4);	//new wheels lol
  	off(controller.motor_right);
  	off(controller.motor_left);
}

/*** ================== ROBOT SPECIFC ================== ***/

//Negative is clockwise
//Positive is counter-clockwise
//revolutions should always be positive, power determines direction
void base_rotate(double revolutions, int power) {
  	int initial = gmpc(BASE);
  	int distance = (int)(revolutions * FULL_REVOLUTION);
  	
  	controller.mav(BASE, power);
  	while(abs(gmpc(BASE) - initial) < abs(distance)) {
      	msleep(50);
    }
  	off(BASE);
  
  	if(power < 0) {
      	base_angle += revolutions;
    }
  	else {
      	base_angle -= revolutions;
    }
}
/*
void base_set_angle(int angle, int power) {
  	base_rotate(abs(angle - base_angle), power);
}
*/

void shake(int counts) {
  	//do stuff
  	int i;
  	for(i = 0; i < counts; i++) {
      controller.servo(WATERARM, WATERARM_UP);
      msleep(500);
      controller.servo(WATERARM, WATERARM_MID);
      msleep(100);
    }
}

void move_under_pipe(int speed) {
  double start = seconds();
  double curr;
  for(curr = start; analog(0)<1900 && curr - start < 5; curr = seconds()) {
    controller.mav(controller.motor_right, speed);
    controller.mav(controller.motor_left, speed);
  }
  ao();
}

void watertank_one() {
  	//empty water tank by moving water arm up and driving backwards
  	//controller.mav(controller.motor_right, 100);
  	//controller.mav(controller.motor_left, 100);
  	controller.slow_servo(WATERARM, WATERARM_MID, 2);
  	controller.mav(controller.motor_right, 290);
  	controller.mav(controller.motor_left, 290);
  	controller.slow_servo(WATERARM, WATERARM_UP, 3.5);
  	msleep(500);
  	ao();
  	shake(6);
  	
  	//lower water arm
  	controller.mav(controller.motor_right, -200);
  	controller.mav(controller.motor_left, -200);
  	controller.servo(WATERARM, WATERARM_DOWN);
  	ao();
}

void watertank_two() {
  	//empty water tank by moving water arm up and driving backwards
    move_under_pipe(-500);
  	msleep(500);
    controller.mav(controller.motor_right, 200);
    controller.mav(controller.motor_left, 200);
    msleep(3500);
    ao();
    msleep(500);
 	//controller.mav(controller.motor_right, -50);
  	//controller.mav(controller.motor_left, -50);
  	controller.slow_servo(WATERARM, WATERARM_MID, 2);
  	controller.mav(controller.motor_right, -360);
  	controller.mav(controller.motor_left, -360);
  	controller.slow_servo(WATERARM, WATERARM_UP, 4.0);
  	msleep(500);
  	ao();
  	shake(6);

  	//lower water arm
  	controller.servo(WATERARM, WATERARM_DOWN);
  	ao();
}

void rotate_one_two(){
  	controller.servo(WATERARM, WATERARM_LOWEST);
  	//mav_forward(-500, 700);
  
  	//	CODE FOR ROTATING BASE MORE
  	base_rotate(0.25, -300);	//POSITION: 0.25
  	controller.servo(WATERARM, WATERARM_DOWN);	//get over side of robot
  	base_rotate(0.25, -300);	//POSITION: 0.50
  	controller.servo(WATERARM, WATERARM_LOWEST);
  	mav_forward(500, 2100);
  	
}

void rotate_two_one(){
  	controller.servo(WATERARM, WATERARM_LOWEST);
  	//mav_forward(-500, 700);
  
  	//	CODE FOR ROTATING BASE MORE
  	base_rotate(0.25, 300);	//POSITION: 0.25
  	controller.servo(WATERARM, WATERARM_DOWN);	//get over side of robot
  	base_rotate(0.25, 300);	//POSITION: 0.50
  	controller.servo(WATERARM, WATERARM_LOWEST);
  	//mav_forward(-550, 2100);
}

void bored(){
  
  	set_servo_position(WATERARM, WATERARM_MID);//waiting for create
  	int i = 0;
  	for(i=0; i<4; i++){
      	if(i%2 == 0){
          base_rotate(1, 300); 
          msleep(700);
        } else {
          base_rotate(1, -300); 
          msleep(700);
        }
    }

  
  	set_servo_position(WATERARM, WATERARM_DOWN);
  
}

void pickup(int rep){
    set_servo_position(POMARM, POMARM_DOWN);
    msleep(250);
    controller.forward(7, 20);
    msleep(250);
    controller.slow_servo(POMARM, POMARM_UP, 1.5);
    msleep(250);
    /*controller.backward(2, 20);
    msleep(250);*/
  
  	int i;
  	for(i=0; i<rep; i++){
      set_servo_position(POMARM, POMARM_DOWN);
      msleep(250);
      controller.slow_servo(POMARM, POMARM_UP, 1.5);
      msleep(250);
    }
}

int main()
{
    
    //---------------------------------Initialize the Robot--------------------------------------
    controller = new_controller(1, 0, 14.5, 4.0);
  	controller.enable_servos();
  	controller.servo(POMARM,POMARM_START);
  	controller.servo(WATERARM, WATERARM_DOWN);
    wait_for_light(1);
    shut_down_in(119);
    double start = seconds();
  
  	//-----------------------------------Pick Up & Doge-----------------------------------------
  	turn_right_in_place(600, 390); //doge create
  	msleep(4000);
  	turn_left_in_place(600, 390);
  	controller.backward(10, 60); //back square up (facing ramp)
  	controller.forward(15, 20);
  	pickup(4);
  	controller.backward(30, 50);//back square up (facing ramp)
  	msleep(250);
  	turn_right_in_place(800, 600); // dodge create (again)
  	msleep(500);
  
  	
  	//bored();	//charlie why you do dis
  	msleep(1000); 
 
  	//------------------------------------Placing Poms-----------------------------------------
  	turn_left_in_place(800, 300);
  	msleep(250);
  	controller.forward(10, 50);
  	msleep(250);
  	turn_left_in_place(1000, 780);
  	msleep(250);
  	controller.backward(20, 50);//back square up (facing haybales)
  	msleep(250);
  	controller.forward(10, 80);
  	turn_left_in_place(60, 780); //small turn correction
 	controller.forward(90, 80); //front square up (facing haybales)
  	msleep(250);
  	set_servo_position(POMARM, POMARM_MID); //deploying poms
  	controller.backward(3, 20);
  	msleep(250);
  	set_servo_position(POMARM, POMARM_UP);
  	msleep(250);
  	set_servo_position(POMARM, POMARM_DOWN);
  	controller.backward(15, 80);
  	msleep(250);
  	set_servo_position(POMARM, POMARM_UP);	//Deposited poms
  	
  	turn_right_in_place(1000, 740);
  	msleep(250);
  	controller.backward(20, 80);
  	msleep(250);
  
    //---------------------------------To the water container---------------------------------------
  	controller.forward(83, 80);
  	msleep(250);
  	turn_right_in_place(970, 750);
  	msleep(250);
  	controller.backward(80, 80); //back square up (facing pom bin)
  	msleep(250);
  
  	controller.forward(18, 50);
  	msleep(250);
  	turn_left_in_place(990, 780);
  	msleep(250);
  	controller.backward(20,50);
  	msleep(250);
  
  	set_servo_position(POMARM, POMARM_DOWN);
  	msleep(250);
  
  	controller.forward(35, 80);
  	msleep(250);
  	
  	set_servo_position(POMARM, POMARM_UP); // flip botguy
  	msleep(250);
  
  	controller.forward(10, 50);
  	msleep(250);
  
 	turn_right_in_place(1000, 370);
  	msleep(250);
  	//controller.forward(3, 50);
  
  	//-----------------------------------Emptying Tanks-------------------------------------------
  
  	watertank_two();
    move_under_pipe(500);
  	controller.forward(2, 50);
  	msleep(250);
  	rotate_two_one();
 	watertank_one();
    printf("Time taken: %f", seconds()-start);
  	//rotate_one_two();
  	return 0;
}

