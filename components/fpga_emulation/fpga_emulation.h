#include <stdio.h>
#include "stdint.h"
#include <string.h>
#include <stdlib.h>
#include "stdint.h" 

typedef uint8_t signal_t;

typedef struct LUT
{
   uint8_t* values;
   uint8_t hold_state;
   uint8_t reset;
   uint8_t set;
   uint8_t output;
   signal_t* inputs[4];
   signal_t* outputs;
}LUT_t;

void instantiate_lut(LUT_t* LUT_Obj, uint8_t* LUT_Content,uint8_t hold_state,uint8_t set,uint16_t lut_size, signal_t* i0, signal_t* i1,signal_t* i2,signal_t* i3, signal_t* output);
void update_lut_state(LUT_t* LUT_Obj);
uint8_t concat_1bit(uint8_t a,uint8_t b, uint8_t position);
