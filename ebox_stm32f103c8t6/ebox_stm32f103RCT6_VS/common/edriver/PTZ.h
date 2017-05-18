#ifndef __PTZ_H
#define __PTZ_H
#include "ebox.h"
#include "servo.h"
#include "pid.hpp"
#define roll 1 
#define pitch 2//暂时用作表示九轴读得数据
class Ptz
{
	greg::PID pitchPID,rollPID;
	I2c *i2c;
	Servo servoPitch,servoRoll;
	float refreshInt;
	//单独对舵机两个角度的PID进行refresh，包括目标角度的设置
	void refreshPID()
	{
		float pitchPercent = 0, rollPercent=0;
		//获取mpu返回的roll和pitch值
		

		//计算两个PID的输出，设置+-以改变控制方向
		pitchPercent -= pitchPID.refresh(pitch);
		rollPercent -= rollPID.refresh(roll);

		//输出舵机角度增量
		servoPitch.setPct(pitchPercent);
		servoRoll.setPct(rollPercent);
	}
	//重置PID累计值
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
		//刷新mpu读数


		//刷新PID
		this->refreshPID();
	}
};
#endif