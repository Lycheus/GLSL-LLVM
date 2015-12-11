/**********************************************************
    Laboratory   : CIIS Lab.
    File name    : _LIGHTING_H
    Author       : KengYu Lin, ShouYi Ju
    Create date  : 11/01/2005
    Note         :
    Modifications:
**********************************************************/
#ifndef _LIGHTING_H
#define _LIGHTING_H

/*INCLUDE HEADER FILE SECTION*/
#include "gl.h"
#include "context.h"
#include "fx_Math.h"
#include "M_Math.h"

/*LOCAL MACRO DECLARATIONS */
#define PI 205887  //Fixed formate

/*EXPORTED SUBPROGRAM PROTOTYPE SECTION*/
void CalcLighting(GLfixed* FinalVertexColor, const GLfixed* p, const GLfixed* n,
                  struct gl_material_attrib ColorMaterials[1], const GLint IsBackfacing);

#endif _LIGHTING_H
//End of LIGHTING_H