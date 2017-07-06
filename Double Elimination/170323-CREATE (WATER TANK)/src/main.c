#include <kipr/botball.h>
#include "Controller.h"
#include "Create.h"

Controller c;

#define ARM 1
#define ARM_UP 0
#define ARM_MID 1100
#define ARM_DOWN 1500

#define turn_constant 0.9042
#define drive_constant 0.754

void crz(){
  	int i;
  	for(i=0; i<8; i++){
      	create.backward(3,345);
       	create.forward(4,345);
      	msleep(200);
    }
}

int main()
{
    c = new_create_controller();
  	create = new_create();
  	create.connect();
  	
  	c.enable_servos();
  	c.servo(ARM, ARM_MID);
  	
  	create.forward(6,100);
  	create.right(88.5*turn_constant,0,100);
  	create.backward(1,100);
  	c.disable_servos(); 
  	
  	wait_for_light(0);
  	shut_down_in(119);
  	
  	c.enable_servos();
    create.drive_direct(-50,-500);
  	msleep(1130*drive_constant);
  	create.drive_direct(-500,-500);
  	msleep(2080*drive_constant);
  	
  	c.servo(ARM, ARM_UP);
  	
  	create.drive_direct(-500,-50);
  	msleep(1160*drive_constant);
  	
  	create.drive_direct(-500,-500);
  	msleep(300*drive_constant);
  	create.stop();
  	c.disable_servos();
  
  	create.forward(11,20);
  	msleep(3000);
  	crz();
  
  	create_disconnect();
}
