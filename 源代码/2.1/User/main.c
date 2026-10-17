#include "stm32f10x.h"

/* 发送使能标志：1 = 正在发送，0 = 暂停发送 */
volatile uint8_t send_flag = 1;

void USART1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate            = 115200;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    /* 使能 RXNE 接收中断：每收到一个字节触发一次中断 */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    /* NVIC 配置 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel                   = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART1, ENABLE);
}

void USART_SendByte(uint8_t ch)
{
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, ch);
}

void USART_SendString(char *str)
{
    while (*str)
    {
        USART_SendByte((uint8_t)*str);
        str++;
    }
}

/* USART1 中断服务函数 */
void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        uint8_t ch = USART_ReceiveData(USART1);   /* 读 DR，RXNE 自动清零 */

        if (ch == '#')
        {
            send_flag = 0;      /* 收到 '#'：暂停发送 */
        }
        else if (ch == '*')
        {
            send_flag = 1;      /* 收到 '*'：恢复发送 */
        }
        /* 其他字符不影响发送状态 */
    }
}

int main(void)
{
    USART1_Init();

    while (1)
    {
        if (send_flag)
        {
            USART_SendString("hello windows!\r\n");
            /* 等待最后一个字节完整移出，保证一帧完整 */
            while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
        }
        /* send_flag == 0 时循环空转，不再发送任何字符 */
    }
}