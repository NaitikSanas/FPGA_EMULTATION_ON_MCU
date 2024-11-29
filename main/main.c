#include <stdio.h>
#include "stdint.h"
#include <string.h>
#include <stdlib.h>
#include "uCanvas_api.h"
#include "uCanvas_User_IO.h"
#include "fpga_emulation.h"

static uint8_t XOR_LUT [4] = {
      0,1,1,0
};

static uint8_t OR_LUT [4] = {
   0,1,1,1
};

static uint8_t AND_LUT [4] = {
   0,0,0,1
};


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
	instantiate_lut(&LUT1, XOR_LUT,0,0,4,&a,&b,NULL,NULL,&sumAB);
	instantiate_lut(&LUT3, XOR_LUT,0,0,4,&sumAB,&carry_in,NULL,NULL,&sumABC);
	instantiate_lut(&LUT4, AND_LUT,0,0,4,&a,&b,NULL,NULL,&carry_f1); 
	instantiate_lut(&LUT2, AND_LUT,0,0,4,&sumAB,&carry_in,NULL,NULL,&carry_f0);
	instantiate_lut(&LUT5, OR_LUT, 0,0,4,&carry_f0,&carry_f1,NULL,NULL,&carry_out);

	printf("init done\r\n");
	char buf [32];
	uint8_t a_prev=0, b_prev=0;
	while (1)
	{
		/* take Input from user switch */
		if(!uCanvas_Get_PushbuttonState(48))a = 1; else a = 0;
		if(!uCanvas_Get_PushbuttonState(47))b = 1; else b = 0;		
		if(a_prev != a || b_prev != b){ /* display result of OLED */
			/* Update All LUTs to process the Input */
			update_lut_state(&LUT1);
			update_lut_state(&LUT3);
			update_lut_state(&LUT4);
			update_lut_state(&LUT2);
			update_lut_state(&LUT5);	

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
		uCanvas_Delay(1);
	}
   	return 0;
}