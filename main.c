#include <msp430.h> 

//Ponte H
#define STANDBY BIT7    // P2.7
#define AIN1 BIT2       // Sa�da A1 P2.2
#define AIN2 BIT3       // Sa�da A2 P2.3
#define BIN1 BIT5       // Sa�da B1 P2.5
#define BIN2 BIT6       // Sa�da B2 P2.6

//Funcoes dos motores
void direitaFrenteTurbo(void);
void esquerdaFrenteTurbo(void);
void direitaFrente(void);
void esquerdaFrente(void);
void direitaRe(void);
void esquerdaRe(void);
void full_brakeD(void);
void full_brakeE(void);
void full_brakeTodo(void);
void stop(void);

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
    P2OUT |= BIT0;
    unsigned long i;
    unsigned char x=0;
    do{
        switch(x){
        case 0:
            direitaFrente();
            break;
        case 1:
            esquerdaFrente();
            break;
        }
       if(x==2) direitaRe();
       if(x==3) esquerdaRe();
       //if(x==4) full_brakeTodo();
        //__no_operation();
        for(i=0;i<100000;i++);

        if(x<5)
            x++;
        else
            x=0;

    }while(1);

}

void ini_TimerA0(void){
    /*
     * TimerA1
     * clock: SMCLK/8 ~ 500 kHz
     * Fdiv: 8
     * Modo: up
     * TA1CCR0: 62499
     * TA1CCR1: 31249
     * TA1CCR2: 31249
     */

    TA1CTL = TASSEL1 + MC0;
    TA1CCTL1 = OUTMOD0 + OUTMOD1 + OUTMOD2 + OUT;
    TA1CCTL2 = OUTMOD0 + OUTMOD1 + OUTMOD2 + OUT;
    TA1CCR0 = 65535;
    TA1CCR1 = 10;
    TA1CCR2 = 32249;
}



void ini_P1_P2(void){
    /*
     * Porta 1.x - saída nivel baixo
     *
     */



     /*
     * Porta 2.x - saída nivel baixo
     *      P2.0 - trigger
     *      P2.1 - saída PWM A
     *      P2.2 - saída A1
     *      P2.3 - saída A2
     *      P2.4 - saída PWM B
     *      P2.5 - saída B1
     *      P2.6 - saída B2
     *      P2.7 - standby
     */

    P2SEL &= ~(BIT6+BIT7);                     //Bit 6 e 7 do P2 como IO
    P2SEL |= BIT1 + BIT4; //selecionando pwma e pwmb
    P2DIR = 0xFF;   //toda a porta P2 como sa�da
    P2OUT = 0;          //todas as sa�das em n�vel l�gico 0



}


void ini_uCon(void){
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    /* CONFIG. BCS
     * MCLK = DCOCLK ~ 1 MHz
     * SMCLK = DCOCLK ~  1 MHz
     */

    DCOCTL = CALDCO_1MHZ;
    BCSCTL1 = CALBC1_1MHZ;
    
    __enable_interrupt();

}


void RE(void)
{
    P2OUT |= AIN1;
    P2OUT &= ~AIN2;


    P2OUT &= ~BIN1;
    P2OUT |= BIN2;
    TA1CCR2 = 28000;
}
void direitaFrente(void)
{
    P2OUT |= AIN2;
    P2OUT &= ~AIN1;
    TA1CCR2 = 550;
}

// Motor B para frente
void esquerdaFrente(void)
{
    P2OUT &= ~BIN1;
    P2OUT |= BIN2;
    TA1CCR2 = 550;
}


// Motor A para tras
void direitaRe(void)
{
    P2OUT |= AIN1;
   P2OUT &= ~AIN2;
   TA1CCR2 = 550;
}

// Motor B para tras
void esquerdaRe(void)
{
    P2OUT &= ~BIN2;
    P2OUT |= BIN1;
    TA1CCR2 = 550;
}
// Parar os motores, todos alto
void full_brakeD(void)
{
    P2OUT &= ~AIN1;
    P2OUT &= ~AIN2;
}

void full_brakeE(void)
{
    P2OUT &= ~BIN1;
    P2OUT &= ~BIN2;
}

void full_brakeTodo(void)
{
    P2OUT |= BIN1;
    P2OUT |= BIN2;
    P2OUT |= AIN1;
    P2OUT |= AIN2;
}
// Parar os motores, todos baixo
void stop(void)
{
    P2OUT &= ~AIN1;
    P2OUT &= ~AIN2;

    P2OUT &= ~BIN1;
    P2OUT &= ~BIN2;
}
