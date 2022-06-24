#include <msp430.h> 


/**
 * main.c
 */

unsigned char v=0;

void  iniP(void);
void ini_uCon(void);

int main(void)
{
    iniP();
    ini_uCon();
    do{

     }while(1);
}

#pragma vector=PORT1_VECTOR
__interrupt void P1_RTI(void){
    //DESATIVA INTERRUPÇÃO, LIMPA FLAG, DISPARA DEBOUNCER
    P1IE &= ~BIT5;
    v = !(P1IN&BIT5);
    P1IFG = 0;
    P1IE |= BIT5;


}


void  iniP(void){
    P1DIR = ~BIT5;      //toda a porta P1 como entrada
    P1REN = BIT5;   //habilita resistores de pull(up/down)
    P1OUT = BIT5;   //pull UP
    P1IE  = BIT5;   //habilita interrup��o das entradas P1
    P1IES= BIT5;   //a borda de descida que provoca interrup��o (descida = 1 - subida = 0)

    P1IFG = 0;          //Apaga o Flag da interrup��o de todas as entradas


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
