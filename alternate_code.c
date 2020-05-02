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
int counter=0;
char buffer[2];
struct student
{
    char name[15];
    char id[12];
    int recordbook;
}s[3];
void main()
{
    
    TRISB = 0x00;
    TRISD = 0x00;
    TRISC = 0xFF;
    PORTDbits.RD6 = 0;
    PORTDbits.RD4 = 0;
    Lcd8_Init();
    UART_Init(9600);
    
    s[0].name = "Rajdeep";
    s[0].id = "4000287DBFAA";
    s[1].name = "Rutuja";
    s[1].id = "4000289834C4";
    s[2].name = "Shubham";
    s[2].id = "4000285C2C18";
    for(int k=0;k<3;k++)
        s[k].recordbook = 0;
    
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
    if(!strcmp(rfid,"400028687474"))
    {
        Lcd8_Set_Cursor(2,0);
        Lcd8_Write_String("Welcome Teacher ");
        delay(50);
        for(counter=0;counter<3;counter++)
        {
            Lcd8_Clear();
            delay(10);
            (s[counter].recordbook==1)?present():absent();
            delay(10);
        }
    }
    for(int x=0;x<3;x++)
    {
        if(!strcmp(rfid,s[x].id))
        {
            if(s[x].recordbook == 1)
            {
                 Lcd8_Set_Cursor(2,0);
                 Lcd8_Write_String("Already Done");
            }
            if(s[x].recordbook == 0)
            {
                Lcd8_Set_Cursor(2,0);
                Lcd8_Write_String(s[x].name);
                Lcd8_Write_String(" Present");
                s[x].recordbook=1;
            }
        }
    }
    
    a = 0;
    delay(100);
    initial_State();
    for(i=0;i<12;i++)
    rfid[i] = 0;
    }
    
}

