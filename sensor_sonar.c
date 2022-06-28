//Usando
//P2.1 como ECHO
//P1.6 como TRIGGER
#include <msp430.h> 
#include <stdlib.h>
#include <stdio.h>

float diff,distance,diff2,distance2;
int i=0,n=0,temp[2],temp0=0, temp1=0;

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
        distance2= diff2/58;
        __delay_cycles(500000);  //0.5 second delay
    }
}

void iniP(void){

    P2DIR = BIT0; // P1.6 como saída (trigger)
    P2SEL = BIT0; // Timer1_A3.CCI1A
    P1SEL = BIT1 + BIT2 + BIT6; // TA0.CCI0A  | TA0.CCI1A | TA0.1

}

void ini_TimerA1(void){
    TA1CTL = TASSEL1 + MC0 ;  //SMCLK Up mode.
    TA1CCR0 = 65535;
    TA1CCR1 = 10;
    TA1CCTL0 = OUTMOD0+OUTMOD1+OUTMOD2+OUT; //Reset-Set
    TA0CTL = TASSEL1 + MC1 ;  //SMCLK Cont.mode
    TA0CCTL0 = CAP + CCIE + CM0 + CM1 + SCS ; //Capture mode, interrupt enable, CCIxA, cap borda desc., sincronizar cap source.
    TA0CCTL1 = CAP + CCIE + CM0 + CM1 + SCS ;
}
void ini_uCon(void){
        WDTCTL = WDTPW + WDTHOLD;   // stop watchdog timer
        BCSCTL1= CALBC1_1MHZ;
        DCOCTL = CALDCO_1MHZ;
        __enable_interrupt();
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void Timer_A(void){
        temp[i] = TA0CCR1;
        i += 1;
        TA0CCTL1 &= ~CCIFG ;
        if (i==2) {
            diff=temp[i-1]-temp[i-2];
            i=0;
        }

}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A2(void){
        temp0 = TA0CCR0;
        //n += 1;
        TA0CCTL0 &= ~CCIFG ;
        if (n==1)
        {
            if (temp0<temp1){
                diff2=(65536-temp1)+temp0;
            }
            else {
                    diff2=temp0-temp1;
                  }

        }
        temp1=temp0;
       n=1;
}
