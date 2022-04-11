#include "address_map_arm.h";

// define mappings manually for simulator
#define MPCORE_PRIV_TIMER 0xFFFEC600
#define HEX3_HEX0_BASE 0xFF200020
#define HEX5_HEX4_BASE 0xFF200030
#define SW_BASE 0xFF200040
#define KEY_BASE 0xFF200050

volatile int *seconds_display_ptr = (int *)HEX3_HEX0_BASE;
volatile int *minutes_display_ptr = (int *)HEX5_HEX4_BASE;

// Displays number on 7 seg display
void DisplayHex(int value)
{
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

    int Byte = 8;
    int digit0;
    int digit1;
    int digit2;
    int digit3; 
    int digit5;

    // Displaying seconds and milliseconds
    *seconds_display_ptr = lookUpTable[s / 10] << 24 | lookUpTable[s % 10] << 16 | lookUpTable[ms / 10] << 8 | lookUpTable[ms % 10];

    // Displaying minutes
    *minutes_display_ptr = lookUpTable[min / 10] << 8 | lookUpTable[min % 10];
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

    // // Is 1 if the timer is running, 0 if it is stopped
    // int running = 0;

    // // Initial timer values
    // int ms = 0;
    // int s = 0;
    // int min = 0;

    // // Initial lap values
    // int lap_ms = 0;
    // int lap_s = 0;
    // int lap_min = 0;

    // Sets initial display to 00:00:00
    DisplayHex(ms, s, min);

    // Main loop
    while (1)
    {   
        // plus
        if (ReadButton(0))
        {

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