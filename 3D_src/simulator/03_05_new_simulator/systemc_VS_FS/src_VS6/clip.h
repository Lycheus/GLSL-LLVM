typedef struct vertex_entry
{
       float entry1[4];
       float entry2[4];
       float entry3[4];
       float entry4[4];
};
class clip {
            public:
                        vertex_entry   vertex_reg[6][9];
                        //vertex_entry min_x_vertex_reg[9];
                        //vertex_entry max_y_vertex_reg[9];
                        //vertex_entry min_y_vertex_reg[9];
                        //vertex_entry max_z_vertex_reg[9];
                        //vertex_entry min_z_vertex_reg[9];//final stage
                        //-------------6 stage counter----------
                        int max_x_reg_count;
                        int min_x_reg_count;
                        int max_y_reg_count;
                        int min_y_reg_count;
                        int max_z_reg_count;
                        int min_z_reg_count;

                        
                        //--------------------------------------
            
                        //boundary
                        float x_max;
                        float x_min;
                        float y_max;
                        float y_min;
                        int vertex_number_out;
                        vertex_entry Vertex[3];//first triangle input
                        vertex_entry vertex_reg_out[21];//final triangle output
                        
                        //fuction
                        //void outcode(float vertex[4],bool outcode[6]);
                        
                        void line_segment_and_plane_intersection(int plane_choice ,vertex_entry v1,vertex_entry v2,int number);
                        void tri_convert();
                        void clip_module();
                        void sh_clip(vertex_entry v1,vertex_entry v2,int plane_choice,int out_vertex_number);
            };
