
#ifndef ADC_H
#define	ADC_H

#include <xc.h> // include processor files - each processor file is guarded.  

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
    TRISA2 = 1;
    ANSA2 = 1;
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

#endif	/* ADC_H */

