#include "os.h"

Os_Var_t Os_Var;

static void Os_Calc_Period(uint8 idx);
static void Os_Calc_Runtime_Start(uint8 idx);
static void Os_Calc_Runtime_Stop(uint8 idx);
static void Os_1ms_Task(void);
static void Os_10ms_Task(void);
static void Os_100ms_Task(void);
static void Os_Background_Task(void);

void Os_Init_Task(void)
{

}
static void Os_1ms_Task(void)
{

}

static void Os_10ms_Task(void)
{

}

static void Os_100ms_Task(void)
{

}

static void Os_Background_Task(void)
{

}

void Os_Time_Handler(void)
{
	Os_Var.Os_Counter.Os_100us_Counter++;
}

void Os_Handler(void)
{
	void(*fp)(void);
    if(Os_Var.Os_Task_Flag.Os_100ms_Flag == 1)
	{
		fp = Os_100ms_Task;
		Os_Var.Os_Task_Add.Os_100ms_Address = fp;
		Os_Var.Os_Task_Flag.Os_100ms_Flag = 0;
		Os_Calc_Runtime_Start(OS_TASK_INDEX_100MS);
		(void)Os_100ms_Task();
		Os_Calc_Runtime_Stop(OS_TASK_INDEX_100MS);
		Os_Calc_Period(OS_TASK_INDEX_100MS);
	}
	else
	{
		Os_Calc_Runtime_Start(OS_TASK_INDEX_BG);
		(void)Os_Background_Task();
		Os_Calc_Runtime_Stop(OS_TASK_INDEX_BG);
		Os_Calc_Period(OS_TASK_INDEX_BG);
	}

    return;
}

void Os_Handler_1ms(void)
{
	if(Os_Var.Os_Counter.Os_1ms_Counter % 1 == 0)
	{
		Os_Var.Os_Counter.Os_1ms_Counter++;
		Os_Var.Os_Task_Flag.Os_1ms_Flag = 1;
	}

	if(Os_Var.Os_Counter.Os_1ms_Counter % 100 == 0)
	{
		Os_Var.Os_Counter.Os_100ms_Counter++;
		Os_Var.Os_Task_Flag.Os_100ms_Flag = 1;
	}
	void(*fp)(void);
	if(Os_Var.Os_Task_Flag.Os_1ms_Flag == 1)
	{
		fp = Os_1ms_Task;
		Os_Var.Os_Task_Add.Os_1ms_Address = fp;
		Os_Var.Os_Task_Flag.Os_1ms_Flag = 0;
		Os_Calc_Runtime_Start(OS_TASK_INDEX_1MS);
		(void)Os_1ms_Task();
		Os_Calc_Runtime_Stop(OS_TASK_INDEX_1MS);
		Os_Calc_Period(OS_TASK_INDEX_1MS);
	}
}

void Os_Handler_10ms(void)
{
	Os_Var.Os_Counter.Os_10ms_Counter++;
	Os_Var.Os_Task_Flag.Os_10ms_Flag = 1;
	void(*fp)(void);
	if(Os_Var.Os_Task_Flag.Os_10ms_Flag == 1)
	{
		fp = Os_10ms_Task;
		Os_Var.Os_Task_Add.Os_10ms_Address = fp;
		Os_Var.Os_Task_Flag.Os_10ms_Flag = 0;
		Os_Calc_Runtime_Start(OS_TASK_INDEX_10MS);
		(void)Os_10ms_Task();
		Os_Calc_Runtime_Stop(OS_TASK_INDEX_10MS);
		Os_Calc_Period(OS_TASK_INDEX_10MS);
	}
}

uint32 Os_Get_Tick_100us(void)
{
	return Os_Var.Os_Counter.Os_100us_Counter;
}

static void Os_Calc_Period(uint8 idx)
{
	uint32 OS_TimeLoc=0;
	uint32 OS_TimeOldLoc=0;
	uint32 Os_TimeDiffLoc=0;

	OS_TimeLoc = Os_Get_Tick_100us();

	switch(idx)
	{
		case OS_TASK_INDEX_1MS : 
			OS_TimeOldLoc = Os_Var.Os_Tick_Period.Os_1ms_Tick_Period;
		break;
		case OS_TASK_INDEX_10MS : 
			OS_TimeOldLoc = Os_Var.Os_Tick_Period.Os_10ms_Tick_Period;
		break;
		case OS_TASK_INDEX_100MS : 
			OS_TimeOldLoc = Os_Var.Os_Tick_Period.Os_100ms_Tick_Period;
		break;
	}

	Os_TimeDiffLoc = OS_TimeLoc - OS_TimeOldLoc;
	OS_TimeOldLoc = OS_TimeLoc;
	
	switch(idx)
	{
		case OS_TASK_INDEX_1MS : 
			Os_Var.Os_Period.Os_1ms_Period = Os_TimeDiffLoc;
			Os_Var.Os_Tick_Period.Os_1ms_Tick_Period = OS_TimeOldLoc;
		break;
		case OS_TASK_INDEX_10MS : 
			Os_Var.Os_Period.Os_10ms_Period = Os_TimeDiffLoc;
			Os_Var.Os_Tick_Period.Os_10ms_Tick_Period = OS_TimeOldLoc;
		break;
		case OS_TASK_INDEX_100MS : 
			Os_Var.Os_Period.Os_100ms_Period = Os_TimeDiffLoc;
			Os_Var.Os_Tick_Period.Os_100ms_Tick_Period = OS_TimeOldLoc;
		break;
	}
	
	return;
}

static void Os_Calc_Runtime_Start(uint8 idx)
{
	uint32 OS_TimeLoc=0;

	OS_TimeLoc = Os_Get_Tick_100us();

	switch(idx)
	{
		case OS_TASK_INDEX_1MS : 
			Os_Var.Os_Tick_Runtime.Os_1ms_Tick_Runtime_Start = OS_TimeLoc;
		break;
		case OS_TASK_INDEX_10MS : 
			Os_Var.Os_Tick_Runtime.Os_10ms_Tick_Runtime_Start = OS_TimeLoc;
		break;
		case OS_TASK_INDEX_100MS : 
			Os_Var.Os_Tick_Runtime.Os_100ms_Tick_Runtime_Start = OS_TimeLoc;
		break;
	}

	return;
}

static void Os_Calc_Runtime_Stop(uint8 idx)
{
	uint32 OS_TimeLoc=0;
	uint32 OS_TimeOldLoc=0;
	uint32 Os_TimeDiffLoc=0;

	OS_TimeLoc = Os_Get_Tick_100us();

	switch(idx)
	{
		case OS_TASK_INDEX_1MS : 
			OS_TimeOldLoc = Os_Var.Os_Tick_Runtime.Os_1ms_Tick_Runtime_Start;
		break;
		case OS_TASK_INDEX_10MS : 
			OS_TimeOldLoc = Os_Var.Os_Tick_Runtime.Os_10ms_Tick_Runtime_Start;
		break;
		case OS_TASK_INDEX_100MS : 
			OS_TimeOldLoc = Os_Var.Os_Tick_Runtime.Os_100ms_Tick_Runtime_Start;
		break;
	}

	Os_TimeDiffLoc = OS_TimeLoc - OS_TimeOldLoc;

	switch(idx)
	{
		case OS_TASK_INDEX_1MS : 
			Os_Var.Os_Runtime.Os_1ms_Runtime = Os_TimeDiffLoc;
			Os_Var.Os_Tick_Runtime.Os_1ms_Tick_Runtime_Stop = OS_TimeLoc;
		break;
		case OS_TASK_INDEX_10MS : 
			Os_Var.Os_Runtime.Os_10ms_Runtime = Os_TimeDiffLoc;
			Os_Var.Os_Tick_Runtime.Os_10ms_Tick_Runtime_Stop = OS_TimeLoc;
		break;
		case OS_TASK_INDEX_100MS : 
			Os_Var.Os_Runtime.Os_100ms_Runtime = Os_TimeDiffLoc;
			Os_Var.Os_Tick_Runtime.Os_100ms_Tick_Runtime_Stop = OS_TimeLoc;
		break;
	}

	return;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM15)
    {
        Os_Handler_10ms();
    }
    if(htim->Instance == TIM17)
    {
        Os_Time_Handler();
    }
}