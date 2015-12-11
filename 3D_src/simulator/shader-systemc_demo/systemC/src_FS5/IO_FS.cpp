#include "IO_FS.h"
#include "frag_define.h"

//added by sheng-chih
//extern unsigned long long temp_SLAVE_MEM[512];
//int temp_INS_NUM=0;
//end added

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
          


    /****initial storage units****/
    //-----------------------------
    //(IM)        
    //frag_shader->IM[0] = 0x8001883636363f80;//ADD R0 R3 R4
    //frag_shader->IM[0] = 0x0801803636363f80;//RCP R0 R3    
    //frag_shader->IM[0] = 0x1001803636363f80;//RSQ R0 R3
    //frag_shader->IM[0] = 0x1801803636363f80;//LOG2 R0 R3
    //frag_shader->IM[0] = 0x2001803636363f80;//POW2 R0 R3
    //frag_shader->IM[0] = 0x3001883636363f80;//DP3 R0 R3 R4
    //frag_shader->IM[0] = 0x9801883636363f80;//DP4 R0 R3 R4
    //frag_shader->IM[0] = 0x9001883636363f80;//MAD R0 R3 R4    

    int INS_NUM=0;

    /*
    */
    /*    
    SLAVE_MEM[IM_BASE+(INS_NUM++)] = 0x00003f80;//TEX R0 R2(00st)
    SLAVE_MEM[IM_BASE+(INS_NUM++)] = 0x78010036;
    SLAVE_MEM[IM_BASE+(INS_NUM++)] = 0x00000000;//FIN
    SLAVE_MEM[IM_BASE+(INS_NUM++)] = 0x70000000;
    */
    
    SLAVE_MEM[IM_BASE+(INS_NUM++)] = 0x00002000;
    SLAVE_MEM[IM_BASE+(INS_NUM++)] = 0x50600000;
    SLAVE_MEM[IM_BASE+(INS_NUM++)] = 0x00002000;
    SLAVE_MEM[IM_BASE+(INS_NUM++)] = 0x50800000;
    SLAVE_MEM[IM_BASE+(INS_NUM++)] = 0x00002000;
    SLAVE_MEM[IM_BASE+(INS_NUM++)] = 0x80418800;
    SLAVE_MEM[IM_BASE+(INS_NUM++)] = 0x00018400;
    SLAVE_MEM[IM_BASE+(INS_NUM++)] = 0x40410000;
    SLAVE_MEM[IM_BASE+(INS_NUM++)] = 0x00010800;
    SLAVE_MEM[IM_BASE+(INS_NUM++)] = 0x40410000;
    SLAVE_MEM[IM_BASE+(INS_NUM++)] = 0x00009000;
    SLAVE_MEM[IM_BASE+(INS_NUM++)] = 0x40410000;
    SLAVE_MEM[IM_BASE+(INS_NUM++)] = 0x00002000;
    SLAVE_MEM[IM_BASE+(INS_NUM++)] = 0x40410000;
    SLAVE_MEM[IM_BASE+(INS_NUM++)] = 0x36003f80;
    SLAVE_MEM[IM_BASE+(INS_NUM++)] = 0x38010036;   
    SLAVE_MEM[IM_BASE+(INS_NUM++)] = 0x00000000;
    SLAVE_MEM[IM_BASE+(INS_NUM++)] = 0x70000000;
    

    //maze
    /*
    SLAVE_MEM[IM_BASE+(INS_NUM++)] = 0x36003f80;
    SLAVE_MEM[IM_BASE+(INS_NUM++)] = 0x38510036;
    SLAVE_MEM[IM_BASE+(INS_NUM++)] = 0x36003f80;
    SLAVE_MEM[IM_BASE+(INS_NUM++)] = 0x3c010036;
    SLAVE_MEM[IM_BASE+(INS_NUM++)] = 0x00000000;
    SLAVE_MEM[IM_BASE+(INS_NUM++)] = 0x70000000;
    */
    /*
    for(int j=0;j<INS_NUM;j++)
      {
	cout<<"SLAVE_MEM["<<j<<"]: "<<SLAVE_MEM[j]<<endl;
      }
    */
    trf_offset[0]=IM_BASE;
    trf_burst[0] =18;

    //-----------------------------
    //(CMEM)
    
    //added by sheng-chih
    int CMEM_NUM=0;

    SLAVE_MEM[CMEM_BASE+(CMEM_NUM++)] = 255;
    SLAVE_MEM[CMEM_BASE+(CMEM_NUM++)] = 0;
    SLAVE_MEM[CMEM_BASE+(CMEM_NUM++)] = 0;
    SLAVE_MEM[CMEM_BASE+(CMEM_NUM++)] = 0;

    trf_offset[1]=CMEM_BASE;
    trf_burst[1] =4;
    //-----------------------------
    //(VERTEX_FIFO) 
    /*VAB[0]  = 130.849869;
    VAB[1]  = 148.991165;
    VAB[2]  = 0.982499;
    VAB[3]  = 1.000000;
    VAB[4]  = 0.000000;
    VAB[5]  = 0.000000;
    VAB[6]  = 1.000000;
    VAB[7]  = 0.000000;
    VAB[8]  = 255.000000;
    VAB[9]  = 0.000000;
    VAB[10] = 0.000000;
    VAB[11] = 255.000000;
    VAB[12] = 1.000000;
    VAB[13] = 1.000000;
    VAB[14] = 0.000000;
    VAB[15] = 0.000000;
            
    VAB[16] = 206.724945;
    VAB[17] = 148.778702;
    VAB[18] = 0.983920;
    VAB[19] = 1.000000;
    VAB[20] = 0.000000;
    VAB[21] = 0.000000;
    VAB[22] = 1.000000;
    VAB[23] = 0.000000;
    VAB[24] = 255.000000;
    VAB[25] = 0.000000;
    VAB[26] = 0.000000;
    VAB[27] = 255.000000;
    VAB[28] = 1.000000;
    VAB[29] = 1.000000;
    VAB[30] = 0.000000;
    VAB[31] = 0.000000;
             
    VAB[32] = 201.430405;
    VAB[33] = 78.428963;
    VAB[34] = 0.985115;
    VAB[35] = 1.000000;
    VAB[36] = 0.000000;
    VAB[37] = 0.000000;
    VAB[38] = 1.000000;
    VAB[39] = 0.000000;
    VAB[40] = 255.000000;
    VAB[41] = 0.000000;
    VAB[42] = 0.000000;
    VAB[43] = 255.000000;
    VAB[44] = 1.000000;
    VAB[45] = 1.000000;
    VAB[46] = 0.000000;
    VAB[47] = 0.000000;
    
     //<vertex1>
    SLAVE_MEM[VERTEX_FIFO_BASE+0]  = *((unsigned int*)((void*)&VAB[0]));//X1
    SLAVE_MEM[VERTEX_FIFO_BASE+1]  = *((unsigned int*)((void*)&VAB[1]));//Y1
    SLAVE_MEM[VERTEX_FIFO_BASE+2]  = *((unsigned int*)((void*)&VAB[2]));//Z1
    SLAVE_MEM[VERTEX_FIFO_BASE+3]  = *((unsigned int*)((void*)&VAB[3]));//W1
    SLAVE_MEM[VERTEX_FIFO_BASE+4]  = *((unsigned int*)((void*)&VAB[4]));//N1x(no used)
    SLAVE_MEM[VERTEX_FIFO_BASE+5]  = *((unsigned int*)((void*)&VAB[5]));//N1y(no used)
    SLAVE_MEM[VERTEX_FIFO_BASE+6]  = *((unsigned int*)((void*)&VAB[6]));//N1z(no used)
    SLAVE_MEM[VERTEX_FIFO_BASE+7]  = *((unsigned int*)((void*)&VAB[7]));//0
    SLAVE_MEM[VERTEX_FIFO_BASE+8]  = *((unsigned int*)((void*)&VAB[8]));//R1 
    SLAVE_MEM[VERTEX_FIFO_BASE+9]  = *((unsigned int*)((void*)&VAB[9]));//G1
    SLAVE_MEM[VERTEX_FIFO_BASE+10] = *((unsigned int*)((void*)&VAB[10]));//B1
    SLAVE_MEM[VERTEX_FIFO_BASE+11] = *((unsigned int*)((void*)&VAB[11]));//A1
    SLAVE_MEM[VERTEX_FIFO_BASE+12] = *((unsigned int*)((void*)&VAB[12]));//U1
    SLAVE_MEM[VERTEX_FIFO_BASE+13] = *((unsigned int*)((void*)&VAB[13]));//V1
    SLAVE_MEM[VERTEX_FIFO_BASE+14] = *((unsigned int*)((void*)&VAB[14]));//0
    SLAVE_MEM[VERTEX_FIFO_BASE+15] = *((unsigned int*)((void*)&VAB[15]));//0
    
    //<vertex2>
    SLAVE_MEM[VERTEX_FIFO_BASE+16] = *((unsigned int*)((void*)&VAB[16]));//X2
    SLAVE_MEM[VERTEX_FIFO_BASE+17] = *((unsigned int*)((void*)&VAB[17]));//Y2
    SLAVE_MEM[VERTEX_FIFO_BASE+18] = *((unsigned int*)((void*)&VAB[18]));//Z2
    SLAVE_MEM[VERTEX_FIFO_BASE+19] = *((unsigned int*)((void*)&VAB[19]));//W2
    SLAVE_MEM[VERTEX_FIFO_BASE+20] = *((unsigned int*)((void*)&VAB[20]));//N2x(no used)
    SLAVE_MEM[VERTEX_FIFO_BASE+21] = *((unsigned int*)((void*)&VAB[21]));//N2y(no used)
    SLAVE_MEM[VERTEX_FIFO_BASE+22] = *((unsigned int*)((void*)&VAB[22]));//N2z(no used)
    SLAVE_MEM[VERTEX_FIFO_BASE+23] = *((unsigned int*)((void*)&VAB[23]));//0
    SLAVE_MEM[VERTEX_FIFO_BASE+24] = *((unsigned int*)((void*)&VAB[24]));//R2 
    SLAVE_MEM[VERTEX_FIFO_BASE+25] = *((unsigned int*)((void*)&VAB[25]));//G2
    SLAVE_MEM[VERTEX_FIFO_BASE+26] = *((unsigned int*)((void*)&VAB[26]));//B2
    SLAVE_MEM[VERTEX_FIFO_BASE+27] = *((unsigned int*)((void*)&VAB[27]));//A2
    SLAVE_MEM[VERTEX_FIFO_BASE+28] = *((unsigned int*)((void*)&VAB[28]));//U2
    SLAVE_MEM[VERTEX_FIFO_BASE+29] = *((unsigned int*)((void*)&VAB[29]));//V2
    SLAVE_MEM[VERTEX_FIFO_BASE+30] = *((unsigned int*)((void*)&VAB[30]));//0
    SLAVE_MEM[VERTEX_FIFO_BASE+31] = *((unsigned int*)((void*)&VAB[31]));//0    
    
    //<vertex3>
    SLAVE_MEM[VERTEX_FIFO_BASE+32] = *((unsigned int*)((void*)&VAB[32]));//X3
    SLAVE_MEM[VERTEX_FIFO_BASE+33] = *((unsigned int*)((void*)&VAB[33]));//Y3
    SLAVE_MEM[VERTEX_FIFO_BASE+34] = *((unsigned int*)((void*)&VAB[34]));//Z3
    SLAVE_MEM[VERTEX_FIFO_BASE+35] = *((unsigned int*)((void*)&VAB[35]));//W3
    SLAVE_MEM[VERTEX_FIFO_BASE+36] = *((unsigned int*)((void*)&VAB[36]));//N3x(no used)
    SLAVE_MEM[VERTEX_FIFO_BASE+37] = *((unsigned int*)((void*)&VAB[37]));//N3y(no used)
    SLAVE_MEM[VERTEX_FIFO_BASE+38] = *((unsigned int*)((void*)&VAB[38]));//N3z(no used)
    SLAVE_MEM[VERTEX_FIFO_BASE+39] = *((unsigned int*)((void*)&VAB[39]));//0
    SLAVE_MEM[VERTEX_FIFO_BASE+40] = *((unsigned int*)((void*)&VAB[40]));//R3 
    SLAVE_MEM[VERTEX_FIFO_BASE+41] = *((unsigned int*)((void*)&VAB[41]));//G3
    SLAVE_MEM[VERTEX_FIFO_BASE+42] = *((unsigned int*)((void*)&VAB[42]));//B3
    SLAVE_MEM[VERTEX_FIFO_BASE+43] = *((unsigned int*)((void*)&VAB[43]));//A3
    SLAVE_MEM[VERTEX_FIFO_BASE+44] = *((unsigned int*)((void*)&VAB[44]));//U3
    SLAVE_MEM[VERTEX_FIFO_BASE+45] = *((unsigned int*)((void*)&VAB[45]));//V3
    SLAVE_MEM[VERTEX_FIFO_BASE+46] = *((unsigned int*)((void*)&VAB[46]));//0
    SLAVE_MEM[VERTEX_FIFO_BASE+47] = *((unsigned int*)((void*)&VAB[47]));//0

    
    trf_offset[1]=VERTEX_FIFO_BASE;
    trf_burst[1] =48;
    */
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
		    cout<<"Addr: "<<Addr<<"\tAddr*4: "<<Addr*4<<"\tdatabuff[i]: "<<databuff[i]<<endl;
		    framebuff<<Addr*4<<endl;
		    //framebuff<<Addr<<endl;
		    //end fixed
		    framebuff<<databuff[i]<<endl;
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
