#include<string.h>
#include<math.h>
#include <htc.h>
#include "lcd.h"
#include "uart.h"
#include "IntToStr.h"
__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_OFF & BOREN_OFF & LVP_OFF & CPD_OFF & WRT_OFF & CP_OFF);



#include<pic.h>

#define RS RD6
#define EN RD7
#define D0 RB0
#define D1 RB1
#define D2 RB2
#define D3 RB3
#define D4 RB4
#define D5 RB5
#define D6 RB6
#define D7 RB7

#define _XTAL_FREQ 20000000
void checkRFID();
void present();
void absent();
void delay(int time);
char rfid[12];
char validIDs[4][12]={"4000287DBFAA","4000289834C4","4000285C2C18","400028687474"};
//char studentName[3][10]={"Rajdeep","Rutuja","Shubham"};
int recordbook[3]=0;
int counter=0;
char buffer[2];

void main()
{
    TRISB = 0x00;
    TRISD = 0x00;
    TRISC = 0xFF;
    PORTDbits.RD6 = 0;
    PORTDbits.RD4 = 0;
    Lcd8_Init();
    UART_Init(9600);
    
    Lcd8_Clear();
    Lcd8_Set_Cursor(1,1);
    Lcd8_Write_String("Place your card");
    while(1)
    checkRFID();
    
}
void initial_State()
{
    Lcd8_Clear();
    Lcd8_Set_Cursor(1,1);
    Lcd8_Write_String("Place your card");
    
}
void delay(int time)
{
    int i,j;
    for(i=0;i<time;i++)
        for(j=0;j<10000;j++);
}
void present()
{   
    ftoa(counter+1,buffer,0);
    Lcd8_Set_Cursor(1,1);
    Lcd8_Write_String("Student_");
    Lcd8_Write_String(buffer);
    Lcd8_Set_Cursor(2,1);
    Lcd8_Write_String(" present");
    delay(100);
}
void absent()
{
    ftoa(counter+1,buffer,0);
    Lcd8_Set_Cursor(1,1);
    Lcd8_Write_String("Student_");
    Lcd8_Write_String(buffer);
    Lcd8_Set_Cursor(2,1);
    Lcd8_Write_String(" absent");
    delay(100);
    
}
void checkRFID()
{  
    int i,a=0;
   
    if(UART_Data_Ready())
    {
    for(i=0;i<12;)
     {
       if(UART_Data_Ready())
       {
         rfid[i] = UART_Read();
         i++;
       }
    }
    a = 1;
    }
    if(a==1)
    {
    Lcd8_Clear();
    Lcd8_Set_Cursor(1,0);
    Lcd8_Write_String("ID:");
    Lcd8_Write_String(rfid);
   
    if(!strcmp(rfid,"4000287DBFAA"))
    {
        if(recordbook[0]==1)
        {
            Lcd8_Set_Cursor(2,0);
            Lcd8_Write_String("Already Done");
        }
        if(recordbook[0]==0)
        {
            Lcd8_Set_Cursor(2,0);
            Lcd8_Write_String("Rajdeep Present ");
            recordbook[0]=1;
        } 
    }   
    if(!strcmp(rfid,"4000289834C4"))
    {
         if(recordbook[1]==1)
        {
            Lcd8_Set_Cursor(2,0);
            Lcd8_Write_String("Already Done");
        }
         if(recordbook[1]==0)
         {
            Lcd8_Set_Cursor(2,0);
            Lcd8_Write_String("Rutuja Present  ");
            recordbook[1]=1;
         }
    }
    if(!strcmp(rfid,"4000285C2C18"))
    {
         if(recordbook[2]==1)
        {
            Lcd8_Set_Cursor(2,0);
            Lcd8_Write_String("Already Done");
        }
        if(recordbook[2]==0)
        {
             Lcd8_Set_Cursor(2,0);
             Lcd8_Write_String("Shubham Present ");
             recordbook[2]=1;
        }
       
        
    }
    if(!strcmp(rfid,"400028687474"))
    {
        Lcd8_Set_Cursor(2,0);
        Lcd8_Write_String("Welcome Teacher ");
        delay(35);
        for(counter=0;counter<3;counter++)
        {
            Lcd8_Clear();
            delay(10);
            (recordbook[counter]==1)?present():absent();
            delay(10);
        }
    }
    
    a = 0;
    delay(100);
    initial_State();
    for(i=0;i<12;i++)
    rfid[i] = 0;
    }
    
}
