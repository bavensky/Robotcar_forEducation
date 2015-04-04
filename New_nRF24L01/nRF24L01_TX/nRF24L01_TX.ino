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

// init Variable 
const byte PIN_ANALOG_X = 0;
const byte PIN_ANALOG_Y = 1;
const byte PIN_A = 2;
const byte PIN_B = 3;
const byte PIN_C = 4;
const byte PIN_D = 5;
const byte PIN_E = 6;
const byte PIN_F = 7;
const byte PIN_K = 8;

int right = 0;
int left = 0;
int forward = 0;
int backward = 0;
int xaxis = 0;
int yaxis = 0;

void setup() 
{
  Serial.begin(9600);
  pinMode(CE,  OUTPUT);
  pinMode(CSN, OUTPUT);
  pinMode(IRQ, INPUT);
  SPI.begin();
  delay(50);
  init_io();  // Initialize IO port
  unsigned char sstatus=SPI_Read(STATUS);
  
  pinMode(PIN_A, INPUT);  
  pinMode(PIN_B, INPUT);  
  pinMode(PIN_C, INPUT);
  pinMode(PIN_D, INPUT);  
  pinMode(PIN_E, INPUT); 
  pinMode(PIN_F, INPUT); 
  pinMode(PIN_K, INPUT);
  
  digitalWrite(PIN_A, HIGH);
  digitalWrite(PIN_B, HIGH);
  digitalWrite(PIN_C, HIGH);
  digitalWrite(PIN_D, HIGH);
  digitalWrite(PIN_E, HIGH);
  digitalWrite(PIN_F, HIGH);
  digitalWrite(PIN_K, HIGH);
  
  TX_Mode();  // set TX mode
  
}

void loop() 
{
  for(;;)
  {
    xaxis = analogRead(PIN_ANALOG_X);
    yaxis = analogRead(PIN_ANALOG_Y);
    Serial.print(digitalRead(PIN_D));
    Serial.print("    ");
    
    
    if(analogRead(PIN_ANALOG_X) < 500 )  {
      left = map(xaxis, 500, 0, 0, 255);
      Serial.println(left);
    }
    if(analogRead(PIN_ANALOG_X) > 510 )  {
      right = map(xaxis, 510, 1023, 0, 255);
      Serial.println(right);
    }
    
    if(analogRead(PIN_ANALOG_Y) < 510 )  {
      backward = map(yaxis, 510, 0, 0, 255);
      Serial.println(backward);
    }
    if(analogRead(PIN_ANALOG_Y) > 520 )  {
      forward = map(yaxis, 520, 1023, 0, 255);
      Serial.println(forward);
    }
    
    //tx_buf[0] = left;
    //tx_buf[1] = right;
    tx_buf[0] = digitalRead(PIN_D);
    tx_buf[1] = digitalRead(PIN_B);
    tx_buf[2] = forward;    
    tx_buf[3] = backward;    
    
    unsigned char sstatus = SPI_Read(STATUS);    // read register STATUS's value
    if(sstatus&TX_DS)  {      // if receive data ready (TX_DS) interrupt
      SPI_RW_Reg(FLUSH_TX,0);                                  
      SPI_Write_Buf(WR_TX_PLOAD,tx_buf,TX_PLOAD_WIDTH);    // write playload to TX_FIFO
    }
    if(sstatus&MAX_RT)    {      // if receive data ready (MAX_RT) interrupt, this is retransmit than  SETUP_RETR                          
      SPI_RW_Reg(FLUSH_TX,0);
      SPI_Write_Buf(WR_TX_PLOAD,tx_buf,TX_PLOAD_WIDTH);    // disable standy-mode
    }
    
    SPI_RW_Reg(WRITE_REG+STATUS,sstatus);      // clear RX_DR or TX_DS or MAX_RT interrupt flag
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
void TX_Mode(void)
{
  digitalWrite(CE, 0);

  SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // Writes TX_Address to nRF24L01
  SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // RX_Addr0 same as TX_Adr for Auto.Ack

  SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      // Enable Auto.Ack:Pipe0
  SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  // Enable Pipe0
  SPI_RW_Reg(WRITE_REG + SETUP_RETR, 0x1a); // 500us + 86us, 10 retrans...
  SPI_RW_Reg(WRITE_REG + RF_CH, 9);        // Select RF channel 40
  SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);   // TX_PWR:0dBm, Datarate:2Mbps, LNA:HCURR
  SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);     // Set PWR_UP bit, enable CRC(2 unsigned chars) & Prim:TX. MAX_RT & TX_DS enabled..
  SPI_Write_Buf(WR_TX_PLOAD,tx_buf,TX_PLOAD_WIDTH);

  digitalWrite(CE, 1);
}
/*********************************  END  Function  TX  *******************************/  
