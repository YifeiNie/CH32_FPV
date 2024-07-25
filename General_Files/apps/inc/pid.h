#ifndef __PID_H_
#define __PID_H_
#define Limit(input, max, min) \
    do {                         \
        if ((input) > (max))     \
        {                        \
            (input) = (max);     \
        }                        \
        else if ((input) < (min))\
        {                        \
            (input) = (min);     \
        }                        \
    } while(0)

#define LimitMin(input,min) {if (input < min){ input = min;}}


typedef struct  _PID{
    float Kp;
    float Ki;
    float Kd;

    float max_out;      //最大输出
    float min_out;      //最小输出
    float max_iout;     //最大积分输出
    float min_iout;     //最大积分输出

    float set;          //目标值
    float fdb;          //当前值

    float out;          //pid输出
    float Pout;         //比例控制量
    float Iout;         //积分控制量
    float Dout;         //微分控制量
    float DeadBand;
    float Dbuf[3];      //微分项 0最新 1上一次 2上上次
    float error[3];     //误差项 0最新 1上一次 2上上次
}PID_STRUCT;

// pid函数结构体
typedef struct _PID_FUNC{

    void (*reset) ( PID_STRUCT *pid);
    void (*init ) ( PID_STRUCT *pid);
    void (*clc  ) ( PID_STRUCT *pid);
    void (*calc ) ( PID_STRUCT *pid, float set, float fdb);
}PID_FUNC;


extern PID_FUNC pid_func;

#endif

