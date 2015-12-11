#include "IO_FS.h"
#include "frag_define.h"
#include "../testbench.h"
#include <stdlib.h>

extern sc_event next_obj_FS;

//added by sheng-chih
//extern unsigned long long temp_SLAVE_MEM[512];
//int temp_INS_NUM=0;
//end added

unsigned int TEX_MEM[TEX_LENGTH];

void IO_FS::master_port()
{
    sc_bv<19> zero_19;
    /**** FRAG parameters ****/
    bool      Z_en;
    sc_bv<3>  Z_func;
    //------------------------
    bool         S_en;
    sc_bv<3>  S_func;
    sc_bv<8>  S_ref,S_mask;
    sc_bv<3>  fail,zfail,zpass;
    //------------------------
    bool      B_en;
    sc_bv<4>  B_srcfunc,B_dstfunc;   
 	    
    //cout<<"Setup FS parameter"<<endl;	       
    /****FRAG parameters****/
    //---------------------------
    //<z compare parameters>
    Z_en = 1;
    Z_func = LESS_BITS;
    //<stencil test parameters>
    S_en = 0;
    S_func = LESS_BITS;
    S_ref = "0xff";
    S_mask = "0xff";
    fail = KEEP_BITS;
    zfail = KEEP_BITS;
    zpass = KEEP_BITS;
    //<blending parameters>
    B_en = 0;
    B_srcfunc = BLEND_SRCCOLOR_BITS;
    B_dstfunc = BLEND_ZERO_BITS;        
          

    int INS_NUM=0;
    unsigned int IM;
    char CMEM[16];
    cout << "IO_FS read Instruction\n";
    while ( !feof(fp_Instruction) )
    {
        fscanf (fp_Instruction ,"%x\n", &IM);
    	SLAVE_MEM[IM_BASE+(INS_NUM++)] = IM;
    }
    printf("INS_NUM: %d\n", INS_NUM);
    trf_offset[0]=IM_BASE;
    trf_burst[0] =INS_NUM;
    
    int o;
    int CMEM_NUM;
    for(o=0;o<OBJ_NUM;o++){
      cout << "IO_FS read Uniform\n";
      CMEM_NUM=0;
      int count=0;
      while ( !feof(fp_Uniform[o]) )
	{       
	  //fixed by sheng-chih
	  for(count=0;count<3;count++)
	    {
	      
	      fscanf(fp_Uniform[o] ,"%s ", CMEM);
	      if(strncmp(CMEM,"0x",2) == 0)
		  SLAVE_MEM[CMEM_BASE+(CMEM_NUM++)] = atoi(&CMEM[2]);
	      else
		{  
		  float temp_float = atof(CMEM);
		  memcpy(&SLAVE_MEM[CMEM_BASE+CMEM_NUM],&temp_float,4);
		  CMEM_NUM++;
		}
	    }
	  
	  fscanf(fp_Uniform[o] ,"%s \n", CMEM);
	  if(strncmp(CMEM,"0x",2) == 0)
	    SLAVE_MEM[CMEM_BASE+(CMEM_NUM++)] = atoi(&CMEM[2]);
	  else
	    {
	      float temp_float = atof(CMEM);
	      memcpy(&SLAVE_MEM[CMEM_BASE+CMEM_NUM],&temp_float,4);
	      CMEM_NUM++;
	    }

	  //fscanf(fp_Uniform[o] ,"%f\n", &CMEM);
	  //SLAVE_MEM[CMEM_BASE+(CMEM_NUM++)] = *(int*)&CMEM;
	  //end fixed
	}
      
      //-----------------------------
    //(CMEM)
    
    trf_offset[1]=CMEM_BASE;
    trf_burst[1] =CMEM_NUM;
    //-----------------------------
    //(CONTROL_REG)
    zero_19 = "0000000000000000000";
    bv_data = (zero_19,S_en,Z_en,B_en,"00",B_srcfunc,B_dstfunc);
    SLAVE_MEM[CONTROL_REG_BASE+0] = bv_data.to_uint();
    bv_data = ("0",S_mask,S_ref,S_func,Z_func,fail,zfail,zpass);
    SLAVE_MEM[CONTROL_REG_BASE+1] = bv_data.to_uint(); 
    //SLAVE_MEM[CONTROL_REG_BASE] = (zero_19,S_en,Z_en,B_en,"00",B_srcfunc,B_dstfunc);
    //SLAVE_MEM[CONTROL_REG_BASE+1] = ("0",S_mask,S_ref,S_func,Z_func,fail,zfail,zpass);
        
    trf_offset[2]=CONTROL_REG_BASE;
    trf_burst[2] =2;
    
    //-----------------------------
    //(TEX_MEM)
    //for(i=0;i<TEX_LENGTH;++i)
    //   TEX_MEM[i] = 0x77777700;  
             
    //-----------------------------     
    //(ZS_MEM & F_MEM)
    /*for(i=0;i<LCD_LENGTH;++i)
    {
        ZS_MEM[i] = 0xffff00ff;//{Z(16-bits),0,S(8-bits)}    
        F_MEM[i] = 0xffffffff;    
    }*/
    
    //-----------------------------
    //start FS
    SLAVE_MEM[SLAVE_LENGTH-1] = 1;
    trf_offset[3]=SLAVE_LENGTH-1;
    trf_burst[3] =1;
    
    //=====================================================================
    //=====================================================================
    //prepare write data
    for(i=0;i<4;i++) 
    {    
        info_mp.Type     = 1;
        info_mp.Addr     = trf_offset[i];
        info_mp.DataDest = &SLAVE_MEM[trf_offset[i]];
        info_mp.Burst    = trf_burst[i];
        mp_transfer.write(info_mp);//FIFO_out
	
        //cout<<"IO.Initiator: wait response"<<endl;
        if(mp_response.read() == 1)
        {//cout<<"IO.Initiator: transaction ok"<<endl;
        }
        else
        {//cout<<"IO.Initiator: transaction err"<<endl;
        }
        
    }

#ifdef texture_cube
#define LOAD_TEXTURE
#endif

#ifdef morphing_ball
#define LOAD_TEXTURE
#endif

#ifdef ocean
#define LOAD_TEXTURE
#endif

#ifdef LOAD_TEXTURE
    //read texture pic
    cout.flush();
    cout<<"Read picture\n";
    cout.flush();
    char bmp_info[54];
    for(i=0;i<54;++i)
      {
	bmp_info[i]=getc(pfile_pic);
      }
    int j;   
    int byte_count=0;
    for(i=0;i<TEX_LENGTH;++i)
      {
	unsigned int catch_texture=0,get_pixel=0;
	for(j=0;j<3;j++)
	  {
	    get_pixel=(int)getc(pfile_pic);
	    catch_texture|=get_pixel<<((j+1)*8);
	  }
	//printf("%d: %x\n",i,catch_texture);

	catch_texture&=0xFFFFFF00;
	TEX_MEM[i]=catch_texture;
      }
        info_mp.Type     = 1;
        info_mp.Addr     = 0;
        info_mp.DataDest = &TEX_MEM[0];
        info_mp.Burst    = TEX_LENGTH;

        mp_transfer.write(info_mp);//FIFO_out

        cout<<"IO.Initiator: wait response"<<endl;
        if(mp_response.read() == 1)
        {cout<<"IO.Initiator: transaction ok"<<endl;
        }
        else
        {cout<<"IO.Initiator: transaction err"<<endl;
        }
#else
        info_mp.Type     = 1;
        info_mp.Addr     = 0;
        info_mp.DataDest = &TEX_MEM[0];
        info_mp.Burst    = 0;
        mp_transfer.write(info_mp);//FIFO_out
	
        //cout<<"IO.Initiator: wait response"<<endl;
        if(mp_response.read() == 1)
        {//cout<<"IO.Initiator: transaction ok"<<endl;
        }
        else
        {//cout<<"IO.Initiator: transaction err"<<endl;
        }
	
#endif
    cout << "IO_FS wait next obj\n";
    wait(next_obj_FS);
    cout << "IO_FS start next obj\n";
} 
    //sc_stop();
};

void IO_FS::slave_port()
{
  while(1)
  {
	//cout<<"IO.Target: wait request"<<endl;	
	
	wait(sp_transfer.data_written_event());
	info_sp  = sp_transfer.read();
	Type     = info_sp.Type;
	Addr     = info_sp.Addr;
	Burst    = info_sp.Burst;	
	DataAddr = info_sp.DataDest;
	
	//cout<<"IO.Target: Type, Addr, Burst:"<<Type<<" "<<Addr<<" "<<Burst<<endl;
	if(Type)//write
	{
		//cout<<"IO.Target: receice Write Data = ";    
		for(i=0;i<Burst;i++) 
		{
		    databuff[i]=DataAddr[i];
		    //fixed by sheng-chih
		    //cout<<databuff[i]<<", "; cout<<endl;
		    //framebuff<<dec<<(Addr+i)%320<<"  "<<(Addr+i)/320<<"  "<<hex<<databuff[i]<<endl;
		    //cout<<"Addr: "<<Addr<<"\tAddr*4: "<<Addr*4<<"\tdatabuff[i]: "<<databuff[i]<<endl;
		    //framebuff<<"Addr:"<<Addr*4<<endl;
		    //framebuff<<"X: "<<Addr%LCD_WIDTH<<"\tY: "<<Addr/LCD_WIDTH<<endl;
		    //end fixed
		    //framebuff<<"Color: "<<databuff[i]<<endl;
		   
		    //for win frame displayer
#ifdef OUTPUT_COLOR_DEC		    
		    framebuff<<dec<<(Addr+i)%320<<"  "<<(Addr+i)/320<<"  "<<databuff[i]<<endl;
#else
		    framebuff<<dec<<(Addr+i)%320<<"  "<<(Addr+i)/320<<"  "<<hex<<databuff[i]<<endl;
#endif		    
		    //for sheng-chih
		    /*
		    framebuff<<Addr*4<<endl;
		    framebuff<<databuff[i]<<endl;
		    */

		}		
	}
	else
	{
		cout<<"IO.Target: Send Read Data"<<endl;
		for(i=0;i<Burst;i++)
		{
		  DataAddr[i]=i+100;
		    
		}
	}
	//cout<<"IO.Target: Send response"<<endl;
	sp_response.write(1);
    	
	//sc_stop();    	
  }
}
