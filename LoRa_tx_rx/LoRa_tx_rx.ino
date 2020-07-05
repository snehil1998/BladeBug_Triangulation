#include <SPI.h>
#include <LoRa.h>

bool role = 0;                //role 1 = base transceiver, role 0 = mobile transceiver

unsigned long counter = 0;
unsigned long total_time = 0;
bool loop_end = 0;

void setup() {
  Serial.begin(250000);
  while (!Serial);

  Serial.println(F("Round Trip time"));
  LoRa.begin(433E6);

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  if ((role == 1)&&(loop_end==0))  
  {
      int x = 0;
      unsigned long start_time = micros(); 
      // send packet
      LoRa.beginPacket();
      LoRa.print("hello ");
      LoRa.print(counter);
      LoRa.endPacket();

      Serial.println(LoRa.parsePacket());
      while(LoRa.parsePacket()==0){}
      int packetSize = LoRa.parsePacket();
      if (packetSize) {
        unsigned long difference = micros()-start_time;
        if((difference>1000)&&(difference<2000)){
          total_time = total_time + difference;
          counter++;
//          Serial.println(counter);
        }
      }
          
      if(counter==1000){
        loop_end=1;
        total_time = total_time/1000;
        Serial.print("Average time of flight: ");
        Serial.println(total_time);
      }
      // Try again 10ms later
      delay(10);
  }
  
  if ( role==0 )
  {
    // try to parse packet
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
      // received a packet
      
      // print RSSI of packet
      Serial.print("' with RSSI ");
      Serial.println(LoRa.packetRssi());

      // send packet
      LoRa.beginPacket();
      LoRa.print("hello ");
      LoRa.print(counter);
      LoRa.endPacket();
      counter++;
    }
  }
}
