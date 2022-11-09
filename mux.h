// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef MUX_H
#define	MUX_H

#include <xc.h> // include processor files - each processor file is guarded.  

void setup_mux(){
    // Make these pins digital
    ANSELC = 0b11110000;
    // RC0 to RC2 are channel select outputs
    TRISC0 = 0;
    TRISC1 = 0;
    TRISC2 = 0;
    // RC3 is inhibit
    TRISC3 = 0;
}

void channel_select(char channel){
    volatile char mask = 0b00000111;
    PORTC = (PORTC & ~mask) | (channel & mask);
}

void inhibit_output(char inhibit){
    RC3 = (__bit)inhibit;
}

#endif	/* MUX_H */

