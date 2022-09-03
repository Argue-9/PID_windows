//
// Created by asus on 2022/9/2.
//
#include "motor.h"
#include "can.h"
#include "stdio.h"
#include "pid.h"
#include "string.h"
#include <stdlib.h>
#define MOTOR_RATIO     19.0f      //10减速比，小了就只能先拆换了
#define ENCODER_TO_ANGLE (360.0 / 8192.0f)
MOTORC_Info_t motor_infos;
PID_PramTypeDef motor_spd_pid;

/**
  * @brief  初始化电机的结构体
  * @retval None
  */
void MOTORC_InfoInit()
{
    motor_infos.current  = 0;
    motor_infos.angle_all= 0;
    motor_infos.ratio    = MOTOR_RATIO;

    motor_infos.last_position = 0;
    motor_infos.position = 0;
    motor_infos.velocity = 0;
    motor_infos.ecd_spd = 0;

    motor_infos.rota_times = 0;

    PID_DeInit(&motor_spd_pid);

    motor_spd_pid.Kp = 60;
    motor_spd_pid.Ki = 0.1;
    motor_spd_pid.Kd = 50;

    motor_spd_pid.err_all_max = 2000;
    motor_spd_pid.ramp_target_step = 0.3;
    motor_spd_pid.out_max = 600;

}

void MOTORC_SetCurrent(int16_t current)
{
    CAN_TxHeaderTypeDef tx_msg;
    uint32_t send_mail_box = 0;
    uint8_t send_data[8];

    tx_msg.StdId = 0x200;
    tx_msg.IDE = CAN_ID_STD;
    tx_msg.RTR = CAN_RTR_DATA;
    tx_msg.DLC = 8;
    send_data[4] = (int8_t )(current >> 8);
    send_data[5] = (int8_t )current & 0xff;
    HAL_CAN_AddTxMessage(&hcan1, &tx_msg, send_data, &send_mail_box);
}

void MOTORC_CANUpdate(int16_t pos, int16_t spd)
{
    motor_infos.position = pos;
    motor_infos.ecd_spd = spd;
}

void MOTORC_InfoUpdate()
{
    motor_infos.velocity = (float)motor_infos.ecd_spd / MOTOR_RATIO; //单位n
    if (motor_infos.position - motor_infos.last_position < - 6000)//正传8191到10
        ++motor_infos.rota_times;
    else if(motor_infos.position - motor_infos.last_position> 6000)//反10 到 8191
        --motor_infos.rota_times;
//		motor_infos.position = motor_infos.position / MOTOR_RATIO;
    motor_infos.angle_all = motor_infos.position * ENCODER_TO_ANGLE + motor_infos.rota_times * 360.0f;
    motor_infos.angle_all = motor_infos.angle_all / MOTOR_RATIO;
    motor_infos.last_position = motor_infos.position;
}
void MOTORC_UpdateCurrent(int16_t current1)
{
    motor_infos.current = current1;
}


void MOTORC_SetVel(float rpm)
{
    motor_spd_pid.target_now = rpm;
}

void MOTORC_SetVelWithRamp(float rpm)
{
    motor_spd_pid.ramp_target = rpm;
}

void MOTORC_InfoUpdateLoop()
{
    // 更新编码器数据，直接赋值
    MOTORC_InfoUpdate();
    PID_GetOutput(&motor_spd_pid, motor_infos.velocity);
    MOTORC_UpdateCurrent(motor_spd_pid.out_now);

    // 更新输出轴数据，角度和速度
    //设置电流
    MOTORC_SetCurrent(motor_infos.current);
}

//通过上位机Vofa来调参
float VofaModifyValue(const uint8_t* buffer,uint16_t len)
{
    char str[len];
    float value;

    for (uint8_t i = 1; i < len - 1; i++)
        str[i - 1] = buffer[i];
    value = atof(str);

    switch (buffer[0]) {
        case 'P':
            motor_spd_pid.Kp = value;
            break;
        case 'I':
            motor_spd_pid.Ki = value;
            break;
        case 'V':
            MOTORC_SetVelWithRamp(value);
            break;
    }
    return value;
}

void MOTORC_Test()
{
    //printf("m:%f, %f, %d, %d\n", motor_infos.velocity, motor_infos.angle_all, motor_infos.rota_times, motor_infos.position);
    printf("pid:%f, %f, %f\n", motor_spd_pid.target_now, motor_spd_pid.input_now, motor_spd_pid.out_now);
    //printf("%f\n",motor_spd_pid.Kp);
}