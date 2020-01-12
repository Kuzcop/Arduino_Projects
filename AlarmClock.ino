/* Adapted from Arduino Projects for Dummies by Brock Craft 
 *
 * Chapter 7: Building an Arduino Clock
 * An alarm clock that uses the Adafruit Industries DS1307 RTC Breakout board
 * and a 16x2 Parallel LCD Display
 *
 * Uses the following libraries:Wire,LiquitCrystal,
 * and the Adafruit RTC library
*/

#include <Wire.h>          // I2C Wire Library for communicating with the DS1307 RTC
#include "RTClib.h"        // Date and time functions for the DS1307 RTC connected
#include <LiquidCrystal.h> // Display functions for the LCD Display 

RTC_DS1307 rtc;                         // Create a realtime clock called rtc
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);  // Create an LCD called lcd 

DateTime now;        // Creates a DateTime object called now so that we can read from the RTC module

boolean displayAlarmSet = false;  // Whether we are in the show time mode or show alarm set mode
boolean alarm = false;   // Whether the alarm is set or not
boolean armed = false;   // Whether the alarm is armed or not

int alarmHrs = 12;   // You can set the alarm time in code, here
int alarmMins = 00;

// User input to set alarm time
const int alarmSetPin=6;           // Used to change to alarm set mode
const int incrementAlarmHrsPin=7;  // Used to increment the alarm hours in alarm set mode
const int incrementAlarmMinsPin=8; // Used to increment the alarm minutes in alarm set mode
const int piezoPin = 9;            // Used for the piezoelectric sounder
const int alarmArmedPin=10;        // Use to enable the alarm to go off


void setup () {
  Wire.begin();      // Enables the communication for the LCD
  rtc.begin();       // Enables the RTC
  lcd.begin(16, 2);  // Enables he LCD
  lcd.print("  It's Alive!");    // Print a message, centered, to the LCD to confirm is working
  delay(1000);                    // Wait a moment so we can read it
  lcd.clear();       // Clear the LCD

  // Set several pins for input and output
  pinMode(alarmSetPin, INPUT);   
  pinMode(incrementAlarmHrsPin, INPUT);
  pinMode(incrementAlarmMinsPin, INPUT);
  pinMode(piezoPin, OUTPUT);
  pinMode(alarmArmedPin, INPUT);
  
}

void loop () {
  now = rtc.now(); // Get the current time
 
  int alarmArmed=digitalRead(alarmArmedPin);
  if (alarmArmed==HIGH){
   armed=true;
  } else {
   armed=false;
  }

  // Determine whether to sound the alarm or not
 if(armed){        // If the alarm is armed and...
  if (!alarm){     // If we are not currently sounding the alarm
    checkAlarm();  // Check to see if it is the time it should be triggered
  } 
  else {
    soundAlarm();  // Otherwise, we should be sounding the alarm, so do it.
  }
}
  // Check whether we are in Alarm Set mode
  int setMode = digitalRead(alarmSetPin);  // Read the pin that the switch is on 
  if (setMode==HIGH){      // If the pin is high
    displayAlarmSet=true;  // Set displayAlarmSet true. It's used by updateDisplay to switch between showing alarm or current time
    setAlarm();            // Go read the switches to set the alarm
  }
  else {                   // If we aren't in set mode
    displayAlarmSet=false; // We are not is net mode so make sure the flag is correct
  }

  // Refresh the display
  updateDisplay();
}

// Update the display with either the current time or the time the alarm is set for
void updateDisplay(){
   
   if(displayAlarmSet){   // If we are in alarm set mode, DISPLAY ALARM SET TEXT
    lcd.setCursor(0, 0);  // Set the cursor at the column zero, upper row
    lcd.print("Set alarm time: ");
    lcd.setCursor(4, 1);  // Move the cursor to column four, lower row
    lcd.print(" ");       // Write over digits of the time that was previously displayed
    lcd.setCursor(5, 1);  // Move to the next column so the time will be centered
    if (alarmHrs<10){     // Integers of 0-9 are only one digit. If so...
      lcd.print(0);       // ... add a zero in front of it     
    }  
    lcd.print(alarmHrs);  // Print the current alarm hour
    lcd.setCursor(7, 1);  // Move to the next column
    lcd.print(":");       // And print the colon
    lcd.setCursor(8, 1);  // Move to the next column
    if (alarmMins<10){    // Integers of 0-9 are only one digit. If so...
      lcd.print(0);       // ... add a zero in front of it
    }  
    lcd.print(alarmMins); // Print the current alarm minutes
    lcd.setCursor(10, 1); // Move to the next column
    lcd.print("  ");      // Write spaces over the digits of time previously displayed
  } 
  else {                  // If we are in NOT alarm set mode, DISPLAY CURRENT TIME

    int h = now.hour();   // Get the hours right now and store them in an integer called h
    int m = now.minute(); // Get the minutes right now and store them in an integer called m
    int s = now.second(); // Get the seconds right now and store them in an integer called s

    lcd.setCursor(0, 0);  // Set the cursor at the column zero, upper row...
    if(armed){
      lcd.print("* The time is: ");
    }
    else {
      lcd.print("  The time is: ");  // ...with spaces to clear characters from setting alarm. 
    }
    lcd.setCursor(4, 1);  // Move the cursor to column four, lower row

    if (h<10){            // Add a zero, if necessary, as above
      lcd.print(0); 
    }  
    lcd.print(h);         // Display the current hour  
    lcd.setCursor(6, 1);  // Move to the next column
    lcd.print(":");       // And print the colon
    lcd.setCursor(7, 1);  // Move to the next column
    if (m<10){            // Add a zero, if necessary, as above
      lcd.print(0); 
    }  
    lcd.print(m);         // Display the current minute
    lcd.setCursor(9, 1);  // Move to the next column
    lcd.print(":");       // And print the colon
    lcd.setCursor(10, 1); // Move to the next column
    if (s<10){            // Add a zero, if necessary, as above
      lcd.print(0);
    }  
    lcd.print(s);         // Display the current second
  }

}

// Check whether to sound the alarm
void checkAlarm(){
  if(alarmHrs==now.hour() && alarmMins==now.minute() && now.second()==0){  // If the alarm time is now, and it's zero seconds
    alarm=true;  // set the alarm flag to be true. The next time the main loop executes, the alarm will be activated
  } 
}

// Increment hours or minutes
void setAlarm(){
  int hrs=digitalRead(incrementAlarmHrsPin); 
  int mins=digitalRead(incrementAlarmMinsPin);

  if (hrs==HIGH){    // If the hours switch is pressed
    alarmHrs+=1;     // Increment the hours upward
    delay(200);      // Wait a moment between incrementing the numbers
    if(alarmHrs>23){ // if the hour is over 23, set it back to 0
      alarmHrs=0; 
    }
  }
  if (mins==HIGH){    // If the minutes switch is pressed
    alarmMins+=1;     // Increment the minutes upward
    delay(200);       // Wait a moment between incrementing the numbers
    if(alarmMins>59){ // if the minute is over 59, set it back to 0
      alarmMins=0; 
    }
  }
}

void soundAlarm() {
  float alarmFrequency=1400;  // The value for the alarm tone in Hz
  float period = (1.0 / alarmFrequency) * 1000000;
  long beepDuration=250000;   // the time in microseconds
  long elapsedTime = 0;

  while (elapsedTime < beepDuration) {
    digitalWrite(piezoPin,HIGH);
    delayMicroseconds(period / 2);
    digitalWrite(piezoPin, LOW);
    delayMicroseconds(period / 2);
    elapsedTime += (period);
  }
  digitalWrite(piezoPin, LOW);         
  delayMicroseconds(beepDuration);

  // Listend for either button to be pressed and if so, turn off the alarm
  int hrs=digitalRead(incrementAlarmHrsPin); 
  int mins=digitalRead(incrementAlarmMinsPin);

  if (hrs==HIGH || mins==HIGH){
    alarm=false;
  }
}


