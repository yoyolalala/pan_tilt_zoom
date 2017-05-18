#ifndef __SERVO_H
#define __SERVO_H
#include "ebox.h"
#define lowLimit 875
#define highLimit 975
class Servo
{
	Pwm pwm;
	uint32_t frq;
public:
	Servo(Gpio *signal);
	void begin();
	void setFrq(uint32_t frq);
	void setPct(float percent);//2.5%(975)µ½12.5%(875)
};
#endif