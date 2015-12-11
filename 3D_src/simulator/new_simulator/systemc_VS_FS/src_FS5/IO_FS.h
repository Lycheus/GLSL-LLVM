#include <iostream>
#include <systemc.h>
#include "handshake.h"
#include "../testbench.h"
#include "FS_ARGU.h"

#ifndef IO_FS_H
#define IO_FS_H
/*
#define SLAVE_LENGTH  1080 //[IM] + [CMEM] + [VERTEX_FIFO] + [control registers]
#define IM_LENGTH     256  
#define GPR_LENGTH    64   
#define CMEM_LENGTH   512  
#define V_FIFO_LENGTH 12   //3 vertex
#define TEX_LENGTH    4096
#define TEX_WIDTH     64
#define LCD_LENGTH    76800//320*240
#define LCD_WIDTH     320
#define LCD_HEIGHT    240

#define IM_BASE          0
#define CMEM_BASE        512 
#define VERTEX_FIFO_BASE 1024
#define CONTROL_REG_BASE 1072 
*/

SC_MODULE(IO_FS)
{
 public:
     	sc_event	  next_obj;
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
	    framebuff.open("./output/framebuff.txt");

#ifdef trig
	    fp_Instruction = fopen("./FS_input/test_VS/txt_bin" , "r");
	    char c[150];
	    int i;
	    for(i=0;i<OBJ_NUM;i++){
	    	sprintf(c,"./FS_input/test_VS/Uniform.txt");
	   	fp_Uniform[i]  = fopen(c , "r");
	    }
#endif
#ifdef red_cube6
	    fp_Instruction = fopen("./FS_input/red_cube6/txt_bin" , "r");
	    char c[150];
	    int i;
	    for(i=0;i<OBJ_NUM;i++){
	    	sprintf(c,"./FS_input/red_cube6/Uniform%d.txt",i+1);
	   	fp_Uniform[i]  = fopen(c , "r");
	    }
#endif
#ifdef red_cube	    
	    fp_Instruction = fopen("./FS_input/red_cube/txt_bin" , "r");
	    fp_Uniform[0]     = fopen("./FS_input/red_cube/Uniform.txt" , "r");
#endif
#ifdef red_cube_transpose
	    fp_Instruction = fopen("./FS_input/red_cube_transpose/txt_bin" , "r");
	    fp_Uniform[0]     = fopen("./FS_input/red_cube_transpose/Uniform.txt" , "r");
#endif
#ifdef texture_cube
    	    pfile_pic=fopen("./FS_input/texture_cube/crate128.bmp","r");
	    if(!pfile_pic){
		cout << "IO_FS: open bmp error\n";
		sc_stop();
	    }
	    fp_Instruction = fopen("./FS_input/texture_cube/txt_bin" , "r");
	    fp_Uniform[0]     = fopen("./FS_input/texture_cube/Uniform.txt" , "r");
#endif
#ifdef lighting_cube
	    fp_Instruction = fopen("./FS_input/lighting_cube/txt_bin" , "r");
	    fp_Uniform[0]     = fopen("./FS_input/lighting_cube/Uniform.txt" , "r");
#endif

#ifdef lighting_cube_for_migration
	    fp_Instruction = fopen("./FS_input/lighting_cube_for_migration/txt_bin" , "r");
	    fp_Uniform[0]     = fopen("./FS_input/lighting_cube_for_migration/Uniform.txt" , "r");
#endif

#ifdef lighting_cube_new
	    fp_Instruction = fopen("./FS_input/lighting_cube_new/txt_bin" , "r");
	    fp_Uniform[0]     = fopen("./FS_input/lighting_cube_new/Uniform.txt" , "r");
#endif
	    
#ifdef morphing_ball
	    //pfile_pic=fopen("./FS_input/morphing_ball/crate64.bmp","r");
//fixed by sheng-chih
	    //pfile_pic=fopen("./FS_input/morphing_ball/Fieldstone.bmp","r");
	    //pfile_pic=fopen("./FS_input/morphing_ball/Wall.bmp","r");
	    pfile_pic=fopen("./FS_input/morphing_ball/Fieldstone128128_flip_v.bmp","r");
//end fixed
            if(!pfile_pic){
	      cout << "IO_FS: open bmp error\n";
	      sc_stop();
            }
	    
	    fp_Instruction = fopen("./FS_input/morphing_ball/txt_bin" , "r");

	    char c[150];
	    int i;
            for(i=0;i<OBJ_NUM;i++){
	      //printf(c,"./FS_input/morphing_ball/Uniform%d.txt",i+1);
	      sprintf(c,"./FS_input/morphing_ball/Uniform%d.txt",2);
	      fp_Uniform[i]  = fopen(c , "r");
	    }
	    
	    //fp_Uniform[0] = fopen("./FS_input/morphing_ball/Uniform1.txt" , "r");
#endif

#ifdef morphing_ball_for_migration
	    //pfile_pic=fopen("./FS_input/morphing_ball/crate64.bmp","r");
//fixed by sheng-chih
	    pfile_pic=fopen("./FS_input/morphing_ball_for_migration/Fieldstone.bmp","r");
	    //pfile_pic=fopen("./FS_input/morphing_ball/Wall.bmp","r");
	    //pfile_pic=fopen("./FS_input/morphing_ball/Fieldstone128128_flip_v.bmp","r");
//end fixed
            if(!pfile_pic){
	      cout << "IO_FS: open bmp error\n";
	      sc_stop();
            }
	    
	    fp_Instruction = fopen("./FS_input/morphing_ball_for_migration/txt_bin" , "r");

	    char c[150];
	    int i;
            for(i=0;i<OBJ_NUM;i++){
	      //printf(c,"./FS_input/morphing_ball/Uniform%d.txt",i+1);
	      sprintf(c,"./FS_input/morphing_ball_for_migration/Uniform%d.txt",2);
	      fp_Uniform[i]  = fopen(c , "r");
	    }
	    
	    //fp_Uniform[0] = fopen("./FS_input/morphing_ball/Uniform1.txt" , "r");
#endif

#ifdef ocean
    	    pfile_pic=fopen("./FS_input/ocean/texture.bmp","r");
	    if(!pfile_pic){
		cout << "IO_FS: open bmp error\n";
		sc_stop();
	    }
	    fp_Instruction = fopen("./FS_input/ocean/txt_bin" , "r");

	    char c[150];
	    int i;
            for(i=0;i<OBJ_NUM;i++){
	      sprintf(c,"./FS_input/ocean/Uniform%d.txt",i+1);
	      fp_Uniform[i]  = fopen(c , "r");
	    }

#endif

#ifdef test_VS	    
	    fp_Instruction = fopen("./FS_input/test_VS/txt_bin" , "r");
	    fp_Uniform[0]     = fopen("./FS_input/test_VS/Uniform.txt" , "r");
	    //char c[150];
	    //int i;
            //for(i=0;i<OBJ_NUM;i++){
	    //sprintf(c,"./FS_input/test_VS/Uniform%d.txt",i+1);
	    //fp_Uniform[i]  = fopen(c , "r");
	    //}

#endif

	    
	    if(!fp_Instruction || !fp_Uniform[0] || !framebuff){
	      cout<<"============================"<<endl;
	      cout<<"===IO_FS: open file error==="<<endl;
	      cout<<"============================"<<endl;
	      sc_stop();
	    }
	}
	~IO_FS()
	{
	   framebuff.close();
	   fclose( fp_Instruction );
	   int i;
	   for(i=0;i<OBJ_NUM;i++){
	   	fclose( fp_Uniform[i] );
	   }
	}
	
	unsigned int databuff[16];
	
 private: 
    	FILE *pfile_pic;
	FILE *fp_Instruction, *fp_Uniform[OBJ_NUM];  
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
