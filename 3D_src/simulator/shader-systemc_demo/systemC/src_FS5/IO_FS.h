#include <iostream>
#include <systemc.h>
#include "handshake.h"

#ifndef IO_FS_H
#define IO_FS_H

#define SLAVE_LENGTH  1080 //[IM] + [CMEM] + [VERTEX_FIFO] + [control registers]
#define IM_LENGTH     256  
#define GPR_LENGTH    64   
#define CMEM_LENGTH   512  
#define V_FIFO_LENGTH 12   //3 vertex
#define TEX_LENGTH    102400//320*320 //65536 //64*64
#define TEX_WIDTH     320
#define LCD_LENGTH    102400//320*320   320*240
#define LCD_WIDTH     320
#define LCD_HEIGHT    320

#define IM_BASE          0
#define CMEM_BASE        512 
#define VERTEX_FIFO_BASE 1024
#define CONTROL_REG_BASE 1072 

SC_MODULE(IO_FS)
{
 public:
	//initiator port
	sc_fifo_out<Req>  mp_transfer;
	sc_fifo_in<bool>  mp_response;
	//target port
	sc_fifo_in<Req>   sp_transfer;
	sc_fifo_out<bool> sp_response;
	
	SC_CTOR(IO_FS)
	{
	    SC_THREAD(master_port);
	    SC_THREAD(slave_port);
	    framebuff.open("./file/framebuff.txt");
	}
	~IO_FS()
	{framebuff.close();}
	
	unsigned int databuff[16];
	
 private: 
        ofstream framebuff;	  
	void master_port();
	void slave_port();
	
	int i;
	//record transaction information:addr, read/write, "data addr", burst size
	//for initiator port
	Req info_mp;
	//for target port
	Req info_sp;
	unsigned int *DataAddr;
	unsigned int Addr;
	bool Type;
	int Burst;
	
	//SystemC variable
	sc_bv<32> bv_data;
	unsigned int SLAVE_MEM[SLAVE_LENGTH];
	float VAB[48];
	int trf_offset[10];
        int trf_burst[10];
};

#endif
