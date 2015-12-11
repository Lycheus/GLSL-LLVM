#include <iostream>
#include <systemc.h>
#include "handshake.h"
#include "../testbench.h"

#ifndef IO_VS_H
#define IO_VS_H

#define IM_offset   0
//#define CMEM_offset 5000
//#define VST_offset  7000
//#define VAB_offset  8000
#define CMEM_offset 9000
#define VST_offset  11000
#define VAB_offset  12000
#define en_offset   43000


//SC_MODULE(IO_VS)
class IO_VS : public sc_module
{
 public:
	//initiator port
	sc_fifo_out<Req>  mp_transfer;
	sc_fifo_in<bool>  mp_response;
	//target port
	sc_fifo_in<Req>   sp_transfer;
	sc_fifo_out<bool> sp_response;
	
	//SC_CTOR(IO_VS)
	SC_HAS_PROCESS(IO_VS);
	IO_VS(sc_module_name name):sc_module(name)
	{
	    SC_THREAD(master_port);
	    SC_THREAD(slave_port);
#ifdef trig
	    fp_Instruction = fopen("./VS_input/test_VS/txt_bin" , "r");
	    fp_VST         = fopen("./VS_input/test_VS/VST.txt" , "r");
	    char c[150];
	    int i;
	    for(i=0;i<OBJ_NUM;i++){
	    	sprintf(c,"./VS_input/test_VS/Vertex_Data.txt");
	    	fp_Vertex[i]   = fopen(c , "r");
	    	sprintf(c,"./VS_input/test_VS/Uniform.txt");
	   	fp_Uniform[i]  = fopen(c , "r");
	    }
	    total_v = 54/OBJ_NUM;//=== total vertex number ===
	    total_a = 2;//attribute number
#endif
#ifdef red_cube6
	    fp_Instruction = fopen("./VS_input/red_cube6/txt_bin" , "r");
	    fp_VST         = fopen("./VS_input/red_cube6/VST.txt" , "r");
	    char c[150];
	    int i;
	    for(i=0;i<OBJ_NUM;i++){
	    	sprintf(c,"./VS_input/red_cube6/Vertex_Data%d.txt",i+1);
	    	fp_Vertex[i]   = fopen(c , "r");
	    	sprintf(c,"./VS_input/red_cube6/Uniform%d.txt",i+1);
	   	fp_Uniform[i]  = fopen(c , "r");
	    }
	    total_v = 36/OBJ_NUM;//=== total vertex number ===
	    total_a = 1;//attribute number
#endif
#ifdef red_cube	    
	    fp_Instruction = fopen("./VS_input/red_cube/txt_bin" , "r");
	    fp_Uniform[0]  = fopen("./VS_input/red_cube/Uniform.txt" , "r");
	    fp_VST         = fopen("./VS_input/red_cube/VST.txt" , "r");
	    fp_Vertex[0]   = fopen("./VS_input/red_cube/Vertex_Data.txt" , "r");
	    total_v = 36;//=== total vertex number ===
	    total_a = 1;//attribute number
#endif
#ifdef red_cube_transpose
	    fp_Instruction = fopen("./VS_input/red_cube_transpose/txt_bin" , "r");
	    fp_Uniform[0]  = fopen("./VS_input/red_cube_transpose/Uniform.txt" , "r");
	    fp_VST         = fopen("./VS_input/red_cube_transpose/VST.txt" , "r");
	    fp_Vertex[0]   = fopen("./VS_input/red_cube_transpose/Vertex_Data.txt" , "r");
	    total_v = 6;//=== total vertex number ===
	    total_a = 1;//attribute number
#endif
#ifdef texture_cube
	    fp_Instruction = fopen("./VS_input/texture_cube/txt_bin" , "r");
	    fp_Uniform[0]  = fopen("./VS_input/texture_cube/Uniform.txt" , "r");
	    fp_VST         = fopen("./VS_input/texture_cube/VST.txt" , "r");
	    fp_Vertex[0]   = fopen("./VS_input/texture_cube/Vertex_Data.txt" , "r");
	    total_v = 36;//=== total vertex number ===
	    total_a = 3;//attribute number
#endif
#ifdef lighting_cube
	    fp_Instruction = fopen("./VS_input/lighting_cube/txt_bin" , "r");
	    fp_Uniform[0]  = fopen("./VS_input/lighting_cube/Uniform.txt" , "r");
	    fp_VST         = fopen("./VS_input/lighting_cube/VST.txt" , "r");
	    fp_Vertex[0]   = fopen("./VS_input/lighting_cube/Vertex_Data.txt" , "r");
	    total_v = 36;//=== total vertex number ===
	    total_a = 3;//attribute number
#endif

#ifdef lighting_cube_for_migration
	    fp_Instruction = fopen("./VS_input/lighting_cube_for_migration/txt_bin" , "r");
	    fp_Uniform[0]  = fopen("./VS_input/lighting_cube_for_migration/Uniform.txt" , "r");
	    fp_VST         = fopen("./VS_input/lighting_cube_for_migration/VST.txt" , "r");
	    fp_Vertex[0]   = fopen("./VS_input/lighting_cube_for_migration/Vertex_Data.txt" , "r");
	    total_v = 36;//=== total vertex number ===
	    total_a = 3;//attribute number
#endif

#ifdef lighting_cube_new
	    fp_Instruction = fopen("./VS_input/lighting_cube_new/txt_bin" , "r");
	    fp_Uniform[0]  = fopen("./VS_input/lighting_cube_new/Uniform.txt" , "r");
	    fp_VST         = fopen("./VS_input/lighting_cube_new/VST.txt" , "r");
	    fp_Vertex[0]   = fopen("./VS_input/lighting_cube_new/Vertex_Data.txt" , "r");
	    total_v = 36;//=== total vertex number ===
	    total_a = 3;//attribute number
#endif

#ifdef morphing_ball
            fp_Instruction = fopen("./VS_input/morphing_ball/txt_bin" , "r");
            fp_VST         = fopen("./VS_input/morphing_ball/VST.txt" , "r");
            char c[150];
            int i;
            for(i=0;i<OBJ_NUM;i++){
	      //sprintf(c,"./VS_input/morphing_ball/Vertex_Data%d.txt",i+1);
	      sprintf(c,"./VS_input/morphing_ball/Vertex_Data%d.txt",2);
	      fp_Vertex[i]   = fopen(c , "r");
	      //sprintf(c,"./VS_input/morphing_ball/Uniform%d.txt",i+1);
	      sprintf(c,"./VS_input/morphing_ball/Uniform%d.txt",2);
	      fp_Uniform[i]  = fopen(c , "r");
            }
            total_v = 2500;//=== total vertex number ===                                                        
            total_a = 3;//attribute number                                                                            
#endif

#ifdef ocean
            fp_Instruction = fopen("./VS_input/ocean/txt_bin" , "r");
            fp_VST         = fopen("./VS_input/ocean/VST.txt" , "r");
            char c[150];
            int i;
            for(i=0;i<OBJ_NUM;i++){
	      sprintf(c,"./VS_input/ocean/Vertex_Data%d.txt",2);
	      fp_Vertex[i]   = fopen(c , "r");
	      sprintf(c,"./VS_input/ocean/Uniform%d.txt",2);
	      fp_Uniform[i]  = fopen(c , "r");
            }
	    printf("%p\n",fp_Instruction);
	    printf("%p\n",fp_VST);
	    printf("%p\n",fp_Vertex[0]);
	    printf("%p\n",fp_Uniform[0]);

            total_v = 2100;//=== total vertex number ===                                                        
            total_a = 3;//attribute number                                                                            
#endif

#ifdef test_VS	    
	    fp_Instruction = fopen("./VS_input/test_VS/txt_bin" , "r");
	    fp_Uniform[0]  = fopen("./VS_input/test_VS/Uniform.txt" , "r");
	    fp_VST         = fopen("./VS_input/test_VS/VST.txt" , "r");
	    fp_Vertex[0]   = fopen("./VS_input/test_VS/Vertex_Data.txt" , "r");
	    total_v = 6;//=== total vertex number ===
	    total_a = 2;//attribute number
#endif
	    
	    if(!fp_Instruction || !fp_Uniform[0] || !fp_VST || !fp_Vertex[0]){
	      cout<<"============================"<<endl;
	      cout<<"===IO_VS: open file error==="<<endl;
	      cout<<"============================"<<endl;
	      sc_stop();
	    }
	    
	}
	~IO_VS()
	{
	   fclose( fp_Instruction );
	   fclose( fp_VST );
	   int i;
	   for(i=0;i<OBJ_NUM;i++){
	   	fclose( fp_Vertex[i] );
	   	fclose( fp_Uniform[i] );
	   }
	}
	//fixed by sheng-chih
	unsigned int databuff[409600];
	//unsigned int databuff[1024];
	//end fixed
 private: 	  
	void master_port();
	void slave_port();
		
	char  IM[65]; 
	float CMEM;  
	int   VST;  
	float Ex_MEM;
	
	int i,j;  
	int im_count;
	int uniform_count;  
	int vst_count;
	int total_v;
	int total_a;
	int v_num;       //vertex number 
	int v_entry_num; //entry number
	int v_word_num;  //offset number
	unsigned int la,ha;
	volatile unsigned int *addr;   

	//fixed by sheng-chih
	FILE *fp_Instruction, *fp_Uniform[OBJ_NUM], *fp_VST, *fp_Vertex[OBJ_NUM] ;  
	//FILE *fp_Instruction, *fp_VST, *fp_Vertex ;  
	//end fixed
	
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
	//int trf_offset[10];
        //int trf_burst[10];
};

#endif
