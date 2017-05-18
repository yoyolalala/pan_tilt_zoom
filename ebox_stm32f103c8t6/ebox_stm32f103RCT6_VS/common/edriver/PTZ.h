#ifndef __PTZ_H
#define __PTZ_H
#include "ebox.h"
#include "servo.h"
#include "pid.hpp"
#define roll 1 
#define pitch 2//��ʱ������ʾ�����������
class Ptz
{
	greg::PID pitchPID,rollPID;
	I2c *i2c;
	Servo servoPitch,servoRoll;
	float refreshInt;
	//�����Զ�������Ƕȵ�PID����refresh������Ŀ��Ƕȵ�����
	void refreshPID()
	{
		float pitchPercent = 0, rollPercent=0;
		//��ȡmpu���ص�roll��pitchֵ
		

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
	Ptz(Gpio *servoPitchx, Gpio *servoRollx, I2c *i2cx, float refreshInterval) :
		servoPitch(servoPitchx),servoRoll(servoRollx),refreshInt(refreshInterval),i2c(i2cx)
	{

	}
	void begin()
	{
		servoPitch.begin();
		servoRoll.begin();
		pitchPID.setRefreshInterval(refreshInt);
		pitchPID.setWeights(0, 0, 0);
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


		//ˢ��PID
		this->refreshPID();
	}
};
#endif