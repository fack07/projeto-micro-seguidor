//Usando
//P2.1 como ECHO
//P1.6 como TRIGGER
#include <msp430.h> 
#include <stdlib.h>
#include <stdio.h>

float diff,distance;
int i=0,temp[2];

void iniP(void);
void ini_uCon(void);
void ini_TimerA1(void);

void main(void)
{
    iniP();
    ini_uCon();
    ini_TimerA1();
    while(1){
        distance = diff/58;
        __delay_cycles(500000);  //0.5 second delay
    }
}

void iniP(void){

    P1DIR = BIT6; // P1.6 como saída (trigger)
    P2SEL = BIT1; // Timer1_A3.CCI1A
    P1SEL = BIT1 + BIT2 + BIT6; // TA0.CCI0A  | TA0.CCI1A | TA0.1

}

void ini_TimerA1(void){
    TA0CTL = TASSEL1 + MC0 ;  //SMCLK Up mode.
    TA0CCR0 = 65535;
    TA0CCR1 = 10;
    TA0CCTL1 = OUTMOD0+OUTMOD1+OUTMOD2; //Reset-Set
    TA1CTL = TASSEL1 + MC1 ;  //SMCLK Cont.mode
    TA1CCTL1 = CAP + CCIE + CCIS_0 + CM0 + CM1 + SCS ; //Capture mode, interrupt enable, CCIxA, cap borda desc., sincronizar cap source.

}
void ini_uCon(void){
        WDTCTL = WDTPW + WDTHOLD;   // stop watchdog timer
        BCSCTL1= CALBC1_1MHZ;
        DCOCTL = CALDCO_1MHZ;
        __enable_interrupt();
}

#pragma vector = TIMER1_A1_VECTOR
__interrupt void Timer_A(void){
        temp[i] = TA1CCR1;
        i += 1;
        TA1CCTL1 &= ~CCIFG ;
        if (i==2) {
            diff=temp[i-1]-temp[i-2];
            i=0;
        }

}
