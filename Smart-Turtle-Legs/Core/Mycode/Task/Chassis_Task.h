#ifndef CHASSIS_TASK_H
#define CHASSIS_TASK_H

#include "struct_typedef.h"
#include "pid.h"
#include "main.h"

typedef struct
{
    fp32 speed;
    fp32 speed_set;
    int16_t give_current;

    PID_TypeDef pid;
}chassis_motor_t;

typedef struct
{
    fp32 vx;
    fp32 wz;

}chassis_control_t;

void Chassis_Task(void const * argument);

#endif


