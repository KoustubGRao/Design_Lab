#include <SPI.h>
#include <LoRa.h>

#define SS_PIN 10
#define RST_PIN 9
#define DI0_PIN 2

#define sensor_1_pin A0
#define sensor_2_pin A4

unsigned int count=1;
int prevcombinedbits =0;
bool flag = false;

int solve(int bits, bool &ack, int prev)
{
    if (bits == 3)
    {
        ack = false;
        return 0;
    }
    else if (bits == 1)
    {
        if (ack == false)
        {
            ack = true;
            return 5;
            
        }
        else
        {
            return 2;
        }
    }
    return 0;
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!LoRa.begin(865E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa End Node Start --");
  Serial.println("    Sensor Data Sending\n");
}

void loop() {
  
  // Send a packet
  delayMicroseconds(30);

    delayMicroseconds(30);
    Serial.print("###########    ");
    Serial.print("COUNT=");
    Serial.print(count);
    Serial.println("    ###########");
    count++;

    // Read sensor values and assign bits
    int Sn1 = analogRead(sensor_1_pin);
    delayMicroseconds(100);
    int Sn2 = analogRead(sensor_2_pin);
    delayMicroseconds(100);
    int bit1= Sn1>880 ? 1:0;
    int bit2=Sn2>880 ? 1: 0;
    // Combine the bits into one byte
    int combinedBits = (bit1 << 1)|bit2;
    

    // Decide data to be sent based on acknowledgment status and combined bits
    int dataToSend = 0;
    dataToSend = solve(combinedBits, flag, prevcombinedbits);
    prevcombinedbits = combinedBits;

    // Print sensor values and combined bits
    if(bit1){
      Serial.println("Main Power On");
    }else{
      Serial.println("Main Power Off");
    }
    if(bit2){
      Serial.println("Generator Power On");
    }
    else{
      Serial.println("Generator Power Off");
    }
    
    
    // Print data to be sent
    Serial.print("Data to be sent: ");
    Serial.println(dataToSend);

  if(dataToSend == 5){
    for(int i = 0 ; i < 5 ; i++){
      LoRa.beginPacket();
      LoRa.print(dataToSend);
      LoRa.endPacket();
      delay(1000);
    }
  }
  LoRa.beginPacket();
  LoRa.print(dataToSend);
  LoRa.endPacket();
  delay(15000);
  Serial.println("");
}