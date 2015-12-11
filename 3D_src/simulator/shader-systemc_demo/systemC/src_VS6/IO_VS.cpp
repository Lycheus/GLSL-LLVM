#include "IO_VS.h"

void IO_VS::master_port()
{
//=============================================================  
    //read instruction
    printf("IO_VS:read instruction\n");
    im_count=0;
    //addr = (volatile unsigned int *)(BaseSUB4+IM_offset);  
    while ( !feof(fp_Instruction) )
    {
        fscanf (fp_Instruction ,"%s\n", IM);
        //printf("%s\n",IM);
	
        ha=la=i=0;
        j=63;
        for(i=0;i<32;i++)
        {
          ha += (IM[i] - '0' );
          if(i!=31) ha <<= 1;
        }
        for(i=32;i<64;i++)
        {
          la += (IM[i] - '0');
          if(i!=63) la <<= 1;
        }
        //addr[im_count] = (unsigned int)la;        
        //addr[im_count+1] = (unsigned int)ha;
        databuff[im_count] = (unsigned int)la;        
        databuff[im_count+1] = (unsigned int)ha;
        im_count+=2;
    } 	
    
    //Send data to VS by FIFO
    info_mp.Type     = 1;//1:write, 0:read
    info_mp.Addr     = IM_offset;
    info_mp.DataDest = &databuff[0];
    info_mp.Burst    = im_count;
    mp_transfer.write(info_mp);//FIFO_out
    
    //cout<<"IO.Initiator: wait response"<<endl;
    if(mp_response.read() == 1)
        cout<<"IO_VS: send IM ok"<<endl;
    else
        cout<<"IO_VS: send IM err"<<endl;     
    
       
//=============================================================           
    //read Uniform data
    printf("IO_VS: read Uniform data\n");
    uniform_count=0;
    //addr = (unsigned int *)(BaseSUB4+CMEM_offset);
    while ( !feof(fp_Uniform) )
    {       
        fscanf (fp_Uniform ,"%f\n", &CMEM);
        //addr[uniform_count] = *((unsigned int*)((void*)&CMEM));   
        databuff[uniform_count] = *((unsigned int*)((void*)&CMEM));
        //printf("CMEM[%d] = %.8f\n", uniform_count, CMEM);        
        uniform_count++;
    }
    
    //Send data to VS by FIFO
    info_mp.Type     = 1;//1:write, 0:read
    info_mp.Addr     = CMEM_offset;
    info_mp.DataDest = &databuff[0];
    info_mp.Burst    = uniform_count;
    mp_transfer.write(info_mp);//FIFO_out
    
    //cout<<"IO.Initiator: wait response"<<endl;
    if(mp_response.read() == 1)
        cout<<"IO_VS: send Uniform ok"<<endl;
    else
        cout<<"IO_VS: send Uniform err"<<endl;   
//=============================================================    
    //read VST data
    printf("IO_VS: read VST data\n");
    vst_count=0;
    //addr = (unsigned int *)(BaseSUB4+VST_offset);
    while ( !feof(fp_VST) )
    {               
        fscanf (fp_VST ,"%d\n", &VST);
        //addr[vst_count] = (unsigned int)VST;
        databuff[vst_count] = (unsigned int)VST;
        //printf("VST[%d] = %d\n", vst_count, VST);        
        vst_count++;
    }
    //total_a have to setup before write Vertex  
    databuff[vst_count]=(unsigned int)total_a;
    vst_count++;
    
    
    //Send data to VS by FIFO
    info_mp.Type     = 1;//1:write, 0:read
    info_mp.Addr     = VST_offset;
    info_mp.DataDest = &databuff[0];
    info_mp.Burst    = vst_count;
    mp_transfer.write(info_mp);//FIFO_out
    
    //cout<<"IO.Initiator: wait response"<<endl;
    if(mp_response.read() == 1)
        cout<<"IO_VS: send VST ok"<<endl;
    else
        cout<<"IO_VS: send VST err"<<endl; 

//=============================================================
    //read vertex data
    printf("IO_VS: read vertex data\n");  
    i = 0;
    //addr = (unsigned int *)(BaseSUB4+VAB_offset);
    for(v_num=0; v_num < total_v; v_num++)
    {
      for(v_entry_num=0; v_entry_num<=total_a; v_entry_num++)
      {
        for(v_word_num=0; v_word_num<=3; v_word_num++)
        {
           fscanf(fp_Vertex ,"%f\n", &Ex_MEM);           
           databuff[i] = *((unsigned int*)((void*)&Ex_MEM));
           i++;
           
           //addr[i] = (unsigned int)Ex_MEM;
           //printf("Ex_MEM[%d][%d][%d] = %f\n", v_num, v_entry_num, v_word_num, Ex_MEM);           
           
           //fscanf(fp_Vertex ,"%f\n", &Ex_MEM[v_num][v_entry_num][v_word_num]);        
           //printf("Ex_MEM[%d][%d][%d] = %f\n", v_num, v_entry_num, v_word_num, Ex_MEM[v_num][v_entry_num][v_word_num]);
           //addr[i] = (unsigned int)Ex_MEM[v_num][v_entry_num][v_word_num];
           //i++;
        }    
      } 
    }
    
    //Send data to VS by FIFO
    info_mp.Type     = 1;//1:write, 0:read
    info_mp.Addr     = VAB_offset;
    info_mp.DataDest = &databuff[0];
    info_mp.Burst    = i;
    mp_transfer.write(info_mp);//FIFO_out
    
    //cout<<"IO.Initiator: wait response"<<endl;
    if(mp_response.read() == 1)
        cout<<"IO_VS: send VAB ok"<<endl;
    else
        cout<<"IO_VS: send VAB err"<<endl; 
//=================================================================
    //start HW
    printf("IO_VS: start HW\n");  
    //addr = (unsigned int *)(BaseSUB4+en_offset);   
    //addr[0]=(unsigned int)total_v;
    
    databuff[0]=(unsigned int)total_v;

    i=1;
    
    //Send data to VS by FIFO
    info_mp.Type     = 1;//1:write, 0:read
    info_mp.Addr     = en_offset;
    info_mp.DataDest = &databuff[0];
    info_mp.Burst    = i;
    mp_transfer.write(info_mp);//FIFO_out
    
    //cout<<"IO.Initiator: wait response"<<endl;
    if(mp_response.read() == 1)
        cout<<"IO_VS: send VST ok"<<endl;
    else
        cout<<"IO_VS: send VST err"<<endl; 
 
    
    //=====================================================================
    //===================================================================== 
};

void IO_VS::slave_port()
{
  while(1)
  {
	cout<<"IO_VS.s: wait request"<<endl;
	
	wait(sp_transfer.data_written_event());
	info_sp  = sp_transfer.read();
	Type     = info_sp.Type;
	Addr     = info_sp.Addr;
	Burst    = info_sp.Burst;	
	DataAddr = info_sp.DataDest;
	
	cout<<"IO.Target: Type, Addr, Burst:"<<Type<<" "<<Addr<<" "<<Burst<<endl;
	if(Type)//write
	{
		cout<<"IO.Target: receice Write Data = ";    
		for(i=0;i<Burst;i++) 
		 {
		    databuff[i]=DataAddr[i];
		    cout<<databuff[i]<<", "; cout<<endl;
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
	 cout<<"IO.Target: Send response"<<endl;
	 sp_response.write(1);
    	
    	//sc_stop();    	
  }
}
