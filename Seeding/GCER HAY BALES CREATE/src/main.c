#include <kipr/botball.h>
#include "Controller.h"
#include "Create.h"

#define THRESHOLD 3500

#define CLAW 3
#define CLAW_OPEN_BIG 450
#define CLAW_OPEN 1000
#define CLAW_CLOSED 1500
#define ARM 1
#define ARM_DOWN 350 //400
#define ARM_UP 1570 //1700
#define ARM_MID 850
#define PUSHER 2
#define PUSHER_CLOSED 2000
#define PUSHER_OPEN 1000
#define STACKER 0
#define STACKER_DOWN 300
#define STACKER_UP 1350

// CAMERA

#define YELLOW_CHANNEL 0

Controller c;

void pickup() {
  c.servo(CLAW, CLAW_CLOSED);
  c.slow_servo(ARM, ARM_MID, 0.75);
  msleep(1000);
  c.servo(CLAW, CLAW_OPEN);
  c.slow_servo(ARM, ARM_DOWN, 1);
  msleep(200);
  c.servo(CLAW, CLAW_CLOSED);
  msleep(1000);
  c.slow_servo(ARM, ARM_UP, 2);
  c.slow_servo(CLAW, CLAW_OPEN,0.3);
  msleep(1000);
}

void pickup_big() {
  c.servo(CLAW, CLAW_CLOSED);
  c.slow_servo(ARM, ARM_MID, 0.75);
  msleep(1000);
  c.servo(CLAW, CLAW_OPEN_BIG);
  c.slow_servo(ARM, ARM_DOWN, 1);
  msleep(200);
  c.servo(CLAW, CLAW_CLOSED);
  msleep(1000);
  c.slow_servo(ARM, ARM_UP, 2);
  c.slow_servo(CLAW, CLAW_OPEN,0.3);
  msleep(1000);
}

void push() {
  c.servo(ARM,ARM_MID);
  msleep(300);
  c.slow_servo(PUSHER, PUSHER_CLOSED, 0.75);
  c.slow_servo(PUSHER, PUSHER_OPEN, 0.5);
  c.slow_servo(PUSHER, PUSHER_CLOSED, 0.5);
  c.slow_servo(PUSHER, PUSHER_OPEN, 0.2);
  c.slow_servo(PUSHER, PUSHER_CLOSED, 0.2);
  msleep(100);
  c.slow_servo(PUSHER, PUSHER_OPEN, 0.2);
  c.slow_servo(PUSHER, PUSHER_CLOSED, 0.2);
  msleep(100);
  c.slow_servo(PUSHER, PUSHER_OPEN, 0.2);
}
void small_push() {
  c.servo(ARM,ARM_MID);
  msleep(300);

  c.slow_servo(PUSHER,1700,5);
  msleep(300);
  c.servo(PUSHER,PUSHER_OPEN);
  msleep(300);
  c.slow_servo(PUSHER,1700,7);
  msleep(300);
  c.servo(PUSHER,PUSHER_OPEN);
  msleep(300);
}

void special_pickup() {
  c.servo(CLAW, CLAW_CLOSED);
  c.slow_servo(ARM, ARM_MID, 0.75);
  msleep(1000);
  c.servo(CLAW, CLAW_OPEN);
  c.slow_servo(ARM, ARM_DOWN, 1);
  msleep(200);
  c.servo(CLAW, CLAW_CLOSED);
  msleep(1000);
  create.forward(5, 30);
  c.servo(CLAW, CLAW_OPEN);
  msleep(200);
  create.backward(3, 30);
  c.servo(CLAW, CLAW_CLOSED);
  msleep(200);
  c.slow_servo(ARM, ARM_UP, 2);
  c.slow_servo(CLAW, CLAW_OPEN,0.3);
  msleep(1000);
}

void claw_down() {
  c.servo(CLAW,CLAW_OPEN);
  msleep(300);
}

// void camera_align();

void run_main_subroutine() {
  // Beginning routine

  // FIRST BLOCK

  msleep(2000);
  create.drive_direct(-50, -50); // square up
  msleep(2000);

  create.forward(4,150);

  create.right(86,0,150);
  create.drive_direct(-200, -200); //to the furrows
  msleep(4000);
  create.left(5, 0, 200);
  msleep(300);
  create.forward(1.5,200);
  //create.right(1,0,20);
  pickup();  // grab 1st
  push();

  // SECOND BLOCK

  c.servo(CLAW,CLAW_CLOSED);
  msleep(300);
  create.right(6,0,50);
  create.drive_direct(-200, -200);
  msleep(500);
  create.right(4,0,20);
  create.forward(1,200);
  pickup();  // grab 2nd
  c.servo(CLAW,CLAW_CLOSED);
  push();

  // THIRD BLOCK

  create.drive_direct(-100, -100); // square up
  msleep(750);
  create.forward(35,180);
  create.right(142,0,150);
  msleep(500);
  create.right(3,0,20);
  //create.backward(1.5,200);
  pickup_big();// grab 3rd
  push();
  msleep(1000);
  
    // FOURTH BLOCK

    create.right(130, 0, 200);
    create.drive_direct(-100, -100); // square up
    msleep(2000);
  
    create.forward(10, 200);
    create.left(20, 0, 200);
    create.forward(10, 200);
    create.right(24, 15, 200);
 
    create.forward(55, 200);

    create.right(55, 0, 200);
    create.backward(27, 100);
    pickup();  // grab 4th
    push();
    //camera_align(1);  //grab the 4th
    //push();
  
    // - SECOND HALF -

    create.forward(26,150);
    create.left(50,0,180);
    create.forward(65,180);
    create.right(85,0,180);
    create.drive_direct(-200, -200);
    msleep(2000);
  
    create.forward(29,180);
    create.right(85,0,180);
    create.drive_direct(-250, -250);
    msleep(2000);
    create.stop();
    msleep(500);
    create.drive_direct(-100, -100);
    msleep(3750);
  
    // FIFTH BLOCK

    create.forward(28,180);
    create.left(82,0,180);
    create.forward(8,150);
    create.left(26,0,180);
    pickup();
    push();
  
    // SIXTH BLOCK

    create.right(15,0,180);
    pickup();
    push();

    // SEVENTH BLOCK

    create.right(14,0,180);
    create.backward(3, 100);
    special_pickup();
    push();//grab the 7th
  
    create.forward(30, 200);
    c.servo(ARM, ARM_MID);
    msleep(200);
  
    c.slow_servo(STACKER, STACKER_UP, 5);
    create.backward(5, 100);
    create.forward(5, 100);
    create.backward(5, 100); 
    create.forward(5, 100);
    create.backward(5, 100);  
    create.left(10, 0, 150);

/*
    // SCORE BLOCKS

    c.servo(ARM,ARM_DOWN / 2);
    msleep(500);
    c.slow_servo(STACKER,STACKER_UP,5000);
    msleep(5000);
    create.left(20, 0, 50);
    msleep(500);
    create.right(20, 0, 50)
    c.enable_servos();
    int i=0;
    for(i=0; i<6; i++){
    c.servo(CLAW,CLAW_OPEN);
    c.servo(PUSHER,PUSHER_OPEN);
    c.servo(ARM,ARM_DOWN);
    c.servo(STACKER,STACKER_DOWN);
    msleep(3000);

    c.servo(CLAW,CLAW_CLOSED);
    msleep(300);
    c.servo(ARM,ARM_MID);
    msleep(300);
    c.slow_servo(ARM,ARM_UP,800);
    c.servo(CLAW,CLAW_OPEN);
    msleep(300);
    c.servo(ARM,ARM_MID);
    msleep(300);
    c.slow_servo(PUSHER,PUSHER_CLOSED,700);
    msleep(300);
    c.servo(PUSHER,PUSHER_OPEN);
    msleep(300);
    c.slow_servo(PUSHER,PUSHER_CLOSED,700);
    msleep(300);
    c.servo(PUSHER,PUSHER_OPEN);
    msleep(300);
}

c.slow_servo(STACKER,STACKER_UP,3000);
c.servo(STACKER,STACKER_UP);
msleep(5000);*/
}

/*
void camera_align(int var) {
    camera.update();
    camera.update();
    camera.update();

    int pivot = 20;
    while(camera.get_object_center_x(YELLOW_CHANNEL, 0) == -1) { // doesn't see
        camera.update();
        printf("Cant see, x: %d, y: %d\n", camera.get_object_center_x(YELLOW_CHANNEL, 0), camera.get_object_center_y(YELLOW_CHANNEL, 0));
        msleep(10);
    }

    camera.update();
    camera.update();
    camera.update();
    int hale_x = camera.get_object_center_x(YELLOW_CHANNEL, 0);
    if(hale_x > pivot) {
        create.drive_direct(-30, 30);
        while(camera.get_object_center_x(YELLOW_CHANNEL, 0) > pivot) {
            printf("Turning left, x: %d, y: %d\n", camera.get_object_center_x(YELLOW_CHANNEL, 0), camera.get_object_center_y(YELLOW_CHANNEL, 0));
            camera.update();
            camera.update();
            msleep(1);
        }
    } else {
        create.drive_direct(30, -30);
        while(camera.get_object_center_x(YELLOW_CHANNEL, 0) < pivot) {
            printf("Turning right, x: %d, y: %d\n", camera.get_object_center_x(YELLOW_CHANNEL, 0), camera.get_object_center_y(YELLOW_CHANNEL, 0));
            camera.update();
            camera.update();
            msleep(1);
        }
    }

    if(var == 1) {
        create.left(2, 0, 20);
    }
    if(var == 2) {
        create.left(2.5,0,20);
    }

    create.stop();
    c.servo(CLAW,CLAW_OPEN);
    c.servo(PUSHER,PUSHER_OPEN);
    c.servo(ARM,ARM_DOWN);
    c.servo(STACKER,STACKER_DOWN);

    msleep(1000);
    if(camera.get_object_center_y(YELLOW_CHANNEL, 0) > 52) {
        create.backward(4, 30);
    }

    create.right(1, 0, 20);
    pickup();
}
*/

void demo() {
  int i;
  for(i = 0; i < 7; i++) {
    create.backward(10,100);
    pickup();
    push();
  }
  c.slow_servo(STACKER, STACKER_UP, 5);
  create.backward(5, 100);
  create.forward(5, 100);
  create.backward(5, 100); 
  create.forward(5, 100);
  create.backward(5, 100);
}

int main()
{
  c = new_create_controller();
  create = new_create();

  create.connect();

  c.servo(ARM,ARM_UP);
  c.servo(CLAW,CLAW_CLOSED);
  c.servo(PUSHER, PUSHER_OPEN);
  c.servo(STACKER,STACKER_DOWN);
  //wait_for_light(0);
  //shut_down_in(119);
  c.enable_servos();

  int test = 1;
  if(test) {
    demo();
  }
  else {
    run_main_subroutine();
  }

  create.disconnect();

  return 0;

}
