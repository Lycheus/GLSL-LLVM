/**********************************************************
    Laboratory   : CIIS Lab.
    File name    : _CONTEXT_H
    Author       : KengYu Lin
    Create date  : 01/18/2006
    Note         : FOR H/W Integration
    Modifications:
**********************************************************/
#ifndef _CONTEXT_H
#define _CONTEXT_H

/*INCLUDE HEADER FILE SECTION*/
#include "gl.h"
#include "type.h"
#include "Register.h"

/*LOCAL MACRO DECLARATIONS */
#define GETCURRENT_CTX(x) (struct context* x = &mycontext;)
#define MAX_NUM_LIGHTS 8
#define MAX_LEVEL_MM 10

/*DATA STRUCTURE DECLARATIONS*/

//*********************************************************
//****                Transform                        ****
//*********************************************************
struct GLmatrix{
   GLfixed m[16];                 // 16 matrix elements (16-byte aligned)
   GLfixed inv[16];	          // optional 16-element inverse (16-byte aligned)
   GLuint flags;                  // possible values determined by (of \link * MatFlags MAT_FLAG_* flags\endlink)
   GLenum matrixType;
}; // structure GLmatrix end

struct matrix_stack
{
   struct GLmatrix *Top;          // points into Stack
   struct GLmatrix *Stack;        // array [MaxDepth] of GLmatrix
   GLuint Depth;                  // 0 <= Depth < MaxDepth
   GLuint MaxDepth;               // size of Stack[] array
   //GLuint DirtyFlag;            // _NEW_MODELVIEW or _NEW_PROJECTION, for example
}; // structure matrix_stack end

struct viewport_attrib {
   GLint X, Y;                    // position
   GLsizei Width, Height;         // size
   GLfixed Near, Far;             // Depth buffer range
   struct GLmatrix _WindowMap;    // Mapping transformation as a matrix.
}; // structure view_port_attrib end



//********************************************************
//****                Color & Lighting                ****
//********************************************************
struct current_Color {
	GLfixed red;
	GLfixed green;
	GLfixed blue;
	GLfixed alpha;
};

struct current_Normal {
	GLfixed nx;
	GLfixed ny;
	GLfixed nz;
};

struct gl_light {
	struct gl_light *next;          // double linked list with sentinel
	struct gl_light *prev;

	GLfixed Ambient[4];             // ambient color
	GLfixed Diffuse[4];             // diffuse color
	GLfixed Specular[4];            // specular color
	GLfixed EyePosition[4];         // position in eye coordinates
	GLfixed SpotDirection[3];       // spotlight direction in eye coordinates

	GLfixed SpotExponent;
	GLfixed SpotCutoff;             // in degrees

	GLfixed ConstantAttenuation;
	GLfixed LinearAttenuation;
	GLfixed QuadraticAttenuation;
	GLboolean Enabled;              // On/off flag
};

struct gl_lightmodel{
	GLfixed Ambient[4];             // ambient color
	GLboolean TwoSide;              // Two (or one) sided lighting
};

struct gl_light_attrib {
	GLboolean Enabled;
	GLenum ShadeModel;
	
	struct gl_light light_i[MAX_NUM_LIGHTS];  // Array of lights
	struct gl_lightmodel Model;               // Lighting model
};

struct gl_material_attrib {
	GLfixed Ambient[4];
	GLfixed Diffuse[4];
	GLfixed Specular[4];
	GLfixed Shininess;
	GLfixed Emission[4];
};

//********************************************************
//****                  ArrayPointer                  ****
//********************************************************

struct array_pointer {
	GLint    size;
	GLenum   type;
	GLsizei  stride;
	const GLvoid  *ptr;
	GLsizei  elementSize;
};

struct draw_array{
	GLenum   mode;
	GLint    first;
	GLsizei  count;
};

struct draw_element{
	GLenum   mode;
	GLsizei  count;
	GLenum   type;
	const GLvoid* indices;
};

//********************************************************
//********************************************************

struct Clear_buffer_value
{
	//initial value of Color buffer
	GLclampx red;
	GLclampx green;
	GLclampx blue;
	GLclampx alpha;

	//initial value of Depth buffer
	GLclampx depth;

	//initial value of Stencil buffer
	GLint s;
}; // structure Clear_buffer_value end


struct Buffer_mask
{
	GLboolean mask_R;
	GLboolean mask_G;
	GLboolean mask_B;
	GLboolean mask_A;

	GLboolean mask_D;
	GLuint    mask_S;
}; // structure Buffer_mask end

struct Texture{
  int WrapS, WrapT, MinFilter, MagFilter;
  int level, format;
  GLfixed *Texels;  // FIXED-POINT RGB (in [0,1]) PER TEXEL
  GLfixed *MipMap[MAX_LEVEL_MM];  //MipMap array for /2, /4, /8, ...
  int W, H;       // TEXTURE DIMENSIONS
};

struct Texture_unit{
  int Texture2dEnabled;               /*!< Enable TEXTURE flag.*/
  int TexEnvMode;                     /*!< Value of color sum function */
  GLfixed EnvColor[4];                /*!< Value of texture env. color */
  GLfixed CurrentTexCoords[4];        /*!< Current texture coords value*/
  int iCurrentBoundtex;  /*!< index this texture obj is belond to which texture object */
};
#define GL_CLAMP                  0x812E
//********************************************
//          Define Performance Struct
//********************************************
struct funs_history
{
	struct ActiveTexture
	{
		GLuint count;
		GLenum texture;
	}glActiveTexture;

	struct AlphaFunc
	{
		GLuint count;
		GLenum func; GLclampf ref;
	}glAlphaFunc;
	
	struct AlphaFuncx
	{
		GLuint count;
		GLenum func; GLclampx ref;
	}glAlphaFuncx;

	struct BindTexture
	{
		GLuint count;
		GLenum target; GLuint texture;
	}glBindTexture;

	struct BlendFunc
	{
		GLuint count;
		GLenum sfactor; GLenum dfactor;
	}glBlendFunc;

	struct Clear
	{
		GLuint count;
		GLbitfield mask;
	}glClear;

	struct ClearColor
	{
		GLuint count;
		GLclampf red, green, blue, alpha;
	}glClearColor;

	struct ClearColorx
	{
		GLuint count;
		GLclampx red, green, blue, alpha;
	}glClearColorx;

	struct ClearDepthf
	{
		GLuint count;
		GLclampf depth;
	}glClearDepthf;

	struct ClearDepthx
	{
		GLuint count;
		GLclampx depth;
	}glClearDepthx;

	struct ClearStencil
	{
		GLuint count;
		GLint s;
	}glClearStencil;

	struct ClientActiveTexture
	{
		GLuint count;
		GLenum texture;
	}glClientActiveTexture;

	struct Color4f
	{
		GLuint count;
		GLfloat red, green, blue, alpha;
	}glColor4f;

	struct Color4x
	{
		GLuint count;
		GLfixed red, green, blue, alpha;
	}glColor4x;

	struct ColorMask
	{
		GLuint count;
		GLboolean red, green, blue, alpha;
	}glColorMask;

	struct ColorPointer
	{
		GLuint count;
		GLint size; GLenum type; GLsizei stride; 
		const GLvoid* pointer;
	}glColorPointer;

	struct CompressedTexImage2D
	{
		GLuint count;
		GLenum target; GLint level; GLenum internalformat; GLsizei width; GLsizei height; GLint border;
		GLsizei imageSize; const GLvoid* data;
	}glCompressTexImage2D;

	struct CompressedTexSubImage2D
	{
		GLuint count;
		GLenum target; GLint level; GLint xoffset; GLint yoffset; GLsizei widthl; GLsizei height; GLenum format;
		GLsizei imageSize; const GLvoid* data;
	}glCompresseTexSubImage2D;

	struct CopyTexImage2D
	{
		GLuint count;
		GLenum target; GLint level; GLenum internalformat; GLint x; GLint y; GLsizei width; GLsizei height; GLint border;
	}glCopyTexImage2D;

	struct CopyTexSubImage2D
	{
		GLuint count;
		GLenum target; GLint level; GLint xoffset; GLint yoffset; GLint x; GLint y; GLsizei width; GLsizei height;
	}glCopyTexSubImage2D;

	struct CullFace
	{
		GLuint count;
		GLenum mode;
	}glCullFace;

	struct DeleteTextures
	{
		GLuint count;
		GLsizei n; const GLuint *textures;
	}glDeleteTextures;

	struct DepthFunc
	{
		GLuint count;
		GLenum func;
	}glDepthFunc;

	struct DepthMask
	{
		GLuint count;
		GLboolean flag;
	}glDepthMask;

	struct DepthRangef
	{
		GLuint count;
		GLclampf zNear; GLclampf zFar;
	}glDepthRangef;

	struct DepthRangex
	{
		GLuint count;
		GLclampx zNear; GLclampx zFar;
	}glDepthRangex;

	struct Disable
	{
		GLuint count;
		GLenum cap;
	}glDisable;

	struct DisableClientState
	{
		GLuint count;
		GLenum array;
	}glDisableClientState;

	struct DrawArrays
	{
		GLuint count;
		GLenum mode; GLint first; GLsizei counter;
	}glDrawArrays;

	struct DrawElements
	{
		GLuint count;
		GLenum mode; GLsizei counter; GLenum type; const GLvoid* indices;
	}glDrawElements;

	struct Enable
	{
		GLuint count;
		GLenum cap;
	}glEnable;

	struct EnableClientState
	{
		GLuint count;
		GLenum array;
	}glEnableClientState;

	struct Finish
	{
		GLuint count;
	}glFinish;

	struct Flush
	{
		GLuint count;
	}glFlush;

	struct Fogf
	{
		GLuint count;
		GLenum pname; GLfloat param;
	}glFogf;
	
	struct Fogfv
	{
		GLuint count;
		GLenum pname; const GLfloat *params;
	}glFogfv;

	struct Fogx
	{
		GLuint count;
		GLenum pname; GLfixed param;
	}glFogx;

	struct Fogxv
	{
		GLuint count;
		GLenum pname; const GLfixed *params;
	}glFogxv;

	struct FrontFace
	{
		GLuint count;
		GLenum mode;
	}glFrontFace;

	struct Frustumf
	{
		GLuint count;
		GLfloat left, right, bottom, top, zNear, zFar;
	}glFrustumf;

	struct Frustumx
	{
		GLuint count;
		GLfixed left, right, bottom, top, zNear, zFar;
	}glFrustumx;

	struct GenTextures
	{
		GLuint count;
		GLsizei n;
		GLuint *textures;
	}glGenTextures;

	struct GetError
	{
		GLuint count;
	}glGetError;

	struct GetIntegerv
	{
		GLuint count;
		GLenum pname; GLint *params;
	}glGetIntegerv;

	struct GetString
	{
		GLuint count;
		GLenum name;
	}glGetString;

	struct Hint
	{
		GLuint count;
		GLenum target, mode;
	}glHint;

	struct LightModelf
	{
		GLuint count;
		GLenum pname; GLfloat param;
	}glLightModelf;

	struct LightModelfv
	{
		GLuint count;
		GLenum pname; const GLfloat* params;
	}glLightModelfv;

	struct LightModelx
	{
		GLuint count;
		GLenum pname; GLfixed param;
	}glLightModelx;

	struct LightModelxv
	{
		GLuint count;
		GLenum pname; const GLfixed* params;
	}glLightModelxv;

	struct Lightf
	{
		GLuint count;
		GLenum light; GLenum pname; GLfloat param;
	}glLightf;
	
	struct Lightfv
	{
		GLuint count;
		GLenum light; GLenum pname; const GLfloat *params;
	}glLightfv;

	struct Lightx
	{
		GLuint count;
		GLenum light, pname; GLfixed param;
	}glLightx;

	struct Lightxv
	{
		GLuint count;
		GLenum light, pname; const GLfixed *param;
	}glLightxv;

	struct LineWidth
	{
		GLuint count;
		GLfloat width;
	}glLineWidth;

	struct LineWidthx
	{
		GLuint count;
		GLfixed width;
	}glLineWidthx;

	struct LoadIdentity
	{
		GLuint count;
	}glLoadIdentity;

	struct LoadMatrixf
	{
		GLuint count;
		const GLfloat *m;
	}glLoadMatrixf;

	struct LoadMatrixx
	{
		GLuint count;
		const GLfixed *m;
	}glLoadMatrixx;

	struct LogicOp
	{
		GLuint count;
		GLenum opcode;
	}glLogicOp;

	struct Materialf
	{
		GLuint count;
		GLenum face, pname; GLfloat param;
	}glMaterialf;

	struct Materialfv
	{
		GLuint count;
		GLenum face, pname; const GLfloat *params;
	}glMaterialfv;

	struct Materialx
	{
		GLuint count;
		GLenum face, pname; GLfixed param;
	}glMaterialx;

	struct Materialxv
	{
		GLuint count;
		GLenum face, pname; const GLfixed *param;
	}glMaterialxv;

	struct MatrixMode
	{
		GLuint count;
		GLenum mode;
	}glMatrixMode;

	struct MultMatrixf
	{
		GLuint count;
		const GLfloat *m;
	}glMultMatrixf;

	struct MultMatrixx
	{
		GLuint count;
		const GLfixed *m;
	}glMultMatrixx;

	struct MultiTexCoord4f
	{
		GLuint count;
		GLenum target;
		GLfloat s, t, r, g;
	}glMultiTexCoord4f;

	struct MultiTexCoord4x
	{
		GLuint count;
		GLenum target;
		GLfixed s,t,r,g;
	}glMultiTexCoord4x;

	struct Normal3f
	{
		GLuint count;
		GLfloat nx, ny, nz;
	}glNormal3f;

	struct Normal3x
	{
		GLuint count;
		GLfixed nx ,ny, nz;
	}glNormal3x;

	struct NormalPointer
	{
		GLuint count;
		GLenum type; GLsizei stride; const GLvoid *pointer;
	}glNormalPointer;

	struct Orthof
	{
		GLuint count;
		GLfloat left, right, bottom, top, zNear, zFar;
	}glOrthof;

	struct Orthox
	{
		GLuint count;
		GLfixed left, right, bottom, top, zNear, zFar;
	}glOrthox;

	struct PixelStorei
	{
		GLuint count;
		GLenum pname; GLint param;
	}glPixelStorei;

	struct PointSize
	{
		GLuint count;
		GLfloat size;
	}glPointSize;

	struct PointSizex
	{
		GLuint count;
		GLfixed size;
	}glPointSizex;

	struct PolygonOffset
	{
		GLuint count;
		GLfloat factor;
		GLfloat units;
	}glPolygonOffset;

	struct PolygonOffsetx
	{
		GLuint count;
		GLfixed factor;
		GLfixed units;
	}glPolygonOffsetx;

	struct PopMatrix
	{
		GLuint count;
	}glPopMatrix;

	struct PushMatrix
	{
		GLuint count;
	}glPushMatrix;

	struct ReadPixels
	{
		GLuint count;
		GLint x, y; GLsizei width, height;
		GLenum format, type;
		GLvoid *pixels;
	}glReadPixels;
	
	struct Rotatef
	{
		GLuint count;
		GLfloat angle, x, y, z;
	}glRotatef;

	struct Rotatex
	{
		GLuint count;
		GLfixed angle, x, y, z;
	}glRotatex;

	struct SampleCoverage
	{
		GLuint count;
		GLclampf value; GLboolean invert;
	}glSampleCoverage;

	struct SampleConveragex
	{
		GLuint count;
		GLclampx value; GLboolean invert;
	}glSampleConveragex;

	struct Scalef
	{
		GLuint count;
		GLfloat x, y, z;
	}glScalef;

	struct Scalex
	{
		GLuint count;
		GLfixed x, y, z;
	}glScalex;

	struct Scissor
	{
		GLuint count;
		GLint x, y;
		GLsizei width, height;
	}glScissor;

	struct ShadeModel
	{
		GLuint count;
		GLenum mode;
	}glShadeModel;

	struct StencilFunc
	{
		GLuint count;
		GLenum func;
		GLint ref;
		GLuint mask;
	}glStencilFunc;

	struct StencilMask
	{
		GLuint count;
		GLuint mask;
	}glStencilMask;

	struct StencilOp
	{
		GLuint count;
		GLenum fail, zfail, zpass;
	}glStencilOp;

	struct TexCoordPointer
	{
		GLuint count;
		GLint size; GLenum type; GLsizei stride; const GLvoid *pointer;
	}glTexCoordPointer;

	struct TexEnvf
	{
		GLuint count;
		GLenum target, pname;
		GLfloat param;
	}glTexEnvf;

	struct TexEnvfv
	{
		GLuint count;
		GLenum target, pname;
		const GLfloat* params;
	}glTexEnvfv;

	struct TexEnvx
	{
		GLuint count;
		GLenum target, pname;
		GLfixed param;
	}glTexEnvx;

	struct TexEnvxv
	{
		GLuint count;
		GLenum target, pname;
		const GLfixed* params;
	}glTexEnvxv;

	struct TexImage2D
	{
		GLuint count;
		GLenum target; GLint level; GLint internalformat; GLsizei width, height;
		GLint border; GLenum format; GLenum type; const GLvoid* pixels;
	}glTexImage2D;

	struct TexParameterf
	{
		GLuint count;
		GLenum target, pname;
		GLfloat param;
	}glTexParameterf;

	struct TexParameterx
	{
		GLuint count;
		GLenum target, pname;
		GLfixed param;
	}glTexParameterx;
	
	struct TexSubImage2D
	{
		GLuint count;
		GLenum target;
		GLint level;
		GLint xoffset, yoffset;
		GLsizei width, height;
		GLenum format;
		GLenum type;
		const GLvoid *pixels;
	}glTexSubImage2D;

	struct Translatef
	{
		GLuint count;
		GLfloat x, y, z;
	}glTranslatef;

	struct Translatex
	{
		GLuint count;
		GLfixed x, y, z;
	}glTranslatex;

	struct VertexPointer
	{
		GLuint count;
		GLint size;
		GLenum type;
		GLsizei stride;
		const GLvoid *pointer;
	}glVertexPointer;

	struct Viewport
	{
		GLuint count;
		GLint x, y;
		GLsizei width, height;
	}glViewport;
};

struct c_performance
{
	//Network variable!!
	char* Ip_address;
	GLint port_number;

	//Switch for each checking operation!!
	GLboolean Enable;
	GLboolean general_Enable;
	GLboolean time_Enable;
	GLboolean pri_Enable;
	GLboolean pre_f_Enable;
	GLboolean bf_Enable;
	GLboolean history_func_Enable;
	
	//Frame record
	GLint frame_count;

	//Record # of Primitive
	GLuint tri_count;
	GLuint real_tri_count;
	
	GLuint point_count;
	GLuint real_point_count;

	GLuint line_count;
	GLuint real_line_count;
	
	GLuint pixel_count;
	GLuint real_pixel_count;

	//Record Processed time
	GLfloat total_time;
	GLfloat vertex_shader_time;
	GLfloat lightting_time;
	GLfloat pixel_shader_time;

	//Utility Rate of each Pre-fragments
	GLbitfield scissor_discard;
	GLbitfield alpha_discard;
	GLbitfield stencil_discard;
	GLbitfield depth_discard;
	
	//R/W Operation for each buffer
	GLuint color_bf_r;
	GLuint color_bf_w;
	GLuint depth_bf_r;
	GLuint depth_bf_w;
	GLuint stencil_bf_r;
	GLuint stencil_bf_w;

	//2006/04/23 Add new measurments
	GLuint Screen_size;
	GLuint local_memory;
	GLuint funs[106];

	//2006/05/23 Add new measurments
	GLuint DA_calls;
	GLuint DE_calls;
	GLuint DP_array[100];
	GLuint tri_count_per_DP;

	//struct funs_history history;

	//**********************************
	//             GPPT GM
	//**********************************
	
	GLuint GM_Ooriginal_triangle_count; 
	GLuint GM_triangle_count;
	GLuint GM_memory_read_count;
	GLuint TD_read_count;
	GLuint TD_write_count;
 	GLuint GM_lighting_count;
	GLuint GM_cycle_count;
	//GLuint TD_memory_space_count;

	//**********************************
	//             GPPT RM
	//**********************************
	GLuint RM_cycle_count;
	GLuint RM_bus_read_count;
	GLuint RM_bus_write_count;
	GLuint RM_prefragment_pixel_count; //GLuint pixel_count;
	GLuint RM_postfragment_pixel_count;
	GLuint RM_processed_triangle_count;
	GLuint RM_processed_tile_count;


};
	//---------Texture mmp for HW-----
struct texture_mmp_hw
	{
		GLint tex_id;
		GLfixed *tex_add;
		GLint tex_width;
		GLint tex_height;
		
	};
struct gen_texture_hw
	{
	GLsizei gen_num;
	GLuint *gen_add;
	};
struct context
{
	//*********************************
	//     Hadware Base Address
	//*********************************
	int fd;
  	
	unsigned int hw_offset;
	struct cmd_request cmd;
	unsigned int cmd_num;

	int frame_NO;
	//int frame;
	int object_frame_NO;
	int no_write;

	//the switch buffer pointer
	GLuint* H_Input_pointer;
	//buffer address
	GLuint* A_Input_pointer;
	GLuint* B_Input_pointer;

	//**********************************
	//             GPPT
	//**********************************
	GLuint* himax_gppt_address;
#if DUMP_FRAME_BUFFER
    unsigned int *frame_buffer;
#endif
	//*********************************
	//     Transformation
	//*********************************
	
	//Define matrix stack
	struct matrix_stack ModelviewMatrixStack;
	struct matrix_stack ProjectionMatrixStack;
	struct matrix_stack *CurrentStack;
	// matrix_stack ColorMatrixStack;
	// matrix_stack TextureMatrixStack[MAX_TEXTURE_COORD_UNITS];
	// matrix_stack ProgramMatrixStack[MAX_PROGRAM_MATRICES];
	
	//Combined modelview and projection matrix
	struct GLmatrix _ModelProjectMatrix;

	//Define view port
	struct viewport_attrib Viewport;
	
	//**********************************
	//    Performance check variable
	//**********************************
	struct c_performance c_perfo;

	
	//**********************************
	//     Define Vertex Params
	//**********************************
	GLint CurrentVert;
	GLboolean TriangleStrip_count;
	GLenum Primitive_type;
	struct VertexParams Verts[3];
	
	//*********************************
	//     Color & Lighting
	//*********************************
	GLenum ShadeMode;
	GLboolean color_matrial_flag;
	GLboolean reNormalize_flag;
	GLboolean reScaleNormal_flag;
	
	struct current_Color           currentcolor;
	struct current_Normal          current_normal;
	
	struct gl_light_attrib         Light;
	struct gl_material_attrib      Material;
	
	//*********************************
	//          Vertex Arrays
	//*********************************
	struct array_pointer Vertexpointer;
	struct array_pointer Colorpointer;
	struct array_pointer Normalpointer;
	struct array_pointer TexCoordPointer;

	struct draw_array DrawArray;
	struct draw_element DrawElement;
	
	//**********************************
	//          Rasterization
	//**********************************
	GLfixed PointSize;
	GLboolean PointSize_flag;
	GLboolean CullFace_flag;
	GLenum FrontFace_enum;
	GLenum CullFace_enum;

	//**********************************
	//       ClientState flag!!
	//**********************************
	GLboolean gl_color_array_flag;
	GLboolean gl_normal_array_flag;
	GLboolean gl_vertex_array_flag;
	GLboolean gl_texture_coord_array_flag;

	//Buffer Mask
	struct Buffer_mask           buffer_mask;
	
	//Clear Buffer Value
	struct Clear_buffer_value   init_buffer_value;

	int buffer_size_x;
	int buffer_size_y;

	//*********************************
	//     Per-Fragment Operation
	//*********************************
	GLboolean scissor_flag;
	GLboolean alpha_flag;
	GLboolean stencil_flag;
	GLboolean depth_flag;
	GLboolean Blending_flag;
	GLboolean LogOp_flag;

	//Scissor testing variable
	GLint scissor_x, scissor_y;
	GLsizei scissor_width, scissor_height;

	//Alpha testing variable
	GLclampx alpha_ref;
	GLenum   alpha_fun;

	//Stencil testing variable
	  /* glStencilFunc */
	  GLenum   stencil_fun;
	  GLint    stencil_ref;
	  GLuint   stencil_mask;

	  /* glStencilOp */
	  GLenum   stencil_fail;
	  GLenum   stencil_zfail;
	  GLenum   stencil_zpass;

	//Depth testing variable
	GLenum   depth_fun;

	//Blending variable
	GLenum   blend_src_fact;
	GLenum   blend_dst_fact;

	//LogOp variable
	GLenum   LogOp_op;

	//*************************
	//  GL State Query
	//*************************
	GLenum Error_flag;                  /*!< Error report flag.*/

	//*********************************
	//     Texture Operation
	//*********************************
	struct Texture **Textures;            /*!< Arrays of textures */
	unsigned int NumTextures;             /*!< # OF TEXTURES ALLOCATED IN Textures (ONLY GROWS)*/
	unsigned int iCurrentBoundTexture;    /*!< INDEX INTO Textures INDICATING CURRENT TEXTURE BEING USED */
	struct Texture_unit Texture_units[2]; /*!< Arrays of texture units. Becuful! Although we set\
                                                   two unit in here but we don't got to use the second unit */
	int iCurrentBoundUnit;                /*!< current bound texture unit */
	unsigned int FreeNumTextures;
	//*********************************
	//     Fog Operation
	//*********************************
	GLboolean fog_flag;
	GLfixed fog_param_mode;

	GLfixed fog_density;
	GLfixed fog_start;
	GLfixed fog_end;
	GLfixed fog_color[4];
	
	//---------Texture mmp for HW-----
	struct texture_mmp_hw tex_mmp_hw;
	struct gen_texture_hw gen_texture;

	
	
	
}mycontext; // structure mycontext end

#endif // _CONTEXT_H
/*End of CONTEXT_H*/
