#ifndef _GPIO_H
#define _GPIO_H

extern char HostBuffer;;

char GetOddParity(char byte_temp);
void pinInit(void);
void SendByteDev2Host(char byte_temp);
void SendBytesDev2Host(char *p_byte_temp, int bytes_length);
int CheckHostHasMessage();



#endif