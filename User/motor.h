//
// Created by asus on 2022/9/2.
//

#ifndef TEST_MOTOR_H
#define TEST_MOTOR_H

#include "stm32f4xx_hal.h"
//�����Ϣ�ṹ��
typedef struct
{
    //unused ���ݾ��Ա�����
    int16_t position;     //��ǰλ�� ��������ֵ��δ��������
    int16_t last_position;//��һ��λ��
    int16_t ecd_spd;


    int32_t rota_times;
    float velocity;     //rpm
    float angle_all;    //�ܽǶ�
    float ratio;        //���ٱ�

    uint16_t ecd_ppr;   //����������
    int16_t current;    //���õ���
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
