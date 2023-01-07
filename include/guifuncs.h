#pragma once
#include "main.h"
#include "pid.h"
#include <string.h>
#include <math.h>
void createButton(const char * _text,lv_obj_t * _page, lv_align_t _alignto,int _x_offset, int _y_offset, int _width, int _height, int _id, lv_obj_t * _TEMPBUTTON, lv_obj_t * _TEMPBUTTONLABEL);
void createButton(const char * _text,lv_obj_t * _page, lv_align_t _alignto,int _x_offset, int _y_offset, int _width, int _height, int _id, lv_obj_t * _TEMPBUTTON, lv_obj_t * _TEMPBUTTONLABEL);
void buildStyles();
void buildMainPage();
void buildPIDPage();
static lv_res_t onClick(lv_obj_t * btn);
void swapPage(int page);