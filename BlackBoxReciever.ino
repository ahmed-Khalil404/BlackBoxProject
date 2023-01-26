// library SPI et de de carte ds

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <LiquidCrystal_I2C.h>

#include <SoftwareSerial.h>

LiquidCrystal_I2C lcd(0x27,20,4);

#define LED_LB 2


SoftwareSerial mySerial (9, 10);


// 
#define vent 7 
RF24 radio(9, 10); // CE, CSN

const byte address[6] = "00001";
char val;   

void setup() {

  mySerial.begin(9600);
   lcd.init();
   lcd.backlight();
   lcd.setCursor(0,0);
   lcd.print("Integration final");
   lcd.clear();       
  pinMode(LED_LB, OUTPUT);
  

  
    Serial.begin(9600);
radio.begin();
radio.openReadingPipe(0, address);   //Setting the address at which we will receive the data
radio.setPALevel(RF24_PA_MAX);       //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
radio.startListening();              //This sets the module as receiver
Serial.println("Program started functionning");
}

 


void loop()
{
if (radio.available())              
{

char temp_str[40] = "";                 
radio.read(&temp_str, sizeof(temp_str));    
Serial.println(temp_str);
Serial.println(temp_str);

      lcd.init();
      lcd.setCursor(1,0);
      lcd.print("temperature");
      lcd.setCursor(6,1);
      lcd.print(temp_str);
      
    
      if (!strcmp(temp_str,"1"))
      {
          digitalWrite(vent,HIGH);
          delay(5000);
          digitalWrite(vent,LOW);
                    delay(5000);


          lcd.setCursor(0,0);
          lcd.print("Alert Temperature");

      }
                digitalWrite(vent,LOW);

}
 if (Serial.available()){
      val = Serial.read();//то "val" дорівнює "Serial.read"
      Serial.println(val);
      
      if ( val == '1') //Якщо  "val" рівне 1 ,
      {
        digitalWrite(LED_LB, HIGH); //то на 13-тий пін подається напруга (Лампочка загорається)
      }
      
      if ( val == '0')//Якщо  "val" рівне 0 ,
      {
        digitalWrite(LED_LB, LOW);//то на 13-тий пін не подається напруга (Лампочка гасне)
      }
    }

  

}