/*******************************************************************************
** Project  : Robot Car nRF24L01 RX                                           **
** Part     : nRF24l01_RX                                                     **
** Compiler : Arduino 1.5.5-r2                                                **
** Board    : Arduino UNO                                                     **
** Author   : Bavensky :3                                                     **
** E-Mail   : Apiruk_Sang-ngenchai@hotmail.com                                **
** Date     : 04/03/2558 [dd/mm/yyyy]                                         **
** Read more: http://arduino-info.wikispaces.com/Nrf24L01-2.4GHz-HowTo        **
** Connect  : nRF24L01                                                        **
**   SPI-compatible                                                           **
**   CE - to digital pin 8    (CE pin)                                        **
**   CSN - to digital pin 10  (SS pin)                                        **
**   MOSI - to digital pin 11 (MOSI pin)                                      **
**   MISO - to digital pin 12 (MISO pin)                                      **
**   CLK - to digital pin 13  (SCK pin)                                       **
*******************************************************************************/

/*****************************  Motor Diagram  *********************************
              ________
            /          \
Motor1      |          |      Motor2
      ||||| |          | |||||
      ||||| |          | ||||| 
      ||||| |          | |||||
            |          |
            |__________|
/***************************  Pin connect Motor  ********************************/

#include <SPI.h>        // เรียกใช้งานไลบรารี่การสื่อสารอนุกรมแบบ Synchronous 
#include "API.h"       
#include "nRF24L01.h"   // เรียกใช้งานไลบรารี่โมดูล nRF24L01

#define motor1A  9  // เชื่อมต่อ pin digital 9 arduino
#define motor1B  6  // เชื่อมต่อ pin digital 6 arduino
#define motor2A  5  // เชื่อมต่อ pin digital 5 arduino
#define motor2B  3  // เชื่อมต่อ pin digital 3 arduino

//***************************************************
#define TX_ADR_WIDTH    5  
#define TX_PLOAD_WIDTH  32  

// กำหนดค่าคงที่ของแอดเดรส TX
unsigned char TX_ADDRESS[TX_ADR_WIDTH]  = 
{
  0x34,0x43,0x10,0x10,0x01
}; 

// เริ่มต้นการกำหนดค่าตัวแปร
unsigned char rx_buf[TX_PLOAD_WIDTH] = {0};
unsigned char tx_buf[TX_PLOAD_WIDTH] = {0};

//***************************************************
void setup() 
{
  pinMode(CE,  OUTPUT);
  pinMode(CSN, OUTPUT);
  pinMode(IRQ, INPUT);
  
  SPI.begin();
  delay(50);
  
  init_io();                        
  
  pinMode(motor1A, OUTPUT);    // กำหนดให้ตัวแปร motor1A ให้เป็น เอาต์พุต
  pinMode(motor1B, OUTPUT);    // กำหนดให้ตัวแปร motor1B ให้เป็น เอาต์พุต
  pinMode(motor2A, OUTPUT);    // กำหนดให้ตัวแปร motor2A ให้เป็น เอาต์พุต
  pinMode(motor2B, OUTPUT);    // กำหนดให้ตัวแปร motor2B ให้เป็น เอาต์พุต
  
  unsigned char sstatus=SPI_Read(STATUS);
  
  RX_Mode();                   // เริ่มต้นการรับข้อมูล (RX mode)
}

void loop() 
{
  for(;;)
  {
    unsigned char status = SPI_Read(STATUS);                  // อ่านค่าจากรีจิสเตอร์ STATUS
    if(status&RX_DR)                                          // ถ้าได้รับข้อมูล
    {
      SPI_Read_Buf(RD_RX_PLOAD, rx_buf, TX_PLOAD_WIDTH);      // อ่านข้อมูลแล้วเก็บไว้ใน rx_buf  
      SPI_RW_Reg(FLUSH_RX,0);                                 // clear RX_FIFO

      if(rx_buf[1] == 0) forword(255);                        // ถ้า rx_buf[1] ได้รับข้อมูลแล้วมีค่าเท่ากับ 0 จะสั่งให้หุ่นยนต์เดินหน้า
      if(rx_buf[2] == 0) turnright(255);                      // ถ้า rx_buf[1] ได้รับข้อมูลแล้วมีค่าเท่ากับ 0 จะสั่งให้หุ่นยนต์เดินหน้า
      if(rx_buf[3] == 0) backword(255);                       // ถ้า rx_buf[1] ได้รับข้อมูลแล้วมีค่าเท่ากับ 0 จะสั่งให้หุ่นยนต์เดินหน้า
      if(rx_buf[4] == 0) turnleft(255);                       // ถ้า rx_buf[1] ได้รับข้อมูลแล้วมีค่าเท่ากับ 0 จะสั่งให้หุ่นยนต์เดินหน้า
      if(rx_buf[1] == 1 && rx_buf[2] == 1 && rx_buf[3] == 1 && rx_buf[4] == 1) stopmotor();  // ถ้าไม่ได้รับข้อมูลการกดสวิตซ์ หุ่นยนต์จะหยุดทำงาน
    }
    SPI_RW_Reg(WRITE_REG+STATUS,status);                             // ล้างข้อมูล RX_DR หรือ TX_DS หรืออMAX_RT 
    delay(100);
  }
}

/********************************    ฟังก์ชั่นสำหรับการเชื่อมต่อสำหรับ nRF24L01    ********************************/
//**************************************************
// Function: init_io();
// Description:
// flash led one time,chip enable(ready to TX or RX Mode),
// Spi disable,Spi clock line init high
//**************************************************
void init_io(void)
{
  digitalWrite(IRQ, 0);
  digitalWrite(CE, 0);			
  digitalWrite(CSN, 1);                 	
}

/************************************************************************
**   * Function: SPI_RW();
 * 
 * Description:
 * Writes one unsigned char to nRF24L01, and return the unsigned char read
 * from nRF24L01 during write, according to SPI protocol
************************************************************************/
unsigned char SPI_RW(unsigned char Byte)
{
  return SPI.transfer(Byte);
}
/**************************************************/
/**************************************************
 * Function: SPI_RW_Reg();
 * 
 * Description:
 * Writes value 'value' to register 'reg'
/**************************************************/
unsigned char SPI_RW_Reg(unsigned char reg, unsigned char value)
{
  unsigned char status;
  digitalWrite(CSN, 0);                   
  SPI_RW(reg);                            
  SPI_RW(value);                        
  digitalWrite(CSN, 1);                 
  return(status);                   
}
/**************************************************/
/**************************************************
 * Function: SPI_Read();
 * 
 * Description:
 * Read one unsigned char from nRF24L01 register, 'reg'
/**************************************************/
unsigned char SPI_Read(unsigned char reg)
{
  unsigned char reg_val;
  digitalWrite(CSN, 0);                
  SPI_RW(reg);                       
  reg_val = SPI_RW(0);                
  digitalWrite(CSN, 1);              
  return(reg_val);                     
}
/**************************************************/
/**************************************************
 * Function: SPI_Read_Buf();
 * 
 * Description:
 * Reads 'unsigned chars' #of unsigned chars from register 'reg'
 * Typically used to read RX payload, Rx/Tx address
/**************************************************/
unsigned char SPI_Read_Buf(unsigned char reg, unsigned char *pBuf, unsigned char bytes)
{
  unsigned char sstatus,i;
  digitalWrite(CSN, 0);                   
  sstatus = SPI_RW(reg);      
  for(i=0;i<bytes;i++)
  {
    pBuf[i] = SPI_RW(0);   
  }
  digitalWrite(CSN, 1);                
  return(sstatus);                
}
/**************************************************/
/**************************************************
 * Function: SPI_Write_Buf();
 * 
 * Description:
 * Writes contents of buffer '*pBuf' to nRF24L01
 * Typically used to write TX payload, Rx/Tx address
/**************************************************/
unsigned char SPI_Write_Buf(unsigned char reg, unsigned char *pBuf, unsigned char bytes)
{
  unsigned char sstatus,i;

  digitalWrite(CSN, 0);                  
  sstatus = SPI_RW(reg);           
  for(i=0;i<bytes; i++)             
  {
    SPI_RW(*pBuf++);
  }
  digitalWrite(CSN, 1);                
  return(sstatus);                  
}
/**************************************************/

/********************************    ฟังก์ชั่นสำหรับตั้งค่าโหมดรับข้อมูล    ********************************/
void RX_Mode(void)
{
  digitalWrite(CE, 0);  
  SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);
  SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      
  SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01); 
  SPI_RW_Reg(WRITE_REG + RF_CH, 9);                        // เลือกรับข้อมูลที่ช่อง 9 
  SPI_RW_Reg(WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH);
  SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x0b);   
  SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f);      
  digitalWrite(CE, 1);                          
}
/********************************    สิ้นสุดฟังก์ชั่นการเชื่อมต่อสำหรับ nRF24L01     ********************************/

/******************************  Motor Function ******************************/
// ฟังก์ชันสำหรับสั่งให้หุ่นยนต์เดินหน้า
void forword(int value)
{
  analogWrite(motor1A,  0);
  analogWrite(motor1B,  value);
  analogWrite(motor2A,  0);
  analogWrite(motor2B,  value);
}
// ฟังก์ชันสำหรับสั่งให้หุ่นยนตถอยหลัง
void backword(int value)
{
  analogWrite(motor1A,  value);
  analogWrite(motor1B,  0);
  analogWrite(motor2A,  value);
  analogWrite(motor2B,  0);
}
// ฟังก์ชันสำหรับสั่งให้หุ่นยนตหมุนขวา
void turnright(int value)
{
  analogWrite(motor1A,  value);
  analogWrite(motor1B,  0);
  analogWrite(motor2A,  0);
  analogWrite(motor2B,  value);
  
}
// ฟังก์ชันสำหรับสั่งให้หุ่นยนต์หมุนซ้าย
void turnleft(int value)
{
  analogWrite(motor1A,  0);
  analogWrite(motor1B,  value);
  analogWrite(motor2A,  value);
  analogWrite(motor2B,  0);
  
}
// ฟังก์ชันสำหรับสั่งให้หุ่นยนตหยุดเคลื่อนไหว
void stopmotor()
{
  analogWrite(motor1A,  0);
  analogWrite(motor1B,  0);
  analogWrite(motor2A,  0);
  analogWrite(motor2B,  0); 
}

