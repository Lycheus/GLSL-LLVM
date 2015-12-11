/**********************************************************
    Laboratory   : CIIS Lab.
    File name    : _RASTER_H
    Author       : KengYu Lin, ShouYi Ju
    Create date  : 07/01/2005
    Note         :
    Modifications:
**********************************************************/
#ifndef _FX_MATH_H
#define _FX_MATH_H

/*INCLUDE HEADER FILE SECTION*/
#include "gl.h"
#include "type.h"

/*LOCAL MACRO DECLARATIONS */
#define FracBit 16
#define itofx(x) (x<<FracBit)			      // Int to fixed_point
#define fxtoi(x) (x>>FracBit)			      // fixed to int point
#define ftofx(x) (GLfixed)((x) * (1<<FracBit))	      // Float to fixed_point
#define fxtof(x) ((float) (x) / (1<<FracBit))         // Fixed_point to float

// Multiply a fixed by a fixed

//old
//#define Mulfx(x,y)    ( (((x)>>2)*((y)>>2)) >>12 )
//#define Mulfx2(x,y)   ( ( ((x)>>8) * ((y)>>8) ) )

//version 1.38
#define Mulfx(x,y) (int)(((long long )x * (long long)y) >> 16)
#define Mulfx2(x,y) (int)(((long long )x * (long long)y) >> 16)

// Divide a fixed by a fixed

//old
//#define Divfx(x,y)  (((x<< 10) / y) <<6 ) 
//#define Divfx2(x,y) (((x<<8) / y)<<8  )
//#define Divfx3(x,y) (((x<<4) / y)<<12 )

//version 1.38
#define Divfx(x,y) (int)(y==0?0:(((long long)x << 16) / y))
#define Divfx2(x,y) (int)(y==0?0:(((long long)x << 16) / y))
#define Divfx3(x,y) (int)(y==0?0:(((long long)x << 16) / y))



// Squrt a fixed
#define Sqrtfx(x) ftofx(sqrt(fxtof(x)))
// Sin
#define Sinfx(x) ftofx(sin(fxtof(x)))
// Cos
#define Cosfx(x) ftofx(cos(fxtof(x)))
// SWAP operation

#define SWAP(a,b)                \
        do{                      \
            a^=b;b^=a;a^=b;      \
        }while(0)                \

#define step(shift)                                     \
    if((0x40000000l >> shift) + root <= value)          \
    {                                                   \
        value -= (0x40000000l >> shift) + root;         \
        root = (root >> 1) | (0x40000000l >> shift);    \
    }                                                   \
    else                                                \
    {                                                   \
        root = root >> 1;                               \
    }


#define MIN(x,y) x>=y?y:x
#define MAX(x,y) x>=y?x:y

#define fx_floor(value) ((value>>FracBit)<<FracBit)

/*EXPORTED SUBPROGRAM PROTOTYPE SECTION*/
extern GLfixed SIN(int);           //Define SIN function
extern GLfixed COS(int);           //Define COS function
extern GLfixed fxSqrt(GLfixed);    //Define fxSqrt function

//Vector Operation
extern struct vector v_Cross(const struct vector *P, const struct vector *Q);

#endif // _FX_MATH_H
/*End of FX_MATH_H*/
