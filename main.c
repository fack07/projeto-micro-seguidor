#include <msp430.h> 


#define STANDBY BIT0    // P2.0
#define AIN1 BIT2       // Sa�da A1 P2.2
#define AIN2 BIT3       // Sa�da A2 P2.3
#define BIN1 BIT5       // Sa�da B1 P2.5
#define BIN2 BIT6       // Sa�da B2 P2.6


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
     * TA1CCR0: 62499
     * TA1CCR1: 31249
     * TA1CCR2: 31249
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
     * Porta 1.x - saída nivel baixo
     *
     */



     /*
     * Porta 2.x - saída nivel baixo
     *      P2.0 - saída Standby
     *      P2.1 - saída PWM A
     *      P2.2 - saída A1
     *      P2.3 - saída A2
     *      P2.4 - saída PWM B
     *      P2.5 - saída B1
     *      P2.6 - saída B2
     */
    
    P2SEL &= ~(BIT6+BIT7);                     //Bit 6 e 7 do P2 como IO
    P2SEL |= BIT1 + BIT4; //selecionando pwma e pwmb
    P2DIR |= BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7;   //toda a porta P2 como sa�da
    P2OUT = 0;          //todas as sa�das em n�vel l�gico 0



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
