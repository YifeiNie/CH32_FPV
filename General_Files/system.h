#ifndef __SYSTEM_H
#define __SYSTEM_H

#ifdef __cplusplus
 extern "C" {
#endif

//============================【系统初始化】================================
void System_Init();

//==============================【主循环】=================================
void System_Loop();

//=============================【错误处理】=================================
void System_Error_Handler();

#ifdef __cplusplus
 }
#endif

#endif /* __SYSTEM_H */


