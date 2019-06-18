//##MESSAGES AND REPLY##
//message from user : # ON : 1 : 30 #
//reply from system : "Pump 1 successfuly ON"
//message from user : # ON : 2 #
//reply from system : "Pump 2 successfuly ON"
//message from user : # ON : 3 : 10 #
//reply from system : "Pump 3 successfuly ON"
//message from user : # OFF : 1 #
//reply from system : "Pump 1 successfuly OFF"
//message from user : # OFF : 2 #
//reply from system : "Pump 2 successfuly OFF"
//message from user : # OFF : 3 #
//reply from system : "Pump 3 successfuly OFF"
//message from user : # HUMIDITY #
//reply from system : humidity sensor's data at that time.
//message from system : "Pump 1 or 2 or 3 is off because humidity increases to x"
//store all the message and mobile numbers in memory card

/*
  SD card read/write

 This example shows how to read and write data to and from an SD card file
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)
*/

#include <GSM.h>
#include <SPI.h>
#include <SD.h>

#define PINNUMBER ""

GSM gsmAccess;
GSM_SMS sms;

File messageFile;
int chipSelect = 4;
char senderNumber[20];
String message = "";

void setup(){
  Serial.begin(9600);
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for native USB port only
//  }

  Serial.print("Initializing SD card...");
  pinMode(chipSelect, OUTPUT);
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization SD card failed!");
//    return;
  }
  Serial.println("initialization SD card done.");
  
  Serial.println("SMS Messages Receiver");
  boolean notConnected = true;
  while(notConnected){
    if(gsmAccess.begin(PINNUMBER) == GSM_READY){
      notConnected = false;
    }else{
      Serial.println("Not connected");
      delay(1000);
    }
  }
  Serial.println("GSM initialized");
  Serial.println("Waiting for messages");
}

void loop(){
  
  if(sms.available()){
    
    if(sms.peek() == '#'){
      Serial.print("Message received from : ");
      sms.remoteNumber(senderNumber, 20);
      Serial.println(senderNumber);
      char c;
      while(c = sms.read()){
          message += c;
      }
      Serial.print("message is : ");
      Serial.println(message);
      Serial.println("<---start reply to last number--->");
      sms.beginSMS(senderNumber);
      sms.print("Thank you for message.");
      sms.endSMS(); 
      Serial.println("<---end reply to last number--->");

      Serial.println("<---start write message to file--->");
      messageFile = SD.open("messages.txt", FILE_WRITE);
      if(messageFile){
        Serial.print("Writing to messages.txt...");
        messageFile.println(senderNumber);
        messageFile.close();
        Serial.println("done.");
      }else{
        Serial.println("error opening messages.txt");
      }
      Serial.println("<---end write message to file--->");
    }

    //for smaple reading file
    Serial.println("<---start read message from file--->");
    messageFile = SD.open("messages.txt", FILE_READ);
    if(messageFile){
      Serial.println("messages.txt : ");
      while(messageFile.available()){
        Serial.write(messageFile.read());
      }
      messageFile.close();
    }else{
      Serial.println("error opening messages.txt");
    }
    Serial.println("<---end read message from file--->");

    sms.flush();
    Serial.println("MESSAGE DELETED...");
  }
  delay(1000);
}
