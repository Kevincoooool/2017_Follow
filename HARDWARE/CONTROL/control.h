#ifndef _CONTROL_H
#define _CONTROL_H
#include "sys.h"
extern void PositionPID(void);
extern void lock(void);							//飞控加锁
extern void Unlock(void);						//飞控解锁
extern void Take_off(void);					//一键起飞
extern void Land_down(void);					//降落
extern void Start_Fixed_high(void);	//开启定高
extern void High_fix(void);
extern float pwm_X,pwm_Y;

void Outter_PID(void);
void Inner_PID(void);
#endif

