STM32F103C8T6 USART1 中断接收 + DMA 连续发送实验

硬件连接：USART1_TX=PA9，USART1_RX=PA10，GND 共地。
串口参数：115200，8 数据位，1 停止位，无校验，无硬件流控。

功能：
1. 上电后通过 DMA 连续发送 hello windows!\r\n。
2. 上位机发送字符 #，暂停发送；发送字符 *，恢复发送。
3. RX 采用 HAL_UART_Receive_IT() 单字节中断接收；TX 采用 DMA1 Channel4。

工程文件：MDK-ARM/STM32_USART_DMA_Interrupt.uvprojx
生成 HEX：在 Keil 中打开工程，Options for Target -> Output 勾选 Create HEX File，然后 Build。
默认输出位置：MDK-ARM\Objects\STM32_USART_DMA_Interrupt.hex
也可在工程根目录执行：powershell -ExecutionPolicy Bypass -File .\build_hex.ps1 -KeilUv4 "C:\Keil_v5\UV4\UV4.exe"

实验二说明：
USART1 RX 使用中断，每次接收 1 字节；收到 # 置暂停标志，当前 DMA 帧发送完毕后不再启动下一帧；收到 * 清除暂停标志并恢复帧发送。
USART1 TX 使用 DMA1_Channel4，采用普通模式发送一帧，传输完成回调中自动启动下一帧。这样避免主循环轮询和 CPU 逐字节发送，适合连续高速发送。
