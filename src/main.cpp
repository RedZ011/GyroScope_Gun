#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;
//++++++++++++++++++++++++++++++VAR++++++++++++++++++++++++++++++++++++++++

int16_t ax, ay, az, gx, gy, gz , tempx , tempz,gx_offset,gz_offset;
float cursorX = 0, cursorY = 0;
float sensitivity = 0.01; 
short LeftClick = 33, RightClick = 25, MotorPin = 26;
short LeftClickState , RightClickState;

//++++++++++++++++++++++++++++++SETUP++++++++++++++++++++++++++++++++++++++++
void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  mpu.initialize();
  
  const int calSamples = 2000;
  long sumX = 0, sumZ = 0;

  for (int i = 0; i < calSamples; i++) {
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    sumX += gx;
    sumZ += gz;
    delay(3);
  }

  gx_offset = sumX / (float)calSamples;
  gz_offset = sumZ / (float)calSamples;

  pinMode(MotorPin, OUTPUT);
  digitalWrite(MotorPin, LOW); 

  pinMode(LeftClick , INPUT_PULLUP);
  pinMode(RightClick, INPUT_PULLUP);
  
}

//++++++++++++++++++++++++++++++LOOP++++++++++++++++++++++++++++++++++++++++

void loop() {
 
  
  tempx = gx;
  tempz = gz;
  
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  
  

  float gx_corrected = gx - tempx ;
  float gz_corrected = gz - tempz ;


  cursorX += (gx_corrected * sensitivity) ;

  cursorY += (-1 * gz_corrected * sensitivity)  ;
  
  LeftClickState = digitalRead(LeftClick);
  RightClickState= digitalRead(RightClick);

  if (LeftClickState == 0) { 
    digitalWrite(MotorPin, HIGH);
  } 
  else {
    digitalWrite(MotorPin, LOW);
  }
  Serial.printf("%.1f,%.1f,%d,%d\n", cursorX, cursorY ,LeftClickState , RightClickState);
  delay(10); // ~100Hz loop
}