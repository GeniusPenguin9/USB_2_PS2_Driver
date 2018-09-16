#include <wiringPi.h>
#include "GPIO.h"
#include <stddef.h>

#define DataLine 0  /*BCM.17 -> wiring Pi.0 -> GPIO.0*/
#define ClockLine 7 /*BCM.4 -> wiring Pi.7 -> GPIO.7*/

char HostBuffer;

void WriteBitDev2Host(char bit_temp)
{
    pinMode(DataLine, OUTPUT);
    digitalWrite(DataLine, bit_temp);
    delayMicroseconds(20);
    pinMode(ClockLine, OUTPUT);
    digitalWrite(ClockLine, LOW);
    delayMicroseconds(40);
    pinMode(ClockLine, GPIO_CLOCK);
    delayMicroseconds(20);
}

char ReadBitHost2Dev(void)
{
    delayMicroseconds(20);
    pinMode(ClockLine, OUTPUT);
    digitalWrite(ClockLine, LOW);
    delayMicroseconds(40);
    pinMode(ClockLine, GPIO_CLOCK);
    delayMicroseconds(20);
    digitalRead(DataLine);
}

void WaitHigh(int gpio)
{
    while (digitalRead(gpio) == LOW)
        ;
}

void WaitLow(int gpio)
{
    while (digitalRead(gpio) == HIGH)
        ;
}

char GetOddParity(char byte_temp)
{
    byte_temp ^= byte_temp >> 4;
    byte_temp ^= byte_temp >> 2;
    byte_temp ^= byte_temp >> 1;

    return (~byte_temp) & 1;
}

void pinInit(void)
{
    wiringPiSetup();
    pinMode(DataLine, INPUT);
    pinMode(ClockLine, GPIO_CLOCK);
    gpioClockSet(ClockLine, 15000);
}

int SendByteDev2Host(char byte_temp)
{
    printf("d->h:0x%x", byte_temp);

    do
    {
        WaitHigh(ClockLine);                 //step1
        delayMicroseconds(50);               //step2
    } while (digitalRead(ClockLine) == LOW); //step3
    if (digitalRead(DataLine) != HIGH)       //step4
    {
        return 1;
    }
    else
    {
        delay(20); //step5
        pinMode(DataLine, OUTPUT);

        /*START BIT, step6*/
        WriteBitDev2Host(LOW);

        /*DATA BITS, step7*/
        WriteBitDev2Host(byte_temp & 1);
        WriteBitDev2Host((byte_temp & 0b10) >> 1);
        WriteBitDev2Host((byte_temp & 0b100) >> 2);
        WriteBitDev2Host((byte_temp & 0b1000) >> 3);
        WriteBitDev2Host((byte_temp & 0b10000) >> 4);
        WriteBitDev2Host((byte_temp & 0b100000) >> 5);
        WriteBitDev2Host((byte_temp & 0b1000000) >> 6);
        WriteBitDev2Host((byte_temp & 0b10000000) >> 7);

        /*PARITY BIT, step8*/
        WriteBitDev2Host(GetOddParity(byte_temp));

        /*STOP BIT, step9*/
        WriteBitDev2Host(HIGH);

        delay(30); //step10
    }
}

void SendBytesDev2Host(char *p_byte_temp, int bytes_length)
{
    for (int i = 0; i < bytes_length; i++)
    {
        SendByteDev2Host(*p_byte_temp);
        p_byte_temp += 1;
    }
}

int CheckHostHasMessage(char *host_temp)
{
    if (digitalRead(ClockLine) == HIGH)
    {
        return 0;
    }
    else
    {
        WaitHigh(ClockLine); //step1

        if (digitalRead(DataLine) == LOW) //step2
        {
            pinMode(ClockLine, GPIO_CLOCK);

            HostBuffer = 0;

            /*DATA BITS, step3*/
            HostBuffer = ReadBitHost2Dev();
            HostBuffer += ReadBitHost2Dev() << 1;
            HostBuffer += ReadBitHost2Dev() << 2;
            HostBuffer += ReadBitHost2Dev() << 3;
            HostBuffer += ReadBitHost2Dev() << 4;
            HostBuffer += ReadBitHost2Dev() << 5;
            HostBuffer += ReadBitHost2Dev() << 6;
            HostBuffer += ReadBitHost2Dev() << 7;

            /*PARITY BIT, step4*/
            char parity_temp = ReadBitHost2Dev();

            //TEST CODE
            printf("h->d:0x%x", HostBuffer);
            *host_temp = HostBuffer;

            /*STOP BIT, step5*/
            if (ReadBitHost2Dev() == LOW)
            {
                return 1; /*Send Abort*/
            }
            else
            {
                if (digitalRead(DataLine) == LOW) //step6
                {
                    while (digitalRead(DataLine) != HIGH)
                    {
                        digitalWrite(ClockLine, digitalRead(ClockLine));
                    }
                    return 1;
                }
                else
                {
                    /*Output Acknowledge bit, step7*/
                    WriteBitDev2Host(LOW);
                    WriteBitDev2Host(HIGH);

                    /*Check parity bit, step8*/
                    if (parity_temp != GetOddParity(HostBuffer))
                    {
                        return 1;
                    }
                    else
                    {
                        delayMicroseconds(45); //step9
                        return 0;
                    }
                }
            }
        }
        else
            return 1;
    }
}
