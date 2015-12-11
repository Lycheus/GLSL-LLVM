/**********************************************************
    Laboratory   : CIIS Lab.
    File name    : _RASTER_H
    Author       : KengYu Lin, ShouYi Ju
    Create date  : 07/01/2005
    Note         :
    Modifications:
**********************************************************/
#ifndef _CONTEXT_H
#define _CONTEXT_H

/*INCLUDE HEADER FILE SECTION*/
//fixed by sheng-chih
//#include "gl.h"
//#include "sct_table.h"
#include "APIGl.h"
//#include "type.h"
//#include "time.h"
//#include "Built-in_Variables.h"
#include <stdio.h> //GLtrace
//#include "gl_texture.h"
//#include "Register.h"
typedef unsigned char GLboolean;
typedef int GLint;
typedef void GLvoid;
typedef int GLfixed;
typedef GLfixed vec4[4];
//end fixed

/*LOCAL MACRO DECLARATIONS */
#define GETCURRENT_CTX(x) (struct context* x = &mycontext;)
#define MAX_NUM_LIGHTS 8
#define MAX_LEVEL_MM 10
#define vertex_shader 0
#define fragment_shader 1



//*******************************
//     Define VertexParams
//*******************************
//!structure to define the vertex parameters
/*!
   contains view coordination, texture coordination, normal vector and color
 */
struct VertexParams
{
  GLfixed EyeCoord[4];   //x,y,z,w coord.
  GLfixed Color[4];      //r,g,b,a color of vertex
  GLfixed Normal[3];     //x,y,z normal coord.
  GLfixed TexCoord[4];   //tx, ty, tz tex coords
};

struct cmd_request {
  int type; // Command type
  union {
    struct {
      unsigned int attribute_offset; // Object data offset in attribute buffer
      unsigned int active_VS_uniform;
      unsigned int active_FS_uniform;
      unsigned int cmd_num; // Command number
      char VS[9*4]; // Vertex Shader registers
      char FS[22*4]; // Fragment Shader registers
    }hw;
    unsigned long clear_color; // Color of clear buffer
    unsigned long clear_depth; // Depth of clear depth
  }u;  
};

/*! \file context.h
    \brief The Context-table definition.
 */

//*********************************************************
//****                Transform                        ****
//*********************************************************


//!Define the 4x4 matrix struct.
/*!
    The Model-view, Projection and Texture matrix are also use this 4x4 matrix struct.
 */
struct GLmatrix{
   GLfixed m[16];                 // 16 matrix elements (16-byte aligned)
   GLfixed inv[16];	          // optional 16-element inverse (16-byte aligned)
   GLuint flags;                  // possible values determined by (of \link * MatFlags MAT_FLAG_* flags\endlink)
   GLenum matrixType;
}; //structure GLmatrix end



//!Define the matrix stack
/*!
   There are three matrix stack in OpenGL ES specification.
   - Model-view matrix stack: 24
   - Projecction matrix stack: 4
   - Texture matrix stack: 3
 */
struct matrix_stack
{
   struct GLmatrix *Top;                // points into Stack
   struct GLmatrix *Stack;              // array [MaxDepth] of GLmatrix
   GLuint Depth;                         // 0 <= Depth < MaxDepth
   GLuint MaxDepth;                      // size of Stack[] array
   //GLuint DirtyFlag;           // _NEW_MODELVIEW or _NEW_PROJECTION, for example
};// structure matrix_stack end


//!Keep the viewport parameters in this struct.
/*!
   The viewport's x, y, width, height, near, far and set the viewport matrix are kept in this struct.
 */
struct viewport_attrib {
   GLint X, Y;                          // position
   GLsizei Width, Height;               // size
   GLfixed Near, Far;                   // Depth buffer range
   struct GLmatrix _WindowMap;        // Mapping transformation as a matrix.
}; // structure view_port_attrib end



//********************************************************
//****                Color & Lighting                ****
//********************************************************

//! Keep the current color in this struct.
struct current_Color {
	GLfixed red;
	GLfixed green;
	GLfixed blue;
	GLfixed alpha;
};


//! Keep current normal in this struct.
struct current_Normal {
	GLfixed nx;
	GLfixed ny;
	GLfixed nz;
};


//! Keep light parameters in this struct
struct gl_light {
	struct gl_light *next;          /*!< *next is to double linked list with sentinel.*/
	struct gl_light *prev;          /*!< *prev is to double linked list with sentinel.*/

	GLfixed Ambient[4];              /*!< Ambient[4] is ambient color.*/
	GLfixed Diffuse[4];              /*!< Diffuse[4] is diffuse color.*/
	GLfixed Specular[4];             /*!< Specular[4] is Specular color.*/
	GLfixed EyePosition[4];          /*!< EyePosition[4] is position in eye coordinates.*/
	GLfixed SpotDirection[3];        /*!< SpotDirection[3] is spotlight direction in eye coordinates.*/

	GLfixed SpotExponent;            /*!< SpotExponent is spot light parameter.*/
	GLfixed SpotCutoff;              /*!< SpotExponent is spot light parameter.*/

	GLfixed ConstantAttenuation;     /*!< ConstantAttenuation.*/
	GLfixed LinearAttenuation;       /*!< LinearAttenuation.*/
	GLfixed QuadraticAttenuation;    /*!< QuadraticAttenuation.*/
	GLboolean Enabled;               /*!< Enabled is On/off flag.*/
};


//! Keep the lighting model(glLightModel) in this struct.
struct gl_lightmodel{
	GLfixed Ambient[4];             /*!< Ambient[4] is ambient color.*/
	GLboolean TwoSide;              /*!< TwoSide is Two (or one) sided lighting. */
};


//! Keep the total light model in this struct.
struct gl_light_attrib {
	GLboolean Enabled;                          /*!< Enabled is enable/disable all lighting. */
	GLenum ShadeModel;                          /*!< ShadeModel represent shadeing technique. */

	struct gl_light light_i[MAX_NUM_LIGHTS];  /*!< light_i[MAX_NUM_LIGHTS] is Array of lights.*/
	struct gl_lightmodel Model;               /*!< Model is lighting mode.*/
};


//! Keep the matrial's lighting parameters in this struct.
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


//! Keep the parameters of Vertex Arrays.
struct array_pointer {
	GLint    size;          /*!< size: Specifies the number of coordinates per vertex.*/
	GLenum   type;          /*!< type: Specifies the data type of each vertex coordinate in the array.*/
	GLsizei  stride;        /*!< stride: Specifies the byte offset between consecutive vertices.*/
	const GLvoid  *ptr;    /*!< *ptr: Specifies a pointer to the first coordinate of\
                          the first vertex in the array. */
	GLsizei  elementSize;
};


//! Keep the parameters of glDrawArrays.
struct draw_array{
	GLenum   mode;           /*!< mode: Specifies what kind of primitives to render.*/
	GLint    first;          /*!< first: Specifies the starting index in the enabled arrays.*/
	GLsizei  count;          /*!< count: Specifies the number of indices to be rendered.*/
};


//! Keep the parameter of glDrawElements.
struct draw_element{
	GLenum   mode;           /*!< mode: Specifies what kind of primitives to render.*/
	GLsizei  count;          /*!< count: Specifies the number of elements to be rendered.*/
	GLenum   type;           /*!< type: Specifies the type of the values in indices.*/
	const GLvoid* indices;  /*!< *indices: Specifies a pointer to the location where the indices are stored.*/
};

//********************************************************
//********************************************************


//! Keep the color, depth, stencil value for clear buffer operation.
struct Clear_buffer_value
{
	//initial value of Color buffer
	GLclampx red;
	GLclampx green;
	GLclampx blue;
	GLclampx alpha;

	//initial value of Depth buffer
	GLclampx depth;                   /*!< depth: Specifies the initial value of Depth buffer.*/

	//initial value of Stencil buffer
	GLint s;                          /*!< s: Specifies the initial value of Stencil buffer.*/
}; // structure Clear_buffer_value end


//! Keep the color, depth, stencil mask for writing buffer components.
struct Buffer_mask
{
	GLboolean mask_R;
	GLboolean mask_G;
	GLboolean mask_B;
	GLboolean mask_A;

	GLboolean mask_D;       /*!< mask_D: Specifies the depth buffer parameter.*/
	GLuint    mask_S;       /*!< mask_S: Specifies the Scissor buffer parameter.*/
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


//*******************************************************
//       Define the shader object and program object used in CreateShader
//*******************************************************
struct varyingobject{
	vec4 varying[3];
};

#if 1 //define in sct_table.h
typedef struct uniformobject{
	int programnumber;

	char *name;//-------------compiler---------------
	GLint location;//-------------compiler---------------
	GLint sizei;//-------------compiler---------------
	GLint offset;//-------------compiler---------------
	float *data_ptr;
        char IsSampler2D;  //Kaymon 
        char IsSamplerCube;  //Kaymon 
        char IsNormal;  //Kaymon 
	GLint doublename;

} UniformObject;


typedef struct shaderobject{
  char *type;             //! define shader type
  int count;              //! shader count
  int count_file;
  //const FILE *pointer;    //! reference the .c file
  GLboolean DeletePending;          // add by Robarter@20121120
  GLboolean CompileStatus;          // add by Robarter@20121120
  const char *pointer;              // = "Source" in mesa
  GLchar *InfoLog;                  // add by Robarter@20121120
  unsigned int shader_length;//-------------compiler---------------
  struct shaderobject *next;//-------------compiler---------------
} ShaderObjectList;

typedef struct programobject{
	unsigned int programobject_count;

	struct shaderobject *shaders[2];//shader[0] is for vertex shader,shader[1] is for fragment shader

	UniformObject uniform[256];
	unsigned int vertex_uniform_number;//-------------compiler---------------
	unsigned int fragment_uniform_number;//-------------compiler---------------
	
	GLboolean LinkerStatus; // add by north@20121213 
	vec4 attribute_data[8];
	char *attribute_name[8];
	GLboolean attribute_flag[8];
	float *attribute_data_ptr[8];
         GLint x, y;
	 GLsizei width, height;
} ProgramObject;
#endif

typedef struct bufferobject{
	unsigned int bufferobject_count;
	int bufferobject_size;
	GLvoid *bufferArray;
	GLboolean binded;
	GLenum target;
	GLenum usage;
} BufferObject;

typedef struct framebufferobject{
	unsigned int framebufferobject_count;
	int framebufferobject_size;
	int *framebufferArray;
	GLboolean framebufferbinded;
} FrameBufferObject;

typedef struct renderbufferobject{
	unsigned int renderbufferobject_count;
	int renderbufferobject_size;
	int *renderbufferArray;
	GLboolean renderbufferbinded;
} RenderBufferObject;

//! Keep the parameters of AttribVertex Arrays.

struct Attrib_array_pointer {
	GLint    size;          /*!< size: Specifies the number of coordinates per vertex.*/
	GLenum   type;          /*!< type: Specifies the data type of each vertex coordinate in the array.*/
	GLsizei  stride;        /*!< stride: Specifies the byte offset between consecutive vertices.*/
	const GLvoid  *ptr;    /*!< *ptr: Specifies a pointer to the first coordinate of\
                          the first vertex in the array. */
	GLboolean normalized;
	GLsizei  elementSize;
	const GLvoid *attribute_data_ptr;
};


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

/*	#ifdef clocktest
	//2008/05/28 Add new measurments
	GLlong GM_time;
	GLlong light_time;
	GLlong RM_time;
	GLlong all_time;
        GLlong forall_time
	#endif*/
	//struct funs_history history;
};


//!    Saving all parameters which are used by rendering pipeline.
/*!
 *     Block function define from which glxxx function.
 */
	//---------Problem Texture mmp for HW-----
struct texture_mmp_hw
	{
		GLint tex_id;
		GLfixed *tex_add;
		GLint tex_width;
		GLint tex_height;

	};
struct gen_texture_struct
	{
	GLsizei gen_num;
	GLuint *gen_add;
	};
#ifndef COMPILER_STANDALONE
struct context
{
	int object_frame_NO;
//----Texture mmp for hw-----
	int fd;
	unsigned int hw_offset;
	struct cmd_request cmd;
//---------------------------
#if HARDWARE_MODE
	//the switch buffer pointer
	/*
	GLuint* Attribute_H_Input_pointer;
	GLuint* VSuniform_H_Input_pointer;
	GLuint* FSuniform_H_Input_pointer;
	*/

	//angry fix by ESLAB
	float* Attribute_H_Input_pointer;
	float* VSuniform_H_Input_pointer;
	float* FSuniform_H_Input_pointer;

	//buffer address
	float* Attribute_A_Input_pointer;
	float* VSuniform_A_Input_pointer;
	float* FSuniform_A_Input_pointer;

	float* Attribute_B_Input_pointer;
	float* VSuniform_B_Input_pointer;
	float* FSuniform_B_Input_pointer;
#endif
//------------------------------------
	//*********************************
	//     Transformation
	//*********************************

	//Define matrix stack
	struct matrix_stack ModelviewMatrixStack;      /*!< Modelview Matrix Stack.*/
	struct matrix_stack ProjectionMatrixStack;     /*!< Projection Matrix Stack.*/
	struct matrix_stack *CurrentStack;             /*!< Point to surrent matrix stack.*/
	// matrix_stack ColorMatrixStack;
	// matrix_stack TextureMatrixStack[MAX_TEXTURE_COORD_UNITS];
	// matrix_stack ProgramMatrixStack[MAX_PROGRAM_MATRICES];

	//Combined modelview and projection matrix
	struct GLmatrix _ModelProjectMatrix;           /*!< Combine the modelview and projection matrix. (non-use)*/

	//Define view port
	struct viewport_attrib Viewport;               /*!< Structure variable of viewport_attrib.*/

	//**********************************
	//    Performance check variable
	//**********************************
	struct c_performance c_perfo;

	//**********************************
	//     Define Vertex Params
	//**********************************
	GLint CurrentVert;                                 /*!<  A counter of vertex which are used by rendering primitive.*/
	GLboolean TriangleStrip_count;                     /*!<  A flag of triangle strip which are used by \
                                                     switching the order of vertex.*/
	GLenum Primitive_type;                             /*!<  Record the Primitive type.*/
	struct VertexParams Verts[3];                     /*!<  A buffer of vertexs.*/


	//*********************************
	//     Shader
	//*********************************
	struct GLmatrix MVP;
	struct Attrib_array_pointer AttribVertexpointer[8];
	GLfixed HomoVertex[12];                            /*!< used in vertex shader */
	GLfixed Color[12];
	GLfixed vPosition[9];
	GLfixed vNormal[9];
	GLfixed Texture[12];
	GLfixed texture;

	struct varyingobject varying[8];
	struct shaderobject shader[100];
	struct programobject program[50];
	struct bufferobject buffer[1000];
	struct framebufferobject framebuffer[100];
	struct renderbufferobject renderbuffer[100];
	int bindedBufferNow;
	int bindedRenderbufferNow;
	int bindedFramebufferNow;
	int Bind_count;
	int shader_count;
	int program_count;
	int buffer_count;
	int framebuffer_count;
	int renderbuffer_count;
	int CurrentProg;                                  //!Enable which programs should be used
	//int gl_Position;

	GLfixed s;
	GLfixed t;


	//*********************************
	//     Color & Lighting
	//*********************************
	GLenum ShadeMode;                                   /*!<  Switch the ShadeMode of primitive.*/
	GLboolean color_matrial_flag;                       /*!<  Enable and Disable the color matrial operation.*/
	GLboolean reNormalize_flag;                         /*!<  Enable and Disable the Re-Normalize operation.*/
	GLboolean reScaleNormal_flag;                       /*!<  Enable and Disable the Re-ScaleNormal operation.*/

	struct current_Color           currentcolor;       /*!<  Record the current color value.*/
	struct current_Normal          current_normal;     /*!<  Record the current normal coord.*/

	struct gl_light_attrib         Light;              /*!<  Structure variable of gl_light_attrib.*/
	struct gl_material_attrib      Material;           /*!<  Structure variable of gl_material_attrib.*/

	//*********************************
	//          Vertex Arrays
	//*********************************
	struct array_pointer Vertexpointer;                /*!<  Structure variable of array_pointer for Vertex pointer.*/
	struct array_pointer Colorpointer;                 /*!<  Structure variable of array_pointer for Color pointer.*/
	struct array_pointer Normalpointer;                /*!<  Structure variable of array_pointer for Normal pointer.*/
	struct array_pointer TexCoordPointer;              /*!<  Structure variable of array_pointer for\
                                                                 texture coords pointer.*/

	struct draw_array DrawArray;                       /*!<  Structure variable of draw_array.*/
	struct draw_element DrawElement;                   /*!<  Structure variable of draw_element.*/

	//**********************************
	//          Rasterization
	//**********************************
	GLfixed PointSize;                    /*!<  Record the point size which is assigned by programmer.*/
	GLboolean PointSize_flag;             /*!<  Enable and Disable the PointSize operation.*/
	GLboolean CullFace_flag;              /*!<  Enable and Disable the culling operation.*/
	GLenum FrontFace_enum;                /*!<  Set the clockwise or counterclockwise to be the front face.*/
	GLenum CullFace_enum;                 /*!<  Set which face (front, back, or front-and-back) are discarded.*/

	//**********************************
	//       ClientState flag!!
	//**********************************
	GLboolean gl_color_array_flag;                      /*!<  Enable and Disable the Color pointer operation.*/
	GLboolean gl_normal_array_flag;                     /*!<  Enable and Disable the Normal pointer operation.*/
	GLboolean gl_vertex_array_flag;                     /*!<  Enable and Disable the Vertex pointer operation.*/
	GLboolean gl_texture_coord_array_flag;              /*!<  Enable and Disable the Texture pointer operation.*/

	//Buffer Mask
	struct Buffer_mask           buffer_mask;          /*!< Structure variable of Buffer_mask.*/

	//Clear Buffer Value
	struct Clear_buffer_value   init_buffer_value;     /*!< Structure variable of clear_buffer_value.*/

	int buffer_size_x;                                  /*!< Record the buffer size.*/
	int buffer_size_y;                                  /*!< Record the buffer size.*/

	//*********************************
	//     Pixel Operation
	//*********************************
	GLint  pixel_store_op_param_pack;
	GLint  pixel_store_op_param_unpack;

	//*********************************
	//     Per-Fragment Operation
	//*********************************
	GLboolean scissor_flag;             /*!<  Enable and Disable the Scissor operation.*/
	GLboolean alpha_flag;               /*!<  Enable and Disable the Alpha testing operation.*/
	GLboolean stencil_flag;             /*!<  Enable and Disable the Stencil operation.*/
	GLboolean depth_flag;               /*!<  Enable and Disable the Depth testing operation.*/
	GLboolean Blending_flag;            /*!<  Enable and Disable the Blending operation.*/
	GLboolean LogOp_flag;               /*!<  Enable and Disable the Logical operation.*/
	GLboolean multisample_flag;		/*!<  Enable and Disable the Anti_aliasing operation.*/

	//Scissor testing variable setting
	GLint scissor_x,                    /*!<  Specify the lower left corner of the scissor box, in pixels..*/
              scissor_y;                    /*!<  Specify the lower left corner of the scissor box, in pixels..*/
	GLsizei scissor_width,              /*!<  Specify the width of the scissor box..*/
                scissor_height;             /*!<  Specify the height of the scissor box..*/

	//Alpha testing variable
	GLclampx alpha_ref;                 /*!< Specifies the reference value that incoming alpha values\
                                        are compared to.*/
	GLenum   alpha_fun;                 /*!< Specifies the alpha comparison function.*/

	//Stencil testing variable
	  /* glStencilFunc */
	  GLenum   stencil_fun;             /*!<  Specifies the test function. Eight tokens are valid:\
                                         GL_NEVER, GL_LESS, GL_LEQUAL, GL_GREATER, GL_GEQUAL,\
                                         GL_EQUAL, GL_NOTEQUAL, and GL_ALWAYS.*/
	  GLint    stencil_ref;             /*!<  Specifies the reference value for the stencil test.  \
                                         ref is clamped to the range [ 0 , 2n - 1 ] , where n \
                                         is the number of bitplanes in the stencil buffer.*/
	  GLuint   stencil_mask;            /*!<  Specifies a mask that is ANDed with both the reference\
                                         value and the stored stencil value when the test is done.*/

	  /* glStencilOp */
	  GLenum   stencil_fail;            /*!<  Specifies the action to take when the stencil test fails.*/
	  GLenum   stencil_zfail;           /*!<  Specifies the stencil action when the stencil test passes,\
                                         but the depth test fails. */
	  GLenum   stencil_zpass;           /*!<  Specifies the stencil action when both the stencil test and\
                                         the depth test pass, or when the stencil test passes and either\
                                         there is no depth buffer or depth testing is not enabled. */

	//Depth testing variable
	/* glDepthFunc */
	GLenum   depth_fun;                 /*!< Specifies the depth comparison function. */

	//Blending variable
	/* glBlendFunc */
	//
	//GLenum   blend_src_fact;            /*!< Specifies how the red, green, blue, and alpha source blending\
    //                                             factors are computed. .*/
	//GLenum   blend_dst_fact;            /*!< Specifies how the red, green, blue, and alpha destination \
    //                                             blending factors are computed.*/


    /* glBlendFunc, glBlendFuncSeparate */
    /* Note: those parameters are added by removing "GLenum blend_src_fact"
            and "GLenum blend_dst_fact", because BlendFuncSeparate uses
            the same parameters:
                blend_src_fact => blend_src_RGB and blend_src_Alpha(just same value)
                blend_dst_fact => blend_dst_RGB and blend_dst_Alpha(just same value)
     */
    /**
        {blend_src_RGB}
        Specifies how the red, green, and blue blending factors are computed.
        Add by Robarter, 2012/08/28
    */
    GLenum blend_src_RGB;
    /**
        {blend_dst_RGB}
        Specifies how the red, green, and blue destination blending factors are computed.
        Add by Robarter, 2012/08/28
    */
    GLenum blend_dst_RGB;
    /**
        {blend_src_Alpha}
        Specified how the alpha source blending factor is computed.
        Add by Robarter, 2012/08/28
    */
    GLenum blend_src_Alpha;
    /**
        {blend_dst_Alpha}
        Specified how the alpha destination blending factor is computed.
        Add by Robarter, 2012/08/28
    */
    GLenum blend_dst_Alpha;

    /**
        {blend_constant_color_red, blend_constant_color_green,
         blend_constant_color_blue, blend_constant_color_alpha}
        specify the components of GL_BLEND_COLOR
        Add by Robarter, 2012/08/26
    */
    GLclampf blend_constant_color_red;
    GLclampf blend_constant_color_green;
    GLclampf blend_constant_color_blue;
    GLclampf blend_constant_color_alpha;

    /**
        {blend_equation_mode_rgb, blend_equation_mode_alpha)
        specify the components of glBlendEquation, glBlendEquationSeparate
        Add by Robarter, 2012/08/28
    */
    GLenum blend_equation_mode_rgb;
    GLenum blend_equation_mode_alpha;

	//LogOp variable
	/* glLogicOp */
	GLenum   LogOp_op;                  /*!< Specifies a symbolic constant that selects a logical operation.*/

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

	//*********************************
	//     Fog Operation
	//*********************************
	GLboolean fog_flag;
	GLfixed fog_param_mode;

	GLfixed fog_density;
	GLfixed fog_start;
	GLfixed fog_end;
	GLfixed fog_color[4];

	//*********************************
	//     Line Operation
	//*********************************
	GLfixed Line_width;
	GLboolean LINE_STIPPLE;
	GLint factor;
	GLushort pattern;

	//---------Texture mmp for HW-----
	unsigned int FreeNumTextures;
	struct texture_mmp_hw tex_mmp_hw;
	struct gen_texture_struct gen_texture_t;
	unsigned int cmd_num;
	//------------------------------------------
	//*********************************
	//     glPolygonOffset
	//*********************************
	/**
        {PointSize_flag}
        Enable and Disable fragment's depth value to be offset.
        Add by Robarter, 2012/09/09
    */
    GLboolean PolygonOffsetFill_flag;
    /**
        {polygonOffsetFactor}
        Specifies a scale factor that is used to create a variable depth offset for each polygon. The initial value is 0.
        Add by Robarter, 2012/09/09
    */
    GLfloat polygonOffsetFactor;
    /**
        {polygonOffsetUnits}
        Is multiplied by an implementation-specific value tocreate a constant depth offset. The initial value is 0.
        Add by Robarter, 2012/09/09
    */
    GLfloat polygonOffsetUnits;


    //*********************************
	//     glSampleCoverage
	//*********************************
    /**
        {sampleCoverageValue}
        Specify a single floating-point sample coverage value.  The value isclamped to the range[0,1].The initial value is 1.0.
        Add by Robarter, 2012/09/12
    */
    GLclampf sampleCoverageValue;
    /**
        {sampleCoverageInvert}
        Specify a single boolean value representing if the coverage masks should beinverted.  GL_TRUE and GL_FALSE are accepted.  The initial valueis GL_FALSE.
        Add by Robarter, 2012/09/12
    */
    GLboolean sampleCoverageInvert;
//******************************************************
/*	#ifdef clocktest
float tranxx;
float tranyy;
float tranzz;
	#endif*/
//******************************************************
}; // structure mycontext end
#else
struct context
{
  struct shaderobject shader[100];
  struct programobject program[50];
};
#endif

#endif // _CONTEXT_H
/*End of CONTEXT_H*/
