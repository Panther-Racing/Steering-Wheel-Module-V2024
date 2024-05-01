 #include <SPI.h>
#include <mcp2515.h>

bool DEBUG=true;
const unsigned long dataRateHZ=20;

const unsigned long interval = 1000/dataRateHZ; //interval in milliseconds
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
int i = 0;
unsigned int firmwareVersion=120;

struct can_frame frame;
//struct can_frame canMsg2;
MCP2515 mcp2515(8);


void setup() {
  pinMode(A0, INPUT);
  digitalWrite(A0, HIGH);

  pinMode(A1, INPUT);
  digitalWrite(A1, HIGH);

  pinMode(A2, INPUT);
  // digitalWrite(A2, HIGH);

  // pinMode(5, INPUT);
  // digitalWrite(PD5, HIGH);
  
  // //pinMode(A3, INPUT);
  // pinMode(A3, INPUT);
  // //digitalWrite(A3, HIGH);
    
  if(DEBUG) { Serial.begin(115200); }
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_1000KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();
  
  if(DEBUG) { Serial.println("Example: Write to CAN"); }
}

void message1() {

  //unsigned input = 0x3FF; //placeholder to test message transmission
  
  unsigned input = map(analogRead(A2), 0, 1023, 0, 255);
  frame.can_id  = 0xE0;
  frame.can_dlc = 3;

  //this is going to seeem like it doesnt match the document but it does 
  //  because of how the values are shifted into the CAN registers. 
  unsigned int bitfield = digitalRead(A1);
      bitfield |= digitalRead(A2) << 1;
      bitfield |= digitalRead(A3) << 2;
      bitfield |= digitalRead(5) << 3;
      bitfield |= i << 4;
      i++;

  frame.data[0] = bitfield;
  frame.data[1] = input;
  frame.data[2] = firmwareVersion;
  mcp2515.sendMessage(&frame);
  if(DEBUG) { Serial.println("Message Sent"); }
}


void loop() {
  currentMillis=millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    message1();
  }

}
