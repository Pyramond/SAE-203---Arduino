#ifndef MUSIQUE_H
#define MUSIQUE_H

#include <cstdint>

#define SPEAKER 19

//#define I2C2_SDA2 19
//#define I2C2_SCL2 18


void pinInit();
void sound(uint8_t note_index);

#endif // MUSIQUE_H