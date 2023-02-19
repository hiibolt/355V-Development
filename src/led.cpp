#include "main.h"
#include "guifuncs.h"
#include "teamlogo.c"
#include "led.h"
#include <string.h>
#include <math.h>

namespace LED
{

	int currentColor = ORANGE_ID;
	// fix trash code
	pros::ADIDigitalOut LEDStrip1Brightness({11,'H'}, HIGH);
	pros::ADILed LEDStrip1({11,'H'}, 44);
	pros::ADIDigitalOut LEDStrip2Brightness({12,8}, HIGH);
	pros::ADILed LEDStrip2({12,8}, 44);
	int getCurrentColorID(){
		return currentColor;
	}
	void cycleColor() {
		currentColor = currentColor == RED_ID ? 0 : currentColor + 1;
		
		switch(currentColor) {
			case ORANGE_ID:
				for(int i = 0;i < 43;i++){
					LEDStrip1[i] = 0xff6400;
					LEDStrip2[i] = 0xff6400;
				}
				LEDStrip1.update();
				LEDStrip2.update();
				break;
			case BLUE_ID:
				for(int i = 0;i < 43;i++){
					LEDStrip1[i] = 0x1745ff;
					LEDStrip2[i] = 0x1745ff;
				}
				LEDStrip1.update();
				LEDStrip2.update();
				break;
			case RED_ID:
				for(int i = 0;i < 43;i++){
					LEDStrip1[i] = 0xFF0000;
					LEDStrip2[i] = 0xFF0000;
				}
				LEDStrip1.update();
				LEDStrip2.update();
				break;
		}
		GUI::updateColorInfo(currentColor,getControllerObj());
	}
	void startupColors(){
		LEDStrip1.clear_all();
		LEDStrip2.clear_all();
		for(int i = 0;i < 43;i++){
			LEDStrip1[i] = 0xff6400;
			LEDStrip2[i] = 0xff6400;
			LEDStrip1.update();
			LEDStrip2.update();
			pros::delay(40);
		}
	}
}