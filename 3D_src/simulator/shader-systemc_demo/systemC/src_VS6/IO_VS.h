#include <iostream>
#include <systemc.h>
#include "handshake.h"

#ifndef IO_VS_H
#define IO_VS_H

#define IM_offset   0
#define CMEM_offset 1000
#define VST_offset  2000
#define VAB_offset  3000
#define en_offset   4000

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
	    
	    //fp_Instruction = fopen("./file/Transformation.txt" , "r");
	    fp_Instruction = fopen("./file/txt_bin" , "r");
	    fp_Uniform     = fopen("./file/Uniform.txt" , "r");
	    fp_VST         = fopen("./file/VST.txt" , "r");
	    fp_Vertex      = fopen("./file/Vertex_Data.txt" , "r");
	    
	    if(!fp_Instruction || !fp_Uniform || !fp_VST || !fp_Vertex){
	      cout<<"============================"<<endl;
	      cout<<"===SC_VS: open file error==="<<endl;
	      cout<<"============================"<<endl;
	      sc_stop();
	    }
	    
	    total_v = 48;//=== total vertex number ===
	    total_a = 1;//attribute number
	}
	~IO_VS()
	{
	   fclose( fp_Instruction );
	   fclose( fp_Vertex );
	   fclose( fp_Uniform );
	   fclose( fp_VST );
	}
	
	unsigned int databuff[1024];
	
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
	FILE *fp_Instruction, *fp_Uniform, *fp_VST, *fp_Vertex ;  
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
