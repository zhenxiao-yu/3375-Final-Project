#include "address_map_arm.h";

// define mappings manually for simulator
#define HEX3_HEX0_BASE 0xFF200020
#define HEX5_HEX4_BASE 0xFF200030
#define SW_BASE 0xFF200040
#define KEY_BASE 0xFF200050

volatile int * Display1_ptr = (int * ) HEX3_HEX0_BASE;
volatile int * Display2_ptr = (int * ) HEX5_HEX4_BASE;

//* utilty functions*//
int countDigit(int val) {
  int count = 0;
  while (val != 0) {
    val = val / 10;
    count++;
  }
  return count;
}

//convert from interger value to binary value
unsigned intToBin(unsigned k) {
  if (k == 0) return 0;
  if (k == 1) return 1; /* optional */
  return (k % 2) + 10 * intToBin(k / 2);
}

//* display message functions*//
// Display 'Error' Message on 7-segment display
void DisplayError() {
  int BYTE = 8;
  int Letter_E = 0x79; //1111001
  int letter_r = 0x50; //1010000
  int letter_o = 0x5C; //1011100
  // Displaying r + o
  * Display1_ptr = letter_r << (3 * BYTE) |
    letter_r << (2 * BYTE) |
    letter_o << (1 * BYTE) |
    letter_r << (0 * BYTE);
  // Displaying E
  * Display2_ptr = Letter_E;
}

//Display the Operator ID on 7-segment Display
void DisplayOperator(int oprID) {
  char oprTable[5];
  oprTable[0] = 0x3F; // used as capital letter 'O'
  oprTable[1] = 0x06; // + 
  oprTable[2] = 0x5B; // -
  oprTable[3] = 0x4F; // x
  oprTable[4] = 0x66; // /
  int BYTE = 8;
  int letter_p = 0x73; //1110011
  int letter_r = 0x50; //1010000
  // Displaying O + p + r + operator ID
  * Display1_ptr = oprTable[0] << (3 * BYTE) |
    letter_p << (2 * BYTE) |
    letter_r << (1 * BYTE) |
    oprTable[oprID] << (0 * BYTE);
  // turn off
  * Display2_ptr = 0x00;
}

// Displays numbers on 7 seg display
void DisplayValue(int value) { // max = 10, displays number from 0 - 9
  char lookUpTable[10];
  lookUpTable[0] = 0x3F;
  lookUpTable[1] = 0x06;
  lookUpTable[2] = 0x5B;
  lookUpTable[3] = 0x4F;
  lookUpTable[4] = 0x66;
  lookUpTable[5] = 0x6D;
  lookUpTable[6] = 0x7D;
  lookUpTable[7] = 0x07;
  lookUpTable[8] = 0x7F;
  lookUpTable[9] = 0x6F;
  int BYTE = 8;
  // if value can be represented by 6 digits
  int bitArray[6];
  int i = 5;
  while (i >= 0) {
    bitArray[i] = value % 10;
    value /= 10;
    i--;
  }
  //assign bit b=values with digits
  int bit6 = lookUpTable[bitArray[0]];
  int bit5 = lookUpTable[bitArray[1]];
  int bit4 = lookUpTable[bitArray[2]];
  int bit3 = lookUpTable[bitArray[3]];
  int bit2 = lookUpTable[bitArray[4]];
  int bit1 = lookUpTable[bitArray[5]];

  // Displaying bit 1 - 4 of value 
  * Display1_ptr = bit4 << (3 * BYTE) |
    bit3 << (2 * BYTE) |
    bit2 << (1 * BYTE) |
    bit1 << (0 * BYTE);

  // Displaying bit 5 - 6 of value 
  * Display2_ptr = bit6 << (1 * BYTE) |
    bit5 << (0 * BYTE);
}

// Reads buttons
int ReadButton(int btn) {
  volatile int * BTN_ptr = (int * ) KEY_BASE;
  // Returns 1 if the given button is pressed, 0 otherwise due to & operator
  return (( * BTN_ptr >> (btn)) & 1);
}

// Reads switches
int ReadSwitch(int sw) {
  // pointer to switch address
  volatile int * SW_ptr = (int * ) SW_BASE;
  // Returns 1 if the given button is pressed, 0 otherwise due to & operator
  return (( * SW_ptr >> (sw)) & 1);
}

// get input from switch 0 - 5
int GetInput(void) {
  // pointer to switch address
  volatile int * SW_ptr = (int * ) SW_BASE;
  volatile int inputValue = * (SW_ptr);
  // reads slide switch bank and return current value (encoded in binary)
  // return lowest 6 bits in the switch bank (2 ^ 6)
  return (inputValue % (2 * 2 * 2 * 2 * 2 * 2));
}



int showingOperator() {
  if (ReadButton(0) || ReadButton(1) || ReadButton(2) || ReadButton(3)) {
    return 1;
  } else {
    return 0;
  }
}

int answer;
int num1;
int num2;
int operator;

int main(void) {

  DisplayValue(0); // Initialize display
  // Main loop
  while (1) {
    //turn display off when switch 9 is not flipped
    if (!ReadSwitch(9)) {
      * Display2_ptr = 0x00;
      * Display1_ptr = 0x00;
    } else if (ReadSwitch(9)) {
      // plus
      if (ReadButton(0) && !ReadButton(1) && !ReadButton(2) && !ReadButton(3)) {
        DisplayOperator(1);
        operator = 1;
        num1 = GetInput();

      }
      // minus
      if (!ReadButton(0) && ReadButton(1) && !ReadButton(2) && !ReadButton(3)) {
        DisplayOperator(2);
        operator = 2;
        num1 = GetInput();
      }
      // multiply
      if (!ReadButton(0) && !ReadButton(1) && ReadButton(2) && !ReadButton(3)) {
        DisplayOperator(3);
        operator = 3;
        num1 = GetInput();
      }
      //divide
      if (!ReadButton(0) && !ReadButton(1) && !ReadButton(2) && ReadButton(3)) {
        DisplayOperator(4);
        operator = 4;
        num1 = GetInput();
      }

      // switch mode
      if (ReadSwitch(6) && showingOperator() == 0 && !ReadSwitch(7)) {
        // display decimal value
        if (countDigit(GetInput()) <= 6) {
          DisplayValue(GetInput());
        } else {
          DisplayError();
        }
      } else if (!ReadSwitch(6) && showingOperator() == 0 && !ReadSwitch(7)) {
        // display binary value
        if (countDigit(intToBin(GetInput())) <= 6) {
          DisplayValue(intToBin(GetInput()));
        } else {
          DisplayError();
        }
      }
      // show answer
      if (ReadSwitch(7)) {
        num2 = GetInput();
        if (operator == 1) {
          answer = num1 + num2;
          if (!ReadSwitch(6)) { //not in decimal mode
            if (countDigit(intToBin(answer)) <= 6) {
              DisplayValue(intToBin(answer));
            } else {
              DisplayError();
            }
          } else {
            if (countDigit(answer) <= 6) {
              DisplayValue(answer);
            } else {
              DisplayError();
            }
          }
        } else if (operator == 2) {
          answer = num1 - num2;
          if (!ReadSwitch(6)) { //not in decimal mode
            if (countDigit(intToBin(answer)) <= 6) {
              DisplayValue(intToBin(answer));
            } else {
              DisplayError();
            }
          } else {
            if (countDigit(answer) <= 6) {
              DisplayValue(answer);
            } else {
              DisplayError();
            }
          }
        } else if (operator == 3) {
          answer = num1 * num2;
          if (!ReadSwitch(6)) { //not in decimal mode
            if (countDigit(intToBin(answer)) <= 6) {
              DisplayValue(intToBin(answer));
            } else {
              DisplayError();
            }
          } else {
            if (countDigit(answer) <= 6) {
              DisplayValue(answer);
            } else {
              DisplayError();
            }
          }
        } else if (operator == 4) {
          answer = num1 / num2;
          if (!ReadSwitch(6)) { //not in decimal mode
            if (countDigit(intToBin(answer)) <= 6) {
              DisplayValue(intToBin(answer));
            } else {
              DisplayError();
            }
          } else {
            if (countDigit(answer) <= 6) {
              DisplayValue(answer);
            } else {
              DisplayError();
            }
          }
        }
      }
    }
  }
}