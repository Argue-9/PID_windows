//
// Created by asus on 2022/9/2.
//

#include "string.h"
#include <stdlib.h>
#include "usart.h"
#include <stdio.h>

//通过上位机Vofa来调参
float VofaModifyValue(const uint8_t* buffer,uint16_t len)
{
    char str[len];
    float value;

    for (uint8_t i = 1; i < len - 1; i++)
        str[i - 1] = buffer[i];
    value = atof(str);

    switch (buffer[0]) {
        case 'p':
            printf("ppp\r\n");
    }
    return value;
}
