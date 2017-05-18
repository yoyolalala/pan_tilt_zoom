#ifndef __PTZ_H
#define __PTZ_H
#include "ebox.h"
#include "servo.h"
#include "pid.hpp"
#include "uart_num.h"
UartNum uartNum(&uart1);
class Ptz
{   
	greg::PID pitchPID,rollPID;
	Servo servoPitch,servoRoll;
	float refreshInt;
	void getData()
	{
		uartNum.begin(115200);
		for (int i = 0; i < uartNum.recievedNum; i++)
		{
			roll = uartNum.numsBuf[2];
			pitch = uartNum.numsBuf[1];
		}
	}
	//�����Զ�������Ƕȵ�PID����refresh�����������̬�Ķ�ȡ
	void refreshPID()
	{
		float pitchPercent = 50, rollPercent=50;
		//��ȡmpu���ص�roll��pitchֵ
		this->getData();

		//��������PID�����������+-�Ըı���Ʒ���
		pitchPercent -= pitchPID.refresh(pitch);
		rollPercent -= rollPID.refresh(roll);

		//�������Ƕ�����
		servoPitch.setPct(pitchPercent);
		servoRoll.setPct(rollPercent);
	}
	//����PID�ۼ�ֵ
	void resetPID()
	{
		pitchPID.reset();
		rollPID.reset();
		pitchPID.setDesiredPoint(50);
		rollPID.setDesiredPoint(50);
	}

public:
	float roll, pitch;
	Ptz(Gpio *servoPitchx, Gpio *servoRollx, float refreshInterval) :
		servoPitch(servoPitchx),servoRoll(servoRollx),refreshInt(refreshInterval),roll(0),pitch(0)
	{

	}
	void begin()
	{
		servoPitch.begin();
		servoRoll.begin();
		pitchPID.setRefreshInterval(refreshInt);
		pitchPID.setWeights(8, 0, 0);
		pitchPID.setOutputLowerLimit(-50);
		pitchPID.setOutputUpperLimit(50);
		pitchPID.setDesiredPoint(50);

		rollPID.setRefreshInterval(refreshInt);
		rollPID.setWeights(0, 0, 0);
		rollPID.setOutputLowerLimit(-50);
		rollPID.setOutputUpperLimit(50);
		rollPID.setDesiredPoint(50);
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