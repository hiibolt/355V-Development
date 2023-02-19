#include "main.h"
#include "pid.h"
#include <string.h>
#include <math.h>
struct img_button_options{
    const lv_img_dsc_t * src;
    lv_obj_t * page;
    lv_align_t alignto;
    int x_offset;
    int y_offset; 
    int id;
    lv_obj_t * btn_obj;
};
struct standard_button_options{
    const char * text;
    lv_obj_t * page;
    lv_align_t alignto;
    int x_offset; 
    int y_offset;
    int width;
    int height;
    int id;
    lv_obj_t * btn_obj;
    lv_obj_t * btn_label_obj;
};
enum BUTTON_NAMES{
    RIGHT_BTN_ID,LEFT_BTN_ID,
    AWP_BTN_ID,SHOOT_BTN_ID,
    SKILLS_BTN_ID,NOTHING_BTN_ID,
    
    DRIVE_BTN_ID,PID_BTN_ID,
    
    COLOR_BTN_ID,
    
    
    PID_HOME_BTN_ID,
    
    PID_KP_PLUS_BTN_ID,PID_KI_PLUS_BTN_ID,PID_KD_PLUS_BTN_ID,
    PID_KP_MINUS_BTN_ID,PID_KI_MINUS_BTN_ID,PID_KD_MINUS_BTN_ID,
    PID_INCREMENT_PLUS_BTN_ID,PID_INCREMENT_MINUS_BTN_ID,
    PID_TYPE_SWAP_BTN_ID
};
enum PAGE_NAMES{
    HOME_PAGE_ID,
    PID_PAGE_ID,
};
void createButton(const char * _text,lv_obj_t * _page, lv_align_t _alignto,int _x_offset, int _y_offset, int _width, int _height, int _id, lv_obj_t * _TEMPBUTTON, lv_obj_t * _TEMPBUTTONLABEL);
void createButton(const char * _text,lv_obj_t * _page, lv_align_t _alignto,int _x_offset, int _y_offset, int _width, int _height, int _id, lv_obj_t * _TEMPBUTTON, lv_obj_t * _TEMPBUTTONLABEL);
namespace GUI{
    void updateDriveInfo(int driveID, Controller controller);
    void updateAutonInfo(int PIDID, Controller controller);
    void updateColorInfo(int colorID, Controller controller);
    void swapPage(int page);
    int  getPage();
    void buildStyles();
    void buildMainPage();
    void buildPIDPage();
}
static lv_res_t onClick(lv_obj_t * btn);