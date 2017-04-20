#include <mcp_can.h>
#include <mcp_can_dfs.h>

#include "Pin_Defines.h"


MCP_CAN CAN(SPI_CS_PIN);

void toggleDPortRegisterIO(uint8_t reg)
{
  DDRD |= reg;
}

void toggleCPortRegisterIO(uint8_t reg)
{
  DDRC |= reg;
}

void toggleBPortRegisterIO(uint8_t reg)
{
  DDRB |= reg;
}

void ioRegisterManipulation(uint8_t len, uint8_t* buf)
{
  // TODO this should be mapped better but it works so leaving it.
  
  //Deal with IO (In order of CAN message location)  
  PORTD ^= (-((buf[0]>>CAN_0)&BIT_1_MASK)^PORTD)&(1U<<PD0); //Brake Light
  PORTC ^= (-((buf[0]>>CAN_1)&BIT_1_MASK)^PORTC)&(1U<<PC5); //FP
  
  PORTC ^= (-((buf[0]>>CAN_3)&BIT_1_MASK)^PORTC)&(1U<<PC0); //RTD
  PORTD ^= (-((buf[0]>>CAN_4)&BIT_1_MASK)^PORTD)&(1U<<PD7); // Micro Squirt
  PORTD ^= (-((buf[0]>>CAN_5)&BIT_1_MASK)^PORTD)&(1U<<PD2); //Starter
  PORTD ^= (-((buf[0]>>CAN_6)&BIT_1_MASK)^PORTD)&(1U<<PD5); //FuelPump
  PORTD ^= (-((buf[0]>>CAN_7)&BIT_1_MASK)^PORTD)&(1U<<PD1); //Shift DOWN
  PORTC ^= (-((buf[1]>>CAN_1)&BIT_1_MASK)^PORTC)&(1U<<PC3); // Brake
  PORTC ^= (-((buf[1]>>CAN_2)&BIT_1_MASK)^PORTC)&(1U<<PC4); // Startup
}

void handleCANMessage(void)
{
  if (CAN_MSGAVAIL == CAN.checkReceive())
  {
      uint8_t buf[8];
      uint8_t len = 0;
      CAN.readMsgBuf(&len, buf);
      ioRegisterManipulation(len, buf);
  }
}

void setup()
{
  Serial.begin(115200);

  uint8_t bPortRegister = 0b00000001;
  uint8_t cPortRegister = 0b00111001;
  uint8_t dPortRegister = 0b11100111;

  toggleBPortRegisterIO(bPortRegister);
  toggleCPortRegisterIO(cPortRegister);
  toggleDPortRegisterIO(dPortRegister);
  
  while(1)
  {
    if(CAN_OK == CAN.begin(CAN_500KBPS))
    {
      Serial.println("CAN BUS INIT GOOD");
      break;
    }
    else
    {
      Serial.println("CAN BUS INIT FAIL, RETRY");
      delay(100);
    }
  }

  CAN.init_Mask(0, 0, 0xFFF);                            // must set both masks; use standard CAN frame
  CAN.init_Mask(1, 0, 0xFFF);                            // must set both masks; use standard CAN frame
  CAN.init_Filt(0, 0, CAN_REAR_POWER_DIST_MSG_ADDRESS);  // filter 0 for receive buffer 0
  CAN.init_Filt(2, 0, CAN_REAR_POWER_DIST_MSG_ADDRESS);  // filter 1 for receive buffer 1 
}

void loop()
{
  handleCANMessage();
}
