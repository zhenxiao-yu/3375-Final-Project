#include "address_map_arm.h";

// define mappings manually for simulator
#define HEX3_HEX0_BASE 0xFF200020
#define HEX5_HEX4_BASE 0xFF200030
#define SW_BASE 0xFF200040
#define KEY_BASE 0xFF200050

volatile int *Display1_ptr = (int *)HEX3_HEX0_BASE;
volatile int *Display2_ptr = (int *)HEX5_HEX4_BASE;

//* utilty functions*//
int countDigit (int val) {
    int count = 0;
    while (val!= 0) {
        val = val/10;
        count++;
    }
    return count;
}

//* display message functions*//
// Display 'Error' Message on 7-segment display
void DisplayError() {
    int BYTE = 8;
    int Letter_E = 0x79; //1111001
    int letter_r = 0x50; //1010000
    int letter_o = 0x5C; //1011100
    // Displaying r + o
    *Display1_ptr = letter_r << (3 * BYTE) |
                    letter_r << (2 * BYTE) |
                    letter_o << (1 * BYTE) |
                    letter_r << (0 * BYTE);
     // Displaying E
    *Display2_ptr = Letter_E;
}

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
    *Display1_ptr = oprTable[0]     << (3 * BYTE) |
                    letter_p        << (2 * BYTE) |
                    letter_r        << (1 * BYTE) |
                    oprTable[oprID] << (0 * BYTE);
    // turn off
    *Display2_ptr = 0x00;
}

// Displays number on 7 seg display
void DisplayValue(int value)
{   // max = 10, displays number from 0 - 9
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
    int bit1, bit2, bit3, bit4, bit5, bit6;
    // if value can be represented by 6 digits
     int bitArray[6];
     int i = 5;
     while (i >= 0) {
         bitArray[i] = value%10;
         value/=10;
         i--;
     }
     if (bitArray[0] != 0) {
         bit6 =  lookUpTable[bitArray[0]];
     } else {
         bit6 = 0x00;
     }
     if (bitArray[0] != 0 && bitArray[1] != 0 ) {
         bit5 =  lookUpTable[bitArray[1]];
     } else {
         bit6 = 0x00;
         bit5 = 0x00;
     }
     if (bitArray[0] != 0 && bitArray[1] != 0 && bitArray[2] != 0 ) {
         bit4 =  lookUpTable[bitArray[2]]; 
     } else {
         bit6 = 0x00;
         bit5 = 0x00;
         bit4 = 0x00;
     }
     if (bitArray[0] != 0 && bitArray[1] != 0 && bitArray[2] != 0 && bitArray[3] != 0) {
         bit3 =  lookUpTable[bitArray[3]];
     } else {
         bit6 = 0x00;
         bit5 = 0x00;
         bit4 = 0x00;  
         bit3 = 0x00; 
     } 
     if (bitArray[0] != 0 && bitArray[1] != 0 && bitArray[2] != 0 && bitArray[3] != 0 &&  bitArray[4] != 0) {
         bit2 =  lookUpTable[bitArray[4]];
     } else {
         bit6 = 0x00;
         bit5 = 0x00;
         bit4 = 0x00;  
         bit3 = 0x00; 
         bit2 = 0x00; 
     }
     bit1 =  lookUpTable[bitArray[5]];

    
    // Displaying bit 1 - 4 of value 
    *Display1_ptr = bit4 << (3 * BYTE) |
                    bit3 << (2 * BYTE) |
                    bit2 << (1 * BYTE) |
                    bit1 << (0 * BYTE);


     // Displaying bit 5 - 6 of value 
    *Display2_ptr = bit6 << (1 * BYTE) |
                    bit5 << (0 * BYTE);
}

// Reads switches
int ReadSwitches(void)
{
    volatile int *SW_ptr = (int *)SW_BASE;
    return (*(SW_ptr));
}

// Reads buttons
int ReadButton(int btn)
{
    volatile int *BTN_ptr = (int *)KEY_BASE;
    // Returns 1 if the given button is pressed, 0 otherwise due to & operator
    return ((*BTN_ptr >> (btn)) & 1);
}

// get input from switch 0 - 5
int GetInput(void)
{
    // pointer to switch address
    volatile int *SW_ptr = (int *)SW_BASE;
    int inputValue = *(SW_ptr);
    // reads slide switch bank and return current value (encoded in binary)
    // return lowest 6 bits in the switch bank (2 ^ 6)
    return (inputValue % (2 * 2 * 2 * 2 * 2 * 2));
}


int main(void)
{


    // Main loop
    while (1)
    {   
        if (countDigit(GetInput()) <= 6) {
            DisplayValue(GetInput());
        } else {
        DisplayError();
        }
        // plus
        if (ReadButton(0) && !ReadButton(1) && !ReadButton(3) && !ReadButton(3))
        {
           DisplayOperator(1);
        }
        // minus
        if (!ReadButton(0) && ReadButton(1) && !ReadButton(2) && !ReadButton(3))
        {
            DisplayOperator(2);
        }
        // multiply
        if (!ReadButton(0) && !ReadButton(1) && ReadButton(2) && !ReadButton(3))
        {
            DisplayOperator(3);
        }
        //divide
        if (!ReadButton(0) && !ReadButton(1) && !ReadButton(2) && ReadButton(3))
        {
            DisplayOperator(4);
        }


        // on / off 
        if (ReadSwitches() == 10)
        {
            
        }

        // clear
        else if (ReadSwitches() == 9){
            
        }

        // show answer
        else if (ReadSwitches() == 8){
            
        }

        // switch mode
        else if (ReadSwitches() == 7){
            
        }
    }
}