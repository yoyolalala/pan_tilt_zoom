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
		uartNum.begin(500000);
		for (int i = 0; i < uartNum.recievedNum; i++)
		{
			if (i%uartNum.recievedNum == 1)
			{
				pitch = uartNum.numsBuf[i];//1
			}
			if (i%uartNum.recievedNum == 2)
			{
				roll = uartNum.numsBuf[i];//2
			}
		}
	}
	//单独对舵机两个角度的PID进行refresh，包括相机姿态的读取
	void refreshPID()
	{
		float pitchPercent = 50, rollPercent=50;
		//获取mpu返回的roll和pitch值
		this->getData();

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
		pitchPID.setDesiredPoint(87.33);
		rollPID.setDesiredPoint(18.33);
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
		pitchPID.setWeights(1, 0, 0);
		pitchPID.setOutputLowerLimit(-50);
		pitchPID.setOutputUpperLimit(50);
		pitchPID.setDesiredPoint(87.33);

		rollPID.setRefreshInterval(refreshInt);
		rollPID.setWeights(0, 0, 0);
		rollPID.setOutputLowerLimit(-50);
		rollPID.setOutputUpperLimit(50);
		rollPID.setDesiredPoint(18.33);
	}
	void showData()
	{
		this->getData();
		uartNum.printf("%f,%f", roll,pitch);
	}
	void refresh()
	{   
		//刷新mpu读数
		this->getData();
		//刷新PID
		this->refreshPID();
	}
};
#endif