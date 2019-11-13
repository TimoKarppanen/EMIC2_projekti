#include <SoftwareSerial.h>
#include <dht11.h>

#define SOFTSERIAL_RX_PIN  2
#define SOFTSERIAL_TX_PIN  3
#define rxPin 10              //Emic2
#define txPin 11              //Emic2
#define ledPin 13             //Led
#define ledBrightness 9       //AnalogWrite Led
#define DHT11PIN 4            //DHT PIN

SoftwareSerial softSerial(SOFTSERIAL_RX_PIN,SOFTSERIAL_TX_PIN);
SoftwareSerial emicSerial = SoftwareSerial(rxPin, txPin);

void waitForCommand();
void checkLimits();
void ledOff();
void ledOn();
void executeCommand(char command);
float getHumidity();
float getTemperature();

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
  pinMode(SOFTSERIAL_TX_PIN, OUTPUT);                                      //Led output
  pinMode(SOFTSERIAL_RX_PIN, INPUT);
  pinMode(txPin, OUTPUT);
  Serial.begin(9600);
  emicSerial.begin(9600);        
  softSerial.begin(9600);
  
  emicSerial.print('\n');
  emicSerial.listen();

  while (emicSerial.read() != ':');
  
  delay(10);
  emicSerial.flush();
}

void loop()
{
  waitForCommand();
  checkLimits();
}

void waitForCommand()
{
   delay(10);
   Serial.println("test");
   char cmd;
   softSerial.print('\n');
   softSerial.listen();
   delay(10);
   

   if (softSerial.available())
   {  
      Serial.println("jeejee");
      cmd = softSerial.read();
      Serial.println(voiceBuffer[cmd - 1]);
      executeCommand(cmd);
   }
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
  Serial.println("check limits");
  if (getTemperature() > 16)                                      //Set temperature limit
    {
      emicSerial.listen();
      emicSerial.print('S');
      emicSerial.print("Hello. Its way too warm in here.");       //EMIC2 notificates if the temperature limit has been exceeded
      emicSerial.print('\n');
      digitalWrite(ledPin, HIGH);
      Serial.println("lool");//Lights up a led in EMIC2
      while (emicSerial.read() != ':');
          digitalWrite(ledPin, LOW);
      Serial.println("check limits2");
    }
   
   else if (getHumidity() > 29)                                   //Set humidity limit
   {
      delay(2000);
      ledOn();
      emicSerial.listen();
      emicSerial.print('S');
      emicSerial.print("The humidity is way too high in here");   //EMIC2 notificates if the humidity limit has been exceeded
      emicSerial.print('\n');
      Serial.println("lool");
      digitalWrite(ledPin, HIGH);                                 //Lights up a led in EMIC2
      
      while (emicSerial.read() != ':');
          digitalWrite(ledPin, LOW);
      Serial.println("check limits2");
   }
 
}

void ledOff()
{
  analogWrite(ledBrightness,0);
  Serial.println("ledi pois");
}

void ledOn()
{
  analogWrite(ledBrightness,255);
  Serial.println("ledi päälle");
}

void executeCommand(char command)
{
  switch(int(command))
  {
    case 20:
      ledOn();
      break;
    case 21:
      ledOff();
      break;
    default:
      Serial.println("Muu komento");
  }
}
