
// PIC16F1459 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection Bits (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover Mode (Internal/External Switchover Mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config CPUDIV = CLKDIV6 // CPU System Clock Selection Bit (CPU system clock divided by 6)
#pragma config USBLSCLK = 48MHz // USB Low SPeed Clock Selection bit (System clock expects 48 MHz, FS/LS USB CLKENs divide-by is set to 8.)
#pragma config PLLMULT = 3x     // PLL Multipler Selection Bit (3x Output Frequency Selected)
#pragma config PLLEN = DISABLED // PLL Enable Bit (3x or 4x PLL Disabled)
#pragma config STVREN = OFF     // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will not cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#define _XTAL_FREQ 16000000
#define _SLAVE_ADDRESS 8
#define VOFFSET 0

#include <xc.h>
#include "mux.h"
#include "adc.h"
#include "pic_libs/i2c.h"

volatile unsigned short voltage = 0;

void setup_clock(){
    OSCCONbits.IRCF = 0b1111;
}

void setup_icsp_pins(){
    
}

void on_read_data(char offset, char data[]){
    char data_chars[2] = {0};
    // offsets 0 - 3 are battery status
    if(offset == VOFFSET){
        short_to_chars(voltage, data_chars);
        data[0] = data_chars[0];
        data[1] = data_chars[1];
    }
    else {
        data[0] = 0xff;
        data[1] = 0xff;
    }
}

void on_write_data(char offset, char data[]){
    switch(offset){
        case VOFFSET:
            voltage = chars_to_short(data[1], data[0]);
            break;
    }
}

void __interrupt() int_routine(void){
    if(SSPIF){ // received data through i2c
        process_interrupt_i2c();
    } else if(ADIF){
        voltage = get_adc_voltage();
    }
}

void main(void) {
    setup_clock();
    setup_mux();
    setup_adc(5000);
    setup_i2c(0, _SLAVE_ADDRESS, on_write_data, on_read_data);
    while(1){
    }
    return;
}