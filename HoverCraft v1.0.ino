#include <Servo.h>
#include <elapsedMillis.h>

int intervalLEDS = 2000;
const int minIntervalLEDS = 2000;

Servo ESC;
Servo rudder;

int data = 2; 
int clock = 3;
int latch = 4;

const int liftPin = 5;
const int leftButtonPin = 11;
const int rightButtonPin = 12;

int throttlePin = 0;
int rudderPin = 10;

int liftState = 0;
int lift = 0;

int ledState = 0;

int rudderState = 90;

const int ON = HIGH;
const int OFF = LOW;

elapsedMillis timerLEDS;

int larsonState = 0;
int larsonLED = 0;

//For more details http://en.wikipedia.org/wiki/Bitwise_operation
int bits[] = {B00000001, B00000010, B00000100, B00001000,
              B00010000, B00100000, B01000000, B10000000};
int masks[] = {B11111110, B11111101, B11111011, B11110111,
               B11101111, B11011111, B10111111, B01111111};

void setup()
{
  ESC.attach(9);
  rudder.attach(10);
  pinMode(liftPin, INPUT);
  pinMode(throttlePin, INPUT);
  
  pinMode(data, OUTPUT);
  pinMode(clock, OUTPUT);  
  pinMode(latch, OUTPUT); 
  
  pinMode(leftButtonPin, INPUT);
  pinMode(rightButtonPin, INPUT);
  Serial.begin(9600);
  
  timerLEDS = 0;

}
void loop()
{
  
  if(digitalRead(liftPin) == LOW)
  {
   
     if(liftState == 0)
     {
      
        liftState = 1;
        lift = 179;
        
     }
     else if(liftState == 1)
     {
      
        liftState = 0;
        lift = 20;
       
     }
     else
     {
      
        liftState = 0;

     }
    
  }
  
//  if(liftState == 0)
//  {
//   
//     if(lift > 0)
//     {
//      
//        lift--;
//       
//     }
//    
//  }
//  else if(liftState == 1)
//  {
//   
//     if(lift < 80)
//     {
//      
//        lift++;
//       
//     }
    
  //}
  
  if (timerLEDS > intervalLEDS) {
    timerLEDS = 0; //reset the timer
    
    changeLED(larsonLED, OFF);
    
    if(larsonState < 7){
      
       larsonLED++;
       larsonState++;
       
    }
    else if(larsonState < 13)
    {
      
      larsonLED--; 
      larsonState++;
       
      
    }
    else
    {
     
     larsonState = 0;
     larsonLED = 0;
    }
    
    changeLED(larsonLED, ON);
    
  }
   
  if(digitalRead(leftButtonPin) == LOW)
  {
   
    if(rudderState > 20)
    {
       rudderState--;
    }
    
  }
  if(digitalRead(rightButtonPin) == LOW)
  {
   
    if(rudderState < 140)
    {
       rudderState++;
    }
    
  }
  
  int throttle = analogRead(throttlePin);
  throttle = map(throttle, 0, 1023, 20, 120);
  ESC.write(throttle);
  Serial.println(throttle);
  
  rudder.write(rudderState);
  
  intervalLEDS = minIntervalLEDS / throttle;
  
}

void updateLEDs(int value){
  digitalWrite(latch, LOW);     //Pulls the chips latch low
  shiftOut(data, clock, MSBFIRST, value); //Shifts out the 8 bits to the shift register
  digitalWrite(latch, HIGH);   //Pulls the latch high displaying the data
}

void changeLED(int led, int state){
   ledState = ledState & masks[led];  //clears ledState of the bit we are addressing

   //if the bit is on we will add it to ledState
   if(state == ON){ledState = ledState | bits[led];} 
   updateLEDs(ledState);              //send the new LED state to the shift register
 }
 
