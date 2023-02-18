#include "main.h"
#include "guifuncs.h"
#include "teamlogo.c"
#include "led.h"
#include <string.h>
#include <math.h>

namespace LED
{

	int currentColor = ORANGE_ID;
	pros::ADIDigitalOut LED_strip_1_brightness({18,1},1);
	pros::ADILed LED_strip_1({18,1}, 44);
	int getCurrentColorID(){
		return currentColor;
	}
	void cycleColor() {
		currentColor = currentColor == RED_ID ? 0 : currentColor + 1;
		switch(currentColor) {
			case ORANGE_ID:
				for(int i = 0;i < 43;i++){
					LED_strip_1[i] = 0xff6400;
				}
				LED_strip_1.update();
				break;
			case BLUE_ID:
				for(int i = 0;i < 43;i++){
					LED_strip_1[i] = 0x1745ff;
				}
				LED_strip_1.update();
				break;
			case RED_ID:
				for(int i = 0;i < 43;i++){
					LED_strip_1[i] = 0xFF0000;
				}
				LED_strip_1.update();
				break;
		}
		GUI::updateColorInfo(currentColor,getControllerObj());
	}
	void startupColors(){
		LED_strip_1.clear_all();
		for(int i = 0;i < 43;i++){
			LED_strip_1[i] = 0xff6400;
			LED_strip_1.update();
			pros::delay(40);
		}
	}
}