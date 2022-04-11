#include "address_map_arm.h";

// define mappings manually for simulator
#define MPCORE_PRIV_TIMER 0xFFFEC600
#define HEX3_HEX0_BASE 0xFF200020
#define HEX5_HEX4_BASE 0xFF200030
#define SW_BASE 0xFF200040
#define KEY_BASE 0xFF200050

volatile int *Display1_ptr = (int *)HEX3_HEX0_BASE;
volatile int *Display2_ptr = (int *)HEX5_HEX4_BASE;

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
    int bit1 = lookUpTable[1];
    int bit2 = lookUpTable[1];
    int bit3 = lookUpTable[1]; 
    int bit4 = lookUpTable[1]; 
    int bit5 = lookUpTable[1];
    int bit6 = lookUpTable[1];

    // if value can be represented by 6 digits
    if (countDigit(value) <= 6)

    // Displaying bit 1 - 4 of value 
    *Display1_ptr = bit4 << (3 * BYTE) |
                    bit3 << (2 * BYTE) |
                    bit2 << (1 * BYTE) |
                    bit1 << (0 * BYTE);


     // Displaying bit 5 - 6 of value 
    *Display2_ptr = bit6 << (1 * BYTE) |
                    bit5 << (0 * BYTE);
}

//* utilty functions*//
int countDigit (int val) {
    int count = 0;
    while (val!= 0) {
        val = val/10;
        count++;
    }
    return count;
}

// Display 'Error' Message on 7-segment display
void DisplayError() {
    int BYTE = 8;
    int Letter_E = 0x79;
    int letter_r = 0x50;
    int letter_o = 0x5C;
    // Displaying r + o
    *Display1_ptr = letter_r << (3 * BYTE) |
                    letter_r << (2 * BYTE) |
                    letter_o << (1 * BYTE) |
                    letter_r << (0 * BYTE);
     // Displaying E
    *Display2_ptr = Letter_E;
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

int main(void)
{
    // Sets initial display to 0
    DisplayValue(0);

    // Main loop
    while (1)
    {   
        // plus
        if (ReadButton(0))
        {
            DisplayError();
        }
        // minus
        if (ReadButton(1))
        {

        }
        // multiply
        if (ReadButton(2))
        {

        }
        //divide
        if (ReadButton(3))
        {

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