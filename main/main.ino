/* 
    Copyright (C) 2015  Efstathios Kladis

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
       GNU GENERAL PUBLIC LICENSE
          Version 2, June 1991
            
            
   Description     
      Auto mode for windows and eco lights. 
   Make your windows move according to sun's position. 
   The window will follow the opposite direction of the sun to control the light that goes into the house. 
   Also, the lights will fade out during day to save energy.
            

 Original can be found at https://github.com/st8ing/light_dependent_servo_window
 
 */
 
#include <Servo.h>
const int buttonPin = 2;     // the number of the pushbutton pin
boolean automode; 
const int ledPin =  5;        // main led with PWM
int buttonState = 0;
const int button2Pin = 3;     // the number of the pushbutton 2 pin
int button2State = 0;
int lightRight = A1;          //
int lightLeft= A2;            // photoresistors
Servo window;                // Window with servo
int sunpos;
int firsttime=1;
int stateled=0;             
int pos=0;                //default servo position 
int dark=80;             //set the threshold to be considered as dark

void setup() {
 Serial.begin(9600);  
  pinMode(ledPin, OUTPUT);
   pinMode(buttonPin, INPUT_PULLUP);
    pinMode(button2Pin, INPUT_PULLUP);
 window.attach(8);
window.write(pos);
}

void loop() {
  
  
   button2State = digitalRead(button2Pin);            //Turn ON/OFF the light
   if (button2State == LOW) {
          if (stateled==0){          
      analogWrite(ledPin, 255);                       //echo mode is disabled
       stateled=1;
       firsttime=0;
       }
        else if (stateled==1){
       analogWrite(ledPin, 0);
        stateled=0;
        
      };
    delay(500);
}
    
   buttonState = digitalRead(buttonPin);
   if (buttonState == LOW) {                             
   automode=!automode;}
   
   if (automode) {                                          //Turn ON automode window control with light eco mode
if (pos!=0){
   if (stateled==1){
     if (firsttime==0){                                             //If it's not dark anymore, turn on eco mode
 for (int fadeValue = 255 ; fadeValue >= 80; fadeValue -= 2) {
    analogWrite(ledPin, fadeValue);
    delay(5);
    firsttime=1;
  }}
}
 }
     delay(100);
    int left=analogRead(lightLeft)/4;        //Reading values
  int right=analogRead(lightRight)/4;
    Serial.print("Left= ");
  Serial.print(left);
 Serial.print("     Right= ");
   Serial.println(right);
 sunpos=left-right;
 Serial.println(sunpos);
   if (sunpos > 20){
     pos=130;}
     else if (sunpos < -25){
       pos=50;}
       else if (right < dark && left < dark){                              // if it is dark outside  
       if (stateled==1){
         if (firsttime==1){
 for (int fadeValue = 80 ; fadeValue <= 255; fadeValue += 2) {         //full brightness, turn off eco mode
    analogWrite(ledPin, fadeValue);
    delay(5);
    firsttime=0;
  }}  
}   
pos=0;}                                                                //Close the window at night
   else {pos=90;}                                  //OPEN the window
 Serial.println(pos);
 window.write(pos);
 
    
  }
  else {                                              //Turn OFF automode window control with light eco mode
     pos=0;                 //Close the window 
     window.write(pos);
 if (stateled==1){
     if (firsttime==1){                                             //If it's not dark anymore, turn on eco mode
 for (int fadeValue = 80 ; fadeValue <= 255; fadeValue += 2) { 
    analogWrite(ledPin, fadeValue);
    delay(5);
    firsttime=0;
  }}
}
 
  }
 

}
