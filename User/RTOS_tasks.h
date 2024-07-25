#include "FreeRTOS.h"
#include "task.h"

/* 线程参数 */
#define TEST_TASK_PRIO       4
#define TEST_TASK_SIZE       256
#define PRINT_TASK_PRIO      3
#define PRINT_STK_SIZE       512
#define SoftStart_PRIO       4
#define SoftStart_SIZE       256
#define ControlHandle_PRIO   0
#define ControlHandle_SIZE   2048
#define IMU_PRIO             2
#define IMU_SIZE             2048

extern void RTOS_init();
