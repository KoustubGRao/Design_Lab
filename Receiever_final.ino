#include <SPI.h>
#include <LoRa.h>

#define SS_PIN 10
#define RST_PIN 9
#define DI0_PIN 2

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!LoRa.begin(865E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  // Change LoRa receiver frequency if necessary
  LoRa.setFrequency(865E6);
}

void loop() {
  // Receive packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    Serial.println("Received packet:");
    // read packet
    String dataToSend="";
    while (LoRa.available()) {
      dataToSend+=(char)LoRa.read();
      
    }
    
    Serial.println(dataToSend);
    
    String message = "curl -d \"key=3T0GOGUI0OITK9SD&field1=" + dataToSend + "\" -k http://api.thingspeak.com/update";
    // Set the last byte of data as the data to be sent
    Serial.println(message);
    Serial.println();
    delay(5000);
  }
}
