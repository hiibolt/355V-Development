#include "main.h"
#include "guifuncs.h"
#include "teamlogo.c"
#include "led.h"
#include <string.h>
#include <math.h>

namespace LED{

	int currentColor = ORANGE_ID;
	// fix trash code
	pros::ADIDigitalOut LEDStrip1Brightness({11,'H'}, HIGH);
	pros::ADILed LEDStrip1({11,'H'}, 44);
	pros::ADIDigitalOut LEDStrip2Brightness({12,8}, HIGH);
	pros::ADILed LEDStrip2({12,8}, 44);
	int getCurrentColorID(){
		return currentColor;
	}
	int getCurrentColorHex(){
		switch(currentColor) {
			case ORANGE_ID:
				return 0xff6400;
				break;
			case BLUE_ID:
				return 0x1745ff;
				break;
			case RED_ID:
				return 0xFF0000;
				break;
		}
		return -1;
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
		GUI::updateColorInfo(currentColor,controller);
	}
	void startupColors(){
		LEDStrip1.clear_all();
		LEDStrip2.clear_all();
		for(int i = 0;i < 43 + 0;i++){
			LEDStrip1[43-i] = 0xff6400;
			LEDStrip2[43-i] = 0xff6400;
			LEDStrip1.update();
			LEDStrip2.update();
			pros::delay(40);
		}
	}
	void updateColorStrips(std::pair<int, int> indexes, int color){
		LEDStrip1.clear_all();
		LEDStrip2.clear_all();
		for(int i = 0;i < 43;i++){
			if(i >= indexes.first && i <= indexes.second){
				LEDStrip1[i] = color;
				LEDStrip2[i] = color;
			}else{
				LEDStrip1[i] = getCurrentColorHex();
				LEDStrip2[i] = getCurrentColorHex();
			}
		}
		pros::delay(15);
		LEDStrip1.update();
		LEDStrip2.update();
	}
}