#include <SoftwareSerial.h>
#include <dht11.h>

#define SOFTSERIAL_RX_PIN  2
#define SOFTSERIAL_TX_PIN  3
#define rxPin 10              // Serial input (connects to Emic 2's SOUT pin)
#define txPin 11              // Serial output (connects to Emic 2's SIN pin)
#define ledPin 13             // Most Arduino boards have an on-board LED on this pin
#define ledBrightness 9       // Output pin for 
#define DHT11PIN 4            // DHT PIN

SoftwareSerial softSerial(SOFTSERIAL_RX_PIN,SOFTSERIAL_TX_PIN);
SoftwareSerial emicSerial = SoftwareSerial(rxPin, txPin);

void waitForCommand();
void parseCommand();
float getHumidity();
float getTemperature();
void checkLimits();

dht11 DHT11;

const char *voiceBuffer[] =
{
    "Turn on the light",
    "Turn off the light",
    "Play music",
    "Pause",
    "Next",
    "Previous",
    "Up",
    "Down",
    "Turn on the TV",
    "Turn off the TV",
    "Increase temperature",
    "Decrease temperature",
    "What's the time",
    "Open the door",
    "Close the door",
    "Left",
    "Right",
    "Stop",
    "Start",
    "Mode 1",
    "Mode 2",
    "Go"
};

void setup()
{
    emicSerial.begin(9600);         //set the data rate for the SoftwareSerial port
    emicSerial.print('\n');
      
    Serial.begin(9600);
  
    softSerial.begin(9600);
    softSerial.listen();

    pinMode(ledPin, OUTPUT); 
    pinMode(rxPin, INPUT);
    pinMode(txPin, OUTPUT);
    digitalWrite(ledPin, LOW);      //turn LED off
    
    emicSerial.listen();
    emicSerial.print('\n');
    while (emicSerial.read() != ':');
    Serial.println("jee");
    delay(10);
    emicSerial.flush();
    
}

void loop()
{

waitForCommand();
//checkLimits();
}

void waitForCommand()
{
   char cmd;
   softSerial.listen();
   Serial.println("here");
   if(softSerial.available())
   {  
      cmd = softSerial.read();
      Serial.println(voiceBuffer[cmd - 1]);
      
   } 
}
void parseCommand()
{
}

float getHumidity()
{
  int chkNewValue = DHT11.read(DHT11PIN);
  return (float)DHT11.humidity;
}

float getTemperature()
{
  int chkNewValue = DHT11.read(DHT11PIN);
  return (float)DHT11.temperature;
}

void checkLimits()
{
  if (getTemperature() > 23)   // Set temperature limit
    {
      emicSerial.print('S');
      emicSerial.print("Hello. Its way too warm in here."); // EMIC2 notificates if the temperature limit has been exceeded
      emicSerial.print('\n');
      digitalWrite(ledPin, HIGH); // Lights up a led in EMIC2
      while (emicSerial.read() != ':');
          digitalWrite(ledPin, LOW);
      delay(500);
    }

   else if (getHumidity() > 29) // Set humidity limit
   {
      delay(2000);
      emicSerial.print('S');
      emicSerial.print("The humidity is way too high in here"); // EMIC2 notificates if the humidity limit has been exceeded
      emicSerial.print('\n');
      digitalWrite(ledPin, HIGH); // Lights up a led in EMIC2
      
      while (emicSerial.read() != ':');
          digitalWrite(ledPin, LOW);

      delay(500);
   }
}
