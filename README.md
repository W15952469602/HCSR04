# HCSR04

## 简介
这是一个简易的超声波模块软件接口，通过引用函数解除耦合，有循环等待和中断读取两种使用方法。当物体太远或照射面角度过大导致echo没有输出时，读取结果将输出65535。

## 使用方法
1. 调用初始化函数，将读写pin接口函数传入，初始化内部结构体。

```c
HCSR04_Init(HAL_WriteTrig, HAL_ReadEcho);
```

2.设置一个定时器，在回调函数中调用超声波处理程序。定时器时长至少1毫秒。

```c
static void vTimerCallback(TimerHandle_t xTimer)
{
    HCSR04_TimerHandle();
}

// 主任务函数
static void vMainTask(void *pvParameters)
{
    TimerHandle_t xTimer = xTimerCreate(
        "Timer1",
        pdMS_TO_TICKS(1),
        pdTRUE,
        0,
        vTimerCallback
    );
    xTimerStart(xTimer, 0);
 
    //其他程序.......
}
```

3.在程序中直接调用`HCSR04_ReadPolling()`读取echo输出高电平的时长使用`uint16_t`类型接收数据。

```c
uint16_t ticks;
ticks = HCSR04_ReadPolling();
```
