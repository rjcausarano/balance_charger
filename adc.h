
#ifndef ADC_H
#define	ADC_H

#include <xc.h> // include processor files - each processor file is guarded.  

static unsigned short vdd_mv_ = 0;
static char is_converting_ = 0;

void do_conversion(){
    // Wait acquisition time
    __delay_us(5);
    is_converting_ = 1;
    GO_nDONE = 1;
}

static void clear_adc_flag(){
    ADIF = 0;
}

char is_converting(){
    return is_converting_;
}

void setup_adc(unsigned short vdd_mv){
    // Configure Port
    TRISC2 = 1;
    ANSC2 = 1;
    // Configure Clock
    ADCON1bits.ADCS = 0b110;
    // Configure voltage reference
    ADCON1bits.ADPREF = 0;
    //ADCON1bits.ADNREF = 0;
    // Channel select
    ADCON0bits.CHS = 0b00110;
    // Right justify result
    ADFM = 1;
    // Turn on ADC
    ADON = 1;
    // Configure interrupt
    clear_adc_flag();
    GIE = 1;
    PEIE = 1;
    ADIE = 1;
    vdd_mv_ = vdd_mv;
}

unsigned short chars_to_short(char hi, char lo){
    unsigned short num = (unsigned short) hi;
    return (num << 8) + lo;
}

void short_to_chars(unsigned short short_num, char chars[]){
    chars[0] = (char)(0x00FF & short_num);
    chars[1] = (char)(0x00FF & (short_num >> 8));
}

unsigned short counts_to_mv(unsigned short counts){
    return (__uint24)vdd_mv_ * counts / 1023;
}

unsigned short mv_to_counts(unsigned short millivolts){
    return (__uint24)1023 * millivolts / vdd_mv_;
}

static unsigned short read_adc(){
    char low = ADRESL;
    char high = ADRESH;
    clear_adc_flag();
    return chars_to_short(high, low);
}

unsigned short get_adc_voltage(){
    is_converting_ = 0;
    unsigned short counts = read_adc();
    clear_adc_flag();
    return counts_to_mv(counts);
}

#endif	/* ADC_H */

