/******************************************************************************
SAMD21G18A Sensor Board Test Code

Thanks to Adafuit for the bootloader and sensor libraries.
Without the hard work of the good people at Adafruit, none of this would be possible.
*******************************************************************************/

#include <Wire.h> 
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h> 
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include "ClosedCube_HDC1080.h"

#define OLED_RESET 6
Adafruit_SSD1306 display(OLED_RESET);

#define Binary 0
#define Hex 1

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 

ClosedCube_HDC1080 hdc1080;

/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

/*******************************************************************************
                    Global variables
*******************************************************************************/ 
const int I2C_address = 0x68;  //RTC I2C write address 
byte    Second;     // Store second value
byte    Minute;     // Store minute value
byte    Hour;       // Store hour value
byte    Day;        // Store day value
byte    Date;       // Store date value
byte    Month;      // Store month value
byte    Year;       // Store year value

int ledPin =  13;

//Button Stuff
const int buttonPin_0 = 0;     //PushButton
const int buttonPin_1 = 1;     //direction Nav In
const int buttonPin_2 = 2;     //direction Nav Down    
const int buttonPin_3 = 3;     //direction Nav Left
const int buttonPin_4 = 5;     //direction Nav Right
const int buttonPin_5 = 11;    //direction Nav Up    

int buttonState_0 = 1; //Buttons Normally High
int buttonState_1 = 1;
int buttonState_2 = 1;
int buttonState_3 = 1;
int buttonState_4 = 1;
int buttonState_5 = 1;

/*******************************************************************************
                      Setup
*******************************************************************************/ 

void setup() 
{ 
 Serial.begin(9600);
 
 Wire.begin();        // join i2c bus (address optional for master) 
 delay(1000);
 
 display.setRotation(2); 
 // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
 display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)

 hdc1080.begin(0x40); 

 pinMode(ledPin, OUTPUT);    // initialize the LED pin as an output:

  pinMode(buttonPin_0, INPUT); 
  pinMode(buttonPin_1, INPUT); 
  pinMode(buttonPin_2, INPUT); 
  pinMode(buttonPin_3, INPUT); 
  pinMode(buttonPin_4, INPUT);
  pinMode(buttonPin_5, INPUT);

  /* Set the range to whatever is appropriate for your project */
  accel.setRange(ADXL345_RANGE_16_G);
  // displaySetRange(ADXL345_RANGE_8_G);
  // displaySetRange(ADXL345_RANGE_4_G);
  // displaySetRange(ADXL345_RANGE_2_G);
  
  // Display some basic information on this sensor 
  //displaySensorDetails();
  
  /* Display additional settings (outside the scope of sensor_t) */
  displayDataRate();
  displayRange();
} 
 
/*******************************************************************************
                      Main Loop
*******************************************************************************/  
void loop() 
{ 
  
 DisplayTime();
 Display_Temp_Humid();
 Display_Buttons();
 Display_Accel();
  
 display.display();
 display.clearDisplay();
}







void DisplayTime()
{
  char tempchar [7];
  byte i = 0;
  Wire.beginTransmission(I2C_address);
  Wire.write(0);
  Wire.endTransmission();
  
  Wire.requestFrom(I2C_address, 7);
  
  while(Wire.available())          // Checkf for data from slave
  { 
    tempchar [i] = Wire.read(); // receive a byte as character 
    i++;
  } 
  Second = tempchar [0];
  Minute = tempchar [1];
  Hour   = tempchar [2];
  Day    = tempchar [3];
  Date   = tempchar [4];
  Month  = tempchar [5];
  Year   = tempchar [6];
  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(38,0); //over,down

  if (Hour <10)
  {
   display.print(" "); 
  }
  display.print(Hour,HEX);
  display.print(":");

  if (Minute <10)
  {
   display.print("0"); 
  }
  display.print(Minute,HEX);
  display.print(":");

  if (Second < 10)
  {
   display.print("0");
  }
  display.print(Second,HEX);
  //display.display();
  //delay(100);
  //display.clearDisplay();
 }

 void Display_Temp_Humid(void)
 {
  //Temperature
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(32,10); //over,down
  display.println("Temp: ");
    
  display.setCursor(65,10); //over,down
  display.print(hdc1080.readTemperature()); 

  //Humidity
  display.setCursor(32,20); //over,down
  display.println("Hum: ");
    
  display.setCursor(65,20); //over,down
  display.print(hdc1080.readHumidity());
 }



 void Display_Buttons(void)
{
 int Button = digitalRead(buttonPin_0);    //read the state of the pushbutton value:
 int Nav_In = digitalRead(buttonPin_1);
 int Nav_Down = digitalRead(buttonPin_2);
 int Nav_Left = digitalRead(buttonPin_3);
 int Nav_Up = digitalRead(buttonPin_4);
 int Nav_Right = digitalRead(buttonPin_5);

 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.setCursor(32,40); //over,down
 display.println("Button: ");

 if (Button == HIGH)
 {
  display.setCursor(75,40); //over,down
  display.println("Ent");
 }

  if (Nav_In == HIGH)
 {
  display.setCursor(75,40); //over,down
  display.println("In");
 }

   if (Nav_Down == HIGH)
 {
  display.setCursor(75,40); //over,down
  display.println("Dwn");
 }

    if (Nav_Left == HIGH)
 {
  display.setCursor(75,40); //over,down
  display.println("Lft");
 }
    if (Nav_Up == HIGH)
 {
  display.setCursor(75,40); //over,down
  display.println("Up");
 }
    if (Nav_Right == HIGH)
 {
  display.setCursor(75,40); //over,down
  display.println("Rgh");
 }
}


void Display_Accel()
{
  /* Get a new sensor event */ 
  sensors_event_t event; 
  accel.getEvent(&event);
  
  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");
  //delay(500); 

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(32,30); //over,down
  display.println("Accel: ");
  display.setCursor(70,30); //over,down
  display.println(event.acceleration.x);
}


void displaySensorDetails(void)
{
  sensor_t sensor;
  accel.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500); 
}



void displayDataRate(void)
{
  //Serial.print  ("Data Rate:    "); 
  
  switch(accel.getDataRate())
  {
    case ADXL345_DATARATE_3200_HZ:
      //Serial.print  ("3200 "); 
      break;
    case ADXL345_DATARATE_1600_HZ:
      Serial.print  ("1600 "); 
      break;
    case ADXL345_DATARATE_800_HZ:
      Serial.print  ("800 "); 
      break;
    case ADXL345_DATARATE_400_HZ:
      Serial.print  ("400 "); 
      break;
    case ADXL345_DATARATE_200_HZ:
      Serial.print  ("200 "); 
      break;
    case ADXL345_DATARATE_100_HZ:
      Serial.print  ("100 "); 
      break;
    case ADXL345_DATARATE_50_HZ:
      Serial.print  ("50 "); 
      break;
    case ADXL345_DATARATE_25_HZ:
      Serial.print  ("25 "); 
      break;
    case ADXL345_DATARATE_12_5_HZ:
      Serial.print  ("12.5 "); 
      break;
    case ADXL345_DATARATE_6_25HZ:
      Serial.print  ("6.25 "); 
      break;
    case ADXL345_DATARATE_3_13_HZ:
      Serial.print  ("3.13 "); 
      break;
    case ADXL345_DATARATE_1_56_HZ:
      Serial.print  ("1.56 "); 
      break;
    case ADXL345_DATARATE_0_78_HZ:
      Serial.print  ("0.78 "); 
      break;
    case ADXL345_DATARATE_0_39_HZ:
      Serial.print  ("0.39 "); 
      break;
    case ADXL345_DATARATE_0_20_HZ:
      Serial.print  ("0.20 "); 
      break;
    case ADXL345_DATARATE_0_10_HZ:
      Serial.print  ("0.10 "); 
      break;
    default:
      Serial.print  ("???? "); 
      break;
  }  
  //Serial.println(" Hz");  
}

void displayRange(void)
{
  Serial.print  ("Range:         +/- "); 
  
  switch(accel.getRange())
  {
    case ADXL345_RANGE_16_G:
      Serial.print  ("16 "); 
      break;
    case ADXL345_RANGE_8_G:
      Serial.print  ("8 "); 
      break;
    case ADXL345_RANGE_4_G:
      Serial.print  ("4 "); 
      break;
    case ADXL345_RANGE_2_G:
      Serial.print  ("2 "); 
      break;
    default:
      Serial.print  ("?? "); 
      break;
  }  
  Serial.println(" g");  
}


  
