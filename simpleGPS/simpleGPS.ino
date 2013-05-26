//#include <string.h>
#include <ctype.h>
#include <SoftwareSerial.h>
#include <SD.h>

#define SoftRXPIN 2
#define SoftTXPIN 3

// Set this value equal to the baud rate of your Serial
#define TERMBAUD  115200
// Set this value equal to the baud rate of your GPS
#define GPSBAUD  4800

SoftwareSerial uart_gps(SoftRXPIN, SoftTXPIN); 
File myFile;

int ledPin = 13;                  // LED test pin
int byteGPS=-1;
char gpsBUFFER[83] = "";
int countBUFFER = 0;

void setup() {
  // Initialize LED pin
  pinMode(ledPin, OUTPUT);
  // Sets baud rate of your terminal program
  Serial.begin(TERMBAUD);
  // Sets baud rate of your GPS
  uart_gps.begin(GPSBAUD);
  Serial.print("Initializing SD card...");

  // pin 10 on most Arduino boards, 53 on the Mega must be left as an output 
  // or the SD library functions will not work. 
  pinMode(10, OUTPUT);

  if (!SD.begin(4))
  {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  if (SD.exists("GPS_DATA.txt")) 
  {
    Serial.println("GPS_DATA.txt exists."); 
  }
  else 
  {
    Serial.println("GPS_DATA.txt doesn't exist. It will be created");
    Serial.println("Creating GPS_DATA.txt...");
    myFile = SD.open("GPS_DATA.txt", FILE_WRITE);
    myFile.close();
  }
}

void loop() {
  digitalWrite(ledPin, HIGH);

  while(uart_gps.available()) {      // If data available 
    byteGPS=uart_gps.read();         // Read a byte of the software serial port
    //Serial.write(byte(byteGPS));     // Write a byte of the serial port

    if (byteGPS==13){ 
      gpsBUFFER[countBUFFER] = byteGPS;
      gpsBUFFER[countBUFFER+1] = '\0';  //termination of String

      myFile = SD.open("GPS_DATA.txt",FILE_WRITE);
      if (myFile){
        myFile.print(gpsBUFFER);
        myFile.close();
      }
      else
      {
        Serial.println("error opening GPS_DATA.txt");
      } 

      Serial.print(gpsBUFFER);

      countBUFFER = 0;
    }
    else{
      gpsBUFFER[countBUFFER] = byteGPS;
      countBUFFER++;
    }
  }
}


