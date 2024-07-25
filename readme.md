# 版本说明
- 用于嵌赛无人机的飞控固件
- CH32V307 & MPU6050

## 项目结构
//待完善

## TODO
- 新电池PID参数细调
- 增加与上位机（香橙派）的串口通讯
- 磁力计QMC7983的驱动和融合IMU数据程序编写
- 增加光流与测距传感器

## 改动记录
### 6.27 by NYF
- 3s电池的PID参数基本完成，可以进行飞行，但需要细调

### 6.11 by fish
- 把control.c全部移植到了User/RTOS_apps/control_handle.c中
- 把控制相关的变量全部整理到了**结构体control**中
- 修复了warning

### 6.11 by fish
- 移植了RTOS调度，并且重新整理了代码
- 线程在/User/RTOS_app中创建，有以下线程：
  - Print_status_task
    - 通过调试串口输出状态信息
  - IMU_task
    - 负责维护与读取IMU数据，并且做平均值滤波
    - 滤波之后得到的数据存储在MPU6050_para_filted结构体中
  - Motor_sort_start
    - 电机软启动线程，防止启动过快电调挂掉
    - 当检测到电机解锁时，抢占电机的控制权，然后缓慢将电机转速提高到怠速
    - 缓启动后释放电机的控制权，随后将电机的转速限制在怠速以上
  - control_handle_task
    - 负责无人机姿态控制
    - 主要程序还在control.c中，计划全部移植到RTOS_apps/control_handle.c中
- 此外，以下进程使用外部中断调度：
  - HandleByteReceived
    - 用于接收机解包，在uart.c中
    - 当进程IMU_task正在读取IMU时被调用会使单片机卡死，因此引入了IMU_IO_STATUS来防止这种情况的发生
      - 测试结果->添加后运行15分钟仍然没有卡死，如果不添加会在1-2分钟内卡死
