// library SPI et de de carte ds
#include <SPI.h>

#include <nRF24L01.h>

#include <RF24.h>
#include <Wire.h>
// library de base de software console arduino 
#include <SoftwareSerial.h>
// library de protocol i2c

// Initiation des port CE,CSN pour la connection wifi 
RF24 radio(9, 10); // CE, CSN  
  
// Adresse q'on va utiliser pour savoir envoyer a qui
const byte address[6] = "00001";     //Byte of array representing the address. This is the address where we will send the data. This should be same on the receiving side.

//Adresse utiliser dans le GYRO
// Adresse de capteur de temperature 1ére adresse valable
#define slave1 0x48 
const int MPU_ADDR=0x68;
int16_t accelerometer_x, accelerometer_y, accelerometer_z;
char tmp_str[40]; // temporary variable used in convert function
 char* convert_int16_to_str(int16_t i)
 { // converts int16 to string. Mo reover, resulting strings will have the same length in the debug monitor.
    sprintf(tmp_str, "%6d", i); 
   return tmp_str;
 }
// Les variable de changement brusque des axe x,y,z


// Fonction de convertissement statique les valeur de type int en un chaine de charactere
 
void setup() {


Serial.begin(9600);

// Configuration i2c de la capture de temperature 
  Wire.begin(); 
  Wire.beginTransmission(slave1);
  Wire.write(0xAC); //entree dans le config
  Wire.write(0);  //set for continious loop
  Wire.beginTransmission(slave1);
  Wire.write(0xEE); 
  Wire.endTransmission(); 



radio.begin();                  //Starting the Wireless communication
radio.openWritingPipe(address); //Setting the address where we will send the data
radio.setPALevel(RF24_PA_MIN);  //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
radio.stopListening();          //This sets the module as transmitter


 Wire.begin();
  Wire.beginTransmission (MPU_ADDR); // Commencé la transmission de la composent de gyroscope
  Wire.write (0x6B);// Registre PWR_MGMT_1 de configuraton initiation                
  Wire.write (0);// set to zero (wakes up the MPU-6050)
  Wire.endTransmission (true);
}



void loop()
{

   // Initiation de la recuperation des données de gyroscope
  Wire.beginTransmission (MPU_ADDR);
  Wire.write (0x3B); // starting with register 0x3B (ACCEL_XOUT_H) MPU-6000 and MPU-6050 Register Map and Desc riptions Revision 4.2, p.40]
  Wire.endTransmission (false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom (MPU_ADDR, 7*2, true); // request a total of 7*2=14 registers
 
  
  //Recuperation des données depuis le premiére esclave dans notre systeme le capteur de temperature 
  Wire.beginTransmission(slave1); 
  Wire.write(0xAA); //lire le valeur de la tempereture
  Wire.endTransmission(); 
  Wire.requestFrom(slave1,2);
  int first=Wire.read(); 
  int second=Wire.read(); 
  int temp=first;

    Wire.beginTransmission (MPU_ADDR);
  Wire.write (0x3B); // starting with register 0x3B (ACCEL_XOUT_H) MPU-6000 and MPU-6050 Register Map and Desc riptions Revision 4.2, p.40]
  Wire.endTransmission (false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom (MPU_ADDR, 7*2, true); // request a total of 7*2=14 registers
 

  //Lecture des données de gyro depuis les registre implementer
  accelerometer_x = Wire.read ()<<8 |  Wire.read (); //reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)               0101010101                              
  accelerometer_y = Wire.read ()<<8 | Wire.read (); //reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)                                                      
  accelerometer_z = Wire.read ()<<8 | Wire.read (); //reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
 
 //Verification de la bonne fonctionnement de gyro sur le console arduio 
  Serial.write("xg",accelerometer_x);
  Serial.write("yg",accelerometer_y);
  Serial.write("zg",accelerometer_z);                                          
  
Serial.print("ax = "); Serial.print (convert_int16_to_str(accelerometer_x));
Serial.print(" |aY = "); Serial.print (convert_int16_to_str(accelerometer_y));
Serial.print(" |aZ = "); Serial.print (convert_int16_to_str(accelerometer_z));
   

  
//Envoie de text d'initiation des données 
 
//Envoie de text de la temperature

  char temp_str[20];
  sprintf(temp_str, "%d", temp);
    radio.write(&temp_str, sizeof(temp_str));

    





 
delay(1000);
}


