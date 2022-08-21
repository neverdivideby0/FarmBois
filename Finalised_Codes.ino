//HPCIC Arduino code
//By:Shen LieZhou

//Import Packages
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Wire.h>
#include <BH1750.h>
#include <dht.h>

//OLED Set-up
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_ADDR 0x3C
Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT);

#define temp_and_humid_sensor A0 
#define soil_moisture_sensor A1
float dry_soil = 890; //Collected reading
float wet_soil = 690; //Collected reading
int fan = 4; //Relay pin IN1
int pump = 7; //Relay pin IN2
int led = 8; //Relay pin IN3

dht DHT; //Initialize DHT Module
BH1750 GY302; //Initialize BH1750 Module

void setup(){
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(4,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  Serial.begin(9600);
  GY302.begin(); //Initialize GY-302 module
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR); //Initialize OLED display
  
  display.clearDisplay();
  display.setTextSize(0.5);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  display.println("FarmBox 1.0");
  display.setCursor(0, 10);
  display.println("Revoluntionising farming one box at a time");
  display.setCursor(0,30);
  display.println("Developed by: ");
  display.println("Team FarmBois of NUS");
  display.display();

  delay(5000);  //Wait before accessing Sensor
}
 
void loop(){
  
  //Temperature and Humidity Function
  bool fan_status = false;
  DHT.read11(temp_and_humid_sensor);
  float temp_reading = DHT.temperature;
  float humidity_reading = DHT.humidity;

  if(temp_reading>32) {  //Temp banding would be from 30 to 32
    digitalWrite(fan,LOW); //Fan on
    fan_status = true;
  }
  else if(temp_reading<32) { 
    digitalWrite(fan,HIGH); //Fan off
    fan_status = false;
  }
  


  //Light Function
  uint16_t light_reading = GY302.readLightLevel();
  bool led_status = false;
  if(light_reading<100){  //Led banding would be from 0 to 100
    digitalWrite(led,LOW);  //Fan on
    led_status = true; 
  }
  else{
    digitalWrite(led,HIGH);  //Fan off
    led_status = false;
  }



  //Soil Moisture Function
  float soil_moisture_reading = analogRead(A1);
  float soil_moisture_percentage = map(soil_moisture_reading, dry_soil, wet_soil, 0, 100);
  bool pump_status = false;
  if(soil_moisture_percentage<30) {
    digitalWrite(pump,LOW);
    pump_status = true;
  }
  else if(soil_moisture_percentage>40) {
    digitalWrite(pump,HIGH);
    pump_status = false;
  }



  //OLED display Function
  display.clearDisplay();
  display.setTextSize(0.5);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Temperature:");
  display.print(temp_reading);
  display.println("C");
  display.print("Humidity:");
  display.print(humidity_reading);
  display.println("%");
  display.print("Light:");
  display.print(light_reading);
  display.println("lux");
  display.print("SoilMoisture:");
  display.print(soil_moisture_percentage);
  display.println("%");
  
  display.print("---------------------");

  display.print("Fan Status:");
  display.println(fan_status);
  display.print("Pump Status:");
  display.println(pump_status);
  display.print("Led Status:");
  display.println(led_status);
  display.display();


  //Data to cloud function
  Serial.print("Humidity = ");
  Serial.print(humidity_reading);
  Serial.print(" %  ");
  Serial.print("Temperature = ");
  Serial.print(temp_reading); 
  Serial.print(" C  ");
  Serial.print("Light = ");
  Serial.print(light_reading);
  Serial.print(" Lx  ");
  Serial.print("Soil moisture = ");
  Serial.print(soil_moisture_percentage);
  Serial.println(" %  ");
  Serial.print("Fan_Status: ");
  Serial.print(fan_status);
  Serial.print("Pump_Status: ");
  Serial.print(pump_status);
  Serial.print("Led_Status: ");
  Serial.print(led_status);
  

  
  
}
