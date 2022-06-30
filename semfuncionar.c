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
void timerA1_Pwm(void);


unsigned char iniciar=0, direcao=0, achou=0,entrou=0;

/*Declarações para o Sensor Sonar*/
float diff,distance,diff2,distance2;
int i=0,n=0,temp[2],temp0=0, temp1=0;
//-----------------------------------------
/*Declarações para o Sensor de Linha*/
unsigned char v=0;
//-----------------------------------------
int main(void)
{
    /*Chamada das Funções*/
    ini_uCon();
    ini_P1_P2();
    timerA1_Pwm();
//-----------------------------
    do{
      //  __delay_cycles(500000);  //0.5 second delay
       if (entrou == 0){

               if(iniciar==0){
                    P2OUT |= STANDBY;
                    direitaFrente();
                    esquerdaFrente();
                    entrou=99;
                    entrou=0;
                    iniciar=1;
                   }
               else if (iniciar ==1){
                   full_brakeTodo();
                   P2OUT &=~(STANDBY);
                   iniciar =0;
                   entrou = 99;
               }
       }

    }while(1);

}

/*RTI's*/

#pragma vector=PORT1_VECTOR
__interrupt void P1_RTI(void){

    if(P1IFG&BIT3){

        entrou =0;
        P1IFG &= ~BIT3;

    }

   if(P1IFG&BIT5){             //sensor saiu da facha branca
        P1IFG &= ~BIT5;     //limpa flag
        while(achou!=1){    //enquando sensores laterais não achar a faixa branca
            if(P1IN&(BIT1+BIT4)){       //sensores laterais achou
                if(P1IN&BIT4){          //sensor esquerdo achou
                    if(direcao==0){         //se tiver indo para frente
                        full_brakeD();          //para roda direita para o carro virar para direita
                        while(P1IN&BIT5);       //???
                        direitaFrente();        //roda direita frente
                    }else if(direcao==1){                      //sentido contrario
                        full_brakeE();
                        while(P1IN&BIT5);
                        esquerdaFrente();
                    }
                }
                if(P1IN&BIT1){      //sensor direito achou
                    if(direcao==0){         //se tiver indo para frente
                        full_brakeE();      //para roda esquerda
                        while(P1IN&BIT5);   //
                        esquerdaFrente();   //roda esquerda frente
                    }else if(direcao==1){              //sentido contrario
                        full_brakeD();
                        while(P1IN&BIT5);
                        direitaFrente();
                    }
                }
                achou=1;
            }
        }
    }
}

void timerA1_Pwm(void){
    /*
     * TimerA1
     * clock: SMCLK/4 ~ 4MHz
     * Fdiv: 8
     * Modo: up
     * TA1CCR0: 39999
     * TA1CCR1: 4
     * TA1CCR2: 20000
     */

    TA0CTL = TASSEL1 + MC0 + ID1 + ID0;
    //TA1CCTL0 = CCIE;
    TA0CCTL1 = OUTMOD0 + OUTMOD1 + OUTMOD2 + OUT;
  //  TA1CCTL2 = OUTMOD0 + OUTMOD1 + OUTMOD2 + OUT;
    TA0CCR0 = 2000 ;
    TA0CCR1 = 500;
   // TA1CCR2 = 20000;

}


void ini_P1_P2(void){
    /*
     *    Porta 1.x - saída nivel baixo
     *         P1.0 - N.C.
     *         P1.1 - SENSOR LINHA DIREITO
     *         P1.2 - PWM A
     *         P1.3 - BOTAO
     *         P1.4 - Sensor linha ESQUERDO
     *         P1.5 - Sensor linha MEIO
     *         P1.6 - PWM B
     *         P1.7 - N.C.
------------------------------------
    * Porta P2.x - saída nivel baixo
     *      P2.0 - trigger
     *      P2.1 - saída
     *      P2.2 - saída A1
     *      P2.3 - saída A2
     *      P2.4 - saída
     *      P2.5 - saída B1
     *      P2.6 - saída B2
     *      P2.7 - standby
     */


    P2DIR = 0xFF;   //toda a porta P2 como saida
    P2OUT = 0;      //todas as saidas em nivel logico 0


    P1DIR = ~(BIT1+BIT3+BIT4+BIT5);      //toda a porta P1 como entrada
    P1REN = BIT3+BIT4+BIT5+BIT1;   //habilita resistores de pull(up/down)
    P1OUT = BIT3+BIT4+BIT5+BIT1;   //pull UP
    P1SEL = BIT2 + BIT6;
    P1IE  = BIT3+BIT5;   //habilita interrup��o das entradas P1
    P1IES  = BIT3+BIT5;   //a borda de descida que provoca interrup��o (descida = 1 - subida = 0)
    P1IFG = 0;      //Apaga o Flag da interrup��o de todas as entradas

}


void ini_uCon(void){
    WDTCTL = WDTPW | WDTHOLD;

    /* CONFIG. BCS
     * MCLK = DCOCLK ~ 1 MHz
     * SMCLK = DCOCLK ~  1 MHz
     */

    DCOCTL = CALDCO_1MHZ;
    BCSCTL1 = CALBC1_1MHZ;
    BCSCTL2= DIVS1;

    __enable_interrupt();

}


/* Funções para atividades do motor*/

void RE(void)
{
    P2OUT |= AIN1;
    P2OUT &= ~AIN2;


    P2OUT &= ~BIN1;
    P2OUT |= BIN2;
    //TA1CCR2 = 28000;
}
void direitaFrente(void)
{
    P2OUT |= AIN2;
    P2OUT &= ~AIN1;
    //TA1CCR2 = 550;
}

// Motor B para frente
void esquerdaFrente(void)
{
    P2OUT &= ~BIN1;
    P2OUT |= BIN2;
    //TA1CCR2 = 550;
}


// Motor A para tras
void direitaRe(void)
{
    P2OUT |= AIN1;
   P2OUT &= ~AIN2;
   //TA1CCR2 = 550;
}

// Motor B para tras
void esquerdaRe(void)
{
    P2OUT &= ~BIN2;
    P2OUT |= BIN1;
    //TA1CCR2 = 550;
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

