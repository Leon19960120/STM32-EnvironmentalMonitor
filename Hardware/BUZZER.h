#ifndef _BUZZER_H
#define _BUZZER_H

void Buzzer_Init(void);
void Buzzer_ON(void);
void Buzzer_OFF(void);
void Buzzer_Turn(void);

// 新增功能
void Buzzer_StartBeep(void);
void Buzzer_DataBeep(void);
void Buzzer_AlarmStart(void);
void Buzzer_AlarmStop(void);
void Buzzer_CheckThreshold(int16_t Temp, int16_t humidity);

#endif
