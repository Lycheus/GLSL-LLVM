/*
 * FS.cpp
 *
 *  Created on: 2009/12/1
 *      Author: Jason
 */
#include <iostream>
#include <cmath>
#include <time.h>
#include "FS.h"
#include "frag_define.h"
#define DEBUG_MODE 1
#define SIMPLE_DEBUG_MODE 1

using namespace std;

//added by sheng-chih
//sc_bv<4>   temp_mask;
//unsigned long long temp_SLAVE_MEM[512];
//extern int temp_INS_NUM;

unsigned int *copy_TEX_MEM;
clock_t previous_c,now_c,total_c=0;
int clock_start=0,exe_count=0;

//end added

void FS::bus_wrap(void)
{   
    int i;
    //cout<<"FS.Target: wait request"<<endl;    
    do{
        info_sp  = sp_transfer.read();
        Type     = info_sp.Type;
        Addr     = info_sp.Addr;
        Burst    = info_sp.Burst;    
        DataAddr = info_sp.DataDest;//point :link to read/write data
        
        //cout<<"FS.Target: Type, Addr, Burst:"<<Type<<" "<<Addr<<" "<<Burst<<endl;
        //cout<<"FS.Target: Data = ";

        for(i=0;i<Burst;i++)
        {   	        
	  SLAVE_MEM[Addr+i]=DataAddr[i];
	  mylog<<Addr+i<<":  "<<SLAVE_MEM[Addr+i]<<endl;
        }
        //cout<<endl<<"FS.Target: Send response"<<endl;
        mylog<<endl;
        sp_response.write(1);
        
    }while(Addr!=SLAVE_LENGTH-1);
    
    //-----------------------------
    //(TEX_MEM)
    //for(i=0;i<TEX_LENGTH;++i)
    //added by sheng-chih
    /*
    FILE *pfile_pic=fopen("./file/crate3.bmp","r");
    //FILE *pfile_pic=fopen("./file/fs1005-sc.bmp","r");
    char bmp_info[54];
    for(i=0;i<54;++i)
      {
	bmp_info[i]=getc(pfile_pic);
      }
    int j;   
    int pixel_count=65535-255;
    int byte_count=0;
    for(i=0;i<102400;++i)
      {
	unsigned int catch_texture=0,get_pixel;
	for(j=0;j<3;j++)
	  {
	    get_pixel=(int)getc(pfile_pic);
	    //printf("getpixel: %x\n",get_pixel);
	    catch_texture|=get_pixel<<((j+1)*8);
	  }
	//printf("%d: %x\n",i,catch_texture);
	catch_texture&=0xFFFFFF00;
	if(i%256==0)
	  pixel_count-=512;
	else
	  pixel_count++;
	//TEX_MEM[pixel_count]=catch_texture;
	TEX_MEM[i]=catch_texture;
	//printf("i: %d\ncatch_texture: %x\n",i*4,catch_texture);
	//printf("%d: %x\n",i,catch_texture);
	//TEX_MEM[i] = 0x77777700;
      }
   
	copy_TEX_MEM=TEX_MEM;


    FILE *pfile_out=fopen("pic_out","w");
    fwrite(bmp_info,sizeof(char),54,pfile_out);
    char convert_bmp[3];
    
    for(i=0;i<65536;i++)
      {
	int convert_char;
	//printf("TEX_MEM: %x\n",TEX_MEM[i]);
	//printf("TEX_MEM>>8: %x\t%d\n",TEX_MEM[i]>>8,(char)TEX_MEM[i]>>8);
	convert_char=TEX_MEM[i]>>8;
       	memcpy(&convert_bmp[0],&convert_char,1);
	convert_char=TEX_MEM[i]>>16;
       	memcpy(&convert_bmp[1],&convert_char,1);
	convert_char=TEX_MEM[i]>>24;
       	memcpy(&convert_bmp[2],&convert_char,1);
	//printf("convert_bmp[0]: %d\n",convert_bmp[0]);
	//printf("convert_bmp[1]: %d\n",convert_bmp[1]);
	//printf("convert_bmp[2]: %d\n",convert_bmp[2]);
	fwrite(convert_bmp,sizeof(char),3,pfile_out);
      }
    printf("i: %d\n",i);

    //exit(1);
    */
    //end added

    //-----------------------------     
    //(ZS_MEM & F_MEM)
    for(i=0;i<LCD_LENGTH;++i)
    {
        ZS_MEM[i] = 0xffff00ff;//{Z(16-bits),0,S(8-bits)}    
        F_MEM[i] = 0xffffffff;    
    }
    
    flag_FS.notify(SC_ZERO_TIME);
    
}

//==================================
float FS::uint2float(sc_bv<32> input)
{
    int s_convt , exp_convt , fraction_convt;    
    float answer;	
    
    s_convt = input[31].to_bool();
    exp_convt = input.range(30,23).to_uint();
    fraction_convt = input.range(22,0).to_uint();

    if(s_convt == 0 && exp_convt ==0 && fraction_convt ==0)//0-detector
       return 0;    
    
    fraction_convt |= 0x00800000;
    
    answer = (float) fraction_convt / pow(2.0,23.0);
    answer *= pow(2.0,(double)(exp_convt-127));
    if(s_convt)
       answer *= -1;
            
    return answer;    
}

void FS::shift_param(struct render_param *temp, struct render_param *ori, int x, int y)
{
    temp->E1 = ori->E1 + (x * process_triangle.dy1) - (y * process_triangle.dx1);
    temp->E2 = ori->E2 + (x * process_triangle.dy2) - (y * process_triangle.dx2);
    temp->E3 = ori->E3 + (x * process_triangle.dy3) - (y * process_triangle.dx3);
    temp->x = ori->x + x;
    temp->y = ori->y + y;
    temp->z = ori->z + process_triangle.dz_x * x + process_triangle.dz_y * y;
    temp->r = ori->r + process_triangle.dr_x * x + process_triangle.dr_y * y;
    temp->g = ori->g + process_triangle.dg_x * x + process_triangle.dg_y * y;
    temp->b = ori->b + process_triangle.db_x * x + process_triangle.db_y * y;
    temp->a = ori->a + process_triangle.da_x * x + process_triangle.da_y * y;
    temp->u = ori->u + process_triangle.du_x * x + process_triangle.du_y * y;
    temp->v = ori->v + process_triangle.dv_x * x + process_triangle.dv_y * y;
    temp->w = ori->w + process_triangle.dw_x * x + process_triangle.dw_y * y;   
}


sc_bv<32> FS::float2bv(float input)
{
    sc_bv<32>    return_data;
    //--------------------
    sc_uint<32>  temp_data;
    int          sign,exp;
    float        fraction;	

    fraction = frexp(input,&exp);

    if(fraction == 0)//0-detector    
       return 0x00000000;    
    
    fraction*=2;
    --exp;

    //sign
    if(fraction>0)
       sign = 0;
    else
       sign = 1;
       
    //exp
    exp += 127;

    //mantissa
    fraction = (fraction-1) * pow(2.0,23.0);//shift 23 bits
    
    //return data
    temp_data = (sign<<31) + (exp <<23) + (unsigned int)fraction;
    return_data = temp_data.range(31,0);

    return return_data;   
}


void FS::fragment(struct render_param q)
{
    static int num=0;
    bool FRAG_finish;
    float s,t;

    //cout<<"<FRAGMENT_START>"<<endl; 
 
    //added by sheng-chih
    //q.u=q.x;
    //q.v=q.y;
    //cout<<"q.u: "<<q.u<<endl;
    //cout<<"q.v: "<<q.v<<endl;
    //end added
     	  
    q.u /= q.w;    
    q.v /= q.w;
    s = q.u * TEX_WIDTH;
    t = q.v * TEX_WIDTH;
    
    if(s>(TEX_WIDTH-1))
     	  s = TEX_WIDTH-1;
    if(t>(TEX_WIDTH-1))
     	  t = TEX_WIDTH-1;   
    if(s<0)
     	  s = 0;
    if(t<0)
     	  t = 0;	  

    //set [GPR]    
    //------------------------------------------------
    //R0 = {R,G,B,A} , R1 = {0,X,Y,Z} , R2 = {0,0,S,T} 
    //------------------------------------------------    
    //added by sheng-chih
    //cout<<endl<<"\tSET GPR!!!"<<endl;
    //printf("GPR:%d\n",GPR);
    //end added
    GPR[0].range(31,0) = float2bv(q.a);
    GPR[0].range(63,32) = float2bv(q.b);
    GPR[0].range(95,64) = float2bv(q.g);
    GPR[0].range(127,96) = float2bv(q.r);
    
    GPR[1].range(31,0) = float2bv(q.z);
    GPR[1].range(63,32) = float2bv(q.y);
    GPR[1].range(95,64) = float2bv(q.x);
    GPR[1].range(127,96) = float2bv((float)0);
    
    GPR[2].range(31,0) = float2bv(t);
    GPR[2].range(63,32) = float2bv(s);
    GPR[2].range(95,64) = float2bv((float)0);
    GPR[2].range(127,96) = float2bv((float)0);    

    //cout<<"<SC start>"<<endl;

    //#if DEBUG_MODE
    #if SIMPLE_DEBUG_MODE

    cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
    cout<<"R0(r) = "<<dec<<q.r<<endl;
    cout<<"        "<<hex<<GPR[0].range(127,96).to_uint()<<endl;
    cout<<"R0(g) = "<<dec<<q.g<<endl;
    cout<<"        "<<hex<<GPR[0].range(95,64).to_uint()<<endl;
    cout<<"R0(b) = "<<dec<<q.b<<endl;
    cout<<"        "<<hex<<GPR[0].range(63,32).to_uint()<<endl;
    cout<<"R0(a) = "<<dec<<q.a<<endl;
    cout<<"        "<<hex<<GPR[0].range(31,0).to_uint()<<endl;
    
    cout<<"R0(x) = "<<q.x<<endl;
    cout<<"R0(y) = "<<q.y<<endl;
    cout<<"R0(z) = "<<q.z<<endl;
    
    cout<<"R0(s) = "<<s<<endl;
    cout<<"R0(t) = "<<t<<endl;                
    cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
    
    #endif
 
    //reset Fragment Shader
    //---------------------
    //<IF reset>
    I_F->is_FETCH_full = 0;   
    I_F->PC = 0;     
    //<ID reset>
    I_D->is_DECODE_full = 0;
    //<WB reset>
    I_W->is_WB_full = 0;
    I_W->is_FIN = 0;    
        
    //active Fragment Shader
    //---------------------
    //<IF start>
    top_FETCH_start.write(1);
    top_DECODE_start.write(1);//CYLin
    top_WB_start.write(1);//CYLin
    //wait for [UNIFIED_FRAG]
    //---------------------       
    do
    {    	 
        FRAG_finish = top_FRAG_finish.read();                
    }
    while(!FRAG_finish);  

/*
    //test
    if(num >=2) 
       sc_stop();
    cout<<"NUM = "<<dec<<num<<endl; 
    num++;           
*/
    //cout<<"<FRAGMENT_END>"<<endl;
}


int FS::pixel_test(struct render_param p,int x,int y)
{
    //cout<<"<PIXEL_TEST_START>"<<endl;
    
    p.E1 = p.E1 + (x * process_triangle.dy1) - (y * process_triangle.dx1);
    p.E2 = p.E2 + (x * process_triangle.dy2) - (y * process_triangle.dx2);
    p.E3 = p.E3 + (x * process_triangle.dy3) - (y * process_triangle.dx3);
    p.x = p.x + x;
    p.y = p.y + y;
    
    if(p.E1>=0 && p.E2>=0 && p.E3>=0)
    {
        
        if(p.x<=process_triangle.x_max && p.x>=process_triangle.x_min && p.y<=process_triangle.y_max && p.y>=process_triangle.y_min)
        {
            p.z = (p.z + process_triangle.dz_x * x + process_triangle.dz_y * y);
            p.r = (p.r + process_triangle.dr_x * x + process_triangle.dr_y * y);
            p.g = (p.g + process_triangle.dg_x * x + process_triangle.dg_y * y);
            p.b = (p.b + process_triangle.db_x * x + process_triangle.db_y * y);
            p.a = (p.a + process_triangle.da_x * x + process_triangle.da_y * y);
            p.u = (p.u + process_triangle.du_x * x + process_triangle.du_y * y);
            p.v = (p.v + process_triangle.dv_x * x + process_triangle.dv_y * y);
            p.w = (p.w + process_triangle.dw_x * x + process_triangle.dw_y * y);

            if(p.r>255)     p.r = 255;
            else if(p.r<0)  p.r = 0;

            if(p.g>255)     p.g = 255;
            else if(p.g<0)  p.g = 0;

            if(p.b>255)     p.b = 255;
            else if(p.b<0)  p.b = 0;
        
            if(p.a>255)     p.a = 255;
            else if(p.a<0)  p.a = 0;
                            
            //cout<<"FRAG_PASS"<<endl;

            fragment(p);
            
            return 1;        
        }     
        else
        {
            //cout<<"FRAG_FAIL_IN"<<endl;
            return 0;   
        }      
    }
    else
    {
        //cout<<"FRAG_FAIL_OUT"<<endl;
        return 0;            
    }
    //cout<<"<PIXEL_TEST_END>"<<endl;
}


void FS::scan_conversion(struct render_param t_p)
{
    int i;
    struct render_param temp;
    float g1dx1,g1dx2,g2dx;
    float g1dy1,g1dy2,g2dy;
    float g1e1,g1e2,g2e;
    float g1e1t,g1e2t,g2et;
    float g1xt,g1yt,g2xt,g2yt;
    float g1x,g1y,g2x,g2y;
    int   lr_side;
    int   l_hit,r_hit;
       
    //cout<<"<SC_START>"<<endl;      
    
    if((process_triangle.dy1>=0 && process_triangle.dy2>=0)||(process_triangle.dy1<0 && process_triangle.dy2<0))
    {
    	  //cout<<"H1"<<endl;
        g1dx1 = process_triangle.dx1;
        g1dy1 = process_triangle.dy1;
        g1dx2 = process_triangle.dx2;
        g1dy2 = process_triangle.dy2;
        g2dx = process_triangle.dx3;
        g2dy = process_triangle.dy3;
        g1e1 = t_p.E1;
        g1e2 = t_p.E2;
        g2e = t_p.E3;
        g1x = t_p.x;
        g1y = t_p.y;
        g2x = t_p.x;
        g2y = t_p.y;
        if(process_triangle.dy1 >=0)
           lr_side = 1;
        else
           lr_side = 2;           
    }
    else if((process_triangle.dy2>=0 && process_triangle.dy3>=0)||(process_triangle.dy2<0 && process_triangle.dy3<0))
    {
    	  //cout<<"H2"<<endl;
        g1dx1 = process_triangle.dx2;
        g1dy1 = process_triangle.dy2;
        g1dx2 = process_triangle.dx3;
        g1dy2 = process_triangle.dy3;
        g2dx = process_triangle.dx1;
        g2dy = process_triangle.dy1;
        g1e1 = t_p.E2;
        g1e2 = t_p.E3;
        g2e = t_p.E1;
        g1x = t_p.x;
        g1y = t_p.y;
        g2x = t_p.x;
        g2y = t_p.y;
        if(process_triangle.dy2 >=0)
           lr_side = 1;
        else
           lr_side = 2;        
    }
    else if((process_triangle.dy1>=0 && process_triangle.dy3>=0)||(process_triangle.dy1<0 && process_triangle.dy3<0))
    {
        //cout<<"H3"<<endl;
        g1dx1 = process_triangle.dx1;
        g1dy1 = process_triangle.dy1;
        g1dx2 = process_triangle.dx3;
        g1dy2 = process_triangle.dy3;
        g2dx = process_triangle.dx2;
        g2dy = process_triangle.dy2;
        g1e1 = t_p.E1;
        g1e2 = t_p.E3;
        g2e = t_p.E2;
        g1x = t_p.x;
        g1y = t_p.y;
        g2x = t_p.x;
        g2y = t_p.y;
        if(process_triangle.dy1 >=0)
           lr_side = 1;
        else
           lr_side = 2;  
    }    

    #if 0
    cout<<"########################"<<endl;
    cout<<"g1dx1 = "<<g1dx1<<endl;
    cout<<"g1dy1 = "<<g1dy1<<endl;
    cout<<"g1dx2 = "<<g1dx2<<endl;
    cout<<"g1dy2 = "<<g1dy2<<endl;
    cout<<"g2dx = "<<g2dx<<endl;
    cout<<"g2dy = "<<g2dy<<endl;
    cout<<"g1e1 = "<<g1e1<<endl;
    cout<<"g1e2 = "<<g1e2<<endl;
    cout<<"g2e = "<<g2e<<endl;
    cout<<"g1x = "<<g1x<<endl;
    cout<<"g1y = "<<g1y<<endl;
    cout<<"g2x = "<<g2x<<endl;
    cout<<"g2y = "<<g2y<<endl;
    cout<<"lr_side = "<<lr_side<<endl;
    cout<<"########################"<<endl;    
    #endif

    while(1)
    {
        l_hit = 0;
        r_hit = 0;
        
        // left edge search
        if(lr_side==1) // group1 is left
        {
            if(g1e1>=0 && g1e2>=0)
            {
                while(1)
                {
                    g1xt = g1x - 1;
                    g1e1t = g1e1 - g1dy1;
                    g1e2t = g1e2 - g1dy2;                    
                    
                    if(g1xt<process_triangle.x_min || g1e1t<0 || g1e2t<0)
                    {
                        l_hit = 1;
                        break;
                    }
                    else
                    {
                        g1x = g1xt;
                        g1e1 = g1e1t;
                        g1e2 = g1e2t;
                        shift_param(&t_p,&t_p,-1,0);
                    }
                }
            }
            else
            {
                while(1)
                {
                    g1xt = g1x + 1;
                    g1e1t = g1e1 + g1dy1;
                    g1e2t = g1e2 + g1dy2;
                    if(g1xt<=process_triangle.x_max && (g1e1t>=0 && g1e2t>=0))
                    {
                        g1x = g1xt;
                        g1e1 = g1e1t;
                        g1e2 = g1e2t;
                        shift_param(&t_p,&t_p,1,0);
                        l_hit = 1;
                        break;
                    }
                    else if(g1xt>process_triangle.x_max)
                    {
                        l_hit = 0;
                        break;
                    }
                    else
                    {
                        g1x = g1xt;
                        g1e1 = g1e1t;
                        g1e2 = g1e2t;
                        shift_param(&t_p,&t_p,1,0);
                    }
                }
            }
        }
        else  //group2 is left
        {
            if(g2e>=0)
            {
                while(1)
                {
                    g2xt = g2x - 1;
                    g2et = g2e - g2dy;
                    if(g2xt<process_triangle.x_min || g2et<0)
                    {
                        l_hit = 1;
                        break;
                    }
                    else
                    {
                        g2x = g2xt;
                        g2e = g2et;
                        shift_param(&t_p,&t_p,-1,0);
                    }
                }
            }
            else
            {
                while(1)
                {
                    g2xt = g2x + 1;
                    g2et = g2e + g2dy;
                    if(g2xt<=process_triangle.x_max && g2et>=0)
                    {
                        g2x = g2xt;
                        g2e = g2et;
                        shift_param(&t_p,&t_p,1,0);
                        l_hit = 1;
                        break;
                    }
                    else if(g2xt>process_triangle.x_max)
                    {
                        l_hit = 0;
                        break;
                    }
                    else
                    {
                        g2x = g2xt;
                        g2e = g2et;
                        shift_param(&t_p,&t_p,1,0);
                    }
                }
            }
        }
        //end of left edge search       

        //right edge search
        if(lr_side==2) // group1 is right
        {
            if(g1e1>=0 && g1e2>=0)
            {
                while(1)
                {
                    g1xt = g1x + 1;
                    g1e1t = g1e1 + g1dy1;
                    g1e2t = g1e2 + g1dy2;
                    if(g1xt>process_triangle.x_max || g1e1t<0 || g1e2t <0)
                    {
                        r_hit = 1;
                        break;
                    }
                    else
                    {
                        g1x = g1xt;
                        g1e1 = g1e1t;
                        g1e2 = g1e2t;
                    }
                }
            }
            else
            {
                while(1)
                {
                    g1xt = g1x - 1;
                    g1e1t = g1e1 - g1dy1;
                    g1e2t = g1e2 - g1dy2;
                    if(g1xt>=process_triangle.x_min && (g1e1t>=0 && g1e2t>=0))
                    {
                        g1x = g1xt;
                        g1e1 = g1e1t;
                        g1e2 = g1e2t;
                        r_hit = 1;
                        break;
                    }
                    else if(g1xt<process_triangle.x_min)
                    {
                        r_hit = 0;
                        break;
                    }
                    else
                    {
                        g1x = g1xt;
                        g1e1 = g1e1t;
                        g1e2 = g1e2t;
                    }
                }
            }
        }
        else //group2 is right
        {
            if(g2e>=0)
            {
                while(1)
                {
                    g2xt = g2x + 1;
                    g2et = g2e + g2dy;
                    if(g2xt>process_triangle.x_max || g2et<0)
                    {
                        r_hit = 1;
                        break;
                    }
                    else
                    {
                        g2x = g2xt;
                        g2e = g2et;
                    }
                }
            }
            else
            {
                while(1)
                {
                    g2xt = g2x - 1;
                    g2et = g2e - g2dy;
                    if(g2xt>=process_triangle.x_min && g2et>=0)
                    {
                        g2x = g2xt;
                        g2e = g2et;
                        r_hit = 1;
                        break;
                    }
                    else if(g2xt<process_triangle.x_min)
                    {
                        r_hit = 0;
                        break;
                    }
                    else
                    {
                        g2x = g2xt;
                        g2e = g2et;
                    }
                }
            }
        }
        //end of right side search                
        
        #if 0
        cout<<"(lhit , rhit) = "<<l_hit<<','<<r_hit<<endl;
        //cout<<"(g1x , g2x) = "<<g1x<<','<<g2x<<endl;
        //cout<<"lr_side = "<<lr_side<<endl;
        #endif
        
        //scan out            
        if(l_hit == 1 && r_hit == 1)
        {
        	  //cout<<"two hit"<<endl;
            shift_param(&temp,&t_p,0,0);                        
            
            if(lr_side == 1)                      
            {
                for(i=g1x;i<=g2x;i+=2)
                {                	  
                	  //cout<<"GGGGG"<<endl;
                    pixel_test(temp,0,0);
                    pixel_test(temp,1,0);                                                            
                    shift_param(&temp,&temp,2,0);                   
                }
            }
            else
            {
                for(i=g2x;i<=g1x;i+=2)
                {
                    //cout<<"88888"<<endl;                	
                    pixel_test(temp,0,0);
                    pixel_test(temp,1,0);
                    shift_param(&temp,&temp,2,0);                   
                }
            }           	
        }
        // end of scanout;

        g1yt = g1y + 1;
        if(g1yt>process_triangle.y_max)
        {
        	  //cout<<"<SCAN_LINE_END>"<<endl;
            //RM_done.write(1);
            break;//outter loop break point
        }
        else
        {
            g1y = g1yt;
            g1e1 = g1e1 - g1dx1*1;
            g1e2 = g1e2 - g1dx2*1;
            g2e = g2e - g2dx*1;
            shift_param(&t_p,&t_p,0,1);
        }
    }
    //cout<<"<SC_END>"<<endl;
}


void FS::render_triangle(void) 
{   
    struct render_param t_p;
    sc_bv<64> IM_temp;
    int i,j;    
    float x1,y1,x2,y2;
    float c;
    float cw;
    float za,zb;
    float ra,rb;
    float ga,gb;
    float ba,bb;
    float aa,ab;
    float ua,ub;
    float va,vb;
    float wa,wb;
    float max,min;

    
    //cout<<"FS:wait data"<<endl;
    wait(flag_FS);
    while(1)
    {
        FRAG->info_num=0;//CYLin::debug
        //wait for <RM_start>
        //--------------------------------------
        //while(!RM_start.read());
        cout<<"FS:<TRI_START>"<<endl;        
            
        //set variables according to [SLAVE_MEM]
        //--------------------------------------
        //(1) [IM]
        for(i=0;i<(IM_LENGTH*2);i+=2)        
        {
            IM_temp = (SLAVE_MEM[i+1],SLAVE_MEM[i]);
            IM[i/2] = (sc_uint<64>)IM_temp;         
        }
        
        //(2) [CMEM]
        //for(i=IM_LENGTH*2;i<(IM_LENGTH*2+CMEM_LENGTH);i++)
        //   CMEM[i] = SLAVE_MEM[i]; 

        j=0; //CYLin::fix the bug
	for(i=IM_LENGTH*2;i<(IM_LENGTH*2+CMEM_LENGTH);i++)
        {   
	  //cout<<"for: i: "<<i<<endl<<"j: "<<j<<endl<<"SLAVE_MEM["<<i<<"]: "<<SLAVE_MEM[i]<<endl<<"CMEM["<<j<<"]: "<<CMEM[j]<<endl;
	    CMEM[j] = SLAVE_MEM[i];
            j++;        
        }
             
        //(3) [VERTEX FIFO]           
        for(i=0;i<48;i++)
        {
            VOB = sp_VOB.read();
            mylog<<"FIFO:\n"<<VERTEX_FIFO_BASE+i<<":  "<<VOB<<endl;   
            SLAVE_MEM[VERTEX_FIFO_BASE+i]  = *((unsigned int*)((void*)&VOB));
                     
        }
        mylog<<"================================"<<endl; 
        cout<<"FS: == Read VOB form FIFO =="<<endl;
        
        i = IM_LENGTH*2+CMEM_LENGTH;
        process_triangle.x1 = (int)(uint2float(SLAVE_MEM[i]) + 0.5);
        process_triangle.x2 = (int)(uint2float(SLAVE_MEM[i+16]) + 0.5);
        process_triangle.x3 = (int)(uint2float(SLAVE_MEM[i+32]) + 0.5);
        i++;

	//added by sheng-chih for debug
	FILE *pfile_out_vertex=fopen("out_vertex","a");
	fprintf(pfile_out_vertex,"X1= %f\tY1= %f\tZ1= %f\n",process_triangle.x1,process_triangle.y1,process_triangle.z1);
	fprintf(pfile_out_vertex,"X2= %f\tY2= %f\tZ2= %f\n",process_triangle.x2,process_triangle.y2,process_triangle.z2);
	fprintf(pfile_out_vertex,"X3= %f\tY3= %f\tZ3= %f\n",process_triangle.x3,process_triangle.y3,process_triangle.z3);

	//end added        
        #if 0
	cout<<"X1 ="<<process_triangle.x1<<endl;                 
        cout<<"X2 ="<<process_triangle.x2<<endl;
        cout<<"X3 ="<<process_triangle.x3<<endl;        
        #endif
        
        /*        
        process_triangle.y1 = LCD_HEIGHT - (int)(uint2float(SLAVE_MEM[i]) + 0.5);
        process_triangle.y2 = LCD_HEIGHT - (int)(uint2float(SLAVE_MEM[i+16]) + 0.5);
        process_triangle.y3 = LCD_HEIGHT - (int)(uint2float(SLAVE_MEM[i+32]) + 0.5);
        */
        process_triangle.y1 = (int)(uint2float(SLAVE_MEM[i]) + 0.5);
        process_triangle.y2 = (int)(uint2float(SLAVE_MEM[i+16]) + 0.5);
        process_triangle.y3 = (int)(uint2float(SLAVE_MEM[i+32]) + 0.5);        
        i++;
        
        #if 0 
        cout<<"Y1 ="<<process_triangle.y1<<endl;                 
        cout<<"Y2 ="<<process_triangle.y2<<endl;
        cout<<"Y3 ="<<process_triangle.y3<<endl;        
        #endif
        
        process_triangle.z1 = uint2float(SLAVE_MEM[i]) * 65536;
        process_triangle.z2 = uint2float(SLAVE_MEM[i+16]) * 65536;
        process_triangle.z3 = uint2float(SLAVE_MEM[i+32]) * 65536;
        i++;
        
        #if 0
        cout<<"Z1 ="<<process_triangle.z1<<endl;                 
        cout<<"Z2 ="<<process_triangle.z2<<endl;
        cout<<"Z3 ="<<process_triangle.z3<<endl;        
        #endif
        
        process_triangle.w1 = uint2float(SLAVE_MEM[i]);
        process_triangle.w2 = uint2float(SLAVE_MEM[i+16]);
        process_triangle.w3 = uint2float(SLAVE_MEM[i+32]);
        i+=5;
        
        #if 0
        cout<<"W1 ="<<process_triangle.w1<<endl;                 
        cout<<"W2 ="<<process_triangle.w2<<endl;
        cout<<"W3 ="<<process_triangle.w3<<endl;        
        #endif
        
        process_triangle.r1 = uint2float(SLAVE_MEM[i]);
        process_triangle.r2 = uint2float(SLAVE_MEM[i+16]);
        process_triangle.r3 = uint2float(SLAVE_MEM[i+32]);
        i++;
        
        #if 0
        cout<<"R1 ="<<process_triangle.r1<<endl;                 
        cout<<"R2 ="<<process_triangle.r2<<endl;
        cout<<"R3 ="<<process_triangle.r3<<endl;        
        #endif
        
        process_triangle.g1 = uint2float(SLAVE_MEM[i]);
        process_triangle.g2 = uint2float(SLAVE_MEM[i+16]);
        process_triangle.g3 = uint2float(SLAVE_MEM[i+32]);
        i++;
        
        #if 0
        cout<<"G1 ="<<process_triangle.g1<<endl;                 
        cout<<"G2 ="<<process_triangle.g2<<endl;
        cout<<"G3 ="<<process_triangle.g3<<endl;        
        #endif
        
        process_triangle.b1 = uint2float(SLAVE_MEM[i]);
        process_triangle.b2 = uint2float(SLAVE_MEM[i+16]);
        process_triangle.b3 = uint2float(SLAVE_MEM[i+32]);
        i++;
        
        #if 0
        cout<<"B1 ="<<process_triangle.b1<<endl;                 
        cout<<"B2 ="<<process_triangle.b2<<endl;
        cout<<"B3 ="<<process_triangle.b3<<endl;        
        #endif
        
        process_triangle.a1 = uint2float(SLAVE_MEM[i]);
        process_triangle.a2 = uint2float(SLAVE_MEM[i+16]);
        process_triangle.a3 = uint2float(SLAVE_MEM[i+32]);
	i-=7;
        //i++;
        
        #if 0
        cout<<"A1 ="<<process_triangle.a1<<endl;                 
        cout<<"A2 ="<<process_triangle.a2<<endl;
        cout<<"A3 ="<<process_triangle.a3<<endl;        
        #endif        
        process_triangle.u1 = uint2float(SLAVE_MEM[i]) * process_triangle.w1;
        process_triangle.u2 = uint2float(SLAVE_MEM[i+16]) * process_triangle.w2;
        process_triangle.u3 = uint2float(SLAVE_MEM[i+32]) * process_triangle.w3;
	i++;
        //i+=3;
        
        #if 0
        cout<<"U1 ="<<process_triangle.u1<<endl;                 
        cout<<"U2 ="<<process_triangle.u2<<endl;
        cout<<"U3 ="<<process_triangle.u3<<endl;        
        #endif
        process_triangle.v1 = uint2float(SLAVE_MEM[i]) * process_triangle.w1;
        process_triangle.v2 = uint2float(SLAVE_MEM[i+16]) * process_triangle.w2;
        process_triangle.v3 = uint2float(SLAVE_MEM[i+32]) * process_triangle.w3;
        
        #if 0    
        cout<<"V1 ="<<process_triangle.v1<<endl;                 
        cout<<"V2 ="<<process_triangle.v2<<endl;
        cout<<"V3 ="<<process_triangle.v3<<endl;  
        cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
        #endif
        
        //(4) [Frag. op. parameters]
        i = IM_LENGTH*2+CMEM_LENGTH+V_FIFO_LENGTH*4;
        FRAG->blending_dstfunc = SLAVE_MEM[i].range(3,0).to_uint();       
        FRAG->blending_srcfunc = SLAVE_MEM[i].range(7,4).to_uint(); 
        FRAG->blending_enable = SLAVE_MEM[i].bit(10).to_bool();
        FRAG->zcompare_enable = SLAVE_MEM[i].bit(11).to_bool();
        FRAG->stencil_enable = SLAVE_MEM[i].bit(12).to_bool();
        i++;
        FRAG->zpass = SLAVE_MEM[i].range(2,0).to_uint(); 
        FRAG->zfail = SLAVE_MEM[i].range(5,3).to_uint();
        FRAG->fail = SLAVE_MEM[i].range(8,6).to_uint();
        FRAG->zcompare_func = SLAVE_MEM[i].range(11,9).to_uint();
        FRAG->stencil_func = SLAVE_MEM[i].range(14,12).to_uint();
        FRAG->stencil_ref = SLAVE_MEM[i].range(22,15).to_uint();
        FRAG->stencil_mask = SLAVE_MEM[i].range(30,23).to_uint();
                	   	     

        c = process_triangle.x1*(process_triangle.y3 - process_triangle.y2) //-C
          + process_triangle.x2*(process_triangle.y1 - process_triangle.y3)
          + process_triangle.x3*(process_triangle.y2 - process_triangle.y1);   
          
        if(c==0)
        {
            cw = 0;
        }	
        else
        {
            cw = 1/c;
        }

	      if(cw!=0)
	      {		        
		        za = process_triangle.y1*(process_triangle.z2 - process_triangle.z3)
               + process_triangle.y2*(process_triangle.z3 - process_triangle.z1)
               + process_triangle.y3*(process_triangle.z1 - process_triangle.z2);
		        zb = process_triangle.x1*(process_triangle.z3 - process_triangle.z2)
               + process_triangle.x2*(process_triangle.z1 - process_triangle.z3)
               + process_triangle.x3*(process_triangle.z2 - process_triangle.z1);                  

            process_triangle.dz_x = za * cw;
            process_triangle.dz_y = zb * cw;                    
            
            #if 0 
            cout<<"Za ="<<za<<endl;                 
            cout<<"Zb ="<<zb<<endl;
            cout<<"Dz_x ="<<process_triangle.dz_x<<endl;                 
            cout<<"Dz_y ="<<process_triangle.dz_y<<endl;            
            #endif

		        ra = process_triangle.y1*(process_triangle.r2 - process_triangle.r3)
               + process_triangle.y2*(process_triangle.r3 - process_triangle.r1)
               + process_triangle.y3*(process_triangle.r1 - process_triangle.r2);
		        rb = process_triangle.x1*(process_triangle.r3 - process_triangle.r2)
               + process_triangle.x2*(process_triangle.r1 - process_triangle.r3)
               + process_triangle.x3*(process_triangle.r2 - process_triangle.r1);

            process_triangle.dr_x = ra * cw;
            process_triangle.dr_y = rb * cw;
            
            #if 0
            cout<<"Ra ="<<ra<<endl;                 
            cout<<"Rb ="<<rb<<endl;
            cout<<"Dr_x ="<<process_triangle.dr_x<<endl;                 
            cout<<"Dr_y ="<<process_triangle.dr_y<<endl;                        
            #endif

		        ga = process_triangle.y1*(process_triangle.g2 - process_triangle.g3)
               + process_triangle.y2*(process_triangle.g3 - process_triangle.g1)
               + process_triangle.y3*(process_triangle.g1 - process_triangle.g2);
		        gb = process_triangle.x1*(process_triangle.g3 - process_triangle.g2)
               + process_triangle.x2*(process_triangle.g1 - process_triangle.g3)
               + process_triangle.x3*(process_triangle.g2 - process_triangle.g1);

            process_triangle.dg_x = ga *cw;
            process_triangle.dg_y = gb *cw;
            
            #if 0
            cout<<"Ga ="<<ga<<endl;                 
            cout<<"Gb ="<<gb<<endl;
            cout<<"Dg_x ="<<process_triangle.dg_x<<endl;                 
            cout<<"Dg_y ="<<process_triangle.dg_y<<endl;            
            #endif
 
            ba = process_triangle.y1*(process_triangle.b2 - process_triangle.b3)
               + process_triangle.y2*(process_triangle.b3 - process_triangle.b1)
               + process_triangle.y3*(process_triangle.b1 - process_triangle.b2);
		        bb = process_triangle.x1*(process_triangle.b3 - process_triangle.b2)
               + process_triangle.x2*(process_triangle.b1 - process_triangle.b3)
               + process_triangle.x3*(process_triangle.b2 - process_triangle.b1);

            process_triangle.db_x = ba *cw;
            process_triangle.db_y = bb *cw;
 
            #if 0
            cout<<"Ba ="<<ba<<endl;                 
            cout<<"Bb ="<<bb<<endl;
            cout<<"Db_x ="<<process_triangle.db_x<<endl;                 
            cout<<"Db_y ="<<process_triangle.db_y<<endl;
            #endif
                            
            aa = process_triangle.y1*(process_triangle.a2 - process_triangle.a3)
               + process_triangle.y2*(process_triangle.a3 - process_triangle.a1)
               + process_triangle.y3*(process_triangle.a1 - process_triangle.a2);
		        ab = process_triangle.x1*(process_triangle.a3 - process_triangle.a2)
               + process_triangle.x2*(process_triangle.a1 - process_triangle.a3)
               + process_triangle.x3*(process_triangle.a2 - process_triangle.a1);

            process_triangle.da_x = aa *cw;
            process_triangle.da_y = ab *cw;

            #if 0
            cout<<"Aa ="<<aa<<endl;                 
            cout<<"Ab ="<<ab<<endl;
            cout<<"Da_x ="<<process_triangle.da_x<<endl;                 
            cout<<"Da_y ="<<process_triangle.da_y<<endl;
            #endif
 
            ua = process_triangle.y1*(process_triangle.u2 - process_triangle.u3)
               + process_triangle.y2*(process_triangle.u3 - process_triangle.u1)
               + process_triangle.y3*(process_triangle.u1 - process_triangle.u2);
		        ub = process_triangle.x1*(process_triangle.u3 - process_triangle.u2)
               + process_triangle.x2*(process_triangle.u1 - process_triangle.u3)
               + process_triangle.x3*(process_triangle.u2 - process_triangle.u1);

            process_triangle.du_x = ua *cw;
            process_triangle.du_y = ub *cw;

            #if 0
            cout<<"Ua ="<<ua<<endl;                 
            cout<<"Ub ="<<ub<<endl;
            cout<<"Du_x ="<<process_triangle.du_x<<endl;                 
            cout<<"Du_y ="<<process_triangle.du_y<<endl;
            #endif

            va = process_triangle.y1*(process_triangle.v2 - process_triangle.v3)
               + process_triangle.y2*(process_triangle.v3 - process_triangle.v1)
               + process_triangle.y3*(process_triangle.v1 - process_triangle.v2);
		        vb = process_triangle.x1*(process_triangle.v3 - process_triangle.v2)
               + process_triangle.x2*(process_triangle.v1 - process_triangle.v3)
               + process_triangle.x3*(process_triangle.v2 - process_triangle.v1);

            process_triangle.dv_x = va *cw;
            process_triangle.dv_y = vb *cw;
            
            #if 0 
            cout<<"Va ="<<va<<endl;                 
            cout<<"Vb ="<<vb<<endl;
            cout<<"Dv_x ="<<process_triangle.dv_x<<endl;                 
            cout<<"Dv_y ="<<process_triangle.dv_y<<endl;            
            #endif
 
            wa = process_triangle.y1*(process_triangle.w2 - process_triangle.w3)
               + process_triangle.y2*(process_triangle.w3 - process_triangle.w1)
               + process_triangle.y3*(process_triangle.w1 - process_triangle.w2);
		        wb = process_triangle.x1*(process_triangle.w3 - process_triangle.w2)
               + process_triangle.x2*(process_triangle.w1 - process_triangle.w3)
               + process_triangle.x3*(process_triangle.w2 - process_triangle.w1);

            process_triangle.dw_x = wa *cw;
            process_triangle.dw_y = wb *cw;
            
            #if 0 
            cout<<"Wa ="<<wa<<endl;                 
            cout<<"Wb ="<<wb<<endl;
            cout<<"Dw_x ="<<process_triangle.dw_x<<endl;                 
            cout<<"Dw_y ="<<process_triangle.dw_y<<endl;            
            #endif    
	      }
	      else
	      {
		        process_triangle.dz_x = 0;
		        process_triangle.dz_y = 0;
		        process_triangle.dr_x = 0;
		        process_triangle.dr_y = 0;
		        process_triangle.dg_x = 0;
		        process_triangle.dg_y = 0;
		        process_triangle.db_x = 0;
		        process_triangle.db_y = 0;
            process_triangle.da_x = 0;
		        process_triangle.da_y = 0;
            process_triangle.du_x = 0;
            process_triangle.du_y = 0;
            process_triangle.dv_x = 0;
            process_triangle.dv_y = 0;
            process_triangle.dw_x = 0;
            process_triangle.dw_y = 0;
	      }
	      

        process_triangle.dx1 = process_triangle.x2 - process_triangle.x1;
        process_triangle.dx2 = process_triangle.x3 - process_triangle.x2;
        process_triangle.dx3 = process_triangle.x1 - process_triangle.x3;
        process_triangle.dy1 = process_triangle.y2 - process_triangle.y1;
        process_triangle.dy2 = process_triangle.y3 - process_triangle.y2;
        process_triangle.dy3 = process_triangle.y1 - process_triangle.y3;

        #if 0
        cout<<"Dx1 ="<<process_triangle.dx1<<endl;                 
        cout<<"Dx2 ="<<process_triangle.dx2<<endl;
        cout<<"Dx3 ="<<process_triangle.dx3<<endl;                 
        cout<<"Dy1 ="<<process_triangle.dy1<<endl;
        cout<<"Dy2 ="<<process_triangle.dy2<<endl;
        cout<<"Dy3 ="<<process_triangle.dy3<<endl;
        #endif
 
        //x
        max = process_triangle.x1;
        min = process_triangle.x1;

        if(process_triangle.x2>max)
            max = process_triangle.x2;
        else if(process_triangle.x2<min)
            min = process_triangle.x2;
        if(process_triangle.x3>max)
            max = process_triangle.x3;
        else if(process_triangle.x3<min)
            min = process_triangle.x3;
        
        #if 0
        cout<<"X~~~(MAX,MIN) = "<<max<<','<<min<<endl;
        #endif
        
        process_triangle.x_max = max;
        process_triangle.x_min = min;

        //y
        max = process_triangle.y1;
        min = process_triangle.y1;

        if(process_triangle.y2>max)
           max = process_triangle.y2;
        else if(process_triangle.y2<min)
           min = process_triangle.y2;
        if(process_triangle.y3>max)
           max = process_triangle.y3;
        else if(process_triangle.y3<min)
           min = process_triangle.y3;

        #if 0
        cout<<"Y~~~(MAX,MIN) = "<<max<<','<<min<<endl;
        #endif
 
        process_triangle.y_max = max;
        process_triangle.y_min = min;


        x1 = 0;  //the leftest pixel of the tile
        y1 = 0;  //the toppest pixel of the tile 
        x2 = LCD_WIDTH -1; //the rightest pixel of the tile
        y2 = LCD_HEIGHT -1; //the downest pixel of the tile

        if(process_triangle.x_min > x1)
           x1 = process_triangle.x_min;
        else
           process_triangle.x_min = x1;
        //x1 -= x1%2;

        if(process_triangle.y_min > y1)
           y1 = process_triangle.y_min;
        else
           process_triangle.y_min = y1;
        //y1 -= y1%2;

        if(process_triangle.x_max < x2)
           x2 = process_triangle.x_max;
        else
           process_triangle.x_max = x2;

        if(process_triangle.y_max < y2)
           y2 = process_triangle.y_max;
        else
           process_triangle.y_max = y2;

        t_p.x = x1;
        t_p.y = y1;

        t_p.E1 = (t_p.x-process_triangle.x1)*process_triangle.dy1 -
                 (t_p.y-process_triangle.y1)*process_triangle.dx1;
                 
        #if 0
        cout<<"E1 information"<<endl;
        cout<<"-----------------"<<endl;
        cout<<"t_p.x = "<<t_p.x<<endl;
        cout<<"t_p.y = "<<t_p.y<<endl;
        cout<<"process_triangle.x1 = "<<process_triangle.x1<<endl;
        cout<<"process_triangle.y1 = "<<process_triangle.y1<<endl;    
        cout<<"process_triangle.dx1 = "<<process_triangle.dx1<<endl;
        cout<<"process_triangle.dy1 = "<<process_triangle.dy1<<endl;        
        #endif                 

        t_p.E2 = (t_p.x-process_triangle.x2)*process_triangle.dy2 -
                 (t_p.y-process_triangle.y2)*process_triangle.dx2;
                 
        #if 0
        cout<<"E2 information"<<endl;
        cout<<"-----------------"<<endl;
        cout<<"t_p.x = "<<t_p.x<<endl;
        cout<<"t_p.y = "<<t_p.y<<endl;
        cout<<"process_triangle.x2 = "<<process_triangle.x2<<endl;
        cout<<"process_triangle.y2 = "<<process_triangle.y2<<endl;    
        cout<<"process_triangle.dx2 = "<<process_triangle.dx2<<endl;
        cout<<"process_triangle.dy2 = "<<process_triangle.dy2<<endl;        
        #endif                     

        t_p.E3 = (t_p.x-process_triangle.x3)*process_triangle.dy3 -
                 (t_p.y-process_triangle.y3)*process_triangle.dx3;                 
        #if 0
        cout<<"E3 information"<<endl;
        cout<<"-----------------"<<endl;
        cout<<"t_p.x = "<<t_p.x<<endl;
        cout<<"t_p.y = "<<t_p.y<<endl;
        cout<<"process_triangle.x3 = "<<process_triangle.x3<<endl;
        cout<<"process_triangle.y3 = "<<process_triangle.y3<<endl;    
        cout<<"process_triangle.dx3 = "<<process_triangle.dx3<<endl;
        cout<<"process_triangle.dy3 = "<<process_triangle.dy3<<endl;        
        #endif                 
                 
        t_p.z =  (process_triangle.z1) + (t_p.x-process_triangle.x1)*process_triangle.dz_x +
                 (t_p.y-process_triangle.y1)*process_triangle.dz_y;
        t_p.r =  (process_triangle.r1) + (t_p.x-process_triangle.x1)*process_triangle.dr_x +
                 (t_p.y-process_triangle.y1)*process_triangle.dr_y;
        t_p.g =  (process_triangle.g1) + (t_p.x-process_triangle.x1)*process_triangle.dg_x +
                 (t_p.y-process_triangle.y1)*process_triangle.dg_y;
        t_p.b =  (process_triangle.b1) + (t_p.x-process_triangle.x1)*process_triangle.db_x +
                 (t_p.y-process_triangle.y1)*process_triangle.db_y;
        t_p.a =  (process_triangle.a1) + (t_p.x-process_triangle.x1)*process_triangle.da_x +
                 (t_p.y-process_triangle.y1)*process_triangle.da_y;
        t_p.u =  (process_triangle.u1) + (t_p.x-process_triangle.x1)*process_triangle.du_x +
                 (t_p.y-process_triangle.y1)*process_triangle.du_y;
        t_p.v =  (process_triangle.v1) + (t_p.x-process_triangle.x1)*process_triangle.dv_x +
                 (t_p.y-process_triangle.y1)*process_triangle.dv_y;
        t_p.w =  (process_triangle.w1) + (t_p.x-process_triangle.x1)*process_triangle.dw_x +
                 (t_p.y-process_triangle.y1)*process_triangle.dw_y;


        #if 0
        cout<<"t_P.E1 ="<<t_p.E1<<endl;                 
        cout<<"t_P.E2 ="<<t_p.E2<<endl;
        cout<<"t_P.E3 ="<<t_p.E3<<endl;                 
        cout<<"t_P.z ="<<t_p.z<<endl;
        cout<<"t_P.r ="<<t_p.r<<endl;
        cout<<"t_P.g ="<<t_p.g<<endl;
        cout<<"t_P.b ="<<t_p.b<<endl;
        cout<<"t_P.a ="<<t_p.a<<endl;
        cout<<"t_P.u ="<<t_p.u<<endl;
        cout<<"t_P.v ="<<t_p.v<<endl;                                       
        cout<<"t_P.w ="<<t_p.w<<endl;
        #endif        
                
        scan_conversion(t_p);
        
        cout<<"FS:<TRI_END>"<<endl;
        //end simulation
        if(sp_VS_END.read()==1 && sp_VOB.num_available()==0)
        {
            cout<<"FS:<System_END>"<<endl;
            sc_stop();
            wait();
        }
        /*access_IO=0;//CYL:currently non-used
        if(access_IO)
        {
		cout<<"FS.Initiator: Send Read data request"<<endl;
		//prepare write data
		
		//setup transfer parameter
		info_mp.Type     = 0; //0:read, 1:write
		info_mp.Addr     = 999;
		info_mp.DataDest = &databuff[0];
		info_mp.Burst    = 6;
		mp_transfer.write(info_mp);
		
		//wait Target port response
		cout<<"FS.Initiator: wait response"<<endl;
		//receive readed data
		if(mp_response.read()==1 && info_mp.Type == 0)
		{
		cout<<"FS.Initiator: read Data = ";
		for(i=0;i<(int)info_mp.Burst;i++)
		   cout<<databuff[i]<<" ";
		cout<<endl;
		
		}
		else if(mp_response.read()==1)
		cout<<"FS.Initiator: write data ok"<<endl;
		else
		cout<<"FS.Initiator: transaction err"<<endl;
        }*/
        
    }      
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void FETCH::behavior(void)
{
    bool         FETCH_start;
    unsigned int branch_target;
    bool         branch_hit,finish;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    unsigned int PC_temp;

    while(1)    
    { 	
        //spin lock
    	wait(1,SC_NS);
        if(!is_FETCH_full)
        {
            do
            {
                FETCH_start = f_FETCH_start_in.read(); 
            }
            while(!FETCH_start);
            
            is_FETCH_full = 1;                                                 
        }    
        //cout<<"<IF start>"<<endl;
        
        //read from [ID] , [WB]
        branch_target = f_branch_target_in.read();
        branch_hit = f_branch_hit_in.read();
        finish = f_finish_in.read();           

        if(is_FETCH_full)
        {
              	
            //send addr to [IM]
            if(branch_hit)
		PC_temp = branch_target;
            else
		PC_temp = PC;
            //write to [ID]
	    //added by sheng-chih
	    //cout<<"PC: "<<PC<<"\tPC_temp: "<<PC_temp<<endl;
	    //cout<<"IM_IF["<<PC_temp<<"]: "<<IM_IF[PC_temp]<<endl;
	    //end added
            f_PC_out_ID.write(PC_temp);
            f_IM_data_out.write(IM_IF[PC_temp]);

            //update PC
	    //fixed by sheng-chih
            //if(!finish)
               PC = PC_temp + 1; 
	    //end fixed
        }
        else
        {
            //write to [ID]
            f_PC_out_ID.write(0);
            f_IM_data_out.write("0xf800000000000000");//NOP	
        }                        
        
        //cout<<"<IF end>"<<endl;
    }    
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


float DECODE::uint2float(sc_bv<32> input)
{
    int s_convt , exp_convt , fraction_convt;    
    float answer;	
    
    s_convt = input[31].to_bool();
    exp_convt = input.range(30,23).to_uint();
    fraction_convt = input.range(22,0).to_uint();

    if(s_convt == 0 && exp_convt ==0 && fraction_convt ==0)//0-detector
       return 0;    
    
    fraction_convt |= 0x00800000;
    
    answer = (float) fraction_convt / pow(2.0,23.0);
    answer *= pow(2.0,(double)(exp_convt-127));
    if(s_convt)
       answer *= -1;
            
    return answer;    
}


void DECODE::IR_ctrl(sc_uint<64> IM_data)
{
  if(opcode==14)
    {
      now_c=clock();
      if(clock_start==0)
	{
	  printf("start!!\n");
	  clock_start=1;
	  previous_c=now_c;
	}
      printf("The time diff is %f s\n",(double)(now_c-previous_c)/CLOCKS_PER_SEC);
      total_c+=(now_c-previous_c);
      printf("Total time: %f s\n",(double)total_c/CLOCKS_PER_SEC);
      previous_c=now_c;
      printf("Execute %d times\n",exe_count++);
    }
    opcode = IM_data.range(63,59);

    //added by sheng-chih
    //cout<<"IR_ctrl: opcode: "<<opcode<<"\t"<<IM_data.range(58,53)<<"\t"<<IM_data.range(52,47)<<endl;
    //cout<<"opcode: "<<IM_data.range(63,0)<<endl;
    //end added

    if(opcode.to_uint() >=24 && opcode.to_uint() <= 29)//BEQ,BNEQ,BLT,BGE,RET,CAL
    {
        dst = 0;
        src0 = IM_data.range(58,53);
        src1 = IM_data.range(52,47);
        src2 = 0;
        swizzle[0] = "00011011";
        swizzle[1] = "00011011";
        swizzle[2] = "00011011";
        neg[0] = 0;
        neg[1] = 0;
        neg[2] = 0;
	//added by sheng-chih
	//temp_mask="1111";
	//end added
        w_mask = "1111";
        RL = 0;
        FXL = IM_data[9];
        SV = IM_data.range(8,7);
        imm_value = 0;
        sa = 0;
        branch_addr = IM_data.range(46,31);
    }
    else if(opcode.to_uint() >=8 && opcode.to_uint() <= 10)//LD,ST,MVI
    {
        dst = IM_data.range(58,53);
        src0 = IM_data.range(52,47);
        src1 = 0;
        src2 = IM_data.range(58,53);
        swizzle[0] = "00011011";
        swizzle[1] = "00000000";
        swizzle[2] = "00000000";
        neg[0] = 0;
        neg[1] = 0;
        neg[2] = 0;
	//added by sheng-chih
	//temp_mask= IM_data.range(13,10);
	//end added
        w_mask = IM_data.range(13,10);
        RL = 0;
        FXL = IM_data[9];
        SV = IM_data.range(8,7);
        imm_value = IM_data.range(46,15);
        sa = 0;
        branch_addr = 0;
    }
    else if(opcode.to_uint() == 30)//SHRL
    {
        dst = IM_data.range(58,53);
        src0 = IM_data.range(52,47);
        src1 = 0;
        src2 = IM_data.range(58,53);
        swizzle[0] = IM_data.range(40,33);
        swizzle[1] = IM_data.range(32,25);
        swizzle[2] = IM_data.range(24,17);
        neg[0] = IM_data[16];
        neg[1] = IM_data[15];
        neg[2] = IM_data[14];
	//added by sheng-chih
	//temp_mask= IM_data.range(13,10);
	//end added
        w_mask = IM_data.range(13,10);
        RL = IM_data[41];
        FXL = IM_data[9];
        SV = IM_data.range(8,7);
        imm_value = 0;
        sa = IM_data.range(46,42);
        branch_addr = 0;
    }
    else
    {
        dst = IM_data.range(58,53);
        src0 = IM_data.range(52,47);
        src1 = IM_data.range(46,41);
        src2 = IM_data.range(58,53);        
        swizzle[0] = IM_data.range(40,33);
        swizzle[1] = IM_data.range(32,25);
        swizzle[2] = IM_data.range(24,17);
        neg[0] = IM_data[16];
        neg[1] = IM_data[15];
        neg[2] = IM_data[14];
	//added by sheng-chih
	//temp_mask= IM_data.range(13,10);
	//end added
        w_mask = IM_data.range(13,10);
        RL = 0;
        FXL = IM_data[9];
        SV = IM_data.range(8,7);
        imm_value = 0;
        sa = 0;
        branch_addr = 0;
    }
}


void DECODE::SWZ(sc_bv<128> *SWZ_data)
{ 	
	  int i;
	  sc_bv<32> X,Y,Z,W;
	  sc_bv<32> out_X,out_Y,out_Z,out_W;
	  
    for(i=0;i<3;++i) 
    {
      //added by sheng-chih
      if(i==2&&opcode!=18)
	break;
      //end added
        X = SWZ_data[i].range(127,96);	
        Y = SWZ_data[i].range(95,64);
        Z = SWZ_data[i].range(63,32);
        W = SWZ_data[i].range(31,0);
	//added by sheng-chih
	//cout<<"SWZ_data["<<i<<"][X]: "<<SWZ_data[i].range(127,96)<<endl
	//    <<"SWZ_data["<<i<<"][Y]: "<<SWZ_data[i].range(95,64)<<endl
	//    <<"SWZ_data["<<i<<"][Z]: "<<SWZ_data[i].range(63,32)<<endl
	//    <<"SWZ_data["<<i<<"][W]: "<<SWZ_data[i].range(31,0)<<endl;
        //end added
        if(neg[i])
        {
            X[31] = ~X[31];	
            Y[31] = ~Y[31];
            Z[31] = ~Z[31];
            W[31] = ~W[31];
        }
        
        //X swizzle
        switch((swizzle[i].range(7,6)).to_uint())
        {
            case 0:	 out_X = X; break;
            case 1:  out_X = Y; break;
            case 2:  out_X = Z; break;
            default: out_X = W; break;
        }
        
        //Y swizzle
        switch((swizzle[i].range(5,4)).to_uint())
        {
            case 0:	 out_Y = X; break;
            case 1:  out_Y = Y; break;
            case 2:  out_Y = Z; break;
            default: out_Y = W; break;
        }
        
        //Z swizzle
        switch((swizzle[i].range(3,2)).to_uint())
        {
            case 0:	 out_Z = X; break;
            case 1:  out_Z = Y; break;
            case 2:  out_Z = Z; break;
            default: out_Z = W; break;
        }
        
        //W swizzle
        switch((swizzle[i].range(1,0)).to_uint())
        {
            case 0:	 out_W = X; break;
            case 1:  out_W = Y; break;
            case 2:  out_W = Z; break;
            default: out_W = W; break;
        }      
        
        SWZ_data[i] = (out_X,out_Y,out_Z,out_W);                  
	//added by sheng-chih
	//cout<<"SWZ_data["<<i<<"][X]: "<<SWZ_data[i].range(127,96)<<endl
	//    <<"SWZ_data["<<i<<"][Y]: "<<SWZ_data[i].range(95,64)<<endl
	//    <<"SWZ_data["<<i<<"][Z]: "<<SWZ_data[i].range(63,32)<<endl
	//    <<"SWZ_data["<<i<<"][W]: "<<SWZ_data[i].range(31,0)<<endl;    	
	//end added
    }    	  
}


void DECODE::IR_output_ctrl(sc_bv<128> R_data0,sc_bv<128> R_data1,sc_bv<128> R_data2)
{
  //fix the opcode for LD by sheng-chih
  int temp_SV=SV;
  if(opcode==8)
    {
      SV=3;
    }
  //end fix
    switch(SV)
    {
        case 0: X0 = R_data0.range(127,96);
        	      Y0 = 0;
        	      Z0 = 0;
        	      W0 = 0;
        	      X1 = R_data1.range(127,96);
        	      Y1 = 0;
        	      Z1 = 0;
        	      W1 = 0;
        	      X2 = R_data2.range(127,96);
        	      Y2 = 0;
        	      Z2 = 0;
        	      W2 = 0;
        	      break;
        case 1: X0 = R_data0.range(127,96);
        	      Y0 = R_data0.range(95,64);
        	      Z0 = 0;
        	      W0 = 0;
        	      X1 = R_data1.range(127,96);
        	      Y1 = R_data1.range(95,64);
        	      Z1 = 0;
        	      W1 = 0;
        	      X2 = R_data2.range(127,96);
        	      Y2 = R_data2.range(95,64);
        	      Z2 = 0;
        	      W2 = 0;
        	      break;
        case 2: X0 = R_data0.range(127,96);
        	      Y0 = R_data0.range(95,64);
        	      Z0 = R_data0.range(63,32);
        	      W0 = 0;
        	      X1 = R_data1.range(127,96);
        	      Y1 = R_data1.range(95,64);
        	      Z1 = R_data1.range(63,32);
        	      W1 = 0;
        	      X2 = R_data2.range(127,96);
        	      Y2 = R_data2.range(95,64);
        	      Z2 = R_data2.range(63,32);
        	      W2 = 0;
        	      break;
        case 3: X0 = R_data0.range(127,96);
        	      Y0 = R_data0.range(95,64);
        	      Z0 = R_data0.range(63,32);
        	      W0 = R_data0.range(31,0);
        	      X1 = R_data1.range(127,96);
        	      Y1 = R_data1.range(95,64);
        	      Z1 = R_data1.range(63,32);
        	      W1 = R_data1.range(31,0);
        	      X2 = R_data2.range(127,96);
        	      Y2 = R_data2.range(95,64);
        	      Z2 = R_data2.range(63,32);
        	      W2 = R_data2.range(31,0);
        	      break;
        default: X0 = 0;
        	       Y0 = 0;
        	       Z0 = 0;
        	       W0 = 0;
        	       X1 = 0;
        	       Y1 = 0;
        	       Z1 = 0;
        	       W1 = 0;
        	       X2 = 0;
        	       Y2 = 0;
        	       Z2 = 0;
        	       W2 = 0;
        	       break;
    }
  //fix the opcode for LD by sheng-chih
  if(opcode==8)
    {
      SV=temp_SV;
    }
  //end fix
  //added by sheng-chih
  /*
  cout<<"DECODE:"<<endl
      <<"X0: "<<X0<<endl
      <<"Y0: "<<Y0<<endl
      <<"Z0: "<<Z0<<endl
      <<"W0: "<<W0<<endl
      <<"X1: "<<X1<<endl
      <<"Y1: "<<Y1<<endl
      <<"Z1: "<<Z1<<endl
      <<"W1: "<<W1<<endl
      <<"X2: "<<X2<<endl
      <<"Y2: "<<Y2<<endl
      <<"Z2: "<<Z2<<endl
      <<"W2: "<<W2<<endl;
  */


  //end added
}


bool DECODE::BLT(void)
{
    int counter=0;

    if(FXL)
    {
        if(uint2float(X0) < uint2float(X1))
           counter++;
        if(uint2float(Y0) < uint2float(Y1))
           counter++;
        if(uint2float(Z0) < uint2float(Z1))
           counter++;
        if(uint2float(W0) < uint2float(W1))
           counter++;
    }
    else
    {
        if(X0.to_int() < X1.to_int())
           counter++;
        if(Y0.to_int() < Y1.to_int())
           counter++;
        if(Z0.to_int() < Z1.to_int())
           counter++;
        if(W0.to_int() < W1.to_int())
           counter++;
    }
    if(counter == 4)
       return 1;
    else
       return 0;
}


bool DECODE::BGE(void)
{
    int counter=0;

    if(FXL)
    {
        if(uint2float(X0) >= uint2float(X1))
           counter++;
        if(uint2float(Y0) >= uint2float(Y1))
           counter++;
        if(uint2float(Z0) >= uint2float(Z1))
           counter++;
        if(uint2float(W0) >= uint2float(W1))
           counter++;
    }
    else
    {
        if(X0.to_int() >= X1.to_int())
           counter++;
        if(Y0.to_int() >= Y1.to_int())
           counter++;
        if(Z0.to_int() >= Z1.to_int())
           counter++;
        if(W0.to_int() >= W1.to_int())
           counter++;
    }
    if(counter == 4)
       return 1;
    else
       return 0;
}


void DECODE::behavior(void)
{
    bool         DECODE_start;
    sc_uint<64>  IM_data;
    unsigned int PC_in;
    unsigned int TEX_offset;    
    sc_bv<128>   SWZ_data[3];
    sc_int<32>   branch_target;

    //added by sheng-chih
    int i;
    //end added
    while(1)
    {            
        
        if(!is_DECODE_full)//write to [IF] ([ID] is empty)
        {      	  
            //CYLin
            do
            {
                DECODE_start = d_DECODE_start_in.read();
            }
            while(!DECODE_start);
            
            d_branch_hit_out.write(0);
            d_branch_target_out.write(0);
            is_DECODE_full = 1;
        }
        //cout<<"<ID start>"<<endl;
        
        IM_data = d_IM_data_in.read();        
        PC_in = d_PC_in.read();                	 
	/*
	if(i<temp_INS_NUM+1)
	  {
	    IM_data=temp_SLAVE_MEM[i];
	    i++;
	  }
	*/
	//cout<<"i: "<<i<<endl;
	//cout<<"IM_data: "<<IM_data<<endl;
	//cout<<"temp_INS_NUM: "<<temp_INS_NUM<<endl;
        //decode instruction
        IR_ctrl(IM_data);               

        //access [GPR]
	//added by sheng-chih
	//cout<<"src0: "<<src0<<endl
	//    <<"src1: "<<src1<<endl
	//    <<"src2: "<<src2<<endl;
	//end added
        SWZ_data[0] = GPR_ID[src0];
        SWZ_data[1] = GPR_ID[src1];
        SWZ_data[2] = GPR_ID[src2];
	//added by sheng-chih
	/*
	printf("GPR_ID: %d\n",GPR_ID);
	cout<<"GPR_ID["<<src0<<"][X]: "<<GPR_ID[src0].range(127,96).to_uint()<<endl;
	cout<<"GPR_ID["<<src0<<"][Y]: "<<GPR_ID[src0].range(95,64).to_uint()<<endl;
	cout<<"GPR_ID["<<src0<<"][Z]: "<<GPR_ID[src0].range(63,32).to_uint()<<endl;
	cout<<"GPR_ID["<<src0<<"][W]: "<<GPR_ID[src0].range(31,0).to_uint()<<endl;
	cout<<"GPR_ID["<<src1<<"][X]: "<<GPR_ID[src1].range(127,96).to_uint()<<endl;
	cout<<"GPR_ID["<<src1<<"][Y]: "<<GPR_ID[src1].range(95,64).to_uint()<<endl;
	cout<<"GPR_ID["<<src1<<"][Z]: "<<GPR_ID[src1].range(63,32).to_uint()<<endl;
	cout<<"GPR_ID["<<src1<<"][W]: "<<GPR_ID[src1].range(31,0).to_uint()<<endl;
	cout<<"GPR_ID["<<src2<<"][X]: "<<GPR_ID[src2].range(127,96).to_uint()<<endl;
	cout<<"GPR_ID["<<src2<<"][Y]: "<<GPR_ID[src2].range(95,64).to_uint()<<endl;
	cout<<"GPR_ID["<<src2<<"][Z]: "<<GPR_ID[src2].range(63,32).to_uint()<<endl;
	cout<<"GPR_ID["<<src2<<"][W]: "<<GPR_ID[src2].range(31,0).to_uint()<<endl;
	*/
        //end added
        //swizzle & neg
        SWZ(SWZ_data);        
        
        //TEX offset(t * 64 + s)                      
        TEX_offset = (unsigned int)(uint2float(SWZ_data[0].range(63,32)) + uint2float(SWZ_data[0].range(31,0)) * TEX_WIDTH);
	if(opcode==15)
	  {
	    cout<<"Final s: "<<uint2float(SWZ_data[0].range(63,32))<<"\nFinal t: "<<uint2float(SWZ_data[0].range(31,0))<<endl<<"TEX_offset: "<<TEX_offset<<endl;
	    printf("copy_TEX_MEM[%d]: %x\n",TEX_offset,copy_TEX_MEM[TEX_offset]);
	  }

        //print info.
        #if DEBUG_MODE
        
        cout<<"***********[DECODE]***********"<<endl;        
        cout<<"R_data0 = ("<<hex<<(SWZ_data[0].range(127,96)).to_uint()<<',';
        cout<<(SWZ_data[0].range(95,64)).to_uint()<<',';
        cout<<(SWZ_data[0].range(63,32)).to_uint()<<',';
        cout<<(SWZ_data[0].range(31,0)).to_uint()<<')'<<endl;
        
        cout<<"R_data1 = ("<<hex<<(SWZ_data[1].range(127,96)).to_uint()<<',';
        cout<<(SWZ_data[1].range(95,64)).to_uint()<<',';
        cout<<(SWZ_data[1].range(63,32)).to_uint()<<',';
        cout<<(SWZ_data[1].range(31,0)).to_uint()<<')'<<endl;        
        
        cout<<"R_data2 = ("<<hex<<(SWZ_data[2].range(127,96)).to_uint()<<',';
        cout<<(SWZ_data[2].range(95,64)).to_uint()<<',';
        cout<<(SWZ_data[2].range(63,32)).to_uint()<<',';
        cout<<(SWZ_data[2].range(31,0)).to_uint()<<')'<<endl;  
        cout<<"//////////////////////////////"<<endl;       
        
        #endif

        //vector selector        
        IR_output_ctrl(SWZ_data[0],SWZ_data[1],SWZ_data[2]);               

        //write to [IF] (ID is full)
        if(is_DECODE_full)
        {
            switch(opcode)
            {
                case 26: d_branch_hit_out.write(BLT());
            	           branch_target = (sc_int<32>)PC_in + (sc_int<32>)branch_addr;
            	           d_branch_target_out.write(branch_target);
            	           break;
                case 27: d_branch_hit_out.write(BGE());
            	           branch_target = (sc_int<32>)PC_in + (sc_int<32>)branch_addr;
            	           d_branch_target_out.write(branch_target);
            	           break;
                case 28: d_branch_hit_out.write(1);//RET
            	           d_branch_target_out.write(X0.to_int());
            	           break;
                case 29: d_branch_hit_out.write(1);//JUMP
            	           branch_target = (sc_int<32>)X0 + (sc_int<32>)(branch_addr<<3);
            	           d_branch_target_out.write(branch_target);
            	           break;
                default: d_branch_hit_out.write(0);
            	           d_branch_target_out.write(0);
            	           break;
            }
        }

        //cout<<"       Dec::opcode= "<< opcode<<endl;//CYLin
        //write to [EX]
        d_opcode_out.write(opcode);
        d_dst_out.write(dst);
        d_w_mask_out.write(w_mask);
        d_RL_out.write(RL);
        d_FXL_out.write(FXL);
        d_sa_out.write(sa);
        d_imm_value_out.write(imm_value);
        d_old_DST_out.write(SWZ_data[2]);
        d_X0_out.write(X0);
        d_Y0_out.write(Y0);
        d_Z0_out.write(Z0);
        d_W0_out.write(W0);
        d_X1_out.write(X1);
        d_Y1_out.write(Y1);
        d_Z1_out.write(Z1);
        d_W1_out.write(W1);
        d_X2_out.write(X2);
        d_Y2_out.write(Y2);
        d_Z2_out.write(Z2);
        d_W2_out.write(W2);
        d_TEX_offset_out.write(TEX_offset);
        
        //cout<<"<ID end>"<<endl;
    }
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
sc_bv<32> EXE::float2bv(float input)
{
    sc_bv<32>    return_data;
    //--------------------
    sc_uint<32>  temp_data;
    int          sign,exp;
    float        fraction;	

    fraction = frexp(input,&exp);

    if(fraction == 0)//0-detector    
       return 0x00000000;    
    
    fraction*=2;
    --exp;

    //sign
    if(fraction>0)
       sign = 0;
    else
       sign = 1;
       
    //exp
    exp += 127;

    //mantissa
    fraction = (fraction-1) * pow(2.0,23.0);//shift 23 bits
    
    //return data
    temp_data = (sign<<31) + (exp <<23) + (unsigned int)fraction;
    return_data = temp_data.range(31,0);

    return return_data;
}


float EXE::uint2float(sc_bv<32> input)
{
    int s_convt , exp_convt , fraction_convt;    
    float answer;	

    s_convt = input[31].to_bool();
    exp_convt = input.range(30,23).to_uint();
    fraction_convt = input.range(22,0).to_uint();
    
    if(s_convt == 0 && exp_convt ==0 && fraction_convt ==0)//0-detector
       return 0;
    
    fraction_convt |= 0x00800000;
    
    answer = (float) fraction_convt / pow(2.0,23.0);
    answer *= pow(2.0,(double)(exp_convt-127));
    if(s_convt)
       answer *= -1;
            
    return answer;    
}


void EXE::MOV(void)
{
    RLT_X = X0;
    RLT_Y = Y0;
    RLT_Z = Z0;
    RLT_W =	W0;
}


void EXE::MAXx(void)
{
    if(FXL)
    {
        if(uint2float(X0) > uint2float(X1))
           RLT_X = X0;
        else
           RLT_X = X1;
        if(uint2float(Y0) > uint2float(Y1))
           RLT_Y = Y0;
        else
           RLT_Y = Y1;
        if(uint2float(Z0) > uint2float(Z1))
           RLT_Z = Z0;
        else
           RLT_Z = Z1;
        if(uint2float(W0) > uint2float(W1))
           RLT_W = W0;
        else
           RLT_W = W1;
    }
    else
    {
        if(X0.to_int() > X1.to_int())
           RLT_X = X0;
        else
           RLT_X = X1;
        if(Y0.to_int() > Y1.to_int())
           RLT_Y = Y0;
        else
           RLT_Y = Y1;
        if(Z0.to_int() > Z1.to_int())
           RLT_Z = Z0;
        else
           RLT_Z = Z1;
        if(W0.to_int() > W1.to_int())
           RLT_W = W0;
        else
           RLT_W = W1;
  	}
}


void EXE::MINx(void)
{
    if(FXL)
    {
        if(uint2float(X0) < uint2float(X1))
           RLT_X = X0;
        else
           RLT_X = X1;
        if(uint2float(Y0) < uint2float(Y1))
           RLT_Y = Y0;
        else
           RLT_Y = Y1;
        if(uint2float(Z0) < uint2float(Z1))
           RLT_Z = Z0;
        else
           RLT_Z = Z1;
        if(uint2float(W0) < uint2float(W1))
           RLT_W = W0;
        else
           RLT_W = W1;
    }
    else
    {
        if(X0.to_int() < X1.to_int())
           RLT_X = X0;
        else
           RLT_X = X1;
        if(Y0.to_int() < Y1.to_int())
           RLT_Y = Y0;
        else
           RLT_Y = Y1;
        if(Z0.to_int() < Z1.to_int())
           RLT_Z = Z0;
        else
           RLT_Z = Z1;
        if(W0.to_int() < W1.to_int())
           RLT_W = W0;
        else
           RLT_W = W1;
  	}
}


void EXE::AND(void)
{
    RLT_X = X0 & X1;
    RLT_Y = Y0 & Y1;
    RLT_Z = Z0 & Z1;
    RLT_W = W0 & W1;
}


void EXE::OR(void)
{
    RLT_X = X0 | X1;
    RLT_Y = Y0 | Y1;
    RLT_Z = Z0 | Z1;
    RLT_W = W0 | W1;
}


void EXE::XOR(void)
{
    RLT_X = X0 ^ X1;
    RLT_Y = Y0 ^ Y1;
    RLT_Z = Z0 ^ Z1;
    RLT_W = W0 ^ W1;
}


void EXE::SHRL(void)
{
	  if(RL)
	  {
        RLT_X = X0<<sa;
        RLT_Y = Y0<<sa;
        RLT_Z = Z0<<sa;
        RLT_W = W0<<sa;
    }
    else
    {
        RLT_X = X0>>sa;
        RLT_Y = Y0>>sa;
        RLT_Z = Z0>>sa;
        RLT_W = W0>>sa;
    }
}


void EXE::MVI(void)
{
    RLT_X = imm_value;
    RLT_Y = imm_value;
    RLT_Z = imm_value;
    RLT_W = imm_value;
}


int EXE::RCP_factor(void)
{
	  int exp;
	  double fraction;
    double msb,lsb;

    fraction = frexp(uint2float(X0),&exp);
    fraction *=2;
    exp--;
    lsb = modf(fraction*4096,&msb);
    msb /= 4096;
    lsb /= 4096;
    a0 = 1/msb;
    a1 = -1 / pow(msb , 2.0);
    Y_rcp = (float)lsb;

    return (-exp);
}


int EXE::RSQ_factor(void)
{
	int exp;
	double fraction;
    double msb,lsb;

    fraction = frexp(uint2float(X0),&exp);
    if(exp%2 != 0)
    {
        fraction*=2;
        exp--;
    }
    lsb = modf(fraction*4096,&msb);
    msb /= 4096;
    lsb /= 4096;
    b0 = 1/sqrt(msb);
    b1 = -0.5 * pow(msb , 1.5);
    Y_rsq = (float)lsb;

    return (-exp/2);
}


int EXE::POW2_factor(void)
{
	double exp;
	double fraction;
    double msb,lsb;

    fraction = modf(uint2float(X0),&exp);//X0 = exp.fraction
    lsb = modf(fraction*4096,&msb);
    msb /= 4096;
    lsb /= 4096;
    c0 = pow(2.0,msb);
    c1 = log(2) * (c0);
    Y_pow2 = (float)lsb;

    return (int)exp;
}


int EXE::LOG2_factor(void)
{
	  int    exp;
	  double fraction;
    double msb,lsb;

    fraction = frexp(uint2float(X0),&exp);
    fraction *=2;
    exp--;
    lsb = modf(fraction*4096,&msb);
    msb /= 4096;
    lsb /= 4096;
    /*
    d0 = 1/msb + exp;//add EXP
    d1 = -1 / pow(msb , 2.0);
    */
    d0 = (log(msb)/log(2.0)) + exp;//add EXP
    d1 = 1 / ( log(2.0) * msb);    
    Y_log2 = (float)lsb;

    return 0;//2^0=1
}


void EXE::MAD_factor(void)
{
	  int i;
	  int EXP_diff;
	  int EXP1,EXP2,EXP3;
	  sc_bv<32> SRC1,SRC2,SRC3;
	  sc_bv<32> temp;
	  float fraction1,fraction2,fraction3;


	  for(i=0 ; i<4 ; ++i)
	  {
	      switch(opcode)
	      {
	      	  //DP3
	      	  case 6: if(i<3)
	      	  	      {
	      	  	         SRC1 = MAD_input[i]; SRC2 = MAD_input[i+4]; SRC3 = "0x00000000";
	      	  	      }
	      	  	      else
	      	  	      {
	      	  	         SRC1 = "0x00000000"; SRC2 = "0x00000000"; SRC3 = "0x00000000";
	      	  	      }
	      	  	      break;
	      	  //TXB
	  	      case 13: if(i<3)
	  	      	       {
	  	      	           SRC1 = float2bv(TEX[i]); SRC2 = "0x3f800000"; SRC3 = float2bv(TEX[3]);
	  	      	       }
	  	      	       else
	  	      	       {
	  	      	       	   SRC1 = float2bv(TEX[i]); SRC2 = "0x40000000"; SRC3 = float2bv(TEX[3]);
	  	      	       }
	  	      	       break;
	  	      //ADD
	          case 16: SRC1 = MAD_input[i]; SRC2 = "0x3f800000"; SRC3 = MAD_input[i+4];                               	                            
	          	       break;
	          //MUL
	          case 17: SRC1 = MAD_input[i]; SRC2 = MAD_input[i+4]; SRC3 = "0x00000000";
	          	       break;
	          //MAD
	          case 18: SRC1 = MAD_input[i]; SRC2 = MAD_input[i+4]; SRC3 = MAD_input[i+8];
	      	           break;
	      	  //DP4
	      	  case 19: SRC1 = MAD_input[i]; SRC2 = MAD_input[i+4]; SRC3 = "0x00000000";
	      	  	       break;
	          default: SRC1 = "0x00000000"; SRC2 ="0x00000000"; SRC3 = "0x00000000";
	      	           break;	      	           	           
	      }
	      if(FXL)
	      {	   
	      	   if(SRC1.to_int() == 0)
	      	   {
	      	   	   fraction1 = 0;
	      	   	   EXP1 = 0;
	      	   }
	      	   else
	      	   {   	   
                 fraction1 = frexp(uint2float(SRC1),&EXP1);
                 fraction1 *= 2;
                 EXP1--;
             }  
             if(SRC2.to_int() == 0)
	      	   {
	      	   	   fraction2 = 0;
	      	   	   EXP2 = 0;
	      	   }   
	      	   else
	      	   {       	           
                 fraction2 = frexp(uint2float(SRC2),&EXP2);
                 fraction2 *= 2;
                 EXP2--;                        
             }
             if(SRC3.to_int() == 0)
	      	   {
	      	   	   fraction3 = 0;
	      	   	   EXP3 = 0;
	      	   }   
	      	   else
	      	   {          
                 fraction3 = frexp(uint2float(SRC3),&EXP3);
                 fraction3 *= 2;
                 EXP3--;
             }
             
             #if DEBUG_MODE
             
             //print info.
             cout<<"------------------------------"<<endl;
             cout<<"pipe["<<i<<"] "<<endl;             
             cout<<"~~~~~~~~~~~~~~~~~~~~~"<<endl;
	           cout<<"SRC_X(fraction1,EXP1) = ("<<fraction1<<','<<EXP1<<')'<<endl;
             cout<<"SRC_Y(fraction2,EXP2) = ("<<fraction2<<','<<EXP2<<')'<<endl;
             cout<<"SRC_Z(fraction3,EXP3) = ("<<fraction3<<','<<EXP3<<')'<<endl;           
             
             #endif
             
             EXP_diff = (EXP1 + EXP2) - EXP3;
             if(EXP_diff >= 0)
             {
                 MAD_EXP[i] = EXP1 + EXP2;
                 MAD_SRC[i*3] = fraction1;
	               MAD_SRC[i*3+1] = fraction2;
	               MAD_SRC[i*3+2] = fraction3/pow(2.0,(double)EXP_diff);	               
             }
             else
             {
                 MAD_EXP[i] = EXP3;
                 MAD_SRC[i*3] = fraction1/pow(2.0,ceil(-(double)EXP_diff/2));
	               MAD_SRC[i*3+1] = fraction2/pow(2.0,floor(-(double)EXP_diff/2));
	               MAD_SRC[i*3+2] = fraction3;	               
             }
	           if(opcode.to_uint() == 13)//TXB
	           {   
	           	   cout<<"<TXB hit>"<<endl;
	           	   MAD_SRC[i*3+2] = -MAD_SRC[i*3+2];
	           }  
	      }
	      else
	      {
	      	  MAD_EXP[i] = 0;
	          MAD_SRC[i*3] = (float)SRC1.to_int();
	          MAD_SRC[i*3+1] = (float)SRC2.to_int();
	          MAD_SRC[i*3+2] = (float)SRC3.to_int();
	      }
	  }
}


void EXE::pipe_SRC_select(void)
{
    switch(opcode)
    {
        //RCP
        case 1: pipe1_EXP = exp_rcp; pipe2_EXP = exp_rcp;
        	      pipe3_EXP = exp_rcp; pipe4_EXP = exp_rcp;
        	      pipe1_X = a1; pipe1_Y = Y_rcp; pipe1_Z = a0;
        	      pipe2_X = a1; pipe2_Y = Y_rcp; pipe2_Z = a0;
        	      pipe3_X = a1; pipe3_Y = Y_rcp; pipe3_Z = a0;
        	      pipe4_X = a1; pipe4_Y = Y_rcp; pipe4_Z = a0;
        	      break;
        //RSQ
        case 2: pipe1_EXP = exp_rsq; pipe2_EXP = exp_rsq;
        	      pipe3_EXP = exp_rsq; pipe4_EXP = exp_rsq;
        	      pipe1_X = b1; pipe1_Y = Y_rsq; pipe1_Z = b0;
        	      pipe2_X = b1; pipe2_Y = Y_rsq; pipe2_Z = b0;
        	      pipe3_X = b1; pipe3_Y = Y_rsq; pipe3_Z = b0;
        	      pipe4_X = b1; pipe4_Y = Y_rsq; pipe4_Z = b0;
        	      break;
        //LOG2
        case 3: pipe1_EXP = exp_log2; pipe2_EXP = exp_log2;
        	      pipe3_EXP = exp_log2; pipe4_EXP = exp_log2;
        	      pipe1_X = d1; pipe1_Y = Y_log2; pipe1_Z = d0;
        	      pipe2_X = d1; pipe2_Y = Y_log2; pipe2_Z = d0;
        	      pipe3_X = d1; pipe3_Y = Y_log2; pipe3_Z = d0;
        	      pipe4_X = d1; pipe4_Y = Y_log2; pipe4_Z = d0;        	      
        	      break;
        //POW2
        case 4: pipe1_EXP = exp_pow2; pipe2_EXP = exp_pow2;
        	      pipe3_EXP = exp_pow2; pipe4_EXP = exp_pow2;
        	      pipe1_X = c1; pipe1_Y = Y_pow2; pipe1_Z = c0;
        	      pipe2_X = c1; pipe2_Y = Y_pow2; pipe2_Z = c0;
        	      pipe3_X = c1; pipe3_Y = Y_pow2; pipe3_Z = c0;
        	      pipe4_X = c1; pipe4_Y = Y_pow2; pipe4_Z = c0;
        	      break;

        default: pipe1_EXP = MAD_EXP[0]; pipe2_EXP = MAD_EXP[1];
        	       pipe3_EXP = MAD_EXP[2]; pipe4_EXP = MAD_EXP[3];
        	       pipe1_X = MAD_SRC[0]; pipe1_Y = MAD_SRC[1]; pipe1_Z = MAD_SRC[2];
        	       pipe2_X = MAD_SRC[3]; pipe2_Y = MAD_SRC[4]; pipe2_Z = MAD_SRC[5];
        	       pipe3_X = MAD_SRC[6]; pipe3_Y = MAD_SRC[7]; pipe3_Z = MAD_SRC[8];
        	       pipe4_X = MAD_SRC[9]; pipe4_Y = MAD_SRC[10];pipe4_Z = MAD_SRC[11];        	       
        	       break;
    }
}


void EXE::DP_op(float P1,float P2,float P3,float P4)
{
	  int   DP4_EXP;
	  int   exp_diff1,exp_diff2,exp_diff3,exp_diff4;
	  float shift_1,shift_2,shift_3,shift_4;
	  float DP_out;

    if(pipe1_EXP > pipe2_EXP)
	     DP4_EXP = pipe1_EXP;
	  else
	     DP4_EXP = pipe2_EXP;

	  if(pipe3_EXP > DP4_EXP)
	     DP4_EXP = pipe3_EXP;

	  if(pipe4_EXP > DP4_EXP)
	     DP4_EXP = pipe3_EXP;

	  exp_diff1 = DP4_EXP - pipe1_EXP;
	  exp_diff2 = DP4_EXP - pipe2_EXP;
	  exp_diff3 = DP4_EXP - pipe3_EXP;
	  exp_diff4 = DP4_EXP - pipe4_EXP;

	  shift_1 = P1 / pow(2.0,(double)exp_diff1);
	  shift_2 = P2 / pow(2.0,(double)exp_diff2);
	  shift_3 = P3 / pow(2.0,(double)exp_diff3);
	  shift_4 = P4 / pow(2.0,(double)exp_diff4);
	  DP_out = (shift_1+shift_2+shift_3+shift_4) * pow(2.0,(double)DP4_EXP);

	  pipe1_out = pipe2_out = pipe3_out = pipe4_out = DP_out;
}


void EXE::out_select(void)
{	    
    switch(opcode)
    {
    	  //MOV
        case 7: MOV();
        	      break;
        //LD
        case 8: 
	  //fix by sheng-chih
	  //cout<<"e_w_mask_in.read(): "<<e_w_mask_in.read()<<endl;
	  //cout<<"e_w_mask_in.read()[0].to_bool(): "<<e_w_mask_in.read()[0].to_bool()<<endl;

	  //cout<<endl<<"temp_mask: "<<temp_mask<<endl;
	  //cout<<endl<<"X2: "<<X2<<endl;
	  //cout<<"Y2: "<<Y2<<endl;
	  //cout<<"Z2: "<<Z2<<endl;
	  //cout<<"W2: "<<W2<<endl;
	  RLT_X = CMEM_value;
	  RLT_Y = CMEM_value;
	  RLT_Z = CMEM_value;
	  RLT_W = CMEM_value;
	  //end fix
	  break;        	      
        //MVI
        case 10: MVI();
        	       break;
        //MIN
        case 11: MINx();
        	       break;
        //MAX
        case 12: MAXx();
        	       break;
        //TEX2D
        case 15: cout<<"<TEX hit>"<<endl;        	       
        	       RLT_X = float2bv(TEX[0]);
        	       RLT_Y = float2bv(TEX[1]);
        	       RLT_Z = float2bv(TEX[2]);
        	       RLT_W = float2bv(TEX[3]);         	                              	       
        	       break;
        //AND
        case 21: AND();
        	       break;
        //OR
        case 22: OR();
        	       break;
        //XOR
        case 23: XOR();
        	       break;
        //SHRL
        case 30: SHRL();
        	       break;        	       
        default: if(FXL)
        	       {
        	           RLT_X = float2bv(pipe1_out);
        	           RLT_Y = float2bv(pipe2_out);
        	           RLT_Z = float2bv(pipe3_out);
        	           RLT_W = float2bv(pipe4_out);
        	       }
        	       else //have not test
        	       {
        	           RLT_X = (int)pipe1_out;
        	           RLT_Y = (int)pipe2_out;
        	           RLT_Z = (int)pipe3_out;
        	           RLT_W = (int)pipe4_out;        	       	
        	       }
        	       break;
    }
}


void EXE::behavior(void)
{    
    float        pipe1_MAD,pipe2_MAD,pipe3_MAD,pipe4_MAD;
    bool         TEX_request;
    unsigned int TEX_fixed;
    unsigned int TEX_offset;
    unsigned int CMEM_offset;
    while(1)
    {        
        //cout<<"<EXE start>"<<endl;
        
        //read from [ID]
        opcode = e_opcode_in.read();
        RL = e_RL_in.read();
        FXL = e_FXL_in.read();
        sa = e_sa_in.read();
        imm_value = e_imm_value_in.read();
        X0 = e_X0_in.read();
        MAD_input[0] = X0;
        Y0 = e_Y0_in.read();
        MAD_input[1] = Y0;
        Z0 = e_Z0_in.read();
        MAD_input[2] = Z0;
        W0 = e_W0_in.read();
        MAD_input[3] = W0;
        X1 = e_X1_in.read();
        MAD_input[4] = X1;
        Y1 = e_Y1_in.read();
        MAD_input[5] = Y1;
        Z1 = e_Z1_in.read();
        MAD_input[6] = Z1;
        W1 = e_W1_in.read();
        MAD_input[7] = W1;
        X2 = e_X2_in.read();
        MAD_input[8] = X2;
        Y2 = e_Y2_in.read();
        MAD_input[9] = Y2;
        Z2 = e_Z2_in.read();
        MAD_input[10] = Z2;
        W2 = e_W2_in.read();
        MAD_input[11] = W2;
        TEX_offset = e_TEX_offset_in.read();       

        #if DEBUG_MODE
        //cout<<"       Exe::opcode= "<< opcode<<endl;//CYLin
        cout<<"*************[EXE]************"<<endl;        
        cout<<"R_data0 = ("<<hex<<X0.to_uint()<<',';
        cout<<Y0.to_uint()<<',';
        cout<<Z0.to_uint()<<',';
        cout<<W0.to_uint()<<')'<<endl;
        
        cout<<"R_data1 = ("<<hex<<X1.to_uint()<<',';
        cout<<Y1.to_uint()<<',';
        cout<<Z1.to_uint()<<',';
        cout<<W1.to_uint()<<')'<<endl;        
        
        cout<<"R_data2 = ("<<hex<<X2.to_uint()<<',';
        cout<<Y2.to_uint()<<',';
        cout<<Z2.to_uint()<<',';
        cout<<W2.to_uint()<<')'<<endl;                 
        
        #endif 

        //TEX request
        if(opcode.to_uint() == 13 || opcode.to_uint() == 15)//TXB or TEX2D 
           TEX_request = 1;
        else
           TEX_request = 0;

        //TEX read
        if(TEX_request)
        {
         	  TEX_fixed = TEX_MEM_EX[TEX_offset];
            TEX[0] = (float)(TEX_fixed>>24);
            TEX[1] = (float)((TEX_fixed>>16)%256);
            TEX[2] = (float)((TEX_fixed>>8)%256);
            TEX[3] = (float)(TEX_fixed%256);
            
            #if DEBUG_MODE
            
            //print info.                                                             
            cout<<"TEX_offset = "<<dec<<TEX_offset<<endl;
            cout<<"TEX R_data = "<<hex<<TEX_fixed<<endl; 
            cout<<"TEX[0] = "<<dec<<TEX[0]<<endl;           
            cout<<"TEX[1] = "<<TEX[1]<<endl;
            cout<<"TEX[2] = "<<TEX[2]<<endl;
            cout<<"TEX[3] = "<<TEX[3]<<endl;            
            
            #endif
        }
        else
        {
            TEX[0] = 0;
            TEX[1] = 0;
            TEX[2] = 0;
            TEX[3] = 0;
        }
        
        //LD 
        if(opcode.to_uint() == 8)
        {
	  //fix by sheng-chih
	  //CMEM_offset = (unsigned int)(uint2float(X0) + uint2float(imm_value));
	    CMEM_offset = (unsigned int)imm_value;
	    //printf("CMEM_offset: %d\n",CMEM_offset);
            CMEM_value = CMEM_EX[CMEM_offset];        
	    //cout<<"CMEM_value: "<<CMEM_value<<endl;

        }
 
        //ST
        if(opcode.to_uint() == 9)
        {
        	  CMEM_offset = (unsigned int)(uint2float(X2) + uint2float(imm_value));
        	  CMEM_EX[CMEM_offset] = X0; 
        	  
        }

        //RCP,RSQ,POW2,LOG2 factors
        exp_rcp = RCP_factor();
        exp_rsq = RSQ_factor();
        exp_pow2 = POW2_factor();
        exp_log2 = LOG2_factor();

        //ADD,MUL,MAD,DP3,DP4 factors
        MAD_factor();

        //select pipe source
        pipe_SRC_select();

        //pipe1
        pipe1_MAD = pipe1_X * pipe1_Y + pipe1_Z;
        pipe1_out = pipe1_MAD * pow(2.0,(double)pipe1_EXP);
        //pipe2
        pipe2_MAD = pipe2_X * pipe2_Y + pipe2_Z;
        pipe2_out = pipe2_MAD * pow(2.0,(double)pipe2_EXP);
        //pipe3
        pipe3_MAD = pipe3_X * pipe3_Y + pipe3_Z;
        pipe3_out = pipe3_MAD * pow(2.0,(double)pipe3_EXP);
        //pipe4
        pipe4_MAD = pipe4_X * pipe4_Y + pipe4_Z;
        pipe4_out = pipe4_MAD * pow(2.0,(double)pipe4_EXP);
        
        #if DEBUG_MODE
        
        //print info.
        cout<<"------------------------------"<<endl;
        cout<<"final(X,Y,Z) = ("<<dec<<pipe1_X<<','<<pipe1_Y<<','<<pipe1_Z<<')'<<endl;
        cout<<"final_EXP = "<<pipe1_EXP<<endl;
        cout<<"fina2(X,Y,Z) = ("<<pipe2_X<<','<<pipe2_Y<<','<<pipe2_Z<<')'<<endl;
        cout<<"fina2_EXP = "<<pipe2_EXP<<endl;
        cout<<"fina3(X,Y,Z) = ("<<pipe3_X<<','<<pipe3_Y<<','<<pipe3_Z<<')'<<endl;
        cout<<"fina3_EXP = "<<pipe3_EXP<<endl;
        cout<<"fina4(X,Y,Z) = ("<<pipe4_X<<','<<pipe4_Y<<','<<pipe4_Z<<')'<<endl;
        cout<<"fina4_EXP = "<<pipe4_EXP<<endl;
        
        #endif

        if(opcode.to_uint() == 6 || opcode.to_uint() == 19)//DP3 or DP4
           DP_op(pipe1_MAD,pipe2_MAD,pipe3_MAD,pipe4_MAD);

        //select output
        out_select();
        
        #if DEBUG_MODE
        
        //print info.
        cout<<"------------------------------"<<endl;
        cout<<"RLT_X = "<<hex<<RLT_X.to_uint()<<endl;
        cout<<"RLT_Y = "<<RLT_Y.to_uint()<<endl;
        cout<<"RLT_Z = "<<RLT_Z.to_uint()<<endl;
        cout<<"RLT_W = "<<RLT_W.to_uint()<<endl;
        cout<<"//////////////////////////////"<<endl;
        
        #endif

        //write to [WB]
        e_RLT_X_out.write(RLT_X);
        e_RLT_Y_out.write(RLT_Y);
        e_RLT_Z_out.write(RLT_Z);
        e_RLT_W_out.write(RLT_W);
        e_opcode_out.write(opcode);
        e_dst_out.write(e_dst_in.read());
        e_w_mask_out.write(e_w_mask_in.read());
        e_old_DST_out.write(e_old_DST_in.read());
        
        //cout<<"<EXE end>"<<endl;
    }
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void WB::behavior(void)
{
    bool       WB_start;
    sc_bv<128> old_DST;
    sc_bv<32>  RLT_X,RLT_Y,RLT_Z,RLT_W;
    sc_uint<5> opcode;
    sc_uint<6> dst;
    sc_bv<4>   w_mask;
    bool       w_en;
    sc_bv<32>  w_data1,w_data2,w_data3,w_data4;
    bool       pre;

    while(1)
    {           
        //write to [IF] ([WB] is empty)
        
    	  if(!is_WB_full)
    	  {
    	      //CYLin
              do
              {
                  WB_start = b_WB_start_in.read();
              }
              while(!WB_start);
    	      
    	      b_finish_out.write(0);
    	      is_WB_full = 1;
    	  }
    	  //cout<<"<WB start>"<<endl;
        
        //read from [EX]
        RLT_X = b_RLT_X_in.read();
        RLT_Y = b_RLT_Y_in.read();
        RLT_Z = b_RLT_Z_in.read();
        RLT_W = b_RLT_W_in.read();
        opcode = b_opcode_in.read();
        dst = b_dst_in.read();
        w_mask = b_w_mask_in.read();
        old_DST = b_old_DST_in.read();               

        //cout<<"       WB::opcode= "<< opcode<<",  is_WB_full= "<<is_WB_full <<endl;//CYLin
        if(is_WB_full)
        {
            
            //previous value of is_FIN
            pre = is_FIN;        

            //update is_FIN
            if(opcode.to_uint() == 14)
               is_FIN = 1;                                 

            //w_en
            if(is_FIN)
               w_en = 0;
            else if(opcode.to_uint() >= 24 && opcode.to_uint()!=30)
               w_en = 0;
            else if(opcode.to_uint() == 9)//ST
               w_en = 0;
            else
               w_en = 1;                        

            //w_data
            if(w_mask[0].to_bool())
               w_data1 = RLT_W;
            else
               w_data1 = old_DST.range(31,0);
            if(w_mask[1].to_bool())
               w_data2 = RLT_Z;
            else
               w_data2 = old_DST.range(63,32);
            if(w_mask[2].to_bool())
               w_data3 = RLT_Y;
            else
               w_data3 = old_DST.range(95,64);
            if(w_mask[3].to_bool())
               w_data4 = RLT_X;
            else
               w_data4 = old_DST.range(127,96);

            //write to [GPR]
            if(w_en)
		GPR_WB[dst] = (w_data4,w_data3,w_data2,w_data1);
	    //added by sheng-chih
	    /*
	    printf("GPR_WB: %d\n",GPR_WB);
	    cout<<"GPR_WB["<<dst<<"][X]: "<<GPR_WB[dst].range(127,96).to_uint()<<endl;
	    cout<<"GPR_WB["<<dst<<"][Y]: "<<GPR_WB[dst].range(95,64).to_uint()<<endl;
	    cout<<"GPR_WB["<<dst<<"][Z]: "<<GPR_WB[dst].range(63,32).to_uint()<<endl;
	    cout<<"GPR_WB["<<dst<<"][W]: "<<GPR_WB[dst].range(31,0).to_uint()<<endl;
	    */
	    //end added
            //write to [IF] ([WB] is full)
            b_finish_out.write(is_FIN);

            //write to [GPU_FRAG_IF]
            b_shader_end_out.write(~pre&is_FIN);//assert 1 time

            #if DEBUG_MODE
            //cout<<"       WB::pre,is_FIN= "<<pre<<" "<< is_FIN<<endl;//CYLin
            //test
            if(~pre&is_FIN)
            {        
                //print info.
                cout<<"------------------------------"<<endl;
                cout<<"RLT_X = "<<hex<<RLT_X.to_uint()<<endl;
                cout<<"RLT_Y = "<<RLT_Y.to_uint()<<endl;
                cout<<"RLT_Z = "<<RLT_Z.to_uint()<<endl;
                cout<<"RLT_W = "<<RLT_W.to_uint()<<endl;
                cout<<"//////////////////////////////"<<endl;
                cout<<"GPR[X] = "<<GPR_WB[0].range(127,96).to_uint()<<endl;
                cout<<"GPR[Y] = "<<GPR_WB[0].range(95,64).to_uint()<<endl;
                cout<<"GPR[Z] = "<<GPR_WB[0].range(63,32).to_uint()<<endl;
                cout<<"GPR[W] = "<<GPR_WB[0].range(31,0).to_uint()<<endl;
                cout<<"//////////////////////////////"<<endl;
                cout<<"w_en = "<<w_en<<endl;
            }   
            
            #endif
                 
        }
        
        //cout<<"<WB end>"<<endl;
    }
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
float GPU_FRAG_IF::uint2float(sc_bv<32> input)
{
    int s_convt , exp_convt , fraction_convt;    
    float answer;	
    
    s_convt = input[31].to_bool();
    exp_convt = input.range(30,23).to_uint();
    fraction_convt = input.range(22,0).to_uint();
    
    if(s_convt == 0 && exp_convt ==0 && fraction_convt ==0)//0-detector
       return 0;    
    
    fraction_convt |= 0x00800000;
    
    answer = (float) fraction_convt / pow(2.0,23.0);
    answer *= pow(2.0,(double)(exp_convt-127));
    if(s_convt)
       answer *= -1;
            
    return answer;    
}


void GPU_FRAG_IF::behavior(void)
{
	  sc_bv<128>   R0,R1;
	  unsigned int X,Y,Z;	 
	  float        temp;
	  sc_uint<8>   R,G,B,A;
	  bool         end;

    while(1)
    {   	
    	  //cout<<"<GPU_FRAG_IF start>"<<endl;
    	  
    	  //read from [WB]
    	  end = i_shader_end_in.read();
    	 
      	//read from [GPR]
      	R0 = GPR_IF[0];// color
      	R1 = GPR_IF[1];// XYZ -> R1[95:0]

        #if DEBUG_MODE

	//added by sheng-chih
	/*     	   
      	cout<<endl<<"  R0(31,0) = "<<hex<<R0.range(31,0).to_uint()<<endl;
      	cout<<"  R0(63,32) = "<<R0.range(63,32).to_uint()<<endl;
      	cout<<"  R0(95,64) = "<<R0.range(95,64).to_uint()<<endl;
      	cout<<"  R0(127,96) = "<<R0.range(127,96).to_uint()<<endl<<endl;
	*/
	//end added
      	if(end)
      	{
	  cout<<"END:"<<endl;
      	cout<<"R0(31,0) = "<<hex<<R0.range(31,0).to_uint()<<endl;
      	cout<<"R0(63,32) = "<<R0.range(63,32).to_uint()<<endl;
      	cout<<"R0(95,64) = "<<R0.range(95,64).to_uint()<<endl;
      	cout<<"R0(127,96) = "<<R0.range(127,96).to_uint()<<endl;
        }
        
        #endif
              	
      	//type convert
      	X = (unsigned int)uint2float(R1.range(95,64));
      	Y = (unsigned int)uint2float(R1.range(63,32));
        Z = (unsigned int)uint2float(R1.range(31,0));
        
      	//write to [UNIFIED_FGAG]      	     	
      	i_shader_finish_out.write(end);       	     	
      	i_X_out.write(X);
      	i_Y_out.write(Y);      	
      	i_Z_out.write(Z);

	//fixed by sheng-chih      	
      	//clamp color(R,G,B,A)

      	temp = uint2float(R0.range(127,96));
	//temp=R0.range(127,96).to_uint();
	//end fixed
	/*
      	if(end)   
	  cout<<"R(float) ="<<showpoint<<temp<<endl;  	                    
	*/
      	if(temp >= 255)
	  R = 255;
      	else if(temp <= 0)
	  R = 0;
      	else
	  R = (unsigned int)temp;
	
      	temp = uint2float(R0.range(95,64)); 
	/*
      	if(end) 
	  cout<<"G(float) ="<<showpoint<<temp<<endl;
	*/
      	if(temp >= 255)
	  G = 255;
      	else if(temp <= 0)
	  G = 0;
      	else
	  G = (unsigned int)temp;
      	   
      	temp = uint2float(R0.range(63,32)); 
	/*
      	if(end)    
	  cout<<"B(float) ="<<showpoint<<temp<<endl; 
	*/ 	
      	if(temp >= 255)
	  B = 255;
      	else if(temp <= 0)
	  B = 0;
      	else
	  B = (unsigned int)temp;
      	   
      	temp = uint2float(R0.range(31,0));
	/*
      	if(end)      
      	   cout<<"A(float) ="<<showpoint<<temp<<endl;	
	*/ 
      	if(temp >= 255)
      	   A = 255;
      	else if(temp <= 0)
      	   A = 0;
      	else
      	   A = (unsigned int)temp;      	         	   


        #if DEBUG_MODE
        
        //print info.
        cout<<"********[GPU_FRAG_IF]********"<<endl;
        cout<<"shader_end(bool) = "<<end<<endl;
        cout<<"A(uint) = "<<dec<<A.to_uint()<<endl;
      	cout<<"R(uint) = "<<R.to_uint()<<endl;
      	cout<<"G(uint) = "<<G.to_uint()<<endl;
      	cout<<"B(uint) = "<<B.to_uint()<<endl;
      	cout<<"==========================================="<<endl;
      	
      	#endif
      	
      	i_color_out.write((A,B,G,R));//ARGB
      	
      	//cout<<"<GPU_FRAG_IF end>"<<endl;
    }
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool UNIFIED_FRAG::test_circuit(sc_uint<3> func,bool sign,bool eq)
{
	  bool SRC1_sig,SRC2_sig;
    bool SRC1_inv,SRC2_inv;
    bool SRC1,SRC2,OR_SRC1,OR_SRC2;

    switch(func)
    {
        case NEVER:  SRC1_sig = 1; SRC2_sig = 1;
        	           SRC1_inv = 0; SRC2_inv = 0;
        	           break;
        case ALWAYS: SRC1_sig = 1; SRC2_sig = 1;
        	           SRC1_inv = 1; SRC2_inv = 1;
        	           break;
        case LESS:   SRC1_sig = 0; SRC2_sig = 1;
        	           SRC1_inv = 0; SRC2_inv = 0;
        	           break;
        case LEQUAL: SRC1_sig = 0; SRC2_sig = 0;
        	           SRC1_inv = 0; SRC2_inv = 0;
        	           break;
        case EQUAL:  SRC1_sig = 1; SRC2_sig = 0;
        	           SRC1_inv = 0; SRC2_inv = 0;
        	           break;
        case GEQUAL: SRC1_sig = 0; SRC2_sig = 0;
        	           SRC1_inv = 1; SRC2_inv = 0;
        	           break;
        case GREATER: SRC1_sig = 0; SRC2_sig = 1;
        	            SRC1_inv = 1; SRC2_inv = 0;
        	            break;
        case NOTEQUAL: SRC1_sig = 1; SRC2_sig = 0;
        	             SRC1_inv = 0; SRC2_inv = 1;
        	             break;
        default:       SRC1_sig = 0; SRC2_sig = 0;
        	             SRC1_inv = 0; SRC2_inv = 0;
        	             break;
    }

    SRC1 = (SRC1_sig)? 0 : sign;
    SRC2 = (SRC2_sig)? 0 : eq;
    OR_SRC1 = (SRC1_inv)? ~SRC1 : SRC1;
    OR_SRC2 = (SRC2_inv)? ~SRC2 : SRC2;

    return (OR_SRC1 | OR_SRC2);
}


sc_uint<8> UNIFIED_FRAG::stencil_update(sc_uint<3> func,sc_uint<8> S_ref,sc_uint<8> old_S)
{
	sc_uint<8> return_data;

    switch(func)
    {
        case KEEP:	  return_data = old_S;
        	            break;
        case ZERO:    return_data = 0;
        	            break;
        case REPLACE: return_data = S_ref;
        	            break;
        case INCR:    if(old_S.to_uint() < 255)
        	               return_data = old_S + (sc_uint<8>)1;
        	            else
        	               return_data = 255;
        	            break;
        case DECR:    if(old_S.to_uint() > 0)
        	               return_data = old_S - (sc_uint<8>)1;
        	            else
        	               return_data = 0;
        	            break;
        case INVERT:  return_data = ~old_S;
        	            break;
        case INCR_WRAP: if(old_S.to_uint() < 255)
        	                 return_data = old_S + (sc_uint<8>)1;
        	              else
        	                 return_data = 0;
        	              break;
        case DECR_WRAP: if(old_S.to_uint() > 0)
        	                 return_data = old_S - (sc_uint<8>)1;
        	              else
        	                 return_data = 255;
        	              break;
        default: return_data = old_S;
        	       break;
    }

    return return_data;
}


sc_uint<32> UNIFIED_FRAG::factor_selector(bool SRC_DST,sc_uint<4> func,sc_uint<32> SRC_color,sc_uint<32> DST_color)
{
    sc_uint<32>  color,return_data;
    unsigned int DST_color_A_com,blend_f;
    unsigned int factor_A,factor_R,factor_G,factor_B;
    unsigned int A,R,G,B;

    //blend_f
    DST_color_A_com = 255 - DST_color.range(31,24);
    if(SRC_color.range(31,24) < DST_color_A_com)
       blend_f = SRC_color.range(31,24);
    else
       blend_f = DST_color_A_com;

    //select factor
    switch(func)
    {
        case BLEND_ZERO: factor_A = 0;
        	               factor_R = 0;
        	               factor_G = 0;
        	               factor_B = 0;
        	               break;
        case BLEND_ONE: factor_A = 255;//8'hff
        	              factor_R = 255;
        	              factor_G = 255;
        	              factor_B = 255;
        	              break;
        case BLEND_SRCCOLOR: factor_A = SRC_color.range(31,24);
        	                   factor_R = SRC_color.range(23,16);
        	                   factor_G = SRC_color.range(15,8);
        	                   factor_B = SRC_color.range(7,0);
        	                   break;
        case BLEND_ONEMINUSSRCCOLOR: factor_A = 255 - SRC_color.range(31,24);
        	                           factor_R = 255 - SRC_color.range(23,16);
        	                           factor_G = 255 - SRC_color.range(15,8);
        	                           factor_B = 255 - SRC_color.range(7,0);
        	                           break;
        case BLEND_DSTCOLOR: factor_A = DST_color.range(31,24);
        	                   factor_R = DST_color.range(23,16);
        	                   factor_G = DST_color.range(15,8);
        	                   factor_B = DST_color.range(7,0);
        	                   break;
        case BLEND_ONEMINUSDSTCOLOR: factor_A = 255 - DST_color.range(31,24);
        	                           factor_R = 255 - DST_color.range(23,16);
        	                           factor_G = 255 - DST_color.range(15,8);
        	                           factor_B = 255 - DST_color.range(7,0);
        	                           break;
        case BLEND_SRCALPHA: factor_A = SRC_color.range(31,24);
        	                   factor_R = SRC_color.range(31,24);
        	                   factor_G = SRC_color.range(31,24);
        	                   factor_B = SRC_color.range(31,24);
        	                   break;
        case BLEND_ONEMINUSSRCALPHA: factor_A = 255 - SRC_color.range(31,24);
        	                           factor_R = 255 - SRC_color.range(31,24);
        	                           factor_G = 255 - SRC_color.range(31,24);
        	                           factor_B = 255 - SRC_color.range(31,24);
        	                           break;
        case BLEND_DSTALPHA: factor_A = DST_color.range(31,24);
        	                   factor_R = DST_color.range(31,24);
        	                   factor_G = DST_color.range(31,24);
        	                   factor_B = DST_color.range(31,24);
        	                   break;
        case BLEND_ONEMINUSDSTALPHA: factor_A = 255 - DST_color.range(31,24);
        	                           factor_R = 255 - DST_color.range(31,24);
        	                           factor_G = 255 - DST_color.range(31,24);
        	                           factor_B = 255 - DST_color.range(31,24);
        	                           break;
        case BLEND_SRCALPHASATURATE: factor_A = 255;
        	                           factor_R = blend_f;
        	                           factor_G = blend_f;
        	                           factor_B = blend_f;
        	                           break;
        default: factor_A = 255;
        	       factor_R = 255;
        	       factor_G = 255;
        	       factor_B = 255;
        	       break;
    }

    #if DEBUG_MODE 
      
    //print info.
    cout<<"factor_A = "<<dec<<factor_A<<endl;
    cout<<"factor_R = "<<factor_R<<endl;
    cout<<"factor_G = "<<factor_G<<endl;
    cout<<"factor_B = "<<factor_B<<endl;
    
    #endif

    //real factor
    if(factor_A != 0)
       ++factor_A;

    if(factor_R != 0)
       ++factor_R;

    if(factor_R != 0)
       ++factor_G;

    if(factor_B != 0)
       ++factor_B;
  
    #if DEBUG_MODE       
    
    //print info.       
    cout<<"factor_A(+1) = "<<factor_A<<endl;
    cout<<"factor_R(+1) = "<<factor_R<<endl;
    cout<<"factor_G(+1) = "<<factor_G<<endl;
    cout<<"factor_B(+1) = "<<factor_B<<endl;       
    
    #endif

    //SRC_color or DST_color?
    if(SRC_DST)
       color = DST_color;
    else
       color = SRC_color;           

    A = (factor_A * color.range(31,24))/256;
    R = (factor_R * color.range(23,16))/256;
    G = (factor_G * color.range(15,8) )/256;
    B = (factor_B * color.range(7,0)  )/256;

    #if DEBUG_MODE
         
    //print info.
    cout<<"blend color = "<<hex<<color.to_uint()<<endl;
    cout<<"A(hex) = "<<dec<<A<<endl;
    cout<<"R(hex) = "<<R<<endl;
    cout<<"G(hex) = "<<G<<endl;
    cout<<"B(hex) = "<<B<<endl;    

    #endif

    return_data = ((sc_uint<8>)A,(sc_uint<8>)R,(sc_uint<8>)G,(sc_uint<8>)B);

    return return_data;
}


void UNIFIED_FRAG::behavior(void)
{
	  bool         shader_finish;
	  unsigned int X,Y,Z;
	  sc_uint<32>  color;
	  //----------------------------
	  unsigned int offset,old_value;
	  sc_uint<3>   update_func;
	  sc_uint<8>   S_ref,old_S,update_S;
	  sc_uint<9>   add_A,add_R,add_G,add_B;
	  sc_uint<16>  update_Z;
	  sc_uint<32>  old_Z,blend_src,blend_dst;
	  bool         sign,eq;
	  bool         S_pass,Z_pass,is_S_pass,is_Z_pass,Z_CMP_pass;
	  bool         ZS_pass;


    while(1)
    {
        //cout<<"<FRAG start>"<<endl;
        
        //read from [GPU_FRAG_IF]
        shader_finish = u_shader_finish_in.read();
        X = u_X_in.read();
        Y = u_Y_in.read();
        Z = u_Z_in.read();
        color = u_color_in.read();

        if(shader_finish)
        {
            //offset , S_ref            
            offset = X + LCD_WIDTH * Y;
            S_ref = stencil_ref & stencil_mask;                                           
            
            #if DEBUG_MODE 
            
            //print info.
            cout<<"********[UNIFIED_FRAG]********"<<endl; 
            cout<<"in_X(uint) = "<<X<<endl;
            cout<<"in_Y(uint) = "<<Y<<endl;
            cout<<"in_Z(uint) = "<<Z<<endl;
            cout<<"in_color(uint) = "<<hex<<color.to_uint()<<endl;
            cout<<"offset(uint) = "<<dec<<offset<<endl;
            cout<<"S_ref(uint) = "<<hex<<S_ref.to_uint()<<endl;
            cout<<"//////////////////////////////"<<endl;
                        
            #endif
            
            //initial ZS_pass
            ZS_pass = 1;

            //do zcompare or stencil test?
            if(zcompare_enable | stencil_enable)
            {
                //read from [ZS_MEM]
                old_value = ZS_MEM_FRAG[offset];

                //old Z , old_S                
                old_Z = (old_value>>16)%65536;//mod 65536 can be removed
                old_S = (old_value%256) & stencil_mask;

                //stencil test
                sign = (S_ref < old_S)? 1 : 0;
                eq = (S_ref == old_S)? 1 : 0;
                S_pass = test_circuit(stencil_func,sign,eq);
                is_S_pass = ~stencil_enable | S_pass;

                //z compare
                sign = (Z < old_Z.to_uint())? 1 : 0;
                eq = (Z == old_Z.to_uint())? 1 : 0;
                Z_pass = test_circuit(zcompare_func,sign,eq);
                is_Z_pass = ~zcompare_enable | S_pass;
                Z_CMP_pass = zcompare_enable & Z_pass;

                //set ZS_pass
                ZS_pass = is_S_pass & is_Z_pass;

                //select stencil update function
                if(!is_S_pass)
                   update_func = fail;
                else if(is_Z_pass)
                   update_func = zpass;
                else
                   update_func = zfail;

                //stencil update op.
                if(stencil_enable)
                   update_S = stencil_update(update_func,S_ref,old_S);
                else
                   update_S = old_S;

                //z compare update op.
                if(Z_CMP_pass)
                   update_Z = Z;
                else
                   update_Z = old_Z.range(15,0);

                //write to [ZS_MEM]?
                if(stencil_enable | Z_CMP_pass)
                   ZS_MEM_FRAG[offset] = (update_Z.to_uint() << 16) + update_S.to_uint();
                
                #if DEBUG_MODE  
                   
                //print info.  
                cout<<"=========="<<endl;
                cout<<"|<ZS CMP>|"<<endl;
                cout<<"=========="<<endl;
                cout<<"ZS_BUF R_data(uint) = "<<hex<<old_value<<endl; 
                cout<<"old_Z(uint) = "<<hex<<old_Z.to_uint()<<endl;
                cout<<"old_S(uint) = "<<hex<<old_S.to_uint()<<endl;                 
                cout<<"(Z_en,S_en) = ("<<zcompare_enable<<','<<stencil_enable<<')'<<endl;
                cout<<"(Z_pass,S_pass) = ("<<Z_pass<<','<<S_pass<<')'<<endl;
                cout<<"Z_BUF W_data = "<<ZS_MEM_FRAG[offset]<<endl;                
                cout<<"//////////////////////////////"<<endl;
                
                #endif
            }

            //do blending op.?
            if(blending_enable & ZS_pass)
            {
                //read from [F_MEM]
                old_value = F_MEM_FRAG[offset];

            	  //selected_factor * color
                blend_src = factor_selector(0,blending_srcfunc,color,old_value);
                blend_dst = factor_selector(1,blending_dstfunc,color,old_value);

                //blend src color & dst color + clamp
                add_A = blend_src.range(31,24) + blend_dst.range(31,24);
                if(add_A.to_uint() > 255)
                   add_A = 255;
                add_R = blend_src.range(23,16) + blend_dst.range(23,16);
                if(add_R.to_uint() > 255)
                   add_R = 255;
                add_G = blend_src.range(15,8) + blend_dst.range(15,8);
                if(add_G.to_uint() > 255)
                   add_G = 255;
                add_B = blend_src.range(7,0) + blend_dst.range(7,0);
                if(add_B.to_uint() > 255)
                   add_B = 255;

                //update write color
                color = (add_A.range(7,0),add_R.range(7,0),add_G.range(7,0),add_B.range(7,0));
 
                #if DEBUG_MODE                  
                
                //print info.
                cout<<"============"<<endl;
                cout<<"|<BLEND OP>|"<<endl;
                cout<<"============"<<endl;
                cout<<"F_BUF R_data(uint) = "<<hex<<old_value<<endl;
                cout<<"BLEND_SRC_color = "<<blend_src.to_uint()<<endl;
                cout<<"BLEND_DST_color = "<<blend_dst.to_uint()<<endl;
                cout<<"//////////////////////////////"<<endl;                                 
                
                #endif
            }

            //write to [F_MEM]?
            if(ZS_pass)
            {
                /*cout<<"in_X(uint) = "<<dec<<X<<endl;
                cout<<"in_Y(uint) = "<<Y<<endl;
                cout<<"in_Z(uint) = "<<Z<<endl;                            	
                F_MEM_FRAG[offset] = color;
                cout<<"F_BUF W_data ="<<hex<<color.to_uint()<<endl<<endl;*/
                //cout.width(8);                
	      //fixed by sheng-chih
	      //if(info_num<4){cout<<dec<<X<<"  "<<Y<<"  "<<Z<<"  "<<hex<<color.to_uint()<<endl;info_num++;}//CYLin::debug
		//end fixed
                //framebuff<<dec<<X<<"  "<<Y<<"  "<<hex<<color.to_uint()<<endl;
                databuff = color.to_uint();
                info_mp.Type     = 1; //0:read, 1:write
		info_mp.Addr     = X+Y*320;
		info_mp.DataDest = &databuff;
		info_mp.Burst    = 1;
		mp_transfer.write(info_mp);
		
		//wait Target port response		
		if(mp_response.read()==1)
		{}
		else
		    cout<<"FS.Initiator: transaction err"<<endl;
            
            }
            
            //notify [SC]            
            u_FRAG_finish_out.write(1);
        }
        else
           u_FRAG_finish_out.write(0);
           
        //cout<<"<FRAG end>"<<endl;
    }
}


