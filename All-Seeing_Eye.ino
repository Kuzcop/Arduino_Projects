// Adapted from Arduino Projects for Dummies by Brock Craft 
// Chapter 4: The All-seeing Eye
// Sequentially lights up a series of LEDs

//////////////////////////////////////////////////////////////////////////////
/* The following are instance variables which are used to create the desired 
effect of the LEDS cycling down the from one to the the other*/
//////////////////////////////////////////////////////////////////////////////
// A variable to set a delay time between each LED
int delayTime = 40;

// A variable to store which LED we are currently working on
int currentLED = 4;

// A constant to store the direction of travel
int dir = 1;

// A variable to store the last time an action has occurred; in this program it would be the time since an led has been turned on
int timeChanged  = 0;

// An array to represent the different pins for each LED
// As we use 10 pins, there are 10 elements in the array from digital pin 4 to digital pin 13
byte ledPin[] = {4,5,6,7,8,9,10,11,12,13};

void setup() {
  // A for loop to quickly assign each pin to be OUTPUT
  for (int i = 0; i < 10; i++){
    pinMode(ledPin[i], OUTPUT);
  }

  // Record the time once the setup has completed
  timeChanged = millis();
}

void loop(){
  // Check whether it has been long enough
  // millis() function gives the current time of the program, while timeChanged is the time since the last action.
  // Thus, the if statement will only be true if the delta of the current time to the time of the last action is greater than the 
  // delayTime value we have set
  if ((millis() - timeChanged) > delayTime) {

    //turn off all the LEDs
    for (int i = 0; i < 10; i++){
      digitalWrite(ledPin[i], LOW);
    }

    // Light the current LED
    digitalWrite(ledPin[currentLED], HIGH);

  // Increase the direction value (up or down)
  currentLED = currentLED + dir;

  // If we are at the end of a row, change direction
  if (currentLED == 9){
    dir = -1;
  }
  if (currentLED == 0){
    dir = 1;
  }
  //store the current time as the time we last changed LEDs
  timeChanged = millis();

  // The potentimeter which is wired to the 0 analog pin gives the new value of the delayTime
  delayTime = analogRead(0);

 Serial.print("%d", delayTime);
}
}

