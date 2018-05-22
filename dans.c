#include <stdio.h>
#include "dans.h"
#include "Types.h"
#include "Console.h"
#include <math.h>
#include "HMI.h"

//enum{NORTH,SOUTH,EAST,WEST};


int Create_Maze(){
    //init the stack to 0 
int count = 0;
int x = 0;
int y = 0;
for (int p = 0; p < MAXSIZE; p++){
    stack[p]=4;
}
    for(x = 0 ; x < x_grid; ++x){
        for(y = 0 ; y < y_grid; ++y){
            maze[x][y].id = count;
            maze[x][y].victim = 0;
            ++count;
        }
    }
x = 0;
y = 0;
        
    X_Pos = 3;
    Y_Pos = 1; 
    //0
        maze[x][y].walls[0] = 1;
        maze[x][y].walls[1] = 0;
        maze[x][y].walls[2] = 0;
        maze[x][y].walls[3] = 1;
        maze[x][y].Options  = 2;
    //1
        ++y;
        maze[x][y].walls[0] = 1;
        maze[x][y].walls[1] = 0;
        maze[x][y].walls[2] = 1;
        maze[x][y].walls[3] = 0;
        maze[x][y].Options = 2;
    //2  
        ++y;
        maze[x][y].walls[0] = 1;
        maze[x][y].walls[1] = 0;
        maze[x][y].walls[2] = 0;
        maze[x][y].walls[3] = 0;
        maze[x][y].Options = 3;
    //3    
        ++y;
        maze[x][y].walls[0] = 1;
        maze[x][y].walls[1] = 1;
        maze[x][y].walls[2] = 0;
        maze[x][y].walls[3] = 0;
        maze[x][y].Options = 2;
    //4  
        ++y;
        maze[x][y].walls[0] = 1;
        maze[x][y].walls[1] = 1;
        maze[x][y].walls[2] = 0;
        maze[x][y].walls[3] = 1;
        maze[x][y].Options = 1;
    //5    
        ++x;
        y=0;
        maze[x][y].walls[0] = 0;
        maze[x][y].walls[1] = 0;
        maze[x][y].walls[2] = 0;
        maze[x][y].walls[3] = 1;
        maze[x][y].Options = 3;
    //6 
        ++y;
        maze[x][y].walls[0] = 1;
        maze[x][y].walls[1] = 1;
        maze[x][y].walls[2] = 1;
        maze[x][y].walls[3] = 0;
        maze[x][y].Options = 1;
    //7 
        ++y;
        maze[x][y].walls[0] = 0;
        maze[x][y].walls[1] = 1;
        maze[x][y].walls[2] = 0;
        maze[x][y].walls[3] = 1;
        maze[x][y].Options = 2;
    //8  
        ++y;
        maze[x][y].walls[0] = 0;
        maze[x][y].walls[1] = 0;
        maze[x][y].walls[2] = 1;
        maze[x][y].walls[3] = 1;
        maze[x][y].Options = 2;
    //9 
        ++y;
        maze[x][y].walls[0] = 0;
        maze[x][y].walls[1] = 1;
        maze[x][y].walls[2] = 1;
        maze[x][y].walls[3] = 0;
        maze[x][y].Options = 2;
    //10 
         ++x;
        y=0;
        maze[x][y].walls[0] = 0;
        maze[x][y].walls[1] = 0;
        maze[x][y].walls[2] = 0;
        maze[x][y].walls[3] = 1;
        maze[x][y].Options = 3;
    //11   
        ++y;
        maze[x][y].walls[0] = 1;
        maze[x][y].walls[1] = 0;
        maze[x][y].walls[2] = 1;
        maze[x][y].walls[3] = 0;
        maze[x][y].Options = 2;
    //12  
        ++y;
        maze[x][y].walls[0] = 0;
        maze[x][y].walls[1] = 0;
        maze[x][y].walls[2] = 0;
        maze[x][y].walls[3] = 0;
        maze[x][y].Options = 4;
    //13  
        ++y;
        maze[x][y].walls[0] = 1;
        maze[x][y].walls[1] = 0;
        maze[x][y].walls[2] = 1;
        maze[x][y].walls[3] = 0;
        maze[x][y].Options = 2;
    //14
        ++y;
        maze[x][y].walls[0] = 1;
        maze[x][y].walls[1] = 1;
        maze[x][y].walls[2] = 0;
        maze[x][y].walls[3] = 0;
        maze[x][y].Options = 2;
    //15 
         ++x;
        y=0;
        maze[x][y].walls[0] = 0;
        maze[x][y].walls[1] = 0;
        maze[x][y].walls[2] = 1;
        maze[x][y].walls[3] = 1;
        maze[x][y].Options = 2;
    //16   
        ++y;
        maze[x][y].walls[0] = 1;
        maze[x][y].walls[1] = 1;
        maze[x][y].walls[2] = 1;
        maze[x][y].walls[3] = 0;
        maze[x][y].Options = 1;
    //17    
        ++y;
        maze[x][y].walls[0] = 0;
        maze[x][y].walls[1] = 1;
        maze[x][y].walls[2] = 1;
        maze[x][y].walls[3] = 1;
        maze[x][y].Options = 1;
    //18 
        ++y;
        maze[x][y].walls[0] = 1;
        maze[x][y].walls[1] = 0;
        maze[x][y].walls[2] = 1;
        maze[x][y].walls[3] = 1;
        maze[x][y].Options = 1;
    //19 
        ++y;
        maze[x][y].walls[0] = 0;
        maze[x][y].walls[1] = 1;
        maze[x][y].walls[2] = 1;
        maze[x][y].walls[3] = 0;
        maze[x][y].Options = 2;
        
        
        
}

void InitADC() // This function used to setup the ADC Module 
{
    ADCON1 = 0b10101101;
    for (thisReading = 0; thisReading < numReadings; thisReading++) {
        readings[thisReading] = 0; // load 0 into all array elements 
    }
}

int FunctReadADC(int channel) //This function reads the ADC and returns value of the ADRES register 
{                               // can read any channel
    if(channel>4) return 0;  //Invalid Channel
    ADCON0=0x00; //initi the ADC 0 
    ADCON0=(channel<<2);   //Select ADC Channels appropriate 
    ADON=1;  //turn on the adc module
    GODONE=1;  //Start conversion
    while(GODONE); //wait for the conversion to finish, cant sample and convert at the same time 
    ADON=0;  //switch off adc
    return ADRES; //result is the ADRES register 
}

int ADCAverage (){
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
        // ...wrap around to the beginning:
        readIndex = 0;
        }

        // calculate the average:
        average = total / numReadings;
        average = -0.0000009*pow(average, 3)+0.0015*pow(average, 2)-0.9208*average+227.33;
        
        //average*=10;
        HMI_SetNumber(average);
        HMI_Render();
        return average;       
}
void eusartSend (UINT8 sendChar){
    while(!TXSTA1bits.TRMT){};
    TXREG1 = sendChar;
}      
UINT8 eusartRec(){
    while(!PIR1bits.RC1IF){}
    return (int) RCREG1;
}
bool isempty() {

   if(top == -1)
      return 1;
   else
      return 0;
}
bool isfull() {

   if(top == MAXSIZE)
      return 1;
   else
      return 0;
}
int peek() {
   return stack[top];
}
int pop() {
   int data;
	
   if(!isempty()) {
      data = stack[top];
      top = top - 1;   
      return data;
   } else {
      printf("Could not retrieve data, Stack is empty.\n");
      
   }
}
int push(int data) {

   if(!isfull()) {
      top = top + 1;   
      stack[top] = data;
   } else {
      printf("Could not insert data, Stack is full.\n");
   }
}

void Drive(int SpeedL, int SpeedR) {
    if (SpeedL < 0){
        SpeedL = -1*SpeedL;
        SpeedL = ~SpeedL;
    }
    if (SpeedR < 0){
        SpeedR = -1*SpeedR;
        SpeedR = ~SpeedR;
    }
    
    eusartSend(145); //drive, (speed hb, lb, radius hb, lb)
    eusartSend(HIGH_BYTE(SpeedL));
    eusartSend(LOW_BYTE(SpeedL));
    eusartSend(HIGH_BYTE(SpeedR));
    eusartSend(LOW_BYTE(SpeedR));
}
int UpdateDistance() { // Update the distance from sensor reading 
    eusartSend(142);
    eusartSend(19);
    Current_Dist1 = eusartRec();
    Current_Dist2 = eusartRec();
    
    Current_Dist = ((Current_Dist1 <<(8)) & 0b1111111100000000 | (Current_Dist2 & 255)); // Using the low byte as the distance reading
    return Current_Dist;
}
void Robot_Stop(){ 
    eusartSend(137); //turn wheels off
    eusartSend(0);
    eusartSend(0);
    eusartSend(0);
    eusartSend(0);
    }
int Get_VirtualWall_State(){
    eusartSend(142);    // get state of sensor
    eusartSend(13);     // virtual wall detect 
    int Vitual_Wall_Sensor = (eusartRec());
    return Vitual_Wall_Sensor;
}
int Detect_Victim(){
    eusartSend(142);    // get state of sensor 
    eusartSend(17);     // Home Base detect 
    int Home_Base_State = (eusartRec());
    if(Home_Base_State >= 240  && Home_Base_State <= 254){
        return 1;
    }

    else{
        printf("\nNOTHING!!!");
        Console_Render();
    }
     return 0;
//    return Home_Base_State;
    
}
Turn(int Angle, int Speed, int Dir){ // function for wheel turn angle  
    int Rad = 1; //CCW
    if (Dir == 0){
        Rad = 65535; //turn cw
        Angle = ~Angle;
    } 

    eusartSend(137); //drive, (speed hb, lb, radius hb, lb)
    eusartSend(HIGH_BYTE(Speed));
    eusartSend(LOW_BYTE(Speed));
    eusartSend(HIGH_BYTE(Rad));
    eusartSend(LOW_BYTE(Rad));
    int C_Angle = 0;
    eusartSend(157);
    eusartSend(HIGH_BYTE(Angle));
    eusartSend(LOW_BYTE(Angle));

    Robot_Stop();

    eusartSend(142);
    eusartSend(7);
    eusartRec();
}
int Travel(int direction, int Dist, int Test_ADC, int Spd, int Wall){
    int Dist_Travelled = 0;
    int Req_Dir = 0;    
    int Dir;            // Dir is a varible to define which way the robot turns
    bool Prev_Angle;
    if (Wall == 0 && flag == 0){ //left wall
        
       // Move_ADC_To(-45);
        Prev_Angle = 0;
    }
    
   
    else if (Wall == 0 && flag != 0){
        if (Prev_Angle != 0)
        Move_ADC_To(-90);
        Prev_Angle = 0;
    }
    
    if (Wall == 1 && flag != 0){
        if (Prev_Angle != 1)
        Move_ADC_To(90);
        Prev_Angle  = 1;
    } 
    
    if(direction == Current_Direction){ //if we are facing the right direction do nothing

    }
    else {                                       //if not oreient the robot to the right direction
        Req_Dir = direction - Current_Direction; //Req_Dir is a holder for the difference numerically between 
                                                 //required direction and current direction  
        if (Req_Dir < 0){               //if the number is negative, we need to convert it to positive
            Req_Dir = 83*Req_Dir*-1;    //multiply the required direction by 83* (makes robot turn 90) to convert between 
            Dir = 1;                    // a numerical difference between to an angle
                    if(Req_Dir > 166){  // if the turn is going to be over 166 (180 degrees), turn the other way 
                        Req_Dir = 332 - Req_Dir;
                        Dir = 0;        
                    }
        }
        else{
            Req_Dir = 83*Req_Dir;       // as above but we don't need to convert to positive 
            Dir = 0;
            if(Req_Dir > 166){
                        Req_Dir = 332 - Req_Dir;
                        Dir = 1;
            }
        }
        Turn(Req_Dir, 200, Dir);        //Turn(turn angle, speed, CW/CCW [1= CW, CCW = 0])
        Current_Direction = direction;  // we have turned so we are now facing the way we wanted
    }
    
    Drive(Spd,Spd);  //drive forward with the speed set
    
    
    ADCAverage();   
   // int Desired_Level = average;
    k = 4; //this is the proportional controller value
    int error;
    int spd1 =0 ;
    int spd2=0;
    
    while (Dist_Travelled<Dist){  // while we haven't gone far enough
         //left Wall
       
            ADCAverage();
            error = Desired_Level - average;
            error*=k;
            
            if (average<= 90){
        
            if (error > 300){ 
                error=30;
            }
             if (error < -30){
                error = -30;
            }
             if (error == Desired_Level + 4 || error == Desired_Level - 4){
                 Drive(Spd,Spd);
            }
            if (error >= 0){
                
            spd1 = Spd - error;   //left wall follow, right wheel
            if (Wall ==0){                
            Drive(spd1,Spd);  
             }
            else if (Wall == 1){
                Drive(Spd,spd1);     
            }
          
            }
            if (error <= 0){
            error=abs(error);
            spd2= Spd - error;
            //left wall follow, left wheel
            if (Wall == 0){   
             Drive(Spd,spd2);  
                }
            else if (Wall == 1){
                Drive(spd2,Spd);
            }
            }
           
            }
            else 
                Drive(Spd,Spd);
        //printf("%c", ENDOFTEXT);        
        Dist_Travelled += UpdateDistance();
        //printf("                     \n");
        //printf("%d \n", Dist_Travelled);  //print how far we have gone
        //printf("\nADC %d",average);
        //printf("\nDesired: %d",Desired_Level);
        //Console_Render();
         
       // Detect_Victim();
        if (Get_VirtualWall_State()){     // check if there is a V_Wall
            maze[X_Pos][Y_Pos].walls[direction] = 1;
            
            int rev_Dist = 0;             //reverse distance
           // Robot_Stop();
            printf("WALL FOUND\n");
            Console_Render();
            Drive(-Spd,-Spd);            //drive backward as the same speed
            UpdateDistance();            //make sure this is 0 before we start 
            while(rev_Dist >= -Dist_Travelled){  //reverse as far as we travelled since last turn
                
                rev_Dist += UpdateDistance();
                Console_Render();
                printf("%c", ENDOFTEXT);
                printf("rev:%d Tar: %d\n", rev_Dist, Dist_Travelled); // print how far we went & the target distance
                Console_Render();
                }
            return;  //we are back where we started, so we don't want to change position
        }
        //if(Test_ADC && ADCAverage() < 50){
        //        break;
        //}
    }
    if (direction == 0){  //if we havn't found a wall, the next 4 statements change our position depending on where we went
        X_Pos -= 1;
    }
    else if (direction == 1){
        Y_Pos += 1;
    }
    else if (direction == 2){
        X_Pos += 1;
    }
    else if (direction == 3){
        Y_Pos -= 1;
    }
    push(direction);
   
    
    maze[X_Pos][Y_Pos].Options -= 1;
        Robot_Stop(); //stop
        
}
/*
        N = 0
 * W = 3  +  E = 1
        S = 2
 
 
 */


int SteppingCCW(){                   // turn motor rotate counter-clock wise
    STP4();
    delay_ms(DLY);
    STP3();
    delay_ms(DLY);
    STP2();
    delay_ms(DLY);
    STP1();
    delay_ms(DLY);
 
 
}

int SteppingCW(){                                // turn motor rotate clock wise
    STP1();
    delay_ms(DLY);
    STP2();
    delay_ms(DLY);
    STP3();
    delay_ms(DLY);
    STP4();
    delay_ms(DLY);
 
}

void Find_Flag () {
    
    Sensor_Observe();
    while (!Found_Flag <= 0) { //If the motor has not turned to the closest object, keep turning
        Found_Flag--; //Decrement the number of steps that have been turned
        printf("%c", ENDOFTEXT);
        printf("\nSteps Travelled: %d \n\n", Found_Flag);
        
        Console_Render();
        ADCAverage(); //Call and display the sensor reading
        SteppingCCW(); //Continue stepping CCW
    }
 }


void Move_ADC_To (int Angle) {
      
    if (flag==0){
    //Angle = 90;
    Steps = (Angle * 1.42222)+200;
    flag++;
    }
    else if (!flag==0){
    Steps = (Angle * 1.42222);    
    }
    
    printf("%c", ENDOFTEXT); 
    printf("\nSteps %d", Steps);
    printf("\nFlag %d", flag);
    Console_Render();
    while (Steps >= 0){
        Steps --;
        SteppingCW();
    }
    while (Steps < 0){
        Steps ++;
        SteppingCCW();
    }
    return;
}
void Go_Backward(int direction, int spd){
    if(direction == 1)
        direction = 3;
    else if(direction == 3)
        direction =1;
    
    else if(direction == 0)
        direction = 2;
    else if(direction == 2)
        direction = 0;
    
    Travel(direction, SQUARE, 0, spd, 0); 
    maze[X_Pos][Y_Pos].Options += 1;
        
}



void Navigate_Maze(){
    int Direction;
    int Follow;
    printf("X=%d, Y=%d\n", X_Pos, Y_Pos);
    Console_Render();
    if(maze[X_Pos][Y_Pos].Options > 0){
        if(maze[X_Pos][Y_Pos].walls[1] == 0 && maze[X_Pos][Y_Pos+1].Options > 0){
            Direction = 1;    
        }
        else if(maze[X_Pos][Y_Pos].walls[2] == 0 && maze[X_Pos+1][Y_Pos].Options > 0){
            Direction = 2;      
        } 
        else if(maze[X_Pos][Y_Pos].walls[3] == 0 && maze[X_Pos][Y_Pos-1].Options > 0){
            Direction = 3;       
        } 
        else if(maze[X_Pos][Y_Pos].walls[0] == 0 && maze[X_Pos-1][Y_Pos].Options > 0){
            Direction = 0;         
        }     
        else{
            printf("&&&&&\n");
            Console_Render();
            while(1);
        }
        
        maze[X_Pos][Y_Pos].Options -= 1;
        Travel(Direction, SQUARE, 0, 200, 0);   
        
    }
    else {
        printf("no options\n");
        Console_Render();
        Go_Backward(pop(), 200);
        top -= 1;
    }
        
}

void Go_Home(){
    Go_Backward(pop(), 200);
}




