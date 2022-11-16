// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef MUX_H
#define	MUX_H

#include <xc.h> // include processor files - each processor file is guarded.  

void setup_mux(){
    // Make these pins digital
    ANSC0 = 0;
    ANSC1 = 0;
    ANSC2 = 0;
    // RC0 and RC1 are channel select outputs
    TRISC0 = 0;
    TRISC1 = 0;
    // RC2 is inhibit
    TRISC2 = 0;
}

void channel_select(char channel){
    char mask = 0b00000111;
    PORTC = (PORTC & ~mask) | (channel & mask);
}

void inhibit_output(char inhibit){
    RC2 = (__bit)inhibit;
}

#endif	/* MUX_H */

