#include "ebox.h"
#include "led.h"
#include "olediic.hpp"
#include "MPU6050.h"
#include "PTZ.h"
#define PID_REFRESH_INTERVAL 0.01
Ptz ptz(&PA9,&PA6,&i2c1,PID_REFRESH_INTERVAL);
Oled oled(&PB6, &PB7);
Mpu6050 mpu(&i2c1);//	PB6 PB7 -scl sda
Servo es3001(&PA9);
Led led(&PC13,1);
void setup()
{
	ebox_init();
	led.begin();
	uart1.begin(115200);
	es3001.begin();
	ptz.begin();
	oled.OLED_Init();
	delay_ms(200);
	oled.OLED_CLS();
}
void main()
{
	setup();
	oled.OLED_printf(0, 0, "%f,%f", roll, pitch);
	ptz.refresh();
	while(1)
	{
		led.toggle();
		delay_ms(500);
	}
}