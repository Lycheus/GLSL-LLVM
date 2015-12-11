#ifndef INLINEFUNCTION_H
#define INLINEFUNCTION_H
#include <stdio.h>
#include <stdlib.h>

#define ARGU_REGI 15
#define RETU_REGI 16
#define OUR_STACK_REGI 2
#define OUR_TEMP_REGI 3

#define W_RECIPROCAL
#define FOR_REAL_FS
#define FS_DMEM_SET 

#define FS_DMEM_START 1024


int GL_Clamp(int return_regi,
	     int x_regi,
	     int min_regi,
	     int max_regi,
	     int remain_regi,
	     int SV,
	     int type,   //type 0 : genType clamp (genType x, genType minVal, genType maxVal)
	                 //type 1 : genType clamp (genType x, float minVal, float maxVal)
	     FILE *output_ass);

int link_Vec3ToVec2(int input_regi,
		    int return_regi,
		    FILE *output_ass);

int link_Vec2ToVec2(int input_regi,
		    int return_regi,
		    FILE *output_ass);

int link_ConVec4(int input_regi, 
		 int input_regi2, 
		 int input_regi3, 
		 int input_regi4, 
		 int return_regi,
		 char type,
		 FILE* output_ass);

int link_ConVec4_Vec2_Vec2(int input_regi,
			   int input_regi2,
			   int return_regi,
			   FILE* output_ass);

int link_ConVec4_Vec2_F_F(int input_regi,
			  int input_regi2,
			  int input_regi3,
			  int return_regi,
			  FILE* output_ass);

int link_ConVec4_Vec3_F(int input_regi,
			int input_regi2,
			int return_regi,
			FILE* output_ass);

int link_ConVec3_Vec2_F(int input_regi,
			int input_regi2,
			int return_regi,
			FILE* output_ass);

int link_ConVec3_F_Vec2(int input_regi,
			int input_regi2,
			int return_regi,
			FILE* output_ass);

int link_ConVec3_F_F_F(int input_regi,
		       int input_regi2,
		       int input_regi3,
		       int return_regi,
		       FILE* output_ass);

int link_ConVec2_F_F(int input_regi,
		     int input_regi2,
		     int return_regi,
		     FILE* output_ass);

int link_Convert_ItoF(int return_regi,
		      int input_regi,
		      FILE* output_ass);

int link_Convert_FtoI(int return_regi,
                      int input_regi,
                      FILE* output_ass);

int link_Convert_ItoB(int return_regi,
                      int input_regi,
                      FILE* output_ass);

int link_VectorNegate(int input_regi,
		      int return_regi,
		      FILE *output_ass);

int link_Vec4ToVec3(int input_regi,
		    int return_regi,
		    FILE *output_ass);

int link_FToVec3(int input_regi,
		 int return_regi,
		 FILE *output_ass);

int GL_Fract(int return_regi,
	     int regiX,
	     int remain_regi,
	     int remain_regi2,
	     char VOrF,
	     int SV,
	     FILE *output_ass);

int GL_Floor(int return_regi,
	     int regiX,
	     int remain_regi,
	     char VOrF,
	     int SV,
	     FILE *output_file);

int GL_Ceil(int return_regi,
	    int regiX,
	    char VOrF,
	    int SV,
	    FILE *output_ass);

int GL_Radians(int return_regi,
	       int regi1,
	       int remain_regi,
	       int SV,
	       FILE *output_ass);

int GL_Degrees(int return_regi,
	      int regi1,
	      int remain_regi,
	      int SV,
	      FILE *output_file);

int GL_Sin(int input_angle_regi,
	   int remain_regi,
	   int remain_regi2,
	   int remain_regi3,
	   int remain_regi4,
	   int return_regi,
	   int type,//1:float, 2:vec2, 3:vec3, and 4:vec4
	   char *assembly_string,
	   FILE *output_ass);

int GL_Cos(int input_angle_regi,
	   int remain_regi,
	   int remain_regi2,
	   int remain_regi3,
	   int remain_regi4,
	   int remain_regi5,
	   int return_regi,
	   int type,
	   char *assembly_string,
	   FILE *output_ass);

int GL_Tan(int input_angle_regi,
           int remain_regi,
           int remain_regi2,
           int remain_regi3,
           int remain_regi4,
           int remain_regi5,
           int return_regi,
           int type,
           char* assembly_string,
           FILE *output_ass);

int GL_ASin(int input_angle_regi,
	   int remain_regi,
	   int remain_regi2,
	   int remain_regi3,
	   int remain_regi4,
	   int return_regi,
	   int type,//1:float, 2:vec2, 3:vec3, and 4:vec4
	   char *assembly_string,
	   FILE *output_ass);

int GL_Pow(int return_regi,
	   int regiX,
	   int regiY,
	   int remain_regi,
	   int SV,
	   FILE *output_ass);


int GL_Exp(int return_regi,
	   int regiX,
	   int remain_regi,
	   int remain_regi2,
	   int SV,
	   FILE *output_ass);

int GL_Refract(int return_regi,
	       int I_regi,
	       int N_regi,
	       int eta_regi,
	       int remain_regi,
	       int remain_regi2,
	       int remain_regi3,
	       int remain_regi4,
	       char VOrF,
	       int SV,
	       FILE *output_ass);

int GL_Reflect(int return_regi,
	       int I_regi,
	       int N_regi,
	       int remain_regi,
	       int SV,
	       FILE *output_ass);

int GL_Length(int return_regi,
	      int input_regi,
	      int remain_regi,
	      int remain_regi2,
	      int SV,
	      FILE *output_ass);

int GL_Log(int return_regi,
	   int regiX,
	   int remain_regi,
	   int remain_regi2,
	   int SV,
	   FILE *output_ass);//remain_regi can be used OUR_TEMP_REGI

int GL_Distance(int return_regi,
		int p0_regi,
		int p1_regi,
		int remain_regi,
		int remain_regi2,
		int SV,
		FILE *output_ass);

int GL_Exp2(int return_regi,
	    int regiX,
	    int remain_regi,
	    int SV,
	    FILE *output_ass);//remain_regi can be used OUR_TEMP_REGI

int GL_Log2(int return_regi,
	    int regiX,
	    int remain_regi,
	    int SV,
	    FILE *output_ass);//remain_regi can be used OUR_TEMP_REGI

int GL_Sqrt(int return_regi,
	    int regiX,
	    int remain_regi,
	    int remain_regi2,
	    int SV,
	    FILE *output_ass);//remain_regi or remain_regi2 can be used OUR_TEMP_REGI

int GL_InverseSqrt(int return_regi,
		   int regiX,
		   int remain_regi,
		   int SV,
		   FILE *output_ass);//remain_regi or remain_regi2 can be used OUR_TEMP_REGI



int GL_CallRegister(FILE *output_ass);

int inline_fneg(int input_Rd,
		int input_Rs,
		FILE *output_ass); //Kaymon

int inline_extractelement(int input_Rd,
			  int input_Rs,
			  int swi, //swizzle
			  int SV,
			  FILE *output_ass); //Kaymon

int inline_insertelement(int input_Rd,
			 int input_Rs,
			 int input_Rt,
			 int swi, //swizzle
			 int SV,
			 FILE *output_ass); //Kaymon

int inline_vsldoi(int input_Rd,
		  int input_Rs,
		  int input_Rt,
		  int input_sa,
		  FILE *output_ass);

int inline_vmrghw(int input_Rd,
		  int input_Rs,
		  int input_Rt,
		  FILE *output_ass);

int inline_SUBF(int Rd,int Rs,int Rt,int WM,int SV);

int GL_VectorTimesScalar(int Vec_regi,
			 int Sca_regi,
			 int bool_float,
			 int Vec_len,
			 FILE *output_ass);

int GL_Start(FILE* output_ass,
	     char VOrF);


int GL_Dot(int return_regi,
	   int regiX,
	   int regiY,
	   int SV,
	   FILE* output_ass);

int GL_Cross(int return_regi,
	     int x_regi,
	     int y_regi,
	     int remain_regi,
	     int remain_regi2,
	     FILE* output_ass);

int GL_Abs(int return_regi,
	   int regiX,
	   char VOrF,
	   int SV,
	   FILE *output_ass);


int GL_Sign(int return_regi,
            int regiX,
            char VOrF,
            int SV,
            FILE *output_ass);

int GL_Mod(int return_regi,
	   int regiX,
	   int regiY,
	   int remain_regi,
	   int remain_regi2,
	   int remain_regi3,
	   char VOrF,
	   int SV,
	   int type,   //type 0 : genType mod (genType x, genType y)
	               //type 1 : genType mod (genType x, float y)
	   FILE* output_ass);

int GL_Max(int return_regi,
	   int regiX,
	   int regiY,
	   int remain_regi,
	   int SV,
	   int type,    //type 0 : genType max (genType x, genType y)
	                //type 1 : genType max (genType x, float y)
	   FILE* output_ass);

int GL_Min(int return_regi,
	   int regiX,
	   int regiY,
	   int remain_regi,
	   int SV,
	   int type,    //type 0 : genType min (genType x, genType y)
	                //type 1 : genType min (genType x, float y)
	   FILE* output_ass);

int GL_Mix(int return_regi,
	   int x_regi,
	   int y_regi,
	   int a_regi,
	   int remain_regi,
	   int type,          //type 0: genType x, genType y, genType a
                              //type 1: genType x, genType y, float a
	   int SV,
	   FILE *output_ass);

int GL_SmoothStep(int return_regi,
		  int edge0_regi,
		  int edge1_regi,
		  int x_regi,
		  int remain_regi,
		  int remain_regi2,
		  int remain_regi3,
		  int type,        //type 0: genType edge0, genType edge1, genType x 
		                   //type 1: float edge0, float edge1, genType x 
		  int VOrF,
		  int SV,
		  FILE *output_ass);

int GL_Step(int return_regi,
	    int edge_regi,
	    int x_regi,
	    int remain_regi,
	    int remain_regi1,
	    int type,        //type 0: genType edge, genType x 
                             //type 1: float edge, genType x 
	    char VOrF,
	    int SV,
	    FILE *output_ass);

int GL_FaceForward(int return_regi,
		   int N_regi,
		   int I_regi,
		   int Nref_regi,
		   int remain_regi,
		   char VOrF,
		   int SV,
		   FILE *output_ass);

int GL_Normalize(int vector_reg,
		 int remain_reg,
		 int remain_reg2,
		 int return_reg,
		 int vector_num,
		 FILE *output_ass);

int GL_FloatDiv(int Y_regi,
		int X_regi,
		int remain_regi,
		int remain_regi2,
		int vector_num,
		FILE* output_ass); // Y=Y/X

int GL_LessThan(int return_regi,
		int x_regi,
		int y_regi,
		int remain_regi,
		char VOrF,
		int mode,
		int SV,
		FILE *output_ass);

int GL_LessThanEqual(int return_regi,
                     int x_regi,
                     int y_regi,
                     int remain_regi,
                     char VOrF,
                     int mode, //mode = 0 => arguments is vector. mode = 1 => arguments is ivector.                                                                                                                
                     int SV,
                     FILE *output_ass);

int GL_GreaterThan(int return_regi,
		   int x_regi,
		   int y_regi,
		   int remain_regi,
		   char VOrF,
		   int mode,
		   int SV,
		   FILE *output_ass);

int GL_GreaterThanEqual(int return_regi,
			int x_regi,
			int y_regi,
			int remain_regi,
			char VOrF,
			int mode,
			int SV,
			FILE *output_ass);

int GL_LessThanEqual(int return_regi,
		     int x_regi,
		     int y_regi,
		     int retmain_regi,
		     char VOrF,
		     int mode,
		     int SV,
		     FILE *output_ass);

int GL_Equal(int return_regi,
	     int x_regi,
	     int y_regi,
	     int remain_regi,
	     char VOrF,
	     int SV,
	     FILE *output_ass);

int GL_NotEqual(int return_regi,
		int x_regi,
		int y_regi,
		int remain_regi,
		char VOrF,
		int SV,
		FILE *output_ass);

int GL_Any(int return_regi,
	   int x_regi,
	   int remain_regi,
	   char VOrF,
	   int SV,
	   FILE *output_ass);

int GL_All(int return_regi,
	   int x_regi,
	   int remain_regi,
	   char VOrF,
	   int SV,
	   FILE *output_ass);

int GL_Not(int return_regi,
	   int x_regi,
	   int remain_regi,
	   char VOrF,
	   int SV,
	   FILE *output_ass);

int GL_texture2D();

int GL_textureCube();

int GL_LoadVarying(int varying_regi);

int GL_LoadAttribute(int att_regi,
		     int vector_size,
		     FILE *output_ass);

int GL_VaryingPut(int att_regi,
		  int vector_size,
		  FILE *output_ass);

int GL_ChangeMatrix(int return_regi,
		    int argu_regi,
		    int argu_regi_2,
		    int argu_regi_3,
		    int argu_regi_4,
		    int remain_regi,
		    int step,
		    int vector_num,
		    FILE* output_ass);

int GL_MatrixTimesVector(int matrix_reg,
			 int matrix_reg1,
			 int matrix_reg2,
			 int matrix_reg3,
			 int vector_reg,
			 int remain_regi,
			 int return_reg,
			 int bool_change_matrix_vector,
			 int computative_size,
			 FILE *output_ass);

int GL_MatrixTimesScalar(int vector_reg,
			 int matrix_reg,
			 int matrix_reg1,
			 int matrix_reg2,
			 int matrix_reg3,
			 int remain_regi,
			 int return_reg,
			 int computative_size,
			 FILE *output_ass);

int GL_END(int return_regi,
	   FILE* output_ass);

int GL_ENDFrag(FILE* output_ass);

/*
int loadFromIMemory(int store_regi,int remain_regi,int vector_size,int address,int row_major_to_column_major,int start_address,FILE *output_file);
int MatrixTimesVector(int matrix_reg,int vector_reg,int return_reg,int ForI,FILE *output_file);
int MatrixTimesMatrix(int matrix_reg,int matrix_reg2,int return_reg,int ForI,FILE *output_file);
int Normalize(int vector_reg,int remain_reg,int remain_reg2,int return_reg,char *vec_name,FILE *output_file);
struct uniform_form *get_uniform(FILE *input_file);
void load_IR(FILE *IR_file,FILE *out);
*/
#endif
