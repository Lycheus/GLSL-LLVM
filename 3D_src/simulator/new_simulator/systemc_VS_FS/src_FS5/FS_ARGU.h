#ifndef __FS_ARGU_H__
#define __FS_ARGU_H__
#define expand_IM_LENGTH //Kaymon add this flag to increase the instruction memory.
//SIZE
 //#define SLAVE_LENGTH  1074 //[IM] + [CMEM] + [VERTEX_FIFO] + [control registers]                                                                  
//#define SLAVE_LENGTH  1080 //CYLin: add start FS register
//#define SLAVE_LENGTH  1128 //CYLin: add start FS register
#ifndef expand_IM_LENGTH
#define SLAVE_LENGTH  2148 //CYLin: add start FS register
#else
#define SLAVE_LENGTH  16392//Kaymon add this memory number.
#endif
//#define IM_LENGTH     256
#define IM_LENGTH     512//=>expand_IM
#ifdef expand_IM_LENGTH
#define IM_LENGTH_EXPAND    4096 //This flag effect the instruction memory length really.
#else
#endif
#define GPR_LENGTH    64
#define CMEM_LENGTH   512
//#define V_FIFO_LENGTH 12   //3 vertex
#define V_FIFO_LENGTH 24   //3 vertex

#define ocean
#ifndef ocean
#define TEX_LENGTH    128*128//65536//524288//65536//16384//4096
#define TEX_WIDTH     128//256//512//256//256//128//64
#else
#define TEX_LENGTH    65536//524288//65536//16384//4096
#define TEX_WIDTH     256//512//256//256//128//64
//#define TEX_LENGTH    1024*1024//524288//65536//16384//4096
//#define TEX_WIDTH     1024//512//256//256//128//64
#endif

#ifndef ocean
#define LCD_LENGTH    320*240
#define LCD_WIDTH     320
#define LCD_HEIGHT    240
#else
#define LCD_LENGTH    320*320
#define LCD_WIDTH     320
#define LCD_HEIGHT    320
#endif

#ifdef expand_IM_LENGTH
#define IM_BASE          2048
#else
#define IM_BASE          0
#endif
//#define CMEM_BASE        512
#define CMEM_BASE        1024
//#define VERTEX_FIFO_BASE 1024
//#define CONTROL_REG_BASE 1072
//#define VERTEX_FIFO_BASE 1024
//#define CONTROL_REG_BASE 1120
#define VERTEX_FIFO_BASE 1536
#define CONTROL_REG_BASE 1632

//#define VARYING_NUM 5


#endif
