## pseudocode
所需全局变量: aRGB//存储颜色
tasks: 
+ 读取aRGB "argb_readin"(暂时实现不了串口接收数据)
    + 通过串口输入更改全局变量aRGB
    + //使该task暂时堵塞 运行优先级较低的"led_show"
+ 颜色 pwm输出 "led_show" 
    + 按照公式输出相应 red, blue , green
    + 将该任务挂起(suspend)