
// PIC16F1705 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection Bits (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON      // MCLR Pin Function Select (MCLR/VPP pin function is digital input)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover Mode (Internal/External Switchover Mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PPS1WAY = ON     // Peripheral Pin Select one-way control (The PPSLOCK bit cannot be cleared once it is set by software)
#pragma config ZCDDIS = ON      // Zero-cross detect disable (Zero-cross detect circuit is disabled at POR)
#pragma config PLLEN = OFF      // Phase Lock Loop enable (4x PLL is enabled when software sets the SPLLEN bit)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#define _XTAL_FREQ 16000000
#define _SLAVE_ADDRESS 8
#define CELL_1 0
#define CELL_2 1
#define CELL_3 2
#define CURRENT 3

#include <xc.h>
#include "adc.h"
#include "mux.h"
#include "balance.h"
#include "pic_libs/i2c.h"

unsigned short cell_1_v = 0, cell_2_v = 0, cell_3_v = 0, current = 0;

void setup_clock(char freq){
    OSCCONbits.IRCF = freq;
}

void on_read_data(char offset, char data[]){
    char data_chars[2] = {0};
    switch(offset){
        case CELL_1:
            short_to_chars(cell_1_v, data_chars);
            data[0] = data_chars[0];
            data[1] = data_chars[1];
            break;
        case CELL_2:
            short_to_chars(cell_2_v, data_chars);
            data[0] = data_chars[0];
            data[1] = data_chars[1];
            break;
        case CELL_3:
            short_to_chars(cell_3_v, data_chars);
            data[0] = data_chars[0];
            data[1] = data_chars[1];
            break;
        case CURRENT:
            short_to_chars(current, data_chars);
            data[0] = data_chars[0];
            data[1] = data_chars[1];
            break;
        default:
            data[0] = 0;
            data[1] = 0;
    }
}

void on_write_data(char offset, char data[]){
    unsigned short data_short = chars_to_short(data[1], data[0]);
    switch(offset){
        case CELL_1:
            cell_1_v = data_short;
            break;
        case CELL_2:
            cell_2_v = data_short;
            break;
        case CELL_3:
            cell_3_v = data_short;
            break;
        case CURRENT:
            current = data_short;
            break;
    }
}

void setup(){
    setup_clock(0b1111);
    setup_adc(5000);
    setup_mux();
    setup_balance();
    setup_i2c(0, _SLAVE_ADDRESS, on_write_data, on_read_data);
}

void __interrupt() int_routine(void){
    if(SSPIF){ // received data through i2c
        process_interrupt_i2c();
    } else if(ADIF){
        volatile unsigned short conversion = read_adc();
        clear_adc_flag();
    }
}

int main(void) {
    setup();
    while(1){}
}