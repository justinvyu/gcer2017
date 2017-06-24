#include <kipr/botball.h>

#include "Controller.h"

#define BASE 3

#define WATERARM 0
#define WATERARM_LOWEST 500
#define WATERARM_DOWN 600
#define WATERARM_MID 1000
#define WATERARM_UP 2000
#define POMARM 1
#define POMARM_UP 1000
#define POMARM_DOWN 2000


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
  	msleep(time);
  	off(controller.motor_right);
  	off(controller.motor_left);
}

void turn_left_in_place(int power, int time) {
	controller.mav(controller.motor_right, power);
	controller.mav(controller.motor_left, -power);
  	msleep(time);
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

void watertank_one() {
  	//empty water tank by moving water arm up and driving backwards
  	msleep(500);
  	controller.mav(controller.motor_right, 100);
  	controller.mav(controller.motor_left, 100);
  	controller.slow_servo(WATERARM, WATERARM_MID, 2);
  	controller.mav(controller.motor_right, 200);
  	controller.mav(controller.motor_left, 200);
  	controller.slow_servo(WATERARM, WATERARM_UP, 3.5);
  	msleep(500);
  	ao();
  	shake(10);
  	
  	//lower water arm
  	controller.mav(controller.motor_right, -200);
  	controller.mav(controller.motor_left, -200);
  	controller.slow_servo(WATERARM, WATERARM_DOWN, 4);
  	ao();
}

void watertank_two() {
  	//empty water tank by moving water arm up and driving backwards
  	msleep(500);
  	//controller.mav(controller.motor_right, -50);
  	//controller.mav(controller.motor_left, -50);
  	controller.slow_servo(WATERARM, WATERARM_MID, 2);
  	controller.mav(controller.motor_right, -200);
  	controller.mav(controller.motor_left, -200);
  	controller.slow_servo(WATERARM, WATERARM_UP, 3.5);
  	msleep(500);
  	ao();
  	shake(10);

  	//lower water arm
  	controller.mav(controller.motor_right, 200);
  	controller.mav(controller.motor_left, 200);
  	controller.slow_servo(WATERARM, WATERARM_DOWN, 4);
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
  	mav_forward(-550, 2100);
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


int main()
{
    
    // instantiate your global robot object
    controller = new_controller(1, 0, 15.24, 5.83);
  	controller.servo(WATERARM, WATERARM_LOWEST);
  	controller.enable_servos();
  
  	/*
  	watertank_one();
  	controller.servo(WATERARM, WATERARM_LOWEST);
  	//mav_forward(-500, 700);
  
  	//	CODE FOR ROTATING BASE MORE
  	base_rotate(0.25, -300);	//POSITION: 0.25
  	controller.servo(WATERARM, WATERARM_DOWN);	//get over side of robot
  	base_rotate(0.25, -300);	//POSITION: 0.50
  	controller.servo(WATERARM, WATERARM_LOWEST);
  	mav_forward(500, 2100);
  	//controller.slow_servo(WATERARM, WATERARM_MID, 2);	//on pipe
  	
  	//base_rotate(0.2, 300);	//POSITION: 0.50
    
    
  	watertank_two();
	
  	msleep(2000);
  	
    return 0;
    */
  
  	enable_servos();
  	
  	set_servo_position(POMARM, POMARM_UP);
  	msleep(250);
  	controller.backward(15, 50);//square up against pipe
  	msleep(250);
  	turn_right_in_place(1000, 600); // doge create
  
  	
  	//bored();	//charlie why
 
  	
  	turn_left_in_place(1000, 300);
  	msleep(250);
  	controller.forward(6, 50);
  	msleep(250);
  	turn_left_in_place(1000, 780);
  	msleep(250);
  	controller.backward(15, 50);//square up against vertical pipe
  	msleep(250);
  	controller.forward(100, 80);
  	msleep(250);
  	set_servo_position(POMARM, POMARM_DOWN);
  	controller.backward(13, 50);
  	msleep(250);
  	set_servo_position(POMARM, POMARM_UP);
  	turn_right_in_place(1000, 780);
  	msleep(250);
  	controller.backward(20, 50);
  	msleep(250);
  	controller.forward(100, 50);
  	msleep(250);
  	turn_right_in_place(1000, 850);
  	msleep(250);
  	controller.backward(80, 50);
  	msleep(250);
  
  	controller.forward(19, 50);
  	msleep(250);
  	turn_left_in_place(1000, 780);
  	msleep(250);
  	controller.backward(15,50);
  	msleep(250);
  
  	set_servo_position(POMARM, POMARM_DOWN);
  	msleep(250);
  
  	controller.forward(50, 50);
  	msleep(250);
  	
  	set_servo_position(POMARM, POMARM_UP); // flip botguy
  	msleep(250);
  
 	turn_right_in_place(1000, 370);
  	msleep(250);
  	//controller.forward(3, 50);
  
  	watertank_two();
  	rotate_two_one();
 	watertank_one();
  
  	rotate_one_two();
  	return(0);
}

