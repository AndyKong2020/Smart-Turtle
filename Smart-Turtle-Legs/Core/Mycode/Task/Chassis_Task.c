#include "Chassis_Task.h"
#include "bsp_can.h"
#include "remote_control.h"
#include "FreeRTOS.h"
#include "task.h"

#define M2006_MOTOR_SPEED_PID_KP 5.0f
#define M2006_MOTOR_SPEED_PID_KI 0.5f
#define M2006_MOTOR_SPEED_PID_KD 0.0f
#define M2006_MOTOR_SPEED_PID_MAX_OUT 16000.0f
#define M2006_MOTOR_SPEED_PID_MAX_IOUT 1000.0f

chassis_motor_t chassis_m2006[4];
chassis_control_t chassis_control;
void Chassis_Motor_Init(void)
{
    my_can_filter_init_recv_all(&hcan1);
    HAL_CAN_Start(&hcan1);
    HAL_CAN_ActivateNotification(&hcan1,CAN_IT_RX_FIFO0_MSG_PENDING);

    const static fp32 motor_speed_pid[3] = {M2006_MOTOR_SPEED_PID_KP, M2006_MOTOR_SPEED_PID_KI, M2006_MOTOR_SPEED_PID_KD};
    for(uint8_t i=0;i<4;i++)
    {
        chassis_m2006[i].speed=0;
        chassis_m2006[i].speed_set=0;
        chassis_m2006[i].give_current=0;

        PID_init(&chassis_m2006[i].pid,PID_DELTA,motor_speed_pid,M2006_MOTOR_SPEED_PID_MAX_OUT,M2006_MOTOR_SPEED_PID_MAX_IOUT);
    }
}

void Chassis_Motor_Data_Update(void)
{
    for(uint8_t i=0;i<4;i++)
    {
        chassis_m2006[i].speed=motor_measure_chassis[i].speed_rpm;
    }
}

static void chassis_rc_ctrl(void)
{
    if(rc_ctrl.rc.s[1]==RC_SW_DOWN)//stop
     {
        chassis_control.vx=0;
        chassis_control.wz=0;
     }
    else if(rc_ctrl.rc.s[1]==RC_SW_UP)//move
    {
        chassis_control.vx=rc_ctrl.rc.ch[3]*19;
        chassis_control.wz=rc_ctrl.rc.ch[2]*19;

        fp32 vl,vr;
        vl=chassis_control.vx-chassis_control.wz;
        vr=-chassis_control.vx-chassis_control.wz;

        chassis_m2006[0].speed_set = vl;
        chassis_m2006[1].speed_set = vl;
        chassis_m2006[2].speed_set = vr;
        chassis_m2006[3].speed_set = vr;

    }
}
void Chassis_Task(void const * argument)
{
//    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
//    remote_control_init();
//    Chassis_Motor_Init();
//    vTaskDelay(200);
//    while(1)
//    {
//        Chassis_Motor_Data_Update();
//        chassis_rc_ctrl();
//
//        if (rc_ctrl.rc.s[1] == RC_SW_DOWN || rc_ctrl.rc.s[1] == RC_SW_MID)
//            set_motor_current(&hcan1, 0, 0, 0, 0);
//        for (uint8_t i = 0; i < 4; i++) {
//            PID_calc(&chassis_m2006[i].pid, chassis_m2006[i].speed, chassis_m2006[i].speed_set);
//            chassis_m2006[i].give_current = chassis_m2006[i].pid.out;
//        }
//        set_motor_current(&hcan1, chassis_m2006[0].give_current, chassis_m2006[1].give_current,
//                          chassis_m2006[2].give_current, chassis_m2006[3].give_current);
//        vTaskDelay(2);
//    }
    while(1)
    {
        HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
        vTaskDelay(1000);
    }

}
