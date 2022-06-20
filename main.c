#include <msp430.h> 


/**
 * main.c
 */

void ini_uCon(void);
void ini_P1_P2(void);
void ini_TimerA0(void);

int main(void)
{
    ini_uCon();
    ini_P1_P2();
    ini_TimerA0();

}

void ini_TimerA0(void){
    /*
     * TimerA0
     * clock: SMCLK/8 ~ 500 kHz
     * Fdiv: 8
     * Modo: up
     * TA0CCR0: 62499
     * TA0CCR1: 31249
     * TA0CCR2: 31249
     */

    TA1CTL = TASSEL1 + MC0 + ID0 + ID1;
    TA1CCTL1 = OUTMOD0 + OUTMOD1 + OUTMOD2 + OUT;
    TA1CCTL2 = OUTMOD0 + OUTMOD1 + OUTMOD2 + OUT;
    TA1CCR0 = 62499;
    TA1CCR1 = 31249;
    TA1CCR2 = 31249;
}



void ini_P1_P2(void){
    /*
     * Porta 1.x - sa�da nivel baixo
     *
     */



     /*
     * Porta 2.x - sa�da nivel baixo
     *      P2.0 - sa�da Standby
     *      P2.1 - sa�da PWM A
     *      P2.2 - sa�da A1
     *      P2.3 - sa�da A2
     *      P2.4 - sa�da PWM B
     *      P2.5 - sa�da B1
     *      P2.6 - sa�da B2
     */



}


void ini_uCon(void){
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    /* CONFIG. BCS
     * MCLK = DCOCLK ~ 1 MHz
     * SMCLK = DCOCLK ~  1 MHz
     * ACLK = LFXT1/1 = 32768 Hz
     */

    DCOCTL = CALDCO_1MHZ;
    BCSCTL1 = CALBC1_1MHZ;
    BCSCTL2 = DIVS1;
    BCSCTL3 = XCAP0+XCAP1;

    while(BCSCTL3&LFXT1OF);
    __enable_interrupt();

}
