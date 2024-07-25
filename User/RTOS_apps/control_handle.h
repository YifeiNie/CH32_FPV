#ifndef  CONTROL_HANDLE_H
#define CONTROL_HANDLE_H

#include "FreeRTOS.h"
#include "task.h"
#include "control_handle.h"
#include "pwm.h"
#include "pid.h"
#include "Crsf.h"
#include "IMU_handle.h"
#include "../General_Files/drivers/MTF01.h"

/*璋冨害鍙傛暟*/
#define CONRTOL_PERIOD  10  //鎺у埗鍛ㄦ湡锛屽崟浣峬s

extern void control_handle_task(void *pvParameters);


/*鎺у埗鍙傛暟*/

#define single_Fly_Pitch_Zero  0.0f
#define single_Fly_Roll_Zero   0.0f
#define single_Fly_Yaw_Zero    0.0f



#define ELRS_Throttle       CrsfChannels[2]
#define ELRS_Pitch          CrsfChannels[1]
#define ELRS_Yaw            CrsfChannels[3]
#define ELRS_Roll           CrsfChannels[0]

#define ELRS_mode           CrsfChannels[6]
#define ELRS_Control_mode   CrsfChannels[5]
#define ELRS_Throttle_lock  CrsfChannels[4]
//#define ELRS_XXX            CrsfChannels[5]
//#define ELRX_XXX            CrsfChannels[7]

//鎺у埗妯″紡
enum
{
    PID_CONTROL_MODE = 1,   //鐢垫満杞?閫熶负PID杈撳嚭
    RAW_CONTROL_MODE,       //鐢垫満杞?閫熺洿鎺ヤ负娌归棬杈撳叆锛坉ebug妯″紡锛?
    STABLE_CONTROL_MODE     //涓嶅彲鎺э紝鑷?绋虫ā寮?
};

//鐢垫満妯″紡
enum
{
    MOTOR_NORMAL = 1,       //姝ｅ父妯″紡
    MOTOR_SOFT_STARTING     //姝ｅ湪缂撳惎鍔ㄤ腑
};

//鍏ㄥ眬鍙橀噺
typedef struct
{
    float Mech_zero_yaw;    // yaw杞存満姊伴浂鐐癸紝鍥犱负闇�瑕佹洿鏂版墍浠ユ槸鍙橀噺
    u8 is_locked;           // 鐢垫満閿?
    u8 flight_mode;         //椋炶?屾ā寮?
    u8 is_landing;          //鑷?鍔ㄩ檷钀?

    PID_STRUCT PID_yaw_outerloop;
    PID_STRUCT PID_yaw_innerloop;
    PID_STRUCT PID_pitch_outerloop;
    PID_STRUCT PID_pitch_innerloop;
    PID_STRUCT PID_roll_outerloop;
    PID_STRUCT PID_roll_innerloop;
    PID_STRUCT MTF01_roll_outerloop;
    PID_STRUCT MTF01_roll_innerloop;
    PID_STRUCT MTF01_pitch_outerloop;
    PID_STRUCT MTF01_pitch_innerloop;
    PID_STRUCT MTF01_height_positionloop;

    uint16_t PWM_Out1;         // 鏈�缁堜綔鐢ㄥ埌鐢垫満1鐨凱WM
    uint16_t PWM_Out2;         // 鏈�缁堜綔鐢ㄥ埌鐢垫満2鐨凱WM
    uint16_t PWM_Out3;         // 鏈�缁堜綔鐢ㄥ埌鐢垫満3鐨凱WM
    uint16_t PWM_Out4;         // 鏈�缁堜綔鐢ㄥ埌鐢垫満4鐨凱WM

    float Yaw;
    float Pitch;
    float Roll;
    float MTF01_roll_agnle;
    float MTF01_pitch_agnle;
    uint16_t Throttle;
    int CONTROL_MODE;    //鎺у埗妯″紡璁惧畾
    int MOTOR_MODE;      //鐢垫満妯″紡璁惧畾
}Control_TypeDef;


//鐢垫満缂撳惎鍔ㄧ浉鍏?
#define SOFT_START_TIME 300 //缂撳惎鍔ㄦ椂闂达紝ms

#define IMU_SAMPLE_SIZE 4 //IMU骞冲潎鍊兼护娉㈠櫒澶у皬

// 绉?鍒?
#define Angle_I_Limit 200
#define Gyro_I_Limit  200

// ELRS鏁版嵁杞?鎹㈠埌瑙掑害鏁版嵁锛欵LRS_data*ELRS2angle=angle锛?30/(1811-1000)=0.037
#define ELRS2angle    0.06
// ELRS鏁版嵁杞?鎹㈠埌娌归棬鏁版嵁锛欵LRS_data*ELRS2throttle=throttle锛?1440/1711=0.8416
#define ELRS2throttle 0.55  // 涓嶆槸0.8419鏄?鍥犱负鐣欎竴鐐规补闂ㄧ粰椋炴満璋冩暣濮挎�?
//#define ELRS2throttle 0.92  // 涓嶆槸0.8419鏄?鍥犱负鐣欎竴鐐规补闂ㄧ粰椋炴満璋冩暣濮挎�?


// 鏈�澶у�炬枩瑙掑害锛岃繕鏈?鎹㈢畻
#define MAX_ROLL_ANGLE  20
#define MAX_PITCH_ANGLE 20

// 鏈烘?伴浂鐐癸紝闇�瑕佽皟
#define Mech_zero_pitch  0
#define Mech_zero_roll   0


// 闇�瑕佺粰椋炴満濮挎�佽皟鏁撮?勭暀PWM锛屾墍浠ユ补闂ㄤ负100鏃朵笉鑳借揪鍒?7200鍗犵┖姣?
// 娌归棬杈惧埌100鏃讹紝PWM涓? 100*PWM_OIL
#define PWM_OIL 45

// 澶勪簬Locked鐘舵�佹椂锛屾补闂ㄥ拰pid琚?绂佺敤
#define Locked    1
#define Unlocked  0

// 椋炶?屾ā寮忥紝鏃犳帶鍒讹紝鑷?绋筹紝GPS妯″紡
#define Free    0
#define Stable  1
#define GPS     2

//璋冭瘯鏋跺瓙鏈夐樆灏硷紝璋冭瘯鏋舵祴鍑烘潵鐨勫弬鏁伴渶瑕佺粰涓�涓?琛板噺
#define damp_rate   0.55

// 鏈哄姩锛岄檷钀?
#define landing      1
#define not_landing  0

//瀹氶珮锛坢m锛?
#define stable_height 700

extern Control_TypeDef control;

extern void Stop_motor();


void control_handle_task(void *pvParameters);
void PIDSTRUCT_Init();
float ELRS_Convert_angle(int ELRS_data);
u16 ELRS_Convert_throttle(unsigned ELRS_data);
void ELRS_Convert_flight_mode();
void ELRS_Convert_lock();
void Update_ELRS();
void Roll_outerloop_ctr(float angle_num);
void Roll_innerloop_ctr();
void Yaw_outerloop_ctr(float angle_num);
void Yaw_innerloop_ctr();
void Pitch_outerloop_ctr(float angle_num);
void Pitch_innerloop_ctr();
void Flight_control();
void Stop_motor();
void Check_control_mode();
void control_para_init();
float angle2rad(float angle);



#endif
