## pseudocode

### 所需全局变量: aRGB//存储颜色
    + 可在main.c 47行对其进行修改而发出不同颜色光
### tasks: 
    + led_green
    + led_blue
    + led_red
    通过调用osDelay() 函数使该任务堵塞，执行同等优先级任务
### aRGB三原色
    一个aRGB值可以通过8位16进制数来描述
    其中alpha值表示透明度
    alpha与对应颜色指相乘即为该颜色 所对应寄存器TIMXccrx值

### 使用PWM对🚨调色
    初始化函数：HAL_TIM_PWM_Start(&htim,channel) 在c板上对应 TIM5 CH1-3
    赋值函数：__HAL_TIM_SetCompare(&htim,channel, alpha*x(green,red,blue) )

### 最终效果
    所指定颜色🚨 亮三秒后灭三秒