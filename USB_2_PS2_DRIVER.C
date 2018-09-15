#include <wiringPi.h>

main(void)
{
    wiringPiSetup();

    /*board.16,18->GPIO.4,5-> WIRINGPI.4,5*/
}

void pinInit(void)
{
    pinMode(4, INPUT); /*DATA*/
    pinMode(5, INPUT); /*CLOCK*/
}

void SendByte2Host(char byte_temp)
{
    while (digitalRead(5) == HIGH)
    {
        delayMicroseconds(50);
        if (digitalRead(5) == HIGH)
        {
            if (digitalRead(4) == HIGH)
            {
                break;
            }
            else
            {
                delay(20);
                pinMode(4, OUTPUT);

                /*START BIT*/
                digitalWrite(4, LOW);
                /*DATA BITS*/
                digitalWrite(4, byte_temp & 1);
                digitalWrite(4, (byte_temp & 0b10) >> 1);
                digitalWrite(4, (byte_temp & 0b100) >> 2);
                digitalWrite(4, (byte_temp & 0b1000) >> 3);
                digitalWrite(4, (byte_temp & 0b10000) >> 4);
                digitalWrite(4, (byte_temp & 0b100000) >> 5);
                digitalWrite(4, (byte_temp & 0b1000000) >> 6);
                digitalWrite(4, (byte_temp & 0b10000000) >> 7);
                /*PARITY BIT*/

                /*STOP BIT*/
                digitalWrite(4, HIGH);

                delay(30);
            }
        }
        else
            break;
    };
}
