
/* Timo Karppanen 23.10.2019 KAMK

* EMIC2

* DHT11

This program reads the DHT11 temperature/humidity sensor. If the value exceeds a predefined value, EMIC2 will give a notification.


*/



#include <dht11.h>
#include <SoftwareSerial.h>

#define rxPin 10 // Serial input (connects to Emic 2's SOUT pin)
#define txPin 11  // Serial output (connects to Emic 2's SIN pin)
#define ledPin 13 // Most Arduino boards have an on-board LED on this pin

#define DHT11PIN 4 // DHT PIN


dht11 DHT11;

SoftwareSerial emicSerial = SoftwareSerial(rxPin, txPin);


void setup()
{
  Serial.begin(9600);

  pinMode(ledPin, OUTPUT); 
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);

  emicSerial.begin(9600); // set the data rate for the SoftwareSerial port

  digitalWrite(ledPin, LOW); // turn LED off

  emicSerial.print('\n');
  
  while (emicSerial.read() != ':');
  delay(10);
  emicSerial.flush();
  
}

void loop()
{

// Creating a program that detects if the temperature/humidity limit has been exceeded.
  
  Serial.println();

  int chk = DHT11.read(DHT11PIN);

  Serial.print("Humidity (%): ");
  Serial.println((float)DHT11.humidity, 2);

  Serial.print("Temperature (C): ");
  Serial.println((float)DHT11.temperature, 2);
  

  if (DHT11.temperature > 23)   // Set temperature limit
    {
      Serial.println("test");

      delay(2000);

      Serial.println("Hello World");

      emicSerial.print('S');
      emicSerial.print("Hello. Its way too warm in here."); // EMIC2 notificates if the temperature limit has been exceeded
      emicSerial.print('\n');
      digitalWrite(ledPin, HIGH); // Lights up a led in EMIC2
      
      while (emicSerial.read() != ':');
          digitalWrite(ledPin, LOW);

      delay(500);
    }

   else if (DHT11.humidity > 29) // Set humidity limit
   {
       delay(2000);

      Serial.println("Hello World");

      emicSerial.print('S');
      emicSerial.print("The humidity is way too high in here"); // EMIC2 notificates if the humidity limit has been exceeded
      emicSerial.print('\n');
      digitalWrite(ledPin, HIGH); // Lights up a led in EMIC2
      
      while (emicSerial.read() != ':');
          digitalWrite(ledPin, LOW);

      delay(500);
   }
   
    

    else
    {
        delay(10000);
        return;
      
    }

  }

    
