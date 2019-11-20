#include <SoftwareSerial.h>
#include <dht11.h>

#define SOFTSERIAL_RX_PIN  2
#define SOFTSERIAL_TX_PIN  3
#define rxPin 10              //Emic2
#define txPin 11              //Emic2
#define ledPin 13             //Led
#define ledBrightness 9       //AnalogWrite Led
#define DHT11PIN 4            //DHT PIN

SoftwareSerial softSerial(SOFTSERIAL_RX_PIN,SOFTSERIAL_TX_PIN);   // SoftwareSerial for Groovre
SoftwareSerial emicSerial = SoftwareSerial(rxPin, txPin);         // SoftwareSerial for EMIC

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
  pinMode(SOFTSERIAL_TX_PIN, OUTPUT);    // PinMode for Groove                                  //Led output
  pinMode(SOFTSERIAL_RX_PIN, INPUT);     // PinMode for Groove
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
  waitForCommand(); // After led closes off or turns on the program starts from here.
  checkLimits();
  
}

void waitForCommand()
{
   delay(10);
   Serial.println("Waiting for a command");
   char cmd;
   softSerial.print('\n');
   Serial.println("debugging");
   softSerial.listen();
   Serial.println("xxx");

  
   

   // EI SUORITA IF LAUSETTA //.
  
   if (softSerial.available()) // Groove gets stuck
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
  Serial.println("executing check limits function");
  if (getTemperature() > 16)                                      //Set temperature limit
    {
      emicSerial.listen();
      emicSerial.print('S');
      Serial.println("");
      emicSerial.print("Hello. Its way too warm in here.");       //EMIC2 notificates if the temperature limit has been exceeded
      emicSerial.print('\n');
      digitalWrite(ledPin, HIGH);
      while (emicSerial.read() != ':'); // Code gets stuck to this loop              // Reads incoming serial data.
          digitalWrite(ledPin, LOW);  // closes up EMIC2 led
      Serial.println("check limits of temperature");
      ledOn();
      delay(5000);
      ledOff();             // Program stops here. It's supposed to go on to check the humidity levels
    }
   
   else if (getHumidity() > 20)                                   //Set humidity limit
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
      Serial.println("check limits of humidity");
   }
 
}

void ledOff()
{
  analogWrite(ledBrightness,0);
  Serial.println("**LED TURNS OFF**");
}

void ledOn()
{
  analogWrite(ledBrightness,255);
  Serial.println("***LED TURNS ON**");
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
