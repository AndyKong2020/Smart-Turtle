/******************************************************************************
/// @brief
/// @copyright Copyright (c) 2017 <dji-innovations, Corp. RM Dept.>
/// @license MIT License
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction,including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense,and/or sell
/// copies of the Software, and to permit persons to whom the Software is furnished
/// to do so,subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
/// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.
*******************************************************************************/

#include "can.h"
#include "bsp_can.h"
#include "usart.h"

motor_measure_t motor_measure_chassis[4] = {0};//4 chassis moto
CAN_TxHeaderTypeDef		Tx1Message;
CAN_RxHeaderTypeDef 	Rx1Message;
uint8_t Rx1Data[8] = {0};
uint8_t Tx1Data[8] = {0};
uint32_t Box = 100;
uint32_t x = 0;


void get_total_angle(motor_measure_t *p);
void get_moto_offset(motor_measure_t *ptr, uint8_t* Data);

/*******************************************************************************************
  * @Func		my_can_filter_init
  * @Brief    CAN1和CAN2滤波器配置
  * @Param		CAN_HandleTypeDef* hcan
  * @Retval		None
  * @Date     2015/11/30
 *******************************************************************************************/
void my_can_filter_init_recv_all(CAN_HandleTypeDef* _hcan)
{
	//can1 &can2 use same filter config
	CAN_FilterTypeDef		CAN_FilterConfigStructure;


	CAN_FilterConfigStructure.FilterBank = 1;
	CAN_FilterConfigStructure.FilterMode = CAN_FILTERMODE_IDMASK;
	CAN_FilterConfigStructure.FilterScale = CAN_FILTERSCALE_32BIT;
	CAN_FilterConfigStructure.FilterIdHigh = 0x0000;
	CAN_FilterConfigStructure.FilterIdLow = 0x0000;
	CAN_FilterConfigStructure.FilterMaskIdHigh = 0x0000;
	CAN_FilterConfigStructure.FilterMaskIdLow = 0x0000;
	CAN_FilterConfigStructure.FilterFIFOAssignment = CAN_FilterFIFO0;
	CAN_FilterConfigStructure.SlaveStartFilterBank = 14;//can1(0-13)和can2(14-27)分别得到一半的filter
	CAN_FilterConfigStructure.FilterActivation = ENABLE;

	if(HAL_CAN_ConfigFilter(_hcan, &CAN_FilterConfigStructure) != HAL_OK)
	{
		Error_Handler();
	}
}
/*******************************************************************************************
  * @Func			void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* _hcan)
  * @Brief    HAL库中标准的CAN接收完成回调函数，需要在此处理通过CAN总线接收到的数据
  * @Param		
  * @Retval		None 
  * @Date     2015/11/24
 *******************************************************************************************/
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	//ignore can1 or can2.
	if (hcan->Instance == CAN1)		// 获得接收到的数据头和数据
	{
		HAL_CAN_GetRxMessage(hcan,CAN_FilterFIFO0,&Rx1Message,Rx1Data);
		switch(Rx1Message.StdId){
			case CAN_2006Moto1_ID:
			case CAN_2006Moto2_ID:
			case CAN_2006Moto3_ID:
			case CAN_2006Moto4_ID:
				{
					static uint8_t i;
					i = Rx1Message.StdId - CAN_2006Moto1_ID;
					get_motor_measure(&motor_measure_chassis[i], Rx1Data);
				}
				break;
	  }
  }
		
	/*#### add enable can it again to solve can receive only one ID problem!!!####**/
	__HAL_CAN_ENABLE_IT(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
}

/*******************************************************************************************
  * @Func			void get_moto_measure(moto_measure_t *ptr, CAN_HandleTypeDef* hcan)
  * @Brief    接收3508电机通过CAN发过来的信息
  * @Param		
  * @Retval		None
  * @Date     2015/11/24
 *******************************************************************************************/
void get_motor_measure(motor_measure_t *ptr, uint8_t* Data)
{

	ptr->last_angle = ptr->angle;
	ptr->angle = (uint16_t)(Data[0]<<8 | Data[1]) ;
	ptr->speed_rpm  = (int16_t)(Data[2]<<8 | Data[3]);
	ptr->real_current = (Data[4]<<8 | Data[5])*5.f/16384.f;

	ptr->hall = Data[6];
	
	
	if(ptr->angle - ptr->last_angle > 4096)
		ptr->round_cnt --;
	else if (ptr->angle - ptr->last_angle < -4096)
		ptr->round_cnt ++;
	ptr->total_angle = ptr->round_cnt * 8192 + ptr->angle - ptr->offset_angle;
}

/*this function should be called after system+can init */
void get_moto_offset(motor_measure_t *ptr, uint8_t* Data)
{
	ptr->angle = (uint16_t)(Data[0]<<8 | Data[1]) ;
	ptr->offset_angle = ptr->angle;
}

#define ABS(x)	( (x>0) ? (x) : (-x) )
/**
*@bref 电机上电角度=0， 之后用这个函数更新3510电机的相对开机后（为0）的相对角度。
	*/
void get_total_angle(motor_measure_t *p){
	
	int res1, res2, delta;
	if(p->angle < p->last_angle){			//可能的情况
		res1 = p->angle + 8192 - p->last_angle;	//正转，delta=+
		res2 = p->angle - p->last_angle;				//反转	delta=-
	}else{	//angle > last
		res1 = p->angle - 8192 - p->last_angle ;//反转	delta -
		res2 = p->angle - p->last_angle;				//正转	delta +
	}
	//不管正反转，肯定是转的角度小的那个是真的
	if(ABS(res1)<ABS(res2))
		delta = res1;
	else
		delta = res2;

	p->total_angle += delta;
	p->last_angle = p->angle;
}

void set_motor_current(CAN_HandleTypeDef* hcan, int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4){

	Tx1Message.StdId = 0x200;
	Tx1Message.IDE = CAN_ID_STD;
	Tx1Message.RTR = CAN_RTR_DATA;
	Tx1Message.DLC = 0x08;
	Tx1Data[0] = (iq1 >> 8);
	Tx1Data[1] = iq1;
	Tx1Data[2] = (iq2 >> 8);
	Tx1Data[3] = iq2;
	Tx1Data[4] = iq3 >> 8;
	Tx1Data[5] = iq3;
	Tx1Data[6] = iq4 >> 8;
	Tx1Data[7] = iq4;
	
	HAL_CAN_AddTxMessage(hcan,&Tx1Message,Tx1Data,&Box);
}	
