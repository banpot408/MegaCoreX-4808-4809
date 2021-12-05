#include <OneWireMega480x.h>

OneWireMega480x  ds(15);  // on pin 10 (a 4.7K resistor is necessary)


const int ledPin =  7;// the number of the LED pin
int ledState = LOW;             // ledState used to set the LED
unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 1000;           // interval at which to blink (milliseconds)


//byte type_s;

byte addr[8];

  
void req_sensor(void);
float get_senser(void);

void req_sensor(void){
  Serial.println("FUNC  req_sensor.");
  Serial.println();
  
  if ( !ds.search(addr)) {
    Serial.println("FUNC  No more addresses.");
    Serial.println();
    ds.reset_search();
    delay(250);
    return;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
}

float get_senser(void){
  byte i;
  byte present = 0;
  byte data[12];
  float celsius;
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }
  
  int16_t raw = (data[1] << 8) | data[0];

  byte cfg = (data[4] & 0x60);
  if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
  else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
  else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms

  celsius = (float)raw / 16.0;
  
  Serial.print("FUNC  Temperature = ");
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
      req_sensor();
    } else {
      ledState = LOW;
      float temp = get_senser();
      Serial.print("LOOP  Temperature = ");
      Serial.print(temp);
      Serial.println(" Celsius, ");
    }

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
  }
}
