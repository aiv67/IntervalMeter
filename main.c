#ifndef _STDINT_H
#include "stdint.h"
#endif /* _STDINT_H */

#include "inc/wh160x_driver.h"

uint16_t period;
uint16_t lastTimer;
uint16_t currentTimer;

int main(void)
{
	LCD_Init(LCD_CURSOR_OFF, LCD_BLINK_OFF);

	NVIC_SetPriority(TIM3_IRQn, 1);
	NVIC_EnableIRQ(TIM3_IRQn);

	/*������������� GPIOA.
	����� PA1 ������������� ��� ������ � ������� TIM2_CH2*/
	RCC->APB2ENR |= (RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN); //������������ ����� GPIOA � �������������� �������
	GPIOA->CRL |= GPIO_CRL_MODE1;//������������ �������� ����� = 50 MHz

	/*������������� ������� TIM3.
	  ��� ��������� ������� �������� ������� ������������ ����� 1 (TIM3_CH1)*/
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;//�������� ������������ TIM3
  TIM3->CCMR1 |= TIM_CCMR1_CC1S_0;//����� ��������� �����. ���������� "01" � ���� CC1S - ��������� ������� TIM3_CCR1 �� ������ TI1
  TIM3->CCMR1 |= (TIM_CCMR1_IC1F_0 | TIM_CCMR1_IC1F_1);//����� ������������ �������� ������� - 8 ������. IC1F = 0011.
  TIM3->CCER &= ~TIM_CCER_CC1P;//�� ��������� ������ - ������������� ������� ��������
  //TIM3->CCMR1 &= ~TIM_CCMR1_IC1PSC;//������������ ��������
  TIM3->PSC = 24000 - 1;
  TIM3->CCER |= TIM_CCER_CC1E;//�������� ������ �������� �������� � ������� TIM3_CCR1
  TIM3->DIER |= TIM_DIER_CC1IE;//��������� ��������� ���������� ��� �������
  TIM3->CR1 |= TIM_CR1_CEN;//��������� ���� �������

  LCD_PrintString("�����...");

  while(1)
  {
  }
}
void TIM3_IRQHandler(void)
{
	TIM3->SR &= ~TIM_SR_UIF;

	currentTimer = TIM3->CCR1;
	period = currentTimer - lastTimer;
	lastTimer = currentTimer;

	LCD_SetCursorPos(0, 0);
	LCD_PrintString("������: ");
	LCD_PrintDec(period);
	LCD_PrintString("ms   ");
}


