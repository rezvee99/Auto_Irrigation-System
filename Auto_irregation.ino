#include <LiquidCrystal.h>

#include "SIM900.h"
#include <SoftwareSerial.h>

#include "sms.h"
SMSGSM sms;

int de=0;

char sms_position;
boolean started = false;
char sms_text[100];
int i;
char phone_number[20] = "01815718918";
//char phone_number[20] = "01618359866";


// initialize the library with the numbers of the interface pins

LiquidCrystal lcd(9, 10, 6, 8, 7, 5);

int Auto_on=0;
int Auto_off=0;


String cmd;
String cmd2;


void del() {
  for (i = 1; i <= 20; i++)
  {
    sms.DeleteSMS(i);
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(A2, INPUT);
  pinMode(A5, OUTPUT);
  lcd.begin(16, 2);
    lcd.setCursor(0, 0);
    lcd.print("   Automatic");
     lcd.setCursor(0, 1);      
        lcd.print("Irrigation System");
        delay(1000);
        

  Serial.begin(9600);
  Serial.println("GSM Shield testing.");
  if (gsm.begin(4800)) {
    Serial.println("\nstatus=READY");
   lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("System Ready");
    started = true;
  }
  if (started) {
    if (sms.SendSMS(phone_number, "Device is Ready")) {
      Serial.println("\nSMS sent OK");
    }
    del();

  }

}

void manual() {

          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("MANUAL MODE");
  while(1){

  if (started)
  {
    sms_position = sms.IsSMSPresent(SMS_UNREAD);
    if (sms_position)
    {
      Serial.print("SMS postion:");
      Serial.println(sms_position, DEC);
      sms.GetSMS(sms_position, phone_number, sms_text, 100);
      Serial.println(phone_number);
      Serial.println(sms_text);



      cmd = sms_text;



      if (cmd == "PUMPON") {
        digitalWrite(A5, 1);
        if (sms.SendSMS(phone_number, "Pump truned On")) {
          Serial.println("\nSMS sent OK");
          lcd.setCursor(0, 1);
          lcd.print("Pump truned  On");
        }
        del();
      }
      if (cmd == "PUMPOFF") {
        digitalWrite(A5, 0);
        if (sms.SendSMS(phone_number, "Pump truned OFF")) {
          Serial.println("\nSMS sent OK");
          lcd.setCursor(0, 1);
          lcd.print("Pump truned Off");
        }
        del();
      }

      if (cmd == "AUTO") {
        if (sms.SendSMS(phone_number, "automatic mode activeted "));
        automatic();
      }

    }
    else {
      Serial.println("no new Msg");
    }

  }


}
}


void automatic() {

  lcd.clear();

  while(1){

  int a = analogRead(A2);

  lcd.setCursor(0, 0);
  lcd.print(a);
  delay(500);
  lcd.clear();


  if (a > 450)
  {
    lcd.setCursor(0, 1);
    lcd.print("Pump On");
    digitalWrite(A5, 1);
    if (started) {

     if(Auto_on==0)
     {
      if (sms.SendSMS(phone_number, "moscier Low,Pump truned On")) {
        Serial.println("\nSMS sent OK");
      }
        Auto_on++;
        Auto_off=0;
      
       }
      
    }
    delay(200);
  }
  else
  {
    lcd.setCursor(0, 1);
    lcd.print("Pump Off");
    digitalWrite(A5, 0);
    if(Auto_off==0)
    {
    if (sms.SendSMS(phone_number, "moscier High,Pump truned off")) {
      Serial.println("\nSMS sent OK");
    }
    Auto_on=0;
    Auto_off++;
    
    }
    delay(200);
  }


   if (started)
  {
    sms_position = sms.IsSMSPresent(SMS_UNREAD);
    if (sms_position)
    {
      lcd.clear();
      Serial.print("SMS postion:");
      Serial.println(sms_position, DEC);
      sms.GetSMS(sms_position, phone_number, sms_text, 100);
      Serial.println(phone_number);
      Serial.println(sms_text);



      cmd2 = sms_text;

       if (cmd2 == "MANUAL") {
        if (sms.SendSMS(phone_number, "manual mode activeted ,Pls Command"));
        manual();
      }



}

  }
  }
}



void loop() {
  // put your main code here, to run repeatedly:
 
 
        if (sms.SendSMS(phone_number, "automatic mode activeted "));
        automatic();
      

}
