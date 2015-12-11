#include "SC_VS.h"
#include "clip.h"
#include "../testbench.h"


union VS_vertex_type // for OR instruction 2010/09/29  ya
{
  unsigned int int_format;
  float float_format;
}; 


VS_vertex_type VS_or_src1 , VS_or_src2 , VS_or_dst ;

void do_or()
{
  //printf("do_or:\n");
  //printf("src1.int: %d %f\n",VS_or_src1.int_format,VS_or_src1.int_format);
  //printf("src1.float: %d %f\n",VS_or_src1.float_format,VS_or_src1.float_format);
  //VS_or_dst.float_format = (float) (VS_or_src1.int_format | VS_or_src2.int_format) ;
  int temp=(VS_or_src1.int_format | VS_or_src2.int_format) ;
  memcpy(&VS_or_dst.float_format,&temp,4);
  memcpy(&VS_or_dst.int_format,&temp,4);
  //printf("dst.int: %d %f\n",VS_or_dst.int_format,VS_or_dst.int_format);
  //printf("dst.float: %d %f\n",VS_or_dst.float_format,VS_or_dst.float_format);
}   


void SC_VS::access()
{
    cout<<"VS: access()"<<endl;
    while(1)
    {      
      unsigned int address;
      unsigned int data;
      int CMEM_entry,CMEM_component;

      //Slave FIFO transaction
      info_sp  = sp_transfer.read();
      Type     = info_sp.Type;
      Addr     = info_sp.Addr;
      Burst    = info_sp.Burst;    
      DataAddr = info_sp.DataDest;//point :link to read/write data    
      cout<<"VS.Target: Type, Addr, Burst:"<<Type<<" "<<Addr<<" "<<Burst<<endl;
      //cout<<"VS.Target: Data = ";      
      for(i=0;i<Burst;i++)
      {  
          address = Addr+i;
          data = DataAddr[i];          
                
          //=====================================================
	  //fixed by sheng-chih
          if(address<CMEM_offset)  //read instruction
	    //if(address<1000)  //read instruction
	    //end fixed
          {
              int i; 
              if(address%2==0)//c::char[32:63],verlog::reg[31:0]
              {
                for(i=63;i>=32;i--)
                {
                  IM[address/2][i] = data%2 + '0';
                  data >>= 1;
                }      
              }
              else  //c::char[0:31],verlog::reg[63:32]
              {
                  for(i=31;i>=0;i--)
         	  {
         	    IM[address/2][i] = data%2 + '0';
                     data >>= 1;
         	  }
         	  IM[address/2][64] = '\0';
                  im_count++;
         	  fprintf(log_slave,"IM[%d]:%s\n",address/2,IM[address/2]); 
         	  //printf("IM[%d]:%s\n",address/2,IM[address/2]); 
              }	     
           }
	  //fixed by sheng-chih
           else if(address<VST_offset)  //read Uniform data
	     //else if(address<2000)  //read Uniform data
           {   
         	//CMEM[address-1000] = *((float*)((void*)&data)); 
         	//address -= 1000;
         	address -= CMEM_offset;
	//end fixed
         	CMEM_component=address%4;
         	CMEM_entry =   address/4;
           	CMEM[CMEM_entry][CMEM_component] = *((unsigned int*)((void*)&data));
           	fprintf(log_slave,"CMEM[%d][%d]:%.8f\n",CMEM_entry,CMEM_component, *(float*)&CMEM[CMEM_entry][CMEM_component]);         
           	//printf("CMEM[%d][%d] = %f\n", CMEM_entry, CMEM_component, CMEM[CMEM_entry][CMEM_component]);    

           }
	  //fixed by sheng-chih
           else if(address<VAB_offset)  //read VST data
	     //else if(address<3000)  //read VST data
           {  
	     //VST[address-2000] = (int)data;
         	//VST[address-5000] = (int)data;
	//end fixed
         	total_a = (int)data;
         	//fprintf(log_slave,"VST[%d]:%d\n",address-2000,VST[address-2000]);      
           }
	  //fixed by sheng-chih
	  //else if(address<4000) //read vertex data
           else if(address<en_offset) //read vertex data
           {   
           	address -= VAB_offset;
           	//address -= 3000;
      //end fixed
           	//v_num = address/16;
           	//v_entry_num = (address%16)>>2;
           	//v_word_num = address%4;
           	v_num = address/(4*total_a);
           	v_entry_num = (address%(4*total_a))/4;
           	v_word_num = address%4;

           	Ex_MEM[v_num][v_entry_num][v_word_num]  = *((unsigned int*)((void*)&data));
		//printf("Ex_MEM:%f\n",Ex_MEM);
           	//fprintf(log_slave,"total_a:%d,address:%d, Ex_MEM[%d][%d][%d]:%.8f\n",total_a,address,v_num,v_entry_num,v_word_num,Ex_MEM[v_num][v_entry_num][v_word_num]);
           	//printf("total_a:%d,address:%d, Ex_MEM[%d][%d][%d]:%.8f\n",total_a,address,v_num,v_entry_num,v_word_num,Ex_MEM[v_num][v_entry_num][v_word_num]);

           }
	  //fixed by sheng-chih
           else if(address==en_offset){
	     //else if(address==4000){
     //end fixed
                 total_v = data;//***** total vertex number *****
                 cout<<"VS: vertex number: "<<total_v<<endl;            
                 cout<<"VS: attribute number: "<<total_a<<endl;
                 
                 vertex_count=0; 
                 dp_temp_FL=0;
                 dp_temp_FX=0;
                 dsq_temp=0;
                 div_temp=0;
                 
                 //GPR[0]=0 , special location 
                 for(int m = 0 ; m<=3 ; m++)
                 GPR[0][m]=0;
                 
                 for(i=0;i<4;i++)
                 {
				   //Reset VOB to 0 modified by Tsao
				   VOB[0][i]=0;
				   VOB[1][i]=0;
				   VOB[2][i]=0;
				   VOB[3][i]=0;
				   //-------
				   Rs_SV_temp[i]=0;
				   Rt_SV_temp[i]=0;
				   Rd_SV_temp[i]=0;
                   Rs_swiz_temp[i]=0;
				   Rt_swiz_temp[i]=0;
				   Rd_swiz_temp[i]=0;
                           
                   mvi_temp_FL[i]=0;
                   mvi_temp_FX[i]=0;
                   
                   min_temp_FL[i]=0;
                   min_temp_FX[i]=0;
                   
                   max_temp_FL[i]=0;
                   max_temp_FX[i]=0;
                   
                   add_temp_FL[i]=0;
                   add_temp_FX[i]=0;
                   
                   mulh_temp[i]=0;
                   mull_temp[i]=0;
                   mull_tmp64b[i]=0;
                   
                   mad_temp_FL[i]=0;
                   mad_temp_FX[i]=0;
                   
                   inv_temp[i]=0;
                   and_temp[i]=0;
                    or_temp[i]=0;
                   xor_temp[i]=0;
                   R1_SV_temp[4]=0; R2_SV_temp[4]=0; R3_SV_temp[4]=0; R4_SV_temp[4]=0;
                   R1_temp_FL[i]=0; R2_temp_FL[i]=0; R3_temp_FL[i]=0; R4_temp_FL[i]=0;
                   R1_temp_FX[i]=0; R2_temp_FX[i]=0; R3_temp_FX[i]=0; R4_temp_FX[i]=0;
                   
                   shrl_temp[i]=0;
                 } 
                 cout<<"VS: notify ->flag_VScore()"<<endl;
                 flag_VScore.notify(0,SC_NS);
                       
            }      
      }//end FIFO->VS transaction
      sp_response.write(1);
            
    }//end while 
    
    i=0;//CYL:currently non-used
    if(i)
    {
	cout<<"FS.Initiator: Send Read data request"<<endl;
	//prepare write data
	
	//setup transfer parameter
	info_mp.Type     = 0; //0:read, 1:write
	info_mp.Addr     = 999;
	info_mp.DataDest = DataAddr;
	info_mp.Burst    = 6;
	mp_transfer.write(info_mp);
	
	//wait Target port response
	cout<<"FS.Initiator: wait response"<<endl;
	//receive readed data
	if(mp_response.read()==1 && info_mp.Type == 0)
	{
	    cout<<"FS.Initiator: read Data = ";
	    for(i=0;i<(int)info_mp.Burst;i++)
	       cout<<DataAddr[i]<<" ";
	    cout<<endl;		
	}
	else if(mp_response.read()==1)
	    cout<<"FS.Initiator: write data ok"<<endl;
	else
	    cout<<"FS.Initiator: transaction err"<<endl;		
    }       
};

#define check(x) {printf("Here: %d\n",x);fflush(stdout);}
void SC_VS::VScore()
{   
  while(1)
  {
	  //gk add for full system
	  wait(flag_VScore);
      cout<<"VS: run VScore()"<<endl; 
      mp_VS_END.write(0);
      //--------- VS start -------------------------------------------------------
      
      for(v_num=0; v_num < total_v; v_num++)
	  {
		  //fixed by sheng-chih
		  printf("\n########## Vertex:%d ##########\n",v_num+1);
		  fprintf(fp_Result, "\n########## Vertex:%d ##########\n",v_num+1);
		  //end fixed
		  //vertex fetch---------------------
		  for(v_entry_num=0; v_entry_num < total_a; v_entry_num++)
			  for(v_word_num=0; v_word_num<=3; v_word_num++)
				  VAB[v_entry_num][v_word_num] = Ex_MEM[v_num][v_entry_num][v_word_num];
		  
		  //---------------------------------
		  PC=0;
		  while(PC<im_count && PC!=8192)
		  {
			  //if(v_num==5 && PC==242)  system ("pause");//For Debug

			  fprintf(fp_Result,"\nVertex %d\n", v_num+1);
			  //-----------initial register---
			  for(i=0 ; i<4 ; i++)
			  {
				  Rs_SV_temp[i]=0;
				  Rt_SV_temp[i]=0;
				  Rd_SV_temp[i]=0;
				  Rs_swiz_temp[i] = 0;
				  Rt_swiz_temp[i] = 0;
				  Rd_swiz_temp[i] = 0;
				  branch_taken[i] = 0;
			  }
			  //check(1);

			  //printf("PC = %d    ",PC);
			  //------------------------------
			  decode(PC);
			  //check(3);

			  if(IF.op!=29)
			  {
				  if(IF.op<24 || IF.op>27)
					  PC++;
				  else
				  {
					  if(branch_taken[0]!=IF.SCC_x || branch_taken[1]!=IF.SCC_y || branch_taken[2]!=IF.SCC_z || branch_taken[3]!=IF.SCC_w)
						  PC++;
				  }
			  }
			  else
			  {
				  if(PC==0)
					  PC = 8192;
			  }
		  }

		  //sheng-chang
		  /*printf("VOB[0] = %f\t%f\t%f\t%f\n", VOB[0][0], VOB[0][1], VOB[0][2], VOB[0][3]);
		  printf("VOB[1] = %f\t%f\t%f\t%f\n", VOB[1][0], VOB[1][1], VOB[1][2], VOB[1][3]);
		  printf("VOB[2] = %f\t%f\t%f\t%f\n", VOB[2][0], VOB[2][1], VOB[2][2], VOB[2][3]);
		  printf("VOB[3] = %f\t%f\t%f\t%f\n", VOB[3][0], VOB[3][1], VOB[3][2], VOB[3][3]);
		  printf("VOB[4] = %f\t%f\t%f\t%f\n", VOB[4][0], VOB[4][1], VOB[4][2], VOB[4][3]);
		  printf("VOB[5] = %f\t%f\t%f\t%f\n", VOB[5][0], VOB[5][1], VOB[5][2], VOB[5][3]);
		  printf("VOB[6] = %f\t%f\t%f\t%f\n", VOB[6][0], VOB[6][1], VOB[6][2], VOB[6][3]);
		  printf("VOB[7] = %f\t%f\t%f\t%f\n", VOB[7][0], VOB[7][1], VOB[7][2], VOB[7][3]);
		  */
		  
		  //-------------------------------------clip c model start---------------------------------------
		  VST[8][3]=0;
		  if(VST[8][3])
		  {
			  cout << "VST[8][3] = " << VST[8][3] << endl;
			  clip clip_model;//clip initail
			  /*
			  clip_model.x_max=VST[1];
			  clip_model.x_min=VST[2];
			  clip_model.y_max=VST[3];
			  clip_model.y_min=VST[4];
			  */
			  
			  clip_model.x_max= 320.0;
			  clip_model.x_min= 0.0;
			  clip_model.y_max= 240.0;
			  clip_model.y_min= 0.0;
			  
			  //vertex input
			  for(i=0;i<4;i++)
			  {
				  clip_model.Vertex[v_num%3].entry1[i]=VOB[0][i];
				  clip_model.Vertex[v_num%3].entry2[i]=VOB[1][i];
				  //clip_model.Vertex[v_num%3].entry3[i]=VOB[2][i];
				  //clip_model.Vertex[v_num%3].entry4[i]=VOB[3][i];
			  }
			  
			  //fixed by sheng-chih
			  /*
			  printf("==Before==\nvertex1: x=%f y=%f z=%f \n",clip_model.Vertex[0].entry1[0],clip_model.Vertex[0].entry1[1],clip_model.Vertex[0].entry1[2]);
			  printf("vertex2: x=%f y=%f z=%f \n",clip_model.Vertex[1].entry1[0],clip_model.Vertex[1].entry1[1],clip_model.Vertex[1].entry1[2]);
			  printf("vertex3: x=%f y=%f z=%f \n",clip_model.Vertex[2].entry1[0],clip_model.Vertex[2].entry1[1],clip_model.Vertex[2].entry1[2]);
			  */
			  //end fixed
			  if(v_num%3==2)// triangle in
			  {		
				  clip_model.clip_module();
				  
				  //fixed by sheng-chih
				  //printf("\nnumber=%d\n",clip_model.vertex_number_out);
				  //end fixed
				  
				  for(i=0;i<clip_model.vertex_number_out;i++)
				  {
					  //fixed by sheng-chih
					  //printf("x=%f,y=%f,z=%f\n",clip_model.vertex_reg_out[i].entry1[0],clip_model.vertex_reg_out[i].entry1[1],clip_model.vertex_reg_out[i].entry1[2]);
					  //end fixed
					  fprintf(V_O,"\nvertex %d out\n",++vertex_count);
					  
					  for(j=0;j<4;j++)
					  {
						  VOB[0][j]=clip_model.vertex_reg_out[i].entry1[j];
						  VOB[1][j]=clip_model.vertex_reg_out[i].entry2[j];
						  //VOB[2][j]=clip_model.vertex_reg_out[i].entry3[j];
						  //VOB[3][j]=clip_model.vertex_reg_out[i].entry4[j];
					  }
					  
					  /*
					  printf("\n==After==\nvertex1: x=%f y=%f z=%f \n",clip_model.Vertex[0].entry1[0],clip_model.Vertex[0].entry1[1],clip_model.Vertex[0].entry1[2]);
					  printf("vertex2: x=%f y=%f z=%f \n",clip_model.Vertex[1].entry1[0],clip_model.Vertex[1].entry1[1],clip_model.Vertex[1].entry1[2]);
					  printf("vertex3: x=%f y=%f z=%f \n",clip_model.Vertex[2].entry1[0],clip_model.Vertex[2].entry1[1],clip_model.Vertex[2].entry1[2]);
					  */
					  
					  fprintf(V_O,"entry1 = ");
					  printf("\nvertex %d out\n",vertex_count);
					  printf("VOB entry1 = ");
					  for(j=0;j<4;j++)
					  {
						  mp_VOB.write(*(float*)&VOB[0][j]);//CYL:Push Vertex to FIFO
						  fprintf(V_O,"%f\t", *(float*)&VOB[0][j]);
						  printf("%f\t", *(float*)&VOB[0][j]);
					  }
					  fprintf(V_O,"\nentry2 = ");
					  printf("\nVOB entry2 = ");
					  
					  for(j=0;j<4;j++)
					  {
						  mp_VOB.write(*(float*)&VOB[1][j]);//CYL:Push Vertex to FIFO
						  fprintf(V_O,"%f\t", *(float*)&VOB[1][j]);
						  printf("%f\t", *(float*)&VOB[1][j]);
					  }
					  fprintf(V_O,"\nentry3 = ");
					  printf("\nVOB entry3 = ");
					  for(j=0;j<4;j++)
					  {
						  mp_VOB.write(*(float*)&VOB[2][j]);//CYL:Push Vertex to FIFO
						  fprintf(V_O,"%f\t", *(float*)&VOB[2][j]);
						  printf("%f\t", *(float*)&VOB[2][j]);
					  }
					  fprintf(V_O,"\nentry4 = ");
					  printf("\nVOB entry4 = ");
					  for(j=0;j<4;j++)
					  {
						  mp_VOB.write(*(float*)&VOB[3][j]);//CYL:Push Vertex to FIFO
						  fprintf(V_O,"%f\t", *(float*)&VOB[3][j]);
						  printf("%f\t", *(float*)&VOB[3][j]);
					  }
					  
/*
//sheng-chang added
#ifdef morphing_ball		      
		      fprintf(V_O,"\nentry5 = ");
		      for(j=0;j<4;j++)
			{
			  mp_VOB.write(VOB[4][j]);//CYL:Push Vertex to FIFO
			  fprintf(V_O,"%f\t",VOB[4][j]);
			}
#endif
//end added
*/
					  fprintf(V_O,"\n");
				  }
			  }
			  printf("~~~After clip~~~\n");
		}
		else
		{
			fprintf(V_O,"\nvertex %d out\n",++vertex_count);
			printf("\nvertex %d out\n",vertex_count);
			fprintf(V_O,"entry1 = ");
			printf("VOB entry1 = ");
			for(j=0;j<4;j++)
			{
				mp_VOB.write(*(float*)&VOB[0][j]);//CYL:Push Vertex to FIFO
				fprintf(V_O,"%f\t", *(float*)&VOB[0][j]);
				printf("%f\t", *(float*)&VOB[0][j]);
			}
			fprintf(V_O,"\nentry2 = ");
			printf("\nVOB entry2 = ");
			for(j=0;j<4;j++)
			{
				mp_VOB.write(*(float*)&VOB[1][j]);//CYL:Push Vertex to FIFO
				fprintf(V_O,"%f\t", *(float*)&VOB[1][j]);
				printf("%f\t", *(float*)&VOB[1][j]);
			}
			fprintf(V_O,"\nentry3 = ");
			printf("\nVOB entry3 = ");
			for(j=0;j<4;j++)
			{
				mp_VOB.write(*(float*)&VOB[2][j]);//CYL:Push Vertex to FIFO
				fprintf(V_O,"%f\t", *(float*)&VOB[2][j]);
				printf("%f\t", *(float*)&VOB[2][j]);
			}
			fprintf(V_O,"\nentry4 = ");
			printf("\nVOB entry4 = ");
			for(j=0;j<4;j++)
			{
				mp_VOB.write(*(float*)&VOB[3][j]);//CYL:Push Vertex to FIFO
				fprintf(V_O,"%f\t", *(float*)&VOB[3][j]);
				printf("%f\t", *(float*)&VOB[3][j]);
			}
	      /*
	      //sheng-chang added
	      #ifdef morphing_ball
	      
	      fprintf(V_O,"\nentry5 = ");
	      for(j=0;j<4;j++)
	      {
	      mp_VOB.write(VOB[4][j]);//CYL:Push Vertex to FIFO
	      fprintf(V_O,"%f\t",VOB[4][j]);
	      }
	      #endif
	      //end added
	      */
			fprintf(V_O,"\n");
		}
	  //fixed by sheng-chih
	  //printf("v_num second: %d\n",v_num);
	  //end fixed
	}  
      //system("pause");
      //fixed by sheng-chih
      //cout<<"VS_end"<<endl;
      //end fixed
	mp_VS_END.write(1);
      //sc_stop();
	};

}


//================================================
//================================================
void SC_VS::DecTofloat(float a)
{
	int y,i;
    //char word[32];
    y=*((int*)((void*)&a));
    for(i=31;i>=0;i--){
        word[i]=(y&1)+48;
        y>>=1;
    }
	/*
    for(i=0;i<32;i++){
        temp[i] = word[i];
        printf("%c",temp[i]);
        //fprintf(ver_out,"%c",word[i]);
    }
    //fprintf(ver_out,"\n");
    printf("\n");*/
    
    //return temp[];
    
}

void SC_VS::chartoint()
{
    int i;
    for(i=0;i<32;i++){
        temp[i] = word[i]-48;
        //printf("%d",temp[i]);

    }
    //printf("\n");
}

void SC_VS::binarytohex()
{
    //int i;
    hex[0] = temp[0]*8 + temp[1]*4 + temp[2]*2 + temp[3];
    hex[1] = temp[4]*8 + temp[5]*4 + temp[6]*2 + temp[7];
    hex[2] = temp[8]*8 + temp[9]*4 + temp[10]*2+ temp[11];
    hex[3] = temp[12]*8+ temp[13]*4+ temp[14]*2+ temp[15]; 
    hex[4] = temp[16]*8+ temp[17]*4+ temp[18]*2+ temp[19];
    hex[5] = temp[20]*8+ temp[21]*4+ temp[22]*2+ temp[23];
    hex[6] = temp[24]*8+ temp[25]*4+ temp[26]*2+ temp[27];
    hex[7] = temp[28]*8+ temp[29]*4+ temp[30]*2+ temp[31];
    
}

void SC_VS::get_float(float a)
{
   int i;
   
   DecTofloat(a);
   chartoint();
   binarytohex();
   for(i=0;i<8;i++)
        fprintf(fp_Result,"%x",hex[i]);
}
//=============================================================================

//=============================================================================
//                                                                           ||
//                                                                           ||
//                    decimal to 32-b fixed function code                    ||
//                                                                           ||
//                                                                           ||
//=============================================================================

void SC_VS::itobs(int n)
{
   int size = 8 * sizeof(n);
   int i = size-1;
  
   for(i=size-1; i>=0; i--)
   {
      c[i] = (1 & n) + '0';
      n >>= 1;
   }
  
   //ps[size] = '\0';
   //return ps;
}

void SC_VS::chatoint()
{
    int i;
    for(i=0;i<32;i++){
        s[i] = c[i]-48;
        //printf("%d",temp[i]);

    }
    //printf("\n");
}

void SC_VS::bintohex()
{
    hx[0] = s[0]*8 + s[1]*4 + s[2]*2 + s[3];
    hx[1] = s[4]*8 + s[5]*4 + s[6]*2 + s[7];
    hx[2] = s[8]*8 + s[9]*4 + s[10]*2+ s[11];
    hx[3] = s[12]*8+ s[13]*4+ s[14]*2+ s[15]; 
    hx[4] = s[16]*8+ s[17]*4+ s[18]*2+ s[19];
    hx[5] = s[20]*8+ s[21]*4+ s[22]*2+ s[23];
    hx[6] = s[24]*8+ s[25]*4+ s[26]*2+ s[27];
    hx[7] = s[28]*8+ s[29]*4+ s[30]*2+ s[31];    
}

void SC_VS::get_fix(int n)
{
   int i; 
    
   itobs(n);
   chatoint();
   bintohex();
   for(i=0;i<8;i++)
       fprintf(fp_Result,"%x",hx[i]);
   //for(i=0;i<32;i++)
   //    printf("%c",c[i]);
   //printf("%d = %s\n", n, itobs(n,s));
}
//=============================================================================

//=============================================================================
//                                                                           ||
//                                                                           ||
//                    hex_int to float function code                    ||
//                                                                           ||
//                                                                           ||
//=============================================================================
//FILE *output;

float int_to_float(int a)
{
    int i;
    int mod[32];
    int sign;
    int exp=0;
    float fraction=1;
    float result;
    
    //output = fopen("output.txt", "w");
    
    for(i=0;i<32;i++)
    {
       mod[31-i] = a%2;
       a=a/2;
       //fprintf(output, "a=%d\n   ",a);
       //fprintf(output, "mod[%d]=%d\n",31-i,mod[31-i]);
    }
    /*
    for(i=0;i<32;i++)
    {
       printf("%d", mod[i]);
    }
    printf("\n");
    */

    if(mod[0]==0){
       sign = 1;
       //printf("sign = %d\n", 0);
       }
    else{
       sign = -1;
       //printf("sign bit= %d\n", 1);
       }
       
    
    for(i=7;i>-1;i--)
       exp = exp + mod[8-i]*int(powf(2,i));
    exp = exp - 127;
    //printf("exp = %d\n", exp);
    
    for(i=-1;i>-24;i--)
       fraction = fraction + mod[8-i]*powf(2,i);
    //printf("fraction = %f\n", fraction);
    
    
    result = sign*(powf(2,exp))*fraction;
    //printf("result = %f\n", result);
        
    return result;
}
//=============================================================================

//=============================================================================
//                                                                           ||
//                                                                           ||
//                            VS C model code                                ||
//                                                                           ||
//                                                                           ||
//=============================================================================

void SC_VS::VS_01_DSQ()
{
	fprintf(fp_Result,"Op:1 = DSQ\n");
	if(IF.Rd>=32)
		fprintf(fp_Result,"Rd:VOB_entry[%d]\n", IF.Rd-32);
	else
		fprintf(fp_Result,"Rd:GPR[%d]\n", IF.Rd);
	if(IF.Rs>=32)
		fprintf(fp_Result,"Rs:VAB_entry[%d]\n", IF.Rs-32);
	else
		fprintf(fp_Result,"Rs:GPR[%d]\n", IF.Rs);
	fprintf(fp_Result,"Rt:Dont care\n");
	fprintf(fp_Result,"Sw_d:Dont care\n");
	fprintf(fp_Result,"Sw_s:Dont care\n");
	fprintf(fp_Result,"Sw_t:Dont care\n");
	fprintf(fp_Result,"Neg:Dont care\n");
	fprintf(fp_Result,"WM:%d%d%d%d\n", IF.WM_x, IF.WM_y, IF.WM_z, IF.WM_w);
	fprintf(fp_Result,"FXL:%d\n", IF.FXL);
	fprintf(fp_Result,"S/V:%d\n", IF.S_V);
	if(IF.S_V!=0 || IF.FXL!=1)
		fprintf(fp_Result,"S/V must be 00 and FXL must be 1.\n");

	//------------Initial variable--------------------------------------
	dsq_temp = 0;

	//------------determine the source----------------------------------
	for(i=0;i<=3;i++)
	{
		Rs_swiz_temp[i] = (IF.Rs>=32) ? VAB[IF.Rs-32][i] : GPR[IF.Rs][i];
		Rd_swiz_temp[i] = (IF.Rd>=32) ? VAB[IF.Rd-32][i] : GPR[IF.Rd][i];
	}

	if(IF.Rs>=32) {
		fprintf(fp_Result,"Rs:VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rs-32, *(float*)&Rs_swiz_temp[0], *(float*)&Rs_swiz_temp[1], *(float*)&Rs_swiz_temp[2], *(float*)&Rs_swiz_temp[3]);
		fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
	}
	else {
		fprintf(fp_Result,"Rs:GPR[%d]:%f_%f_%f_%f\n", IF.Rs, *(float*)&Rs_swiz_temp[0], *(float*)&Rs_swiz_temp[1], *(float*)&Rs_swiz_temp[2], *(float*)&Rs_swiz_temp[3]);
		fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
	}
	if(IF.Rd>=32) {
		fprintf(fp_Result,"Rd:VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rd-32, *(float*)&Rd_swiz_temp[0], *(float*)&Rd_swiz_temp[1], *(float*)&Rd_swiz_temp[2], *(float*)&Rd_swiz_temp[3]);
		fprintf(fp_Result,"Rd:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rd-32, Rd_swiz_temp[0], Rd_swiz_temp[1], Rd_swiz_temp[2], Rd_swiz_temp[3]);
	}
	else {
		fprintf(fp_Result,"Rd:GPR[%d]:%f_%f_%f_%f\n", IF.Rd, *(float*)&Rd_swiz_temp[0], *(float*)&Rd_swiz_temp[1], *(float*)&Rd_swiz_temp[2], *(float*)&Rd_swiz_temp[3]);
		fprintf(fp_Result,"Rd:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rd, Rd_swiz_temp[0], Rd_swiz_temp[1], Rd_swiz_temp[2], Rd_swiz_temp[3]);
	}

	//------------do DSQ function---------------------------------------
	Rs_swiz_temp_FL[0] = *((float*)((void*)&Rs_swiz_temp[0]));
	Rd_swiz_temp_FL[0] = *((float*)((void*)&Rd_swiz_temp[0]));

	dsq_temp_FL = Rd_swiz_temp_FL[0]/sqrt(Rs_swiz_temp_FL[0]);

	dsq_temp = *((unsigned int*)((void*)&dsq_temp_FL));
	
	//------------ WB --------------------------------------------------
	if(IF.S_V==0 && IF.FXL==1)
	{
		if(IF.WM_x==1) {
			if(IF.Rd>=32)	VOB[IF.Rd-32][0] = dsq_temp;
			else			GPR[IF.Rd][0] = dsq_temp;
		}
		if(IF.WM_y==1) {
			if(IF.Rd>=32)	VOB[IF.Rd-32][1] = dsq_temp;
			else			GPR[IF.Rd][1] = dsq_temp;
		}
		if(IF.WM_z==1) {
			if(IF.Rd>=32)	VOB[IF.Rd-32][2] = dsq_temp;
			else			GPR[IF.Rd][2] = dsq_temp;
		}
		if(IF.WM_w==1) {
			if(IF.Rd>=32)	VOB[IF.Rd-32][3] = dsq_temp;
			else			GPR[IF.Rd][3] = dsq_temp;
		}
	}
	fprintf(fp_Result,"-----------------------------------------------------------------\n");
	if(IF.Rd>=32) {
		fprintf(fp_Result,"Rd:VOB_entry[%d] = %f_%f_%f_%f\n", IF.Rd-32, *(float*)&VOB[IF.Rd-32][0], *(float*)&VOB[IF.Rd-32][1], *(float*)&VOB[IF.Rd-32][2], *(float*)&VOB[IF.Rd-32][3]);
		fprintf(fp_Result,"Rd:VOB_entry[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd-32, VOB[IF.Rd-32][0], VOB[IF.Rd-32][1], VOB[IF.Rd-32][2], VOB[IF.Rd-32][3]);
	}
	else {
		fprintf(fp_Result,"Rd:GPR[%d] = %f_%f_%f_%f\n", IF.Rd, *(float*)&GPR[IF.Rd][0], *(float*)&GPR[IF.Rd][1], *(float*)&GPR[IF.Rd][2], *(float*)&GPR[IF.Rd][3]);
		fprintf(fp_Result,"Rd:GPR[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
	}
}

void SC_VS::VS_02_RCP()
{
	fprintf(fp_Result,"Op:2 = RCP\n");
	if(IF.Rd>=32)
		fprintf(fp_Result,"Rd:VOB_entry[%d]\n", IF.Rd-32);
	else
		fprintf(fp_Result,"Rd:GPR[%d]\n", IF.Rd);
	if(IF.Rs>=32)
		fprintf(fp_Result,"Rs:VAB_entry[%d]\n", IF.Rs-32);
	else
		fprintf(fp_Result,"Rs:GPR[%d]\n", IF.Rs);
	fprintf(fp_Result,"Rt:Dont care\n");
	fprintf(fp_Result,"Sw_d:Dont care\n");
	fprintf(fp_Result,"Sw_s:Dont care\n");
	fprintf(fp_Result,"Sw_t:Dont care\n");
	fprintf(fp_Result,"Neg:Dont care\n");
	fprintf(fp_Result,"WM:%d%d%d%d\n", IF.WM_x, IF.WM_y, IF.WM_z, IF.WM_w);
	fprintf(fp_Result,"FXL:%d\n", IF.FXL);
	fprintf(fp_Result,"S/V:%d\n", IF.S_V);
	if(IF.S_V!=0 || IF.FXL!=1)
		fprintf(fp_Result,"S/V must be 00 and FXL must be 1.\n");

	//------------Initial variable--------------------------------------
	rcp_temp = 0;

	//------------determine the source----------------------------------
	for(i=0;i<=3;i++)
		Rs_swiz_temp[i] = (IF.Rs>=32) ? VAB[IF.Rs-32][i] : GPR[IF.Rs][i];

	if(IF.Rs>=32) {
		fprintf(fp_Result,"Rs:VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rs-32, *(float*)&Rs_swiz_temp[0], *(float*)&Rs_swiz_temp[1], *(float*)&Rs_swiz_temp[2], *(float*)&Rs_swiz_temp[3]);
		fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
	}
	else {
		fprintf(fp_Result,"Rs:GPR[%d]:%f_%f_%f_%f\n", IF.Rs, *(float*)&Rs_swiz_temp[0], *(float*)&Rs_swiz_temp[1], *(float*)&Rs_swiz_temp[2], *(float*)&Rs_swiz_temp[3]);
		fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
	}

	//------------do RCP function---------------------------------------
	Rs_swiz_temp_FL[0] = *((float*)((void*)&Rs_swiz_temp[0]));

	rcp_temp_FL = 1/Rs_swiz_temp_FL[0];

	rcp_temp = *((unsigned int*)((void*)&rcp_temp_FL));
	
	//------------ WB --------------------------------------------------
	if(IF.S_V==0 && IF.FXL==1)
	{
		if(IF.WM_x==1) {
			if(IF.Rd>=32)	VOB[IF.Rd-32][0] = rcp_temp;
			else			GPR[IF.Rd][0] = rcp_temp;
		}
		if(IF.WM_y==1) {
			if(IF.Rd>=32)	VOB[IF.Rd-32][1] = rcp_temp;
			else			GPR[IF.Rd][1] = rcp_temp;
		}
		if(IF.WM_z==1) {
			if(IF.Rd>=32)	VOB[IF.Rd-32][2] = rcp_temp;
			else			GPR[IF.Rd][2] = rcp_temp;
		}
		if(IF.WM_w==1) {
			if(IF.Rd>=32)	VOB[IF.Rd-32][3] = rcp_temp;
			else			GPR[IF.Rd][3] = rcp_temp;
		}
	}
	fprintf(fp_Result,"-----------------------------------------------------------------\n");
	if(IF.Rd>=32) {
		fprintf(fp_Result,"Rd:VOB_entry[%d] = %f_%f_%f_%f\n", IF.Rd-32, *(float*)&VOB[IF.Rd-32][0], *(float*)&VOB[IF.Rd-32][1], *(float*)&VOB[IF.Rd-32][2], *(float*)&VOB[IF.Rd-32][3]);
		fprintf(fp_Result,"Rd:VOB_entry[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd-32, VOB[IF.Rd-32][0], VOB[IF.Rd-32][1], VOB[IF.Rd-32][2], VOB[IF.Rd-32][3]);
	}
	else {
		fprintf(fp_Result,"Rd:GPR[%d] = %f_%f_%f_%f\n", IF.Rd, *(float*)&GPR[IF.Rd][0], *(float*)&GPR[IF.Rd][1], *(float*)&GPR[IF.Rd][2], *(float*)&GPR[IF.Rd][3]);
		fprintf(fp_Result,"Rd:GPR[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
	}
}

void SC_VS::VS_03_RSQ()
{
	fprintf(fp_Result,"Op:3 = RSQ\n");
	if(IF.Rd>=32)
		fprintf(fp_Result,"Rd:VOB_entry[%d]\n", IF.Rd-32);
	else
		fprintf(fp_Result,"Rd:GPR[%d]\n", IF.Rd);
	if(IF.Rs>=32)
		fprintf(fp_Result,"Rs:VAB_entry[%d]\n", IF.Rs-32);
	else
		fprintf(fp_Result,"Rs:GPR[%d]\n", IF.Rs);
	fprintf(fp_Result,"Rt:Dont care\n");
	fprintf(fp_Result,"Sw_d:Dont care\n");
	fprintf(fp_Result,"Sw_s:Dont care\n");
	fprintf(fp_Result,"Sw_t:Dont care\n");
	fprintf(fp_Result,"Neg:Dont care\n");
	fprintf(fp_Result,"WM:%d%d%d%d\n", IF.WM_x, IF.WM_y, IF.WM_z, IF.WM_w);
	fprintf(fp_Result,"FXL:%d\n", IF.FXL);
	fprintf(fp_Result,"S/V:%d\n", IF.S_V);
	if(IF.S_V!=0 || IF.FXL!=1)
		fprintf(fp_Result,"S/V must be 00 and FXL must be 1.\n");

	//------------Initial variable--------------------------------------
	rsq_temp = 0;

	//------------determine the source----------------------------------
	for(i=0;i<=3;i++)
		Rs_swiz_temp[i] = (IF.Rs>=32) ? VAB[IF.Rs-32][i] : GPR[IF.Rs][i];

	if(IF.Rs>=32) {
		fprintf(fp_Result,"Rs:VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rs-32, *(float*)&Rs_swiz_temp[0], *(float*)&Rs_swiz_temp[1], *(float*)&Rs_swiz_temp[2], *(float*)&Rs_swiz_temp[3]);
		fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
	}
	else {
		fprintf(fp_Result,"Rs:GPR[%d]:%f_%f_%f_%f\n", IF.Rs, *(float*)&Rs_swiz_temp[0], *(float*)&Rs_swiz_temp[1], *(float*)&Rs_swiz_temp[2], *(float*)&Rs_swiz_temp[3]);
		fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
	}

	//------------do RSQ function---------------------------------------
	Rs_swiz_temp_FL[0] = *((float*)((void*)&Rs_swiz_temp[0]));

	rsq_temp_FL = 1/sqrt(Rs_swiz_temp_FL[0]);

	rsq_temp = *((unsigned int*)((void*)&rsq_temp_FL));
	
	//------------ WB --------------------------------------------------
	if(IF.S_V==0 && IF.FXL==1)
	{
		if(IF.WM_x==1) {
			if(IF.Rd>=32)	VOB[IF.Rd-32][0] = rsq_temp;
			else			GPR[IF.Rd][0] = rsq_temp;
		}
		if(IF.WM_y==1) {
			if(IF.Rd>=32)	VOB[IF.Rd-32][1] = rsq_temp;
			else			GPR[IF.Rd][1] = rsq_temp;
		}
		if(IF.WM_z==1) {
			if(IF.Rd>=32)	VOB[IF.Rd-32][2] = rsq_temp;
			else			GPR[IF.Rd][2] = rsq_temp;
		}
		if(IF.WM_w==1) {
			if(IF.Rd>=32)	VOB[IF.Rd-32][3] = rsq_temp;
			else			GPR[IF.Rd][3] = rsq_temp;
		}
	}
	fprintf(fp_Result,"-----------------------------------------------------------------\n");
	if(IF.Rd>=32) {
		fprintf(fp_Result,"Rd:VOB_entry[%d] = %f_%f_%f_%f\n", IF.Rd-32, *(float*)&VOB[IF.Rd-32][0], *(float*)&VOB[IF.Rd-32][1], *(float*)&VOB[IF.Rd-32][2], *(float*)&VOB[IF.Rd-32][3]);
		fprintf(fp_Result,"Rd:VOB_entry[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd-32, VOB[IF.Rd-32][0], VOB[IF.Rd-32][1], VOB[IF.Rd-32][2], VOB[IF.Rd-32][3]);
	}
	else {
		fprintf(fp_Result,"Rd:GPR[%d] = %f_%f_%f_%f\n", IF.Rd, *(float*)&GPR[IF.Rd][0], *(float*)&GPR[IF.Rd][1], *(float*)&GPR[IF.Rd][2], *(float*)&GPR[IF.Rd][3]);
		fprintf(fp_Result,"Rd:GPR[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
	}
}

void SC_VS::VS_04_LOG2()
{
	fprintf(fp_Result,"Op:4 = LOG2\n");
	if(IF.Rd>=32)
		fprintf(fp_Result,"Rd:VOB_entry[%d]\n", IF.Rd-32);
	else
		fprintf(fp_Result,"Rd:GPR[%d]\n", IF.Rd);
	if(IF.Rs>=32)
		fprintf(fp_Result,"Rs:VAB_entry[%d]\n", IF.Rs-32);
	else
		fprintf(fp_Result,"Rs:GPR[%d]\n", IF.Rs);
	fprintf(fp_Result,"Rt:Dont care\n");
	fprintf(fp_Result,"Sw_d:Dont care\n");
	fprintf(fp_Result,"Sw_s:Dont care\n");
	fprintf(fp_Result,"Sw_t:Dont care\n");
	fprintf(fp_Result,"Neg:Dont care\n");
	fprintf(fp_Result,"WM:%d%d%d%d\n", IF.WM_x, IF.WM_y, IF.WM_z, IF.WM_w);
	fprintf(fp_Result,"FXL:%d\n", IF.FXL);
	fprintf(fp_Result,"S/V:%d\n", IF.S_V);
	if(IF.S_V!=0 || IF.FXL!=1)
		fprintf(fp_Result,"S/V must be 00 and FXL must be 1.\n");

	//------------Initial variable--------------------------------------
	log2_temp = 0;

	//------------determine the source----------------------------------
	for(i=0;i<=3;i++)
		Rs_swiz_temp[i] = (IF.Rs>=32) ? VAB[IF.Rs-32][i] : GPR[IF.Rs][i];

	if(IF.Rs>=32) {
		fprintf(fp_Result,"Rs:VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rs-32, *(float*)&Rs_swiz_temp[0], *(float*)&Rs_swiz_temp[1], *(float*)&Rs_swiz_temp[2], *(float*)&Rs_swiz_temp[3]);
		fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
	}
	else {
		fprintf(fp_Result,"Rs:GPR[%d]:%f_%f_%f_%f\n", IF.Rs, *(float*)&Rs_swiz_temp[0], *(float*)&Rs_swiz_temp[1], *(float*)&Rs_swiz_temp[2], *(float*)&Rs_swiz_temp[3]);
		fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
	}

	//------------do LOG2 function---------------------------------------
	Rs_swiz_temp_FL[0] = *((float*)((void*)&Rs_swiz_temp[0]));

	log2_temp_FL = log(Rs_swiz_temp_FL[0])/logf(2);

	log2_temp = *((unsigned int*)((void*)&log2_temp_FL));
	
	//------------ WB --------------------------------------------------
	if(IF.S_V==0 && IF.FXL==1)
	{
		if(IF.WM_x==1) {
			if(IF.Rd>=32)	VOB[IF.Rd-32][0] = log2_temp;
			else			GPR[IF.Rd][0] = log2_temp;
		}
		if(IF.WM_y==1) {
			if(IF.Rd>=32)	VOB[IF.Rd-32][1] = log2_temp;
			else			GPR[IF.Rd][1] = log2_temp;
		}
		if(IF.WM_z==1) {
			if(IF.Rd>=32)	VOB[IF.Rd-32][2] = log2_temp;
			else			GPR[IF.Rd][2] = log2_temp;
		}
		if(IF.WM_w==1) {
			if(IF.Rd>=32)	VOB[IF.Rd-32][3] = log2_temp;
			else			GPR[IF.Rd][3] = log2_temp;
		}
	}
	fprintf(fp_Result,"-----------------------------------------------------------------\n");
	if(IF.Rd>=32) {
		fprintf(fp_Result,"Rd:VOB_entry[%d] = %f_%f_%f_%f\n", IF.Rd-32, *(float*)&VOB[IF.Rd-32][0], *(float*)&VOB[IF.Rd-32][1], *(float*)&VOB[IF.Rd-32][2], *(float*)&VOB[IF.Rd-32][3]);
		fprintf(fp_Result,"Rd:VOB_entry[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd-32, VOB[IF.Rd-32][0], VOB[IF.Rd-32][1], VOB[IF.Rd-32][2], VOB[IF.Rd-32][3]);
	}
	else {
		fprintf(fp_Result,"Rd:GPR[%d] = %f_%f_%f_%f\n", IF.Rd, *(float*)&GPR[IF.Rd][0], *(float*)&GPR[IF.Rd][1], *(float*)&GPR[IF.Rd][2], *(float*)&GPR[IF.Rd][3]);
		fprintf(fp_Result,"Rd:GPR[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
	}
}

void SC_VS::VS_05_POW2()
{
	fprintf(fp_Result,"Op:5 = POW2\n");
	if(IF.Rd>=32)
		fprintf(fp_Result,"Rd:VOB_entry[%d]\n", IF.Rd-32);
	else
		fprintf(fp_Result,"Rd:GPR[%d]\n", IF.Rd);
	if(IF.Rs>=32)
		fprintf(fp_Result,"Rs:VAB_entry[%d]\n", IF.Rs-32);
	else
		fprintf(fp_Result,"Rs:GPR[%d]\n", IF.Rs);
	fprintf(fp_Result,"Rt:Dont care\n");
	fprintf(fp_Result,"Sw_d:Dont care\n");
	fprintf(fp_Result,"Sw_s:Dont care\n");
	fprintf(fp_Result,"Sw_t:Dont care\n");
	fprintf(fp_Result,"Neg:Dont care\n");
	fprintf(fp_Result,"WM:%d%d%d%d\n", IF.WM_x, IF.WM_y, IF.WM_z, IF.WM_w);
	fprintf(fp_Result,"FXL:%d\n", IF.FXL);
	fprintf(fp_Result,"S/V:%d\n", IF.S_V);
	if(IF.S_V!=0 || IF.FXL!=1)
		fprintf(fp_Result,"S/V must be 00 and FXL must be 1.\n");

	//------------Initial variable--------------------------------------
	pow2_temp = 0;

	//------------determine the source----------------------------------
	for(i=0;i<=3;i++)
		Rs_swiz_temp[i] = (IF.Rs>=32) ? VAB[IF.Rs-32][i] : GPR[IF.Rs][i];

	if(IF.Rs>=32) {
		fprintf(fp_Result,"Rs:VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rs-32, *(float*)&Rs_swiz_temp[0], *(float*)&Rs_swiz_temp[1], *(float*)&Rs_swiz_temp[2], *(float*)&Rs_swiz_temp[3]);
		fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
	}
	else {
		fprintf(fp_Result,"Rs:GPR[%d]:%f_%f_%f_%f\n", IF.Rs, *(float*)&Rs_swiz_temp[0], *(float*)&Rs_swiz_temp[1], *(float*)&Rs_swiz_temp[2], *(float*)&Rs_swiz_temp[3]);
		fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
	}

	//------------do POW2 function---------------------------------------
	Rs_swiz_temp_FL[0] = *((float*)((void*)&Rs_swiz_temp[0]));

	pow2_temp_FL = pow(2, Rs_swiz_temp_FL[0]);

	pow2_temp = *((unsigned int*)((void*)&pow2_temp_FL));
	
	//------------ WB --------------------------------------------------
	if(IF.S_V==0 && IF.FXL==1)
	{
		if(IF.WM_x==1) {
			if(IF.Rd>=32)	VOB[IF.Rd-32][0] = pow2_temp;
			else			GPR[IF.Rd][0] = pow2_temp;
		}
		if(IF.WM_y==1) {
			if(IF.Rd>=32)	VOB[IF.Rd-32][1] = pow2_temp;
			else			GPR[IF.Rd][1] = pow2_temp;
		}
		if(IF.WM_z==1) {
			if(IF.Rd>=32)	VOB[IF.Rd-32][2] = pow2_temp;
			else			GPR[IF.Rd][2] = pow2_temp;
		}
		if(IF.WM_w==1) {
			if(IF.Rd>=32)	VOB[IF.Rd-32][3] = pow2_temp;
			else			GPR[IF.Rd][3] = pow2_temp;
		}
	}
	fprintf(fp_Result,"-----------------------------------------------------------------\n");
	if(IF.Rd>=32) {
		fprintf(fp_Result,"Rd:VOB_entry[%d] = %f_%f_%f_%f\n", IF.Rd-32, *(float*)&VOB[IF.Rd-32][0], *(float*)&VOB[IF.Rd-32][1], *(float*)&VOB[IF.Rd-32][2], *(float*)&VOB[IF.Rd-32][3]);
		fprintf(fp_Result,"Rd:VOB_entry[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd-32, VOB[IF.Rd-32][0], VOB[IF.Rd-32][1], VOB[IF.Rd-32][2], VOB[IF.Rd-32][3]);
	}
	else {
		fprintf(fp_Result,"Rd:GPR[%d] = %f_%f_%f_%f\n", IF.Rd, *(float*)&GPR[IF.Rd][0], *(float*)&GPR[IF.Rd][1], *(float*)&GPR[IF.Rd][2], *(float*)&GPR[IF.Rd][3]);
		fprintf(fp_Result,"Rd:GPR[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
	}
}

void SC_VS::VS_06_DIV()
{
	fprintf(fp_Result,"Op:6 = DIV\n");
	if(IF.Rd>=32)
		fprintf(fp_Result,"Rd:VOB_entry[%d]\n", IF.Rd-32);
	else
		fprintf(fp_Result,"Rd:GPR[%d]\n", IF.Rd);
	if(IF.Rs>=32)
		fprintf(fp_Result,"Rs:VAB_entry[%d]\n", IF.Rs-32);
	else
		fprintf(fp_Result,"Rs:GPR[%d]\n", IF.Rs);
	fprintf(fp_Result,"Rt:Dont care\n");
	fprintf(fp_Result,"Sw_d:Dont care\n");
	fprintf(fp_Result,"Sw_s:Dont care\n");
	fprintf(fp_Result,"Sw_t:Dont care\n");
	fprintf(fp_Result,"Neg:Dont care\n");
	fprintf(fp_Result,"WM:%d%d%d%d\n", IF.WM_x, IF.WM_y, IF.WM_z, IF.WM_w);
	fprintf(fp_Result,"FXL:%d\n", IF.FXL);
	fprintf(fp_Result,"S/V:%d\n", IF.S_V);
	if(IF.S_V!=0 || IF.FXL!=1)
		fprintf(fp_Result,"S/V must be 00 and FXL must be 1.\n");

	//------------Initial variable--------------------------------------
	div_temp = 0;

	//------------determine the source----------------------------------
	for(i=0;i<=3;i++)
	{
		Rs_swiz_temp[i] = (IF.Rs>=32) ? VAB[IF.Rs-32][i] : GPR[IF.Rs][i];
		Rd_swiz_temp[i] = (IF.Rd>=32) ? VAB[IF.Rd-32][i] : GPR[IF.Rd][i];
	}

	if(IF.Rs>=32) {
		fprintf(fp_Result,"Rs:VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rs-32, *(float*)&Rs_swiz_temp[0], *(float*)&Rs_swiz_temp[1], *(float*)&Rs_swiz_temp[2], *(float*)&Rs_swiz_temp[3]);
		fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
	}
	else {
		fprintf(fp_Result,"Rs:GPR[%d]:%f_%f_%f_%f\n", IF.Rs, *(float*)&Rs_swiz_temp[0], *(float*)&Rs_swiz_temp[1], *(float*)&Rs_swiz_temp[2], *(float*)&Rs_swiz_temp[3]);
		fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
	}
	if(IF.Rd>=32) {
		fprintf(fp_Result,"Rd:VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rd-32, *(float*)&Rd_swiz_temp[0], *(float*)&Rd_swiz_temp[1], *(float*)&Rd_swiz_temp[2], *(float*)&Rd_swiz_temp[3]);
		fprintf(fp_Result,"Rd:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rd-32, Rd_swiz_temp[0], Rd_swiz_temp[1], Rd_swiz_temp[2], Rd_swiz_temp[3]);
	}
	else {
		fprintf(fp_Result,"Rd:GPR[%d]:%f_%f_%f_%f\n", IF.Rd, *(float*)&Rd_swiz_temp[0], *(float*)&Rd_swiz_temp[1], *(float*)&Rd_swiz_temp[2], *(float*)&Rd_swiz_temp[3]);
		fprintf(fp_Result,"Rd:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rd, Rd_swiz_temp[0], Rd_swiz_temp[1], Rd_swiz_temp[2], Rd_swiz_temp[3]);
	}

	//------------do DIV function---------------------------------------
	Rs_swiz_temp_FL[0] = *((float*)((void*)&Rs_swiz_temp[0]));
	Rd_swiz_temp_FL[0] = *((float*)((void*)&Rd_swiz_temp[0]));

	div_temp_FL = Rd_swiz_temp_FL[0]/Rs_swiz_temp_FL[0] ;

	div_temp = *((unsigned int*)((void*)&div_temp_FL));
	
	//------------ WB --------------------------------------------------
	if(IF.S_V==0 && IF.FXL==1)
	{
		if(IF.WM_x==1) {
			if(IF.Rd>=32)	VOB[IF.Rd-32][0] = div_temp;
			else			GPR[IF.Rd][0] = div_temp;
		}
		if(IF.WM_y==1) {
			if(IF.Rd>=32)	VOB[IF.Rd-32][1] = div_temp;
			else			GPR[IF.Rd][1] = div_temp;
		}
		if(IF.WM_z==1) {
			if(IF.Rd>=32)	VOB[IF.Rd-32][2] = div_temp;
			else			GPR[IF.Rd][2] = div_temp;
		}
		if(IF.WM_w==1) {
			if(IF.Rd>=32)	VOB[IF.Rd-32][3] = div_temp;
			else			GPR[IF.Rd][3] = div_temp;
		}
	}
	fprintf(fp_Result,"-----------------------------------------------------------------\n");
	if(IF.Rd>=32) {
		fprintf(fp_Result,"Rd:VOB_entry[%d] = %f_%f_%f_%f\n", IF.Rd-32, *(float*)&VOB[IF.Rd-32][0], *(float*)&VOB[IF.Rd-32][1], *(float*)&VOB[IF.Rd-32][2], *(float*)&VOB[IF.Rd-32][3]);
		fprintf(fp_Result,"Rd:VOB_entry[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd-32, VOB[IF.Rd-32][0], VOB[IF.Rd-32][1], VOB[IF.Rd-32][2], VOB[IF.Rd-32][3]);
	}
	else {
		fprintf(fp_Result,"Rd:GPR[%d] = %f_%f_%f_%f\n", IF.Rd, *(float*)&GPR[IF.Rd][0], *(float*)&GPR[IF.Rd][1], *(float*)&GPR[IF.Rd][2], *(float*)&GPR[IF.Rd][3]);
		fprintf(fp_Result,"Rd:GPR[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
	}
}

void SC_VS::VS_07_MVR()
{
	fprintf(fp_Result,"Op:7 = MVR\n");
	if(IF.Rd>=32)
		fprintf(fp_Result,"Rd:VOB_entry[%d]\n", IF.Rd-32);
	else
		fprintf(fp_Result,"Rd:GPR[%d]\n", IF.Rd);
	if(IF.Rs>=32)
		fprintf(fp_Result,"Rs:VAB_entry[%d]\n", IF.Rs-32);
	else
		fprintf(fp_Result,"Rs:GPR[%d]\n", IF.Rs);
	fprintf(fp_Result,"Rt:Dont care\n");
	fprintf(fp_Result,"Sw_d:%d%d%d%d\n", IF.Sw_d_x, IF.Sw_d_y, IF.Sw_d_z, IF.Sw_d_w); 
	fprintf(fp_Result,"Sw_s:%d%d%d%d\n", IF.Sw_s_x, IF.Sw_s_y, IF.Sw_s_z, IF.Sw_s_w); 
	fprintf(fp_Result,"Sw_t:Dont care\n");
	fprintf(fp_Result,"Neg:%d_Dont care_Dont care\n", IF.Neg_Rs);
	fprintf(fp_Result,"WM:%d%d%d%d\n", IF.WM_x, IF.WM_y, IF.WM_z, IF.WM_w);
	fprintf(fp_Result,"FXL:%d\n", IF.FXL);
	fprintf(fp_Result,"S/V:%d\n", IF.S_V);

	//------------Initial variable--------------------------------------
	for(i=0;i<=3;i++)
	{
		mvr_temp[i] = 0;
		Rs_SV_temp[i] = 0;
	}	
	//S_V modified by TSAO
	//Load Rs
	Rs_SV_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0];
	Rs_SV_temp[1] = (IF.Rs>=32) ? VAB[IF.Rs-32][1] : GPR[IF.Rs][1];
	Rs_SV_temp[2] = (IF.Rs>=32) ? VAB[IF.Rs-32][2] : GPR[IF.Rs][2];
	Rs_SV_temp[3] = (IF.Rs>=32) ? VAB[IF.Rs-32][3] : GPR[IF.Rs][3];
	//----------------S/V---------------------------------
	for(i=0; i<=3; i++)
	{
		if(i>IF.S_V)
		{
			Rs_SV_temp[i] = 0;
		}
	}
	//Rs do swizzle
	switch(IF.Sw_s_x) {
	case 0:	Rs_swiz_temp[0] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[0] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[0] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[0] = Rs_SV_temp[3];break;
	}
	switch(IF.Sw_s_y) {
	case 0:	Rs_swiz_temp[1] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[1] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[1] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[1] = Rs_SV_temp[3];break;
	}
	switch(IF.Sw_s_z) {
	case 0:	Rs_swiz_temp[2] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[2] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[2] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[2] = Rs_SV_temp[3];break;
	}
	switch(IF.Sw_s_w) {
	case 0:	Rs_swiz_temp[3] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[3] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[3] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[3] = Rs_SV_temp[3];break;
	}
	//----------determine Rs Neg--------------------------
	if(IF.Neg_Rs==1)
	{
		for(i=0; i<=3; i++)
		{
			Rs_swiz_temp_FL[i] = *((float*)((void*)&Rs_swiz_temp[i]));
			Rs_swiz_temp_FL[i] = -Rs_swiz_temp_FL[i];
			Rs_swiz_temp[i] = *((unsigned int*)((void*)&Rs_swiz_temp_FL[i]));
		}
	}

	if(IF.Rs>=32) {
		fprintf(fp_Result,"Rs:VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rs-32, *(float*)&VAB[IF.Rs-32][0], *(float*)&VAB[IF.Rs-32][1], *(float*)&VAB[IF.Rs-32][2], *(float*)&VAB[IF.Rs-32][3]);
		fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, VAB[IF.Rs-32][0], VAB[IF.Rs-32][1], VAB[IF.Rs-32][2], VAB[IF.Rs-32][3]);
		fprintf(fp_Result,"After Swizzle/Neg/SV:\n");
		fprintf(fp_Result,"Rs:VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rs-32, *(float*)&Rs_swiz_temp[0], *(float*)&Rs_swiz_temp[1], *(float*)&Rs_swiz_temp[2], *(float*)&Rs_swiz_temp[3]);
		fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
	}
	else {
		fprintf(fp_Result,"Rs:GPR[%d]:%f_%f_%f_%f\n", IF.Rs, *(float*)&GPR[IF.Rs][0], *(float*)&GPR[IF.Rs][1], *(float*)&GPR[IF.Rs][2], *(float*)&GPR[IF.Rs][3]);
		fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs, GPR[IF.Rs][0], GPR[IF.Rs][1], GPR[IF.Rs][2], GPR[IF.Rs][3]);
		fprintf(fp_Result,"After Swizzle/Neg/SV:\n");
		fprintf(fp_Result,"Rs:GPR[%d]:%f_%f_%f_%f\n", IF.Rs-32, *(float*)&Rs_swiz_temp[0], *(float*)&Rs_swiz_temp[1], *(float*)&Rs_swiz_temp[2], *(float*)&Rs_swiz_temp[3]);
		fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
	}

	//----------MVR------------------------------------
	for(i=0;i<=3;i++)
		mvr_temp[i] = Rs_swiz_temp[i];
	
	//------------ WB --------------------------------------------------
	if(IF.WM_x==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][0] = mvr_temp[0];
		else			GPR[IF.Rd][0] = mvr_temp[0];
	}
	if(IF.WM_y==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][1] = mvr_temp[1];
		else			GPR[IF.Rd][1] = mvr_temp[1];
	}
	if(IF.WM_z==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][2] = mvr_temp[2];
		else			GPR[IF.Rd][2] = mvr_temp[2];
	}
	if(IF.WM_w==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][3] = mvr_temp[3];
		else			GPR[IF.Rd][3] = mvr_temp[3];
	}
	fprintf(fp_Result,"-----------------------------------------------------------------\n");
	if(IF.Rd>=32) {
		fprintf(fp_Result,"Rd:VOB_entry[%d] = %f_%f_%f_%f\n", IF.Rd-32, *(float*)&VOB[IF.Rd-32][0], *(float*)&VOB[IF.Rd-32][1], *(float*)&VOB[IF.Rd-32][2], *(float*)&VOB[IF.Rd-32][3]);
		fprintf(fp_Result,"Rd:VOB_entry[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd-32, VOB[IF.Rd-32][0], VOB[IF.Rd-32][1], VOB[IF.Rd-32][2], VOB[IF.Rd-32][3]);
	}
	else {
		fprintf(fp_Result,"Rd:GPR[%d] = %f_%f_%f_%f\n", IF.Rd, *(float*)&GPR[IF.Rd][0], *(float*)&GPR[IF.Rd][1], *(float*)&GPR[IF.Rd][2], *(float*)&GPR[IF.Rd][3]);
		fprintf(fp_Result,"Rd:GPR[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
	}
}

void SC_VS::VS_08_LDV()
{
	fprintf(fp_Result,"Op:8 = LDV\n");
	if(IF.Rd>=32)
		fprintf(fp_Result,"Rd:VOB_entry[%d]\n", IF.Rd-32);
	else
		fprintf(fp_Result,"Rd:GPR[%d]\n", IF.Rd);
	if(IF.Rs>=32)
		fprintf(fp_Result,"Rs:VAB_entry[%d]\n", IF.Rs-32);
	else
		fprintf(fp_Result,"Rs:GPR[%d]\n", IF.Rs);
	fprintf(fp_Result,"Imm:%d\n", IF.Imm);
	fprintf(fp_Result,"WM:Dont care\n");
	fprintf(fp_Result,"FXL:%d\n", IF.FXL);
	fprintf(fp_Result,"S/V:%d\n", IF.S_V);
	if(IF.S_V!=3)
		fprintf(fp_Result,"S/V must be 11.\n");
	if(IF.FXL!=0)
		fprintf(fp_Result,"FXL must be 0.\n");

	//------------Initial variable--------------------------------------
	for(i=0;i<=3;i++)
			ldv_temp[i] = 0;

	//------------determine the source----------------------------------
	for(i=0;i<=IF.S_V;i++)
		Rs_swiz_temp[i] = (IF.Rs>=32) ? VAB[IF.Rs-32][i] : GPR[IF.Rs][i];

	if(IF.Rs>=32) {
		fprintf(fp_Result,"Rs:VAB_entry[%d]:%d_%d_%d_%d\n", IF.Rs-32, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
		fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
	}
	else {
		fprintf(fp_Result,"Rs:GPR[%d]:%d_%d_%d_%d\n", IF.Rs, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
		fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
	}

	//------------Add Imm---------------------------------------
	Rs_swiz_temp_FX[0] = *((int*)((void*)&Rs_swiz_temp[0]));
	Imm_FX = *((int*)((void*)&IF.Imm));

	LD_addr = Rs_swiz_temp_FX[0] + Imm_FX;

	if(LD_addr < 0)
		fprintf(fp_Result, "Error Address:%d\n", LD_addr);
	else
	{
		for(i=0;i<=3;i++)
			ldv_temp[i] = CMEM[LD_addr][i];
		fprintf(fp_Result,"CMEM[%d] = %.8x_%.8x_%.8x_%.8x\n", LD_addr, CMEM[LD_addr][0], CMEM[LD_addr][1], CMEM[LD_addr][2], CMEM[LD_addr][3]);
	}

	//------------ WB --------------------------------------------------
	if(IF.WM_x==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][0] = ldv_temp[0];
		else			GPR[IF.Rd][0] = ldv_temp[0];
	}
	if(IF.WM_y==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][1] = ldv_temp[1];
		else			GPR[IF.Rd][1] = ldv_temp[1];
	}
	if(IF.WM_z==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][2] = ldv_temp[2];
		else			GPR[IF.Rd][2] = ldv_temp[2];
	}
	if(IF.WM_w==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][3] = ldv_temp[3];
		else			GPR[IF.Rd][3] = ldv_temp[3];
	}
	fprintf(fp_Result,"-----------------------------------------------------------------\n");
	if(IF.Rd>=32)
		fprintf(fp_Result,"Rd:VOB_entry[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd-32, VOB[IF.Rd-32][0], VOB[IF.Rd-32][1], VOB[IF.Rd-32][2], VOB[IF.Rd-32][3]);
	else
		fprintf(fp_Result,"Rd:GPR[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
}

void SC_VS::VS_09_STV()
{
	fprintf(fp_Result,"Op:9 = STV\n");
	if(IF.Rd>=32)
		fprintf(fp_Result,"Rd:VOB_entry[%d]\n", IF.Rd-32);
	else
		fprintf(fp_Result,"Rd:GPR[%d]\n", IF.Rd);
	if(IF.Rs>=32)
		fprintf(fp_Result,"Rs:VAB_entry[%d]\n", IF.Rs-32);
	else
		fprintf(fp_Result,"Rs:GPR[%d]\n", IF.Rs);
	fprintf(fp_Result,"Imm:%d\n", IF.Imm);
	fprintf(fp_Result,"WM:%d%d%d%d\n", IF.WM_x, IF.WM_y, IF.WM_z, IF.WM_w);
	fprintf(fp_Result,"FXL:%d\n", IF.FXL);
	fprintf(fp_Result,"S/V:%d\n", IF.S_V);
	if(IF.FXL!=0)
		fprintf(fp_Result,"FXL must be 0.\n");

	//------------Initial variable--------------------------------------
	for(i=0;i<=3;i++)
			stv_temp[i] = 0;

	//------------determine the source----------------------------------
	for(i=0;i<=IF.S_V;i++)
	{
		Rs_swiz_temp[i] = (IF.Rs>=32) ? VAB[IF.Rs-32][i] : GPR[IF.Rs][i];
		Rd_swiz_temp[i] = (IF.Rd>=32) ? VAB[IF.Rd-32][i] : GPR[IF.Rd][i];
	}

	if(IF.Rs>=32)
		fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, VAB[IF.Rs-32][0], VAB[IF.Rs-32][1], VAB[IF.Rs-32][2], VAB[IF.Rs-32][3]);
	else
		fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs, GPR[IF.Rs][0], GPR[IF.Rs][1], GPR[IF.Rs][2], GPR[IF.Rs][3]);

	if(IF.Rd>=32) {
		fprintf(fp_Result,"Rd:VAB_entry[%d]:%d_%d_%d_%d\n", IF.Rd-32, Rd_swiz_temp[0], Rd_swiz_temp[1], Rd_swiz_temp[2], Rd_swiz_temp[3]);
		fprintf(fp_Result,"Rd:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rd-32, Rd_swiz_temp[0], Rd_swiz_temp[1], Rd_swiz_temp[2], Rd_swiz_temp[3]);
	}
	else {
		fprintf(fp_Result,"Rd:GPR[%d]:%d_%d_%d_%d\n", IF.Rd, Rd_swiz_temp[0], Rd_swiz_temp[1], Rd_swiz_temp[2], Rd_swiz_temp[3]);
		fprintf(fp_Result,"Rd:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rd, Rd_swiz_temp[0], Rd_swiz_temp[1], Rd_swiz_temp[2], Rd_swiz_temp[3]);
	}

	//------------Add Imm---------------------------------------
	Rd_swiz_temp_FX[0] = *((int*)((void*)&Rd_swiz_temp[0]));
	Imm_FX = *((int*)((void*)&IF.Imm));

	ST_addr = Rd_swiz_temp_FX[0] + Imm_FX;

	if(ST_addr<0)
		fprintf(fp_Result, "Error Address:%d\n", ST_addr);
	else
	{
		for(i=0;i<=3;i++)
			stv_temp[i] = Rs_swiz_temp[i];

	//------------ WB --------------------------------------------------
		if(IF.WM_x==1)
			CMEM[ST_addr][0] = stv_temp[0];
		if(IF.WM_y==1) 
			CMEM[ST_addr][1] = stv_temp[1];
		if(IF.WM_z==1)
			CMEM[ST_addr][2] = stv_temp[2];
		if(IF.WM_w==1)
			CMEM[ST_addr][3] = stv_temp[3];
		fprintf(fp_Result,"-----------------------------------------------------------------\n");
		fprintf(fp_Result,"CMEM[%d] = %.8x_%.8x_%.8x_%.8x\n", ST_addr, CMEM[ST_addr][0], CMEM[ST_addr][1], CMEM[ST_addr][2], CMEM[ST_addr][3]);
	}
}

void SC_VS::VS_10_MVI()
{
	fprintf(fp_Result,"Op:10 = MVI\n");
	if(IF.Rd>=32)
		fprintf(fp_Result,"Rd:VOB_entry[%d]\n", IF.Rd-32);
	else
		fprintf(fp_Result,"Rd:GPR[%d]\n", IF.Rd);
	if(IF.Rs>=32)
		fprintf(fp_Result,"Rs:VAB_entry[%d]\n", IF.Rs-32);
	else
		fprintf(fp_Result,"Rs:GPR[%d]\n", IF.Rs);
	//------------------------------------------------------------------
	if(IF.FXL==0)
		fprintf(fp_Result,"Imm:%d\n", IF.Imm);
	else
		fprintf(fp_Result,"Imm:%f\n", *(float*)&IF.Imm);
	
	//------------------------------------------------------------------
	fprintf(fp_Result,"WM:%d%d%d%d\n", IF.WM_x, IF.WM_y, IF.WM_z, IF.WM_w);
	fprintf(fp_Result,"FXL:%d\n", IF.FXL);
	fprintf(fp_Result,"S/V:%d\n", IF.S_V);

	//------------Initial variable--------------------------------------
	for(i=0;i<=3;i++)
	{
		mvi_temp[i] = 0;
		Rs_swiz_temp[i] = 0;
	}

	//------------determine the source----------------------------------
	for(i=0;i<=IF.S_V;i++)
		Rs_swiz_temp[i] = (IF.Rs>=32) ? VAB[IF.Rs-32][i] : GPR[IF.Rs][i];

	if(IF.Rs>=32) {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%d_%d_%d_%d\n", IF.Rs-32, VAB[IF.Rs-32][0], VAB[IF.Rs-32][1], VAB[IF.Rs-32][2], VAB[IF.Rs-32][3]);
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, VAB[IF.Rs-32][0], VAB[IF.Rs-32][1], VAB[IF.Rs-32][2], VAB[IF.Rs-32][3]);
		}
		else {
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rs-32, *(float*)&VAB[IF.Rs-32][0], *(float*)&VAB[IF.Rs-32][1], *(float*)&VAB[IF.Rs-32][2], *(float*)&VAB[IF.Rs-32][3]);
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, VAB[IF.Rs-32][0], VAB[IF.Rs-32][1], VAB[IF.Rs-32][2], VAB[IF.Rs-32][3]);
		}
	}
	else {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rs:GPR[%d]:%d_%d_%d_%d\n", IF.Rs, GPR[IF.Rs][0], GPR[IF.Rs][1], GPR[IF.Rs][2], GPR[IF.Rs][3]);
			fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs, GPR[IF.Rs][0], GPR[IF.Rs][1], GPR[IF.Rs][2], GPR[IF.Rs][3]);
		}
		else {
			fprintf(fp_Result,"Rs:GPR[%d]:%f_%f_%f_%f\n", IF.Rs, *(float*)&GPR[IF.Rs][0], *(float*)&GPR[IF.Rs][1], *(float*)&GPR[IF.Rs][2], *(float*)&GPR[IF.Rs][3]);
			fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs, GPR[IF.Rs][0], GPR[IF.Rs][1], GPR[IF.Rs][2], GPR[IF.Rs][3]);
		}
	}

	//------------Add Imm---------------------------------------
	if(IF.FXL==0)
	{
		for(i=0;i<=3;i++)
		{
			Rs_swiz_temp_FX[i] = *((int*)((void*)&Rs_swiz_temp[i]));
			Imm_FX = *((int*)((void*)&IF.Imm));
			mvi_temp_FX[i] = Rs_swiz_temp_FX[i] + Imm_FX;
			mvi_temp[i] = *((unsigned int*)((void*)&mvi_temp_FX[i]));
		}
	}
	else
	{
		for(i=0;i<=3;i++)
		{
			Rs_swiz_temp_FL[i] = *((float*)((void*)&Rs_swiz_temp[i]));
			Imm_FL = *((float*)((void*)&IF.Imm));
			mvi_temp_FL[i] = Rs_swiz_temp_FL[i] + Imm_FL;
			mvi_temp[i] = *((unsigned int*)((void*)&mvi_temp_FL[i]));
		}
	}

	//------------ WB --------------------------------------------------
	if(IF.WM_x==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][0] = mvi_temp[0];
		else			GPR[IF.Rd][0] = mvi_temp[0];
	}
	if(IF.WM_y==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][1] = mvi_temp[1];
		else			GPR[IF.Rd][1] = mvi_temp[1];
	}
	if(IF.WM_z==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][2] = mvi_temp[2];
		else			GPR[IF.Rd][2] = mvi_temp[2];
	}
	if(IF.WM_w==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][3] = mvi_temp[3];
		else			GPR[IF.Rd][3] = mvi_temp[3];
	}
	fprintf(fp_Result,"-----------------------------------------------------------------\n");
	if(IF.Rd>=32)
	{
		if(IF.FXL==0)
		{
			fprintf(fp_Result,"Rd:VOB_entry[%d] = %d_%d_%d_%d\n", IF.Rd-32, VOB[IF.Rd-32][0], VOB[IF.Rd-32][1], VOB[IF.Rd-32][2], VOB[IF.Rd-32][3]);
			fprintf(fp_Result,"Rd:VOB_entry[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd-32, VOB[IF.Rd-32][0], VOB[IF.Rd-32][1], VOB[IF.Rd-32][2], VOB[IF.Rd-32][3]);
		}
		else
		{
			fprintf(fp_Result,"Rd:VOB_entry[%d] = %f_%f_%f_%f\n", IF.Rd-32, *(float*)&VOB[IF.Rd-32][0], *(float*)&VOB[IF.Rd-32][1], *(float*)&VOB[IF.Rd-32][2], *(float*)&VOB[IF.Rd-32][3]);
			fprintf(fp_Result,"Rd:VOB_entry[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd-32, VOB[IF.Rd-32][0], VOB[IF.Rd-32][1], VOB[IF.Rd-32][2], VOB[IF.Rd-32][3]);
		}
	}
	else
	{
		if(IF.FXL==0)
		{
			fprintf(fp_Result,"Rd:GPR[%d] = %d_%d_%d_%d\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
			fprintf(fp_Result,"Rd:GPR[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
		}
		else
		{
			fprintf(fp_Result,"Rd:GPR[%d] = %f_%f_%f_%f\n", IF.Rd, *(float*)&GPR[IF.Rd][0], *(float*)&GPR[IF.Rd][1], *(float*)&GPR[IF.Rd][2], *(float*)&GPR[IF.Rd][3]);
			fprintf(fp_Result,"Rd:GPR[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
		}
	}
}

void SC_VS::VS_11_MIN()
{
	fprintf(fp_Result,"Op:11 = MIN\n");
	if(IF.Rd>=32)		fprintf(fp_Result,"Rd:VOB_entry[%d]\n", IF.Rd-32);
	else				fprintf(fp_Result,"Rd:GPR[%d]\n", IF.Rd);
	if(IF.Rs>=32)		fprintf(fp_Result,"Rs:VAB_entry[%d]\n", IF.Rs-32);
	else				fprintf(fp_Result,"Rs:GPR[%d]\n", IF.Rs);
	if(IF.Rt>=32)		fprintf(fp_Result,"Rt:VAB_entry[%d]\n", IF.Rt-32);
	else				fprintf(fp_Result,"Rt:GPR[%d]\n", IF.Rt);
	fprintf(fp_Result,"Sw_d:%d%d%d%d\n", IF.Sw_d_x, IF.Sw_d_y, IF.Sw_d_z, IF.Sw_d_w); 
	fprintf(fp_Result,"Sw_s:%d%d%d%d\n", IF.Sw_s_x, IF.Sw_s_y, IF.Sw_s_z, IF.Sw_s_w); 
	fprintf(fp_Result,"Sw_t:%d%d%d%d\n", IF.Sw_t_x, IF.Sw_t_y, IF.Sw_t_z, IF.Sw_t_w); 
	fprintf(fp_Result,"Neg:Dont care\n");
	fprintf(fp_Result,"WM:%d%d%d%d\n", IF.WM_x, IF.WM_y, IF.WM_z, IF.WM_w);
	fprintf(fp_Result,"FXL:%d\n", IF.FXL);
	fprintf(fp_Result,"S/V:%d\n", IF.S_V);

	//------------Initial variable--------------------------------------
	for(i=0;i<=3;i++)
	{
		min_temp[i] = 0;
		Rs_SV_temp[i] = 0;
		Rt_SV_temp[i] = 0;
		Rs_swiz_temp[i] = 0;
		Rt_swiz_temp[i] = 0;

	}
	//S_V modified by TSAO
	//Load Rs
	Rs_SV_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0];
	Rs_SV_temp[1] = (IF.Rs>=32) ? VAB[IF.Rs-32][1] : GPR[IF.Rs][1];
	Rs_SV_temp[2] = (IF.Rs>=32) ? VAB[IF.Rs-32][2] : GPR[IF.Rs][2];
	Rs_SV_temp[3] = (IF.Rs>=32) ? VAB[IF.Rs-32][3] : GPR[IF.Rs][3];
	//Load Rt
	Rt_SV_temp[0] = (IF.Rt>=32) ? VAB[IF.Rt-32][0] : GPR[IF.Rt][0];
	Rt_SV_temp[1] = (IF.Rt>=32) ? VAB[IF.Rt-32][1] : GPR[IF.Rt][1];
	Rt_SV_temp[2] = (IF.Rt>=32) ? VAB[IF.Rt-32][2] : GPR[IF.Rt][2];
	Rt_SV_temp[3] = (IF.Rt>=32) ? VAB[IF.Rt-32][3] : GPR[IF.Rt][3];
	//----------------S/V---------------------------------
	for(i=0; i<=3; i++)
	{
		if(i>IF.S_V)
		{
			Rs_SV_temp[i] = 0;
			Rt_SV_temp[i] = 0;
		}
	}
	//Rs do swizzle
	switch(IF.Sw_s_x) {
	case 0:	Rs_swiz_temp[0] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[0] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[0] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[0] = Rs_SV_temp[3];break;
	}
	switch(IF.Sw_s_y) {
	case 0:	Rs_swiz_temp[1] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[1] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[1] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[1] = Rs_SV_temp[3];break;
	}
	switch(IF.Sw_s_z) {
	case 0:	Rs_swiz_temp[2] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[2] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[2] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[2] = Rs_SV_temp[3];break;
	}
	switch(IF.Sw_s_w) {
	case 0:	Rs_swiz_temp[3] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[3] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[3] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[3] = Rs_SV_temp[3];break;
	}
	
	//Rt do swizzle
	switch(IF.Sw_t_x) {
	case 0:	Rt_swiz_temp[0] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[0] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[0] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[0] = Rt_SV_temp[3];break;
	}
	switch(IF.Sw_t_y) {
	case 0:	Rt_swiz_temp[1] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[1] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[1] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[1] = Rt_SV_temp[3];break;
	}
	switch(IF.Sw_t_z) {
	case 0:	Rt_swiz_temp[2] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[2] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[2] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[2] = Rt_SV_temp[3];break;
	}
	switch(IF.Sw_t_w) {
	case 0:	Rt_swiz_temp[3] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[3] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[3] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[3] = Rt_SV_temp[3];break;
	}
	
	// Before Swizzle
	if(IF.Rs>=32) {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%d_%d_%d_%d\n", IF.Rs-32, VAB[IF.Rs-32][0], VAB[IF.Rs-32][1], VAB[IF.Rs-32][2], VAB[IF.Rs-32][3]);
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, VAB[IF.Rs-32][0], VAB[IF.Rs-32][1], VAB[IF.Rs-32][2], VAB[IF.Rs-32][3]);
		}
		else {
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rs-32, *(float*)&VAB[IF.Rs-32][0], *(float*)&VAB[IF.Rs-32][1], *(float*)&VAB[IF.Rs-32][2], *(float*)&VAB[IF.Rs-32][3]);
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, VAB[IF.Rs-32][0], VAB[IF.Rs-32][1], VAB[IF.Rs-32][2], VAB[IF.Rs-32][3]);
		}
	}
	else {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rs:GPR[%d]:%d_%d_%d_%d\n", IF.Rs, GPR[IF.Rs][0], GPR[IF.Rs][1], GPR[IF.Rs][2], GPR[IF.Rs][3]);
			fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs, GPR[IF.Rs][0], GPR[IF.Rs][1], GPR[IF.Rs][2], GPR[IF.Rs][3]);
		}
		else {
			fprintf(fp_Result,"Rs:GPR[%d]:%f_%f_%f_%f\n", IF.Rs, *(float*)&GPR[IF.Rs][0], *(float*)&GPR[IF.Rs][1], *(float*)&GPR[IF.Rs][2], *(float*)&GPR[IF.Rs][3]);
			fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs, GPR[IF.Rs][0], GPR[IF.Rs][1], GPR[IF.Rs][2], GPR[IF.Rs][3]);
		}
	}
	if(IF.Rt>=32) {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%d_%d_%d_%d\n", IF.Rt-32, VAB[IF.Rt-32][0], VAB[IF.Rt-32][1], VAB[IF.Rt-32][2], VAB[IF.Rt-32][3]);
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt-32, VAB[IF.Rt-32][0], VAB[IF.Rt-32][1], VAB[IF.Rt-32][2], VAB[IF.Rt-32][3]);
		}
		else {
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rt-32, *(float*)&VAB[IF.Rt-32][0], *(float*)&VAB[IF.Rt-32][1], *(float*)&VAB[IF.Rt-32][2], *(float*)&VAB[IF.Rt-32][3]);
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt-32, VAB[IF.Rt-32][0], VAB[IF.Rt-32][1], VAB[IF.Rt-32][2], VAB[IF.Rt-32][3]);
		}
	}
	else {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rt:GPR[%d]:%d_%d_%d_%d\n", IF.Rt, GPR[IF.Rt][0], GPR[IF.Rt][1], GPR[IF.Rt][2], GPR[IF.Rt][3]);
			fprintf(fp_Result,"Rt:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt, GPR[IF.Rt][0], GPR[IF.Rt][1], GPR[IF.Rt][2], GPR[IF.Rt][3]);
		}
		else {
			fprintf(fp_Result,"Rt:GPR[%d]:%f_%f_%f_%f\n", IF.Rt, *(float*)&GPR[IF.Rt][0], *(float*)&GPR[IF.Rt][1], *(float*)&GPR[IF.Rt][2], *(float*)&GPR[IF.Rt][3]);
			fprintf(fp_Result,"Rt:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt, GPR[IF.Rt][0], GPR[IF.Rt][1], GPR[IF.Rt][2], GPR[IF.Rt][3]);
		}		
	}

	// After Swizzle
	fprintf(fp_Result, "After Swizzle/SV:\n");
	if(IF.Rs>=32) {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%d_%d_%d_%d\n", IF.Rs-32, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
		}
		else {
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rs-32, *(float*)&Rs_swiz_temp[0], *(float*)&Rs_swiz_temp[1], *(float*)&Rs_swiz_temp[2], *(float*)&Rs_swiz_temp[3]);
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
		}
	}
	else {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rs:GPR[%d]:%d_%d_%d_%d\n", IF.Rs,Rs_swiz_temp[0],Rs_swiz_temp[1],Rs_swiz_temp[2],Rs_swiz_temp[3]);
			fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs,Rs_swiz_temp[0],Rs_swiz_temp[1],Rs_swiz_temp[2],Rs_swiz_temp[3]);
		}
		else {
			fprintf(fp_Result,"Rs:GPR[%d]:%f_%f_%f_%f\n", IF.Rs, *(float*)&Rs_swiz_temp[0], *(float*)&Rs_swiz_temp[1], *(float*)&Rs_swiz_temp[2], *(float*)&Rs_swiz_temp[3]);
			fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs,Rs_swiz_temp[0],Rs_swiz_temp[1],Rs_swiz_temp[2],Rs_swiz_temp[3]);
		}
	}
	if(IF.Rt>=32) {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%d_%d_%d_%d\n", IF.Rt-32, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt-32, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
		}
		else {
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rt-32, *(float*)&Rt_swiz_temp[0], *(float*)&Rt_swiz_temp[1], *(float*)&Rt_swiz_temp[2], *(float*)&Rt_swiz_temp[3]);
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt-32, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
		}
	}
	else {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rt:GPR[%d]:%d_%d_%d_%d\n", IF.Rt, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
			fprintf(fp_Result,"Rt:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
		}
		else {
			fprintf(fp_Result,"Rt:GPR[%d]:%f_%f_%f_%f\n", IF.Rt, *(float*)&Rt_swiz_temp[0], *(float*)&Rt_swiz_temp[1], *(float*)&Rt_swiz_temp[2], *(float*)&Rt_swiz_temp[3]);
			fprintf(fp_Result,"Rt:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
		}
	}

	//------------Min Function---------------------------------------
	if(IF.FXL==0)
	{
		for(i=0;i<=3;i++)
		{
			Rs_swiz_temp_FX[i] = *((int*)((void*)&Rs_swiz_temp[i]));
			Rt_swiz_temp_FX[i] = *((int*)((void*)&Rt_swiz_temp[i]));
			min_temp_FX[i] = MIN(Rs_swiz_temp_FX[i], Rt_swiz_temp_FX[i]);
			min_temp[i] = *((unsigned int*)((void*)&min_temp_FX[i]));
		}
	}
	else
	{
		for(i=0;i<=3;i++)
		{
			Rs_swiz_temp_FL[i] = *((float*)((void*)&Rs_swiz_temp[i]));
			Rt_swiz_temp_FL[i] = *((float*)((void*)&Rt_swiz_temp[i]));
			min_temp_FL[i] = MIN(Rs_swiz_temp_FL[i], Rt_swiz_temp_FL[i]);
			min_temp[i] = *((unsigned int*)((void*)&min_temp_FL[i]));
		}
	}

	//------------ WB --------------------------------------------------
	if(IF.WM_x==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][0] = min_temp[0];
		else			GPR[IF.Rd][0] = min_temp[0];
	}
	if(IF.WM_y==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][1] = min_temp[1];
		else			GPR[IF.Rd][1] = min_temp[1];
	}
	if(IF.WM_z==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][2] = min_temp[2];
		else			GPR[IF.Rd][2] = min_temp[2];
	}
	if(IF.WM_w==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][3] = min_temp[3];
		else			GPR[IF.Rd][3] = min_temp[3];
	}
	fprintf(fp_Result,"-----------------------------------------------------------------\n");
	if(IF.Rd>=32)
	{
		if(IF.FXL==0)
		{
			fprintf(fp_Result,"Rd:VOB_entry[%d] = %d_%d_%d_%d\n", IF.Rd-32, VOB[IF.Rd-32][0], VOB[IF.Rd-32][1], VOB[IF.Rd-32][2], VOB[IF.Rd-32][3]);
			fprintf(fp_Result,"Rd:VOB_entry[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd-32, VOB[IF.Rd-32][0], VOB[IF.Rd-32][1], VOB[IF.Rd-32][2], VOB[IF.Rd-32][3]);
		}
		else
		{
			fprintf(fp_Result,"Rd:VOB_entry[%d] = %f_%f_%f_%f\n", IF.Rd-32, *(float*)&VOB[IF.Rd-32][0], *(float*)&VOB[IF.Rd-32][1], *(float*)&VOB[IF.Rd-32][2], *(float*)&VOB[IF.Rd-32][3]);
			fprintf(fp_Result,"Rd:VOB_entry[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd-32, VOB[IF.Rd-32][0], VOB[IF.Rd-32][1], VOB[IF.Rd-32][2], VOB[IF.Rd-32][3]);
		}
	}
	else
	{
		if(IF.FXL==0)
		{
			fprintf(fp_Result,"Rd:GPR[%d] = %d_%d_%d_%d\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
			fprintf(fp_Result,"Rd:GPR[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
		}
		else
		{
			fprintf(fp_Result,"Rd:GPR[%d] = %f_%f_%f_%f\n", IF.Rd, *(float*)&GPR[IF.Rd][0], *(float*)&GPR[IF.Rd][1], *(float*)&GPR[IF.Rd][2], *(float*)&GPR[IF.Rd][3]);
			fprintf(fp_Result,"Rd:GPR[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
		}
	}
}

void SC_VS::VS_12_MAX()
{
	fprintf(fp_Result,"Op:12 = MAX\n");
	if(IF.Rd>=32)		fprintf(fp_Result,"Rd:VOB_entry[%d]\n", IF.Rd-32);
	else				fprintf(fp_Result,"Rd:GPR[%d]\n", IF.Rd);
	if(IF.Rs>=32)		fprintf(fp_Result,"Rs:VAB_entry[%d]\n", IF.Rs-32);
	else				fprintf(fp_Result,"Rs:GPR[%d]\n", IF.Rs);
	if(IF.Rt>=32)		fprintf(fp_Result,"Rt:VAB_entry[%d]\n", IF.Rt-32);
	else				fprintf(fp_Result,"Rt:GPR[%d]\n", IF.Rt);
	fprintf(fp_Result,"Sw_d:%d%d%d%d\n", IF.Sw_d_x, IF.Sw_d_y, IF.Sw_d_z, IF.Sw_d_w); 
	fprintf(fp_Result,"Sw_s:%d%d%d%d\n", IF.Sw_s_x, IF.Sw_s_y, IF.Sw_s_z, IF.Sw_s_w); 
	fprintf(fp_Result,"Sw_t:%d%d%d%d\n", IF.Sw_t_x, IF.Sw_t_y, IF.Sw_t_z, IF.Sw_t_w); 
	fprintf(fp_Result,"Neg:Dont care\n");
	fprintf(fp_Result,"WM:%d%d%d%d\n", IF.WM_x, IF.WM_y, IF.WM_z, IF.WM_w);
	fprintf(fp_Result,"FXL:%d\n", IF.FXL);
	fprintf(fp_Result,"S/V:%d\n", IF.S_V);

	//------------Initial variable--------------------------------------
	for(i=0;i<=3;i++)
	{
		max_temp[i] = 0;
		Rs_SV_temp[i] = 0;
		Rt_SV_temp[i] = 0;
		Rs_swiz_temp[i] = 0;
		Rt_swiz_temp[i] = 0;

	}
	//S_V modified by TSAO
	//Load Rs
	Rs_SV_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0];
	Rs_SV_temp[1] = (IF.Rs>=32) ? VAB[IF.Rs-32][1] : GPR[IF.Rs][1];
	Rs_SV_temp[2] = (IF.Rs>=32) ? VAB[IF.Rs-32][2] : GPR[IF.Rs][2];
	Rs_SV_temp[3] = (IF.Rs>=32) ? VAB[IF.Rs-32][3] : GPR[IF.Rs][3];
	//Load Rt
	Rt_SV_temp[0] = (IF.Rt>=32) ? VAB[IF.Rt-32][0] : GPR[IF.Rt][0];
	Rt_SV_temp[1] = (IF.Rt>=32) ? VAB[IF.Rt-32][1] : GPR[IF.Rt][1];
	Rt_SV_temp[2] = (IF.Rt>=32) ? VAB[IF.Rt-32][2] : GPR[IF.Rt][2];
	Rt_SV_temp[3] = (IF.Rt>=32) ? VAB[IF.Rt-32][3] : GPR[IF.Rt][3];
	//----------------S/V---------------------------------
	for(i=0; i<=3; i++)
	{
		if(i>IF.S_V)
		{
			Rs_SV_temp[i] = 0;
			Rt_SV_temp[i] = 0;
		}
	}
	//Rs do swizzle
	switch(IF.Sw_s_x) {
	case 0:	Rs_swiz_temp[0] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[0] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[0] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[0] = Rs_SV_temp[3];break;
	}
	switch(IF.Sw_s_y) {
	case 0:	Rs_swiz_temp[1] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[1] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[1] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[1] = Rs_SV_temp[3];break;
	}
	switch(IF.Sw_s_z) {
	case 0:	Rs_swiz_temp[2] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[2] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[2] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[2] = Rs_SV_temp[3];break;
	}
	switch(IF.Sw_s_w) {
	case 0:	Rs_swiz_temp[3] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[3] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[3] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[3] = Rs_SV_temp[3];break;
	}
	
	//Rt do swizzle
	switch(IF.Sw_t_x) {
	case 0:	Rt_swiz_temp[0] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[0] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[0] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[0] = Rt_SV_temp[3];break;
	}
	switch(IF.Sw_t_y) {
	case 0:	Rt_swiz_temp[1] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[1] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[1] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[1] = Rt_SV_temp[3];break;
	}
	switch(IF.Sw_t_z) {
	case 0:	Rt_swiz_temp[2] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[2] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[2] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[2] = Rt_SV_temp[3];break;
	}
	switch(IF.Sw_t_w) {
	case 0:	Rt_swiz_temp[3] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[3] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[3] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[3] = Rt_SV_temp[3];break;
	}

	if(IF.Rs>=32) {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%d_%d_%d_%d\n", IF.Rs-32, VAB[IF.Rs-32][0], VAB[IF.Rs-32][1], VAB[IF.Rs-32][2], VAB[IF.Rs-32][3]);
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, VAB[IF.Rs-32][0], VAB[IF.Rs-32][1], VAB[IF.Rs-32][2], VAB[IF.Rs-32][3]);
		}
		else {
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rs-32, *(float*)&VAB[IF.Rs-32][0], *(float*)&VAB[IF.Rs-32][1], *(float*)&VAB[IF.Rs-32][2], *(float*)&VAB[IF.Rs-32][3]);
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, VAB[IF.Rs-32][0], VAB[IF.Rs-32][1], VAB[IF.Rs-32][2], VAB[IF.Rs-32][3]);
		}
	}
	else {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rs:GPR[%d]:%d_%d_%d_%d\n", IF.Rs, GPR[IF.Rs][0], GPR[IF.Rs][1], GPR[IF.Rs][2], GPR[IF.Rs][3]);
			fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs, GPR[IF.Rs][0], GPR[IF.Rs][1], GPR[IF.Rs][2], GPR[IF.Rs][3]);
		}
		else {
			fprintf(fp_Result,"Rs:GPR[%d]:%f_%f_%f_%f\n", IF.Rs, *(float*)&GPR[IF.Rs][0], *(float*)&GPR[IF.Rs][1], *(float*)&GPR[IF.Rs][2], *(float*)&GPR[IF.Rs][3]);
			fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs, GPR[IF.Rs][0], GPR[IF.Rs][1], GPR[IF.Rs][2], GPR[IF.Rs][3]);
		}
	}
	if(IF.Rt>=32) {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%d_%d_%d_%d\n", IF.Rt-32, VAB[IF.Rt-32][0], VAB[IF.Rt-32][1], VAB[IF.Rt-32][2], VAB[IF.Rt-32][3]);
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt-32, VAB[IF.Rt-32][0], VAB[IF.Rt-32][1], VAB[IF.Rt-32][2], VAB[IF.Rt-32][3]);
		}
		else {
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rt-32, *(float*)&VAB[IF.Rt-32][0], *(float*)&VAB[IF.Rt-32][1], *(float*)&VAB[IF.Rt-32][2], *(float*)&VAB[IF.Rt-32][3]);
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt-32, VAB[IF.Rt-32][0], VAB[IF.Rt-32][1], VAB[IF.Rt-32][2], VAB[IF.Rt-32][3]);
		}
	}
	else {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rt:GPR[%d]:%d_%d_%d_%d\n", IF.Rt, GPR[IF.Rt][0], GPR[IF.Rt][1], GPR[IF.Rt][2], GPR[IF.Rt][3]);
			fprintf(fp_Result,"Rt:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt, GPR[IF.Rt][0], GPR[IF.Rt][1], GPR[IF.Rt][2], GPR[IF.Rt][3]);
		}
		else {
			fprintf(fp_Result,"Rt:GPR[%d]:%f_%f_%f_%f\n", IF.Rt, *(float*)&GPR[IF.Rt][0], *(float*)&GPR[IF.Rt][1], *(float*)&GPR[IF.Rt][2], *(float*)&GPR[IF.Rt][3]);
			fprintf(fp_Result,"Rt:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt, GPR[IF.Rt][0], GPR[IF.Rt][1], GPR[IF.Rt][2], GPR[IF.Rt][3]);
		}		
	}

	fprintf(fp_Result, "After Swizzle/SV:\n");
	if(IF.Rs>=32) {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%d_%d_%d_%d\n", IF.Rs-32, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
		}
		else {
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rs-32, *(float*)&Rs_swiz_temp[0], *(float*)&Rs_swiz_temp[1], *(float*)&Rs_swiz_temp[2], *(float*)&Rs_swiz_temp[3]);
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
		}
	}
	else {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rs:GPR[%d]:%d_%d_%d_%d\n", IF.Rs,Rs_swiz_temp[0],Rs_swiz_temp[1],Rs_swiz_temp[2],Rs_swiz_temp[3]);
			fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs,Rs_swiz_temp[0],Rs_swiz_temp[1],Rs_swiz_temp[2],Rs_swiz_temp[3]);
		}
		else {
			fprintf(fp_Result,"Rs:GPR[%d]:%f_%f_%f_%f\n", IF.Rs, *(float*)&Rs_swiz_temp[0], *(float*)&Rs_swiz_temp[1], *(float*)&Rs_swiz_temp[2], *(float*)&Rs_swiz_temp[3]);
			fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs,Rs_swiz_temp[0],Rs_swiz_temp[1],Rs_swiz_temp[2],Rs_swiz_temp[3]);
		}
	}
	if(IF.Rt>=32) {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%d_%d_%d_%d\n", IF.Rt-32, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt-32, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
		}
		else {
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rt-32, *(float*)&Rt_swiz_temp[0], *(float*)&Rt_swiz_temp[1], *(float*)&Rt_swiz_temp[2], *(float*)&Rt_swiz_temp[3]);
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt-32, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
		}
	}
	else {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rt:GPR[%d]:%d_%d_%d_%d\n", IF.Rt, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
			fprintf(fp_Result,"Rt:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
		}
		else {
			fprintf(fp_Result,"Rt:GPR[%d]:%f_%f_%f_%f\n", IF.Rt, *(float*)&Rt_swiz_temp[0], *(float*)&Rt_swiz_temp[1], *(float*)&Rt_swiz_temp[2], *(float*)&Rt_swiz_temp[3]);
			fprintf(fp_Result,"Rt:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
		}
	}

	//------------Max Function---------------------------------------
	if(IF.FXL==0)
	{
		for(i=0;i<=3;i++)
		{
			Rs_swiz_temp_FX[i] = *((int*)((void*)&Rs_swiz_temp[i]));
			Rt_swiz_temp_FX[i] = *((int*)((void*)&Rt_swiz_temp[i]));
			max_temp_FX[i] = MAX(Rs_swiz_temp_FX[i], Rt_swiz_temp_FX[i]);
			max_temp[i] = *((unsigned int*)((void*)&max_temp_FX[i]));
		}
	}
	else
	{
		for(i=0;i<=3;i++)
		{
			Rs_swiz_temp_FL[i] = *((float*)((void*)&Rs_swiz_temp[i]));
			Rt_swiz_temp_FL[i] = *((float*)((void*)&Rt_swiz_temp[i]));
			max_temp_FL[i] = MAX(Rs_swiz_temp_FL[i], Rt_swiz_temp_FL[i]);
			max_temp[i] = *((unsigned int*)((void*)&max_temp_FL[i]));
		}
	}

	//------------ WB --------------------------------------------------
	if(IF.WM_x==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][0] = max_temp[0];
		else			GPR[IF.Rd][0] = max_temp[0];
	}
	if(IF.WM_y==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][1] = max_temp[1];
		else			GPR[IF.Rd][1] = max_temp[1];
	}
	if(IF.WM_z==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][2] = max_temp[2];
		else			GPR[IF.Rd][2] = max_temp[2];
	}
	if(IF.WM_w==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][3] = max_temp[3];
		else			GPR[IF.Rd][3] = max_temp[3];
	}
	fprintf(fp_Result,"-----------------------------------------------------------------\n");
	if(IF.Rd>=32)
	{
		if(IF.FXL==0)
		{
			fprintf(fp_Result,"Rd:VOB_entry[%d] = %d_%d_%d_%d\n", IF.Rd-32, VOB[IF.Rd-32][0], VOB[IF.Rd-32][1], VOB[IF.Rd-32][2], VOB[IF.Rd-32][3]);
			fprintf(fp_Result,"Rd:VOB_entry[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd-32, VOB[IF.Rd-32][0], VOB[IF.Rd-32][1], VOB[IF.Rd-32][2], VOB[IF.Rd-32][3]);
		}
		else
		{
			fprintf(fp_Result,"Rd:VOB_entry[%d] = %f_%f_%f_%f\n", IF.Rd-32, *(float*)&VOB[IF.Rd-32][0], *(float*)&VOB[IF.Rd-32][1], *(float*)&VOB[IF.Rd-32][2], *(float*)&VOB[IF.Rd-32][3]);
			fprintf(fp_Result,"Rd:VOB_entry[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd-32, VOB[IF.Rd-32][0], VOB[IF.Rd-32][1], VOB[IF.Rd-32][2], VOB[IF.Rd-32][3]);
		}
	}
	else
	{
		if(IF.FXL==0)
		{
			fprintf(fp_Result,"Rd:GPR[%d] = %d_%d_%d_%d\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
			fprintf(fp_Result,"Rd:GPR[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
		}
		else
		{
			fprintf(fp_Result,"Rd:GPR[%d] = %f_%f_%f_%f\n", IF.Rd, *(float*)&GPR[IF.Rd][0], *(float*)&GPR[IF.Rd][1], *(float*)&GPR[IF.Rd][2], *(float*)&GPR[IF.Rd][3]);
			fprintf(fp_Result,"Rd:GPR[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
		}
	}
}

void SC_VS::VS_13_ITOF_FTOI()
{
	fprintf(fp_Result,"Op:13 = ITOF or FTOI\n");
	if(IF.Rd>=32)		fprintf(fp_Result,"Rd:VOB_entry[%d]\n", IF.Rd-32);
	else				fprintf(fp_Result,"Rd:GPR[%d]\n", IF.Rd);
	if(IF.Rs>=32)		fprintf(fp_Result,"Rs:VAB_entry[%d]\n", IF.Rs-32);
	else				fprintf(fp_Result,"Rs:GPR[%d]\n", IF.Rs);
	fprintf(fp_Result,"WM:%d%d%d%d\n", IF.WM_x, IF.WM_y, IF.WM_z, IF.WM_w);
	fprintf(fp_Result,"FXL:%d\n", IF.FXL);
	fprintf(fp_Result,"S/V:%d\n", IF.S_V);

	//------------Initial variable--------------------------------------
	for(i=0;i<=3;i++)
	{
		itof_ftoi_temp[i] = 0;
		Rs_swiz_temp[i] = 0;
	}

	//------------determine the source----------------------------------
	for(i=0;i<=IF.S_V;i++)
		Rs_swiz_temp[i] = (IF.Rs>=32) ? VAB[IF.Rs-32][i] : GPR[IF.Rs][i];

	if(IF.Rs>=32) {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%d_%d_%d_%d\n", IF.Rs-32, VAB[IF.Rs-32][0], VAB[IF.Rs-32][1], VAB[IF.Rs-32][2], VAB[IF.Rs-32][3]);
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, VAB[IF.Rs-32][0], VAB[IF.Rs-32][1], VAB[IF.Rs-32][2], VAB[IF.Rs-32][3]);
		}
		else {
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rs-32, *(float*)&VAB[IF.Rs-32][0], *(float*)&VAB[IF.Rs-32][1], *(float*)&VAB[IF.Rs-32][2], *(float*)&VAB[IF.Rs-32][3]);
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, VAB[IF.Rs-32][0], VAB[IF.Rs-32][1], VAB[IF.Rs-32][2], VAB[IF.Rs-32][3]);
		}
	}
	else {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rs:GPR[%d]:%d_%d_%d_%d\n", IF.Rs, GPR[IF.Rs][0], GPR[IF.Rs][1], GPR[IF.Rs][2], GPR[IF.Rs][3]);
			fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs, GPR[IF.Rs][0], GPR[IF.Rs][1], GPR[IF.Rs][2], GPR[IF.Rs][3]);
		}
		else {
			fprintf(fp_Result,"Rs:GPR[%d]:%f_%f_%f_%f\n", IF.Rs, *(float*)&GPR[IF.Rs][0], *(float*)&GPR[IF.Rs][1], *(float*)&GPR[IF.Rs][2], *(float*)&GPR[IF.Rs][3]);
			fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs, GPR[IF.Rs][0], GPR[IF.Rs][1], GPR[IF.Rs][2], GPR[IF.Rs][3]);
		}
	}

	//------------ITOF FTOI Function---------------------------------------
	if(IF.FXL==0) // ITOF
	{
		for(i=0;i<=3;i++)
		{
			Rs_swiz_temp_FX[i] =  *((int*)((void*)&Rs_swiz_temp[i]));
			itof_temp_FL[i] = (float)Rs_swiz_temp_FX[i];
			itof_ftoi_temp[i] = *((unsigned int*)((void*)&itof_temp_FL[i]));
		}
	}
	else          // FTOI
	{
		for(i=0;i<=3;i++)
		{
			Rs_swiz_temp_FL[i] =  *((float*)((void*)&Rs_swiz_temp[i]));
			ftoi_temp_FX[i] = (int)Rs_swiz_temp_FL[i];
			itof_ftoi_temp[i] = *((unsigned int*)((void*)&ftoi_temp_FX[i]));
		}
	}

	//------------ WB --------------------------------------------------
	if(IF.WM_x==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][0] = itof_ftoi_temp[0];
		else			GPR[IF.Rd][0] = itof_ftoi_temp[0];
	}
	if(IF.WM_y==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][1] = itof_ftoi_temp[1];
		else			GPR[IF.Rd][1] = itof_ftoi_temp[1];
	}
	if(IF.WM_z==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][2] = itof_ftoi_temp[2];
		else			GPR[IF.Rd][2] = itof_ftoi_temp[2];
	}
	if(IF.WM_w==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][3] = itof_ftoi_temp[3];
		else			GPR[IF.Rd][3] = itof_ftoi_temp[3];
	}
	fprintf(fp_Result,"-----------------------------------------------------------------\n");
	if(IF.Rd>=32)
	{
		if(IF.FXL==0)  // ITOF
		{
			fprintf(fp_Result,"Rd:VOB_entry[%d] = %f_%f_%f_%f\n", IF.Rd-32, *(float*)&VOB[IF.Rd-32][0], *(float*)&VOB[IF.Rd-32][1], *(float*)&VOB[IF.Rd-32][2], *(float*)&VOB[IF.Rd-32][3]);
			fprintf(fp_Result,"Rd:VOB_entry[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd-32, VOB[IF.Rd-32][0], VOB[IF.Rd-32][1], VOB[IF.Rd-32][2], VOB[IF.Rd-32][3]);
		}
		else           // FTOI
		{
			fprintf(fp_Result,"Rd:VOB_entry[%d] = %d_%d_%d_%d\n", IF.Rd-32, VOB[IF.Rd-32][0], VOB[IF.Rd-32][1], VOB[IF.Rd-32][2], VOB[IF.Rd-32][3]);
			fprintf(fp_Result,"Rd:VOB_entry[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd-32, VOB[IF.Rd-32][0], VOB[IF.Rd-32][1], VOB[IF.Rd-32][2], VOB[IF.Rd-32][3]);
		}
	}
	else
	{
		if(IF.FXL==0)  // ITOF
		{
			fprintf(fp_Result,"Rd:GPR[%d] = %f_%f_%f_%f\n", IF.Rd, *(float*)&GPR[IF.Rd][0], *(float*)&GPR[IF.Rd][1], *(float*)&GPR[IF.Rd][2], *(float*)&GPR[IF.Rd][3]);
			fprintf(fp_Result,"Rd:GPR[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
		}
		else           // FTOI
		{
			fprintf(fp_Result,"Rd:GPR[%d] = %d_%d_%d_%d\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
			fprintf(fp_Result,"Rd:GPR[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
		}
	}
}

void SC_VS::VS_14_FIN()
{
	fprintf(fp_Result,"Op:14 = FIN\n");
	fprintf(fp_Result, "\n########## Vertex:%d Finish##########\n",v_num+1);
}

void SC_VS::VS_16_ADD()
{
	fprintf(fp_Result,"Op:16 = ADD\n");
	if(IF.Rd>=32)		fprintf(fp_Result,"Rd:VOB_entry[%d]\n", IF.Rd-32);
	else				fprintf(fp_Result,"Rd:GPR[%d]\n", IF.Rd);
	if(IF.Rs>=32)		fprintf(fp_Result,"Rs:VAB_entry[%d]\n", IF.Rs-32);
	else				fprintf(fp_Result,"Rs:GPR[%d]\n", IF.Rs);
	if(IF.Rt>=32)		fprintf(fp_Result,"Rt:VAB_entry[%d]\n", IF.Rt-32);
	else				fprintf(fp_Result,"Rt:GPR[%d]\n", IF.Rt);
	fprintf(fp_Result,"Sw_d:Dont care.\n"); 
	fprintf(fp_Result,"Sw_s:%d%d%d%d\n", IF.Sw_s_x, IF.Sw_s_y, IF.Sw_s_z, IF.Sw_s_w); 
	fprintf(fp_Result,"Sw_t:%d%d%d%d\n", IF.Sw_t_x, IF.Sw_t_y, IF.Sw_t_z, IF.Sw_t_w); 
	fprintf(fp_Result,"Neg:%d%d%d\n", IF.Neg_Rs, IF.Neg_Rt, IF.Neg_Rd);
	fprintf(fp_Result,"WM:%d%d%d%d\n", IF.WM_x, IF.WM_y, IF.WM_z, IF.WM_w);
	fprintf(fp_Result,"FXL:%d\n", IF.FXL);
	fprintf(fp_Result,"S/V:%d\n", IF.S_V);

	//------------Initial variable--------------------------------------
	for(i=0;i<=3;i++)
	{
		add_temp[i] = 0;
		Rs_SV_temp[i] = 0;
		Rt_SV_temp[i] = 0;
		Rs_swiz_temp[i] = 0;
		Rt_swiz_temp[i] = 0;

	}
	//S_V modified by TSAO
	//Load Rs
	Rs_SV_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0];
	Rs_SV_temp[1] = (IF.Rs>=32) ? VAB[IF.Rs-32][1] : GPR[IF.Rs][1];
	Rs_SV_temp[2] = (IF.Rs>=32) ? VAB[IF.Rs-32][2] : GPR[IF.Rs][2];
	Rs_SV_temp[3] = (IF.Rs>=32) ? VAB[IF.Rs-32][3] : GPR[IF.Rs][3];
	//Load Rt
	Rt_SV_temp[0] = (IF.Rt>=32) ? VAB[IF.Rt-32][0] : GPR[IF.Rt][0];
	Rt_SV_temp[1] = (IF.Rt>=32) ? VAB[IF.Rt-32][1] : GPR[IF.Rt][1];
	Rt_SV_temp[2] = (IF.Rt>=32) ? VAB[IF.Rt-32][2] : GPR[IF.Rt][2];
	Rt_SV_temp[3] = (IF.Rt>=32) ? VAB[IF.Rt-32][3] : GPR[IF.Rt][3];
	//----------------S/V---------------------------------
	for(i=0; i<=3; i++)
	{
		if(i>IF.S_V)
		{
			Rs_SV_temp[i] = 0;
			Rt_SV_temp[i] = 0;
		}
	}
	//Rs do swizzle
	switch(IF.Sw_s_x) {
	case 0:	Rs_swiz_temp[0] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[0] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[0] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[0] = Rs_SV_temp[3];break;
	}
	switch(IF.Sw_s_y) {
	case 0:	Rs_swiz_temp[1] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[1] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[1] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[1] = Rs_SV_temp[3];break;
	}
	switch(IF.Sw_s_z) {
	case 0:	Rs_swiz_temp[2] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[2] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[2] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[2] = Rs_SV_temp[3];break;
	}
	switch(IF.Sw_s_w) {
	case 0:	Rs_swiz_temp[3] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[3] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[3] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[3] = Rs_SV_temp[3];break;
	}
	
	//Rt do swizzle
	switch(IF.Sw_t_x) {
	case 0:	Rt_swiz_temp[0] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[0] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[0] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[0] = Rt_SV_temp[3];break;
	}
	switch(IF.Sw_t_y) {
	case 0:	Rt_swiz_temp[1] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[1] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[1] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[1] = Rt_SV_temp[3];break;
	}
	switch(IF.Sw_t_z) {
	case 0:	Rt_swiz_temp[2] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[2] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[2] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[2] = Rt_SV_temp[3];break;
	}
	switch(IF.Sw_t_w) {
	case 0:	Rt_swiz_temp[3] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[3] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[3] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[3] = Rt_SV_temp[3];break;
	}

	//------------Neg---------------------------------------
	if(IF.Neg_Rs==1)
	{
		if(IF.FXL==0)
		{
			for(i=0;i<=3;i++)
			{
				Rs_swiz_temp_FX[i] = -(*((int*)((void*)&Rs_swiz_temp[i])));
				Rs_swiz_temp[i] = *((unsigned int*)((void*)&Rs_swiz_temp_FX[i]));
			}
		}
		else
		{
			for(i=0;i<=3;i++)
			{
				Rs_swiz_temp_FL[i] = -(*((float*)((void*)&Rs_swiz_temp[i])));
				Rs_swiz_temp[i] = *((unsigned int*)((void*)&Rs_swiz_temp_FL[i]));
			}
		}
	}

	if(IF.Neg_Rt==1)
	{
		if(IF.FXL==0)
		{
			for(i=0;i<=3;i++)
			{
				Rt_swiz_temp_FX[i] = -(*((int*)((void*)&Rt_swiz_temp[i])));
				Rt_swiz_temp[i] = *((unsigned int*)((void*)&Rt_swiz_temp_FX[i]));
			}
		}
		else
		{
			for(i=0;i<=3;i++)
			{
				Rt_swiz_temp_FL[i] = -(*((float*)((void*)&Rt_swiz_temp[i])));
				Rt_swiz_temp[i] = *((unsigned int*)((void*)&Rt_swiz_temp_FL[i]));
			}
		}
	}
	//----------------S/V---------------------------------
	for(i=0; i<=3; i++)
	{
		if(i>IF.S_V)
		{
			Rs_swiz_temp[i] = 0;
			Rt_swiz_temp[i] = 0;
		}
	}

	if(IF.Rs>=32) {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%d_%d_%d_%d\n", IF.Rs-32, VAB[IF.Rs-32][0], VAB[IF.Rs-32][1], VAB[IF.Rs-32][2], VAB[IF.Rs-32][3]);
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, VAB[IF.Rs-32][0], VAB[IF.Rs-32][1], VAB[IF.Rs-32][2], VAB[IF.Rs-32][3]);
		}
		else {
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rs-32, *(float*)&VAB[IF.Rs-32][0], *(float*)&VAB[IF.Rs-32][1], *(float*)&VAB[IF.Rs-32][2], *(float*)&VAB[IF.Rs-32][3]);
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, VAB[IF.Rs-32][0], VAB[IF.Rs-32][1], VAB[IF.Rs-32][2], VAB[IF.Rs-32][3]);
		}
	}
	else {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rs:GPR[%d]:%d_%d_%d_%d\n", IF.Rs, GPR[IF.Rs][0], GPR[IF.Rs][1], GPR[IF.Rs][2], GPR[IF.Rs][3]);
			fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs, GPR[IF.Rs][0], GPR[IF.Rs][1], GPR[IF.Rs][2], GPR[IF.Rs][3]);
		}
		else {
			fprintf(fp_Result,"Rs:GPR[%d]:%f_%f_%f_%f\n", IF.Rs, *(float*)&GPR[IF.Rs][0], *(float*)&GPR[IF.Rs][1], *(float*)&GPR[IF.Rs][2], *(float*)&GPR[IF.Rs][3]);
			fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs, GPR[IF.Rs][0], GPR[IF.Rs][1], GPR[IF.Rs][2], GPR[IF.Rs][3]);
		}
	}
	if(IF.Rt>=32) {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%d_%d_%d_%d\n", IF.Rt-32, VAB[IF.Rt-32][0], VAB[IF.Rt-32][1], VAB[IF.Rt-32][2], VAB[IF.Rt-32][3]);
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt-32, VAB[IF.Rt-32][0], VAB[IF.Rt-32][1], VAB[IF.Rt-32][2], VAB[IF.Rt-32][3]);
		}
		else {
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rt-32, *(float*)&VAB[IF.Rt-32][0], *(float*)&VAB[IF.Rt-32][1], *(float*)&VAB[IF.Rt-32][2], *(float*)&VAB[IF.Rt-32][3]);
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt-32, VAB[IF.Rt-32][0], VAB[IF.Rt-32][1], VAB[IF.Rt-32][2], VAB[IF.Rt-32][3]);
		}
	}
	else {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rt:GPR[%d]:%d_%d_%d_%d\n", IF.Rt, GPR[IF.Rt][0], GPR[IF.Rt][1], GPR[IF.Rt][2], GPR[IF.Rt][3]);
			fprintf(fp_Result,"Rt:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt, GPR[IF.Rt][0], GPR[IF.Rt][1], GPR[IF.Rt][2], GPR[IF.Rt][3]);
		}
		else {
			fprintf(fp_Result,"Rt:GPR[%d]:%f_%f_%f_%f\n", IF.Rt, *(float*)&GPR[IF.Rt][0], *(float*)&GPR[IF.Rt][1], *(float*)&GPR[IF.Rt][2], *(float*)&GPR[IF.Rt][3]);
			fprintf(fp_Result,"Rt:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt, GPR[IF.Rt][0], GPR[IF.Rt][1], GPR[IF.Rt][2], GPR[IF.Rt][3]);
		}		
	}

	fprintf(fp_Result, "After Swizzle/Neg/SV:\n");
	if(IF.Rs>=32) {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%d_%d_%d_%d\n", IF.Rs-32, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
		}
		else {
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rs-32, *(float*)&Rs_swiz_temp[0], *(float*)&Rs_swiz_temp[1], *(float*)&Rs_swiz_temp[2], *(float*)&Rs_swiz_temp[3]);
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
		}
	}
	else {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rs:GPR[%d]:%d_%d_%d_%d\n", IF.Rs,Rs_swiz_temp[0],Rs_swiz_temp[1],Rs_swiz_temp[2],Rs_swiz_temp[3]);
			fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs,Rs_swiz_temp[0],Rs_swiz_temp[1],Rs_swiz_temp[2],Rs_swiz_temp[3]);
		}
		else {
			fprintf(fp_Result,"Rs:GPR[%d]:%f_%f_%f_%f\n", IF.Rs, *(float*)&Rs_swiz_temp[0], *(float*)&Rs_swiz_temp[1], *(float*)&Rs_swiz_temp[2], *(float*)&Rs_swiz_temp[3]);
			fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs,Rs_swiz_temp[0],Rs_swiz_temp[1],Rs_swiz_temp[2],Rs_swiz_temp[3]);
		}
	}
	if(IF.Rt>=32) {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%d_%d_%d_%d\n", IF.Rt-32, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt-32, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
		}
		else {
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rt-32, *(float*)&Rt_swiz_temp[0], *(float*)&Rt_swiz_temp[1], *(float*)&Rt_swiz_temp[2], *(float*)&Rt_swiz_temp[3]);
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt-32, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
		}
	}
	else {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rt:GPR[%d]:%d_%d_%d_%d\n", IF.Rt, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
			fprintf(fp_Result,"Rt:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
		}
		else {
			fprintf(fp_Result,"Rt:GPR[%d]:%f_%f_%f_%f\n", IF.Rt, *(float*)&Rt_swiz_temp[0], *(float*)&Rt_swiz_temp[1], *(float*)&Rt_swiz_temp[2], *(float*)&Rt_swiz_temp[3]);
			fprintf(fp_Result,"Rt:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
		}
	}
	/*
	//------------Neg---------------------------------------
	if(IF.Neg_Rs==1)
	{
		if(IF.FXL==0)
		{
			for(i=0;i<=3;i++)
				Rs_swiz_temp_FX[i] = -(*((int*)((void*)&Rs_swiz_temp[i])));
		}
		else
		{
			for(i=0;i<=3;i++)
				Rs_swiz_temp_FL[i] = -(*((float*)((void*)&Rs_swiz_temp[i])));
		}
	}
	else
	{
		if(IF.FXL==0)
		{
			for(i=0;i<=3;i++)
				Rs_swiz_temp_FX[i] = *((int*)((void*)&Rs_swiz_temp[i]));
		}
		else
		{
			for(i=0;i<=3;i++)
				Rs_swiz_temp_FL[i] = *((float*)((void*)&Rs_swiz_temp[i]));
		}
	}

	if(IF.Neg_Rt==1)
	{
		if(IF.FXL==0)
		{
			for(i=0;i<=3;i++)
				Rt_swiz_temp_FX[i] = -(*((int*)((void*)&Rt_swiz_temp[i])));
		}
		else
		{
			for(i=0;i<=3;i++)
				Rt_swiz_temp_FL[i] = -(*((float*)((void*)&Rt_swiz_temp[i])));
		}
	}
	else
	{
		if(IF.FXL==0)
		{
			for(i=0;i<=3;i++)
				Rt_swiz_temp_FX[i] = *((int*)((void*)&Rt_swiz_temp[i]));
		}
		else
		{
			for(i=0;i<=3;i++)
				Rt_swiz_temp_FL[i] = *((float*)((void*)&Rt_swiz_temp[i]));
		}
	}
	*/
	//------------ADD Function---------------------------------------
	if(IF.FXL==0)
	{
		for(i=0;i<=3;i++)
		{
			Rs_swiz_temp_FX[i] = *((int*)((void*)&Rs_swiz_temp[i]));
			Rt_swiz_temp_FX[i] = *((int*)((void*)&Rt_swiz_temp[i]));
			add_temp_FX[i] = Rs_swiz_temp_FX[i] + Rt_swiz_temp_FX[i];
			add_temp[i] = *((unsigned int*)((void*)&add_temp_FX[i]));
		}
	}
	else
	{
		for(i=0;i<=3;i++)
		{
			Rs_swiz_temp_FL[i] = *((float*)((void*)&Rs_swiz_temp[i]));
			Rt_swiz_temp_FL[i] = *((float*)((void*)&Rt_swiz_temp[i]));
			add_temp_FL[i] = Rs_swiz_temp_FL[i] + Rt_swiz_temp_FL[i];
			add_temp[i] = *((unsigned int*)((void*)&add_temp_FL[i]));
		}
	}

	//------------ WB --------------------------------------------------
	if(IF.WM_x==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][0] = add_temp[0];
		else			GPR[IF.Rd][0] = add_temp[0];
	}
	if(IF.WM_y==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][1] = add_temp[1];
		else			GPR[IF.Rd][1] = add_temp[1];
	}
	if(IF.WM_z==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][2] = add_temp[2];
		else			GPR[IF.Rd][2] = add_temp[2];
	}
	if(IF.WM_w==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][3] = add_temp[3];
		else			GPR[IF.Rd][3] = add_temp[3];
	}
	fprintf(fp_Result,"-----------------------------------------------------------------\n");
	if(IF.Rd>=32)
	{
		if(IF.FXL==0)
		{
			fprintf(fp_Result,"Rd:VOB_entry[%d] = %d_%d_%d_%d\n", IF.Rd-32, VOB[IF.Rd-32][0], VOB[IF.Rd-32][1], VOB[IF.Rd-32][2], VOB[IF.Rd-32][3]);
			fprintf(fp_Result,"Rd:VOB_entry[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd-32, VOB[IF.Rd-32][0], VOB[IF.Rd-32][1], VOB[IF.Rd-32][2], VOB[IF.Rd-32][3]);
		}
		else
		{
			fprintf(fp_Result,"Rd:VOB_entry[%d] = %f_%f_%f_%f\n", IF.Rd-32, *(float*)&VOB[IF.Rd-32][0], *(float*)&VOB[IF.Rd-32][1], *(float*)&VOB[IF.Rd-32][2], *(float*)&VOB[IF.Rd-32][3]);
			fprintf(fp_Result,"Rd:VOB_entry[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd-32, VOB[IF.Rd-32][0], VOB[IF.Rd-32][1], VOB[IF.Rd-32][2], VOB[IF.Rd-32][3]);
		}
	}
	else
	{
		if(IF.FXL==0)
		{
			fprintf(fp_Result,"Rd:GPR[%d] = %d_%d_%d_%d\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
			fprintf(fp_Result,"Rd:GPR[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
		}
		else
		{
			fprintf(fp_Result,"Rd:GPR[%d] = %f_%f_%f_%f\n", IF.Rd, *(float*)&GPR[IF.Rd][0], *(float*)&GPR[IF.Rd][1], *(float*)&GPR[IF.Rd][2], *(float*)&GPR[IF.Rd][3]);
			fprintf(fp_Result,"Rd:GPR[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
		}
	}
}

void SC_VS::VS_17_MULF()
{
	fprintf(fp_Result,"Op:17 = MULF\n");
	if(IF.Rd>=32)		fprintf(fp_Result,"Rd:VOB_entry[%d]\n", IF.Rd-32);
	else				fprintf(fp_Result,"Rd:GPR[%d]\n", IF.Rd);
	if(IF.Rs>=32)		fprintf(fp_Result,"Rs:VAB_entry[%d]\n", IF.Rs-32);
	else				fprintf(fp_Result,"Rs:GPR[%d]\n", IF.Rs);
	if(IF.Rt>=32)		fprintf(fp_Result,"Rt:VAB_entry[%d]\n", IF.Rt-32);
	else				fprintf(fp_Result,"Rt:GPR[%d]\n", IF.Rt);
	fprintf(fp_Result,"Sw_d:Dont care.\n"); 
	fprintf(fp_Result,"Sw_s:%d%d%d%d\n", IF.Sw_s_x, IF.Sw_s_y, IF.Sw_s_z, IF.Sw_s_w); 
	fprintf(fp_Result,"Sw_t:%d%d%d%d\n", IF.Sw_t_x, IF.Sw_t_y, IF.Sw_t_z, IF.Sw_t_w); 
	fprintf(fp_Result,"Neg:%d%d%d\n", IF.Neg_Rs, IF.Neg_Rt, IF.Neg_Rd);
	fprintf(fp_Result,"WM:%d%d%d%d\n", IF.WM_x, IF.WM_y, IF.WM_z, IF.WM_w);
	fprintf(fp_Result,"FXL:%d\n", IF.FXL);
	fprintf(fp_Result,"S/V:%d\n", IF.S_V);
	if(IF.FXL!=1)
		fprintf(fp_Result,"FXL must be 1.\n");

	//------------Initial variable--------------------------------------
	for(i=0;i<=3;i++)
	{
		mulf_temp[i] = 0;
		Rs_SV_temp[i] = 0;
		Rt_SV_temp[i] = 0;
		Rs_swiz_temp[i] = 0;
		Rt_swiz_temp[i] = 0;

	}
	//S_V modified by TSAO
	//Load Rs
	Rs_SV_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0];
	Rs_SV_temp[1] = (IF.Rs>=32) ? VAB[IF.Rs-32][1] : GPR[IF.Rs][1];
	Rs_SV_temp[2] = (IF.Rs>=32) ? VAB[IF.Rs-32][2] : GPR[IF.Rs][2];
	Rs_SV_temp[3] = (IF.Rs>=32) ? VAB[IF.Rs-32][3] : GPR[IF.Rs][3];
	//Load Rt
	Rt_SV_temp[0] = (IF.Rt>=32) ? VAB[IF.Rt-32][0] : GPR[IF.Rt][0];
	Rt_SV_temp[1] = (IF.Rt>=32) ? VAB[IF.Rt-32][1] : GPR[IF.Rt][1];
	Rt_SV_temp[2] = (IF.Rt>=32) ? VAB[IF.Rt-32][2] : GPR[IF.Rt][2];
	Rt_SV_temp[3] = (IF.Rt>=32) ? VAB[IF.Rt-32][3] : GPR[IF.Rt][3];
	//----------------S/V---------------------------------
	for(i=0; i<=3; i++)
	{
		if(i>IF.S_V)
		{
			Rs_SV_temp[i] = 0;
			Rt_SV_temp[i] = 0;
		}
	}
	//Rs do swizzle
	switch(IF.Sw_s_x) {
	case 0:	Rs_swiz_temp[0] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[0] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[0] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[0] = Rs_SV_temp[3];break;
	}
	switch(IF.Sw_s_y) {
	case 0:	Rs_swiz_temp[1] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[1] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[1] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[1] = Rs_SV_temp[3];break;
	}
	switch(IF.Sw_s_z) {
	case 0:	Rs_swiz_temp[2] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[2] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[2] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[2] = Rs_SV_temp[3];break;
	}
	switch(IF.Sw_s_w) {
	case 0:	Rs_swiz_temp[3] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[3] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[3] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[3] = Rs_SV_temp[3];break;
	}
	
	//Rt do swizzle
	switch(IF.Sw_t_x) {
	case 0:	Rt_swiz_temp[0] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[0] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[0] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[0] = Rt_SV_temp[3];break;
	}
	switch(IF.Sw_t_y) {
	case 0:	Rt_swiz_temp[1] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[1] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[1] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[1] = Rt_SV_temp[3];break;
	}
	switch(IF.Sw_t_z) {
	case 0:	Rt_swiz_temp[2] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[2] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[2] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[2] = Rt_SV_temp[3];break;
	}
	switch(IF.Sw_t_w) {
	case 0:	Rt_swiz_temp[3] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[3] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[3] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[3] = Rt_SV_temp[3];break;
	}

	//------------Neg---------------------------------------
	if(IF.Neg_Rs==1)
	{
		if(IF.FXL==1)
		{
			for(i=0;i<=3;i++)
			{
				Rs_swiz_temp_FL[i] = -(*((float*)((void*)&Rs_swiz_temp[i])));
				Rs_swiz_temp[i] = *((unsigned int*)((void*)&Rs_swiz_temp_FL[i]));
			}
		}
	}

	if(IF.Neg_Rt==1)
	{
		if(IF.FXL==1)
		{
			for(i=0;i<=3;i++)
			{
				Rt_swiz_temp_FL[i] = -(*((float*)((void*)&Rt_swiz_temp[i])));
				Rt_swiz_temp[i] = *((unsigned int*)((void*)&Rt_swiz_temp_FL[i]));
			}
		}
	}

	if(IF.Rs>=32) {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rs:MULF doesn't support fixed point.\n");
		}
		else {
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rs-32, *(float*)&VAB[IF.Rs-32][0], *(float*)&VAB[IF.Rs-32][1], *(float*)&VAB[IF.Rs-32][2], *(float*)&VAB[IF.Rs-32][3]);
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, VAB[IF.Rs-32][0], VAB[IF.Rs-32][1], VAB[IF.Rs-32][2], VAB[IF.Rs-32][3]);
		}
	}
	else {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rs:MULF doesn't support fixed point.\n");
		}
		else {
			fprintf(fp_Result,"Rs:GPR[%d]:%f_%f_%f_%f\n", IF.Rs, *(float*)&GPR[IF.Rs][0], *(float*)&GPR[IF.Rs][1], *(float*)&GPR[IF.Rs][2], *(float*)&GPR[IF.Rs][3]);
			fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs, GPR[IF.Rs][0], GPR[IF.Rs][1], GPR[IF.Rs][2], GPR[IF.Rs][3]);
		}
	}
	if(IF.Rt>=32) {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rt:MULF doesn't support fixed point.\n");
		}
		else {
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rt-32, *(float*)&VAB[IF.Rt-32][0], *(float*)&VAB[IF.Rt-32][1], *(float*)&VAB[IF.Rt-32][2], *(float*)&VAB[IF.Rt-32][3]);
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt-32, VAB[IF.Rt-32][0], VAB[IF.Rt-32][1], VAB[IF.Rt-32][2], VAB[IF.Rt-32][3]);
		}
	}
	else {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rt:MULF doesn't support fixed point.\n");
		}
		else {
			fprintf(fp_Result,"Rt:GPR[%d]:%f_%f_%f_%f\n", IF.Rt, *(float*)&GPR[IF.Rt][0], *(float*)&GPR[IF.Rt][1], *(float*)&GPR[IF.Rt][2], *(float*)&GPR[IF.Rt][3]);
			fprintf(fp_Result,"Rt:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt, GPR[IF.Rt][0], GPR[IF.Rt][1], GPR[IF.Rt][2], GPR[IF.Rt][3]);
		}		
	}

	fprintf(fp_Result, "After Swizzle:\n");
	if(IF.Rs>=32) {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rs:MULF doesn't support fixed point.\n");
		}
		else {
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rs-32, *(float*)&Rs_swiz_temp[0], *(float*)&Rs_swiz_temp[1], *(float*)&Rs_swiz_temp[2], *(float*)&Rs_swiz_temp[3]);
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
		}
	}
	else {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rs:MULF doesn't support fixed point.\n");
		}
		else {
			fprintf(fp_Result,"Rs:GPR[%d]:%f_%f_%f_%f\n", IF.Rs, *(float*)&Rs_swiz_temp[0], *(float*)&Rs_swiz_temp[1], *(float*)&Rs_swiz_temp[2], *(float*)&Rs_swiz_temp[3]);
			fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs,Rs_swiz_temp[0],Rs_swiz_temp[1],Rs_swiz_temp[2],Rs_swiz_temp[3]);
		}
	}
	if(IF.Rt>=32) {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rt:MULF doesn't support fixed point.\n");
		}
		else {
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rt-32, *(float*)&Rt_swiz_temp[0], *(float*)&Rt_swiz_temp[1], *(float*)&Rt_swiz_temp[2], *(float*)&Rt_swiz_temp[3]);
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt-32, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
		}
	}
	else {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rt:MULF doesn't support fixed point.\n");
		}
		else {
			fprintf(fp_Result,"Rt:GPR[%d]:%f_%f_%f_%f\n", IF.Rt, *(float*)&Rt_swiz_temp[0], *(float*)&Rt_swiz_temp[1], *(float*)&Rt_swiz_temp[2], *(float*)&Rt_swiz_temp[3]);
			fprintf(fp_Result,"Rt:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
		}
	}
	/*
	//------------Neg---------------------------------------
	if(IF.Neg_Rs==1)
	{
		if(IF.FXL==1)
		{
			for(i=0;i<=3;i++)
				Rs_swiz_temp_FL[i] = -(*((float*)((void*)&Rs_swiz_temp[i])));
		}
	}
	else
	{
		if(IF.FXL==1)
		{
			for(i=0;i<=3;i++)
				Rs_swiz_temp_FL[i] = *((float*)((void*)&Rs_swiz_temp[i]));
		}
	}

	if(IF.Neg_Rt==1)
	{
		if(IF.FXL==1)
		{
			for(i=0;i<=3;i++)
				Rt_swiz_temp_FL[i] = -(*((float*)((void*)&Rt_swiz_temp[i])));
		}
	}
	else
	{
		if(IF.FXL==1)
		{
			for(i=0;i<=3;i++)
				Rt_swiz_temp_FL[i] = *((float*)((void*)&Rt_swiz_temp[i]));
		}
	}
	*/
	//------------MULF Function---------------------------------------
	if(IF.FXL==1)
	{
		for(i=0;i<=3;i++)
		{
			Rs_swiz_temp_FL[i] = *((float*)((void*)&Rs_swiz_temp[i]));
			Rt_swiz_temp_FL[i] = *((float*)((void*)&Rt_swiz_temp[i]));
			mulf_temp_FL[i] = Rs_swiz_temp_FL[i] * Rt_swiz_temp_FL[i];
			mulf_temp[i] = *((unsigned int*)((void*)&mulf_temp_FL[i]));
		}
	}

	//------------ WB --------------------------------------------------
	if(IF.WM_x==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][0] = mulf_temp[0];
		else			GPR[IF.Rd][0] = mulf_temp[0];
	}
	if(IF.WM_y==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][1] = mulf_temp[1];
		else			GPR[IF.Rd][1] = mulf_temp[1];
	}
	if(IF.WM_z==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][2] = mulf_temp[2];
		else			GPR[IF.Rd][2] = mulf_temp[2];
	}
	if(IF.WM_w==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][3] = mulf_temp[3];
		else			GPR[IF.Rd][3] = mulf_temp[3];
	}
	fprintf(fp_Result,"-----------------------------------------------------------------\n");
	if(IF.Rd>=32)
	{
		if(IF.FXL==0)
			fprintf(fp_Result,"Rd:MULF doesn't support fixed point.\n");
		else
		{
			fprintf(fp_Result,"Rd:VOB_entry[%d] = %f_%f_%f_%f\n", IF.Rd-32, *(float*)&VOB[IF.Rd-32][0], *(float*)&VOB[IF.Rd-32][1], *(float*)&VOB[IF.Rd-32][2], *(float*)&VOB[IF.Rd-32][3]);
			fprintf(fp_Result,"Rd:VOB_entry[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd-32, VOB[IF.Rd-32][0], VOB[IF.Rd-32][1], VOB[IF.Rd-32][2], VOB[IF.Rd-32][3]);
		}
	}
	else
	{
		if(IF.FXL==0)
			fprintf(fp_Result,"Rd:MULF doesn't support fixed point.\n");
		else
		{
			fprintf(fp_Result,"Rd:GPR[%d] = %f_%f_%f_%f\n", IF.Rd, *(float*)&GPR[IF.Rd][0], *(float*)&GPR[IF.Rd][1], *(float*)&GPR[IF.Rd][2], *(float*)&GPR[IF.Rd][3]);
			fprintf(fp_Result,"Rd:GPR[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
		}
	}
}

void SC_VS::VS_18_MAD()
{
	fprintf(fp_Result,"Op:18 = MAD\n");
	if(IF.Rd>=32)		fprintf(fp_Result,"Rd:VOB_entry[%d]\n", IF.Rd-32);
	else				fprintf(fp_Result,"Rd:GPR[%d]\n", IF.Rd);
	if(IF.Rs>=32)		fprintf(fp_Result,"Rs:VAB_entry[%d]\n", IF.Rs-32);
	else				fprintf(fp_Result,"Rs:GPR[%d]\n", IF.Rs);
	if(IF.Rt>=32)		fprintf(fp_Result,"Rt:VAB_entry[%d]\n", IF.Rt-32);
	else				fprintf(fp_Result,"Rt:GPR[%d]\n", IF.Rt);
	fprintf(fp_Result,"Sw_d:%d%d%d%d\n", IF.Sw_d_x, IF.Sw_d_y, IF.Sw_d_z, IF.Sw_d_w); 
	fprintf(fp_Result,"Sw_s:%d%d%d%d\n", IF.Sw_s_x, IF.Sw_s_y, IF.Sw_s_z, IF.Sw_s_w); 
	fprintf(fp_Result,"Sw_t:%d%d%d%d\n", IF.Sw_t_x, IF.Sw_t_y, IF.Sw_t_z, IF.Sw_t_w); 
	fprintf(fp_Result,"Neg:%d%d%d\n", IF.Neg_Rs, IF.Neg_Rt, IF.Neg_Rd);
	fprintf(fp_Result,"WM:%d%d%d%d\n", IF.WM_x, IF.WM_y, IF.WM_z, IF.WM_w);
	fprintf(fp_Result,"FXL:%d\n", IF.FXL);
	fprintf(fp_Result,"S/V:%d\n", IF.S_V);

	//------------Initial variable--------------------------------------
	for(i=0;i<=3;i++)
	{
		mad_temp[i] = 0;
		Rd_SV_temp[i] = 0;
		Rs_SV_temp[i] = 0;
		Rt_SV_temp[i] = 0;
		Rd_swiz_temp[i] = 0;
		Rs_swiz_temp[i] = 0;
		Rt_swiz_temp[i] = 0;
	}

	//S_V modified by TSAO
	//Load Rd
	Rd_SV_temp[0] = (IF.Rd>=32) ? VAB[IF.Rd-32][0] : GPR[IF.Rd][0];
	Rd_SV_temp[1] = (IF.Rd>=32) ? VAB[IF.Rd-32][1] : GPR[IF.Rd][1];
	Rd_SV_temp[2] = (IF.Rd>=32) ? VAB[IF.Rd-32][2] : GPR[IF.Rd][2];
	Rd_SV_temp[3] = (IF.Rd>=32) ? VAB[IF.Rd-32][3] : GPR[IF.Rd][3];
	//Load Rs
	Rs_SV_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0];
	Rs_SV_temp[1] = (IF.Rs>=32) ? VAB[IF.Rs-32][1] : GPR[IF.Rs][1];
	Rs_SV_temp[2] = (IF.Rs>=32) ? VAB[IF.Rs-32][2] : GPR[IF.Rs][2];
	Rs_SV_temp[3] = (IF.Rs>=32) ? VAB[IF.Rs-32][3] : GPR[IF.Rs][3];
	//Load Rt
	Rt_SV_temp[0] = (IF.Rt>=32) ? VAB[IF.Rt-32][0] : GPR[IF.Rt][0];
	Rt_SV_temp[1] = (IF.Rt>=32) ? VAB[IF.Rt-32][1] : GPR[IF.Rt][1];
	Rt_SV_temp[2] = (IF.Rt>=32) ? VAB[IF.Rt-32][2] : GPR[IF.Rt][2];
	Rt_SV_temp[3] = (IF.Rt>=32) ? VAB[IF.Rt-32][3] : GPR[IF.Rt][3];
	//----------------S/V---------------------------------
	for(i=0; i<=3; i++)
	{
		if(i>IF.S_V)
		{
			Rd_SV_temp[i] = 0;
			Rs_SV_temp[i] = 0;
			Rt_SV_temp[i] = 0;
		}
	}
	//Rd do swizzle
	switch(IF.Sw_d_x) {
	case 0:	Rd_swiz_temp[0] = Rd_SV_temp[0];break;
	case 1:	Rd_swiz_temp[0] = Rd_SV_temp[1];break;
	case 2:	Rd_swiz_temp[0] = Rd_SV_temp[2];break;
	case 3:	Rd_swiz_temp[0] = Rd_SV_temp[3];break;
	}
	switch(IF.Sw_d_y) {
	case 0:	Rd_swiz_temp[1] = Rd_SV_temp[0];break;
	case 1:	Rd_swiz_temp[1] = Rd_SV_temp[1];break;
	case 2:	Rd_swiz_temp[1] = Rd_SV_temp[2];break;
	case 3:	Rd_swiz_temp[1] = Rd_SV_temp[3];break;
	}
	switch(IF.Sw_d_z) {
	case 0:	Rd_swiz_temp[2] = Rd_SV_temp[0];break;
	case 1:	Rd_swiz_temp[2] = Rd_SV_temp[1];break;
	case 2:	Rd_swiz_temp[2] = Rd_SV_temp[2];break;
	case 3:	Rd_swiz_temp[2] = Rd_SV_temp[3];break;
	}
	switch(IF.Sw_d_w) {
	case 0:	Rd_swiz_temp[3] = Rd_SV_temp[0];break;
	case 1:	Rd_swiz_temp[3] = Rd_SV_temp[1];break;
	case 2:	Rd_swiz_temp[3] = Rd_SV_temp[2];break;
	case 3:	Rd_swiz_temp[3] = Rd_SV_temp[3];break;
	}
	
	//Rs do swizzle
	switch(IF.Sw_s_x) {
	case 0:	Rs_swiz_temp[0] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[0] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[0] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[0] = Rs_SV_temp[3];break;
	}
	switch(IF.Sw_s_y) {
	case 0:	Rs_swiz_temp[1] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[1] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[1] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[1] = Rs_SV_temp[3];break;
	}
	switch(IF.Sw_s_z) {
	case 0:	Rs_swiz_temp[2] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[2] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[2] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[2] = Rs_SV_temp[3];break;
	}
	switch(IF.Sw_s_w) {
	case 0:	Rs_swiz_temp[3] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[3] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[3] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[3] = Rs_SV_temp[3];break;
	}
	
	//Rt do swizzle
	switch(IF.Sw_t_x) {
	case 0:	Rt_swiz_temp[0] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[0] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[0] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[0] = Rt_SV_temp[3];break;
	}
	switch(IF.Sw_t_y) {
	case 0:	Rt_swiz_temp[1] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[1] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[1] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[1] = Rt_SV_temp[3];break;
	}
	switch(IF.Sw_t_z) {
	case 0:	Rt_swiz_temp[2] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[2] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[2] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[2] = Rt_SV_temp[3];break;
	}
	switch(IF.Sw_t_w) {
	case 0:	Rt_swiz_temp[3] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[3] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[3] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[3] = Rt_SV_temp[3];break;
	}

	//------------Neg---------------------------------------
	if(IF.Neg_Rd==1)
	{
		if(IF.FXL==0)
		{
			for(i=0;i<=3;i++)
			{
				Rd_swiz_temp_FX[i] = -(*((int*)((void*)&Rd_swiz_temp[i])));
				Rd_swiz_temp[i] = *((unsigned int*)((void*)&Rd_swiz_temp_FX[i]));
			}
		}
		else
		{
			for(i=0;i<=3;i++)
			{
				Rd_swiz_temp_FL[i] = -(*((float*)((void*)&Rd_swiz_temp[i])));
				Rd_swiz_temp[i] = *((unsigned int*)((void*)&Rd_swiz_temp_FL[i]));
			}
		}
	}

	if(IF.Neg_Rs==1)
	{
		if(IF.FXL==0)
		{
			for(i=0;i<=3;i++)
			{
				Rs_swiz_temp_FX[i] = -(*((int*)((void*)&Rs_swiz_temp[i])));
				Rs_swiz_temp[i] = *((unsigned int*)((void*)&Rs_swiz_temp_FX[i]));
			}
		}
		else
		{
			for(i=0;i<=3;i++)
			{
				Rs_swiz_temp_FL[i] = -(*((float*)((void*)&Rs_swiz_temp[i])));
				Rs_swiz_temp[i] = *((unsigned int*)((void*)&Rs_swiz_temp_FL[i]));
			}
		}
	}

	if(IF.Neg_Rt==1)
	{
		if(IF.FXL==0)
		{
			for(i=0;i<=3;i++)
			{
				Rt_swiz_temp_FX[i] = -(*((int*)((void*)&Rt_swiz_temp[i])));
				Rt_swiz_temp[i] = *((unsigned int*)((void*)&Rt_swiz_temp_FX[i]));
			}
		}
		else
		{
			for(i=0;i<=3;i++)
			{
				Rt_swiz_temp_FL[i] = -(*((float*)((void*)&Rt_swiz_temp[i])));
				Rt_swiz_temp[i] = *((unsigned int*)((void*)&Rt_swiz_temp_FL[i]));
			}
		}
	}


	if(IF.Rd>=32) {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rd:VAB_entry[%d]:%d_%d_%d_%d\n", IF.Rd-32, VAB[IF.Rd-32][0], VAB[IF.Rd-32][1], VAB[IF.Rd-32][2], VAB[IF.Rd-32][3]);
			fprintf(fp_Result,"Rd:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rd-32, VAB[IF.Rd-32][0], VAB[IF.Rd-32][1], VAB[IF.Rd-32][2], VAB[IF.Rd-32][3]);
		}
		else {
			fprintf(fp_Result,"Rd:VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rd-32, *(float*)&VAB[IF.Rd-32][0], *(float*)&VAB[IF.Rd-32][1], *(float*)&VAB[IF.Rd-32][2], *(float*)&VAB[IF.Rd-32][3]);
			fprintf(fp_Result,"Rd:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rd-32, VAB[IF.Rd-32][0], VAB[IF.Rd-32][1], VAB[IF.Rd-32][2], VAB[IF.Rd-32][3]);
		}
	}
	else {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rd:GPR[%d]:%d_%d_%d_%d\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
			fprintf(fp_Result,"Rd:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
		}
		else {
			fprintf(fp_Result,"Rd:GPR[%d]:%f_%f_%f_%f\n", IF.Rd, *(float*)&GPR[IF.Rd][0], *(float*)&GPR[IF.Rd][1], *(float*)&GPR[IF.Rd][2], *(float*)&GPR[IF.Rd][3]);
			fprintf(fp_Result,"Rd:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
		}
	}
	if(IF.Rs>=32) {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%d_%d_%d_%d\n", IF.Rs-32, VAB[IF.Rs-32][0], VAB[IF.Rs-32][1], VAB[IF.Rs-32][2], VAB[IF.Rs-32][3]);
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, VAB[IF.Rs-32][0], VAB[IF.Rs-32][1], VAB[IF.Rs-32][2], VAB[IF.Rs-32][3]);
		}
		else {
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rs-32, *(float*)&VAB[IF.Rs-32][0], *(float*)&VAB[IF.Rs-32][1], *(float*)&VAB[IF.Rs-32][2], *(float*)&VAB[IF.Rs-32][3]);
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, VAB[IF.Rs-32][0], VAB[IF.Rs-32][1], VAB[IF.Rs-32][2], VAB[IF.Rs-32][3]);
		}
	}
	else {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rs:GPR[%d]:%d_%d_%d_%d\n", IF.Rs, GPR[IF.Rs][0], GPR[IF.Rs][1], GPR[IF.Rs][2], GPR[IF.Rs][3]);
			fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs, GPR[IF.Rs][0], GPR[IF.Rs][1], GPR[IF.Rs][2], GPR[IF.Rs][3]);
		}
		else {
			fprintf(fp_Result,"Rs:GPR[%d]:%f_%f_%f_%f\n", IF.Rs, *(float*)&GPR[IF.Rs][0], *(float*)&GPR[IF.Rs][1], *(float*)&GPR[IF.Rs][2], *(float*)&GPR[IF.Rs][3]);
			fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs, GPR[IF.Rs][0], GPR[IF.Rs][1], GPR[IF.Rs][2], GPR[IF.Rs][3]);
		}
	}
	if(IF.Rt>=32) {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%d_%d_%d_%d\n", IF.Rt-32, VAB[IF.Rt-32][0], VAB[IF.Rt-32][1], VAB[IF.Rt-32][2], VAB[IF.Rt-32][3]);
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt-32, VAB[IF.Rt-32][0], VAB[IF.Rt-32][1], VAB[IF.Rt-32][2], VAB[IF.Rt-32][3]);
		}
		else {
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rt-32, *(float*)&VAB[IF.Rt-32][0], *(float*)&VAB[IF.Rt-32][1], *(float*)&VAB[IF.Rt-32][2], *(float*)&VAB[IF.Rt-32][3]);
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt-32, VAB[IF.Rt-32][0], VAB[IF.Rt-32][1], VAB[IF.Rt-32][2], VAB[IF.Rt-32][3]);
		}
	}
	else {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rt:GPR[%d]:%d_%d_%d_%d\n", IF.Rt, GPR[IF.Rt][0], GPR[IF.Rt][1], GPR[IF.Rt][2], GPR[IF.Rt][3]);
			fprintf(fp_Result,"Rt:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt, GPR[IF.Rt][0], GPR[IF.Rt][1], GPR[IF.Rt][2], GPR[IF.Rt][3]);
		}
		else {
			fprintf(fp_Result,"Rt:GPR[%d]:%f_%f_%f_%f\n", IF.Rt, *(float*)&GPR[IF.Rt][0], *(float*)&GPR[IF.Rt][1], *(float*)&GPR[IF.Rt][2], *(float*)&GPR[IF.Rt][3]);
			fprintf(fp_Result,"Rt:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt, GPR[IF.Rt][0], GPR[IF.Rt][1], GPR[IF.Rt][2], GPR[IF.Rt][3]);
		}		
	}

	fprintf(fp_Result, "After Swizzle/Neg/SV:\n");
	if(IF.Rd>=32) {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rd:VAB_entry[%d]:%d_%d_%d_%d\n", IF.Rd-32, Rd_swiz_temp[0], Rd_swiz_temp[1], Rd_swiz_temp[2], Rd_swiz_temp[3]);
			fprintf(fp_Result,"Rd:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rd-32, Rd_swiz_temp[0], Rd_swiz_temp[1], Rd_swiz_temp[2], Rd_swiz_temp[3]);
		}
		else {
			fprintf(fp_Result,"Rd:VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rd-32, *(float*)&Rd_swiz_temp[0], *(float*)&Rd_swiz_temp[1], *(float*)&Rd_swiz_temp[2], *(float*)&Rd_swiz_temp[3]);
			fprintf(fp_Result,"Rd:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rd-32, Rd_swiz_temp[0], Rd_swiz_temp[1], Rd_swiz_temp[2], Rd_swiz_temp[3]);
		}
	}
	else {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rd:GPR[%d]:%d_%d_%d_%d\n", IF.Rd,Rd_swiz_temp[0],Rd_swiz_temp[1],Rd_swiz_temp[2],Rd_swiz_temp[3]);
			fprintf(fp_Result,"Rd:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rd,Rd_swiz_temp[0],Rd_swiz_temp[1],Rd_swiz_temp[2],Rd_swiz_temp[3]);
		}
		else {
			fprintf(fp_Result,"Rd:GPR[%d]:%f_%f_%f_%f\n", IF.Rd, *(float*)&Rd_swiz_temp[0], *(float*)&Rd_swiz_temp[1], *(float*)&Rd_swiz_temp[2], *(float*)&Rd_swiz_temp[3]);
			fprintf(fp_Result,"Rd:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rd,Rd_swiz_temp[0],Rd_swiz_temp[1],Rd_swiz_temp[2],Rd_swiz_temp[3]);
		}
	}
	if(IF.Rs>=32) {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%d_%d_%d_%d\n", IF.Rs-32, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
		}
		else {
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rs-32, *(float*)&Rs_swiz_temp[0], *(float*)&Rs_swiz_temp[1], *(float*)&Rs_swiz_temp[2], *(float*)&Rs_swiz_temp[3]);
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
		}
	}
	else {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rs:GPR[%d]:%d_%d_%d_%d\n", IF.Rs,Rs_swiz_temp[0],Rs_swiz_temp[1],Rs_swiz_temp[2],Rs_swiz_temp[3]);
			fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs,Rs_swiz_temp[0],Rs_swiz_temp[1],Rs_swiz_temp[2],Rs_swiz_temp[3]);
		}
		else {
			fprintf(fp_Result,"Rs:GPR[%d]:%f_%f_%f_%f\n", IF.Rs, *(float*)&Rs_swiz_temp[0], *(float*)&Rs_swiz_temp[1], *(float*)&Rs_swiz_temp[2], *(float*)&Rs_swiz_temp[3]);
			fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs,Rs_swiz_temp[0],Rs_swiz_temp[1],Rs_swiz_temp[2],Rs_swiz_temp[3]);
		}
	}
	if(IF.Rt>=32) {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%d_%d_%d_%d\n", IF.Rt-32, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt-32, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
		}
		else {
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rt-32, *(float*)&Rt_swiz_temp[0], *(float*)&Rt_swiz_temp[1], *(float*)&Rt_swiz_temp[2], *(float*)&Rt_swiz_temp[3]);
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt-32, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
		}
	}
	else {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rt:GPR[%d]:%d_%d_%d_%d\n", IF.Rt, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
			fprintf(fp_Result,"Rt:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
		}
		else {
			fprintf(fp_Result,"Rt:GPR[%d]:%f_%f_%f_%f\n", IF.Rt, *(float*)&Rt_swiz_temp[0], *(float*)&Rt_swiz_temp[1], *(float*)&Rt_swiz_temp[2], *(float*)&Rt_swiz_temp[3]);
			fprintf(fp_Result,"Rt:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
		}
	}
	/*
	//------------Neg---------------------------------------
	if(IF.Neg_Rd==1)
	{
		if(IF.FXL==0)
		{
			for(i=0;i<=3;i++)
				Rd_swiz_temp_FX[i] = -(*((int*)((void*)&Rd_swiz_temp[i])));
		}
		else
		{
			for(i=0;i<=3;i++)
				Rd_swiz_temp_FL[i] = -(*((float*)((void*)&Rd_swiz_temp[i])));
		}
	}
	else
	{
		if(IF.FXL==0)
		{
			for(i=0;i<=3;i++)
				Rd_swiz_temp_FX[i] = *((int*)((void*)&Rd_swiz_temp[i]));
		}
		else
		{
			for(i=0;i<=3;i++)
				Rd_swiz_temp_FL[i] = *((float*)((void*)&Rd_swiz_temp[i]));
		}
	}

	if(IF.Neg_Rs==1)
	{
		if(IF.FXL==0)
		{
			for(i=0;i<=3;i++)
				Rs_swiz_temp_FX[i] = -(*((int*)((void*)&Rs_swiz_temp[i])));
		}
		else
		{
			for(i=0;i<=3;i++)
				Rs_swiz_temp_FL[i] = -(*((float*)((void*)&Rs_swiz_temp[i])));
		}
	}
	else
	{
		if(IF.FXL==0)
		{
			for(i=0;i<=3;i++)
				Rs_swiz_temp_FX[i] = *((int*)((void*)&Rs_swiz_temp[i]));
		}
		else
		{
			for(i=0;i<=3;i++)
				Rs_swiz_temp_FL[i] = *((float*)((void*)&Rs_swiz_temp[i]));
		}
	}

	if(IF.Neg_Rt==1)
	{
		if(IF.FXL==0)
		{
			for(i=0;i<=3;i++)
				Rt_swiz_temp_FX[i] = -(*((int*)((void*)&Rt_swiz_temp[i])));
		}
		else
		{
			for(i=0;i<=3;i++)
				Rt_swiz_temp_FL[i] = -(*((float*)((void*)&Rt_swiz_temp[i])));
		}
	}
	else
	{
		if(IF.FXL==0)
		{
			for(i=0;i<=3;i++)
				Rt_swiz_temp_FX[i] = *((int*)((void*)&Rt_swiz_temp[i]));
		}
		else
		{
			for(i=0;i<=3;i++)
				Rt_swiz_temp_FL[i] = *((float*)((void*)&Rt_swiz_temp[i]));
		}
	}
	*/
	//------------MAD Function---------------------------------------
	if(IF.FXL==0)
	{
		for(i=0;i<=3;i++)
		{
			Rd_swiz_temp_FX[i] = *((int*)((void*)&Rd_swiz_temp[i]));
			Rs_swiz_temp_FX[i] = *((int*)((void*)&Rs_swiz_temp[i]));
			Rt_swiz_temp_FX[i] = *((int*)((void*)&Rt_swiz_temp[i]));
			mad_temp_FX[i] = Rs_swiz_temp_FX[i] * Rt_swiz_temp_FX[i] + Rd_swiz_temp_FX[i];
			mad_temp[i] = *((unsigned int*)((void*)&mad_temp_FX[i]));
		}
	}
	else
	{
		for(i=0;i<=3;i++)
		{
			Rd_swiz_temp_FL[i] = *((float*)((void*)&Rd_swiz_temp[i]));
			Rs_swiz_temp_FL[i] = *((float*)((void*)&Rs_swiz_temp[i]));
			Rt_swiz_temp_FL[i] = *((float*)((void*)&Rt_swiz_temp[i]));
			mad_temp_FL[i] = Rs_swiz_temp_FL[i] * Rt_swiz_temp_FL[i] + Rd_swiz_temp_FL[i];
			mad_temp[i] = *((unsigned int*)((void*)&mad_temp_FL[i]));
		}
	}

	//------------ WB --------------------------------------------------
	if(IF.WM_x==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][0] = mad_temp[0];
		else			GPR[IF.Rd][0] = mad_temp[0];
	}
	if(IF.WM_y==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][1] = mad_temp[1];
		else			GPR[IF.Rd][1] = mad_temp[1];
	}
	if(IF.WM_z==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][2] = mad_temp[2];
		else			GPR[IF.Rd][2] = mad_temp[2];
	}
	if(IF.WM_w==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][3] = mad_temp[3];
		else			GPR[IF.Rd][3] = mad_temp[3];
	}
	fprintf(fp_Result,"-----------------------------------------------------------------\n");
	if(IF.Rd>=32)
	{
		if(IF.FXL==0)
		{
			fprintf(fp_Result,"Rd:VOB_entry[%d] = %d_%d_%d_%d\n", IF.Rd-32, VOB[IF.Rd-32][0], VOB[IF.Rd-32][1], VOB[IF.Rd-32][2], VOB[IF.Rd-32][3]);
			fprintf(fp_Result,"Rd:VOB_entry[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd-32, VOB[IF.Rd-32][0], VOB[IF.Rd-32][1], VOB[IF.Rd-32][2], VOB[IF.Rd-32][3]);
		}
		else
		{
			fprintf(fp_Result,"Rd:VOB_entry[%d] = %f_%f_%f_%f\n", IF.Rd-32, *(float*)&VOB[IF.Rd-32][0], *(float*)&VOB[IF.Rd-32][1], *(float*)&VOB[IF.Rd-32][2], *(float*)&VOB[IF.Rd-32][3]);
			fprintf(fp_Result,"Rd:VOB_entry[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd-32, VOB[IF.Rd-32][0], VOB[IF.Rd-32][1], VOB[IF.Rd-32][2], VOB[IF.Rd-32][3]);
		}
	}
	else
	{
		if(IF.FXL==0)
		{
			fprintf(fp_Result,"Rd:GPR[%d] = %d_%d_%d_%d\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
			fprintf(fp_Result,"Rd:GPR[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
		}
		else
		{
			fprintf(fp_Result,"Rd:GPR[%d] = %f_%f_%f_%f\n", IF.Rd, *(float*)&GPR[IF.Rd][0], *(float*)&GPR[IF.Rd][1], *(float*)&GPR[IF.Rd][2], *(float*)&GPR[IF.Rd][3]);
			fprintf(fp_Result,"Rd:GPR[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
		}
	}
}

void SC_VS::VS_19_DP()
{
	fprintf(fp_Result,"Op:19 = DP\n");
	if(IF.Rd>=32)		fprintf(fp_Result,"Rd:VOB_entry[%d]\n", IF.Rd-32);
	else				fprintf(fp_Result,"Rd:GPR[%d]\n", IF.Rd);
	if(IF.Rs>=32)		fprintf(fp_Result,"Rs:VAB_entry[%d]\n", IF.Rs-32);
	else				fprintf(fp_Result,"Rs:GPR[%d]\n", IF.Rs);
	if(IF.Rt>=32)		fprintf(fp_Result,"Rt:VAB_entry[%d]\n", IF.Rt-32);
	else				fprintf(fp_Result,"Rt:GPR[%d]\n", IF.Rt);
	fprintf(fp_Result,"Sw_d:Dont care.\n"); 
	fprintf(fp_Result,"Sw_s:%d%d%d%d\n", IF.Sw_s_x, IF.Sw_s_y, IF.Sw_s_z, IF.Sw_s_w); 
	fprintf(fp_Result,"Sw_t:%d%d%d%d\n", IF.Sw_t_x, IF.Sw_t_y, IF.Sw_t_z, IF.Sw_t_w); 
	fprintf(fp_Result,"Neg:%d%d%d\n", IF.Neg_Rs, IF.Neg_Rt, IF.Neg_Rd);
	fprintf(fp_Result,"WM:%d%d%d%d\n", IF.WM_x, IF.WM_y, IF.WM_z, IF.WM_w);
	fprintf(fp_Result,"FXL:%d\n", IF.FXL);
	fprintf(fp_Result,"S/V:%d\n", IF.S_V);

	//------------Initial variable--------------------------------------
	dp_temp = 0;
	for(i=0;i<=3;i++)
	{
		Rs_SV_temp[i] = 0;
		Rt_SV_temp[i] = 0;
		Rs_swiz_temp[i] = 0;
		Rt_swiz_temp[i] = 0;

	}
	//S_V modified by TSAO
	//Load Rs
	Rs_SV_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0];
	Rs_SV_temp[1] = (IF.Rs>=32) ? VAB[IF.Rs-32][1] : GPR[IF.Rs][1];
	Rs_SV_temp[2] = (IF.Rs>=32) ? VAB[IF.Rs-32][2] : GPR[IF.Rs][2];
	Rs_SV_temp[3] = (IF.Rs>=32) ? VAB[IF.Rs-32][3] : GPR[IF.Rs][3];
	//Load Rt
	Rt_SV_temp[0] = (IF.Rt>=32) ? VAB[IF.Rt-32][0] : GPR[IF.Rt][0];
	Rt_SV_temp[1] = (IF.Rt>=32) ? VAB[IF.Rt-32][1] : GPR[IF.Rt][1];
	Rt_SV_temp[2] = (IF.Rt>=32) ? VAB[IF.Rt-32][2] : GPR[IF.Rt][2];
	Rt_SV_temp[3] = (IF.Rt>=32) ? VAB[IF.Rt-32][3] : GPR[IF.Rt][3];
	//----------------S/V---------------------------------
	for(i=0; i<=3; i++)
	{
		if(i>IF.S_V)
		{
			Rs_SV_temp[i] = 0;
			Rt_SV_temp[i] = 0;
		}
	}
	//Rs do swizzle
	switch(IF.Sw_s_x) {
	case 0:	Rs_swiz_temp[0] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[0] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[0] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[0] = Rs_SV_temp[3];break;
	}
	switch(IF.Sw_s_y) {
	case 0:	Rs_swiz_temp[1] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[1] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[1] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[1] = Rs_SV_temp[3];break;
	}
	switch(IF.Sw_s_z) {
	case 0:	Rs_swiz_temp[2] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[2] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[2] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[2] = Rs_SV_temp[3];break;
	}
	switch(IF.Sw_s_w) {
	case 0:	Rs_swiz_temp[3] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[3] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[3] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[3] = Rs_SV_temp[3];break;
	}
	
	//Rt do swizzle
	switch(IF.Sw_t_x) {
	case 0:	Rt_swiz_temp[0] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[0] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[0] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[0] = Rt_SV_temp[3];break;
	}
	switch(IF.Sw_t_y) {
	case 0:	Rt_swiz_temp[1] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[1] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[1] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[1] = Rt_SV_temp[3];break;
	}
	switch(IF.Sw_t_z) {
	case 0:	Rt_swiz_temp[2] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[2] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[2] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[2] = Rt_SV_temp[3];break;
	}
	switch(IF.Sw_t_w) {
	case 0:	Rt_swiz_temp[3] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[3] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[3] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[3] = Rt_SV_temp[3];break;
	}

	//------------Neg---------------------------------------
	if(IF.Neg_Rs==1)
	{
		if(IF.FXL==0)
		{
			for(i=0;i<=3;i++)
			{
				Rs_swiz_temp_FX[i] = -(*((int*)((void*)&Rs_swiz_temp[i])));
				Rs_swiz_temp[i] = *((unsigned int*)((void*)&Rs_swiz_temp_FX[i]));
			}
		}
		else
		{
			for(i=0;i<=3;i++)
			{
				Rs_swiz_temp_FL[i] = -(*((float*)((void*)&Rs_swiz_temp[i])));
				Rs_swiz_temp[i] = *((unsigned int*)((void*)&Rs_swiz_temp_FL[i]));
			}
		}
	}

	if(IF.Neg_Rt==1)
	{
		if(IF.FXL==0)
		{
			for(i=0;i<=3;i++)
			{
				Rt_swiz_temp_FX[i] = -(*((int*)((void*)&Rt_swiz_temp[i])));
				Rt_swiz_temp[i] = *((unsigned int*)((void*)&Rt_swiz_temp_FX[i]));
			}
		}
		else
		{
			for(i=0;i<=3;i++)
			{
				Rt_swiz_temp_FL[i] = -(*((float*)((void*)&Rt_swiz_temp[i])));
				Rt_swiz_temp[i] = *((unsigned int*)((void*)&Rt_swiz_temp_FL[i]));
			}
		}
	}

	if(IF.Rs>=32) {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%d_%d_%d_%d\n", IF.Rs-32, VAB[IF.Rs-32][0], VAB[IF.Rs-32][1], VAB[IF.Rs-32][2], VAB[IF.Rs-32][3]);
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, VAB[IF.Rs-32][0], VAB[IF.Rs-32][1], VAB[IF.Rs-32][2], VAB[IF.Rs-32][3]);
		}
		else {
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rs-32, *(float*)&VAB[IF.Rs-32][0], *(float*)&VAB[IF.Rs-32][1], *(float*)&VAB[IF.Rs-32][2], *(float*)&VAB[IF.Rs-32][3]);
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, VAB[IF.Rs-32][0], VAB[IF.Rs-32][1], VAB[IF.Rs-32][2], VAB[IF.Rs-32][3]);
		}
	}
	else {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rs:GPR[%d]:%d_%d_%d_%d\n", IF.Rs, GPR[IF.Rs][0], GPR[IF.Rs][1], GPR[IF.Rs][2], GPR[IF.Rs][3]);
			fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs, GPR[IF.Rs][0], GPR[IF.Rs][1], GPR[IF.Rs][2], GPR[IF.Rs][3]);
		}
		else {
			fprintf(fp_Result,"Rs:GPR[%d]:%f_%f_%f_%f\n", IF.Rs, *(float*)&GPR[IF.Rs][0], *(float*)&GPR[IF.Rs][1], *(float*)&GPR[IF.Rs][2], *(float*)&GPR[IF.Rs][3]);
			fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs, GPR[IF.Rs][0], GPR[IF.Rs][1], GPR[IF.Rs][2], GPR[IF.Rs][3]);
		}
	}
	if(IF.Rt>=32) {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%d_%d_%d_%d\n", IF.Rt-32, VAB[IF.Rt-32][0], VAB[IF.Rt-32][1], VAB[IF.Rt-32][2], VAB[IF.Rt-32][3]);
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt-32, VAB[IF.Rt-32][0], VAB[IF.Rt-32][1], VAB[IF.Rt-32][2], VAB[IF.Rt-32][3]);
		}
		else {
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rt-32, *(float*)&VAB[IF.Rt-32][0], *(float*)&VAB[IF.Rt-32][1], *(float*)&VAB[IF.Rt-32][2], *(float*)&VAB[IF.Rt-32][3]);
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt-32, VAB[IF.Rt-32][0], VAB[IF.Rt-32][1], VAB[IF.Rt-32][2], VAB[IF.Rt-32][3]);
		}
	}
	else {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rt:GPR[%d]:%d_%d_%d_%d\n", IF.Rt, GPR[IF.Rt][0], GPR[IF.Rt][1], GPR[IF.Rt][2], GPR[IF.Rt][3]);
			fprintf(fp_Result,"Rt:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt, GPR[IF.Rt][0], GPR[IF.Rt][1], GPR[IF.Rt][2], GPR[IF.Rt][3]);
		}
		else {
			fprintf(fp_Result,"Rt:GPR[%d]:%f_%f_%f_%f\n", IF.Rt, *(float*)&GPR[IF.Rt][0], *(float*)&GPR[IF.Rt][1], *(float*)&GPR[IF.Rt][2], *(float*)&GPR[IF.Rt][3]);
			fprintf(fp_Result,"Rt:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt, GPR[IF.Rt][0], GPR[IF.Rt][1], GPR[IF.Rt][2], GPR[IF.Rt][3]);
		}		
	}

	fprintf(fp_Result, "After Swizzle/Neg/SV:\n");
	if(IF.Rs>=32) {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%d_%d_%d_%d\n", IF.Rs-32, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
		}
		else {
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rs-32, *(float*)&Rs_swiz_temp[0], *(float*)&Rs_swiz_temp[1], *(float*)&Rs_swiz_temp[2], *(float*)&Rs_swiz_temp[3]);
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
		}
	}
	else {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rs:GPR[%d]:%d_%d_%d_%d\n", IF.Rs,Rs_swiz_temp[0],Rs_swiz_temp[1],Rs_swiz_temp[2],Rs_swiz_temp[3]);
			fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs,Rs_swiz_temp[0],Rs_swiz_temp[1],Rs_swiz_temp[2],Rs_swiz_temp[3]);
		}
		else {
			fprintf(fp_Result,"Rs:GPR[%d]:%f_%f_%f_%f\n", IF.Rs, *(float*)&Rs_swiz_temp[0], *(float*)&Rs_swiz_temp[1], *(float*)&Rs_swiz_temp[2], *(float*)&Rs_swiz_temp[3]);
			fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs,Rs_swiz_temp[0],Rs_swiz_temp[1],Rs_swiz_temp[2],Rs_swiz_temp[3]);
		}
	}
	if(IF.Rt>=32) {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%d_%d_%d_%d\n", IF.Rt-32, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt-32, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
		}
		else {
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%f_%f_%f_%f\n", IF.Rt-32, *(float*)&Rt_swiz_temp[0], *(float*)&Rt_swiz_temp[1], *(float*)&Rt_swiz_temp[2], *(float*)&Rt_swiz_temp[3]);
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt-32, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
		}
	}
	else {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rt:GPR[%d]:%d_%d_%d_%d\n", IF.Rt, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
			fprintf(fp_Result,"Rt:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
		}
		else {
			fprintf(fp_Result,"Rt:GPR[%d]:%f_%f_%f_%f\n", IF.Rt, *(float*)&Rt_swiz_temp[0], *(float*)&Rt_swiz_temp[1], *(float*)&Rt_swiz_temp[2], *(float*)&Rt_swiz_temp[3]);
			fprintf(fp_Result,"Rt:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
		}
	}
	/*
	//------------Neg---------------------------------------
	if(IF.Neg_Rs==1)
	{
		if(IF.FXL==0)
		{
			for(i=0;i<=3;i++)
				Rs_swiz_temp_FX[i] = -(*((int*)((void*)&Rs_swiz_temp[i])));
		}
		else
		{
			for(i=0;i<=3;i++)
				Rs_swiz_temp_FL[i] = -(*((float*)((void*)&Rs_swiz_temp[i])));
		}
	}
	else
	{
		if(IF.FXL==0)
		{
			for(i=0;i<=3;i++)
				Rs_swiz_temp_FX[i] = *((int*)((void*)&Rs_swiz_temp[i]));
		}
		else
		{
			for(i=0;i<=3;i++)
				Rs_swiz_temp_FL[i] = *((float*)((void*)&Rs_swiz_temp[i]));
		}
	}

	if(IF.Neg_Rt==1)
	{
		if(IF.FXL==0)
		{
			for(i=0;i<=3;i++)
				Rt_swiz_temp_FX[i] = -(*((int*)((void*)&Rt_swiz_temp[i])));
		}
		else
		{
			for(i=0;i<=3;i++)
				Rt_swiz_temp_FL[i] = -(*((float*)((void*)&Rt_swiz_temp[i])));
		}
	}
	else
	{
		if(IF.FXL==0)
		{
			for(i=0;i<=3;i++)
				Rt_swiz_temp_FX[i] = *((int*)((void*)&Rt_swiz_temp[i]));
		}
		else
		{
			for(i=0;i<=3;i++)
				Rt_swiz_temp_FL[i] = *((float*)((void*)&Rt_swiz_temp[i]));
		}
	}
	*/
	//------------DP Function---------------------------------------
	if(IF.FXL==0)
	{
		dp_temp_FX = 0;
		for(i=0;i<=3;i++)
		{
			Rs_swiz_temp_FX[i] = *((int*)((void*)&Rs_swiz_temp[i]));
			Rt_swiz_temp_FX[i] = *((int*)((void*)&Rt_swiz_temp[i]));
			dp_temp_FX += Rs_swiz_temp_FX[i] * Rt_swiz_temp_FX[i];
			dp_temp = *((unsigned int*)((void*)&dp_temp_FX));
		}
	}
	else
	{
		dp_temp_FL = 0;
		for(i=0;i<=3;i++)
		{
			Rs_swiz_temp_FL[i] = *((float*)((void*)&Rs_swiz_temp[i]));
			Rt_swiz_temp_FL[i] = *((float*)((void*)&Rt_swiz_temp[i]));
			dp_temp_FL += Rs_swiz_temp_FL[i] * Rt_swiz_temp_FL[i];
			dp_temp = *((unsigned int*)((void*)&dp_temp_FL));
		}
	}

	//------------ WB --------------------------------------------------
	if(IF.WM_x==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][0] = dp_temp;
		else			GPR[IF.Rd][0] = dp_temp;
	}
	if(IF.WM_y==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][1] = dp_temp;
		else			GPR[IF.Rd][1] = dp_temp;
	}
	if(IF.WM_z==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][2] = dp_temp;
		else			GPR[IF.Rd][2] = dp_temp;
	}
	if(IF.WM_w==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][3] = dp_temp;
		else			GPR[IF.Rd][3] = dp_temp;
	}
	fprintf(fp_Result,"-----------------------------------------------------------------\n");
	if(IF.Rd>=32)
	{
		if(IF.FXL==0)
		{
			fprintf(fp_Result,"Rd:VOB_entry[%d] = %d_%d_%d_%d\n", IF.Rd-32, VOB[IF.Rd-32][0], VOB[IF.Rd-32][1], VOB[IF.Rd-32][2], VOB[IF.Rd-32][3]);
			fprintf(fp_Result,"Rd:VOB_entry[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd-32, VOB[IF.Rd-32][0], VOB[IF.Rd-32][1], VOB[IF.Rd-32][2], VOB[IF.Rd-32][3]);
		}
		else
		{
			fprintf(fp_Result,"Rd:VOB_entry[%d] = %f_%f_%f_%f\n", IF.Rd-32, *(float*)&VOB[IF.Rd-32][0], *(float*)&VOB[IF.Rd-32][1], *(float*)&VOB[IF.Rd-32][2], *(float*)&VOB[IF.Rd-32][3]);
			fprintf(fp_Result,"Rd:VOB_entry[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd-32, VOB[IF.Rd-32][0], VOB[IF.Rd-32][1], VOB[IF.Rd-32][2], VOB[IF.Rd-32][3]);
		}
	}
	else
	{
		if(IF.FXL==0)
		{
			fprintf(fp_Result,"Rd:GPR[%d] = %d_%d_%d_%d\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
			fprintf(fp_Result,"Rd:GPR[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
		}
		else
		{
			fprintf(fp_Result,"Rd:GPR[%d] = %f_%f_%f_%f\n", IF.Rd, *(float*)&GPR[IF.Rd][0], *(float*)&GPR[IF.Rd][1], *(float*)&GPR[IF.Rd][2], *(float*)&GPR[IF.Rd][3]);
			fprintf(fp_Result,"Rd:GPR[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
		}
	}
}

void SC_VS::VS_20_INV()
{
	fprintf(fp_Result,"Op:20 = INV\n");
	if(IF.Rd>=32)		fprintf(fp_Result,"Rd:VOB_entry[%d]\n", IF.Rd-32);
	else				fprintf(fp_Result,"Rd:GPR[%d]\n", IF.Rd);
	if(IF.Rs>=32)		fprintf(fp_Result,"Rs:VAB_entry[%d]\n", IF.Rs-32);
	else				fprintf(fp_Result,"Rs:GPR[%d]\n", IF.Rs);
	fprintf(fp_Result,"Rt:Dont care.\n"); 
	fprintf(fp_Result,"Sw_d:Dont care.\n"); 
	fprintf(fp_Result,"Sw_s:%d%d%d%d\n", IF.Sw_s_x, IF.Sw_s_y, IF.Sw_s_z, IF.Sw_s_w); 
	fprintf(fp_Result,"Sw_t:%d%d%d%d\n", IF.Sw_t_x, IF.Sw_t_y, IF.Sw_t_z, IF.Sw_t_w); 
	fprintf(fp_Result,"Neg:%d%d%d\n", IF.Neg_Rs, IF.Neg_Rt, IF.Neg_Rd);
	fprintf(fp_Result,"WM:%d%d%d%d\n", IF.WM_x, IF.WM_y, IF.WM_z, IF.WM_w);
	fprintf(fp_Result,"FXL:%d\n", IF.FXL);
	fprintf(fp_Result,"S/V:%d\n", IF.S_V);
	if(IF.Neg_Rs!=0 || IF.Neg_Rt!=0)
		fprintf(fp_Result,"***Neg must be 00X***\n");

	//------------Initial variable--------------------------------------
	for(i=0;i<=3;i++)
	{
		inv_temp[i] = 0;
		Rs_SV_temp[i] = 0;
		Rs_swiz_temp[i] = 0;

	}
	//S_V modified by TSAO
	//Load Rs
	Rs_SV_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0];
	Rs_SV_temp[1] = (IF.Rs>=32) ? VAB[IF.Rs-32][1] : GPR[IF.Rs][1];
	Rs_SV_temp[2] = (IF.Rs>=32) ? VAB[IF.Rs-32][2] : GPR[IF.Rs][2];
	Rs_SV_temp[3] = (IF.Rs>=32) ? VAB[IF.Rs-32][3] : GPR[IF.Rs][3];

	//----------------S/V---------------------------------
	for(i=0; i<=3; i++)
	{
		if(i>IF.S_V)
		{
			Rs_SV_temp[i] = 0;
		}
	}
	//Rs do swizzle
	switch(IF.Sw_s_x) {
	case 0:	Rs_swiz_temp[0] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[0] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[0] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[0] = Rs_SV_temp[3];break;
	}
	switch(IF.Sw_s_y) {
	case 0:	Rs_swiz_temp[1] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[1] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[1] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[1] = Rs_SV_temp[3];break;
	}
	switch(IF.Sw_s_z) {
	case 0:	Rs_swiz_temp[2] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[2] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[2] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[2] = Rs_SV_temp[3];break;
	}
	switch(IF.Sw_s_w) {
	case 0:	Rs_swiz_temp[3] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[3] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[3] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[3] = Rs_SV_temp[3];break;
	}
	
	if(IF.Rs>=32)
		fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, VAB[IF.Rs-32][0], VAB[IF.Rs-32][1], VAB[IF.Rs-32][2], VAB[IF.Rs-32][3]);
	else
		fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs, GPR[IF.Rs][0], GPR[IF.Rs][1], GPR[IF.Rs][2], GPR[IF.Rs][3]);

	fprintf(fp_Result, "After Swizzle:\n");
	if(IF.Rs>=32)
		fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
	else
		fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs,Rs_swiz_temp[0],Rs_swiz_temp[1],Rs_swiz_temp[2],Rs_swiz_temp[3]);

	//------------INV Function---------------------------------------
	for(i=0;i<=3;i++)
		inv_temp[i] = ~Rs_swiz_temp[i];

	//------------ WB --------------------------------------------------
	if(IF.WM_x==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][0] = inv_temp[0];
		else			GPR[IF.Rd][0] = inv_temp[0];
	}
	if(IF.WM_y==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][1] = inv_temp[1];
		else			GPR[IF.Rd][1] = inv_temp[1];
	}
	if(IF.WM_z==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][2] = inv_temp[2];
		else			GPR[IF.Rd][2] = inv_temp[2];
	}
	if(IF.WM_w==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][3] = inv_temp[3];
		else			GPR[IF.Rd][3] = inv_temp[3];
	}
	fprintf(fp_Result,"-----------------------------------------------------------------\n");
	if(IF.Rd>=32)
		fprintf(fp_Result,"Rd:VOB_entry[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd-32, VOB[IF.Rd-32][0], VOB[IF.Rd-32][1], VOB[IF.Rd-32][2], VOB[IF.Rd-32][3]);
	else
		fprintf(fp_Result,"Rd:GPR[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
}

void SC_VS::VS_21_AND()
{
	fprintf(fp_Result,"Op:21 = AND\n");
	if(IF.Rd>=32)		fprintf(fp_Result,"Rd:VOB_entry[%d]\n", IF.Rd-32);
	else				fprintf(fp_Result,"Rd:GPR[%d]\n", IF.Rd);
	if(IF.Rs>=32)		fprintf(fp_Result,"Rs:VAB_entry[%d]\n", IF.Rs-32);
	else				fprintf(fp_Result,"Rs:GPR[%d]\n", IF.Rs);
	if(IF.Rt>=32)		fprintf(fp_Result,"Rt:VAB_entry[%d]\n", IF.Rt-32);
	else				fprintf(fp_Result,"Rt:GPR[%d]\n", IF.Rt);
	fprintf(fp_Result,"Sw_d:Dont care.\n"); 
	fprintf(fp_Result,"Sw_s:%d%d%d%d\n", IF.Sw_s_x, IF.Sw_s_y, IF.Sw_s_z, IF.Sw_s_w); 
	fprintf(fp_Result,"Sw_t:%d%d%d%d\n", IF.Sw_t_x, IF.Sw_t_y, IF.Sw_t_z, IF.Sw_t_w); 
	fprintf(fp_Result,"Neg:%d%d%d\n", IF.Neg_Rs, IF.Neg_Rt, IF.Neg_Rd);
	fprintf(fp_Result,"WM:%d%d%d%d\n", IF.WM_x, IF.WM_y, IF.WM_z, IF.WM_w);
	fprintf(fp_Result,"FXL:%d\n", IF.FXL);
	fprintf(fp_Result,"S/V:%d\n", IF.S_V);
	if(IF.Neg_Rs!=0 || IF.Neg_Rt!=0)
		fprintf(fp_Result,"***Neg must be 00X***\n");

	//------------Initial variable--------------------------------------
	for(i=0;i<=3;i++)
	{
		and_temp[i] = 0;
		Rs_SV_temp[i] = 0;
		Rt_SV_temp[i] = 0;
		Rs_swiz_temp[i] = 0;
		Rt_swiz_temp[i] = 0;

	}
	//S_V modified by TSAO
	//Load Rs
	Rs_SV_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0];
	Rs_SV_temp[1] = (IF.Rs>=32) ? VAB[IF.Rs-32][1] : GPR[IF.Rs][1];
	Rs_SV_temp[2] = (IF.Rs>=32) ? VAB[IF.Rs-32][2] : GPR[IF.Rs][2];
	Rs_SV_temp[3] = (IF.Rs>=32) ? VAB[IF.Rs-32][3] : GPR[IF.Rs][3];
	//Load Rt
	Rt_SV_temp[0] = (IF.Rt>=32) ? VAB[IF.Rt-32][0] : GPR[IF.Rt][0];
	Rt_SV_temp[1] = (IF.Rt>=32) ? VAB[IF.Rt-32][1] : GPR[IF.Rt][1];
	Rt_SV_temp[2] = (IF.Rt>=32) ? VAB[IF.Rt-32][2] : GPR[IF.Rt][2];
	Rt_SV_temp[3] = (IF.Rt>=32) ? VAB[IF.Rt-32][3] : GPR[IF.Rt][3];
	//----------------S/V---------------------------------
	for(i=0; i<=3; i++)
	{
		if(i>IF.S_V)
		{
			Rs_SV_temp[i] = 0;
			Rt_SV_temp[i] = 0;
		}
	}
	//Rs do swizzle
	switch(IF.Sw_s_x) {
	case 0:	Rs_swiz_temp[0] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[0] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[0] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[0] = Rs_SV_temp[3];break;
	}
	switch(IF.Sw_s_y) {
	case 0:	Rs_swiz_temp[1] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[1] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[1] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[1] = Rs_SV_temp[3];break;
	}
	switch(IF.Sw_s_z) {
	case 0:	Rs_swiz_temp[2] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[2] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[2] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[2] = Rs_SV_temp[3];break;
	}
	switch(IF.Sw_s_w) {
	case 0:	Rs_swiz_temp[3] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[3] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[3] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[3] = Rs_SV_temp[3];break;
	}
	
	//Rt do swizzle
	switch(IF.Sw_t_x) {
	case 0:	Rt_swiz_temp[0] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[0] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[0] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[0] = Rt_SV_temp[3];break;
	}
	switch(IF.Sw_t_y) {
	case 0:	Rt_swiz_temp[1] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[1] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[1] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[1] = Rt_SV_temp[3];break;
	}
	switch(IF.Sw_t_z) {
	case 0:	Rt_swiz_temp[2] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[2] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[2] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[2] = Rt_SV_temp[3];break;
	}
	switch(IF.Sw_t_w) {
	case 0:	Rt_swiz_temp[3] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[3] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[3] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[3] = Rt_SV_temp[3];break;
	}

	if(IF.Rs>=32)
		fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, VAB[IF.Rs-32][0], VAB[IF.Rs-32][1], VAB[IF.Rs-32][2], VAB[IF.Rs-32][3]);
	else
		fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs, GPR[IF.Rs][0], GPR[IF.Rs][1], GPR[IF.Rs][2], GPR[IF.Rs][3]);

	if(IF.Rt>=32)
		fprintf(fp_Result,"Rt:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt-32, VAB[IF.Rt-32][0], VAB[IF.Rt-32][1], VAB[IF.Rt-32][2], VAB[IF.Rt-32][3]);
	else
		fprintf(fp_Result,"Rt:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt, GPR[IF.Rt][0], GPR[IF.Rt][1], GPR[IF.Rt][2], GPR[IF.Rt][3]);

	fprintf(fp_Result, "After Swizzle:\n");
	if(IF.Rs>=32) 
		fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
	else
		fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs,Rs_swiz_temp[0],Rs_swiz_temp[1],Rs_swiz_temp[2],Rs_swiz_temp[3]);

	if(IF.Rt>=32)
		fprintf(fp_Result,"Rt:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt-32, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
	else
		fprintf(fp_Result,"Rt:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);

	//------------AND Function---------------------------------------
	for(i=0;i<=3;i++)
		and_temp[i] = Rs_swiz_temp[i] & Rt_swiz_temp[i];

	//------------ WB --------------------------------------------------
	if(IF.WM_x==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][0] = and_temp[0];
		else			GPR[IF.Rd][0] = and_temp[0];
	}
	if(IF.WM_y==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][1] = and_temp[1];
		else			GPR[IF.Rd][1] = and_temp[1];
	}
	if(IF.WM_z==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][2] = and_temp[2];
		else			GPR[IF.Rd][2] = and_temp[2];
	}
	if(IF.WM_w==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][3] = and_temp[3];
		else			GPR[IF.Rd][3] = and_temp[3];
	}
	fprintf(fp_Result,"-----------------------------------------------------------------\n");
	if(IF.Rd>=32)
		fprintf(fp_Result,"Rd:VOB_entry[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd-32, VOB[IF.Rd-32][0], VOB[IF.Rd-32][1], VOB[IF.Rd-32][2], VOB[IF.Rd-32][3]);
	else
		fprintf(fp_Result,"Rd:GPR[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
}

void SC_VS::VS_22_OR()
{
	fprintf(fp_Result,"Op:22 = OR\n");
	if(IF.Rd>=32)		fprintf(fp_Result,"Rd:VOB_entry[%d]\n", IF.Rd-32);
	else				fprintf(fp_Result,"Rd:GPR[%d]\n", IF.Rd);
	if(IF.Rs>=32)		fprintf(fp_Result,"Rs:VAB_entry[%d]\n", IF.Rs-32);
	else				fprintf(fp_Result,"Rs:GPR[%d]\n", IF.Rs);
	if(IF.Rt>=32)		fprintf(fp_Result,"Rt:VAB_entry[%d]\n", IF.Rt-32);
	else				fprintf(fp_Result,"Rt:GPR[%d]\n", IF.Rt);
	fprintf(fp_Result,"Sw_d:Dont care.\n"); 
	fprintf(fp_Result,"Sw_s:%d%d%d%d\n", IF.Sw_s_x, IF.Sw_s_y, IF.Sw_s_z, IF.Sw_s_w); 
	fprintf(fp_Result,"Sw_t:%d%d%d%d\n", IF.Sw_t_x, IF.Sw_t_y, IF.Sw_t_z, IF.Sw_t_w); 
	fprintf(fp_Result,"Neg:%d%d%d\n", IF.Neg_Rs, IF.Neg_Rt, IF.Neg_Rd);
	fprintf(fp_Result,"WM:%d%d%d%d\n", IF.WM_x, IF.WM_y, IF.WM_z, IF.WM_w);
	fprintf(fp_Result,"FXL:%d\n", IF.FXL);
	fprintf(fp_Result,"S/V:%d\n", IF.S_V);
	if(IF.Neg_Rs!=0 || IF.Neg_Rt!=0)
		fprintf(fp_Result,"***Neg must be 00X***\n");

	//------------Initial variable--------------------------------------
	for(i=0;i<=3;i++)
	{
		or_temp[i] = 0;
		Rs_SV_temp[i] = 0;
		Rt_SV_temp[i] = 0;
		Rs_swiz_temp[i] = 0;
		Rt_swiz_temp[i] = 0;

	}
	//S_V modified by TSAO
	//Load Rs
	Rs_SV_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0];
	Rs_SV_temp[1] = (IF.Rs>=32) ? VAB[IF.Rs-32][1] : GPR[IF.Rs][1];
	Rs_SV_temp[2] = (IF.Rs>=32) ? VAB[IF.Rs-32][2] : GPR[IF.Rs][2];
	Rs_SV_temp[3] = (IF.Rs>=32) ? VAB[IF.Rs-32][3] : GPR[IF.Rs][3];
	//Load Rt
	Rt_SV_temp[0] = (IF.Rt>=32) ? VAB[IF.Rt-32][0] : GPR[IF.Rt][0];
	Rt_SV_temp[1] = (IF.Rt>=32) ? VAB[IF.Rt-32][1] : GPR[IF.Rt][1];
	Rt_SV_temp[2] = (IF.Rt>=32) ? VAB[IF.Rt-32][2] : GPR[IF.Rt][2];
	Rt_SV_temp[3] = (IF.Rt>=32) ? VAB[IF.Rt-32][3] : GPR[IF.Rt][3];
	//----------------S/V---------------------------------
	for(i=0; i<=3; i++)
	{
		if(i>IF.S_V)
		{
			Rs_SV_temp[i] = 0;
			Rt_SV_temp[i] = 0;
		}
	}
	//Rs do swizzle
	switch(IF.Sw_s_x) {
	case 0:	Rs_swiz_temp[0] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[0] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[0] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[0] = Rs_SV_temp[3];break;
	}
	switch(IF.Sw_s_y) {
	case 0:	Rs_swiz_temp[1] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[1] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[1] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[1] = Rs_SV_temp[3];break;
	}
	switch(IF.Sw_s_z) {
	case 0:	Rs_swiz_temp[2] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[2] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[2] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[2] = Rs_SV_temp[3];break;
	}
	switch(IF.Sw_s_w) {
	case 0:	Rs_swiz_temp[3] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[3] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[3] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[3] = Rs_SV_temp[3];break;
	}
	
	//Rt do swizzle
	switch(IF.Sw_t_x) {
	case 0:	Rt_swiz_temp[0] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[0] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[0] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[0] = Rt_SV_temp[3];break;
	}
	switch(IF.Sw_t_y) {
	case 0:	Rt_swiz_temp[1] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[1] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[1] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[1] = Rt_SV_temp[3];break;
	}
	switch(IF.Sw_t_z) {
	case 0:	Rt_swiz_temp[2] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[2] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[2] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[2] = Rt_SV_temp[3];break;
	}
	switch(IF.Sw_t_w) {
	case 0:	Rt_swiz_temp[3] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[3] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[3] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[3] = Rt_SV_temp[3];break;
	}

	if(IF.Rs>=32)
		fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, VAB[IF.Rs-32][0], VAB[IF.Rs-32][1], VAB[IF.Rs-32][2], VAB[IF.Rs-32][3]);
	else
		fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs, GPR[IF.Rs][0], GPR[IF.Rs][1], GPR[IF.Rs][2], GPR[IF.Rs][3]);

	if(IF.Rt>=32)
		fprintf(fp_Result,"Rt:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt-32, VAB[IF.Rt-32][0], VAB[IF.Rt-32][1], VAB[IF.Rt-32][2], VAB[IF.Rt-32][3]);
	else
		fprintf(fp_Result,"Rt:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt, GPR[IF.Rt][0], GPR[IF.Rt][1], GPR[IF.Rt][2], GPR[IF.Rt][3]);

	fprintf(fp_Result, "After Swizzle:\n");
	if(IF.Rs>=32) 
		fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
	else
		fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs,Rs_swiz_temp[0],Rs_swiz_temp[1],Rs_swiz_temp[2],Rs_swiz_temp[3]);

	if(IF.Rt>=32)
		fprintf(fp_Result,"Rt:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt-32, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
	else
		fprintf(fp_Result,"Rt:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);

	//------------OR Function---------------------------------------
	for(i=0;i<=3;i++)
		or_temp[i] = Rs_swiz_temp[i] | Rt_swiz_temp[i];

	//------------ WB --------------------------------------------------
	if(IF.WM_x==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][0] = or_temp[0];
		else			GPR[IF.Rd][0] = or_temp[0];
	}
	if(IF.WM_y==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][1] = or_temp[1];
		else			GPR[IF.Rd][1] = or_temp[1];
	}
	if(IF.WM_z==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][2] = or_temp[2];
		else			GPR[IF.Rd][2] = or_temp[2];
	}
	if(IF.WM_w==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][3] = or_temp[3];
		else			GPR[IF.Rd][3] = or_temp[3];
	}
	fprintf(fp_Result,"-----------------------------------------------------------------\n");
	if(IF.Rd>=32)
		fprintf(fp_Result,"Rd:VOB_entry[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd-32, VOB[IF.Rd-32][0], VOB[IF.Rd-32][1], VOB[IF.Rd-32][2], VOB[IF.Rd-32][3]);
	else
		fprintf(fp_Result,"Rd:GPR[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
}

void SC_VS::VS_23_XOR()
{
	fprintf(fp_Result,"Op:23 = XOR\n");
	if(IF.Rd>=32)		fprintf(fp_Result,"Rd:VOB_entry[%d]\n", IF.Rd-32);
	else				fprintf(fp_Result,"Rd:GPR[%d]\n", IF.Rd);
	if(IF.Rs>=32)		fprintf(fp_Result,"Rs:VAB_entry[%d]\n", IF.Rs-32);
	else				fprintf(fp_Result,"Rs:GPR[%d]\n", IF.Rs);
	if(IF.Rt>=32)		fprintf(fp_Result,"Rt:VAB_entry[%d]\n", IF.Rt-32);
	else				fprintf(fp_Result,"Rt:GPR[%d]\n", IF.Rt);
	fprintf(fp_Result,"Sw_d:Dont care.\n"); 
	fprintf(fp_Result,"Sw_s:%d%d%d%d\n", IF.Sw_s_x, IF.Sw_s_y, IF.Sw_s_z, IF.Sw_s_w); 
	fprintf(fp_Result,"Sw_t:%d%d%d%d\n", IF.Sw_t_x, IF.Sw_t_y, IF.Sw_t_z, IF.Sw_t_w); 
	fprintf(fp_Result,"Neg:%d%d%d\n", IF.Neg_Rs, IF.Neg_Rt, IF.Neg_Rd);
	fprintf(fp_Result,"WM:%d%d%d%d\n", IF.WM_x, IF.WM_y, IF.WM_z, IF.WM_w);
	fprintf(fp_Result,"FXL:%d\n", IF.FXL);
	fprintf(fp_Result,"S/V:%d\n", IF.S_V);
	if(IF.Neg_Rs!=0 || IF.Neg_Rt!=0)
		fprintf(fp_Result,"***Neg must be 00X***\n");

	//------------Initial variable--------------------------------------
	for(i=0;i<=3;i++)
	{
		xor_temp[i] = 0;
		Rs_SV_temp[i] = 0;
		Rt_SV_temp[i] = 0;
		Rs_swiz_temp[i] = 0;
		Rt_swiz_temp[i] = 0;

	}
	//S_V modified by TSAO
	//Load Rs
	Rs_SV_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0];
	Rs_SV_temp[1] = (IF.Rs>=32) ? VAB[IF.Rs-32][1] : GPR[IF.Rs][1];
	Rs_SV_temp[2] = (IF.Rs>=32) ? VAB[IF.Rs-32][2] : GPR[IF.Rs][2];
	Rs_SV_temp[3] = (IF.Rs>=32) ? VAB[IF.Rs-32][3] : GPR[IF.Rs][3];
	//Load Rt
	Rt_SV_temp[0] = (IF.Rt>=32) ? VAB[IF.Rt-32][0] : GPR[IF.Rt][0];
	Rt_SV_temp[1] = (IF.Rt>=32) ? VAB[IF.Rt-32][1] : GPR[IF.Rt][1];
	Rt_SV_temp[2] = (IF.Rt>=32) ? VAB[IF.Rt-32][2] : GPR[IF.Rt][2];
	Rt_SV_temp[3] = (IF.Rt>=32) ? VAB[IF.Rt-32][3] : GPR[IF.Rt][3];
	//----------------S/V---------------------------------
	for(i=0; i<=3; i++)
	{
		if(i>IF.S_V)
		{
			Rs_SV_temp[i] = 0;
			Rt_SV_temp[i] = 0;
		}
	}
	//Rs do swizzle
	switch(IF.Sw_s_x) {
	case 0:	Rs_swiz_temp[0] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[0] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[0] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[0] = Rs_SV_temp[3];break;
	}
	switch(IF.Sw_s_y) {
	case 0:	Rs_swiz_temp[1] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[1] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[1] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[1] = Rs_SV_temp[3];break;
	}
	switch(IF.Sw_s_z) {
	case 0:	Rs_swiz_temp[2] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[2] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[2] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[2] = Rs_SV_temp[3];break;
	}
	switch(IF.Sw_s_w) {
	case 0:	Rs_swiz_temp[3] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[3] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[3] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[3] = Rs_SV_temp[3];break;
	}
	
	//Rt do swizzle
	switch(IF.Sw_t_x) {
	case 0:	Rt_swiz_temp[0] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[0] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[0] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[0] = Rt_SV_temp[3];break;
	}
	switch(IF.Sw_t_y) {
	case 0:	Rt_swiz_temp[1] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[1] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[1] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[1] = Rt_SV_temp[3];break;
	}
	switch(IF.Sw_t_z) {
	case 0:	Rt_swiz_temp[2] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[2] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[2] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[2] = Rt_SV_temp[3];break;
	}
	switch(IF.Sw_t_w) {
	case 0:	Rt_swiz_temp[3] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[3] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[3] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[3] = Rt_SV_temp[3];break;
	}

	if(IF.Rs>=32)
		fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, VAB[IF.Rs-32][0], VAB[IF.Rs-32][1], VAB[IF.Rs-32][2], VAB[IF.Rs-32][3]);
	else
		fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs, GPR[IF.Rs][0], GPR[IF.Rs][1], GPR[IF.Rs][2], GPR[IF.Rs][3]);

	if(IF.Rt>=32)
		fprintf(fp_Result,"Rt:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt-32, VAB[IF.Rt-32][0], VAB[IF.Rt-32][1], VAB[IF.Rt-32][2], VAB[IF.Rt-32][3]);
	else
		fprintf(fp_Result,"Rt:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt, GPR[IF.Rt][0], GPR[IF.Rt][1], GPR[IF.Rt][2], GPR[IF.Rt][3]);

	fprintf(fp_Result, "After Swizzle:\n");
	if(IF.Rs>=32) 
		fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
	else
		fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs,Rs_swiz_temp[0],Rs_swiz_temp[1],Rs_swiz_temp[2],Rs_swiz_temp[3]);

	if(IF.Rt>=32)
		fprintf(fp_Result,"Rt:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt-32, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
	else
		fprintf(fp_Result,"Rt:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);

	//------------XOR Function---------------------------------------
	for(i=0;i<=3;i++)
		xor_temp[i] = Rs_swiz_temp[i] ^ Rt_swiz_temp[i];

	//------------ WB --------------------------------------------------
	if(IF.WM_x==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][0] = xor_temp[0];
		else			GPR[IF.Rd][0] = xor_temp[0];
	}
	if(IF.WM_y==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][1] = xor_temp[1];
		else			GPR[IF.Rd][1] = xor_temp[1];
	}
	if(IF.WM_z==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][2] = xor_temp[2];
		else			GPR[IF.Rd][2] = xor_temp[2];
	}
	if(IF.WM_w==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][3] = xor_temp[3];
		else			GPR[IF.Rd][3] = xor_temp[3];
	}
	fprintf(fp_Result,"-----------------------------------------------------------------\n");
	if(IF.Rd>=32)
		fprintf(fp_Result,"Rd:VOB_entry[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd-32, VOB[IF.Rd-32][0], VOB[IF.Rd-32][1], VOB[IF.Rd-32][2], VOB[IF.Rd-32][3]);
	else
		fprintf(fp_Result,"Rd:GPR[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
}

void SC_VS::VS_24_BEQ()
{
    fprintf(fp_Result,"Op:24 = BEQ\n");
    if(IF.R1>=32)		fprintf(fp_Result,"R1:VOB_entry[%d]\n", IF.R1-32);
    else				fprintf(fp_Result,"R1:GPR[%d]\n", IF.R1);
    if(IF.R2>=32)		fprintf(fp_Result,"R2:VAB_entry[%d]\n", IF.R2-32);
    else				fprintf(fp_Result,"R2:GPR[%d]\n", IF.R2);
    if(IF.R3>=32)		fprintf(fp_Result,"R3:VAB_entry[%d]\n", IF.R3-32);
    else				fprintf(fp_Result,"R3:GPR[%d]\n", IF.R3);
    fprintf(fp_Result,"R4:GPR[%d]\n", IF.R4);
    fprintf(fp_Result,"Addr:[%d]\n", ((short signed)IF.Addr));
    fprintf(fp_Result,"SCC:%d%d%d%d\n", IF.SCC_x, IF.SCC_y, IF.SCC_z, IF.SCC_w);
    fprintf(fp_Result,"FXL:%d\n", IF.FXL);
    fprintf(fp_Result,"S/V:%d\n", IF.S_V);

    //-----------Initial variable------------------------------------------
    for(i=0; i<=3; i++)
    {
		R1_SV_temp[i] = 0;
		R2_SV_temp[i] = 0;
		R3_SV_temp[i] = 0;
		R4_SV_temp[i] = 0;
        R1_temp[i] = 0;
        R2_temp[i] = 0;
        R3_temp[i] = 0;
        R4_temp[i] = 0;
        R1_temp_FL[i] = 0;
        R2_temp_FL[i] = 0;
        R3_temp_FL[i] = 0;
        R4_temp_FL[i] = 0;
        R1_temp_FX[i] = 0;
        R2_temp_FX[i] = 0;
        R3_temp_FX[i] = 0;
        R4_temp_FX[i] = 0;
        branch_taken[i] = 0;
    }


    if(IF.Br_m==1)//BEQS
    {
        IF.SCC_x=1;IF.SCC_y=1;IF.SCC_z=1;IF.SCC_w=1;
		//S_V modified by TSAO
		//Load Rs
		R1_SV_temp[0] = (IF.R1>=32) ? VAB[IF.R1-32][0] : GPR[IF.R1][0];
		R1_SV_temp[1] = (IF.R1>=32) ? VAB[IF.R1-32][1] : GPR[IF.R1][1];
		R1_SV_temp[2] = (IF.R1>=32) ? VAB[IF.R1-32][2] : GPR[IF.R1][2];
		R1_SV_temp[3] = (IF.R1>=32) ? VAB[IF.R1-32][3] : GPR[IF.R1][3];
		//Load Rt
		R2_SV_temp[0] = (IF.R2>=32) ? VAB[IF.R2-32][0] : GPR[IF.R2][0];
		R2_SV_temp[1] = (IF.R2>=32) ? VAB[IF.R2-32][1] : GPR[IF.R2][1];
		R2_SV_temp[2] = (IF.R2>=32) ? VAB[IF.R2-32][2] : GPR[IF.R2][2];
		R2_SV_temp[3] = (IF.R2>=32) ? VAB[IF.R2-32][3] : GPR[IF.R2][3];
		//S_V
        for(i=0; i<=3; i++)
        {
            if(i>IF.S_V)
			{
				R1_SV_temp[i] = 0;
				R2_SV_temp[i] = 0;
				R3_SV_temp[i] = 0;
				R4_SV_temp[i] = 0;
            }
		}
        //Rs do swizzle
        switch(IF.Sw_s_x) {
        case 0: R1_temp[0] = R1_SV_temp[0]; break;
        case 1: R1_temp[0] = R1_SV_temp[1]; break;
        case 2: R1_temp[0] = R1_SV_temp[2]; break;
        case 3: R1_temp[0] = R1_SV_temp[3]; break;
        }
        switch(IF.Sw_s_y) {
        case 0: R1_temp[1] = R1_SV_temp[0]; break;
        case 1: R1_temp[1] = R1_SV_temp[1]; break;
        case 2: R1_temp[1] = R1_SV_temp[2]; break;
        case 3: R1_temp[1] = R1_SV_temp[3]; break;
        }
        switch(IF.Sw_s_z) {
        case 0: R1_temp[2] = R1_SV_temp[0]; break;
        case 1: R1_temp[2] = R1_SV_temp[1]; break;
        case 2: R1_temp[2] = R1_SV_temp[2]; break;
        case 3: R1_temp[2] = R1_SV_temp[3];break;
        }
        switch(IF.Sw_s_w) {
        case 0: R1_temp[3] = R1_SV_temp[0]; break;
        case 1: R1_temp[3] = R1_SV_temp[1]; break;
        case 2: R1_temp[3] = R1_SV_temp[2]; break;
        case 3: R1_temp[3] = R1_SV_temp[3]; break;
        }
        //Rt do swizzle
        switch(IF.Sw_t_x) {
        case 0: R2_temp[0] = R2_SV_temp[0]; break;
        case 1: R2_temp[0] = R2_SV_temp[1]; break;
        case 2: R2_temp[0] = R2_SV_temp[2]; break;
        case 3: R2_temp[0] = R2_SV_temp[3]; break;
        }
        switch(IF.Sw_t_y) {
        case 0: R2_temp[1] = R2_SV_temp[0]; break;
        case 1: R2_temp[1] = R2_SV_temp[1]; break;
        case 2: R2_temp[1] = R2_SV_temp[2]; break;
        case 3: R2_temp[1] = R2_SV_temp[3]; break;
        }
        switch(IF.Sw_t_z) {
        case 0: R2_temp[2] = R2_SV_temp[0]; break;
        case 1: R2_temp[2] = R2_SV_temp[1]; break;
        case 2: R2_temp[2] = R2_SV_temp[2]; break;
        case 3: R2_temp[2] = R2_SV_temp[3]; break;
        }
        switch(IF.Sw_t_w) {
        case 0: R2_temp[3] = R2_SV_temp[0]; break;
        case 1: R2_temp[3] = R2_SV_temp[1]; break;
        case 2: R2_temp[3] = R2_SV_temp[2]; break;
        case 3: R2_temp[3] = R2_SV_temp[3]; break;
        }
        
	    for(i=0; i<=3; i++)
		{
		R3_temp[i] = R1_temp[i];
	    R4_temp[i] = R2_temp[i];
		}
    }
    else//BEQV
    {
        //IF.Sw_s_x=0;IF.Sw_s_y=1;IF.Sw_s_z=2;IF.Sw_s_w=3;IF.Sw_t_x=0;IF.Sw_t_y=1;IF.Sw_t_z=2;IF.Sw_t_w=3;
	for(i=0; i<=IF.S_V; i++)
	{
	  R1_temp[i] = (IF.R1>=32) ? VAB[IF.R1-32][i] : GPR[IF.R1][i];
	  R2_temp[i] = (IF.R2>=32) ? VAB[IF.R2-32][i] : GPR[IF.R2][i];
	  R3_temp[i] = (IF.R3>=32) ? VAB[IF.R3-32][i] : GPR[IF.R3][i];
	  R4_temp[i] = GPR[IF.R4][i];//---R4 only support read from GPR
	}
    }

    //------------determine the source----------------------------------
    /*for(i=0; i<=IF.S_V; i++)
    {
        R1_temp[i] = (IF.R1>=32) ? VAB[IF.R1-32][i] : GPR[IF.R1][i];
        R2_temp[i] = (IF.R2>=32) ? VAB[IF.R2-32][i] : GPR[IF.R2][i];
        R3_temp[i] = (IF.R3>=32) ? VAB[IF.R3-32][i] : GPR[IF.R3][i];
        R4_temp[i] = GPR[IF.R4][i];//---R4 only support read from GPR
    }*/

    //-----------compare component x---------------------------------------
    if(IF.SCC_x==1)
    {
        if(IF.FXL==1)
        {
            R1_temp_FL[0] = *((float*)((void*)&R1_temp[0]));
            R2_temp_FL[0] = *((float*)((void*)&R2_temp[0]));
            R3_temp_FL[0] = *((float*)((void*)&R3_temp[0]));
            R4_temp_FL[0] = *((float*)((void*)&R4_temp[0]));
            if((R1_temp_FL[0]==R2_temp_FL[0]) && (R3_temp_FL[0]==R4_temp_FL[0]))
                branch_taken[0] = 1;
        }
        else
        {
            R1_temp_FX[0] = *((int*)((void*)&R1_temp[0]));
            R2_temp_FX[0] = *((int*)((void*)&R2_temp[0]));
            R3_temp_FX[0] = *((int*)((void*)&R3_temp[0]));
            R4_temp_FX[0] = *((int*)((void*)&R4_temp[0]));
            if((R1_temp_FX[0]==R2_temp_FX[0]) && (R3_temp_FX[0]==R4_temp_FX[0]))
                branch_taken[0] = 1;
        }
    }

    //-----------compare component y---------------------------------------
    if(IF.SCC_y==1)
    {
        if(IF.FXL==1)
        {
            R1_temp_FL[1] = *((float*)((void*)&R1_temp[1]));
            R2_temp_FL[1] = *((float*)((void*)&R2_temp[1]));
            R3_temp_FL[1] = *((float*)((void*)&R3_temp[1]));
            R4_temp_FL[1] = *((float*)((void*)&R4_temp[1]));
            if((R1_temp_FL[1]==R2_temp_FL[1]) && (R3_temp_FL[1]==R4_temp_FL[1]))
                branch_taken[1] = 1;
        }
        else
        {
            R1_temp_FX[1] = *((int*)((void*)&R1_temp[1]));
            R2_temp_FX[1] = *((int*)((void*)&R2_temp[1]));
            R3_temp_FX[1] = *((int*)((void*)&R3_temp[1]));
            R4_temp_FX[1] = *((int*)((void*)&R4_temp[1]));
            if((R1_temp_FX[1]==R2_temp_FX[1]) && (R3_temp_FX[1]==R4_temp_FX[1]))
                branch_taken[1] = 1;
        }
    }

    //-----------compare component z---------------------------------------
    if(IF.SCC_z==1)
    {
        if(IF.FXL==1)
        {
            R1_temp_FL[2] = *((float*)((void*)&R1_temp[2]));
            R2_temp_FL[2] = *((float*)((void*)&R2_temp[2]));
            R3_temp_FL[2] = *((float*)((void*)&R3_temp[2]));
            R4_temp_FL[2] = *((float*)((void*)&R4_temp[2]));
            if((R1_temp_FL[2]==R2_temp_FL[2]) && (R3_temp_FL[2]==R4_temp_FL[2]))
                branch_taken[2] = 1;
        }
        else
        {
            R1_temp_FX[2] = *((int*)((void*)&R1_temp[2]));
            R2_temp_FX[2] = *((int*)((void*)&R2_temp[2]));
            R3_temp_FX[2] = *((int*)((void*)&R3_temp[2]));
            R4_temp_FX[2] = *((int*)((void*)&R4_temp[2]));
            if((R1_temp_FX[2]==R2_temp_FX[2]) && (R3_temp_FX[2]==R4_temp_FX[2]))
                branch_taken[2] = 1;
        }
    }

    //-----------compare component w---------------------------------------
    if(IF.SCC_w==1)
    {
        if(IF.FXL==1)
        {
            R1_temp_FL[3] = *((float*)((void*)&R1_temp[3]));
            R2_temp_FL[3] = *((float*)((void*)&R2_temp[3]));
            R3_temp_FL[3] = *((float*)((void*)&R3_temp[3]));
            R4_temp_FL[3] = *((float*)((void*)&R4_temp[3]));
            if((R1_temp_FL[3]==R2_temp_FL[3]) && (R3_temp_FL[3]==R4_temp_FL[3]))
                branch_taken[3] = 1;
        }
        else
        {
            R1_temp_FX[3] = *((int*)((void*)&R1_temp[3]));
            R2_temp_FX[3] = *((int*)((void*)&R2_temp[3]));
            R3_temp_FX[3] = *((int*)((void*)&R3_temp[3]));
            R4_temp_FX[3] = *((int*)((void*)&R4_temp[3]));
            if((R1_temp_FX[3]==R2_temp_FX[3]) && (R3_temp_FX[3]==R4_temp_FX[3]))
                branch_taken[3] = 1;
        }
    }

    //-----------determein branch or not---------------------------------------
    if(branch_taken[0]==IF.SCC_x && branch_taken[1]==IF.SCC_y && branch_taken[2]==IF.SCC_z && branch_taken[3]==IF.SCC_w)
        PC = PC+(short signed)IF.Addr;

    //----------print for debug------------------------------------------------------------------------
    if(IF.FXL==1)
    {
        fprintf(fp_Result,"R1_GPR[%d]:%f_%f_%f_%f\n", IF.R1, *(float*)&R1_temp[0], *(float*)&R1_temp[1], *(float*)&R1_temp[2], *(float*)&R1_temp[3]);
        fprintf(fp_Result,"R2_GPR[%d]:%f_%f_%f_%f\n", IF.R2, *(float*)&R2_temp[0], *(float*)&R2_temp[1], *(float*)&R2_temp[2], *(float*)&R2_temp[3]);
        fprintf(fp_Result,"R3_GPR[%d]:%f_%f_%f_%f\n", IF.R3, *(float*)&R3_temp[0], *(float*)&R3_temp[1], *(float*)&R3_temp[2], *(float*)&R3_temp[3]);
        fprintf(fp_Result,"R4_GPR[%d]:%f_%f_%f_%f\n", IF.R4, *(float*)&R4_temp[0], *(float*)&R4_temp[1], *(float*)&R4_temp[2], *(float*)&R4_temp[3]);
    }
    else
    {
        fprintf(fp_Result,"R1_GPR[%d]:%d_%d_%d_%d\n", IF.R1, R1_temp[0], R1_temp[1], R1_temp[2], R1_temp[3]);
        fprintf(fp_Result,"R2_GPR[%d]:%d_%d_%d_%d\n", IF.R2, R2_temp[0], R2_temp[1], R2_temp[2], R2_temp[3]);
        fprintf(fp_Result,"R3_GPR[%d]:%d_%d_%d_%d\n", IF.R3, R3_temp[0], R3_temp[1], R3_temp[2], R3_temp[3]);
        fprintf(fp_Result,"R4_GPR[%d]:%d_%d_%d_%d\n", IF.R4, R4_temp[0], R4_temp[1], R4_temp[2], R4_temp[3]);
    }

    fprintf(fp_Result,"-----------------------------------------------------------------\n");
    //------------ out -----------------
    if(branch_taken[0]==IF.SCC_x && branch_taken[1]==IF.SCC_y && branch_taken[2]==IF.SCC_z && branch_taken[3]==IF.SCC_w)
    {
        fprintf(fp_Result,"branch:%d%d%d%d = SCC:%d%d%d%d\n", branch_taken[0], branch_taken[1], branch_taken[2], branch_taken[3], IF.SCC_x, IF.SCC_y, IF.SCC_z, IF.SCC_w);
        fprintf(fp_Result,"New PC = %d\n", PC);
    }
    else
    {
        fprintf(fp_Result,"branch:%d%d%d%d != SCC:%d%d%d%d\n", branch_taken[0], branch_taken[1], branch_taken[2], branch_taken[3], IF.SCC_x, IF.SCC_y, IF.SCC_z, IF.SCC_w);
        fprintf(fp_Result,"branch not taken.\n");
    }
}

void SC_VS::VS_25_BNEQ()
{
	fprintf(fp_Result,"Op:25 = BNEQ\n");
	if(IF.R1>=32)		fprintf(fp_Result,"R1:VOB_entry[%d]\n", IF.R1-32);
	else				fprintf(fp_Result,"R1:GPR[%d]\n", IF.R1);
	if(IF.R2>=32)		fprintf(fp_Result,"R2:VAB_entry[%d]\n", IF.R2-32);
	else				fprintf(fp_Result,"R2:GPR[%d]\n", IF.R2);
	if(IF.R3>=32)		fprintf(fp_Result,"R3:VAB_entry[%d]\n", IF.R3-32);
	else				fprintf(fp_Result,"R3:GPR[%d]\n", IF.R3);	
	fprintf(fp_Result,"R4:GPR[%d]\n", IF.R4);
	fprintf(fp_Result,"Addr:[%d]\n", ((short signed)IF.Addr));
	fprintf(fp_Result,"SCC:%d%d%d%d\n", IF.SCC_x, IF.SCC_y, IF.SCC_z, IF.SCC_w);
	fprintf(fp_Result,"FXL:%d\n", IF.FXL);
	fprintf(fp_Result,"S/V:%d\n", IF.S_V);

    //-----------Initial variable------------------------------------------
    for(i=0; i<=3; i++)
    {
		R1_SV_temp[i] = 0;
		R2_SV_temp[i] = 0;
		R3_SV_temp[i] = 0;
		R4_SV_temp[i] = 0;
        R1_temp[i] = 0;
        R2_temp[i] = 0;
        R3_temp[i] = 0;
        R4_temp[i] = 0;
        R1_temp_FL[i] = 0;
        R2_temp_FL[i] = 0;
        R3_temp_FL[i] = 0;
        R4_temp_FL[i] = 0;
        R1_temp_FX[i] = 0;
        R2_temp_FX[i] = 0;
        R3_temp_FX[i] = 0;
        R4_temp_FX[i] = 0;
        branch_taken[i] = 0;
    }


    if(IF.Br_m==1)//BEQS
    {
        IF.SCC_x=1;IF.SCC_y=1;IF.SCC_z=1;IF.SCC_w=1;
		//S_V modified by TSAO
		//Load Rs
		R1_SV_temp[0] = (IF.R1>=32) ? VAB[IF.R1-32][0] : GPR[IF.R1][0];
		R1_SV_temp[1] = (IF.R1>=32) ? VAB[IF.R1-32][1] : GPR[IF.R1][1];
		R1_SV_temp[2] = (IF.R1>=32) ? VAB[IF.R1-32][2] : GPR[IF.R1][2];
		R1_SV_temp[3] = (IF.R1>=32) ? VAB[IF.R1-32][3] : GPR[IF.R1][3];
		//Load Rt
		R2_SV_temp[0] = (IF.R2>=32) ? VAB[IF.R2-32][0] : GPR[IF.R2][0];
		R2_SV_temp[1] = (IF.R2>=32) ? VAB[IF.R2-32][1] : GPR[IF.R2][1];
		R2_SV_temp[2] = (IF.R2>=32) ? VAB[IF.R2-32][2] : GPR[IF.R2][2];
		R2_SV_temp[3] = (IF.R2>=32) ? VAB[IF.R2-32][3] : GPR[IF.R2][3];
		//S_V
        for(i=0; i<=3; i++)
        {
            if(i>IF.S_V)
			{
				R1_SV_temp[i] = 0;
				R2_SV_temp[i] = 0;
				R3_SV_temp[i] = 0;
				R4_SV_temp[i] = 0;
            }
		}
        //Rs do swizzle
        switch(IF.Sw_s_x) {
        case 0: R1_temp[0] = R1_SV_temp[0]; break;
        case 1: R1_temp[0] = R1_SV_temp[1]; break;
        case 2: R1_temp[0] = R1_SV_temp[2]; break;
        case 3: R1_temp[0] = R1_SV_temp[3]; break;
        }
        switch(IF.Sw_s_y) {
        case 0: R1_temp[1] = R1_SV_temp[0]; break;
        case 1: R1_temp[1] = R1_SV_temp[1]; break;
        case 2: R1_temp[1] = R1_SV_temp[2]; break;
        case 3: R1_temp[1] = R1_SV_temp[3]; break;
        }
        switch(IF.Sw_s_z) {
        case 0: R1_temp[2] = R1_SV_temp[0]; break;
        case 1: R1_temp[2] = R1_SV_temp[1]; break;
        case 2: R1_temp[2] = R1_SV_temp[2]; break;
        case 3: R1_temp[2] = R1_SV_temp[3];break;
        }
        switch(IF.Sw_s_w) {
        case 0: R1_temp[3] = R1_SV_temp[0]; break;
        case 1: R1_temp[3] = R1_SV_temp[1]; break;
        case 2: R1_temp[3] = R1_SV_temp[2]; break;
        case 3: R1_temp[3] = R1_SV_temp[3]; break;
        }
        //Rt do swizzle
        switch(IF.Sw_t_x) {
        case 0: R2_temp[0] = R2_SV_temp[0]; break;
        case 1: R2_temp[0] = R2_SV_temp[1]; break;
        case 2: R2_temp[0] = R2_SV_temp[2]; break;
        case 3: R2_temp[0] = R2_SV_temp[3]; break;
        }
        switch(IF.Sw_t_y) {
        case 0: R2_temp[1] = R2_SV_temp[0]; break;
        case 1: R2_temp[1] = R2_SV_temp[1]; break;
        case 2: R2_temp[1] = R2_SV_temp[2]; break;
        case 3: R2_temp[1] = R2_SV_temp[3]; break;
        }
        switch(IF.Sw_t_z) {
        case 0: R2_temp[2] = R2_SV_temp[0]; break;
        case 1: R2_temp[2] = R2_SV_temp[1]; break;
        case 2: R2_temp[2] = R2_SV_temp[2]; break;
        case 3: R2_temp[2] = R2_SV_temp[3]; break;
        }
        switch(IF.Sw_t_w) {
        case 0: R2_temp[3] = R2_SV_temp[0]; break;
        case 1: R2_temp[3] = R2_SV_temp[1]; break;
        case 2: R2_temp[3] = R2_SV_temp[2]; break;
        case 3: R2_temp[3] = R2_SV_temp[3]; break;
        }
        
	    for(i=0; i<=3; i++)
		{
		R3_temp[i] = R1_temp[i];
	    R4_temp[i] = R2_temp[i];
		}     
    }
    else//BNEV
    {
        //IF.Sw_s_x=0;IF.Sw_s_y=1;IF.Sw_s_z=2;IF.Sw_s_w=3;IF.Sw_t_x=0;IF.Sw_t_y=1;IF.Sw_t_z=2;IF.Sw_t_w=3;
	for(i=0; i<=IF.S_V; i++)
	{
	  R1_temp[i] = (IF.R1>=32) ? VAB[IF.R1-32][i] : GPR[IF.R1][i];
	  R2_temp[i] = (IF.R2>=32) ? VAB[IF.R2-32][i] : GPR[IF.R2][i];
	  R3_temp[i] = (IF.R3>=32) ? VAB[IF.R3-32][i] : GPR[IF.R3][i];
	  R4_temp[i] = GPR[IF.R4][i];//---R4 only support read from GPR
	}
    }
	
	//------------determine the source----------------------------------
	/*for(i=0; i<=IF.S_V; i++)
	{
		R1_temp[i] = (IF.R1>=32) ? VAB[IF.R1-32][i] : GPR[IF.R1][i];
		R2_temp[i] = (IF.R2>=32) ? VAB[IF.R2-32][i] : GPR[IF.R2][i];
		R3_temp[i] = (IF.R3>=32) ? VAB[IF.R3-32][i] : GPR[IF.R3][i];
		R4_temp[i] = GPR[IF.R4][i];//---R4 only support read from GPR
	}*/ 
	
	//-----------compare component x---------------------------------------
	if(IF.SCC_x==1)
	{
		if(IF.FXL==1)
		{
			R1_temp_FL[0] = *((float*)((void*)&R1_temp[0]));
			R2_temp_FL[0] = *((float*)((void*)&R2_temp[0]));
			R3_temp_FL[0] = *((float*)((void*)&R3_temp[0]));
			R4_temp_FL[0] = *((float*)((void*)&R4_temp[0]));
			if((R1_temp_FL[0]!=R2_temp_FL[0]) && (R3_temp_FL[0]!=R4_temp_FL[0]))
				branch_taken[0] = 1;
		}
		else
		{
			R1_temp_FX[0] = *((int*)((void*)&R1_temp[0]));
			R2_temp_FX[0] = *((int*)((void*)&R2_temp[0]));
			R3_temp_FX[0] = *((int*)((void*)&R3_temp[0]));
			R4_temp_FX[0] = *((int*)((void*)&R4_temp[0]));
			if((R1_temp_FX[0]!=R2_temp_FX[0]) && (R3_temp_FX[0]!=R4_temp_FX[0]))
				branch_taken[0] = 1;
		}
	} 
	
	//-----------compare component y---------------------------------------
	if(IF.SCC_y==1)
	{
		if(IF.FXL==1)
		{
			R1_temp_FL[1] = *((float*)((void*)&R1_temp[1]));
			R2_temp_FL[1] = *((float*)((void*)&R2_temp[1]));
			R3_temp_FL[1] = *((float*)((void*)&R3_temp[1]));
			R4_temp_FL[1] = *((float*)((void*)&R4_temp[1]));
			if((R1_temp_FL[1]!=R2_temp_FL[1]) && (R3_temp_FL[1]!=R4_temp_FL[1]))
				branch_taken[1] = 1;
		}
		else
		{
			R1_temp_FX[1] = *((int*)((void*)&R1_temp[1]));
			R2_temp_FX[1] = *((int*)((void*)&R2_temp[1]));
			R3_temp_FX[1] = *((int*)((void*)&R3_temp[1]));
			R4_temp_FX[1] = *((int*)((void*)&R4_temp[1]));
			if((R1_temp_FX[1]!=R2_temp_FX[1]) && (R3_temp_FX[1]!=R4_temp_FX[1]))
				branch_taken[1] = 1;
		}
	} 
	
	//-----------compare component z---------------------------------------
	if(IF.SCC_z==1)
	{
		if(IF.FXL==1)
		{
			R1_temp_FL[2] = *((float*)((void*)&R1_temp[2]));
			R2_temp_FL[2] = *((float*)((void*)&R2_temp[2]));
			R3_temp_FL[2] = *((float*)((void*)&R3_temp[2]));
			R4_temp_FL[2] = *((float*)((void*)&R4_temp[2]));
			if((R1_temp_FL[2]!=R2_temp_FL[2]) && (R3_temp_FL[2]!=R4_temp_FL[2]))
				branch_taken[2] = 1;
		}
		else
		{
			R1_temp_FX[2] = *((int*)((void*)&R1_temp[2]));
			R2_temp_FX[2] = *((int*)((void*)&R2_temp[2]));
			R3_temp_FX[2] = *((int*)((void*)&R3_temp[2]));
			R4_temp_FX[2] = *((int*)((void*)&R4_temp[2]));
			if((R1_temp_FX[2]!=R2_temp_FX[2]) && (R3_temp_FX[2]!=R4_temp_FX[2]))
				branch_taken[2] = 1;
		}
	}
	
	//-----------compare component w---------------------------------------
	if(IF.SCC_w==1)
	{
		if(IF.FXL==1)
		{
			R1_temp_FL[3] = *((float*)((void*)&R1_temp[3]));
			R2_temp_FL[3] = *((float*)((void*)&R2_temp[3]));
			R3_temp_FL[3] = *((float*)((void*)&R3_temp[3]));
			R4_temp_FL[3] = *((float*)((void*)&R4_temp[3]));
			if((R1_temp_FL[3]!=R2_temp_FL[3]) && (R3_temp_FL[3]!=R4_temp_FL[3]))
				branch_taken[3] = 1;
		}
		else
		{
			R1_temp_FX[3] = *((int*)((void*)&R1_temp[3]));
			R2_temp_FX[3] = *((int*)((void*)&R2_temp[3]));
			R3_temp_FX[3] = *((int*)((void*)&R3_temp[3]));
			R4_temp_FX[3] = *((int*)((void*)&R4_temp[3]));
			if((R1_temp_FX[3]!=R2_temp_FX[3]) && (R3_temp_FX[3]!=R4_temp_FX[3]))
				branch_taken[3] = 1;
		}
	}  
	
	//-----------determein branch or not---------------------------------------
	if(branch_taken[0]==IF.SCC_x && branch_taken[1]==IF.SCC_y && branch_taken[2]==IF.SCC_z && branch_taken[3]==IF.SCC_w)
		PC = PC+(short signed)IF.Addr;
	
	//----------print for debug------------------------------------------------------------------------
	if(IF.FXL==1)
	{
		fprintf(fp_Result,"R1_GPR[%d]:%f_%f_%f_%f\n", IF.R1, *(float*)&R1_temp[0], *(float*)&R1_temp[1], *(float*)&R1_temp[2], *(float*)&R1_temp[3]);
		fprintf(fp_Result,"R2_GPR[%d]:%f_%f_%f_%f\n", IF.R2, *(float*)&R2_temp[0], *(float*)&R2_temp[1], *(float*)&R2_temp[2], *(float*)&R2_temp[3]);
		fprintf(fp_Result,"R3_GPR[%d]:%f_%f_%f_%f\n", IF.R3, *(float*)&R3_temp[0], *(float*)&R3_temp[1], *(float*)&R3_temp[2], *(float*)&R3_temp[3]);
		fprintf(fp_Result,"R4_GPR[%d]:%f_%f_%f_%f\n", IF.R4, *(float*)&R4_temp[0], *(float*)&R4_temp[1], *(float*)&R4_temp[2], *(float*)&R4_temp[3]);
	}
	else
	{
		fprintf(fp_Result,"R1_GPR[%d]:%d_%d_%d_%d\n", IF.R1, R1_temp[0], R1_temp[1], R1_temp[2], R1_temp[3]);
		fprintf(fp_Result,"R2_GPR[%d]:%d_%d_%d_%d\n", IF.R2, R2_temp[0], R2_temp[1], R2_temp[2], R2_temp[3]);
		fprintf(fp_Result,"R3_GPR[%d]:%d_%d_%d_%d\n", IF.R3, R3_temp[0], R3_temp[1], R3_temp[2], R3_temp[3]);
		fprintf(fp_Result,"R4_GPR[%d]:%d_%d_%d_%d\n", IF.R4, R4_temp[0], R4_temp[1], R4_temp[2], R4_temp[3]);
	}
	
	fprintf(fp_Result,"-----------------------------------------------------------------\n");
	//------------ out -----------------
	if(branch_taken[0]==IF.SCC_x && branch_taken[1]==IF.SCC_y && branch_taken[2]==IF.SCC_z && branch_taken[3]==IF.SCC_w)
	{
		fprintf(fp_Result,"branch:%d%d%d%d = SCC:%d%d%d%d\n", branch_taken[0], branch_taken[1], branch_taken[2], branch_taken[3], IF.SCC_x, IF.SCC_y, IF.SCC_z, IF.SCC_w);
		fprintf(fp_Result,"New PC = %d\n", PC);
	}     
	else
	{
		fprintf(fp_Result,"branch:%d%d%d%d != SCC:%d%d%d%d\n", branch_taken[0], branch_taken[1], branch_taken[2], branch_taken[3], IF.SCC_x, IF.SCC_y, IF.SCC_z, IF.SCC_w);
		fprintf(fp_Result,"branch not taken.\n");
	}
}

void SC_VS::VS_26_BLT()
{
	fprintf(fp_Result,"Op:26 = BLT\n");
	if(IF.R1>=32)		fprintf(fp_Result,"R1:VOB_entry[%d]\n", IF.R1-32);
	else				fprintf(fp_Result,"R1:GPR[%d]\n", IF.R1);
	if(IF.R2>=32)		fprintf(fp_Result,"R2:VAB_entry[%d]\n", IF.R2-32);
	else				fprintf(fp_Result,"R2:GPR[%d]\n", IF.R2);
	if(IF.R3>=32)		fprintf(fp_Result,"R3:VAB_entry[%d]\n", IF.R3-32);
	else				fprintf(fp_Result,"R3:GPR[%d]\n", IF.R3);	
	fprintf(fp_Result,"R4:GPR[%d]\n", IF.R4);
	fprintf(fp_Result,"Addr:[%d]\n", ((short signed)IF.Addr));
	fprintf(fp_Result,"SCC:%d%d%d%d\n", IF.SCC_x, IF.SCC_y, IF.SCC_z, IF.SCC_w);
	fprintf(fp_Result,"FXL:%d\n", IF.FXL);
	fprintf(fp_Result,"S/V:%d\n", IF.S_V);

    //-----------Initial variable------------------------------------------
    for(i=0; i<=3; i++)
    {
		R1_SV_temp[i] = 0;
		R2_SV_temp[i] = 0;
		R3_SV_temp[i] = 0;
		R4_SV_temp[i] = 0;
        R1_temp[i] = 0;
        R2_temp[i] = 0;
        R3_temp[i] = 0;
        R4_temp[i] = 0;
        R1_temp_FL[i] = 0;
        R2_temp_FL[i] = 0;
        R3_temp_FL[i] = 0;
        R4_temp_FL[i] = 0;
        R1_temp_FX[i] = 0;
        R2_temp_FX[i] = 0;
        R3_temp_FX[i] = 0;
        R4_temp_FX[i] = 0;
        branch_taken[i] = 0;
    }


    if(IF.Br_m==1)//BEQS
    {
        IF.SCC_x=1;IF.SCC_y=1;IF.SCC_z=1;IF.SCC_w=1;
		//S_V modified by TSAO
		//Load Rs
		R1_SV_temp[0] = (IF.R1>=32) ? VAB[IF.R1-32][0] : GPR[IF.R1][0];
		R1_SV_temp[1] = (IF.R1>=32) ? VAB[IF.R1-32][1] : GPR[IF.R1][1];
		R1_SV_temp[2] = (IF.R1>=32) ? VAB[IF.R1-32][2] : GPR[IF.R1][2];
		R1_SV_temp[3] = (IF.R1>=32) ? VAB[IF.R1-32][3] : GPR[IF.R1][3];
		//Load Rt
		R2_SV_temp[0] = (IF.R2>=32) ? VAB[IF.R2-32][0] : GPR[IF.R2][0];
		R2_SV_temp[1] = (IF.R2>=32) ? VAB[IF.R2-32][1] : GPR[IF.R2][1];
		R2_SV_temp[2] = (IF.R2>=32) ? VAB[IF.R2-32][2] : GPR[IF.R2][2];
		R2_SV_temp[3] = (IF.R2>=32) ? VAB[IF.R2-32][3] : GPR[IF.R2][3];
		//S_V
        for(i=0; i<=3; i++)
        {
            if(i>IF.S_V)
			{
				R1_SV_temp[i] = 0;
				R2_SV_temp[i] = 0;
				R3_SV_temp[i] = 0;
				R4_SV_temp[i] = 0;
            }
		}
        //Rs do swizzle
        switch(IF.Sw_s_x) {
        case 0: R1_temp[0] = R1_SV_temp[0]; break;
        case 1: R1_temp[0] = R1_SV_temp[1]; break;
        case 2: R1_temp[0] = R1_SV_temp[2]; break;
        case 3: R1_temp[0] = R1_SV_temp[3]; break;
        }
        switch(IF.Sw_s_y) {
        case 0: R1_temp[1] = R1_SV_temp[0]; break;
        case 1: R1_temp[1] = R1_SV_temp[1]; break;
        case 2: R1_temp[1] = R1_SV_temp[2]; break;
        case 3: R1_temp[1] = R1_SV_temp[3]; break;
        }
        switch(IF.Sw_s_z) {
        case 0: R1_temp[2] = R1_SV_temp[0]; break;
        case 1: R1_temp[2] = R1_SV_temp[1]; break;
        case 2: R1_temp[2] = R1_SV_temp[2]; break;
        case 3: R1_temp[2] = R1_SV_temp[3];break;
        }
        switch(IF.Sw_s_w) {
        case 0: R1_temp[3] = R1_SV_temp[0]; break;
        case 1: R1_temp[3] = R1_SV_temp[1]; break;
        case 2: R1_temp[3] = R1_SV_temp[2]; break;
        case 3: R1_temp[3] = R1_SV_temp[3]; break;
        }
        //Rt do swizzle
        switch(IF.Sw_t_x) {
        case 0: R2_temp[0] = R2_SV_temp[0]; break;
        case 1: R2_temp[0] = R2_SV_temp[1]; break;
        case 2: R2_temp[0] = R2_SV_temp[2]; break;
        case 3: R2_temp[0] = R2_SV_temp[3]; break;
        }
        switch(IF.Sw_t_y) {
        case 0: R2_temp[1] = R2_SV_temp[0]; break;
        case 1: R2_temp[1] = R2_SV_temp[1]; break;
        case 2: R2_temp[1] = R2_SV_temp[2]; break;
        case 3: R2_temp[1] = R2_SV_temp[3]; break;
        }
        switch(IF.Sw_t_z) {
        case 0: R2_temp[2] = R2_SV_temp[0]; break;
        case 1: R2_temp[2] = R2_SV_temp[1]; break;
        case 2: R2_temp[2] = R2_SV_temp[2]; break;
        case 3: R2_temp[2] = R2_SV_temp[3]; break;
        }
        switch(IF.Sw_t_w) {
        case 0: R2_temp[3] = R2_SV_temp[0]; break;
        case 1: R2_temp[3] = R2_SV_temp[1]; break;
        case 2: R2_temp[3] = R2_SV_temp[2]; break;
        case 3: R2_temp[3] = R2_SV_temp[3]; break;
        }
        
	    for(i=0; i<=3; i++)
		{
		R3_temp[i] = R1_temp[i];
	    R4_temp[i] = R2_temp[i];
		}
	}
    else//BLTV
    {
        //IF.Sw_s_x=0;IF.Sw_s_y=1;IF.Sw_s_z=2;IF.Sw_s_w=3;IF.Sw_t_x=0;IF.Sw_t_y=1;IF.Sw_t_z=2;IF.Sw_t_w=3;
	for(i=0; i<=IF.S_V; i++)
	{
	  R1_temp[i] = (IF.R1>=32) ? VAB[IF.R1-32][i] : GPR[IF.R1][i];
	  R2_temp[i] = (IF.R2>=32) ? VAB[IF.R2-32][i] : GPR[IF.R2][i];
	  R3_temp[i] = (IF.R3>=32) ? VAB[IF.R3-32][i] : GPR[IF.R3][i];
	  R4_temp[i] = GPR[IF.R4][i];//---R4 only support read from GPR
	}
    }
	
	//------------determine the source----------------------------------
	/*for(i=0; i<=IF.S_V; i++)
	{
		R1_temp[i] = (IF.R1>=32) ? VAB[IF.R1-32][i] : GPR[IF.R1][i];
		R2_temp[i] = (IF.R2>=32) ? VAB[IF.R2-32][i] : GPR[IF.R2][i];
		R3_temp[i] = (IF.R3>=32) ? VAB[IF.R3-32][i] : GPR[IF.R3][i];
		R4_temp[i] = GPR[IF.R4][i];//---R4 only support read from GPR
	} */
	
	//-----------compare component x---------------------------------------
	if(IF.SCC_x==1)
	{
		if(IF.FXL==1)
		{
			R1_temp_FL[0] = *((float*)((void*)&R1_temp[0]));
			R2_temp_FL[0] = *((float*)((void*)&R2_temp[0]));
			R3_temp_FL[0] = *((float*)((void*)&R3_temp[0]));
			R4_temp_FL[0] = *((float*)((void*)&R4_temp[0]));
			if((R1_temp_FL[0]<R2_temp_FL[0]) && (R3_temp_FL[0]<R4_temp_FL[0]))
				branch_taken[0] = 1;
		}
		else
		{
			R1_temp_FX[0] = *((int*)((void*)&R1_temp[0]));
			R2_temp_FX[0] = *((int*)((void*)&R2_temp[0]));
			R3_temp_FX[0] = *((int*)((void*)&R3_temp[0]));
			R4_temp_FX[0] = *((int*)((void*)&R4_temp[0]));
			if((R1_temp_FX[0]<R2_temp_FX[0]) && (R3_temp_FX[0]<R4_temp_FX[0]))
				branch_taken[0] = 1;
		}
	} 
	
	//-----------compare component y---------------------------------------
	if(IF.SCC_y==1)
	{
		if(IF.FXL==1)
		{
			R1_temp_FL[1] = *((float*)((void*)&R1_temp[1]));
			R2_temp_FL[1] = *((float*)((void*)&R2_temp[1]));
			R3_temp_FL[1] = *((float*)((void*)&R3_temp[1]));
			R4_temp_FL[1] = *((float*)((void*)&R4_temp[1]));
			if((R1_temp_FL[1]<R2_temp_FL[1]) && (R3_temp_FL[1]<R4_temp_FL[1]))
				branch_taken[1] = 1;
		}
		else
		{
			R1_temp_FX[1] = *((int*)((void*)&R1_temp[1]));
			R2_temp_FX[1] = *((int*)((void*)&R2_temp[1]));
			R3_temp_FX[1] = *((int*)((void*)&R3_temp[1]));
			R4_temp_FX[1] = *((int*)((void*)&R4_temp[1]));
			if((R1_temp_FX[1]<R2_temp_FX[1]) && (R3_temp_FX[1]<R4_temp_FX[1]))
				branch_taken[1] = 1;
		}
	} 
	
	//-----------compare component z---------------------------------------
	if(IF.SCC_z==1)
	{
		if(IF.FXL==1)
		{
			R1_temp_FL[2] = *((float*)((void*)&R1_temp[2]));
			R2_temp_FL[2] = *((float*)((void*)&R2_temp[2]));
			R3_temp_FL[2] = *((float*)((void*)&R3_temp[2]));
			R4_temp_FL[2] = *((float*)((void*)&R4_temp[2]));
			if((R1_temp_FL[2]<R2_temp_FL[2]) && (R3_temp_FL[2]<R4_temp_FL[2]))
				branch_taken[2] = 1;
		}
		else
		{
			R1_temp_FX[2] = *((int*)((void*)&R1_temp[2]));
			R2_temp_FX[2] = *((int*)((void*)&R2_temp[2]));
			R3_temp_FX[2] = *((int*)((void*)&R3_temp[2]));
			R4_temp_FX[2] = *((int*)((void*)&R4_temp[2]));
			if((R1_temp_FX[2]<R2_temp_FX[2]) && (R3_temp_FX[2]<R4_temp_FX[2]))
				branch_taken[2] = 1;
		}
	}
	
	//-----------compare component w---------------------------------------
	if(IF.SCC_w==1)
	{
		if(IF.FXL==1)
		{
			R1_temp_FL[3] = *((float*)((void*)&R1_temp[3]));
			R2_temp_FL[3] = *((float*)((void*)&R2_temp[3]));
			R3_temp_FL[3] = *((float*)((void*)&R3_temp[3]));
			R4_temp_FL[3] = *((float*)((void*)&R4_temp[3]));
			if((R1_temp_FL[3]<R2_temp_FL[3]) && (R3_temp_FL[3]<R4_temp_FL[3]))
				branch_taken[3] = 1;
		}
		else
		{
			R1_temp_FX[3] = *((int*)((void*)&R1_temp[3]));
			R2_temp_FX[3] = *((int*)((void*)&R2_temp[3]));
			R3_temp_FX[3] = *((int*)((void*)&R3_temp[3]));
			R4_temp_FX[3] = *((int*)((void*)&R4_temp[3]));
			if((R1_temp_FX[3]<R2_temp_FX[3]) && (R3_temp_FX[3]<R4_temp_FX[3]))
				branch_taken[3] = 1;
		}
	}  
	
	//-----------determein branch or not---------------------------------------
	if(branch_taken[0]==IF.SCC_x && branch_taken[1]==IF.SCC_y && branch_taken[2]==IF.SCC_z && branch_taken[3]==IF.SCC_w)
		PC = PC+(short signed)IF.Addr;
	
	//----------print for debug------------------------------------------------------------------------
	if(IF.FXL==1)
	{
		fprintf(fp_Result,"R1_GPR[%d]:%f_%f_%f_%f\n", IF.R1, *(float*)&R1_temp[0], *(float*)&R1_temp[1], *(float*)&R1_temp[2], *(float*)&R1_temp[3]);
		fprintf(fp_Result,"R2_GPR[%d]:%f_%f_%f_%f\n", IF.R2, *(float*)&R2_temp[0], *(float*)&R2_temp[1], *(float*)&R2_temp[2], *(float*)&R2_temp[3]);
		fprintf(fp_Result,"R3_GPR[%d]:%f_%f_%f_%f\n", IF.R3, *(float*)&R3_temp[0], *(float*)&R3_temp[1], *(float*)&R3_temp[2], *(float*)&R3_temp[3]);
		fprintf(fp_Result,"R4_GPR[%d]:%f_%f_%f_%f\n", IF.R4, *(float*)&R4_temp[0], *(float*)&R4_temp[1], *(float*)&R4_temp[2], *(float*)&R4_temp[3]);
	}
	else
	{
		fprintf(fp_Result,"R1_GPR[%d]:%d_%d_%d_%d\n", IF.R1, R1_temp[0], R1_temp[1], R1_temp[2], R1_temp[3]);
		fprintf(fp_Result,"R2_GPR[%d]:%d_%d_%d_%d\n", IF.R2, R2_temp[0], R2_temp[1], R2_temp[2], R2_temp[3]);
		fprintf(fp_Result,"R3_GPR[%d]:%d_%d_%d_%d\n", IF.R3, R3_temp[0], R3_temp[1], R3_temp[2], R3_temp[3]);
		fprintf(fp_Result,"R4_GPR[%d]:%d_%d_%d_%d\n", IF.R4, R4_temp[0], R4_temp[1], R4_temp[2], R4_temp[3]);
	}
	
	fprintf(fp_Result,"-----------------------------------------------------------------\n");
	//------------ out -----------------
	if(branch_taken[0]==IF.SCC_x && branch_taken[1]==IF.SCC_y && branch_taken[2]==IF.SCC_z && branch_taken[3]==IF.SCC_w)
	{
		fprintf(fp_Result,"branch:%d%d%d%d = SCC:%d%d%d%d\n", branch_taken[0], branch_taken[1], branch_taken[2], branch_taken[3], IF.SCC_x, IF.SCC_y, IF.SCC_z, IF.SCC_w);
		fprintf(fp_Result,"New PC = %d\n", PC);
	}     
	else
	{
		fprintf(fp_Result,"branch:%d%d%d%d != SCC:%d%d%d%d\n", branch_taken[0], branch_taken[1], branch_taken[2], branch_taken[3], IF.SCC_x, IF.SCC_y, IF.SCC_z, IF.SCC_w);
		fprintf(fp_Result,"branch not taken.\n");
	}
}

void SC_VS::VS_27_BGT()
{
	fprintf(fp_Result,"Op:27 = BGT\n");
	if(IF.R1>=32)		fprintf(fp_Result,"R1:VOB_entry[%d]\n", IF.R1-32);
	else				fprintf(fp_Result,"R1:GPR[%d]\n", IF.R1);
	if(IF.R2>=32)		fprintf(fp_Result,"R2:VAB_entry[%d]\n", IF.R2-32);
	else				fprintf(fp_Result,"R2:GPR[%d]\n", IF.R2);
	if(IF.R3>=32)		fprintf(fp_Result,"R3:VAB_entry[%d]\n", IF.R3-32);
	else				fprintf(fp_Result,"R3:GPR[%d]\n", IF.R3);	
	fprintf(fp_Result,"R4:GPR[%d]\n", IF.R4);
	fprintf(fp_Result,"Addr:[%d]\n", ((short signed)IF.Addr));
	fprintf(fp_Result,"SCC:%d%d%d%d\n", IF.SCC_x, IF.SCC_y, IF.SCC_z, IF.SCC_w);
	fprintf(fp_Result,"FXL:%d\n", IF.FXL);
	fprintf(fp_Result,"S/V:%d\n", IF.S_V);

    //-----------Initial variable------------------------------------------
    for(i=0; i<=3; i++)
    {
		R1_SV_temp[i] = 0;
		R2_SV_temp[i] = 0;
		R3_SV_temp[i] = 0;
		R4_SV_temp[i] = 0;
        R1_temp[i] = 0;
        R2_temp[i] = 0;
        R3_temp[i] = 0;
        R4_temp[i] = 0;
        R1_temp_FL[i] = 0;
        R2_temp_FL[i] = 0;
        R3_temp_FL[i] = 0;
        R4_temp_FL[i] = 0;
        R1_temp_FX[i] = 0;
        R2_temp_FX[i] = 0;
        R3_temp_FX[i] = 0;
        R4_temp_FX[i] = 0;
        branch_taken[i] = 0;
    }


    if(IF.Br_m==1)//BEQS
    {
        IF.SCC_x=1;IF.SCC_y=1;IF.SCC_z=1;IF.SCC_w=1;
		//S_V modified by TSAO
		//Load Rs
		R1_SV_temp[0] = (IF.R1>=32) ? VAB[IF.R1-32][0] : GPR[IF.R1][0];
		R1_SV_temp[1] = (IF.R1>=32) ? VAB[IF.R1-32][1] : GPR[IF.R1][1];
		R1_SV_temp[2] = (IF.R1>=32) ? VAB[IF.R1-32][2] : GPR[IF.R1][2];
		R1_SV_temp[3] = (IF.R1>=32) ? VAB[IF.R1-32][3] : GPR[IF.R1][3];
		//Load Rt
		R2_SV_temp[0] = (IF.R2>=32) ? VAB[IF.R2-32][0] : GPR[IF.R2][0];
		R2_SV_temp[1] = (IF.R2>=32) ? VAB[IF.R2-32][1] : GPR[IF.R2][1];
		R2_SV_temp[2] = (IF.R2>=32) ? VAB[IF.R2-32][2] : GPR[IF.R2][2];
		R2_SV_temp[3] = (IF.R2>=32) ? VAB[IF.R2-32][3] : GPR[IF.R2][3];
		//S_V
        for(i=0; i<=3; i++)
        {
            if(i>IF.S_V)
			{
				R1_SV_temp[i] = 0;
				R2_SV_temp[i] = 0;
				R3_SV_temp[i] = 0;
				R4_SV_temp[i] = 0;
            }
		}
        //Rs do swizzle
        switch(IF.Sw_s_x) {
        case 0: R1_temp[0] = R1_SV_temp[0]; break;
        case 1: R1_temp[0] = R1_SV_temp[1]; break;
        case 2: R1_temp[0] = R1_SV_temp[2]; break;
        case 3: R1_temp[0] = R1_SV_temp[3]; break;
        }
        switch(IF.Sw_s_y) {
        case 0: R1_temp[1] = R1_SV_temp[0]; break;
        case 1: R1_temp[1] = R1_SV_temp[1]; break;
        case 2: R1_temp[1] = R1_SV_temp[2]; break;
        case 3: R1_temp[1] = R1_SV_temp[3]; break;
        }
        switch(IF.Sw_s_z) {
        case 0: R1_temp[2] = R1_SV_temp[0]; break;
        case 1: R1_temp[2] = R1_SV_temp[1]; break;
        case 2: R1_temp[2] = R1_SV_temp[2]; break;
        case 3: R1_temp[2] = R1_SV_temp[3];break;
        }
        switch(IF.Sw_s_w) {
        case 0: R1_temp[3] = R1_SV_temp[0]; break;
        case 1: R1_temp[3] = R1_SV_temp[1]; break;
        case 2: R1_temp[3] = R1_SV_temp[2]; break;
        case 3: R1_temp[3] = R1_SV_temp[3]; break;
        }
        //Rt do swizzle
        switch(IF.Sw_t_x) {
        case 0: R2_temp[0] = R2_SV_temp[0]; break;
        case 1: R2_temp[0] = R2_SV_temp[1]; break;
        case 2: R2_temp[0] = R2_SV_temp[2]; break;
        case 3: R2_temp[0] = R2_SV_temp[3]; break;
        }
        switch(IF.Sw_t_y) {
        case 0: R2_temp[1] = R2_SV_temp[0]; break;
        case 1: R2_temp[1] = R2_SV_temp[1]; break;
        case 2: R2_temp[1] = R2_SV_temp[2]; break;
        case 3: R2_temp[1] = R2_SV_temp[3]; break;
        }
        switch(IF.Sw_t_z) {
        case 0: R2_temp[2] = R2_SV_temp[0]; break;
        case 1: R2_temp[2] = R2_SV_temp[1]; break;
        case 2: R2_temp[2] = R2_SV_temp[2]; break;
        case 3: R2_temp[2] = R2_SV_temp[3]; break;
        }
        switch(IF.Sw_t_w) {
        case 0: R2_temp[3] = R2_SV_temp[0]; break;
        case 1: R2_temp[3] = R2_SV_temp[1]; break;
        case 2: R2_temp[3] = R2_SV_temp[2]; break;
        case 3: R2_temp[3] = R2_SV_temp[3]; break;
        }
        
	    for(i=0; i<=3; i++)
		{
		R3_temp[i] = R1_temp[i];
	    R4_temp[i] = R2_temp[i];
		}
/*	
        for(i=0; i<=3; i++)
        {
            if(i>IF.S_V)
            {
                R1_temp[i] = 0;R2_temp[i] = 0;R3_temp[i] = 0;R4_temp[i] = 0;
            }
        }
*/        
    }
    else//BGTV
    {
        //IF.Sw_s_x=0;IF.Sw_s_y=1;IF.Sw_s_z=2;IF.Sw_s_w=3;IF.Sw_t_x=0;IF.Sw_t_y=1;IF.Sw_t_z=2;IF.Sw_t_w=3;
	for(i=0; i<=IF.S_V; i++)
	{
	  R1_temp[i] = (IF.R1>=32) ? VAB[IF.R1-32][i] : GPR[IF.R1][i];
	  R2_temp[i] = (IF.R2>=32) ? VAB[IF.R2-32][i] : GPR[IF.R2][i];
	  R3_temp[i] = (IF.R3>=32) ? VAB[IF.R3-32][i] : GPR[IF.R3][i];
	  R4_temp[i] = GPR[IF.R4][i];//---R4 only support read from GPR
	}
    }
	
	//------------determine the source----------------------------------
	/*for(i=0; i<=IF.S_V; i++)
	{
		R1_temp[i] = (IF.R1>=32) ? VAB[IF.R1-32][i] : GPR[IF.R1][i];
		R2_temp[i] = (IF.R2>=32) ? VAB[IF.R2-32][i] : GPR[IF.R2][i];
		R3_temp[i] = (IF.R3>=32) ? VAB[IF.R3-32][i] : GPR[IF.R3][i];
		R4_temp[i] = GPR[IF.R4][i];//---R4 only support read from GPR
	} */
	
	//-----------compare component x---------------------------------------
	if(IF.SCC_x==1)
	{
		if(IF.FXL==1)
		{
			R1_temp_FL[0] = *((float*)((void*)&R1_temp[0]));
			R2_temp_FL[0] = *((float*)((void*)&R2_temp[0]));
			R3_temp_FL[0] = *((float*)((void*)&R3_temp[0]));
			R4_temp_FL[0] = *((float*)((void*)&R4_temp[0]));
			if((R1_temp_FL[0]>R2_temp_FL[0]) && (R3_temp_FL[0]>R4_temp_FL[0]))
				branch_taken[0] = 1;
		}
		else
		{
			R1_temp_FX[0] = *((int*)((void*)&R1_temp[0]));
			R2_temp_FX[0] = *((int*)((void*)&R2_temp[0]));
			R3_temp_FX[0] = *((int*)((void*)&R3_temp[0]));
			R4_temp_FX[0] = *((int*)((void*)&R4_temp[0]));
			if((R1_temp_FX[0]>R2_temp_FX[0]) && (R3_temp_FX[0]>R4_temp_FX[0]))
				branch_taken[0] = 1;
		}
	} 
	
	//-----------compare component y---------------------------------------
	if(IF.SCC_y==1)
	{/*
	  fprintf(fp_Result,"%f _ %f _ %f _ %f\n", R1_temp[1],R2_temp[1],R3_temp[1],R4_temp[1]);//Kaymon
	  fprintf(fp_Result,"%d _ %d _ %d _ %d\n", R1_temp[1],R2_temp[1],R3_temp[1],R4_temp[1]);//Kaymon
	  fprintf(fp_Result,"%x _ %x _ %x _ %x\n", R1_temp[1],R2_temp[1],R3_temp[1],R4_temp[1]);//Kaymon
	  fprintf(fp_Result,"before %u _ %u _ %u _ %u\n", R1_temp[1],R2_temp[1],R3_temp[1],R4_temp[1]);//Kaymon
	  fprintf(fp_Result,"size %d %d \n", sizeof(R1_temp_FL[1]),sizeof(R2_temp_FL[1]));//Kaymon
	  fprintf(fp_Result,"size %d %d %d %d\n", R1_temp_FL[1],R2_temp_FL[1],R3_temp_FL[1],R4_temp_FL[1]);//Kaymon
	  fprintf(fp_Result,"size2 %f %f %f %f\n", R1_temp_FL[1],R2_temp_FL[1],R3_temp_FL[1],R4_temp_FL[1]);//Kaymon*/
		if(IF.FXL==1)
		{
			R1_temp_FL[1] = *((float*)((void*)&R1_temp[1]));
			R2_temp_FL[1] = *((float*)((void*)&R2_temp[1]));
			R3_temp_FL[1] = *((float*)((void*)&R3_temp[1]));
			R4_temp_FL[1] = *((float*)((void*)&R4_temp[1]));
			if((R1_temp_FL[1]>R2_temp_FL[1]) && (R3_temp_FL[1]>R4_temp_FL[1]) && fabs(R3_temp_FL[1]-R4_temp_FL[1])>0.000001 ) //Kaymon add third item
			  {
				branch_taken[1] = 1;
			  }
		}
		else
		{
			R1_temp_FX[1] = *((int*)((void*)&R1_temp[1]));
			R2_temp_FX[1] = *((int*)((void*)&R2_temp[1]));
			R3_temp_FX[1] = *((int*)((void*)&R3_temp[1]));
			R4_temp_FX[1] = *((int*)((void*)&R4_temp[1]));
			if((R1_temp_FX[1]>R2_temp_FX[1]) && (R3_temp_FX[1]>R4_temp_FX[1]))
				branch_taken[1] = 1;

		}
	} 
	
	//-----------compare component z---------------------------------------
	if(IF.SCC_z==1)
	{
		if(IF.FXL==1)
		{
			R1_temp_FL[2] = *((float*)((void*)&R1_temp[2]));
			R2_temp_FL[2] = *((float*)((void*)&R2_temp[2]));
			R3_temp_FL[2] = *((float*)((void*)&R3_temp[2]));
			R4_temp_FL[2] = *((float*)((void*)&R4_temp[2]));
			if((R1_temp_FL[2]>R2_temp_FL[2]) && (R3_temp_FL[2]>R4_temp_FL[2]))
				branch_taken[2] = 1;
		}
		else
		{
			R1_temp_FX[2] = *((int*)((void*)&R1_temp[2]));
			R2_temp_FX[2] = *((int*)((void*)&R2_temp[2]));
			R3_temp_FX[2] = *((int*)((void*)&R3_temp[2]));
			R4_temp_FX[2] = *((int*)((void*)&R4_temp[2]));
			if((R1_temp_FX[2]>R2_temp_FX[2]) && (R3_temp_FX[2]>R4_temp_FX[2]))
				branch_taken[2] = 1;
		}
	}
	
	//-----------compare component w---------------------------------------
	if(IF.SCC_w==1)
	{
		if(IF.FXL==1)
		{
			R1_temp_FL[3] = *((float*)((void*)&R1_temp[3]));
			R2_temp_FL[3] = *((float*)((void*)&R2_temp[3]));
			R3_temp_FL[3] = *((float*)((void*)&R3_temp[3]));
			R4_temp_FL[3] = *((float*)((void*)&R4_temp[3]));
			if((R1_temp_FL[3]>R2_temp_FL[3]) && (R3_temp_FL[3]>R4_temp_FL[3]))
				branch_taken[3] = 1;
		}
		else
		{
			R1_temp_FX[3] = *((int*)((void*)&R1_temp[3]));
			R2_temp_FX[3] = *((int*)((void*)&R2_temp[3]));
			R3_temp_FX[3] = *((int*)((void*)&R3_temp[3]));
			R4_temp_FX[3] = *((int*)((void*)&R4_temp[3]));
			if((R1_temp_FX[3]>R2_temp_FX[3]) && (R3_temp_FX[3]>R4_temp_FX[3]))
				branch_taken[3] = 1;
		}
	}  
	
	//-----------determein branch or not---------------------------------------
	if(branch_taken[0]==IF.SCC_x && branch_taken[1]==IF.SCC_y && branch_taken[2]==IF.SCC_z && branch_taken[3]==IF.SCC_w)
		PC = PC+(short signed)IF.Addr;
	
	//----------print for debug------------------------------------------------------------------------
	if(IF.FXL==1)
	{
		fprintf(fp_Result,"R1_GPR[%d]:%f_%f_%f_%f\n", IF.R1, *(float*)&R1_temp[0], *(float*)&R1_temp[1], *(float*)&R1_temp[2], *(float*)&R1_temp[3]);
		fprintf(fp_Result,"R2_GPR[%d]:%f_%f_%f_%f\n", IF.R2, *(float*)&R2_temp[0], *(float*)&R2_temp[1], *(float*)&R2_temp[2], *(float*)&R2_temp[3]);
		fprintf(fp_Result,"R3_GPR[%d]:%f_%f_%f_%f\n", IF.R3, *(float*)&R3_temp[0], *(float*)&R3_temp[1], *(float*)&R3_temp[2], *(float*)&R3_temp[3]);
		fprintf(fp_Result,"R4_GPR[%d]:%f_%f_%f_%f\n", IF.R4, *(float*)&R4_temp[0], *(float*)&R4_temp[1], *(float*)&R4_temp[2], *(float*)&R4_temp[3]);
	}
	else
	{
		fprintf(fp_Result,"R1_GPR[%d]:%d_%d_%d_%d\n", IF.R1, R1_temp[0], R1_temp[1], R1_temp[2], R1_temp[3]);
		fprintf(fp_Result,"R2_GPR[%d]:%d_%d_%d_%d\n", IF.R2, R2_temp[0], R2_temp[1], R2_temp[2], R2_temp[3]);
		fprintf(fp_Result,"R3_GPR[%d]:%d_%d_%d_%d\n", IF.R3, R3_temp[0], R3_temp[1], R3_temp[2], R3_temp[3]);
		fprintf(fp_Result,"R4_GPR[%d]:%d_%d_%d_%d\n", IF.R4, R4_temp[0], R4_temp[1], R4_temp[2], R4_temp[3]);
	}
	
	fprintf(fp_Result,"-----------------------------------------------------------------\n");
	//------------ out -----------------
	if(branch_taken[0]==IF.SCC_x && branch_taken[1]==IF.SCC_y && branch_taken[2]==IF.SCC_z && branch_taken[3]==IF.SCC_w)
	{
		fprintf(fp_Result,"branch:%d%d%d%d = SCC:%d%d%d%d\n", branch_taken[0], branch_taken[1], branch_taken[2], branch_taken[3], IF.SCC_x, IF.SCC_y, IF.SCC_z, IF.SCC_w);
		fprintf(fp_Result,"New PC = %d\n", PC);
	}     
	else
	{
		fprintf(fp_Result,"branch:%d%d%d%d != SCC:%d%d%d%d\n", branch_taken[0], branch_taken[1], branch_taken[2], branch_taken[3], IF.SCC_x, IF.SCC_y, IF.SCC_z, IF.SCC_w);
		fprintf(fp_Result,"branch not taken.\n");
	}
}

void SC_VS::VS_28_MULX()
{
	fprintf(fp_Result,"Op:28 = MULX\n");
	if(IF.Rd>=32)		fprintf(fp_Result,"Rd:VOB_entry[%d]\n", IF.Rd-32);
	else				fprintf(fp_Result,"Rd:GPR[%d]\n", IF.Rd);
	if(IF.Rs>=32)		fprintf(fp_Result,"Rs:VAB_entry[%d]\n", IF.Rs-32);
	else				fprintf(fp_Result,"Rs:GPR[%d]\n", IF.Rs);
	if(IF.Rt>=32)		fprintf(fp_Result,"Rt:VAB_entry[%d]\n", IF.Rt-32);
	else				fprintf(fp_Result,"Rt:GPR[%d]\n", IF.Rt);
	fprintf(fp_Result,"Rd2:GPR[%d]\n", IF.Rd2);
	fprintf(fp_Result,"Sw_s:%d%d%d%d\n", IF.Sw_s_x, IF.Sw_s_y, IF.Sw_s_z, IF.Sw_s_w); 
	fprintf(fp_Result,"Sw_t:%d%d%d%d\n", IF.Sw_t_x, IF.Sw_t_y, IF.Sw_t_z, IF.Sw_t_w); 
	fprintf(fp_Result,"Neg:%d%d%d\n", IF.Neg_Rs, IF.Neg_Rt, IF.Neg_Rd);
	fprintf(fp_Result,"WM:%d%d%d%d\n", IF.WM_x, IF.WM_y, IF.WM_z, IF.WM_w);
	fprintf(fp_Result,"FXL:%d\n", IF.FXL);
	fprintf(fp_Result,"S/V:%d\n", IF.S_V);
	if(IF.FXL!=0)
		fprintf(fp_Result,"FXL must be 0.\n");

	//------------Initial variable--------------------------------------
	for(i=0;i<=3;i++)
	{
		mulx_temp_H[i] = 0;
		mulx_temp_L[i] = 0;
		Rs_SV_temp[i] = 0;
		Rt_SV_temp[i] = 0;
		Rs_swiz_temp[i] = 0;
		Rt_swiz_temp[i] = 0;

	}
	//S_V modified by TSAO
	//Load Rs
	Rs_SV_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0];
	Rs_SV_temp[1] = (IF.Rs>=32) ? VAB[IF.Rs-32][1] : GPR[IF.Rs][1];
	Rs_SV_temp[2] = (IF.Rs>=32) ? VAB[IF.Rs-32][2] : GPR[IF.Rs][2];
	Rs_SV_temp[3] = (IF.Rs>=32) ? VAB[IF.Rs-32][3] : GPR[IF.Rs][3];
	//Load Rt
	Rt_SV_temp[0] = (IF.Rt>=32) ? VAB[IF.Rt-32][0] : GPR[IF.Rt][0];
	Rt_SV_temp[1] = (IF.Rt>=32) ? VAB[IF.Rt-32][1] : GPR[IF.Rt][1];
	Rt_SV_temp[2] = (IF.Rt>=32) ? VAB[IF.Rt-32][2] : GPR[IF.Rt][2];
	Rt_SV_temp[3] = (IF.Rt>=32) ? VAB[IF.Rt-32][3] : GPR[IF.Rt][3];
	//----------------S/V---------------------------------
	for(i=0; i<=3; i++)
	{
		if(i>IF.S_V)
		{
			Rs_SV_temp[i] = 0;
			Rt_SV_temp[i] = 0;
		}
	}
	//Rs do swizzle
	switch(IF.Sw_s_x) {
	case 0:	Rs_swiz_temp[0] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[0] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[0] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[0] = Rs_SV_temp[3];break;
	}
	switch(IF.Sw_s_y) {
	case 0:	Rs_swiz_temp[1] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[1] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[1] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[1] = Rs_SV_temp[3];break;
	}
	switch(IF.Sw_s_z) {
	case 0:	Rs_swiz_temp[2] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[2] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[2] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[2] = Rs_SV_temp[3];break;
	}
	switch(IF.Sw_s_w) {
	case 0:	Rs_swiz_temp[3] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[3] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[3] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[3] = Rs_SV_temp[3];break;
	}
	
	//Rt do swizzle
	switch(IF.Sw_t_x) {
	case 0:	Rt_swiz_temp[0] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[0] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[0] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[0] = Rt_SV_temp[3];break;
	}
	switch(IF.Sw_t_y) {
	case 0:	Rt_swiz_temp[1] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[1] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[1] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[1] = Rt_SV_temp[3];break;
	}
	switch(IF.Sw_t_z) {
	case 0:	Rt_swiz_temp[2] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[2] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[2] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[2] = Rt_SV_temp[3];break;
	}
	switch(IF.Sw_t_w) {
	case 0:	Rt_swiz_temp[3] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[3] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[3] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[3] = Rt_SV_temp[3];break;
	}

	//------------Neg---------------------------------------
	if(IF.Neg_Rs==1)
	{
		if(IF.FXL==0)
		{
			for(i=0;i<=3;i++)
			{
				Rs_swiz_temp_FX[i] = -(*((int*)((void*)&Rs_swiz_temp[i])));
				Rs_swiz_temp[i] = *((unsigned int*)((void*)&Rs_swiz_temp_FX[i]));
			}
		}
	}

	if(IF.Neg_Rt==1)
	{
		if(IF.FXL==0)
		{
			for(i=0;i<=3;i++)
			{
				Rt_swiz_temp_FX[i] = -(*((int*)((void*)&Rt_swiz_temp[i])));
				Rt_swiz_temp[i] = *((unsigned int*)((void*)&Rt_swiz_temp_FX[i]));
			}
		}
	}

	if(IF.Rs>=32) {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%d_%d_%d_%d\n", IF.Rs-32, VAB[IF.Rs-32][0], VAB[IF.Rs-32][1], VAB[IF.Rs-32][2], VAB[IF.Rs-32][3]);
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, VAB[IF.Rs-32][0], VAB[IF.Rs-32][1], VAB[IF.Rs-32][2], VAB[IF.Rs-32][3]);
		}
		else
			fprintf(fp_Result,"Rs:MULX doesn't support floating point.\n");
	}
	else {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rs:GPR[%d]:%d_%d_%d_%d\n", IF.Rs, GPR[IF.Rs][0], GPR[IF.Rs][1], GPR[IF.Rs][2], GPR[IF.Rs][3]);
			fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs, GPR[IF.Rs][0], GPR[IF.Rs][1], GPR[IF.Rs][2], GPR[IF.Rs][3]);
		}
		else
			fprintf(fp_Result,"Rs:MULX doesn't support floating point.\n");
	}
	if(IF.Rt>=32) {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%d_%d_%d_%d\n", IF.Rt-32, VAB[IF.Rt-32][0], VAB[IF.Rt-32][1], VAB[IF.Rt-32][2], VAB[IF.Rt-32][3]);
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt-32, VAB[IF.Rt-32][0], VAB[IF.Rt-32][1], VAB[IF.Rt-32][2], VAB[IF.Rt-32][3]);
		}
		else
			fprintf(fp_Result,"Rt:MULX doesn't support floating point.\n");
	}
	else {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rt:GPR[%d]:%d_%d_%d_%d\n", IF.Rt, GPR[IF.Rt][0], GPR[IF.Rt][1], GPR[IF.Rt][2], GPR[IF.Rt][3]);
			fprintf(fp_Result,"Rt:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt, GPR[IF.Rt][0], GPR[IF.Rt][1], GPR[IF.Rt][2], GPR[IF.Rt][3]);
		}
		else
			fprintf(fp_Result,"Rt:MULX doesn't support floating point.\n");
	}

	fprintf(fp_Result, "After Swizzle/Neg/SV:\n");
	if(IF.Rs>=32) {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%d_%d_%d_%d\n", IF.Rs-32, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
			fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
		}
		else
			fprintf(fp_Result,"Rs:MULX doesn't support floating point.\n");
	}
	else {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rs:GPR[%d]:%d_%d_%d_%d\n", IF.Rs,Rs_swiz_temp[0],Rs_swiz_temp[1],Rs_swiz_temp[2],Rs_swiz_temp[3]);
			fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs,Rs_swiz_temp[0],Rs_swiz_temp[1],Rs_swiz_temp[2],Rs_swiz_temp[3]);
		}
		else
			fprintf(fp_Result,"Rs:MULX doesn't support floating point.\n");
	}
	if(IF.Rt>=32) {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%d_%d_%d_%d\n", IF.Rt-32, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
			fprintf(fp_Result,"Rt:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt-32, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
		}
		else
			fprintf(fp_Result,"Rt:MULX doesn't support floating point.\n");
	}
	else {
		if(IF.FXL==0) {
			fprintf(fp_Result,"Rt:GPR[%d]:%d_%d_%d_%d\n", IF.Rt, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
			fprintf(fp_Result,"Rt:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
		}
		else
			fprintf(fp_Result,"Rt:MULX doesn't support floating point.\n");
	}
	/*
	//------------Neg---------------------------------------
	if(IF.Neg_Rs==1)
	{
		if(IF.FXL==0)
		{
			for(i=0;i<=3;i++)
				Rs_swiz_temp_FX[i] = -(*((int*)((void*)&Rs_swiz_temp[i])));
		}
	}
	else
	{
		if(IF.FXL==0)
		{
			for(i=0;i<=3;i++)
				Rs_swiz_temp_FX[i] = *((int*)((void*)&Rs_swiz_temp[i]));
		}
	}

	if(IF.Neg_Rt==1)
	{
		if(IF.FXL==0)
		{
			for(i=0;i<=3;i++)
				Rt_swiz_temp_FX[i] = -(*((int*)((void*)&Rt_swiz_temp[i])));
		}
	}
	else
	{
		if(IF.FXL==0)
		{
			for(i=0;i<=3;i++)
				Rt_swiz_temp_FX[i] = *((int*)((void*)&Rt_swiz_temp[i]));
		}
	}
	*/
	//------------MULX Function---------------------------------------
	if(IF.FXL==1)
	{
		for(i=0;i<=3;i++)
		{
			Rs_swiz_temp_FX[i] = *((int*)((void*)&Rs_swiz_temp[i]));
			Rt_swiz_temp_FX[i] = *((int*)((void*)&Rt_swiz_temp[i]));
			mulx64_temp_FX[i] = Rs_swiz_temp_FX[i] * Rt_swiz_temp_FX[i];
			mulx_temp_L[i] = *((unsigned int*)((void*)&mulx64_temp_FX[i]));
			mulx64_temp_FX[i] = mulx64_temp_FX[i] >> 32;
			mulx_temp_H[i] = *((unsigned int*)((void*)&mulx64_temp_FX[i]));
		}
	}

	//------------ WB --------------------------------------------------
	if(IF.WM_x==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][0] = mulx_temp_L[0];
		else			GPR[IF.Rd][0] = mulx_temp_L[0];
		GPR[IF.Rd2][0] = mulx_temp_H[0];
	}
	if(IF.WM_y==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][1] = mulx_temp_L[1];
		else			GPR[IF.Rd][1] = mulx_temp_L[1];
		GPR[IF.Rd2][0] = mulx_temp_H[1];
	}
	if(IF.WM_z==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][2] = mulx_temp_L[2];
		else			GPR[IF.Rd][2] = mulx_temp_L[2];
		GPR[IF.Rd2][0] = mulx_temp_H[2];
	}
	if(IF.WM_w==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][3] = mulx_temp_L[3];
		else			GPR[IF.Rd][3] = mulx_temp_L[3];
		GPR[IF.Rd2][0] = mulx_temp_H[3];
	}
	fprintf(fp_Result,"-----------------------------------------------------------------\n");
	if(IF.Rd>=32)
	{
		if(IF.FXL==0)
		{
			fprintf(fp_Result,"Rd1:VOB_entry[%d] = %d_%d_%d_%d\n", IF.Rd-32, VOB[IF.Rd-32][0], VOB[IF.Rd-32][1], VOB[IF.Rd-32][2], VOB[IF.Rd-32][3]);
			fprintf(fp_Result,"Rd1:VOB_entry[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd-32, VOB[IF.Rd-32][0], VOB[IF.Rd-32][1], VOB[IF.Rd-32][2], VOB[IF.Rd-32][3]);
		}
		else
			fprintf(fp_Result,"Rd1:MULX doesn't support floating point.\n");
	}
	else
	{
		if(IF.FXL==0)
		{
			fprintf(fp_Result,"Rd1:GPR[%d] = %d_%d_%d_%d\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
			fprintf(fp_Result,"Rd1:GPR[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
		}
		else
			fprintf(fp_Result,"Rd1:MULX doesn't support floating point.\n");
		
	}

	if(IF.FXL==0)
	{
		fprintf(fp_Result,"Rd2:GPR[%d] = %d_%d_%d_%d\n", IF.Rd2, GPR[IF.Rd2][0], GPR[IF.Rd2][1], GPR[IF.Rd2][2], GPR[IF.Rd2][3]);
		fprintf(fp_Result,"Rd2:GPR[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd2, GPR[IF.Rd2][0], GPR[IF.Rd2][1], GPR[IF.Rd2][2], GPR[IF.Rd2][3]);
	}
	else
		fprintf(fp_Result,"Rd2:MULX doesn't support floating point.\n");
}

void SC_VS::VS_29_CAL()
{
	fprintf(fp_Result,"Op:29 = CAL\n");
	if(IF.R1>=32)		fprintf(fp_Result,"R1:VOB_entry[%d]\n", IF.R1-32);
	else				fprintf(fp_Result,"R1:GPR[%d]\n", IF.R1);
	if(IF.R2>=32)		fprintf(fp_Result,"R2:VAB_entry[%d]\n", IF.R2-32);
	else				fprintf(fp_Result,"R2:GPR[%d]\n", IF.R2);
	fprintf(fp_Result,"Addr:[%d]\n", ((short signed)IF.Addr));
	fprintf(fp_Result,"FXL:%d\n", IF.FXL);
	fprintf(fp_Result,"S/V:%d\n", IF.S_V);
	if(IF.FXL!=0 || IF.S_V!=0)
		fprintf(fp_Result,"FXL must be 0 and S/V must be 00.\n");

	//-----------Initial variable------------------------------------------
	for(i=0;i<=3;i++)
	{
		PC_temp = 0;
		R2_temp[i] = 0;
		R2_temp_FX[i] = 0;
	}

	//------------determine the source----------------------------------
	for(i=0; i<=3; i++)
		R2_temp[i] = (IF.R2>=32) ? VAB[IF.R2-32][i] : GPR[IF.R2][i];

	//----------print for debug------------------------------------------------------------------------
	if(IF.R2>=32)
		fprintf(fp_Result,"R2:VAB_entry[%d]:%d_%d_%d_%d\n", IF.R2, R2_temp[0], R2_temp[1], R2_temp[2], R2_temp[3]);
	else
		fprintf(fp_Result,"R2:GPR[%d]:%d_%d_%d_%d\n", IF.R2, R2_temp[0], R2_temp[1], R2_temp[2], R2_temp[3]);

	//----------CAL function----------------------------------------------------
	if(IF.R1!=0)
	{
		PC_temp = *((unsigned int*)((void*)&PC));
		GPR[IF.R1][0] = PC_temp;
	}
	R2_temp_FX[0] = *((int*)((void*)&R2_temp[0]));
	PC = R2_temp_FX[0]+(short signed)IF.Addr;
	
	fprintf(fp_Result,"-----------------------------------------------------------------\n");
	//------------ out -----------------
	if(IF.R1!=0)
		fprintf(fp_Result,"R1:GPR[%d].x:%d\n", IF.R1, GPR[IF.R1][0]);
	else
		fprintf(fp_Result,"GPR0 cant be written!!\n");
               
	if(PC==0)
		fprintf(fp_Result,"PC returns to 0\n");
	else
		fprintf(fp_Result,"PC will jump to:%d\n", PC);
}

void SC_VS::VS_30_SHRL()
{
	fprintf(fp_Result,"Op:30 = SHRL\n");
	if(IF.Rd>=32)		fprintf(fp_Result,"Rd:VOB_entry[%d]\n", IF.Rd-32);
	else				fprintf(fp_Result,"Rd:GPR[%d]\n", IF.Rd);
	if(IF.Rs>=32)		fprintf(fp_Result,"Rs:VAB_entry[%d]\n", IF.Rs-32);
	else				fprintf(fp_Result,"Rs:GPR[%d]\n", IF.Rs);
	if(IF.Rt>=32)		fprintf(fp_Result,"Rt:VAB_entry[%d]\n", IF.Rt-32);
	else				fprintf(fp_Result,"Rt:GPR[%d]\n", IF.Rt);
	fprintf(fp_Result,"Sw_d:%d%d%d%d\n", IF.Sw_d_x, IF.Sw_d_y, IF.Sw_d_z, IF.Sw_d_w); 
	fprintf(fp_Result,"Sw_s:%d%d%d%d\n", IF.Sw_s_x, IF.Sw_s_y, IF.Sw_s_z, IF.Sw_s_w); 
	fprintf(fp_Result,"Sw_t:%d%d%d%d\n", IF.Sw_t_x, IF.Sw_t_y, IF.Sw_t_z, IF.Sw_t_w); 
	fprintf(fp_Result,"Neg:%d%d\n", IF.Neg_Rs, IF.Neg_Rt);
	fprintf(fp_Result,"R/L:%d\n", IF.R_L);
	fprintf(fp_Result,"WM:%d%d%d%d\n", IF.WM_x, IF.WM_y, IF.WM_z, IF.WM_w);
	fprintf(fp_Result,"L/A:%d\n", IF.L_A);
	fprintf(fp_Result,"S/V:%d\n", IF.S_V);
	if( IF.Neg_Rs!=0 || IF.Neg_Rt!=0 )
		fprintf(fp_Result,"Neg must be 00.\n");

	//------------Initial variable--------------------------------------
	for(i=0;i<=3;i++)
	{
		shrl_temp[i] = 0;
		Rs_SV_temp[i] = 0;
		Rt_SV_temp[i] = 0;
		Rs_swiz_temp[i] = 0;
		Rt_swiz_temp[i] = 0;
	}
	//S_V modified by TSAO
	//Load Rs
	Rs_SV_temp[0] = (IF.Rs>=32) ? VAB[IF.Rs-32][0] : GPR[IF.Rs][0];
	Rs_SV_temp[1] = (IF.Rs>=32) ? VAB[IF.Rs-32][1] : GPR[IF.Rs][1];
	Rs_SV_temp[2] = (IF.Rs>=32) ? VAB[IF.Rs-32][2] : GPR[IF.Rs][2];
	Rs_SV_temp[3] = (IF.Rs>=32) ? VAB[IF.Rs-32][3] : GPR[IF.Rs][3];
	//Load Rt
	Rt_SV_temp[0] = (IF.Rt>=32) ? VAB[IF.Rt-32][0] : GPR[IF.Rt][0];
	Rt_SV_temp[1] = (IF.Rt>=32) ? VAB[IF.Rt-32][1] : GPR[IF.Rt][1];
	Rt_SV_temp[2] = (IF.Rt>=32) ? VAB[IF.Rt-32][2] : GPR[IF.Rt][2];
	Rt_SV_temp[3] = (IF.Rt>=32) ? VAB[IF.Rt-32][3] : GPR[IF.Rt][3];
	//----------------S/V---------------------------------
	for(i=0; i<=3; i++)
	{
		if(i>IF.S_V)
		{
			Rs_SV_temp[i] = 0;
			Rt_SV_temp[i] = 0;
		}
	}
	//Rs do swizzle
	switch(IF.Sw_s_x) {
	case 0:	Rs_swiz_temp[0] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[0] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[0] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[0] = Rs_SV_temp[3];break;
	}
	switch(IF.Sw_s_y) {
	case 0:	Rs_swiz_temp[1] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[1] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[1] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[1] = Rs_SV_temp[3];break;
	}
	switch(IF.Sw_s_z) {
	case 0:	Rs_swiz_temp[2] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[2] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[2] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[2] = Rs_SV_temp[3];break;
	}
	switch(IF.Sw_s_w) {
	case 0:	Rs_swiz_temp[3] = Rs_SV_temp[0];break;
	case 1:	Rs_swiz_temp[3] = Rs_SV_temp[1];break;
	case 2:	Rs_swiz_temp[3] = Rs_SV_temp[2];break;
	case 3:	Rs_swiz_temp[3] = Rs_SV_temp[3];break;
	}
	
	//Rt do swizzle
	switch(IF.Sw_t_x) {
	case 0:	Rt_swiz_temp[0] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[0] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[0] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[0] = Rt_SV_temp[3];break;
	}
	switch(IF.Sw_t_y) {
	case 0:	Rt_swiz_temp[1] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[1] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[1] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[1] = Rt_SV_temp[3];break;
	}
	switch(IF.Sw_t_z) {
	case 0:	Rt_swiz_temp[2] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[2] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[2] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[2] = Rt_SV_temp[3];break;
	}
	switch(IF.Sw_t_w) {
	case 0:	Rt_swiz_temp[3] = Rt_SV_temp[0];break;
	case 1:	Rt_swiz_temp[3] = Rt_SV_temp[1];break;
	case 2:	Rt_swiz_temp[3] = Rt_SV_temp[2];break;
	case 3:	Rt_swiz_temp[3] = Rt_SV_temp[3];break;
	}

	if(IF.Rs>=32)
		fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, VAB[IF.Rs-32][0], VAB[IF.Rs-32][1], VAB[IF.Rs-32][2], VAB[IF.Rs-32][3]);
	else
		fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs, GPR[IF.Rs][0], GPR[IF.Rs][1], GPR[IF.Rs][2], GPR[IF.Rs][3]);

	if(IF.Rt>=32) {
		fprintf(fp_Result,"Rt:VAB_entry[%d]:%d_%d_%d_%d\n", IF.Rt-32, VAB[IF.Rt-32][0], VAB[IF.Rt-32][1], VAB[IF.Rt-32][2], VAB[IF.Rt-32][3]);
		fprintf(fp_Result,"Rt:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt-32, VAB[IF.Rt-32][0], VAB[IF.Rt-32][1], VAB[IF.Rt-32][2], VAB[IF.Rt-32][3]);
	}
	else {
		fprintf(fp_Result,"Rt:GPR[%d]:%d_%d_%d_%d\n", IF.Rt, GPR[IF.Rt][0], GPR[IF.Rt][1], GPR[IF.Rt][2], GPR[IF.Rt][3]);
		fprintf(fp_Result,"Rt:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt, GPR[IF.Rt][0], GPR[IF.Rt][1], GPR[IF.Rt][2], GPR[IF.Rt][3]);
	}

	fprintf(fp_Result, "After Swizzle/SV:\n");
	if(IF.Rs>=32)
		fprintf(fp_Result,"Rs:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs-32, Rs_swiz_temp[0], Rs_swiz_temp[1], Rs_swiz_temp[2], Rs_swiz_temp[3]);
	else
		fprintf(fp_Result,"Rs:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rs,Rs_swiz_temp[0],Rs_swiz_temp[1],Rs_swiz_temp[2],Rs_swiz_temp[3]);
	if(IF.Rt>=32) {
		fprintf(fp_Result,"Rt:VAB_entry[%d]:%d_%d_%d_%d\n", IF.Rt-32, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
		fprintf(fp_Result,"Rt:VAB_entry[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt-32, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
	}
	else {
		fprintf(fp_Result,"Rt:GPR[%d]:%d_%d_%d_%d\n", IF.Rt, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
		fprintf(fp_Result,"Rt:GPR[%d]:%.8x_%.8x_%.8x_%.8x\n", IF.Rt, Rt_swiz_temp[0], Rt_swiz_temp[1], Rt_swiz_temp[2], Rt_swiz_temp[3]);
	}

	//------------SHRL Function---------------------------------------
	if(IF.L_A==0)
	{
		for(i=0;i<=3;i++)
		{
			if(IF.R_L==0)
				shrl_temp_L[i] = Rs_swiz_temp[i] >> Rt_swiz_temp[i];
			else
				shrl_temp_L[i] = Rs_swiz_temp[i] << Rt_swiz_temp[i];
			shrl_temp[i] = shrl_temp_L[i];
		}
	}
	else
	{
		for(i=0;i<=3;i++)
		{
			Rs_swiz_temp_FX[i] = *((int*)((void*)&Rs_swiz_temp[i]));
			if(IF.R_L==0)
				shrl_temp_A[i] = Rs_swiz_temp_FX[i] >> Rt_swiz_temp[i];
			else
				shrl_temp_A[i] = Rs_swiz_temp_FX[i] << Rt_swiz_temp[i];
			shrl_temp[i] = *((unsigned int*)((void*)&shrl_temp_A[i]));
		}
	}

	//------------ WB --------------------------------------------------
	if(IF.WM_x==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][0] = shrl_temp[0];
		else			GPR[IF.Rd][0] = shrl_temp[0];
	}
	if(IF.WM_y==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][1] = shrl_temp[1];
		else			GPR[IF.Rd][1] = shrl_temp[1];
	}
	if(IF.WM_z==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][2] = shrl_temp[2];
		else			GPR[IF.Rd][2] = shrl_temp[2];
	}
	if(IF.WM_w==1) {
		if(IF.Rd>=32)	VOB[IF.Rd-32][3] = shrl_temp[3];
		else			GPR[IF.Rd][3] = shrl_temp[3];
	}
	fprintf(fp_Result,"-----------------------------------------------------------------\n");
	if(IF.Rd>=32)
		fprintf(fp_Result,"Rd:VOB_entry[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd-32, VOB[IF.Rd-32][0], VOB[IF.Rd-32][1], VOB[IF.Rd-32][2], VOB[IF.Rd-32][3]);
	else
		fprintf(fp_Result,"Rd:GPR[%d] = %.8x_%.8x_%.8x_%.8x\n", IF.Rd, GPR[IF.Rd][0], GPR[IF.Rd][1], GPR[IF.Rd][2], GPR[IF.Rd][3]);
}

void SC_VS::VS_31_NOP()
{
	fprintf(fp_Result,"Op:31 = NOP\n");
	fprintf(fp_Result,"stall:[%d] cycles\n", IF.NOP_stall_cycle);
	fprintf(fp_Result,"For RAW situation\n");  
	fprintf(fp_Result,"==============================================\n"); 
}

void SC_VS::decode(int program_counter)
{
//---------------------char-to-int for each block for decode---------------	
	//--------Op-code------------
	IF.op = 0;
	for(i=0;i<=4;i++){
		IF.op <<= 1;
		IF.op = IF.op + IM[program_counter][i]-48;
	}
	//printf("OPcode = %d\n",IF.op);                 
    if(IF.op==31)//---------------NOP
    {
		IF.NOP_stall_cycle = 0;
		for(i=5;i<=8;i++){
			IF.NOP_stall_cycle <<= 1;
			IF.NOP_stall_cycle = IF.NOP_stall_cycle + IM[program_counter][i]-48;
		}
	}
    else if((IF.op>=24 && IF.op<=27) || IF.op==29)//-----for branch instructions
    {
		//--------R1------------
		IF.R1 = 0;
		for(i=5;i<=10;i++){
			IF.R1 <<= 1;
			IF.R1 = IF.R1 + IM[program_counter][i]-48;
		}
		
		//--------R2------------
		IF.R2 = 0;
		for(i=11;i<=16;i++){
			IF.R2 <<= 1;
			IF.R2 = IF.R2 + IM[program_counter][i]-48;
		}
		
		//--------Addr------------
		IF.Addr = 0;
		for(i=17;i<=32;i++){
			IF.Addr <<= 1;
			IF.Addr = IF.Addr + IM[program_counter][i]-48;
		}
		
		//--------R3------------
		IF.R3 = 0;
		for(i=33;i<=38;i++){
			IF.R3 <<= 1;
			IF.R3 = IF.R3 + IM[program_counter][i]-48;
		}
		
		//--------R4------------
		IF.R4 = 0;
		for(i=39;i<=43;i++){
			IF.R4 <<= 1;
			IF.R4 = IF.R4 + IM[program_counter][i]-48;
		}
		
		//--------Branch MODE------------
		IF.Br_m = 0;
		IF.Br_m = IM[program_counter][49]-48;
		
		//END--------Branch MODE------------
		
		//--------Sw_Rs1 (Rs)------------
		IF.Sw_s_x = 0;
		for(i=33;i<=34;i++){
			IF.Sw_s_x <<= 1;
			IF.Sw_s_x = IF.Sw_s_x + IM[program_counter][i]-48;
		}
		
		IF.Sw_s_y = 0;
		for(i=35;i<=36;i++){
			IF.Sw_s_y <<= 1;
			IF.Sw_s_y = IF.Sw_s_y + IM[program_counter][i]-48;
		}
		
		IF.Sw_s_z = 0;
		for(i=37;i<=38;i++){
			IF.Sw_s_z <<= 1;
			IF.Sw_s_z = IF.Sw_s_z + IM[program_counter][i]-48;
		}
		
		IF.Sw_s_w = 0;
		for(i=39;i<=40;i++){
			IF.Sw_s_w <<= 1;
			IF.Sw_s_w = IF.Sw_s_w + IM[program_counter][i]-48;
		}
		//END--------Sw_Rs1 (Rs)------------
		
		//--------Sw_Rs2 (Rt)------------         
		IF.Sw_t_x = 0;
		for(i=41;i<=42;i++){
			IF.Sw_t_x <<= 1;
			IF.Sw_t_x = IF.Sw_t_x + IM[program_counter][i]-48;
		}
     
		IF.Sw_t_y = 0;
		for(i=43;i<=44;i++){
			IF.Sw_t_y <<= 1;
			IF.Sw_t_y = IF.Sw_t_y + IM[program_counter][i]-48;
		}
    
		IF.Sw_t_z = 0;
		for(i=45;i<=46;i++){
			IF.Sw_t_z <<= 1;
			IF.Sw_t_z = IF.Sw_t_z + IM[program_counter][i]-48;
		}
     
		IF.Sw_t_w = 0;
		for(i=47;i<=48;i++){
			IF.Sw_t_w <<= 1;
			IF.Sw_t_w = IF.Sw_t_w + IM[program_counter][i]-48;
		}
		//END--------Sw_Rs2 (Rt)------------
		
		if(IF.op!=29){
			//--------SCC------------
			IF.SCC_x = IM[program_counter][50]-48;
			IF.SCC_y = IM[program_counter][51]-48;
			IF.SCC_z = IM[program_counter][52]-48;
			IF.SCC_w = IM[program_counter][53]-48;
		}
	}
    else//-----------------------for other instructions
    {
		//--------Rd------------
		IF.Rd = 0;
		for(i=5;i<=10;i++){
			IF.Rd <<= 1;
			IF.Rd = IF.Rd + IM[program_counter][i]-48;
		}

		//--------Rd2------------
		IF.Rd2 = 0;
		for(i=23;i<=28;i++){
			IF.Rd2 <<= 1;
			IF.Rd2 = IF.Rd2 + IM[program_counter][i]-48;
		}
		
		//--------Rs------------
		IF.Rs = 0;
		for(i=11;i<=16;i++){
			IF.Rs <<= 1;
			IF.Rs = IF.Rs + IM[program_counter][i]-48;
		}
		
		//-------FXL-----------
		IF.FXL = IM[program_counter][54]-48;

		//--------Imm------------
		IF.Imm = 0;
		for(i=17;i<=48;i++){
			IF.Imm <<= 1;
			IF.Imm = IF.Imm + IM[program_counter][i]-48;
		}

		//--------Rt------------ 
		IF.Rt = 0;
		for(i=17;i<=22;i++){
			IF.Rt <<= 1;
			IF.Rt = IF.Rt + IM[program_counter][i]-48;
		}
       
       //--------Sw_d------------
		IF.Sw_d_x = 0;
		for(i=23;i<=24;i++){
			IF.Sw_d_x <<= 1;
			IF.Sw_d_x = IF.Sw_d_x + IM[program_counter][i]-48;
		}
		
		IF.Sw_d_y = 0;
		for(i=25;i<=26;i++){
			IF.Sw_d_y <<= 1;
			IF.Sw_d_y = IF.Sw_d_y + IM[program_counter][i]-48;
		}
		
		IF.Sw_d_z = 0;
		for(i=27;i<=28;i++){
			IF.Sw_d_z <<= 1;
			IF.Sw_d_z = IF.Sw_d_z + IM[program_counter][i]-48;
		}
				
		IF.Sw_d_w = 0;
		for(i=29;i<=30;i++){
			IF.Sw_d_w <<= 1;
			IF.Sw_d_w = IF.Sw_d_w + IM[program_counter][i]-48;
		}
		
		//--------Sw_s------------
		IF.Sw_s_x = 0;
		for(i=31;i<=32;i++){
			IF.Sw_s_x <<= 1;
			IF.Sw_s_x = IF.Sw_s_x + IM[program_counter][i]-48;
		}
		
		IF.Sw_s_y = 0;
		for(i=33;i<=34;i++){
			IF.Sw_s_y <<= 1;
			IF.Sw_s_y = IF.Sw_s_y + IM[program_counter][i]-48;
		}
		
		IF.Sw_s_z = 0;
		for(i=35;i<=36;i++){
			IF.Sw_s_z <<= 1;
			IF.Sw_s_z = IF.Sw_s_z + IM[program_counter][i]-48;
		}
		
		IF.Sw_s_w = 0;
		for(i=37;i<=38;i++){
			IF.Sw_s_w <<= 1;
			IF.Sw_s_w = IF.Sw_s_w + IM[program_counter][i]-48;
		}
		
		//--------Sw_t------------         
		IF.Sw_t_x = 0;
		for(i=39;i<=40;i++){
			IF.Sw_t_x <<= 1;
			IF.Sw_t_x = IF.Sw_t_x + IM[program_counter][i]-48;
		}
     
		IF.Sw_t_y = 0;
		for(i=41;i<=42;i++){
			IF.Sw_t_y <<= 1;
			IF.Sw_t_y = IF.Sw_t_y + IM[program_counter][i]-48;
		}
    
		IF.Sw_t_z = 0;
		for(i=43;i<=44;i++){
			IF.Sw_t_z <<= 1;
			IF.Sw_t_z = IF.Sw_t_z + IM[program_counter][i]-48;
		}
     
		IF.Sw_t_w = 0;
		for(i=45;i<=46;i++){
			IF.Sw_t_w <<= 1;
			IF.Sw_t_w = IF.Sw_t_w + IM[program_counter][i]-48;
		}
		
		
		//--------Neg------------    
		IF.Neg_Rs = IM[program_counter][47]-48; 
		IF.Neg_Rt = IM[program_counter][48]-48;
		IF.Neg_Rd = IM[program_counter][49]-48;

		//--------R_L------------
		IF.R_L = IM[program_counter][49]-48;

		//--------WM------------
		IF.WM_x = IM[program_counter][50]-48;
		IF.WM_y = IM[program_counter][51]-48;
		IF.WM_z = IM[program_counter][52]-48;
		IF.WM_w = IM[program_counter][53]-48;
	}
	//END else
	
	//-------FXL-----------
	IF.FXL = IM[program_counter][54]-48;
	IF.L_A = IM[program_counter][54]-48;
	
	//-------S/V-----------
    IF.S_V = 0;
    for(i=55;i<=56;i++){
		IF.S_V <<= 1;
		IF.S_V = IF.S_V + IM[program_counter][i]-48;
	}
//------------------------------------------------------------------------------      
        
      
//-------------printf for debug------------------------------------------------- 
    
    fprintf(fp_Result,"PC = %d\n", PC);

	switch(IF.op)
    {
		case  1 : VS_01_DSQ();    break;
		case  2 : VS_02_RCP();    break;
		case  3 : VS_03_RSQ();    break;
		case  4 : VS_04_LOG2();   break;
		case  5 : VS_05_POW2();   break;
		case  6 : VS_06_DIV();    break;
		case  7 : VS_07_MVR();    break;
		case  8 : VS_08_LDV();    break;
		case  9 : VS_09_STV();    break;
		case 10 : VS_10_MVI();    break;
		case 11 : VS_11_MIN();    break;
		case 12 : VS_12_MAX();    break;
		case 13 : VS_13_ITOF_FTOI(); break;
		case 14 : VS_14_FIN();    break;
		case 16 : VS_16_ADD();    break;
		case 17 : VS_17_MULF();   break;
		case 18 : VS_18_MAD();    break;
		case 19 : VS_19_DP();     break;
		case 20 : VS_20_INV();    break;
		case 21 : VS_21_AND();    break;
		case 22 : VS_22_OR();     break;
		case 23 : VS_23_XOR();    break;
		case 24 : VS_24_BEQ();    break;
		case 25 : VS_25_BNEQ();   break;
		case 26 : VS_26_BLT();    break;
		case 27 : VS_27_BGT();    break;
		case 28 : VS_28_MULX();   break;
		case 29 : VS_29_CAL();    break;
		case 30 : VS_30_SHRL();   break;
		case 31 : VS_31_NOP();    break;
    }
}

