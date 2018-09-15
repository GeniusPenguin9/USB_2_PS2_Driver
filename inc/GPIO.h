#ifndef _GPIO_H
#define _GPIO_H

void SendByteDev2Host(char byte_temp);
void SendBytesDev2Host(char *byte_temp, int bytes_length);
bool CheckHostHasMessage();
char ReadByteHost2Dev();

typedef char bool;

#endif