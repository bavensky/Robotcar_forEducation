/*******************************************************************************
** Project  : Robot Car nRF24L01 TX                                           **
** Part     : nRF24l01_RX                                                     **
** Compiler : Arduino 1.5.5-r2                                                **
** Board    : Arduino UNO                                                     **
** Author   : Bavensky :3                                                     **
** E-Mail   : Apiruk_Sang-ngenchai@hotmail.com                                **
** Date     : 04/03/2558 [dd/mm/yyyy]                                         **
** Read more: http://arduino-info.wikispaces.com/Nrf24L01-2.4GHz-HowTo        **
** Connect  : nRF24L01                                                        **
**   SPI-compatible                                                           **
**   CE - to digital pin 9    (CE pin)                                        **
**   CSN - to digital pin 10  (SS pin)                                        **
**   MOSI - to digital pin 11 (MOSI pin)                                      **
**   MISO - to digital pin 12 (MISO pin)                                      **
**   CLK - to digital pin 13  (SCK pin)                                       **
*******************************************************************************/

#include <SPI.h>       // เรียกใช้งานไลบรารี่การสื่อสารอนุกรมแบบ Synchronous 
#include "API.h"
#include "nRF24L01.h"  // เรียกใช้งานไลบรารี่โมดูล nRF24L01

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

const byte PIN_ANALOG_X = 0;
const byte PIN_ANALOG_Y = 1;
const byte PIN_A = 2;
const byte PIN_B = 3;
const byte PIN_C = 4;
const byte PIN_D = 5;
const byte PIN_E = 6;
const byte PIN_F = 7;
const byte PIN_K = 8;

//***************************************************
void setup() 
{
  pinMode(CE,  OUTPUT);
  pinMode(CSN, OUTPUT);
  pinMode(IRQ, INPUT);
  
  SPI.begin();
  delay(50);
  
  init_io();                        
  
  pinMode(PIN_A, INPUT);    // กำหนดให้ตัวแปร PIN_A ให้เป็น อินพุต  
  pinMode(PIN_B, INPUT);    // กำหนดให้ตัวแปร PIN_B ให้เป็น อินพุต   
  pinMode(PIN_C, INPUT);    // กำหนดให้ตัวแปร PIN_C ให้เป็น อินพุต 
  pinMode(PIN_D, INPUT);    // กำหนดให้ตัวแปร PIN_D ให้เป็น อินพุต   
  pinMode(PIN_E, INPUT);    // กำหนดให้ตัวแปร PIN_E ให้เป็น อินพุต  
  pinMode(PIN_F, INPUT);    // กำหนดให้ตัวแปร PIN_F ให้เป็น อินพุต  
  pinMode(PIN_K, INPUT);    // กำหนดให้ตัวแปร PIN_K ให้เป็น อินพุต 
  
  unsigned char sstatus=SPI_Read(STATUS);
  
  TX_Mode();                   // เริ่มต้นการรับข้อมูล (RX mode)
}

void loop() 
{
  for(;;)
  {
    tx_buf[1] = digitalRead(PIN_A);      // อ่านข้อมูลแบบดิจิตอลจากตัวแปร PIN_A และเก็บไว้ใน tx_buf[1]    
    tx_buf[2] = digitalRead(PIN_B);      // อ่านข้อมูลแบบดิจิตอลจากตัวแปร PIN_B และเก็บไว้ใน tx_buf[2]  
    tx_buf[3] = digitalRead(PIN_C);      // อ่านข้อมูลแบบดิจิตอลจากตัวแปร PIN_C และเก็บไว้ใน tx_buf[3]      
    tx_buf[4] = digitalRead(PIN_D);      // อ่านข้อมูลแบบดิจิตอลจากตัวแปร PIN_D และเก็บไว้ใน tx_buf[4]     
    
    unsigned char sstatus = SPI_Read(STATUS);                  // อ่านค่าจากรีจิสเตอร์ STATUS
    
    if(sstatus&TX_DS)                                          // ถ้าได้รับข้อมูล
    {
      SPI_RW_Reg(FLUSH_TX,0);                                  
      SPI_Write_Buf(WR_TX_PLOAD,tx_buf,TX_PLOAD_WIDTH);        // เขียนข้อมูลลงไปที่ TX_FIFO
    }
    if(sstatus&MAX_RT)                         
    {
      SPI_RW_Reg(FLUSH_TX,0);
      SPI_Write_Buf(WR_TX_PLOAD,tx_buf,TX_PLOAD_WIDTH);
    }
    SPI_RW_Reg(WRITE_REG+STATUS,sstatus);                      // ล้างข้อมูล RX_DR หรือ TX_DS หรืออMAX_RT 
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

/********************************    ฟังก์ชั่นสำหรับตั้งค่าโหมส่งข้อมูล    ********************************/
void TX_Mode(void)
{
  digitalWrite(CE, 0);
  SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    
  SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); 
  SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      
  SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  
  SPI_RW_Reg(WRITE_REG + SETUP_RETR, 0x1a);
  SPI_RW_Reg(WRITE_REG + RF_CH, 9);                        // เลือกรับข้อมูลที่ช่อง 9 
  SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x0b);   
  SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);    
  SPI_Write_Buf(WR_TX_PLOAD,tx_buf,TX_PLOAD_WIDTH);
  digitalWrite(CE, 1);
}
/********************************    สิ้นสุดฟังก์ชั่นการเชื่อมต่อสำหรับ nRF24L01     ********************************/
