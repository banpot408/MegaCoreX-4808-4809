#include <OneWireMega480x.h>

OneWireMega480x  ds1(16);  // on pin 10 (a 4.7K resistor is necessary)
OneWireMega480x  ds2(17);  // on pin 10 (a 4.7K resistor is necessary)


const int ledPin =  7;// the number of the LED pin
int ledState = LOW;             // ledState used to set the LED
unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 1000;           // interval at which to blink (milliseconds)


//byte type_s;

byte addr1[8];
byte addr2[8];
  
void req_sensor1(void);
void req_sensor2(void);

float get_senser1(void);
float get_senser2(void);

void req_sensor1(void){
  Serial.println("FUNC  req_sensor1.");
  Serial.println();
  
  if ( !ds1.search(addr1)) {
    Serial.println("FUNC  No more addresses1.");
    Serial.println();
    ds1.reset_search();
    delay(250);
    return;
  }
  
  ds1.reset();
  ds1.select(addr1);
  ds1.write(0x44, 1);        // start conversion, with parasite power on at the end
}

void req_sensor2(void){
  Serial.println("FUNC  req_sensor2.");
  Serial.println();
  
  if ( !ds2.search(addr2)) {
    Serial.println("FUNC  No more addresses2.");
    Serial.println();
    ds2.reset_search();
    delay(250);
    return;
  }
  
  ds2.reset();
  ds2.select(addr2);
  ds2.write(0x44, 1);        // start conversion, with parasite power on at the end
}







float get_senser1(void){
  byte i;
  byte present = 0;
  byte data_sensor[12];
  float celsius;
  
  present = ds1.reset();
  ds1.select(addr1);    
  ds1.write(0xBE);         // Read Scratchpad

  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data_sensor[i] = ds1.read();
  }
  
  int16_t raw = (data_sensor[1] << 8) | data_sensor[0];

  byte cfg = (data_sensor[4] & 0x60);
  if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
  else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
  else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms

  celsius = (float)raw / 16.0;
  
  Serial.print("FUNC  Temperature1 = ");
  Serial.print(celsius);
  Serial.println(" Celsius, ");

  return celsius ;
}

float get_senser2(void){
  byte i;
  byte present = 0;
  byte data_sensor[12];
  float celsius;
  
  present = ds2.reset();
  ds2.select(addr2);    
  ds2.write(0xBE);         // Read Scratchpad

  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data_sensor[i] = ds2.read();
  }
  
  int16_t raw = (data_sensor[1] << 8) | data_sensor[0];

  byte cfg = (data_sensor[4] & 0x60);
  if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
  else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
  else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms

  celsius = (float)raw / 16.0;
  
  Serial.print("FUNC  Temperature2 = ");
  Serial.print(celsius);
  Serial.println(" Celsius, ");

  return celsius ;
}


void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void loop() {

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {

    previousMillis = currentMillis;

    if (ledState == LOW) {
      ledState = HIGH;
      req_sensor1();
      req_sensor2();
    } else {
      ledState = LOW;
      float temp1 = get_senser1();
      float temp2 = get_senser2();
      Serial.print("LOOP  Temperature1 = ");
      Serial.print(temp1);
      Serial.print("  Temperature2 = ");
      Serial.print(temp2);
      Serial.println(" Celsius, ");
    }

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
  }
}
