#include <msp430.h> 

unsigned int up_counter=0,distance_cm=0;
void ini_uCon(void);
void ini_TimerA0(void);
void iniP(void);
/**
 * main.c
 */
int main(void)
{
	ini_uCon();
	ini_TimerA0();
	iniP();
	
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
    if (TACCTL0 & CCI)            // Raising edge
    {
        up_counter = TA0CCR0;      // Copy counter to variable
    }
    else                        // Falling edge
    {
        // Formula: Distance in cm = (Time in uSec)/58
        distance_cm = (TA0CCR0 - up_counter)/58;
    }
    TA0CTL &= ~TAIFG;           // Clear interrupt flag - handled


}



void iniP(void){
    P1DIR = ~(BIT1+BIT2);
    P1SEL = (BIT1+BIT2);
    P1OUT = ~BIT0;
}

void ini_TimerA0(void){
    /*
         * TimerA0
         * clock: SMCLK/8 ~ 500 kHz
         * Fdiv: 8
         * Modo: continuo
         * Modo: captura
         *  Borda subida/descida, CCI0A, CCIE, CAP, SCS
         */

    TA0CTL = TASSEL1 + MC0 + MC1 + ID0 + ID1;
    TA0CCTL0 = CM0 + CM1 + SCS + CCIE + CAP;
   // TA0CCTL1 = CM0 + CM1 + SCS + CCIE + CAP;

    _BIS_SR(GIE);

    /*
     * TimerA1
     * clock: SMCLK/8 ~ 500 kHz
     * Fdiv: 8
     * Modo: up
     * TA1CCR0: 62499
     * TA1CCR1: 31249
     * TA1CCR2: 31249
     */

/*    TA1CTL = TASSEL1 + MC0 + ID0 + ID1;
    TA1CCTL1 = OUTMOD0 + OUTMOD1 + OUTMOD2 + OUT;
    TA1CCTL2 = OUTMOD0 + OUTMOD1 + OUTMOD2 + OUT;
    TA1CCR0 = 62499;
    TA1CCR1 = 31249;
    TA1CCR2 = 31249;*/
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


    __enable_interrupt();

}
