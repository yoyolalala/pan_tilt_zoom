#ifndef __SERVO_H
#define __SERVO_H
#include "ebox.h"

//�������
//֧�ֶ˿ڣ�    
//TIM1 PA8  PA9  PA10 PA11 
//TIM2 PA0  PA1  PA2  PA3
//TIM3 PA6  PA7  PA0  PA1
//TIM4 PB6  PB7  PB8  PB9
class Servo
{
	Pwm pwm;
	uint32_t frq;
	uint16_t limLow,limHigh;

	template<typename T>
	static void limit(T &num, T limL, T limH);

public:

	//�����������Ĭ��Ƶ��50Hz
	//��׼50Hzʱ��ռ�ձȷ�Χ3%~13%
	//��׼����ߵ�ƽ��Χ0.5ms~2.5ms
	//�˴�ȡlimLowMs=0.6ms~limHighMs=2.4ms����һЩ����
	Servo(Gpio* pin, uint32_t frequency = 50, float limLowMs = 0.6, float limHighMs = 2.4);

	//��ʼ��pwm
	void begin();

	//���ðٷֱ�
	void setPct(float percent);
};

template<typename T>
void Servo::limit(T &num, T limL, T limH)
{
	if (num < limL)
	{
		num = limL;
	}
	else if (num > limH)
	{
		num = limH;
	}
}

#endif