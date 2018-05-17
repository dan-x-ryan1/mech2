/* 
 * File:   main.c
 * Author: Robert Stephenson
 *
 * 05/04/18
 */
#include <xc.h>
#include "ProcessorConfig.h"
#include "ISR.h"
#include "MXK.h"
#include "Config.h"
#include "Functions.h"
#include <math.h>
#define _XTAL_FREQ 8000000  
#include "Colours.h"
#include "Console.h"
#include <stdio.h>
#include "Motor.h"
#include "LCD.h"
#include "LED.h"
#include "HMI.h"
#include "dans.h"



#define HIGH_BYTE(y) (((y)>>8) & 255)
#define LOW_BYTE(y) ((y) & 255)
//Laclan Global Variables 




int T_VAR = 0;
int Square_Dist = 0;
int mod = 0;
UINT8 BumpSensor = 0;
UINT8 Cliff_FRight = 0;
UINT8 Cliff_FLeft = 0;
UINT8 Cliff_Right = 0;
UINT8 Cliff_Left = 0;


int BumpState = 0;

//PositionMode Variables 
int mode;
//int Motor_Count=0;
int Low_Sensor=10000;
int ADC_Reading=0;
int Close_Pos = 0;
int count=0;
int CCW_Count;
int CW_Count;
int Sensor_Observe_Fin=0;
//ADC VAriables


//LED Variables
int countOn = 124;       //When to turn LED on based on frequency. 1Hz.
int countOff = 248; 
int timercount = 0;

//temp vars 
int test;
//dans 


int Init_Timer0()
{
    T0CONbits.TMR0ON = 1;       //1:TMR0 ON
    T0CONbits.T08BIT = 1;       //1:8-BIT, 0:16-BIT
    T0CONbits.T0CS=0;           //0:Internal clock cycle (CLKO)
    T0CONbits.T0SE=0;           //0:Increment low-high transition
    T0CONbits.PSA=0;            //0:Use Prescaler
    T0CONbits.T0PS2=1;          //TOPS2,1,0 = Set Prescaler Value
    T0CONbits.T0PS1=1;          //111=256, 110=128, 101=64, 100=32, 011=16
    T0CONbits.T0PS0=1;          //010=8, 001=4, 000=2

    INTCONbits.TMR0IE = 1;      //1:Enable TMR0 overflow interrupt 
    INTCONbits.PEIE = 1;        //1: Enables all unmasked peripheral interrupts
    INTCONbits.GIE = 1;         //1: Enable Global Interrupt Enable bit
    
}
 






void InitSerialCommunication(){
    //Setup Serial comms
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC7 = 1;
    
    TXSTA1 = 0b00100100;
    RCSTA1 = 0b10010000;
    
    BAUDCON1bits.DTRXP = 0;
    BAUDCON1bits.SCKP = 0;
    BAUDCON1bits.WUE = 0;
    BAUDCON1bits.ABDEN = 0;
    
    //SET BAUD RATE HIGH
    TXSTA1bits.BRGH = 1;
    BAUDCON1bits.BRG16 = 0;
    SPBRG1 = 51; //0.16% error
}




int Get_Sensor_State(){
    
    eusartSend(142);        // get state of sensor
    eusartSend(7);     // bump sensor 
    while(!PIR1bits.RC1IF);
    BumpSensor = RCREG1;  
    
    eusartSend(142);        // get state of sensor
    eusartSend(9);     // Left Cliff Sensor
    while(!PIR1bits.RC1IF);
    Cliff_Left = RCREG1;  

    eusartSend(142);        // get state of sensor
    eusartSend(10);     // Front Left Cliff Sensor
    while(!PIR1bits.RC1IF);
    Cliff_FLeft = RCREG1;  
        
    eusartSend(142);        // get state of sensor
    eusartSend(11);     // Front Right Cliff Sensor
    while(!PIR1bits.RC1IF);
    Cliff_FRight = RCREG1; 
    
    eusartSend(142);        // get state of sensor
    eusartSend(12);     // Right Cliff sensor
    while(!PIR1bits.RC1IF);
    Cliff_Right = RCREG1;  

    return 1;
}



void BangBang(int RSpeedH, int RSpeedL, int LSpeedH, int LSpeedL){      // Function used for BangBang and contains 4 variables: 
                                                                        //Right wheel high and low bytes && Left wheel High and Low bytes
    eusartSend(145);                                                        
    eusartSend(HIGH_BYTE(RSpeedH));
    eusartSend(LOW_BYTE(RSpeedL));
    eusartSend(HIGH_BYTE(LSpeedH));
    eusartSend(LOW_BYTE(LSpeedL));
}

void Robot_Init(){
    eusartSend(128); // TURN IO ON
    eusartSend(132); // Full Mode
    Robot_Stop();
    UpdateDistance();
}


void LEDOn (){
    eusartSend(139);
    eusartSend(10);
    eusartSend(255);
    eusartSend(255);
}

void LEDOff (){
    eusartSend(139);
    eusartSend(10);
    eusartSend(0);
    eusartSend(0);
}

void LEDToggle(){
    //LED
        if (INTCONbits.TMR0IF == 1){    //If Overflow flag is set
            INTCONbits.TMR0IF=0;        //Reset Flag
            timercount++;                    //count the number of times overflow flag is set
            if (timercount==countOn){
                LEDOn();
            }
            if(timercount==countOff){
                LEDOff();
                timercount=0;
            }
        }
}





int STP1 (){        //energise the IN1 pin
    TRISA2=0;
    TRISA3=0;
    TRISD5=0;
    TRISD7=0;
    RD5=0;
    RD7=0;
    RA2=0;
    RA3=0;
    RA2 = 1;
    }

int STP2 (){        //energise the IN2 pin
    TRISA2=0;
    TRISA3=0;
    TRISD5=0;
    TRISD7=0;
    RD5=0;
    RD7=0;
    RA2=0;
    RA3=0;
    RA3 = 1;
    }
int STP3 (){        //energise the IN3 pin
    TRISA2=0;
    TRISA3=0;
    TRISD5=0;
    TRISD7=0;
    RD5=0;
    RD7=0;
    RA2=0;
    RA3=0;
    RD5 = 1;
     }
int STP4 (){        //energise the IN4 pin 
    TRISA2=0;
    TRISA3=0;
    TRISD5=0;
    TRISD7=0;
    RD5=0;
    RD7=0;
    RA2=0;
    RA3=0;
    RD7 = 1;
    }

void Sensor_Observe() { //Scans the surroundings and locates the closest object
    int High_Sensor = 0;
    int Low_Sensor  = 10000;
    
    printf("%c", ENDOFTEXT);
   
    printf("Sensor Moving!   \n");
    
    Console_Render();
    
    while (Motor_Count <= 512) { //Continue the steps unless the stepper has turned 360 degrees
        printf("%c",ENDOFTEXT);
        Motor_Count++; //Increment the motor counter
        ADCAverage(); //Find the ADC reading at this point
        printf("\nADC: %d \n\n", average);
            Console_Render();
        if (Low_Sensor > average) { //If the current ADC is lower, then this will be the Closest Object so far
            Low_Sensor = average;
            Close_Pos = Motor_Count; // The number of steps to find the closest object is now identified
        }
        else if (average > High_Sensor){
            High_Sensor = average;
            Found_Flag = Motor_Count;
            printf ("\n Found Flag: %d", Found_Flag);
            Console_Render();
        }
        SteppingCW(); //Step 4 times. One time is unnecessarily specific
    }
    RD5 = 0;
    RD7 = 0;
    RA2 = 0;
    RA3 = 0;
    Sensor_Observe_Fin = 1; //Set all pins to low. 
    //Flag_Wall = Close_Pos - Found_Flag;
    
    CCW_Count = 512 - Close_Pos; //This determines the number of steps to turn to the closest object, but in the CCW direction
    Found_Flag = 512 - Found_Flag;
    return;
}



void Closest_Object(){
    printf("%c", ENDOFTEXT);
    printf("Stepped 360 degrees");
    printf("\nThe Closes object (mm): %d", Low_Sensor);
    printf("\nGoing back to: %d", Close_Pos);    
    Console_Render();
    while (!CCW_Count<=0){      //If the motor has not turned to the closest object, keep turning
        CCW_Count--;            //Decrement the number of steps that have been turned
        printf("%c", ENDOFTEXT); 
        printf("\nSteps Travelled: %d",CCW_Count);
        Console_Render();
        ADCAverage ();          //Call and display the sensor reading
       SteppingCCW();           //Continue stepping CCW
    }
    RD5=0;
    RD7=0;
    RA2=0;
    RA3=0; 
    return;                     //Set all pins to low and return from this function
}


void Reset_variables(){     //A list of all the variables that need to be reset every time they exit the mode
    
    Square_Dist=0;
    Sensor_Observe_Fin=0;
    Motor_Count = 0;
    Low_Sensor=10000;
    ADC_Reading=0;
    Close_Pos = 0;
    count=0;
    T_VAR = 0;
    
    mod = 0;
  /*  Get_Sensor_State();
    test = (BumpSensor & 0b00000011);  
    if (test>0){
    mode=0;
    } */
    
    return;
}


//END SETUP
void main(){
    struct square square[4][5];
    
    
    //Init MXK Pins
    MXK_Init();
    Init_Timer0();
    InitSerialCommunication();
    Robot_Init();
    ISR_Enable();
    FunctInitButton();
    InitADC();
    LED_Init(eLED2);
   
   // dans new functions 
         
    
    //Init HMI
    if (MXK_BlockSwitchTo(eMXK_HMI)) {
        HMI_Init();
        LCD_Init();
        Console_Init();
        if (MXK_Release())
            MXK_Dequeue();
    }

    
    loop(){
        
        ADCAverage();
        
        HMI_SetNumber(average);
        
        HMI_Render();
        
        HMI_Poll();
        
        //Get_Sensor_State();
        
        bool down = HMIBoard.mDown.mGetState();
        bool right = HMIBoard.mRight.mGetState();
        bool up = HMIBoard.mUp.mGetState();
        bool left = HMIBoard.mLeft.mGetState();
        if (down) {mode=3;} 
        if (right) {mode =1;}
        if (up) {mode=2;}//change back to 2 later  
        if (left) {mode=4;}
        
        
       
        
        if (MXK_BlockSwitchTo(eMXK_HMI)) {
            
            if (mode==0){
                
                
            printf("%c", ENDOFTEXT); 
            printf("Robot Ready! \n\n");
            
            Console_Render();
            
            }
            if (mode == 3){
            
           // Init_Pos_Array();
            Console_Render();
            
            //Sensor_Observe();
            //Closest_Object();
            //LED_Set(eLED2, 1);
            //delay_ms(350);
            //LED_Set(eLED2, 0);
            //Move_ADC_To (0);
            Create_Maze();
            movement1();
            //movement2();
            //untilVirtual();
            //movement4();
            //printf("done");
            Console_Render();
            mode=0;
            
            
            Console_Render();  
            
            }
            
            Console_Render();                                         
            if (MXK_Release())
            MXK_Dequeue();
        }
    }
}
