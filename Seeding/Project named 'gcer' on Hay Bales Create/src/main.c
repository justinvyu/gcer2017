#include <kipr/botball.h>

#define CLAW 3
#define CLAW_OPEN 490
#define CLAW_CLOSED 1500
#define ARM 1
#define ARM_DOWN 400
#define ARM_UP 1700
#define ARM_MID 850
#define PUSHER 2
#define PUSHER_IN 2000
#define PUSHER_OUT 1000
#define BOX 0
#define BOX_DOWN 250
#define BOX_UP 1500


#define ssp slow_servo

void slow_servo(int port, int position, float time) {
    float increment = .01;
	float curr, start = get_servo_position(port);
	float i = ((position - start) / time) * increment;
	curr = start;
	if (start > position)
	{
		while(curr > position)
		{
			set_servo_position(port, curr);
			curr += i;
			msleep((long)(increment * 1000));
		}
	}
	else if (start < position)
	{
		while(curr < position)
		{
			set_servo_position(port, curr);
			curr += i;
			msleep((long)(increment * 1000));
		}
	}
	set_servo_position(port, position);
}

int main()
{
    set_servo_position(PUSHER, PUSHER_OUT);
    set_servo_position(CLAW, CLAW_CLOSED);
    set_servo_position(ARM, ARM_UP - 200);
    set_servo_position(BOX, BOX_DOWN);
    enable_servos();
    int i;
    for(i = 0; i < 7; i++) {
      	set_servo_position(CLAW, CLAW_CLOSED);
        ssp(ARM, ARM_MID, 0.75);
        msleep(1000);
        set_servo_position(CLAW, CLAW_OPEN);
        ssp(ARM, ARM_DOWN, 1);
  	    msleep(200);
        set_servo_position(CLAW, CLAW_CLOSED);
        msleep(1000);
        ssp(ARM, ARM_UP, 2);
        ssp(CLAW, CLAW_OPEN,0.3);
        msleep(1000);
        ssp(PUSHER, PUSHER_IN, 0.5);
        ssp(PUSHER, PUSHER_OUT, 0.5);
        ssp(PUSHER, PUSHER_IN, 0.2);
        ssp(PUSHER, PUSHER_OUT, 0.2);
        msleep(300);
    }
  	msleep(5000);
    set_servo_position(ARM, ARM_MID);
    ssp(BOX, BOX_UP,3);
    msleep(1000);
    disable_servos();
    return 0;
}
