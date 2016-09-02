/*
  LedMatrix.cpp - Library for the Arduino Nano PlayBoard
  Created by Antonio Morales, June, 2016.
  Released into the public domain.
*/

#include <Arduino.h>
#include "Register.h"
#include "LedMatrix.h"

uint8_t LedMatrix::_columns[5] = {2, 4, 5, 16, 17};

LedMatrix::LedMatrix()
{
  // Initialize the pins used for the columns as outputs
  for(int i = 0; i < 5; i++){
    pinMode(_columns[i], OUTPUT);
    digitalWrite(_columns[i], LOW);
  }
  
  // Clear the value used for the shift register
  _register.clear();

  // Initialize the text scroll speed used to display messages
  _scrollSpeed = 10;
}

// Display the symbol passed as argument.
// A checking is needed to validate if the argument is inside the valid range.
// If the argument is outside the range then a white space is displayed.
//*************************************************************
void LedMatrix::print(char symbol)
{
  if ((symbol >= 0x20) & (symbol <=0x7e)) {
    for(int i = 0; i < 5; i++){
      _register.write(ascii[symbol-0x20][i]);
      digitalWrite(_columns[i], HIGH);
      delay(2);
      digitalWrite(_columns[i], LOW);
    }
  } else {
    for(int i = 0; i < 5; i++){
      _register.write(ascii[0][i]);
      digitalWrite(_columns[i], HIGH);
      delay(2);
      digitalWrite(_columns[i], LOW);
    }
  }
}

// Display the pattern stored in the array of bytes
void LedMatrix::print(byte pattern[5])
{
  for(int i = 0; i < 5; i++) {
    _register.write(pattern[i]);
    digitalWrite(_columns[i], HIGH);
    delay(2);
    digitalWrite(_columns[i], LOW);
  }
}

// Display the message stored in the array of chars
void LedMatrix::print(char message[])
{
  byte pattern[5];
  for (int i = 0; i < (strlen(message)*5-4); i++){  
    pattern[0]=ascii[message[i/5]-0x20][i%5];
    pattern[1]=ascii[message[(i+1)/5]-0x20][(i+1)%5];
    pattern[2]=ascii[message[(i+2)/5]-0x20][(i+2)%5];
    pattern[3]=ascii[message[(i+3)/5]-0x20][(i+3)%5];
    pattern[4]=ascii[message[(i+4)/5]-0x20][(i+4)%5];

    // Display the pattern several times
    for(int n = 0; n < _scrollSpeed; n++) {
      print(pattern);
    }
  }
}

// Display the message stored in the String object
void LedMatrix::print(String message)
{
  byte pattern[5];
  for (int i = 0; i < message.length()*5-4; i++){  
    pattern[0]=ascii[message.charAt(i/5)-0x20][i%5];
    pattern[1]=ascii[message.charAt((i+1)/5)-0x20][(i+1)%5];
    pattern[2]=ascii[message.charAt((i+2)/5)-0x20][(i+2)%5];
    pattern[3]=ascii[message.charAt((i+3)/5)-0x20][(i+3)%5];
    pattern[4]=ascii[message.charAt((i+4)/5)-0x20][(i+4)%5];

    // Display the pattern several times
    for(int n = 0; n < _scrollSpeed; n++) {
      print(pattern);
    }
  }
}


void LedMatrix::setScrollSpeed(int speed)
{
  _scrollSpeed = speed;
}

void LedMatrix::clear()
{
  print(' ');
}

void LedMatrix::printInLandscape(int number)
{
  byte pattern[5];
  // Workaround to avoid a: call of overloaded 'print(const byte [5])' is ambiguous
  memcpy(pattern, landscape_numbers[number], sizeof(landscape_numbers[number]));
  print(pattern);
}
