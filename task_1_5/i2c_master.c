/*In current example STM32F4 Microcontroller is used as a master device*/
#include "stm32f4xx.h"
#include "stm32f4xx_i2c.h"
#include "stm32f4xx_rcc.h"
#define I2C_SPEED 100000 // 100 KHz
#define I2C_SLAVE_ADDRESS 0xA0 // Slave address would be mentioned in the Datasheet of the device
                               // working as a Slave

I2C_InitTypeDef I2C_InitStruct;
void I2C1_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct;
  // Enable the I2C peripheral clock
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
  // Enable GPIO peripheral clock
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  // Configure the GPIO pins for I2C1
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStruct);
  // Connect I2C1 pins to the alternate function (AF4)
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1);
  // Initialize I2C1 peripheral
  I2C_DeInit(I2C1);
  I2C_InitStruct.I2C_ClockSpeed = I2C_SPEED;
  I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStruct.I2C_OwnAddress1 = 0x00;
  I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_8bit;
  I2C_Init(I2C1, &I2C_InitStruct);
  // Enable I2C1 peripheral
  I2C_Cmd(I2C1, ENABLE);
}
void I2C1_StartReception(uint8_t slaveAddr, uint8_t *buffer, uint8_t length,
                         uint8_t stopBit) {
  // Wait until I2C1 is not busy
  while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
    ;
  // Send start condition
  I2C_GenerateSTART(I2C1, ENABLE);
  // Wait for start condition sent successfully
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
    ;
  // Send slave address
  I2C_Send8bitAddress(I2C1, slaveAddr, I2C_Direction_Transmitter);
  // Wait for slave address sent successfully
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    ;
  for (uint8_t i = 0; i < length; i++) {
    I2C_ReceiveData(I2C1, buffer[i]);
    // Wait for data received successfully
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
      ;
  }
  if (stopBit) {
    // Send stop condition
    I2C_GenerateSTOP(I2C1, ENABLE);
  }
}
void I2C1_ReadData(uint8_t slaveAddr, uint8_t *buffer, uint8_t length) {
  // Wait until I2C1 is not busy
  while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
    ;
  // Send start condition
  I2C_GenerateSTART(I2C1, ENABLE);
  // Wait for start condition sent successfully
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
    ;
  // Send slave address
  I2C_Send8bitAddress(I2C1, slaveAddr, I2C_Direction_Receiver);
  // Wait for slave address sent successfully
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    ;
  // Read data
  for (uint8_t i = 0; i < length; i++) {
    if (i == (length - 1)) {
      // Disable Acknowledgement
      I2C_AcknowledgeConfig(I2C1, DISABLE);
    }
    // Wait for data received successfully
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
      ;
    buffer[i] = I2C_ReceiveData(I2C1);
  }
  // Enable Acknowledgement
  I2C_AcknowledgeConfig(I2C1, ENABLE);
  // Send stop condition
  I2C_GenerateSTOP(I2C1, ENABLE);
}
int main(void) {
  uint8_t data[2];
  // Initialize I2C1
  I2C1_Init();
  // Read data from the sensor
  I2C1_StartTransmission(I2C_SLAVE_ADDRESS, data, 2, 1);
  // Process the received data
  // Write data to the sensor
  data[0] = 0x01;
  data[1] = 0x02;
  I2C1_StartTransmission(I2C_SLAVE_ADDRESS, data, 2, 1);
  while (1)
    ;
  return 0;
}