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
**   CE - to digital pin 9                                                    **
**   CSN - to digital pin 10  (SS pin)                                        **
**   MOSI - to digital pin 11 (MOSI pin)                                      **
**   MISO - to digital pin 12 (MISO pin)                                      **
**   CLK - to digital pin 13 (SCK pin)                                        **
*******************************************************************************/

#include <SPI.h>
#include "API.h"
#include "nRF24L01.h"

//***************************************************
#define TX_ADR_WIDTH    5   // 5 unsigned chars TX(RX) address width
#define TX_PLOAD_WIDTH  32  // 32 unsigned chars TX payload

unsigned char TX_ADDRESS[TX_ADR_WIDTH]  = 
{
  0x34,0x43,0x10,0x10,0x01
}; // Define a static TX address

unsigned char rx_buf[TX_PLOAD_WIDTH] = {0}; // initialize value
unsigned char tx_buf[TX_PLOAD_WIDTH] = {0};

/************  Drive Motor  ****************************
Motor Right
  motorA_a  =  1  Go Forward
  motorA_b  =  1  Go Reverse
Motor Left  
  motorB_a  =  1  Go Reverse
  motorB_b  =  1  Go Forward
********************************************************/
int motorA_a = 9;  
int motorA_b = 6; 
int motorB_a = 5;   
int motorB_b = 3; 

void setup() 
{
  Serial.begin(9600);
  pinMode(CE,  OUTPUT);
  pinMode(CSN, OUTPUT);
  pinMode(IRQ, INPUT);
  SPI.begin();
  delay(50);
  init_io();      // Initialize IO port
  unsigned char sstatus=SPI_Read(STATUS);
  
  pinMode(motorA_a, OUTPUT);        
  pinMode(motorA_b, OUTPUT);      
  pinMode(motorB_a, OUTPUT);       
  pinMode(motorB_b, OUTPUT);
  
  RX_Mode();      // set RX mode
}

void loop() 
{
  for(;;)
  {
    unsigned char status = SPI_Read(STATUS);  // read register STATUS's value
    if(status&RX_DR)    {      // if receive data ready (TX_DS) interrupt
      SPI_Read_Buf(RD_RX_PLOAD, rx_buf, TX_PLOAD_WIDTH);  // read playload to rx_buf
      SPI_RW_Reg(FLUSH_RX,0);                             // clear RX_FIFO
      
      Serial.print(" ");
      Serial.print(rx_buf[0]);                     
      Serial.print(" ");
      Serial.print(rx_buf[1]);
      Serial.print(" ");
      Serial.print(rx_buf[2]); 
      Serial.print(" ");
      Serial.print(rx_buf[3]);
    }
    
    SPI_RW_Reg(WRITE_REG+STATUS,status);      // clear RX_DR or TX_DS or MAX_RT interrupt flag
    delay(100);  
  }
}

/****************************  initiate  Function  TX  *******************************/
void init_io(void)
{
  digitalWrite(IRQ, 0);
  digitalWrite(CE, 0);			// chip enable
  digitalWrite(CSN, 1);                 // Spi disable	
}

unsigned char SPI_RW(unsigned char Byte)
{
  return SPI.transfer(Byte);
}

unsigned char SPI_RW_Reg(unsigned char reg, unsigned char value)
{
  unsigned char status;

  digitalWrite(CSN, 0);                   // CSN low, init SPI transaction
  SPI_RW(reg);                            // select register
  SPI_RW(value);                          // ..and write value to it..
  digitalWrite(CSN, 1);                   // CSN high again

  return(status);                   // return nRF24L01 status unsigned char
}

unsigned char SPI_Read(unsigned char reg)
{
  unsigned char reg_val;

  digitalWrite(CSN, 0);                // CSN low, initialize SPI communication...
  SPI_RW(reg);                         // Select register to read from..
  reg_val = SPI_RW(0);                 // ..then read register value
  digitalWrite(CSN, 1);                // CSN high, terminate SPI communication

  return(reg_val);                     // return register value
}

unsigned char SPI_Read_Buf(unsigned char reg, unsigned char *pBuf, unsigned char bytes)
{
  unsigned char sstatus,i;

  digitalWrite(CSN, 0);                   // Set CSN low, init SPI tranaction
  sstatus = SPI_RW(reg);       	    // Select register to write to and read status unsigned char

  for(i=0;i<bytes;i++)
  {
    pBuf[i] = SPI_RW(0);    // Perform SPI_RW to read unsigned char from nRF24L01
  }

  digitalWrite(CSN, 1);                   // Set CSN high again

  return(sstatus);                  // return nRF24L01 status unsigned char
}

unsigned char SPI_Write_Buf(unsigned char reg, unsigned char *pBuf, unsigned char bytes)
{
  unsigned char sstatus,i;

  digitalWrite(CSN, 0);                   // Set CSN low, init SPI tranaction
  sstatus = SPI_RW(reg);             // Select register to write to and read status unsigned char
  for(i=0;i<bytes; i++)             // then write all unsigned char in buffer(*pBuf)
  {
    SPI_RW(*pBuf++);
  }
  digitalWrite(CSN, 1);                   // Set CSN high again
  return(sstatus);                  // return nRF24L01 status unsigned char
}

void RX_Mode(void)
{
  digitalWrite(CE, 0);
  
  SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // Use the same address on the RX device as the TX device
  SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      // Enable Auto.Ack:Pipe0
  SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  // Enable Pipe0
  SPI_RW_Reg(WRITE_REG + RF_CH, 9);        // Select RF channel 40
  SPI_RW_Reg(WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH); // Select same RX payload width as TX Payload width
  SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);   // TX_PWR:0dBm, Datarate:2Mbps, LNA:HCURR
  SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f);     // Set PWR_UP bit, enable CRC(2 unsigned chars) & Prim:RX. RX_DR enabled..
  digitalWrite(CE, 1);                             // Set CE pin high to enable RX device
  //  This device is now ready to receive one packet of 16 unsigned chars payload from a TX device sending to address
  //  '3443101001', with auto acknowledgment, retransmit count of 10, RF channel 40 and datarate = 2Mbps.
}
/*********************************  END  Function  TX  *******************************/ 
