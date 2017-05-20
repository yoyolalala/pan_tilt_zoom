#include "ebox.h"
#include "led.h"
#include "olediic.hpp"
#include "PTZ.h"
#define PID_REFRESH_INTERVAL 0.01
Ptz ptz(&PB8, &PA6, PID_REFRESH_INTERVAL);
Oled oled(&PB6, &PB7);//scl sda
Led led(&PC13,1);
void setup()
{
	ebox_init();
	led.begin();
	ptz.begin();

	oled.OLED_Init();
	delay_ms(200);
	oled.OLED_CLS();
}
void main()
{
	setup();
	while(1)
	{ 
		oled.OLED_printf(0, 0, "roll=%f", ptz.roll);
	    oled.OLED_printf(2, 2, "pitch=%f",ptz.pitch);
		ptz.refresh();
		led.toggle();
		delay_ms(10);
	}
}