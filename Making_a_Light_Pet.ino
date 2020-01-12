/* Adapted from Arduino Projects for Dummies by Brock Craft 
 *
 * Chapter 5: Making a Light Pet
 * Changes the intensity of output of three LEDs
 * with different combinations to simulate "moods" 
 *
*/

// Set which pins will use each color
#define redLED  9
#define greenLED  10
#define blueLED  11

// Assign variables for the LEDs, current value and the new value to change to

int redValue = 0;    // The current value of brightness
int newRedValue = 0; // The new value of brightness

int blueValue = 0;
int newBlueValue = 0;

int greenValue = 0;
int newGreenValue = 0;

// Assign "utility" variables for the random number, and the fading speed
int randomValue = 0;
int fadeSpeed = 50;

// Assign variables to choose the mood
int mood = 0;
#define moodTime  10000 // the time in millisections 10000 = 10 seconds

// Setup the three LED pins for OUTPUT
void setup(){
  pinMode(redLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
}

// the main loop of the program
void loop() {
  // first, determine and set the mood of the pet
  mood = random(20); // randomly pick a number from 0-19 to select what mood the pet should be in
  if (mood == 0){   // if the number picked was 0, run the purr function
    purr(); 
  }
  if (mood == 1){  // if the number picked was 1, run the happy function
    happy();
  }
  if (mood == 2){  // if the number picked was 2, run the sad function
    sad(); 
  }
  if (mood > 2){   // if the number picked was anything higher than 2, run the color blending function
    blendColors(); 
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
/* In the following functions, the use of the new(Red/Blue/Green)value is to define the new intensity of the light of a given LED.
   The if statments check if the new intensity is greater than or less than the current value as to use the correct for loop to
   ramp up or ramp down the intensity of the light to reach its new value.*/
/////////////////////////////////////////////////////////////////////////////////////////

void blendColors(){

  // Pick a random value for the red LED
  newRedValue= random(255); 
  if (redValue < newRedValue){
    for (int x=redValue; x<newRedValue; x++) {
      analogWrite(redLED, x);
      delay(fadeSpeed);
    }
  } 
  else {
    for (int x=redValue; x>newRedValue; x--) {
      analogWrite(redLED, x);
      delay(20);      
      delay(fadeSpeed);    
    } 
  }
  redValue=newRedValue;

  // Pick a random value for the green LED
  newGreenValue= random(255);
  if (greenValue < newGreenValue){
    for (int x=greenValue; x<newGreenValue; x++) {
      analogWrite(greenLED, x);
      delay(fadeSpeed);
    }
  } 
  else {
    for (int x=greenValue; x>newGreenValue; x--) {
      analogWrite(greenLED, x);
      delay(fadeSpeed);
    } 
  }
  greenValue=newGreenValue;

  // Pick a random value for the blue LED
  newBlueValue= random(255);
  if (blueValue < newBlueValue){
    for (int x=blueValue; x<newBlueValue; x++) {
      analogWrite(blueLED, x);
      delay(fadeSpeed);
    }
  } 
  else {
    for (int x=blueValue; x>newBlueValue; x--) {
      analogWrite(blueLED, x);
      delay(fadeSpeed);
    } 
  }
  blueValue=newBlueValue;
}


  // the purr function makes all three LEDs pulsate 10 times
void purr(){
  for (int count=0;count<10;count++){
    for(int x=0;x<255;x++){
      analogWrite(redLED, x);
      analogWrite(greenLED, x);     
      analogWrite(blueLED, x);
      delay(10);
    }
    for(int x=255;x>0;x--){
      analogWrite(redLED, x);
      analogWrite(greenLED, x);     
      analogWrite(blueLED, x);
      delay(10);
    }
  }
}

// The happy function turns on only the green LED for the mood time
void happy(){
  for(int x=greenValue;x<255;x++){
    analogWrite(greenLED, x);
    delay(fadeSpeed);
  }
  for(int x=redValue;x>0;x--){
    analogWrite(redLED, x);
    delay(fadeSpeed);
  }
  for(int x=blueValue;x>0;x--){
    analogWrite(blueLED, x);
    delay(fadeSpeed);
  }
  delay(moodTime); // Sets how long the pet will wait in this mood
  for(int x=255;x>0;x--){
    analogWrite(greenLED, x);
    delay(fadeSpeed);
  }
}

// The sad function turns on only the red LED for the mood time
void sad(){
  for(int x=redValue;x<255;x++){
    analogWrite(redLED, x);
    delay(fadeSpeed);
  }
  for(int x=greenValue;x>0;x--){
    analogWrite(greenLED, x);
    delay(fadeSpeed);
  }
  for(int x=blueValue;x>0;x--){
    analogWrite(blueLED, x);
    delay(fadeSpeed);
  }
  delay(moodTime); // Sets how long the pet will wait in this mood
  for(int x=255;x>0;x--){
    analogWrite(redLED, x);
    delay(fadeSpeed);
  }
} 

