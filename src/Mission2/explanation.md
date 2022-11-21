## 红外二极管发射装置

本来去网上搜索相关资料发现挺麻烦的，以为要用二进制脉冲码(?)，或许是我想复杂的，因为得知红外接受只是一个简简单单readpin函数的调用，所以说猜测发射装置也就是用WritePin函数输出低电平(?)由于最后没有红外发射管和接收，于是代码基于以上猜测进行的编成

### 主要使用函数
HAL_GPIO_WritePin(Port,Pin,SET or RESET)
HAL_GPIO_ReadPin(Port,Pin)

### 变量
Recieve: 1代表接收到了信号 0代表暂未接收到信号
可以添加亮🚨来代表接收状态只需添加一个if else对recieve的值进行判断即可

### GPIO
#define REC_Pin GPIO_PIN_13
#define REC_GPIO_Port GPIOE


#define EMI_Pin GPIO_PIN_14
#define EMI_GPIO_Port GPIOE

REC  接收信号串口
EMI  发射信号串口