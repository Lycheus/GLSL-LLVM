#include <systemc.h>
#include <stdlib.h>
#include <iostream>
//VS
#include "./src_VS6/SC_VS.h"
#include "./src_VS6/IO_VS.h"
//FS 
#include "./src_FS5/FS.h"
#include "./src_FS5/IO_FS.h"
using namespace std;

  IO_FS *FILE_FS;
  SC_VS *vertex_shader;
  IO_VS *FILE_VS;
  FS *frag_shader;
  //move to global by gk
  sc_event next_obj_VS;
  sc_event next_obj_FS;
  //gk add

int sc_main(int argc, char* argv[])
{    

    //VS
    sc_fifo<Req>   f_req_1(1);
    sc_fifo<bool>  f_bool_1(1);
    sc_fifo<Req>   f_req_2(1);
    sc_fifo<bool>  f_bool_2(1);
    //FS
    sc_fifo<Req>   f_req_3(1);
    sc_fifo<bool>  f_bool_3(1);
    sc_fifo<Req>   f_req_4(1);
    sc_fifo<bool>  f_bool_4(1);
    //connect VS and FS
    //sc_fifo<float>  f_VOB(48);
    //sheng-chang changed size 48->96
    sc_fifo<float>  f_VOB(96);
    sc_signal<bool> s_VS_END;
    
    //=====VS=====
    vertex_shader = new SC_VS("Vertex_Shader");    
    //FS_top *frag_shader;
    //frag_shader = new FS_top("frag_shader");  
    vertex_shader->mp_transfer(f_req_1);
    vertex_shader->mp_response(f_bool_1);
    vertex_shader->sp_transfer(f_req_2);
    vertex_shader->sp_response(f_bool_2);
    
    FILE_VS = new IO_VS("FILE_VS");  
    FILE_VS->sp_transfer(f_req_1);
    FILE_VS->sp_response(f_bool_1);
    FILE_VS->mp_transfer(f_req_2);
    FILE_VS->mp_response(f_bool_2);
    
    //=====FS=====

    frag_shader = new FS("frag_shader");    
    //frag_shader->FRAG->mp_transfer(f_req_3);
    //frag_shader->FRAG->mp_response(f_bool_3);
    frag_shader->mp_transfer(f_req_3);
    frag_shader->mp_response(f_bool_3);
    frag_shader->sp_transfer(f_req_4);
    frag_shader->sp_response(f_bool_4);
    
    FILE_FS = new IO_FS("FILE_FS");  
    FILE_FS->sp_transfer(f_req_3);
    FILE_FS->sp_response(f_bool_3);
    FILE_FS->mp_transfer(f_req_4);
    FILE_FS->mp_response(f_bool_4);
    
    //VS & FS
    vertex_shader->mp_VOB(f_VOB);
    frag_shader->sp_VOB(f_VOB);
    vertex_shader->mp_VS_END(s_VS_END);
    frag_shader->sp_VS_END(s_VS_END);
    
    cout<<"=====start simulation====="<<endl;
    sc_start();
    cout<<"=====end simulation====="<<endl;

    delete vertex_shader;
    delete FILE_VS;
    delete frag_shader;
    delete FILE_FS;

    return 0;      	
}
