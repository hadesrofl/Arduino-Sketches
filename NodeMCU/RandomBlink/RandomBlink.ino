/**
 * Copyright 2016   Rene Kremer
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

 

/**
 * Pin number for the green led
 */
int green = 2;
/**
 * Pin number for the yellow led
 */
int yellow = 3;
/**
 * Pin number for the red led
 */
int red = 4;
/**
 * Length of the array of leds
 */
int arrayLength = 3;
/**
 * Array of leds
 */
int leds[] = {green, yellow, red};
/**
 * Number of runs for the random blink
 */
int randomRuns = 50;
/**
 * Delay time
 */
int delayTime = 500;

/**
 * Sets up the led pins
 */
void setup() {
  pinMode(green, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(red, OUTPUT);
}
/**
 * Triple blinks and then random blinks for a given number of times
 */
void loop() {
  //forward();
  //backward();
  for (int i = 0; i < 3; i++) {
    tripleBlink();
    delay(delayTime);
  }
  for (int i = 0; i < randomRuns; i++) {
    randomBlink();
    delay(delayTime/2);
  }
}
/**
 * Blinks with all leds
 */
void tripleBlink() {
  digitalWrite(red, HIGH);
  digitalWrite(yellow, HIGH);
  digitalWrite(green, HIGH);
  delay(delayTime);
  digitalWrite(red, LOW);
  digitalWrite(yellow, LOW);
  digitalWrite(green, LOW);
}

/**
 * Blinks randomly with one led
 */
void randomBlink() {
  int index = random(0, arrayLength);
  digitalWrite(leds[index], HIGH);
  delay(delayTime/2);
  digitalWrite(leds[index], LOW);
}

/**
 * Blinks from red to green
 */
void forward() {
  digitalWrite(red, HIGH);
  delay(delayTime);
  digitalWrite(yellow, HIGH);
  delay(delayTime);
  digitalWrite(red, LOW);
  digitalWrite(green, HIGH);
  delay(delayTime);
  digitalWrite(yellow, LOW);
  delay(delayTime);
  digitalWrite(green, LOW);
}
/**
 * Blinks from green to red
 */
void backward() {
  digitalWrite(green, HIGH);
  delay(delayTime);
  digitalWrite(yellow, HIGH);
  delay(delayTime);
  digitalWrite(green, LOW);
  digitalWrite(red, HIGH);
  delay(delayTime);
  digitalWrite(yellow, LOW);
  delay(delayTime);
  digitalWrite(red, LOW);
}

