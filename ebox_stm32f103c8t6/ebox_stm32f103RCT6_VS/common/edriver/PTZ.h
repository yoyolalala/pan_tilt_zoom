#ifndef __PTZ_H
#define __PTZ_H
#include "ebox.h"
#include "servo.h"
#include "pid.hpp"
#include "uart_num.h"
#include "math.h"
UartNum uartNum(&uart1);
class Ptz
{   
	greg::PID pitchPID,rollPID;
	Servo servoPitch,servoRoll;
	float refreshInt;
	void getData()
	{
		uartNum.begin(500000);
		for (int i = 0; i < uartNum.recievedNum; i++)
		{
			if (i%uartNum.recievedNum == 1)
			{
				pitch =uartNum.numsBuf[i];//1
			}
			if (i%uartNum.recievedNum == 2)
			{
				roll =uartNum.numsBuf[i];//2
			}
		}
	}
	//�����Զ�������Ƕȵ�PID����refresh�����������̬�Ķ�ȡ
	void refreshPID()
	{
		float pitchPercent = 40, rollPercent=40;
		//��ȡmpu���ص�roll��pitchֵ
		this->getData();

		//��������PID�����������+-�Ըı���Ʒ���
		pitchPercent -= pitchPID.refresh(pitch);
		rollPercent -= rollPID.refresh(roll);

		//�������Ƕ�����
		servoPitch.setPct(pitchPercent);
		servoRoll.setPct(rollPercent);
	}
	//����PID�ۼ�ֵ ��ʱ�ò��� ��ע�͵�
	//void resetPID()
	//{
	//	pitchPID.reset();
	//	rollPID.reset();
	//	pitchPID.setDesiredPoint(6.33);
	//	rollPID.setDesiredPoint(18.33);
	//}
public:
	float roll, pitch;
	Ptz(Gpio *servoPitchx, Gpio *servoRollx, float refreshInterval) :
		servoPitch(servoPitchx,200,0.7,2.4),servoRoll(servoRollx),refreshInt(refreshInterval),roll(0),pitch(0)
	{

	}
	void begin()
	{
		servoPitch.begin();
		servoRoll.begin();
		pitchPID.setRefreshInterval(refreshInt);
		pitchPID.setWeights(0.8, 1.5, 0);
		pitchPID.setOutputLowerLimit(0);
		pitchPID.setOutputUpperLimit(100);
		pitchPID.setDesiredPoint(6.33);

		rollPID.setRefreshInterval(refreshInt);
		rollPID.setWeights(0, 0, 0);
		rollPID.setOutputLowerLimit(0);
		rollPID.setOutputUpperLimit(100);
		rollPID.setDesiredPoint(18.33);
	}
	void showData()
	{
		this->getData();
		uartNum.printf("%f,%f", roll,pitch);
	}
	void refresh()
	{   
		//ˢ��mpu����
		this->getData();
		//ˢ��PID
		this->refreshPID(); 
	}
};
#endif