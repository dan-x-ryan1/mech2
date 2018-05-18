/* 
 * File:   dans.h
 * Author: Daniel
 *
 * Created on 14 May 2018, 2:05 PM
 */

#ifndef DANS_H
#define	DANS_H
#include "Types.h"
#include <math.h>


int Current_Direction = 3;

//================================================//
int top = -1;
#define MAXSIZE 20
int stack[MAXSIZE];
#define x_grid 5
#define y_grid 4
#define DLY 2 
#define SQUARE 10000

int Current_Dist = 0;
int Current_Dist1 = 0;
int Current_Dist2 = 0;
int Found_Flag = 0;
int mode = 0;
int Motor_Count = 0;
int Steps = 0;

//adc vars
int numReadings = 10;
int thisReading;
int readings[10 ]; // the readings from the analog input
int readIndex = 0; // the index of the current reading
int total = 0; // the running total
int average = 0; // the average
int x = 0;

//Pos tracking Var
int X_Pos;
int Y_Pos;

//Controller Variables
int k;
int Spd = 200;
int Previous_Angle;
int flag = 0;
int Desired_Level;

//Victim Count
int VictimCount=0;

void InitADC();
int FunctReadADC(int channel);
int ADCAverage();
bool isempty();
bool isfull();
int peek();
int pop();
int push(int data);
void giveID();
void Init_Pos_Array();
int Travel(int direction, int Dist, int Test_ADC, int Spd);
Turn(int Angle, int Speed, int Dir);
void movement1();
void movement2();
void movement4();
void Drive(int SpeedL, int SpeedR);
void eusartSend(UINT8 sendChar);
UINT8 eusartRec();
int UpdateDistance();
void Robot_Stop();
void untilVirtual();
int Get_VirtualWall_State();
int SteppingCCW();
int SteppingCW();
void Sensor_Observe();
void Move_ADC_To(int Angle);
void Find_Flag();
int Create_Maze(); 

int Detect_Victim();






//======================================================//
struct square
{
    int id;
    int victim;

};

#endif	/* DANS_H */

