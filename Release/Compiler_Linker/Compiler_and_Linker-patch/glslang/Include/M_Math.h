/**********************************************************
    Laboratory   : CIIS Lab.
    File name    : _M_MATH_H
    Author       : ShouYi Ju
    Create date  : 07/15/2005
    Note         :
    Modifications:
**********************************************************/
#ifndef _M_MATH_H
#define _M_MATH_H

/*INCLUDE HEADER FILE SECTION*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "fx_Math.h"
#include "context.h"

/*LOCAL MACRO DECLARATIONS */
#define FX_ONE (1<<16)
#define FX_TWO (1<<17)
#define MAT_FLAG_ROTATION       0x2 
#define A(row,col)  a[(col<<2)+row]
#define B(row,col)  b[(col<<2)+row]
#define P(row,col)  product[(col<<2)+row]
#define M(row,col)  m[col*4+row]

/*DATA STRUCTURE DECLARATIONS*/
static GLfixed Identity[16] = {
   FX_ONE, 0, 0, 0,
   0, FX_ONE, 0,0,
   0, 0, FX_ONE, 0,
   0, 0, 0, FX_ONE
}; // structure Identity end

/*EXPORTED SUBPROGRAM PROTOTYPE SECTION*/
extern void matrix_multfx44(GLfixed *product, GLfixed *a, GLfixed *b);

extern void matrix_multfx44_vector3(GLfixed* product, GLfixed* a, GLfixed* b);
extern void matrix_multfx_vector(GLfixed *product, GLfixed *a, GLfixed *b);

extern void matrix_transp_16(GLfixed* M);

extern void TransformNormalIntoEyeCoords(GLfixed* NewN, GLfixed* M, GLfixed* N);

#endif // _M_MATH_H
/*End of _M_MATH_H*/