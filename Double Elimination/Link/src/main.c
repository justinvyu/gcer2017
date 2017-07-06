#include <kipr/botball.h>
#include "Controller.h"
#include "Create.h"
#define CAGE 1
#define CAGE_DOWN 280
#define CAGE_MUP 600
#define CAGE_MID 440
#define CAGE_UP 920
#define CAGE_UPPER 1250
#define CAGE_START 1450
#define CAGE_ALL_UP 1600
#define CAGE_BACKUP 1890
#define CAGE_READY 1050
#define ARM 0
#define SPINNER_PORT 0
#define SPINNER_SPIN 40
#define SPINNER_TIME 9600 //5200 for 2 turns //later 7800

Controller c;

void turnInPlace(int direction, int time, int power)
{
  	if(direction == 0)
    {
    	motor(2, power);
  		motor(3, -power);
  		msleep(time);  
    }
  	if(direction == 1)
    {
    	motor(2, -power);
  		motor(3, power);
  		msleep(time);  
    }
}

void get_orange_tribbles() {
  motor(ARM, 40);
  msleep(2200);
  ao();
}

void turn_left() {
  c.left(102, 2, 80);
}

void turn_right() {
  c.right(98, 2, 80);
}

void turn_45_right() {
  c.right(49, 2, 80);
}

int main()
{
 
  	//start spinner horizontal to the right (motor)---arm--- (looking at bot from behind base)
  	
    c = new_controller(3,2,16.5,4);
  	enable_servos();
    c.servo(CAGE, CAGE_START);
    wait_for_light(0);
    shut_down_in(119);
  	c.servo(CAGE, CAGE_DOWN);
  	msleep(500);
  	c.forward(6, 30);
  	c.backward(2, 30); //back and forth to make sure tribbles don't fall out
  	c.forward(2, 30);
  	c.backward(2, 30); //back and forth to make sure tribbles don't fall out
  	c.forward(2, 30);
  	c.servo(CAGE, CAGE_MID);
    msleep(250);
  	c.servo(CAGE, CAGE_DOWN);
  	msleep(500);
  	c.servo(CAGE, CAGE_UP);	//Got poms in base
    msleep(250);
  	turn_left();
  	c.forward(30, 80);
  	c.slow_servo(CAGE, CAGE_MUP, 1);
  	c.forward(22, 80); //decreased from 22
  	c.motor(SPINNER_PORT,SPINNER_SPIN);
  	msleep(SPINNER_TIME);
  	c.motor(SPINNER_PORT,0);
	c.servo(CAGE, CAGE_UP);
  	msleep(250);
  	c.slow_servo(CAGE, CAGE_DOWN, 1); //changed to slow servo 
  	msleep(250);
  	c.servo(CAGE, CAGE_UP);
  	c.backward(27, 80); //increased from 20
  	turnInPlace(1,1800,50);
  	c.backward(45,80);
  	c.backward(10,30); //reduced from 15, squaring up
  	c.forward(95, 80);
  	turn_45_right();
  	c.forward(30,80);
  	turn_45_right();
  	double start = seconds();
    double curr;
  	for(curr = start; curr - start < 6 && !digital(9); curr = seconds()) { //squaring up
      c.motor(2, 70);
      c.motor(3, 70);
    }
    c.backward(13,70);
  	c.servo(CAGE, CAGE_ALL_UP);
  	turn_left();
  	c.forward(10, 80);
  	while(analog(4)<3000 && analog(5) < 3000)
    {
     	c.motor(2, 70);
      	c.motor(3, 63);
    }
  	c.forward(56,80); //originally 70
  	c.backward(3, 80);
  	turn_left(); //turnInPlace(0,1800,50);
  	c.backward(35,60); //squaring up
  	c.forward(80,80);
  	c.servo(CAGE, CAGE_READY);
  	c.forward(21,80);
  	//c.forward(5, 60);
  	c.right(30, 2, 60); //turn back and forth and try to get haystacks out of the way
  	motor(3, -60);
  	msleep(900);
  	ao();
  	//c.right(30, 2, 60);
  	//c.left(30, 2, 60);
  	motor(2, 60);
  	msleep(500);
  	ao();
 	motor(3, -60);
  	msleep(500);
  	motor(3,60);
  	msleep(400);
  	ao();
    
 
  	//c.right(40, 2, 60);
  	//c.left(40, 2, 60);
  	//c.right(40, 2, 60);
  	//c.left(20, 2, 60);
  	c.forward(17, 40);
  	c.slow_servo(CAGE, CAGE_DOWN, 1.5);
  	msleep(1000);
  	c.backward(120,80);
  	//c.servo(CAGE, CAGE_DOWN);
  	
  	
  	
  	/*get_orange_tribbles();
  	get_orange_tribbles(); //2nd time getting orange tribbles
  	c.backward(41, 80);
  	turn_right();
  	c.servo(CAGE, CAGE_MID);
  	c.forward(52, 80);
  	turn_right();
  	int i = 0;
  	while(digital(9) == 0) { //squaring up
      c.forward(5, 80);
      i ++;
      if (i > 10) {
        break;
      }
    }
  	c.backward(13, 60);
  	msleep(250);
  	c.left(97, 0, 60);	//98
  	msleep(250); 	
  	while(analog(4)<3000 && analog(5)<3000)
    {
   		c.forward(1, 60);
    }
  	msleep(250);
  	c.forward(77, 60);	//pushing seed bin to wall
  	msleep(250); 
  	c.servo(CAGE, CAGE_UPPER);
  	c.backward(13, 60);
  	turn_left();
  	c.backward(22, 60); //squaring up in preparation to drop tribbles
    */
  	return 0;
}
