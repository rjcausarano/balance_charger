
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
// Offsets 0 - 3 reserved for reading battery status
#define CELL_BALANCE 4
#define CELL_MEASURE 5
#define CHARGE 6

#include <xc.h>
#include "mux.h"
#include "adc.h"
#include "pic_libs/i2c.h"

// Battery status [cell_1_mv, cell_2_mv, cell_3_mv, current_ma]
unsigned short battery_status[4] = {0};
char selected_cell = 0;

void set_charge(char state){
    RB7 = state;
}

char get_charge(){
    return RB7;
}

void setup_charge(){
    // Make RB7 digital
    char change_mask = 0b10000000;
    ANSELB = (ANSELB & ~change_mask) | 0b00000000;
    TRISB7 = 0;
    WPUB7 = 1;
    set_charge(0);
}

void setup_clock(){
    OSCCONbits.IRCF = 0b1111;
}

void on_read_data(char offset, char data[]){
    char data_chars[2] = {0};
    // offsets 0 - 3 are battery status
    if(offset >= 0 && offset <= 3){
        short_to_chars(battery_status[offset], data_chars);
        data[0] = data_chars[0];
        data[1] = data_chars[1];
    }
    else {
        switch(offset){
            case CHARGE:
                data[0] = get_charge();
                data[1] = 0x00;
                break;
            default:
                data[0] = 0xff;
                data[1] = 0xff;
        }
    }
}

void on_write_data(char offset, char data[]){
    switch(offset){
        case CELL_BALANCE:
            break;
        case CELL_MEASURE:
            // If ADC is busy or if the selected channel is out of range, return
            if(is_converting() || data[0] > 2){
                return;
            }
            selected_cell = data[0];
            channel_select(selected_cell);
            inhibit_output(0);
            do_conversion();
            break;
        case CHARGE:
            // If state is > 1, return
            if(data[0] > 1){
                return;
            }
            set_charge(data[0]);
            break;
    }
}

void __interrupt() int_routine(void){
    if(SSPIF){ // received data through i2c
        process_interrupt_i2c();
    } else if(ADIF){
        battery_status[selected_cell] = get_adc_voltage();
        inhibit_output(1);
    }
}

void setup(){
    setup_clock();
    setup_mux();
    setup_adc(4880);
    setup_i2c(0, _SLAVE_ADDRESS, on_write_data, on_read_data);
    setup_charge();
}

void main(void) {
    setup();
    inhibit_output(1);
    while(1){}
    return;
}