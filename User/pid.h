//
// Created by asus on 2022/9/2.
//

#ifndef TEST_PID_H
#define TEST_PID_H

typedef struct
{
    float Kp;
    float Ki;
    float Kd;
    float err_now;
    float err_last;
    float err_lastlast; 		//增量式PID需要
    float err_all;
    float input_now;
    float out_now;

    float out_last;
    float target_now;

    float err_all_max;
    float ramp_target;
    float ramp_target_step;

    float out_max;
    float out_step_max;
} PID_PramTypeDef;

void PID_DeInit(PID_PramTypeDef *WhichPID);
float PID_GetOutput(PID_PramTypeDef *WhichPID, float nowInput);
#endif //TEST_PID_H
