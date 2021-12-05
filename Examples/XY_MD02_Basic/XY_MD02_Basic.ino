//#include <SoftwareSerial.h>
#include <ModbusMaster.h>

//SoftwareSerial mySerial(2, 3); // RX, TX

#define MAX485_DE      4
#define MAX485_RE_NEG  5

// instantiate ModbusMaster object
ModbusMaster node;

void setup()
{
  
  Serial.begin(9600);
  Serial.println("start init serial 0");  

  Serial2.swap(1);
  Serial2.begin(9600);
  Serial.println("start init software serial");
  
  // Modbus slave ID 1
  node.begin(2, Serial2);
  
}

void loop()
{
  uint8_t result;
  uint16_t data[2]; // prepare variable of storage data from sensor
   
  Serial.println("get data");
  result = node.readInputRegisters(1, 2); // Read 2 registers starting at 1)
  if (result == node.ku8MBSuccess)
  {
    Serial.print("Temp: ");
    Serial.print(node.getResponseBuffer(0)/10.0f);
    Serial.print("  Humi: ");
    Serial.println(node.getResponseBuffer(1)/10.0f);
  }
  delay(1000);
}
