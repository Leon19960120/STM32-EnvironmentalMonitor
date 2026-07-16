#ifndef __ESP8266_H
#define __ESP8266_H

void ESP8266_Init(void);
void ESP8266_Clear(void);

extern unsigned char esp8266_buf[512];
extern unsigned short esp8266_cnt;

_Bool ESP8266_WaitRev(void);

_Bool ESP8266_SendCmd(const char *cmd, char* res);

_Bool ESP8266_SendData(unsigned char *data, unsigned short len);

unsigned char*  ESP8266_GetIPD(unsigned short timeout);
#endif
