#include <msp430.h> 


#define STANDBY BIT7    // P2.7
#define AIN1 BIT2       // Sa�da A1 P2.2
#define AIN2 BIT3       // Sa�da A2 P2.3
#define BIN1 BIT5       // Sa�da B1 P2.5
#define BIN2 BIT6       // Sa�da B2 P2.6


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
    if(P1IFG&BIT3){
        P1IE &= ~BIT3;
        TA0CTL|=MC0;

    }
    if(P1IFG&BIT5){
        P1IE &= ~BIT5;
        P2OUT &= ~(AIN1+BIN1);
        __delay_cycles(500000);
        P2OUT |= (AIN2+BIN2);
        P1IFG &= ~BIT5;
        P1IE |= BIT5;
    }

}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void RTI_do_Timer0(void){
    //parar o timer1
    TA0CTL &= ~MC0;
    //verificar chave
    if(P1IFG & BIT3){
        if((~P1IN) & BIT3){
           // entrou =0;
        }
        P1IFG &= ~BIT3; //limpar flag
        P1IE |= BIT3;   //ativar interrupcao
    }
}


void  iniP(void){

    P1SEL = BIT2+BIT6;
    P1DIR = ~(BIT5+BIT4+BIT3+BIT1);      //toda a porta P1 como entrada
    P1REN = BIT5+BIT4+BIT3+BIT1;   //habilita resistores de pull(up/down)
    P1OUT = BIT5+BIT4+BIT3+BIT1;   //pull UP
    P1IE  = BIT3+BIT5;   //habilita interrup��o das entradas P1
    P1IES &= ~(BIT3+BIT5);   //a borda de descida que provoca interrup��o (descida = 1 - subida = 0)

    P1IFG = 0;          //Apaga o Flag da interrup��o de todas as entradas

    P2SEL &= ~(BIT6+BIT7); //selecionando pwma e pwmb
    P2DIR = 0xFF;   //toda a porta P2 como sa�da
    P2OUT = 0;          //todas as sa�das em n�vel l�gico 0




}

void ini_TimerA1(void){
    /*
     *tempo debouncer: 2ms
     *
     * CONTADOR
     *      - Clock: SMCLK ~250khz
     *      - Modo: up
     *      - Inter. desabilitada
     * MODULO 0
     *      - Funcao comparacao
     *      - Int. habilitada
     *      - TA0CCR0 = 0 //Contador parado
     */

    TA0CTL = TASSEL1;
    TA0CCTL0 = CCIE;
    TA0CCR0 = 4999;
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
