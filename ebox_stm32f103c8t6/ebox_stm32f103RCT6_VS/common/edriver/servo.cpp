#include "servo.h"

Servo::Servo(Gpio *signal) :
	pwm(signal), frq(50)
{

}

void Servo::begin()
{
	pwm.begin(50, 75);
	pwm.set_oc_polarity(1);
}

void Servo::setFrq(uint32_t frq)
{
	pwm.set_frq(frq);
}

void Servo::setPct(float percent) //2.5%(975)µ½12.5%(875)
{
	uint16_t pct = percent + 875;
	if (pct >= lowLimit || pct <= highLimit)
		pwm.set_duty(pct);
	else if (pct < lowLimit)
		pwm.set_duty(lowLimit);
	else pwm.set_duty(highLimit);
}