//
// Created by asus on 2022/9/2.
//

#ifndef TEST_MOTOR_H
#define TEST_MOTOR_H

#include "stm32f4xx_hal.h"
//电机信息结构体
typedef struct
{
    //unused 兼容绝对编码器
    int16_t position;     //当前位置 编码器的值，未做连续化
    int16_t last_position;//上一刻位置
    int16_t ecd_spd;


    int32_t rota_times;
    float velocity;     //rpm
    float angle_all;    //总角度
    float ratio;        //减速比

    uint16_t ecd_ppr;   //编码器线数
    int16_t current;    //设置电流
}MOTORC_Info_t;

void MOTORC_InfoInit();
void MOTORC_SetCurrent(int16_t current);
void MOTORC_CANUpdate(int16_t pos, int16_t spd);
void MOTORC_InfoUpdate();
void MOTORC_Test();
void MOTORC_UpdateCurrent(int16_t current1);
void MOTORC_InfoUpdateLoop();
void MOTORC_SetVel(float rpm);
void MOTORC_SetVelWithRamp(float rpm);
float VofaModifyValue(const uint8_t* buffer,uint16_t len);
#endif //TEST_MOTOR_H
