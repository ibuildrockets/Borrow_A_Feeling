/*

Can I Borrow a Feeling? Or, how I can access a random emotion at the press of a button.

I work in a therapeutic environment and part of our daily routine is to check in with 
everyone to find out how they're feeling. 
Good, OK, Fine - not acceptable. In fact we have a published list of approved feelings. 
They fall into two groupings - positive & happy or negative & sad. 

Since these 'check-ins' take place in the mornings - often before an adequate volume of 
caffeine has been consumed - coming up with an approved feeling is a challenge. 

Using my shiny new HAD Trinket, I'm taking inspiration from Kirk Van Houten and his 
immortal album "Can I Borrow a Feeling?" - My project is to populate an array with near 
on 300 words and, with a press of a button (well, two buttons - one green button for positive 
feelings, the other red for negative feelings) a randomly chosen emotion will be displayed 
on an LCD screen for all to see. 

No longer will I be left struggling with how I feel - the HAD Trinket will tell me!

This code is being released as Attribution-ShareAlike 2.5 Generic (CC BY-SA 2.5)

http://creativecommons.org/licenses/by-sa/2.5/

Comments and Feedback welcome - http://www.ibuildrockets.com/

*/

#include <Wire.h>
#include <avr/pgmspace.h>
#include "wordList.h" // List of words
#include "stringTable.h" // String Table
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

// Using Tronix i2c LCD backpack
// See here for the tutorial on how these word
// http://tronixlabs.com/news/tutorial-serial-i2c-backpack-for-hd44780compatible-lcd-modules-with-arduino/

LiquidCrystal_I2C	lcd(0x27,2,1,0,4,5,6,7); // 0x27 is the I2C bus address for an unmodified backpack
long randNumber;
unsigned long randomSeedNumber;
char buffer[30];    // make sure this is large enough for the largest string it must hold

const int greenButton = 8;  // The green or good feeling button goes between Pin 8 and GND
const int redButton = 9;  // The red or bad feeling button goes between Pin 9 and GND
int greenButtonState = 0;
int redButtonState = 0;
int i = 0;

void setup()
{
  pinMode(greenButton, INPUT_PULLUP);  // Using internal resistors for the buttons - makes wiring them up simpler.
  pinMode(redButton, INPUT_PULLUP);
  
  Wire.begin();


  lcd.begin (16,2); // for 16 x 2 LCD module
  lcd.setBacklightPin(3,POSITIVE);  
  lcd.setBacklight(HIGH); 
  lcd.home (); // set cursor to 0,0

}

void loop()
{
  
  greenButtonState = digitalRead(greenButton);
  redButtonState = digitalRead(redButton);
  
  // Buttons are pulled high and go low when pressed.
  
  if (greenButtonState == LOW && redButtonState == HIGH)  // If green but not red pressed
  {
    // Positive Response on Green Button being pressed
    lcd.setCursor (0,0);  // set cursor to the first position on the first line.
    lcd.print("  I'm Feeling   ");  // Display the words
    randomSeedNumber = millis();   // Set the random seed to millis - this constantly changes.
    randomSeed(randomSeedNumber);  // Seed the random number generator
    i = random(129,229);  // pick a random number between 129 and 299 - these are our 'good' words
    strcpy_P(buffer, (char*)pgm_read_word(&(string_table[i])));  // load the chosen word into the buffer space
    lcd.setCursor (0,1);        // go to start of 2nd line
    lcd.print( buffer );  //write out buffer (chosen word)
    delay(1500);  // Wait for 1.5 seconds to allow the word to be read before looping back to the wait screen.
     
  }
  
  else if (greenButtonState == HIGH && redButtonState == LOW)  // If red but not green pressed
  {
    // Negative Response on Red Button
    lcd.setCursor (0,0);
    lcd.print("  I'm Feeling   ");
    randomSeedNumber = millis();
    randomSeed(randomSeedNumber);
    i = random(0,128);
    strcpy_P(buffer, (char*)pgm_read_word(&(string_table[i])));
    lcd.setCursor (0,1);     
    lcd.print( buffer ); 
    delay(1500);
  }
  
 
 else if (greenButtonState == LOW && redButtonState == LOW)  // If green and red pressed at the same time
 {
   // Two buttons pressed!! Error!! Error!!
    lcd.setCursor (0,0);
    lcd.print("     ERROR      ");
    lcd.setCursor (0,1);
    lcd.print("JUST ONE BUTTON!");
 }
 
 else  // no buttons being pressed
 {
   // Display the ask screen and wait for an imput.
    lcd.setCursor (0,0);
    lcd.print("How you feeling?");
    lcd.setCursor (0,1);
    lcd.print("<- Good   Bad ->");
   }
 

  
   
}

