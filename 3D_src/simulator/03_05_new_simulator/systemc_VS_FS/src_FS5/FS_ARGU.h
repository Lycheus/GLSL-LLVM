#ifndef __FS_ARGU_H__
#define __FS_ARGU_H__

//SIZE
 //#define SLAVE_LENGTH  1074 //[IM] + [CMEM] + [VERTEX_FIFO] + [control registers]                                                                  
//#define SLAVE_LENGTH  1080 //CYLin: add start FS register
#define SLAVE_LENGTH  1128 //CYLin: add start FS register
#define IM_LENGTH     256
#define GPR_LENGTH    64
#define CMEM_LENGTH   512
//#define V_FIFO_LENGTH 12   //3 vertex
#define V_FIFO_LENGTH 24   //3 vertex
#define TEX_LENGTH    4096
#define TEX_WIDTH     64
#define LCD_LENGTH    76800//320*240
#define LCD_WIDTH     320
#define LCD_HEIGHT    240

#define IM_BASE          0
#define CMEM_BASE        512
#define VERTEX_FIFO_BASE 1024
//#define VERTEX_FIFO_BASE 1024
//#define CONTROL_REG_BASE 1072
#define CONTROL_REG_BASE 1120

//#define VARYING_NUM 5


#endif
