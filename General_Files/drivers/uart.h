#ifndef  __UART_H_
#define __UART_H_

void USART2_IRQHandler(void);
void UART5_IRQHandler(void);
void USARTx_CFG(void);
void USART2_RxCpltCallback(USART_TypeDef *USARTx);
void UART5_RxCpltCallback(USART_TypeDef *USARTx);
#endif
