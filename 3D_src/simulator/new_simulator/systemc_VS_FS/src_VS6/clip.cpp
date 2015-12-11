#include <iostream>
#include <string>
#include <cstddef>
#include "clip.h"
#include <stdio.h>
using namespace std;
#define DEBUG_CLIP 1
#if DEBUG_CLIP
  #define dprintf(x...) printf(x)
#else
  #define dprintf(x...) 
#endif

void clip::line_segment_and_plane_intersection( int plane_choice ,vertex_entry v1,vertex_entry v2,int number)
{
    float t;
    float vertex1[4];
    float vertex2[4];
    int i;
    
    //MY ADD for perspective
    vertex_entry CV1,CV2,CVb,CNV;
    
    //int index;
    for(i=0;i<4;i++)
      {
	vertex1[i]=v1.entry1[i];
	vertex2[i]=v2.entry1[i];
	//dprintf("v1=%f v2=%f\n",v1.entry1[i],v2.entry1[i]);
      }
    //dprintf("\n");
    switch(plane_choice){
      //線段與 x= x_min 平面求交點
    case 0 :
      {
	if( vertex1[0]==x_min )
	  {
	    vertex_reg[plane_choice][number].entry1[0]=vertex1[0];
	    vertex_reg[plane_choice][number].entry1[1]=vertex1[1];
	    vertex_reg[plane_choice][number].entry1[2]=vertex1[2];
	    vertex_reg[plane_choice][number].entry1[3]=vertex1[3];
	    t=0;
	    
	  }
	else if( vertex2[0]==x_min )
	  {
	    vertex_reg[plane_choice][number].entry1[0]=vertex2[0];
	    vertex_reg[plane_choice][number].entry1[1]=vertex2[1];
	    vertex_reg[plane_choice][number].entry1[2]=vertex2[2];
	    vertex_reg[plane_choice][number].entry1[3]=vertex2[3];
	    t=1;
	    
	  }
	else
	  {
	    //求 t 值
	    
	    t=float(x_min-vertex1[0])/float(vertex2[0]-vertex1[0]);
	    vertex_reg[plane_choice][number].entry1[0]=x_min;
	    vertex_reg[plane_choice][number].entry1[1]=vertex1[1]+(vertex2[1]-vertex1[1])*t;
	    vertex_reg[plane_choice][number].entry1[2]=vertex1[2]+(vertex2[2]-vertex1[2])*t;
	    vertex_reg[plane_choice][number].entry1[3]=1/(1/vertex1[3]+(1/vertex2[3]-1/vertex1[3])*t);
	    
	    	  }
	break;
      }
      //線段與 x= 1 平面求交點
	case 1:
		{
			if( vertex1[0]==x_max )
			{
			    vertex_reg[plane_choice][number].entry1[0]=vertex1[0];
		        vertex_reg[plane_choice][number].entry1[1]=vertex1[1];
		        vertex_reg[plane_choice][number].entry1[2]=vertex1[2];
		        vertex_reg[plane_choice][number].entry1[3]=vertex1[3];
				t=0;
			}
		    else if( vertex2[0]==x_max )
			{
			    vertex_reg[plane_choice][number].entry1[0]=vertex2[0];
		        vertex_reg[plane_choice][number].entry1[1]=vertex2[1];
		        vertex_reg[plane_choice][number].entry1[2]=vertex2[2];
		        vertex_reg[plane_choice][number].entry1[3]=vertex2[3];
				t=1;
			}
		    else
			{
			    //求 t 值
		        t=float(x_max-vertex1[0])/float(vertex2[0]-vertex1[0]);
		        vertex_reg[plane_choice][number].entry1[0]=x_max;
		        vertex_reg[plane_choice][number].entry1[1]=vertex1[1]+(vertex2[1]-vertex1[1])*t;
		        vertex_reg[plane_choice][number].entry1[2]=vertex1[2]+(vertex2[2]-vertex1[2])*t;
		        vertex_reg[plane_choice][number].entry1[3]=1/(1/vertex1[3]+(1/vertex2[3]-1/vertex1[3])*t);
			}
		    break;
		}
	//線段與 y=-1 平面求交點
	case 2:
		{
			if( vertex1[1]==y_min )
			{
				vertex_reg[plane_choice][number].entry1[0]=vertex1[0];
		        vertex_reg[plane_choice][number].entry1[1]=vertex1[1];
		        vertex_reg[plane_choice][number].entry1[2]=vertex1[2];
		        vertex_reg[plane_choice][number].entry1[3]=vertex1[3];
				t=0;
			}
		    else if( vertex2[1]==y_min )
			{
			    vertex_reg[plane_choice][number].entry1[0]=vertex2[0];
		        vertex_reg[plane_choice][number].entry1[1]=vertex2[1];
		        vertex_reg[plane_choice][number].entry1[2]=vertex2[2];
		        vertex_reg[plane_choice][number].entry1[3]=vertex2[3];
				t=1;
			}
		    else
			{
			    //求 t 值
			  //t=float(-1.0-vertex1[1])/float(vertex2[1]-vertex1[1]);
			  //shang-chang fix -1.0 => y_min
			t=float(y_min-vertex1[1])/float(vertex2[1]-vertex1[1]);
		        vertex_reg[plane_choice][number].entry1[0]=vertex1[0]+(vertex2[0]-vertex1[0])*t;
		        vertex_reg[plane_choice][number].entry1[1]=y_min;
		        vertex_reg[plane_choice][number].entry1[2]=vertex1[2]+(vertex2[2]-vertex1[2])*t;
		        vertex_reg[plane_choice][number].entry1[3]=1/(1/vertex1[3]+(1/vertex2[3]-1/vertex1[3])*t);
			}
			break;
		}
	//線段與 y= 1 平面求交點
	case 3:
		{
			if( vertex1[1]==y_max )
			{
			    vertex_reg[plane_choice][number].entry1[0]=vertex1[0];
		        vertex_reg[plane_choice][number].entry1[1]=vertex1[1];
		        vertex_reg[plane_choice][number].entry1[2]=vertex1[2];
		        vertex_reg[plane_choice][number].entry1[3]=vertex1[3];
				t=0;
			}
		    else if( vertex2[1]==y_max )
			{
			    vertex_reg[plane_choice][number].entry1[0]=vertex2[0];
		        vertex_reg[plane_choice][number].entry1[1]=vertex2[1];
		        vertex_reg[plane_choice][number].entry1[2]=vertex2[2];
		        vertex_reg[plane_choice][number].entry1[3]=vertex2[3];
				t=1;
			}
		    else
			{
			  //求 t 值
			  
			  //t=float(1.0-vertex1[1])/float(vertex2[1]-vertex1[1]);
			  //sheng-chang fix 1.0=>y_max
			  t=float(y_max-vertex1[1])/float(vertex2[1]-vertex1[1]);
			    vertex_reg[plane_choice][number].entry1[0]=vertex1[0]+(vertex2[0]-vertex1[0])*t;
			    vertex_reg[plane_choice][number].entry1[1]=y_max;
			    vertex_reg[plane_choice][number].entry1[2]=vertex1[2]+(vertex2[2]-vertex1[2])*t;
			    vertex_reg[plane_choice][number].entry1[3]=1/(1/vertex1[3]+(1/vertex2[3]-1/vertex1[3])*t);
			 			  
			}
			break;
		}
	//線段與 z=-1 平面求交點
	case 4:
		{
			if( vertex1[2]==-1 )
			{
			    vertex_reg[plane_choice][number].entry1[0]=vertex1[0];
		        vertex_reg[plane_choice][number].entry1[1]=vertex1[1];
		        vertex_reg[plane_choice][number].entry1[2]=vertex1[2];
		        vertex_reg[plane_choice][number].entry1[3]=vertex1[3];
				t=0;
			}
		    else if( vertex2[2]==-1 )
			{
			    vertex_reg[plane_choice][number].entry1[0]=vertex2[0];
			    vertex_reg[plane_choice][number].entry1[1]=vertex2[1];
			    vertex_reg[plane_choice][number].entry1[2]=vertex2[2];
			    vertex_reg[plane_choice][number].entry1[3]=vertex2[3];
			    t=1;
			}
		    else
			{
			    //求 t 值
		        t=float(-1.0-vertex1[2])/float(vertex2[2]-vertex1[2]);
		        vertex_reg[plane_choice][number].entry1[0]=vertex1[0]+(vertex2[0]-vertex1[0])*t;
		        vertex_reg[plane_choice][number].entry1[1]=vertex1[1]+(vertex2[1]-vertex1[1])*t;
		        vertex_reg[plane_choice][number].entry1[2]=-1;
		        vertex_reg[plane_choice][number].entry1[3]=1/(1/vertex1[3]+(1/vertex2[3]-1/vertex1[3])*t);
			}
		    break;
		}
	//線段與 z= 1 平面求交點
	case 5:
		{
			if( vertex1[2]==1 )
			{
			    vertex_reg[plane_choice][number].entry1[0]=vertex1[0];
		        vertex_reg[plane_choice][number].entry1[1]=vertex1[1];
		        vertex_reg[plane_choice][number].entry1[2]=vertex1[2];
		        vertex_reg[plane_choice][number].entry1[3]=vertex1[3];
				t=0;
			}
		    else if( vertex2[2]==1 )
			{
			    vertex_reg[plane_choice][number].entry1[0]=vertex2[0];
		        vertex_reg[plane_choice][number].entry1[1]=vertex2[1];
		        vertex_reg[plane_choice][number].entry1[2]=vertex2[2];
		        vertex_reg[plane_choice][number].entry1[3]=vertex2[3];
				t=1;
			}
		    else
			{
			    //求 t 值
		        t=float(1.0-vertex1[2])/float(vertex2[2]-vertex1[2]);
		        vertex_reg[plane_choice][number].entry1[0]=vertex1[0]+(vertex2[0]-vertex1[0])*t;
		        vertex_reg[plane_choice][number].entry1[1]=vertex1[1]+(vertex2[1]-vertex1[1])*t;
		        vertex_reg[plane_choice][number].entry1[2]=1;
		        vertex_reg[plane_choice][number].entry1[3]=1/(1/vertex1[3]+(1/vertex2[3]-1/vertex1[3])*t);
			}
		    break;
		}

    default:
		{
		    vertex_reg[plane_choice][number].entry1[0]=0;
		    vertex_reg[plane_choice][number].entry1[1]=0;
		    vertex_reg[plane_choice][number].entry1[2]=0;
		    vertex_reg[plane_choice][number].entry1[3]=0;
			t=0;
			break;
		}
	}//end switch
	//for(i=0;i<2;i++)
	//{
	//dprintf("(%d) %f\n", i, vertex_reg[plane_choice][number].entry1[i]);
	//}
/*
   for(i=0;i<4;i++){
	   vertex_reg[plane_choice][number].entry2[i]=Vertex[0].entry2[i]+(Vertex[1].entry2[i]-Vertex[0].entry2[i])*t;
   }

    for(i=0;i<4;i++){
	   vertex_reg[plane_choice][number].entry3[i]=Vertex[0].entry3[i]+(Vertex[1].entry3[i]-Vertex[0].entry3[i])*t;
   }

    for(i=0;i<4;i++){
	   vertex_reg[plane_choice][number].entry4[i]=Vertex[0].entry4[i]+(Vertex[1].entry4[i]-Vertex[0].entry4[i])*t;
   }
*/
//correct coding bug by gk
//e.g. Vertex[0].entry2[0] => v1.entry2[0]


	for(i=0;i<4;i++){
		vertex_reg[plane_choice][number].entry2[i]=v1.entry2[i]+(v2.entry2[i]-v1.entry2[i])*t;
	}

	for(i=0;i<4;i++){
		vertex_reg[plane_choice][number].entry3[i]=v1.entry3[i]+(v2.entry3[i]-v1.entry3[i])*t;
	}

	for(i=0;i<4;i++){
		vertex_reg[plane_choice][number].entry4[i]=v1.entry4[i]+(v2.entry4[i]-v1.entry4[i])*t;
	}
	dprintf("after clip\n");
	dprintf("c.x=%f, c.y=%f, c.w=%f\n", vertex_reg[plane_choice][number].entry1[0],vertex_reg[plane_choice][number].entry1[1],vertex_reg[plane_choice][number].entry1[3]);
	dprintf("c.u=%f, c.v=%f\n",vertex_reg[plane_choice][number].entry2[0],vertex_reg[plane_choice][number].entry2[1]);
  
	//CV 1,2,b
	for(int i=0;i<4;i++)
      {
	    CV1.entry1[i]=(v1.entry1[i])/(v1.entry1[3]);
	    CV1.entry2[i]=(v1.entry2[i])/(v1.entry1[3]);
	    CV1.entry3[i]=(v1.entry3[i])/(v1.entry1[3]);
	    CV1.entry4[i]=(v1.entry4[i])/(v1.entry1[3]);
	    CV2.entry1[i]=(v2.entry1[i])/(v2.entry1[3]);
	    CV2.entry2[i]=(v2.entry2[i])/(v2.entry1[3]);
	    CV2.entry3[i]=(v2.entry3[i])/(v2.entry1[3]);
	    CV2.entry4[i]=(v2.entry4[i])/(v2.entry1[3]);
	    CVb.entry1[i]=(vertex_reg[plane_choice][number].entry1[i])/(vertex_reg[plane_choice][number].entry1[3]);
	    CVb.entry2[i]=(vertex_reg[plane_choice][number].entry2[i])/(vertex_reg[plane_choice][number].entry1[3]);
	    CVb.entry3[i]=(vertex_reg[plane_choice][number].entry3[i])/(vertex_reg[plane_choice][number].entry1[3]);
	    CVb.entry4[i]=(vertex_reg[plane_choice][number].entry4[i])/(vertex_reg[plane_choice][number].entry1[3]);
      }
      //t : no change
      //CNV
      for(int i=0;i<3;i++)
      {
	CNV.entry1[i]=CVb.entry1[i];  
      }
      CNV.entry1[3]=CV1.entry1[3]+(CV2.entry1[3]-CV1.entry1[3])*t;
      for(int i=0;i<4;i++)
      {
	CNV.entry2[i]=CV1.entry2[i]+(CV2.entry2[i]-CV1.entry2[i])*t;  
      }
      for(int i=0;i<4;i++)
      {
	CNV.entry3[i]=CV1.entry3[i]+(CV2.entry3[i]-CV1.entry3[i])*t;  
      }
      for(int i=0;i<4;i++)
      {
	CNV.entry4[i]=CV1.entry4[i]+(CV2.entry4[i]-CV1.entry4[i])*t;  
      }
      //CNVF
      for(int i=0;i<4;i++)
	{
	    vertex_reg[plane_choice][number].entry1[i]=CNV.entry1[i]*(vertex_reg[plane_choice][number].entry1[3]);
	    //CNV.entry1[3] should be 1
	}
      for(int i=0;i<4;i++)
	{
	    vertex_reg[plane_choice][number].entry2[i]=CNV.entry2[i]*(vertex_reg[plane_choice][number].entry1[3]);
	}
	for(int i=0;i<4;i++){
		vertex_reg[plane_choice][number].entry3[i]=CNV.entry3[i]*(vertex_reg[plane_choice][number].entry1[3]);
	}

	for(int i=0;i<4;i++){
		vertex_reg[plane_choice][number].entry4[i]=CNV.entry4[i]*(vertex_reg[plane_choice][number].entry1[3]);
	}
	
}

void clip::sh_clip(vertex_entry v1,vertex_entry v2,int plane_choice,int out_vertex_number)
{
     int case_sel,i;
     bool v1_in;
     bool v2_in;
    // vertex_entry Vertex[2];
    
	dprintf("plane_choice = %d\n",plane_choice);
	dprintf("v1.x=%f v1.y=%f v1.z=%f\n",v1.entry1[0],v1.entry1[1],v1.entry1[2]);
	dprintf("v2.x=%f v2.y=%f\n",v2.entry1[0],v2.entry1[1],v2.entry1[2]);

     switch (plane_choice)//CHOICE which boundary
     {
        case 0:              //x_min
        
               //dprintf("v1.x=%f\n",v1.entry1[0]);
               //dprintf("v2.x=%f\n",v2.entry1[0]);
               //dprintf("x_min=%f",x_min);
               
               if(v1.entry1[0]<x_min)
               v1_in=0;
               else
               v1_in=1;
               
               if(v2.entry1[0]<x_min)
               v2_in=0;
               else
               v2_in=1;
               break;
        case 1:
               if(v1.entry1[0]>x_max)
               v1_in=0;
               else
               v1_in=1;
               
               if(v2.entry1[0]>x_max)
               v2_in=0;
               else
               v2_in=1;
               break;
        case 2:
               if(v1.entry1[1]<y_min)
               v1_in=0;
               else
               v1_in=1;
               
               if(v2.entry1[1]<y_min)
               v2_in=0;
               else
               v2_in=1;
               break;
        case 3:
             if(v1.entry1[1]>y_max)
               v1_in=0;
               else
               v1_in=1;
               
               if(v2.entry1[1]>y_max)
               v2_in=0;
               else
               v2_in=1;
               break;
        case 4:
             if(v1.entry1[2]<-1.0)//z_min
               v1_in=0;
               else
               v1_in=1;
               
               if(v2.entry1[2]<-1.0)
               v2_in=0;
               else
               v2_in=1;
               break;
        case 5:
             if(v1.entry1[2]> 1.0)//z_min
               v1_in=0;
               else
               v1_in=1;
               
               if(v2.entry1[2]> 1.0)
               v2_in=0;
               else
               v2_in=1;
               break;
     }
     if(v1_in==1&&v2_in==1)//in-in
     case_sel=0;
     else if(v1_in==1&&v2_in==0)//in-out     
     case_sel=1;
     else if(v1_in==0&&v2_in==0)//out-out
     case_sel=2;
     else if(v1_in==0&&v2_in==1)//out-in
     case_sel=3;
     else{
     dprintf("sh_clip_error");
     }

     dprintf("v1-v2 case%d\n",case_sel);
     
     switch(case_sel)
       {
       case 0:
	 for(i=0;i<4;i++)
	   {
	     vertex_reg[plane_choice][out_vertex_number].entry1[i]=v2.entry1[i];
	     vertex_reg[plane_choice][out_vertex_number].entry2[i]=v2.entry2[i];
	     vertex_reg[plane_choice][out_vertex_number].entry3[i]=v2.entry3[i];
	     vertex_reg[plane_choice][out_vertex_number].entry4[i]=v2.entry4[i];
	     dprintf("vertex_reg[%d][%d].entry1[%d]=%f\n",plane_choice,out_vertex_number,i,vertex_reg[plane_choice][out_vertex_number].entry1[i]);
	   }
	 out_vertex_number++;
	 break;
       case 1:
	 
	 line_segment_and_plane_intersection(plane_choice,v1,v2,out_vertex_number);
	 out_vertex_number++;
	 break;
       case 2:
	   
	 break;
       case 3:
	 line_segment_and_plane_intersection(plane_choice,v1,v2,out_vertex_number);
	 out_vertex_number++;
	 
	 
	 for(i=0;i<4;i++)
	   {
	     vertex_reg[plane_choice][out_vertex_number].entry1[i]=v2.entry1[i];
	     vertex_reg[plane_choice][out_vertex_number].entry2[i]=v2.entry2[i];
	     vertex_reg[plane_choice][out_vertex_number].entry3[i]=v2.entry3[i];
	     vertex_reg[plane_choice][out_vertex_number].entry4[i]=v2.entry4[i];
	   }
	 out_vertex_number++;
	 
         
         
	 break;
     }
/*
     if(case_sel==3){
	     dprintf("vertex_reg[%d][%d].entry1[0]=%f\n",plane_choice,out_vertex_number-2,vertex_reg[plane_choice][out_vertex_number-2].entry1[0]);
	     dprintf("vertex_reg[%d][%d].entry1[1]=%f\n",plane_choice,out_vertex_number-2,vertex_reg[plane_choice][out_vertex_number-2].entry1[1]);
	     dprintf("vertex_reg[%d][%d].entry1[0]=%f\n",plane_choice,out_vertex_number-1,vertex_reg[plane_choice][out_vertex_number-1].entry1[0]);
	     dprintf("vertex_reg[%d][%d].entry1[1]=%f\n",plane_choice,out_vertex_number-1,vertex_reg[plane_choice][out_vertex_number-1].entry1[1]);
	
     }else{
	     dprintf("vertex_reg[%d][%d].entry1[0]=%f\n",plane_choice,out_vertex_number-1,vertex_reg[plane_choice][out_vertex_number-1].entry1[0]);
	     dprintf("vertex_reg[%d][%d].entry1[1]=%f\n",plane_choice,out_vertex_number-1,vertex_reg[plane_choice][out_vertex_number-1].entry1[1]);
     }
*/

     switch(plane_choice)
     {
           case 0: min_x_reg_count=out_vertex_number;
                   break;
           case 1: max_x_reg_count=out_vertex_number;
                   break;
           case 2: min_y_reg_count=out_vertex_number;
                   break;
           case 3: max_y_reg_count=out_vertex_number;
                   break;
           case 4: min_z_reg_count=out_vertex_number;
                   break;
	   case 5: max_z_reg_count=out_vertex_number;
	     dprintf("vertex_reg[5][%d].x = %f\n",out_vertex_number-1,vertex_reg[5][out_vertex_number-1].entry1[0]);
	     dprintf("vertex_reg[5][%d].y = %f\n",out_vertex_number-1,vertex_reg[5][out_vertex_number-1].entry1[1]);
	     dprintf("vertex_reg[5][%d].z = %f\n",out_vertex_number-1,vertex_reg[5][out_vertex_number-1].entry1[2]);
	     dprintf("vertex_reg[5][%d].w = %f\n",out_vertex_number-1,vertex_reg[5][out_vertex_number-1].entry1[3]);
                   break;

     }
}
void clip::tri_convert()
{
  int first,last,third,j;
  dprintf("max_z_reg_count= %d\n",max_z_reg_count);
  
  if(max_z_reg_count>=3)
    {
      for(vertex_number_out=0;vertex_number_out<3;vertex_number_out++)
        {
	  for(j=0;j<4;j++)
	    {
	      vertex_reg_out[vertex_number_out].entry1[j]=vertex_reg[5][vertex_number_out].entry1[j];
	      vertex_reg_out[vertex_number_out].entry2[j]=vertex_reg[5][vertex_number_out].entry2[j];
	      vertex_reg_out[vertex_number_out].entry3[j]=vertex_reg[5][vertex_number_out].entry3[j];
	      vertex_reg_out[vertex_number_out].entry4[j]=vertex_reg[5][vertex_number_out].entry4[j];
	    }
	  dprintf("tri.x=%f, tri.y=%f\n",vertex_reg[5][vertex_number_out].entry1[0],vertex_reg[5][vertex_number_out].entry1[1]);
        }
    }
  else 
    {
      vertex_number_out=0;
      return ;
    }
  
  
  if(max_z_reg_count>3)
    {
      first=0;
      last=2;
      for(third=3;third<max_z_reg_count;third++)
	{    
	  dprintf("third=%d,vertex_number_out=%d\n",third,vertex_number_out);
	  for(j=0;j<4;j++)
	    {
	      vertex_reg_out[vertex_number_out].entry1[j]=vertex_reg[5][first].entry1[j];
	      vertex_reg_out[vertex_number_out].entry2[j]=vertex_reg[5][first].entry2[j];
	      vertex_reg_out[vertex_number_out].entry3[j]=vertex_reg[5][first].entry3[j];
	      vertex_reg_out[vertex_number_out].entry4[j]=vertex_reg[5][first].entry4[j];
	    } 
	  
	  dprintf("tri.x=%f, tri.y=%f\n",vertex_reg[5][first].entry1[0],vertex_reg[5][first].entry1[1]);
	  vertex_number_out++;
          
	  for(j=0;j<4;j++)
	    {
	      vertex_reg_out[vertex_number_out].entry1[j]=vertex_reg[5][last].entry1[j];
	      vertex_reg_out[vertex_number_out].entry2[j]=vertex_reg[5][last].entry2[j];
	      vertex_reg_out[vertex_number_out].entry3[j]=vertex_reg[5][last].entry3[j];
	      vertex_reg_out[vertex_number_out].entry4[j]=vertex_reg[5][last].entry4[j];
	    } 
	  dprintf("tri.x=%f, tri.y=%f\n",vertex_reg[5][last].entry1[0],vertex_reg[5][last].entry1[1]);
          
	  vertex_number_out++;
          
	  for(j=0;j<4;j++)
	    {
	      vertex_reg_out[vertex_number_out].entry1[j]=vertex_reg[5][third].entry1[j];
	      vertex_reg_out[vertex_number_out].entry2[j]=vertex_reg[5][third].entry2[j];
	      vertex_reg_out[vertex_number_out].entry3[j]=vertex_reg[5][third].entry3[j];
	      vertex_reg_out[vertex_number_out].entry4[j]=vertex_reg[5][third].entry4[j];
	    }  
	  dprintf("tri.x=%f, tri.y=%f\n",vertex_reg[5][third].entry1[0],vertex_reg[5][third].entry1[1]);
	  vertex_number_out++;
          
	  last=third;//gk correct the coding error
	}
      
      //last=third;
      
    }    
 }

void clip::clip_module()
{
    // bool v1_outcode[6];
     //bool v2_outcode[6];
     //int  v1_in_out;
     //int  v2_in_out;
     int  vertex_number=3;//triangle with 3 vertex
     int i;
    
    max_x_reg_count=0;
    min_x_reg_count=0;
    max_y_reg_count=0;
    min_y_reg_count=0;
    max_z_reg_count=0;
    min_z_reg_count=0;


//fix coding bug by gk
//correct the last vertext 
//e.g. Vertex[vertex_number-1] => Vertex[min_x_reg_count-1]
     //--------------------------clip (-)x --------------------------------------------
         for(i=0;i<vertex_number-1;i++)
         {
               sh_clip(Vertex[i],Vertex[i+1],0,min_x_reg_count);
         }
	 cout << "min_x_reg_count = " << min_x_reg_count << "\n"; 
	 sh_clip(Vertex[vertex_number-1],Vertex[0],0,min_x_reg_count);
	 cout << "min_x_reg_count = " << min_x_reg_count << "\n"; 
	 dprintf("x_min_end\n\n");
     //--------------------------clip(+)x----------------------------------------------
         for(i=0;i<min_x_reg_count-1;i++)
         {
               sh_clip(vertex_reg[0][i],vertex_reg[0][i+1],1,max_x_reg_count);
         }
	 cout << "max_x_reg_count = " << max_x_reg_count << "\n"; 
	 //sh_clip(vertex_reg[0][min_x_reg_count-1],vertex_reg[0][0],1,max_x_reg_count);   
	 sh_clip(vertex_reg[0][min_x_reg_count-1],vertex_reg[0][0],1,max_x_reg_count);   
	 cout << "max_x_reg_count = " << max_x_reg_count << "\n"; 
	 dprintf("x_max_end\n\n");
     //--------------------------clip (-)y ---------------------------------------------
         for(i=0;i<max_x_reg_count-1;i++)
         {
               sh_clip(vertex_reg[1][i],vertex_reg[1][i+1],2,min_y_reg_count);
         }
	 cout << "min_y_reg_count = " << min_y_reg_count << "\n";       
	 sh_clip(vertex_reg[1][max_x_reg_count-1],vertex_reg[1][0],2,min_y_reg_count);
	 cout << "min_y_reg_count = " << min_y_reg_count << "\n";       
	 dprintf("y_min_end\n\n");
               
     //--------------------------clip (+)y --------------------------------------------- 
     for(i=0;i<min_y_reg_count-1;i++)
         {
               sh_clip(vertex_reg[2][i],vertex_reg[2][i+1],3,max_y_reg_count);
         }
     cout << "max_y_reg_count = " << max_y_reg_count << "\n";                      
     sh_clip(vertex_reg[2][min_y_reg_count-1],vertex_reg[2][0],3,max_y_reg_count);  
     cout << "max_y_reg_count = " << max_y_reg_count << "\n";                      
     dprintf("y_max_end\n\n");
     //--------------------------clip (-)z ---------------------------------------------
     for(i=0;i<max_y_reg_count-1;i++)
         {
               sh_clip(vertex_reg[3][i],vertex_reg[3][i+1],4,min_z_reg_count);
         }
               sh_clip(vertex_reg[3][max_y_reg_count-1],vertex_reg[3][0],4,min_z_reg_count);
               
               dprintf("z_min_end\n\n");
     //--------------------------clip (+)z ---------------------------------------------
     for(i=0;i<min_z_reg_count-1;i++)
         {
               sh_clip(vertex_reg[4][i],vertex_reg[4][i+1],5,max_z_reg_count);
         }
               sh_clip(vertex_reg[4][min_z_reg_count-1],vertex_reg[4][0],5,max_z_reg_count);
               
               dprintf("z_max_end\n\n");
    //====================================triange strip convert=========================
    
    tri_convert();
        
}
