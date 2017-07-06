#include <kipr/botball.h>
#include "Controller.h"
#include "Create.h"

#define THRESHOLD 3500

#define CLAW 3
#define CLAW_OPEN_BIG 450
#define CLAW_OPEN 1100
#define CLAW_CLOSED 1500
#define ARM 1
#define ARM_DOWN 370 //400
#define ARM_UP 1570 //1700
#define ARM_MID 850
#define PUSHER 2
#define PUSHER_CLOSED 2000
#define PUSHER_OPEN 1000
#define STACKER 0
#define STACKER_DOWN 300
#define STACKER_UP 1650
#define TOPHAT 5
#define TOPHAT_THRESHOLD 1000
#define LIGHT 0

// CAMERA

#define YELLOW_CHANNEL 0

Controller c;

void pickup() {
  c.servo(CLAW, CLAW_CLOSED);
  c.slow_servo(ARM, ARM_MID, 0.5);
  c.servo(CLAW, CLAW_OPEN);
  c.slow_servo(ARM, ARM_DOWN, 1);
  msleep(300);
  c.servo(CLAW, CLAW_CLOSED);
  msleep(400);
  c.slow_servo(ARM, ARM_UP, 2);
  c.slow_servo(CLAW, CLAW_OPEN,0.3);
}

void pickup_big() {
  c.servo(CLAW, CLAW_CLOSED);
  c.slow_servo(ARM, ARM_MID, 0.5);
  c.servo(CLAW, CLAW_OPEN_BIG);
  c.slow_servo(ARM, ARM_DOWN + 25, 1);
  msleep(800);
  c.servo(CLAW, CLAW_CLOSED);
  msleep(400);
  c.slow_servo(ARM, ARM_UP, 2);
  c.slow_servo(CLAW, CLAW_OPEN,0.3);
}

void pickup_raised() {
  c.servo(CLAW, CLAW_CLOSED);
  c.slow_servo(ARM, ARM_MID, 0.5);
  c.servo(CLAW, CLAW_OPEN);
  c.slow_servo(ARM, ARM_DOWN + 30, 1);
  msleep(300);
  c.servo(CLAW, CLAW_CLOSED);
  msleep(400);
  c.slow_servo(ARM, ARM_UP, 2);
  c.slow_servo(CLAW, CLAW_OPEN,0.3);
}

void push() {
  c.servo(ARM,ARM_MID);
  msleep(300);
  msleep(800);
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
  c.slow_servo(ARM, ARM_MID, 0.5);
  msleep(1000);
  c.servo(CLAW, CLAW_OPEN);
  c.slow_servo(ARM, ARM_DOWN, 1);
  msleep(200);
  c.slow_servo(CLAW, CLAW_CLOSED, 1);
  msleep(500);
  create.forward(4, 50);
  c.servo(CLAW, CLAW_OPEN);
  msleep(200);
  create.backward(2, 50);
  c.servo(CLAW, CLAW_CLOSED);
  msleep(200);
  c.slow_servo(ARM, ARM_UP, 2);
  c.slow_servo(CLAW, CLAW_OPEN,0.25);
  msleep(100);
}

void claw_down() {
  c.servo(CLAW,CLAW_OPEN);
  msleep(300);
}

// void camera_align();

void run_main_subroutine() {
  // Beginning routine

   float correction = 1.2;
  
  thread tid;
  tid = thread_create(push);
  
  // FIRST BLOCK

 
  create.left(correction * 20., 0, 50);
  create.left(correction * 72, 0, 150);
  create.backward(25, 200);
  create.left(87, 0, 250);
  create.drive_direct(-100, -100);
  msleep(1500);

  create.forward(5,150);

  create.right(87,0,150);
  create.drive_direct(-200, -200); //to the furrows
  msleep(2200);
  create.forward(1.5, 200);
  msleep(300);
  create.left(5, 0, 200);
  //create.right(1,0,20);
  pickup();  // grab 1st
  thread_start(tid); // start pushing
  
  // SECOND BLOCK

  create.right(6,0,50);
  create.drive_direct(-200, -200);
  msleep(500);
  create.right(4,0,20);
  create.forward(1,200);
  pickup();  // grab 2nd
  thread_wait(tid);
  thread_destroy(tid);
  
  tid = thread_create(push);
  thread_start(tid); // start pushing

  msleep(800);
  c.servo(CLAW, CLAW_CLOSED);
  c.servo(ARM, ARM_UP - 150);
  msleep(250);

  // THIRD BLOCK

  create.drive_direct(-100, -100); // square up
  msleep(750);
  create.forward(22,180);
  msleep(250);
  create.right(148,0,150);
  msleep(500);
  //create.right(3,0,20);
  //create.forward(1.5,200);
  pickup_big();// grab 3rd
  thread_wait(tid);
  thread_destroy(tid);
  
  tid = thread_create(push);
  thread_start(tid); // start pushing

  msleep(200);

  // FOURTH BLOCK

  create.right(33, 0, 150);
  create.backward(4, 150);
  create.right(92, 0, 250);
  create.drive_direct(-100, -100); // square up
  msleep(1000);
  create.forward(20, 200);
  create.left(20, 12, 200);
  create.right(16, 12, 200);
  create.forward(34, 200);
  create.right(62, 0, 200);
  create.backward(21,100);
  pickup_raised();  // grab 4th
  thread_wait(tid);
  thread_destroy(tid);
  
  tid = thread_create(push);
  thread_start(tid); // start pushing
  
  //camera_align(1);  //grab the 4th
  //push();

  // - SECOND HALF -

    create.forward(18,150);
    create.left(72,0,180);
    create.forward(65,180);
    create.right(90,0,180);
    create.drive_direct(-200, -200);
    msleep(2000);

    create.forward(29,180);
    create.right(87,0,180);
  /*
    create.drive_direct(-250, -250);
    msleep(2800);*/
    create.backward(correction * 35, 250);
    create.left(29, 0, 200);
    c.slow_servo(ARM, ARM_DOWN, 0.8);
    create.right(54, 0, 200);
    msleep(300);
    create.left(10, 0, 200);
    c.slow_servo(ARM, ARM_UP, 1.0);
    
    create.backward(36, 250);
    create.drive_direct(-150, -150);
    msleep(2800);

    // FIFTH BLOCK

    create.forward(6, 150);
    create.left(96, 0, 150);
    
    create.drive_direct(-100, -100);
    while(c.analog(TOPHAT) < TOPHAT_THRESHOLD) {
      msleep(1);
    }
    create.stop();
    msleep(500);
  
    create.forward(26, 150);
    create.right(90, 0, 200);
    
    create.drive_direct(-100, -100);
    msleep(1500);

    create.forward(27,180);
    create.left(88,0,180);
    create.forward(8,150);
    create.left(25,0,180);
    special_pickup();
    thread_wait(tid);
    thread_destroy(tid);
  
    msleep(500);
    tid = thread_create(push);
    thread_start(tid); // start pushing

    // SIXTH BLOCK

    create.right(17,0,180);

    special_pickup();  
    thread_wait(tid);
    thread_destroy(tid);
    tid = thread_create(push);
    thread_start(tid); // start pushing

    // SEVENTH BLOCK

    create.right(15,0,180);
    create.backward(6, 100);	//old value: 3
    special_pickup();
    thread_wait(tid);
    thread_destroy(tid);
  
    tid = thread_create(push);
    thread_start(tid); // start pushing
    create.left(10, 0, 200);
    create.forward(22, 200);
    c.servo(ARM, ARM_MID);
    msleep(200);

    c.slow_servo(STACKER, STACKER_UP, 5);
    create.backward(5, 50);
    create.forward(5, 50);
    create.backward(2, 50);
    create.forward(6, 50);
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

int main()
{
  c = new_create_controller();
  create = new_create();

  create.connect();

  c.servo(ARM,ARM_UP);
  c.servo(CLAW,CLAW_CLOSED);
  c.servo(PUSHER, PUSHER_OPEN);
  c.servo(STACKER,STACKER_DOWN);
  wait_for_light(LIGHT);
  shut_down_in(119);
  double start = seconds();
  c.enable_servos();

  int test = 0;
  if(test) {
      while(1) {
          printf("%d\n", c.analog(TOPHAT));
          msleep(300);
      }
  }
  else {
    run_main_subroutine();
  }

  create.disconnect();
  printf("\n\nTIME TAKEN: %f\n\n", seconds() - start);
  return 0;

}
