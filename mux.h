// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef MUX_H
#define	MUX_H

#include <xc.h> // include processor files - each processor file is guarded.

void setup_mux(){
    // Will use RB4, RB5 and RB6
    // Make these pins digital
    char change_mask = 0b01110000;
    ANSELB = (ANSELB & ~change_mask) | 0b00000000;
    // RB4 and RB5 are channel select outputs
    TRISB4 = 0;
    TRISB5 = 0;
    // RB6 is inhibit
    TRISB6 = 0;
    // use weak pull ups
    WPUB4 = 1;
    WPUB5 = 1;
    WPUB6 = 1;
}

void channel_select(char channel){
    if(channel > 3) return;
    char change_mask = 0b00110000;
    channel = (char) (channel << 4);
    PORTB = (PORTB & ~change_mask) | (channel & change_mask);
}

void inhibit_output(char inhibit){
    RB6 = (__bit)inhibit;
}

#endif	/* MUX_H */