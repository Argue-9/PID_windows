//
// Created by asus on 2022/9/2.
//

#include "pid.h"

void PID_DeInit(PID_PramTypeDef *WhichPID)
{
    WhichPID->Kp = 0.0;
    WhichPID->Ki = 0.0;
    WhichPID->Kd = 0.0;
    WhichPID->err_now = 0.0;
    WhichPID->err_last = 0.0;
    WhichPID->err_lastlast = 0.0;
    WhichPID->err_all = 0.0;
    WhichPID->out_now = 0.0;
    WhichPID->out_last = 0.0;
    WhichPID->target_now = 0.0;
    WhichPID->input_now = 0.0;
}

float PID_UpdateTargetNow(PID_PramTypeDef *WhichPID)
{

    if (WhichPID->target_now < WhichPID->ramp_target)
    {
        WhichPID->target_now += WhichPID->ramp_target_step;

        if (WhichPID->target_now >= WhichPID->ramp_target)
            WhichPID->target_now = WhichPID->ramp_target;
    }
    else if (WhichPID->target_now > WhichPID->ramp_target)  //反方向
    {
        WhichPID->target_now -= WhichPID->ramp_target_step;
        if (WhichPID->target_now <= WhichPID->ramp_target)
            WhichPID->target_now = WhichPID->ramp_target;
    }

    return WhichPID->target_now;
}

float PID_GetOutput(PID_PramTypeDef *WhichPID, float nowInput)
{
    //更新参数
    PID_UpdateTargetNow(WhichPID);

    WhichPID->input_now = nowInput;
    WhichPID->err_lastlast = WhichPID->err_last;
    WhichPID->err_last = WhichPID->err_now;
    WhichPID->err_now = WhichPID->target_now - nowInput;
    WhichPID->err_all += WhichPID->err_now;

    //对error_all限幅
    if(WhichPID->err_all > WhichPID->err_all_max)
        WhichPID->err_all = WhichPID->err_all_max;
    else if(WhichPID->err_all < -WhichPID->err_all_max)
        WhichPID->err_all = -WhichPID->err_all_max;

    //PID公式
    WhichPID->out_now = WhichPID->Kp * WhichPID->err_now + WhichPID->Kd * (WhichPID->err_now - WhichPID->err_last) + (WhichPID->err_all * WhichPID->Ki);

    //对输出电流限幅
    if(WhichPID->out_now > WhichPID->out_max)
        WhichPID->out_now = WhichPID->out_max;
    else if(WhichPID->out_now < -WhichPID->out_max)
        WhichPID->out_now = -WhichPID->out_max;

    return WhichPID->out_now;
}

