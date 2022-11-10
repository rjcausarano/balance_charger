#ifndef BALANCE_H
#define	BALANCE_H

#include <xc.h>

void setup_balance(){
    // Make these pins digital
    ANS5 = 0;
    ANSA4 = 0;
    ANSC5 = 0;
    // Pins are outputs
    TRISA5 = 0;
    TRISA4 = 0;
    TRISC5 = 0;
}

void set_balance_bits(char bits[3]){
    RA5 = bits[0];
    RA4 = bits[1];
    RC5 = bits[2];
}

#endif	/* BALANCE_H */

