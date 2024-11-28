#include <stdio.h>
#include "stdint.h"
#include <string.h>
#include <stdlib.h>
#include "uCanvas_api.h"
#include "uCanvas_User_IO.h"
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

static uint8_t XOR_LUT [4] = {
      0,1,1,0
   };

static uint8_t OR_LUT [4] = {
   0,1,1,1
};

static uint8_t AND_LUT [4] = {
   0,0,0,1
};

void map_lut(LUT_t* LUT_Obj, uint8_t* LUT_Content,uint8_t hold_state,uint8_t set,uint16_t lut_size, signal_t* i0, signal_t* i1,signal_t* i2,signal_t* i3, signal_t* output){
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

uint8_t lut_process(LUT_t* LUT_Obj, uint8_t data){
   if(LUT_Obj->hold_state)LUT_Obj->output = LUT_Obj->values[data]; 
   else  LUT_Obj->output  = 0;
//   printf("LUT STATE %d\r\n", LUT_Obj->output );
   return LUT_Obj->values[data];
}

uint8_t concat(uint8_t a,uint8_t b){
   return (a | b << 1);
}
// void execute_full_adder_process(uint8_t a, uint8_t b, uint8_t c_in,uint8_t* sum_out, uint8_t* carry_out){
// 	uint8_t x_in = a | b<<1;
// 	printf("xin %d\r\n",x_in);

// 	uint8_t tmp = lut_process(&LUT1,x_in);
// 	tmp = tmp | c_in<<1;
// 	*sum_out = lut_process(&LUT3,tmp);

// 	uint8_t c1_in = tmp | c_in<<1; 
// 	uint8_t c2_in = a | b<<1; 
// 	uint8_t c1_out = lut_process(&LUT2,c1_in);
// 	uint8_t c2_out = lut_process(&LUT4,c2_in);

// 	uint8_t cout = c1_out | c2_out<<1;
// 	*carry_out = lut_process(&LUT5,cout);

// 	printf("ab = %d%d\r\n",b,a);
// 	printf("sum %d carry %d\r\n",*sum_out, *carry_out);
// }

void process_lut(LUT_t* LUT_Obj){
	signal_t LUT_Address_Vector = 0;// = *LUT_Obj->inputs[3] <<3 | *LUT_Obj->inputs[2] << 3 | *LUT_Obj->inputs[1] << 1 | *LUT_Obj->inputs[0];
	for(int i = 0; i < 4; i++){
		if( LUT_Obj->inputs[i] != NULL){
			// printf("%d",*LUT_Obj->inputs[i]);
			LUT_Address_Vector |= *LUT_Obj->inputs[i] << i;
		}
		// printf("\n");
	}
	// printf("LAV %d\r\n",LUT_Address_Vector);
	*LUT_Obj->outputs = LUT_Obj->values[LUT_Address_Vector];
	// printf("LUT STATE %d \r\n", *LUT_Obj->outputs);
}

int app_main() {
	LUT_t LUT1,LUT2,LUT3,LUT4,LUT5,LUT6;

	uCanvas_Scene_t* scene1;
	
	uCanvas_obj_t* rect;
	uCanvas_obj_t* textbox;
	uCanvas_obj_t* textbox2;
	start_uCanvas_engine();
	scene1 = New_uCanvas_Scene();
	uCanvas_set_active_scene(scene1);

	textbox = New_uCanvas_2DTextbox("FPGA",10,32);
	textbox2 = New_uCanvas_2DTextbox("FPGA",10,10);

	uCanvas_Init_PushButton(48);
	uCanvas_Init_PushButton(47);
	//define signals with initial value
	signal_t a = 0, b = 0, sumAB = 0, sumABC = 0,carry_in = 0, carry_f0 = 0, carry_f1 = 0, carry_out = 0;
	
	//Define your design by instantiating LUT and Mapping the signal variables.
	map_lut(&LUT1, XOR_LUT,0,0,4,&a,&b,NULL,NULL,&sumAB);
	map_lut(&LUT3, XOR_LUT,0,0,4,&sumAB,&carry_in,NULL,NULL,&sumABC);
	map_lut(&LUT4, AND_LUT,0,0,4,&a,&b,NULL,NULL,&carry_f1); 
	map_lut(&LUT2, AND_LUT,0,0,4,&sumAB,&carry_in,NULL,NULL,&carry_f0);
	map_lut(&LUT5, OR_LUT, 0,0,4,&carry_f0,&carry_f1,NULL,NULL,&carry_out);

	printf("init done\r\n");
	char buf [32];
	uint8_t a_prev=0, b_prev=0;
	while (1)
	{
		/* take Input from user switch */
		if(!uCanvas_Get_PushbuttonState(48))a = 1; else a = 0;
		if(!uCanvas_Get_PushbuttonState(47))b = 1; else b = 0;

		/* Update All LUTs to process the Input */
		process_lut(&LUT1);
		process_lut(&LUT3);
		process_lut(&LUT4);
		process_lut(&LUT2);
		process_lut(&LUT5);	
		
		if(a_prev != a || b_prev != b){ /* display result of OLED */
			printf("ab %d%d\r\n",a,b);
			sprintf(buf,"sum - %d carry %d",sumABC,carry_out);
			uCanvas_Set_Text(textbox,buf);
			sprintf(buf,"a = %d b = %d",a,b);
			uCanvas_Set_Text(textbox2,buf);
			printf("carry_f1 - %d  | carry_f0  - %d \r\n",carry_f1,carry_f0);
			printf("sumABC   - %d  | carry_out - %d \r\n",sumABC,carry_out);
		}
		a_prev = a;
		b_prev = b;
		uCanvas_Delay(6);
	}
	
	
   	return 0;
}