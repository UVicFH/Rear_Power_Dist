#include <mcp_can.h>
#include <mcp_can_dfs.h>
#include <TimerOne.h>

#include "Pin_Defines.h"

MCP_CAN CAN(SPI_CS_PIN);

void set_outputs(byte len, byte* buf)
{
  
  // Set each digital
  digitalWrite(BRAKE_LIGHT_PIN, buf[BRAKE_LIGHT_BYTE]>>BRAKE_LIGHT_BIT&MASK_1);
  digitalWrite(FUEL_PUMP_PIN, buf[FUEL_PUMP_BYTE]>>FUEL_PUMP_BIT&MASK_1);
  digitalWrite(FUEL_PUMP_PIN, buf[FUEL_PUMP_BYTE]>>FUEL_PUMP_BIT&MASK_1);
  digitalWrite(FUEL_PUMP_PIN, buf[FUEL_PUMP_BYTE]>>FUEL_PUMP_BIT&MASK_1);
  digitalWrite(FUEL_PUMP_PIN, buf[FUEL_PUMP_BYTE]>>FUEL_PUMP_BIT&MASK_1);
  digitalWrite(FUEL_PUMP_PIN, buf[FUEL_PUMP_BYTE]>>FUEL_PUMP_BIT&MASK_1);
  digitalWrite(FUEL_PUMP_PIN, buf[FUEL_PUMP_BYTE]>>FUEL_PUMP_BIT&MASK_1);
  digitalWrite(FUEL_PUMP_PIN, buf[FUEL_PUMP_BYTE]>>FUEL_PUMP_BIT&MASK_1);

  // Set the pwm on the fan output
  word faninput = buf[2]*4;
  Timer1.pwm(ENGINE_FAN_PIN, faninput); // (pin, output out of 1024)
}

void setup(){

  // Initialize a timer with 25 us cycles (40kHz) for strobing the transistor on the fan output
  Timer1.initialize(25);

  // Create an infinite loop to prevent the program from starting before CAN is established
  for(;;)
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

  // Set both masks to check all digits (compare the entire ID to the filter) of the arbitration IDs
  CAN.init_Mask(0, 0, 0xFFF);
  CAN.init_Mask(1, 0, 0xFFF);

  // Set the first filter on each buffer to only accept the desired arbitration ID
  CAN.init_Filt(0, 0, CAN_REAR_POWER_DIST_MSG_ADDRESS);
  CAN.init_Filt(2, 0, CAN_REAR_POWER_DIST_MSG_ADDRESS);
}

void loop(){

  // If there is a message available in the buffer
  if (CAN_MSGAVAIL == CAN.checkReceive())
  {

    // Create a buffer and insert the three byte message into it
    byte buf[8];
    byte len = 3;
    CAN.readMsgBuf(&len, buf);

    // Set the outputs of the board given the 
    set_outputs(len, buf);
    
  }
  
}
