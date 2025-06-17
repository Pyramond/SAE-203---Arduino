#include "musique.h"
#include <Arduino.h>

int BassTab[]={1911,1702,1516,1431,1275,1136,1012};//bass 1~7


void pinInit()
{
    pinMode(SPEAKER,OUTPUT);
    digitalWrite(SPEAKER,LOW);
}
void sound(uint8_t note_index)
{
    for(int i=0;i<100;i++)
    {
        digitalWrite(SPEAKER,HIGH);
        delayMicroseconds(BassTab[note_index]);
        digitalWrite(SPEAKER,LOW);
        delayMicroseconds(BassTab[note_index]);
    }
}