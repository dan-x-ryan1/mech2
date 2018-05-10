/* 
 * File:   main.c
 * Author: Robert Stephenson
 *
 * 10/05/18
 * CODE CLEANED
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

#define DLY 2 

#define HIGH_BYTE(y) (((y)>>8) & 255)
#define LOW_BYTE(y) ((y) & 255)
//Laclan Global Variables 

int Current_Dist = 0;
UINT8 Current_Dist1 = 0;
UINT8 Current_Dist2 = 0;
int mode = 0;
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
int Motor_Count = 0;
int Low_Sensor = 10000;
int ADC_Reading = 0;
int Close_Pos = 0;
int count = 0;
int CCW_Count;
int CW_Count;
int Sensor_Observe_Fin = 0;
//ADC VAriables
int numReadings = 50;
int thisReading;
int readings[50]; // the readings from the analog input
int readIndex = 0; // the index of the current reading
int total = 0; // the running total
int average = 0; // the average
int x = 0;

//LED Variables
int countOn = 124; //When to turn LED on based on frequency. 1Hz.
int countOff = 248;
int timercount = 0;

//temp vars 
int test;

int Init_Timer0() {
    T0CONbits.TMR0ON = 1; //1:TMR0 ON
    T0CONbits.T08BIT = 1; //1:8-BIT, 0:16-BIT
    T0CONbits.T0CS = 0; //0:Internal clock cycle (CLKO)
    T0CONbits.T0SE = 0; //0:Increment low-high transition
    T0CONbits.PSA = 0; //0:Use Prescaler
    T0CONbits.T0PS2 = 1; //TOPS2,1,0 = Set Prescaler Value
    T0CONbits.T0PS1 = 1; //111=256, 110=128, 101=64, 100=32, 011=16
    T0CONbits.T0PS0 = 1; //010=8, 001=4, 000=2
    INTCONbits.TMR0IE = 1; //1:Enable TMR0 overflow interrupt 
    INTCONbits.PEIE = 1; //1: Enables all unmasked peripheral interrupts
    INTCONbits.GIE = 1; //1: Enable Global Interrupt Enable bit
}

void InitADC() // This function used to setup the ADC Module 
{
    ADCON1 = 0b10101101;
    for (thisReading = 0; thisReading < numReadings; thisReading++) {
        readings[thisReading] = 0; // load 0 into all array elements 
    }
}

int FunctReadADC(int channel) //This function reads the ADC and returns value of the ADRES register 
{ // can read any channel
    if (channel > 4) return 0; //Invalid Channel
    ADCON0 = 0x00; //initi the ADC 0 
    ADCON0 = (channel << 2); //Select ADC Channels appropriate 
    ADON = 1; //turn on the adc module
    GODONE = 1; //Start conversion
    while (GODONE); //wait for the conversion to finish, cant sample and convert at the same time 
    ADON = 0; //switch off adc
    return ADRES; //result is the ADRES register 
}

int ADCAverage() {
    // subtract the last reading:
    total = total - readings[readIndex];
    // read from the sensor:
    readings[readIndex] = (FunctReadADC(0));
    // add the reading to the total:
    total = total + readings[readIndex];
    // advance to the next position in the array:
    readIndex++;
    // if we're at the end of the array...
    if (readIndex >= numReadings) {
        //...wrap around to the beginning:
        readIndex = 0;
    }
    // calculate the average:
    average = total / numReadings;
    average = -0.0000009 * pow(average, 3) + 0.0015 * pow(average, 2) - 0.9208 * average + 227.33;
    //  average = (((1/(average/296.29))-0.1226)/0.0141)
    average *= 10; // convert to mm
    
    HMI_SetNumber(average);
    HMI_Render();
    return average;
}

void InitSerialCommunication() {
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

void eusartSend(UINT8 sendChar) {
    while (!TXSTA1bits.TRMT) {}
    TXREG1 = sendChar;
}

UINT8 eusartRec() {
    while (!PIR1bits.RC1IF) {}
    return RCREG1;
}

void Robot_Stop() {
    eusartSend(137); //turn wheels off
    eusartSend(0);
    eusartSend(0);
    eusartSend(0);
    eusartSend(0);
}
void Get_Sensor_State() {
    eusartSend(142); // get state of sensor
    eusartSend(7); // bump sensor 
    BumpSensor = (eusartRec() & 0b00000011);
}

int UpdateDistance() { // Update the distance from sensor reading 
    eusartSend(142);
    eusartSend(19);
    Current_Dist1 = eusartRec();
    Current_Dist2 = eusartRec();
    
    Current_Dist = Current_Dist2; // Using the low byte as the distance reading


    return Current_Dist;

}

void Drive(int Speed) {
    int Rad = 32768;
    eusartSend(137); //drive, (speed hb, lb, radius hb, lb)
    eusartSend(HIGH_BYTE(Speed));
    eusartSend(LOW_BYTE(Speed));
    eusartSend(HIGH_BYTE(Rad));
    eusartSend(LOW_BYTE(Rad));
}

int Turn(int Angle, int Speed) { // function for wheel turn angle  
    eusartSend(137); //drive, (speed hb, lb, radius hb, lb)
    eusartSend(HIGH_BYTE(Speed));
    eusartSend(LOW_BYTE(Speed));
    eusartSend(HIGH_BYTE(0));
    eusartSend(LOW_BYTE(1));
    int C_Angle = 0;
    eusartSend(157);
    eusartSend(HIGH_BYTE(Angle));
    eusartSend(LOW_BYTE(Angle));

    Robot_Stop();

    eusartSend(142);
    eusartSend(7);
    eusartRec();

}

void BangBang(int RSpeedH, int RSpeedL, int LSpeedH, int LSpeedL) { // Function used for BangBang and contains 4 variables: 
    //Right wheel high and low bytes && Left wheel High and Low bytes
    eusartSend(145);
    eusartSend(HIGH_BYTE(RSpeedH));
    eusartSend(LOW_BYTE(RSpeedL));
    eusartSend(HIGH_BYTE(LSpeedH));
    eusartSend(LOW_BYTE(LSpeedL));
}



void Robot_Init() {
    eusartSend(128); // TURN IO ON
    eusartSend(132); // Full Mode
    Robot_Stop();
    UpdateDistance();
}

void LEDOn() {
    eusartSend(139);
    eusartSend(10);
    eusartSend(255);
    eusartSend(255);
}

void LEDOff() {
    eusartSend(139);
    eusartSend(10);
    eusartSend(0);
    eusartSend(0);
}

void LEDToggle() {
    //LED
    if (INTCONbits.TMR0IF == 1) { //If Overflow flag is set
        INTCONbits.TMR0IF = 0; //Reset Flag
        timercount++; //count the number of times overflow flag is set
        if (timercount == countOn) {
            LEDOn();
        }
        if (timercount == countOff) {
            LEDOff();
            timercount = 0;
        }
    }
}

void Coil_Setup() {
    TRISA2 = 0;
    TRISA3 = 0;
    TRISD5 = 0;
    TRISD7 = 0;
    RD5 = 0;
    RD7 = 0;
    RA2 = 0;
    RA3 = 0;
}

int SteppingCCW() { // turn motor rotate counter-clock wise
    STP4();
    delay_ms(DLY);
    STP3();
    delay_ms(DLY);
    STP2();
    delay_ms(DLY);
    STP1();
    delay_ms(DLY);
}

int SteppingCW() { // turn motor rotate clock wise
    STP1();
    delay_ms(DLY);
    STP2();
    delay_ms(DLY);
    STP3();
    delay_ms(DLY);
    STP4();
    delay_ms(DLY);

}

int STP1() { //energise the IN1 pin
    Coil_Setup();
    RA2 = 1;
}

int STP2() { //energise the IN2 pin
    Coil_Setup();
    RA3 = 1;
}

int STP3() { //energise the IN3 pin
    Coil_Setup();
    RD5 = 1;
}

int STP4() { //energise the IN4 pin 
    Coil_Setup();
    RD7 = 1;
}

void Sensor_Observe2() { //Sensor Scan for Integration function. More details in Sensor_Observe 
    printf("%c", ENDOFTEXT);
    printf("                 \n");
    printf("Sensor Moving!\n");
    printf("                 \n");
    printf("                 \n");
    printf("                 \n");
    printf("                 \n");
    printf("                 \n");
    printf("                 \n");
    printf("                 \n");
    Console_Render();
    while (Motor_Count <= 512) {
        Motor_Count++;
        ADCAverage();
        if (Low_Sensor > average) {
            Low_Sensor = average;
            Close_Pos = Motor_Count;
        }
        SteppingCCW();
    }

    Console_Render();
    RD5 = 0;
    RD7 = 0;
    RA2 = 0;
    RA3 = 0;
    Sensor_Observe_Fin = 1;

    CW_Count = 512 - Close_Pos;
    return;
}

void Closest_Object2() { //Closest Object found for Integration Function. More details in Closest_Object
    while (!CW_Count <= 0) {
        CW_Count--;
        printf("%c", ENDOFTEXT);
        printf("Stepped 360 degrees");
        printf("\nThe Closes object (mm): %d", Low_Sensor);
        printf("\nGoing back to: %d", Close_Pos);
        Console_Render();
        ADCAverage();
        SteppingCW();
    }
    RD5 = 0;
    RD7 = 0;
    RA2 = 0;
    RA3 = 0;
    return;
}

void Sensor_Observe() { //Scans the surroundings and locates the closest object
    printf("%c", ENDOFTEXT);
    printf("                 \n");
    printf("Sensor Moving!   \n");
    printf("                 \n");
    printf("                 \n");
    printf("                 \n");
    printf("                 \n");
    printf("                 \n");
    printf("                 \n");
    Console_Render();
    while (Motor_Count <= 512) { //Continue the steps unless the stepper has turned 360 degress
        Motor_Count++; //Increment the motor counter
        ADCAverage(); //Find the ADC reading at this point
        if (Low_Sensor > average) { //If the current ADC is lower, then this will be the Closest Object so far
            Low_Sensor = average;
            Close_Pos = Motor_Count; // The number of steps to find the closest object is now identified
        }
        SteppingCW(); //Step 4 times. One time is unnecessarily specific
    }
    RD5 = 0;
    RD7 = 0;
    RA2 = 0;
    RA3 = 0;
    Sensor_Observe_Fin = 1; //Set all pins to low. 

    CCW_Count = 512 - Close_Pos; //This determines the number of steps to turn to the closest object, but in the CCW direction
    return;
}

void Closest_Object() {
    printf("%c", ENDOFTEXT);
    printf("Stepped 360 degrees");
    printf("\nThe Closes object (mm): %d", Low_Sensor);
    printf("\nGoing back to: %d", Close_Pos);
    Console_Render();
    while (!CCW_Count <= 0) { //If the motor has not turned to the closest object, keep turning
        CCW_Count--; //Decrement the number of steps that have been turned
        printf("%c", ENDOFTEXT);
        printf("\nSteps Travelled: %d", CCW_Count);
        Console_Render();
        ADCAverage(); //Call and display the sensor reading
        SteppingCCW(); //Continue stepping CCW
    }
    RD5 = 0;
    RD7 = 0;
    RA2 = 0;
    RA3 = 0; //Set all pins to low and return from this function             
}



void Reset_variables() { //A list of all the variables that need to be reset every time they exit the mode

    Square_Dist = 0;
    Sensor_Observe_Fin = 0;
    Motor_Count = 0;
    Low_Sensor = 10000;
    ADC_Reading = 0;
    Close_Pos = 0;
    count = 0;
    T_VAR = 0;
    mod = 0;
    return;
}


//END SETUP

void main() {
    //Init MXK Pins
    MXK_Init();
    Init_Timer0();
    InitSerialCommunication();
    Robot_Init();
    ISR_Enable();
    FunctInitButton();
    InitADC();
    LED_Init(eLED2);
    //Init HMI
    if (MXK_BlockSwitchTo(eMXK_HMI)) {
        HMI_Init();
        LCD_Init();
        Console_Init();
        if (MXK_Release())
            MXK_Dequeue();
    }

    loop() {

        LEDToggle();
        ADCAverage();
        
        HMI_SetNumber(average);
        HMI_Render();
        HMI_Poll();
        
        bool down = HMIBoard.mDown.mGetState();
        bool right = HMIBoard.mRight.mGetState();
        bool up = HMIBoard.mUp.mGetState();
        bool left = HMIBoard.mLeft.mGetState();
        
        if (down) {
            mode = 3;
        }
        if (right) {
            mode = 1;
        }
        if (up) {
            mode = 2;
        }//change back to 2 later  
        if (left) {
            mode = 4;
        }




        if (MXK_BlockSwitchTo(eMXK_HMI)) {

            if (mode == 0) {
                LED_Set(eLED2, OFF);
                Robot_Stop();
                Reset_variables();

                printf("%c", ENDOFTEXT);
                printf("Robot Ready! \n");
                printf("Press right for 5m\n drive,\n");
                printf("Press down for\nsquare drive,\n");
                printf("Press up for IR scan mode,\n");
                printf("Press left for wall follow\n");
                
                printf("                 \n");
                printf("                 \n");
                printf("                 \n");
                printf("                 \n");
                Console_Render();
            }
            else if (mode == 1) { // Mode 1 5 meter drive 
                LED_Set(eLED2, ON);
                Drive(200);
                T_VAR += UpdateDistance();
                printf("%c", ENDOFTEXT);
                printf("Traveled Distance:\n%d", T_VAR);
                printf("                 \n");
                printf("                 \n");
                printf("                 \n");
                printf("                 \n");
                printf("                 \n");
                printf("                 \n");
                printf("                 \n");
                printf("                 \n");
                Console_Render();
                if (T_VAR >= 5000) {
                    Robot_Stop();
                    mode = 0;
                }
            }
            else if (mode == 2) { // Mode 2 point the IR to the nearest object
                LED_Set(eLED2, ON);
                Sensor_Observe();

                Closest_Object();
                delay_ms(2000);
                mode = 0;
            }
            else if (mode == 3) { //Mode 3: Square Drive
                Drive(250);
                LED_Set(eLED2, ON);
                T_VAR += UpdateDistance();

                Square_Dist = T_VAR + mod;
                printf("%c", ENDOFTEXT);
                printf("SQUARE DRIVE MODE 3! \n");
                printf("Total: %d\n", Square_Dist);
                printf("                 \n");
                printf("                 \n");
                printf("                 \n");
                printf("                 \n");
                printf("                 \n");
                printf("                 \n");
                printf("                 \n");
                printf("                 \n");
                Console_Render();

                if (T_VAR >= 1000) {
                    Turn(82, 100);
                    T_VAR = 0;
                    mod += 1000;
                }

                if (Square_Dist >= 4000) {

                    Robot_Stop();
                    delay_ms(500);
                    mode = 0;
                }

            } else if (mode == 4) { // Mode 4: Robot follows wall    
                Sensor_Observe2();
                Closest_Object2();

                int Target_Distance = Low_Sensor;
                LED_Set(eLED2, ON);
                Get_Sensor_State();
                if (test > 0) {
                    mode = 0;
                }

                while (BumpSensor == 0) {
                    ADCAverage();
                    printf("%c", ENDOFTEXT);
                    printf("The target distance is: %d\n", Target_Distance);
                    printf("ADC AVERAGE %d\n", average);
                    printf("                 \n");
                    printf("                 \n");
                    printf("                 \n");
                    printf("                 \n");
                    printf("                 \n");
                    printf("                 \n");
                    printf("                 \n");
                    printf("                 \n");
                    Console_Render();
                    HMI_SetNumber(average);

                    HMI_Render();

                    HMI_Poll();
                    if (Target_Distance <= average + 2 || Target_Distance >= average - 2) {
                        BangBang(50, 50, 50, 50);
                    }
                    if (Target_Distance > average + 2) {
                        BangBang(50, 50, 40, 40);
                    }

                    if (Target_Distance < average - 2) {
                        BangBang(40, 40, 50, 50);
                    }

                    if (Target_Distance > average + 5) {
                        BangBang(50, 50, 30, 30);
                    }

                    if (Target_Distance < average - 5) {
                        BangBang(30, 30, 50, 50);
                    }

                    Get_Sensor_State();
                    printf("Besneor: %d", BumpSensor);
                    if (BumpSensor > 0) {
                        Target_Distance = 0;
                        break;
                    }

                    if (HMIBoard.mUp.mGetState()) {
                        up = HMIBoard.mUp.mGetState();
                        if (up) {
                            mode = 0;
                        }
                        break;
                    }
                }
                mode = 0;
            }
            Console_Render();
            if (MXK_Release())
                MXK_Dequeue();
        }
    }
}
