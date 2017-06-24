#include "Controller.h"
#include <math.h>

static void forward(float dist, int speed);
static void backward(float dist, int speed);
static void left(int angle, float radius, int speed);
static void right(int angle, float radius, int speed);

/**
 * Converts from centimeters to BackEMF ticks (robot measurement).
 * @param  cm a distance in centimeters that you want to convert
 * @return    the parameter `cm` in BackEMF ticks
 */
long CMtoBEMF(float cm) {
 	return (long)(cm * 1400. / (M_PI * controller.wheel_diameter));
}

/**
 * Converts from BackEMF ticks to centimeters.
 * @param  ticks a distance in BackEMF ticks that you want to convert
 * @return       the parameter `ticks` in centimeters
 */
float BEMFtoCM(long ticks) {
    return (float)(ticks * (M_PI * controller.wheel_diameter) / 1400.);
}

static void forward(float dist, int speed) {
    if(dist < 0.) {
    	backward(-dist, speed);
      	return;
    }

    cmpc(controller.motor_left);
    cmpc(controller.motor_right);
	float left_cm = 0;
  	float right_cm = 0;
    float left_p = 1.0;
    float right_p = 1.0;
    float inc = 0.005 * (float)speed / 1100.0;
  	// Keep moving until both motors reach their desired distance
	while(abs(left_cm) < abs(dist) && abs(right_cm) < abs(dist)) {
        left_cm = BEMFtoCM(gmpc(controller.motor_left));
        right_cm = BEMFtoCM(gmpc(controller.motor_right));
        if(left_cm == right_cm) {
            left_p = 1.0;
            right_p = 1.0;
        }
        else if(left_cm > right_cm) {
            right_p = 1.0;
            left_p -= inc;
        }
        else if(right_cm > left_cm) {
            left_p = 1.0;
            right_p -= inc;
        }
        
		if (left_cm < dist) {
            mav(controller.motor_left, left_p * (float)speed);
        }
        else {
			freeze(controller.motor_left);
        }
        
        if (right_cm < dist) {
            mav(controller.motor_right, right_p * (float)speed);
        }
        else {
			freeze(controller.motor_right);
        }      
        msleep(1);
 	}

	off(controller.motor_left);
  	off(controller.motor_right);
}

static void forward_with_speed(float dist, int left_speed, int right_speed) {
    if(dist < 0.) {
    	backward(-dist, left_speed);
      	return;
    }

  	// Calculate the # of ticks the robot must move for each wheel
	long ticks = CMtoBEMF(dist);
	long totalLeftTicks = gmpc(controller.motor_left) + ticks;
	long totalRightTicks = gmpc(controller.motor_right) + ticks;

  	// Start motors
	motor(controller.motor_left, left_speed);
	motor(controller.motor_right, right_speed);

  	// Keep moving until both motors reach their desired # of ticks
	while(gmpc(controller.motor_left) < totalLeftTicks
          && gmpc(controller.motor_right) < totalRightTicks) {
		if (gmpc(controller.motor_left) >= totalLeftTicks)
			off(controller.motor_left);
		if (gmpc(controller.motor_right) >= totalRightTicks)
			off(controller.motor_right);
	}

	off(controller.motor_left);
  	off(controller.motor_right);
}

static void backward(float dist, int speed) {
    if(dist < 0.) {
    	forward(-dist, speed);
      	return;
    }

    cmpc(controller.motor_left);
    cmpc(controller.motor_right);
	float left_cm = 0;
  	float right_cm = 0;
    float left_p = 1.0;
    float right_p = 1.0;
    float inc = 0.005 * (float)speed / 1100.0;
  	// Keep moving until both motors reach their desired distance
	while(abs(left_cm) < dist && abs(right_cm) < dist) {
        left_cm = BEMFtoCM(gmpc(controller.motor_left));
        right_cm = BEMFtoCM(gmpc(controller.motor_right));
        if(left_cm == right_cm) {
            left_p = 1.0;
            right_p = 1.0;
        }
        else if(abs(left_cm) > abs(right_cm)) {
            right_p = 1.0;
            left_p -= inc;
        }
        else if(abs(right_cm) > abs(left_cm)) {
            left_p = 1.0;
            right_p -= inc;
        }
        
		if (abs(left_cm) < dist) {
            mav(controller.motor_left, -left_p * speed);
        }
        else {
			freeze(controller.motor_left);
        }
        
        if (abs(right_cm) < dist) {
            mav(controller.motor_right, -right_p * speed);
        }
        else {
			freeze(controller.motor_right);
        }      
        msleep(1);
 	}

	off(controller.motor_left);
  	off(controller.motor_right);
}

static void left_in_place(int angle, int speed) {
    int radius = controller.distance_between_wheels / 2;
    
    float fraction_of_turn = ((float)angle / 360.);
    printf("Fraction of turn: %f\n", fraction_of_turn);
    
    float wheel_distance = 2. * fraction_of_turn * M_PI * radius;
    
    controller.cmpc(controller.motor_left);
    controller.cmpc(controller.motor_right);
  
    controller.motor(controller.motor_left, -speed);
    controller.motor(controller.motor_right, speed);
    
    long ticks = CMtoBEMF(wheel_distance);
    while(gmpc(controller.motor_left) > -ticks && gmpc(controller.motor_right) < ticks);
      
    off(controller.motor_left);
    off(controller.motor_right);
}

static void right_in_place(int angle, int speed) {
    int radius = controller.distance_between_wheels / 2;
    
    float fraction_of_turn = ((float)angle / 360.);
    printf("Fraction of turn: %f\n", fraction_of_turn);
    
    float wheel_distance = 2. * fraction_of_turn * M_PI * radius;
    
    controller.cmpc(controller.motor_left);
    controller.cmpc(controller.motor_right);
  
    controller.motor(controller.motor_left, speed);
    controller.motor(controller.motor_right, -speed);
    
    long ticks = CMtoBEMF(wheel_distance);
    while(gmpc(controller.motor_right) > -ticks && gmpc(controller.motor_left) < ticks);
      
    off(controller.motor_left);
    off(controller.motor_right);
}

static void left(int angle, float radius, int speed) {
    // calculate radii
    float left_radius = radius;
    float right_radius = radius + controller.distance_between_wheels;

    if(left_radius < 0)
        return;
    if(right_radius <= 0)
        return;

    // calculate distance in CM
    float right_distance = (right_radius * M_PI) * ((float)(angle) / 180.);

    // calculate relative velocity

    int left_speed = left_radius * (float)speed / right_radius;
    int right_speed = speed;

    long right_distance_ticks = CMtoBEMF(right_distance);

    // clear motor tick counter
    controller.cmpc(controller.motor_left);
    controller.cmpc(controller.motor_right);

    // power motors

    controller.motor(controller.motor_left, left_speed);
    controller.motor(controller.motor_right, right_speed);

    while(abs(controller.gmpc(controller.motor_right)) <= abs(right_distance_ticks)) {
        msleep(50);
    }

    off(controller.motor_left);
    off(controller.motor_right);
}


static void right(int angle, float radius, int speed) {
	// calculate radii
    float left_radius = radius + controller.distance_between_wheels;
    float right_radius = radius;

    if(left_radius <= 0)
        return;
    if(right_radius < 0)
        return;

    // calculate distance in CM
    float left_distance = (left_radius * M_PI) * ((float)(angle) / 180.);

    // calculate relative velocity

    int left_speed = speed;
    int right_speed = right_radius * (float)speed / left_radius;

    long left_distance_ticks = CMtoBEMF(left_distance);

    // clear motor tick counter
    controller.cmpc(controller.motor_left);
    controller.cmpc(controller.motor_right);

    // power motors

    controller.motor(controller.motor_left, left_speed);
    controller.motor(controller.motor_right, right_speed);

    while(abs(controller.gmpc(controller.motor_left)) <= abs(left_distance_ticks)) {
        msleep(50);
    }

    off(controller.motor_left);
    off(controller.motor_right);
}

// FIX THIS
static void slow_servo(int port, int position, float time) {
    float increment = .01;
	float curr, start = controller.get_servo_position(port);
	float i = ((position - start) / time) * increment;
	curr = start;
	if (start > position)
	{
		while(curr > position)
		{
			controller.servo(port, curr);
			curr += i;
			msleep((long)(increment * 1000));
		}
	}
	else if (start < position)
	{
		while(curr < position)
		{
			controller.servo(port, curr);
			curr += i;
			msleep((long)(increment * 1000));
		}
	}
	controller.servo(port, position);
}

// Constructors

Controller new_controller(int motor_left, int motor_right, float distance_between_wheels, float wheel_diameter) {
	Controller instance = {

        // Instance Variables (with no setters)
        .distance_between_wheels = distance_between_wheels,
        .wheel_diameter = wheel_diameter,

        // Assign method references

        .motor_left = motor_left,
        .motor_right = motor_right,
        .forward = &forward, .backward = &backward,
        .left = &left, .right = &right,
        .left_in_place = &left_in_place,
        .right_in_place = &right_in_place,
        .forward_with_speed = &forward_with_speed,
        .motor = &motor,
        .mav = &mav, .mtp = &mtp, .mrp = &mrp,
        .stop = &ao,
        .motor_off = &off,
        .gmpc = &gmpc,
        .clear_motor_position_counter = &cmpc,
        .gmpc = &gmpc, .cmpc = &cmpc,
        .enable_servo = &enable_servo,
        .disable_servo = &disable_servo,
        .enable_servos = &enable_servos,
        .disable_servos = &disable_servos,
        .get_servo_position = &get_servo_position,
        .servo = &set_servo_position,
        .slow_servo = &slow_servo,
        .digital = &digital,
        .analog = &analog,
        .analog10 = &analog10,
        .analog_et = &analog_et
    };

    controller = instance;

    return instance;
}

Controller new_create_controller() {
	Controller instance = {
        // Assign method references
        .motor = &motor,
        .mav = &mav, .mtp = &mtp, .mrp = &mrp,
        .stop = &ao,
        .motor_off = &off,
        .gmpc = &gmpc,
        .clear_motor_position_counter = &cmpc,
        .gmpc = &gmpc, .cmpc = &cmpc,
        .enable_servo = &enable_servo,
        .disable_servo = &disable_servo,
        .enable_servos = &enable_servos,
        .disable_servos = &disable_servos,
        .get_servo_position = &get_servo_position,
        .servo = &set_servo_position,
        .slow_servo = &slow_servo,
        .digital = &digital,
        .analog = &analog,
        .analog10 = &analog10,
        .analog_et = &analog_et
    };
    controller = instance;
    return instance;
}