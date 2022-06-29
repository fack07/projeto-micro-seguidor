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
void timerA0_Captura(void);
void timerA0_Debounce(void);


unsigned char inicar=0, direcao=0, achou=0;

/*Declarações para o Sensor Sonar*/
float diff,distance=16,diff2,distance2=16;
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
    timerA0_Captura();
    timerA0_Debounce();
//-----------------------------

    do{
        switch(direcao){        //verifica direcao que esta indo
            case 0:                     //sentido frente
                if(distance <= 15)      //verifica distancia do objeto
                    direcao = 1;        //muda a direcao
                    fullbrake();        
                    direitare();
                    esquerdare();
                break;
            case 1:
                if(distance2 <= 15)
                    direcao = 0;
                    fullbrake();
                    direitafrente();
                    esquerdafrente();
                break;
        }
      
        
        __delay_cycles(500000);  //0.5 second delay

    }while(1);

}

/*RTI's*/

#pragma vector=PORT1_VECTOR
__interrupt void P1_RTI(void){
    
    if(P1IFG&BIT3){

        P1IFG &= ~BIT3;
        if(iniciar==0){
            direitafrente();
            esquerddafrente();
            iniciar=1;
        }else{
            fullbreake();
            iniciar=0;
        }
                
    }
    
    if(P1IFG&BIT5){             //sensor saiu da facha branca
        P1IFG &= ~BIT5;     //limpa flag
        while(achou!=1){    //enquando sensores laterais não achar a faixa branca
            if(P1IN&(BIT4+BIT6)){       //sensores laterais achou
                if(P1IN&BIT4){          //sensor esquerdo achou
                    if(direcao==0){         //se tiver indo para frente
                        full_brakeD();          //para roda direita para o carro virar para direita
                        while(P1IN&BIT5);       //???
                        direitafrente();        //roda direita frente
                    }else if(direcao==1){                      //sentido contrario
                        full_brakeE();
                        while(P1IN&BIT5);
                        esquerdafrente();
                    }
                }
                if(P1IN&BIT6){      //sensor direito achou
                    if(direcao==0){         //se tiver indo para frente
                        full_brakeE();      //para roda esquerda
                        while(P1IN&BIT5);   //
                        esquerdafrente();   //roda esquerda frente
                    }else if(direcao==1){              //sentido contrario
                        full_brakeD();      
                        while(P1IN&BIT5);
                        direitafrente();
                    }
                }
                achou=1;
            }
        }
    }
    
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_A0(void){
    TA1CCTL0 &= ~CCIE; //desabilita interrupção do timer1
    if( (~P1IN) & BIT3 ){// Verifica se tecla realmente foi pressionada
        if(iniciar==0){
            direitafrente();
            esquerddafrente();
            iniciar=1;
        }else{
            fullbreake();
            iniciar=0;
        }
        
    }
    P1IFG &= ~BIT3; // Obrigatorio limpar flag aqui!
    P1IE |= BIT3; // Habilita int. do BIT3 da P1
}


#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A2(void){
        temp0 = TA0CCR0;
        //n += 1;
        TA0CCTL0 &= ~CCIFG ;
        if (n==1)
        {
            if (temp0<temp1){
                diff=(65536-temp1)+temp0;
            }
            else {
                    diff=temp0-temp1;
                  }

        }
       temp1=temp0;
       n=1;
      distance = diff/58;
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void Timer_A(void){
    temp[0] = TA0CCR1;
    //n += 1;
    TA0CCTL1 &= ~CCIFG ;
    if (i==1)
    {
        if (temp[0]<temp[1]){
            diff2=(65536-temp[1])+temp[0];
        }
        else {
                diff2=temp[0]-temp[1];
              }

    }
    temp[1]=temp[0];
    i=1;
    distance2= diff2/58;
}


void timerA0_Captura(void){
    
    TA0CTL = TASSEL1 + MC1 ;  //SMCLK Cont.mode
    TA0CCTL0 = CAP + CCIE + CM0 + CM1 + SCS ; //Capture mode, interrupt enable, CCIxA, cap borda desc., sincronizar cap source.
    TA0CCTL1 = CAP + CCIE + CM0 + CM1 + SCS ;
    
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

    TA1CTL = TASSEL1 + MC0 + ID1;
    //TA1CCTL0 = CCIE;
    TA1CCTL1 = OUTMOD0 + OUTMOD1 + OUTMOD2 + OUT;
    TA1CCTL2 = OUTMOD0 + OUTMOD1 + OUTMOD2 + OUT;
    TA1CCR0 = 65535;
    TA1CCR1 = 10;
    TA1CCR2 = 20000;
    
}




void ini_P1_P2(void){
    /*
     *    Porta 1.x - saída nivel baixo
     *         P1.0 - N.C.
     *         P1.1 - ECHO1
     *         P1.2 - ECHO2
     *         P1.3 - BOTAO
     *         P1.4 - Sensor linha ESQUERDO
     *         P1.5 - Sensor linha MEIO
     *         P1.6 - Sensor linha DIREITO
     *         P1.7 - N.C.
------------------------------------
    * Porta P2.x - saída nivel baixo
     *      P2.0 - trigger
     *      P2.1 - saída PWM A
     *      P2.2 - saída A1
     *      P2.3 - saída A2
     *      P2.4 - saída PWM B
     *      P2.5 - saída B1
     *      P2.6 - saída B2
     *      P2.7 - standby
     */

    //P2DIR = BIT0; // P2.0 como saída (trigger)
    P2SEL = BIT0; // Timer1_A3.CCI1A
    P2SEL &= ~(BIT6+BIT7);   //Bit 6 e 7 do P2 como IO
    P2SEL |= BIT1 + BIT4; //selecionando pwma e pwmb
    P2DIR = 0xFF;   //toda a porta P2 como saida
    P2OUT = 0;      //todas as saidas em nivel logico 0


    P1DIR = ~(BIT3+BIT4+BIT5+BIT6);      //toda a porta P1 como entrada
    P1SEL = BIT1 + BIT2; // TA0.CCI0A  | TA0.CCI1A 
    P1REN = BIT3+BIT4+BIT5+BIT6;   //habilita resistores de pull(up/down)
    P1OUT = BIT3+BIT4+BIT5+BIT6;   //pull UP
    P1IE  = BIT3 + BIT5;   //habilita interrup��o das entradas P1
    //P1IES= BIT5+BIT6+BIT7;   //a borda de descida que provoca interrup��o (descida = 1 - subida = 0)
    P1IFG = 0;      //Apaga o Flag da interrup��o de todas as entradas

}


void ini_uCon(void){
    WDTCTL = WDTPW | WDTHOLD;

    /* CONFIG. BCS
     * MCLK = DCOCLK ~ 1 MHz
     * SMCLK = DCOCLK ~  1 MHz
     */

    DCOCTL = CALDCO_16MHZ;
    BCSCTL1 = CALBC1_16MHZ;
    
    

    __enable_interrupt();

}



/* Funções para atividades do motor*/

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
