
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

#include <xc.h>

void setup_clock(){
    IRCF3 = 1;
    IRCF2 = 1;
    IRCF1 = 1;
    IRCF0 = 1;
}

void do_conversion(){
    // Wait acquisition time
    __delay_us(5);
    GO_nDONE = 1;
}

void clear_adc_flag(){
    ADIF = 0;
}

void setup_adc(){
    // Configure Port
    TRISA0 = 1;
    ANSA0 = 1;
    // Configure Clock
    ADCON1bits.ADCS = 0b110;
    // Configure voltage reference
    ADCON1bits.ADPREF = 0;
    ADCON1bits.ADNREF = 0;
    // Channel select
    ADCON0bits.CHS = 0b00010;
    // Right justify result
    ADFM = 1;
    // Turn on ADC
    ADON = 1;
    // Configure interrupt
    clear_adc_flag();
    GIE = 1;
    PEIE = 1;
    ADIE = 1;
}

unsigned short char_to_short(char lo, char hi){
    unsigned short num = (unsigned short) hi;
    return (num << 8) + lo;
}

unsigned short read_adc(){
    char low = ADRESL;
    char high = ADRESH;
    clear_adc_flag();
    return char_to_short(low, high);
}

void setup(){
    setup_clock();
}

void __interrupt() int_routine(void){
    if(ADIF){
        volatile unsigned short conversion = read_adc();
        clear_adc_flag();
    }
}

void main(void) {
    setup();
    setup_adc();
    do_conversion();
    while(1){
        PORTC = 3;
        __delay_ms(5000);
        PORTC = 0;
        __delay_ms(5000);
    }
}