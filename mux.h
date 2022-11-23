// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef MUX_H
#define	MUX_H

#include <xc.h> // include processor files - each processor file is guarded.

void setup_mux(){
    // Will use RA5, RA4 and RB5
    // Make these pins digital
    char change_mask = 0b00100000;
    ANSELB = (ANSELB & ~change_mask) | 0b00000000;
    change_mask = 0b00110000;
    ANSELA = (ANSELA & ~change_mask) | 0b00000000;
    // RA4 and RA5 are channel select outputs
    TRISA4 = 0;
    TRISA5 = 0;
    // RB5 is inhibit
    TRISB5 = 0;
    // use weak pull ups
    WPUA4 = 1;
    WPUA5 = 1;
    WPUB5 = 1;
}

void channel_select(char channel){
    if(channel > 3) return;
    char change_mask = 0b00110000;
    channel = (char) (channel << 4);
    PORTA = (PORTA & ~change_mask) | (channel & change_mask);
}

void inhibit_output(char inhibit){
    RB5 = (__bit)inhibit;
}

#endif	/* MUX_H */