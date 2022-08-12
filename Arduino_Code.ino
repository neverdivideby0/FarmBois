//HPCIC Arduino code
//By:Shen LieZhou

//#include <SPI.h>
//#include <Wire.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>
#include <dht.h>

//#define screen_width 128
//#define screen_height 64
//#define oled_reset 4
//Adafruit_SSD1306 display(screen_width, screen_height, &Wire, oled_reset);
#define temp_and_humid_sensor A0 
#define light_sensor A1
#define soil_moisture_sensor A2
//float daylight = 30; //collect reading
//float moonlight = 800; //collect reading
//float led_on_reading = 100; //collect reading by oning led in the dark
float dry_soil = 890; //collect reading
float wet_soil = 690; //collect reading
int fan = 3; //relay pin IN1
int led = 7; //relay pin IN3
int pump = 8; //relay pin IN2

dht DHT;
 
void setup(){
  pinMode(3,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  Serial.begin(9600);
  
//  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
//  display.clearDisplay();
//  display.setTextSize(1); //1:1 pixel scale
//  display.setTExtcolor(SSD1306_WHITE);
//  display.setCursor(0,0);
  
  delay(500);//Delay to let system boot
  Serial.println("FarmBox \n Revolutionising farming one box at a time \n \n");
//  display.println("FarmBox \n Revolutionising farming one box at a time \n \n");
  delay(1000);//Wait before accessing Sensor

}
 
void loop(){

  DHT.read11(temp_and_humid_sensor);
  float temp_reading = DHT.temperature;
  float humidity_reading = DHT.humidity;
  //temp banding would be from 30 to 32
  if(temp_reading>32) {
    digitalWrite(4,LOW); //fan on
  }
  else if(temp_reading<30) { 
    digitalWrite(4,HIGH); //fan off
  }

 

  float light_reading = analogRead(A1);
  float light_percentage = map(light_reading, daylight, moonlight, 0, 100); //might want to convert this to lux if possible via phone light sensor as bridge
  if(light_reading>500) {
    digitalWrite(7,LOW); //led on
  }
  else if(light_reading<470) {
    digitalWrite(7,HIGH); //led off
  }

  

  float soil_moisture_reading = analogRead(A2);
  float soil_moisture_percentage = map(soil_moisture_reading, dry_soil, wet_soil, 0, 100);
  if(soil_moisture_percentage<30) {
    digitalWrite(8,LOW);
  }
  else if(soil_moisture_percentage>40) {
    digitalWrite(8,HIGH);
  }

  

  Serial.print("Current humidity/% = ");
  Serial.print(humidity_reading);
  Serial.print("%  ");
  Serial.print("Temperature/C = ");
  Serial.print(temp_reading); 
  Serial.print("C  ");
  Serial.print("Light reading/Lux = ");
  Serial.print(light_reading);
  Serial.print("%  ");
  Serial.print("Soil moisture reading/% = ");
  Serial.print(soil_moisture_percentage);
  Serial.println("%  ");
  
}
