#include "fpga_emulation.h"
#define DEBUG
void instantiate_lut(LUT_t* LUT_Obj, uint8_t* LUT_Content,uint8_t hold_state,uint8_t set,uint16_t lut_size, signal_t* i0, signal_t* i1,signal_t* i2,signal_t* i3, signal_t* output){
   LUT_Obj->values = malloc(lut_size);
   LUT_Obj->inputs[0] = i0;
   LUT_Obj->inputs[1] = i1;
   LUT_Obj->inputs[2] = i2;
   LUT_Obj->inputs[3] = i3;
   LUT_Obj->outputs = output;
   memcpy(LUT_Obj->values,LUT_Content,4);
   LUT_Obj->hold_state = hold_state;
   LUT_Obj->set = set;
   if(set){
      LUT_Obj->output = 1;
   }
}

uint8_t concat_1bit(uint8_t a,uint8_t b, uint8_t position){
   return (a | b << position);
}

void update_lut_state(LUT_t* LUT_Obj){
	signal_t LUT_Address_Vector = 0;
	for(int i = 0; i < 4; i++){
		if( LUT_Obj->inputs[i] != NULL){
			#ifdef FDEBUG
            printf("%d",*LUT_Obj->inputs[i]);
			#endif
            LUT_Address_Vector |= *LUT_Obj->inputs[i] << i;
		}
		#ifdef FDEBUG
        printf("\n");
        #endif
	}
	
   
	*LUT_Obj->outputs = LUT_Obj->values[LUT_Address_Vector];
    #ifdef FDEBUG
    printf("LAV %d\r\n",LUT_Address_Vector);
	printf("LUT STATE %d \r\n", *LUT_Obj->outputs);
    #endif
}