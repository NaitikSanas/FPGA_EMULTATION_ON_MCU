// #include <stdio.h>
// #include "stdint.h"
// #include <string.h>
// #include <stdlib.h>
// typedef uint8_t signal_t;
// typedef struct LUT
// {
//    uint8_t* values;
//    uint8_t hold_state;
//    uint8_t reset;
//    uint8_t set;
//    uint8_t output;
//    signal_t* inputs[4];
//    signal_t* outputs;
// }LUT_t;

// static uint8_t XOR_LUT [4] = {
//       0,1,1,0
//    };

// static uint8_t OR_LUT [4] = {
//    0,1,1,1
// };

// static uint8_t AND_LUT [4] = {
//    0,0,0,1
// };

// void map_lut(LUT_t* LUT_Obj, uint8_t* LUT_Content,uint8_t hold_state,uint8_t set,uint16_t lut_size, signal_t* i0, signal_t* i1,signal_t* i2,signal_t* i3, signal_t* output){
//    LUT_Obj->values = malloc(lut_size);
//    for(int i = 0; i < 4; i++)
//     LUT_Obj->inputs[i] = malloc(1);
//    LUT_Obj->inputs[0] = i0;
//    LUT_Obj->inputs[1] = i1;
//    LUT_Obj->inputs[2] = i2;
//    LUT_Obj->inputs[3] = i3;
//    LUT_Obj->outputs = output;
//    memcpy(LUT_Obj->values,LUT_Content,4);
//    LUT_Obj->hold_state = hold_state;
//    LUT_Obj->set = set;
//    if(set){
//       LUT_Obj->output = 1;
//    }
// }

// uint8_t lut_process(LUT_t* LUT_Obj, uint8_t data){
//    if(LUT_Obj->hold_state)LUT_Obj->output = LUT_Obj->values[data]; 
//    else  LUT_Obj->output  = 0;
// //   printf("LUT STATE %d\r\n", LUT_Obj->output );
//    return LUT_Obj->values[data];
// }
// LUT_t LUT1,LUT2,LUT3,LUT4,LUT5,LUT6;
// uint8_t concat(uint8_t a,uint8_t b){
//    return (a | b << 1);
// }
// void execute_full_adder_process(uint8_t a, uint8_t b, uint8_t c_in,uint8_t* sum_out, uint8_t* carry_out){
//    uint8_t x_in = a | b<<1;
//    printf("xin %d\r\n",x_in);
   
//    uint8_t tmp = lut_process(&LUT1,x_in);
//    tmp = tmp | c_in<<1;
//    *sum_out = lut_process(&LUT3,tmp);
   
//    uint8_t c1_in = tmp | c_in<<1; 
//    uint8_t c2_in = a | b<<1; 
//    uint8_t c1_out = lut_process(&LUT2,c1_in);
//    uint8_t c2_out = lut_process(&LUT4,c2_in);
   
//    uint8_t cout = c1_out | c2_out<<1;
//    *carry_out = lut_process(&LUT5,cout);
   
//    printf("ab = %d%d\r\n",b,a);
//    printf("sum %d carry %d\r\n",*sum_out, *carry_out);
// }

// void process_lut(LUT_t* LUT_Obj){
//    signal_t LUT_Address_Vector = 0;// = *LUT_Obj->inputs[3] <<3 | *LUT_Obj->inputs[2] << 3 | *LUT_Obj->inputs[1] << 1 | *LUT_Obj->inputs[0];
//   for(int i = 0; i < 4; i++){
//       if( LUT_Obj->inputs[i] != NULL){
//         printf("%d",*LUT_Obj->inputs[i]);
//         LUT_Address_Vector |= *LUT_Obj->inputs[i] << i;
//       }
//       printf("\n");
//   }
//   printf("LAV %d\r\n",LUT_Address_Vector);
//    *LUT_Obj->outputs = LUT_Obj->values[LUT_Address_Vector];
//    printf("LUT STATE %d \r\n", *LUT_Obj->outputs);
// }
//  signal_t a = 1, b = 0, sumAB = 0, sumABC = 0,carry_in = 0, carry_f0 = 0, carry_f1 = 0, carry_out = 0;

// typedef struct Full_Adder_Instance
// {
//     LUT_t FAdder[5];
//     signal_t a;
//     signal_t b;
//     signal_t carry_in;

//     signal_t sumAB;
//     signal_t sumABC;
    
//     signal_t carry_f0;
//     signal_t carry_f1;
//     signal_t carry_out;
// }Full_Adder_Instance_t;



// int main() {
    

//    map_lut(&LUT1, XOR_LUT,0,0,4,&a,&b,NULL,NULL,&sumAB);
//    map_lut(&LUT3, XOR_LUT,0,0,4,&sumAB,&carry_in,NULL,NULL,&sumABC);

//    map_lut(&LUT4, AND_LUT,0,0,4,&a,&b,NULL,NULL,&carry_f1); 
//    map_lut(&LUT2, AND_LUT,0,0,4,&sumAB,&carry_in,NULL,NULL,&carry_f0);
//    map_lut(&LUT5, OR_LUT, 0,0,4,&carry_f0,&carry_f1,NULL,NULL,&carry_out);
   
//    printf("init done\r\n");
//    a = 1; b = 1; carry_in = 1;
//    //run to stabilizae signals
//      for(int i = 0; i < 4; i++){
//    process_lut(&LUT1);
//    process_lut(&LUT3);
//    process_lut(&LUT4);
//    process_lut(&LUT2);
//    process_lut(&LUT5);
//      }
//     printf("carry_f1 %d  | carry_f0 %d \r\n",carry_f1,carry_f0);
//    printf("sumABC %d  | carry_out %d \r\n",sumABC,carry_out);

//    // uint8_t a0 = 1, b0 = 0, c0 = 0;
//    // uint8_t a1 = 1, b1 = 1;
//    // uint8_t a2 = 0, b2 = 1;
//    // uint8_t a3 = 0, b3 = 1;
   
//    // uint8_t sum0, carry0;
//    // uint8_t sum1, carry1;
//    // uint8_t sum2, carry2;
//    // uint8_t sum3, carry3;
   
//    // execute_full_adder_process(a0,b0,c0    ,&sum0,&carry0);
//    // execute_full_adder_process(a1,b1,carry0,&sum1,&carry1);
//    // execute_full_adder_process(a2,b2,carry1,&sum2,&carry2);
//    // execute_full_adder_process(a3,b3,carry2,&sum3,&carry3);

//    return 0;
// }