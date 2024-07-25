
// File name:           pid.c
// Last modified Date:  2024/04/21 23:30:27
// Last Version:        V1.0
//----------------------------------------------------------------------------------------
// Created by:          Nyf
// Created date:        2024/04/21 23:30:27
// Version:             V1.0
// Descriptions:        The original version

#include "../apps/inc/pid.h"




extern void pid_reset  (PID_STRUCT *p);
extern void pid_init   (PID_STRUCT *pid);
extern void pid_clc    (PID_STRUCT *p);
extern void pid_calc   (PID_STRUCT *p, float fb, float ref);

PID_FUNC pid_func = {
    .reset  = pid_reset,
    .init   = pid_init ,
    .clc    = pid_clc  ,
    .calc   = pid_calc ,
};


void pid_reset( PID_STRUCT  *p)
{
    p->Kp           = 0;
    p->Ki           = 0;
    p->Kd           = 0;

    p->max_out      = 0;
    p->min_out      = 0;
    p->min_iout     = 0;
    p->max_iout     = 0;

    p->out          = 0;     //周期
    p->Pout         = 0;     //比例系数P
    p->Iout         = 0;     //积分时间I
    p->Dout         = 0;     //微分时间D

}

// initialize
void pid_init ( PID_STRUCT *pid)
{
    pid->Dbuf[0] = pid->Dbuf[1] = pid->Dbuf[2] = 0.0f;
    pid->error[0] = pid->error[1] = pid->error[2] = pid->Pout = pid->Iout = pid->Dout = pid->out = 0.0f;

}


// reset
void pid_clc( PID_STRUCT *pid)
{
    pid->error[0] = pid->error[1] = pid->error[2] = 0.0f;
    pid->Dbuf[0] = pid->Dbuf[1] = pid->Dbuf[2] = 0.0f;
    pid->out = pid->Pout = pid->Iout = pid->Dout = 0.0f;
    pid->fdb = pid->set = 0.0f;
}


// set是设定值，fdb是当前值
void pid_calc( PID_STRUCT *pid, float set, float fdb)
{
    pid->error[2] = pid->error[1];
    pid->error[1] = pid->error[0];
    pid->set = set;
    pid->fdb = fdb;
    pid->error[0] = set - fdb;

    pid->Pout = pid->Kp * pid->error[0];
    pid->Iout += pid->Ki * pid->error[0];
    pid->Dbuf[2] = pid->Dbuf[1];
    pid->Dbuf[1] = pid->Dbuf[0];
    pid->Dbuf[0] = (pid->error[0] - pid->error[1]);
    pid->Dout = pid->Kd * pid->Dbuf[0];
    Limit(pid->Iout,pid->max_iout,pid->min_iout);
//    LimitMax(pid->Iout,pid->max_iout);
//    LimitMin(pid->Iout,pid->min_iout);
    pid->out = pid->Pout + pid->Iout + pid->Dout;
    Limit(pid->out,pid->max_out,pid->min_out);
//    LimitMax(pid->out,pid->max_out);
//    LimitMin(pid->out,pid->min_out);
    /*输出死区*/
    if(pid->error[0] <= pid->DeadBand && pid->error[0] >= -pid->DeadBand){
        pid->out = 0;
        pid->Iout = 0;
    }
}



