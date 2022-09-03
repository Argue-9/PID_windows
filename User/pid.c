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

float PID_GetOutput(PID_PramTypeDef *WhichPID, float nowInput)
{

    WhichPID->input_now = nowInput;
    WhichPID->err_lastlast = WhichPID->err_last;
    WhichPID->err_last = WhichPID->err_now;
    WhichPID->err_now = WhichPID->target_now - nowInput;
    WhichPID->err_all += WhichPID->err_now;

    WhichPID->out_now = WhichPID->Kp * WhichPID->err_now + WhichPID->Kd * (WhichPID->err_now - WhichPID->err_last) + (WhichPID->err_all * WhichPID->Ki);

    return WhichPID->out_now;
}