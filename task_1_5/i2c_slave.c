#include "stm32f10x.h"
// Slave Address, would be different for each slave device
#define I2C_SLAVE_ADDRESS 0x50
void I2C1_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct;
  I2C_InitTypeDef I2C_InitStruct;
  // Enable the I2C & GPIO peripheral clock
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
  // GPIO Configuration
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_Init(GPIOB, &GPIO_InitStruct);
  // I2C Configuration
  I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStruct.I2C_OwnAddress1 = I2C_SLAVE_ADDRESS << 1;
  I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStruct.I2C_ClockSpeed = 100000;
  I2C_Init(I2C1, &I2C_InitStruct);
  // Enable I2C
  I2C_Cmd(I2C1, ENABLE);
}
int main(void) {
  uint8_t data[2];
  // I2C Initialization
  I2C1_Init();
  while (1) {
    // Wait for the start condition
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED));
    // Check the data
    for (uint8_t i = 0; i < 2; i++) {
      while (!I2C_CheckEvent(I2C1, I2C_EVENT_SLAVE_BYTE_RECEIVED));
      data[i] = I2C_ReceiveData(I2C1);
    }
    // Process data
    // Wait for stop condition
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_SLAVE_STOP_DETECTED));
    I2C_ClearFlag(I2C1, I2C_FLAG_STOPF);
  }
}
return 0