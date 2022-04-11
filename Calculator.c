#include "address_map_arm.h" // system address mappings

// define mappings manually for simulator
#define HEX3_HEX0_BASE 0xFF200020
#define HEX5_HEX4_BASE 0xFF200030
#define SW_BASE 0xFF200040
#define KEY_BASE 0xFF200050

//flag variables
int isDecimal = 0; 

// pointer to displays
volatile int * DISPLAY_1_ptr = (int * ) HEX3_HEX0_BASE; // for displaying digit of seconds and centiseconds
volatile int * DISPLAY_2_ptr = (int * ) HEX5_HEX4_BASE; // for displaying digit of minutes

// pointer to switches
volatile int * SW_ptr = (int * ) SW_BASE;
// pointer to push buttons
volatile int * BUTTON_ptr = (int * ) KEY_BASE;

void switchMode() {

}

void DisplayHex(int value)
{
}

// Reads switches
int ReadSwitches(void)
{
    volatile int *SW_ptr = (int *)SW_BASE;
    return (*(SW_ptr));
}


int main(void) {
}
