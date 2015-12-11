/**********************************************************
    Laboratory   : CIIS Lab.
    File name    : _LIGHTING_C
    Author       : KengYu Lin, ShouYi Ju
    Create date  : 11/01/2005
    Note         :
    Modifications:
**********************************************************/
#include "lighting.h"
#include <math.h>


void CalcLighting(GLfixed* FinalVertexColor, const GLfixed* p, const GLfixed* n,
                  struct gl_material_attrib ColorMaterials[1], const GLint IsBackfacing)
{
  //Get the current context
  struct context *ctx = &mycontext;

  GLfixed   AttenuationFactor, SpotlightEffect, dist, dotprod;
  GLfixed   TempVec[3], Vertex2Viewer[3], Vertex2Light[3], Light2Vertex[3], TempRGB[4], N[3];
  GLint     i, j, faceindex;

  // CLEAR THE VERTEX COLOR
  FinalVertexColor[0] = FinalVertexColor[1] = FinalVertexColor[2] = FinalVertexColor[3] = 0;

  // DETERMINE WHETHER WE ARE USING FRONT OR BACK FACE MATERIAL
  if (ctx->Light.Model.TwoSide && IsBackfacing) {
    N[0] = -n[0]; N[1] = -n[1]; N[2] = -n[2];     // REVERSE THE NORMAL
    //faceindex = 1;
  } else {
    N[0] = n[0]; N[1] = n[1]; N[2] = n[2];
    //faceindex = 0;
  }

  // DETERMINE THE LIGHT MODEL (LOCAL OR DISTANT?)
  Vertex2Viewer[0] = Vertex2Viewer[1] = 0; Vertex2Viewer[2] = 65536;

  // PROCESS THE COLORING THAT IS INDEPENDENT OF INDIVIDUAL LIGHTS
  if (ctx->color_matrial_flag == GL_TRUE)
  {
      FinalVertexColor[0] += ctx->Material.Emission[0];                                      // MATERIAL EMISSION
      FinalVertexColor[1] += ctx->Material.Emission[1];
      FinalVertexColor[2] += ctx->Material.Emission[2];
      FinalVertexColor[3] += ctx->Material.Emission[3];

      FinalVertexColor[0] += Mulfx2(ctx->currentcolor.red   , ctx->Light.Model.Ambient[0]); // SCALED GLOBAL AMBIENT LIGHT
      FinalVertexColor[1] += Mulfx2(ctx->currentcolor.green , ctx->Light.Model.Ambient[1]);
      FinalVertexColor[2] += Mulfx2(ctx->currentcolor.blue  , ctx->Light.Model.Ambient[2]);
      FinalVertexColor[3] += Mulfx2(ctx->currentcolor.alpha , ctx->Light.Model.Ambient[3]);
  }else{
    for (j = 0; j < 4; j++) {
      FinalVertexColor[j] += ctx->Material.Emission[j];                                      // MATERIAL EMISSION
      FinalVertexColor[j] += Mulfx2(ctx->Material.Ambient[j] , ctx->Light.Model.Ambient[j]); // SCALED GLOBAL AMBIENT LIGHT
    }
  }

  // LOOP THROUGH EACH LIGHT
  for (i = 0; i < MAX_NUM_LIGHTS; i++) {

    if (ctx->Light.light_i[i].Enabled == GL_TRUE) {

      TempRGB[0] = TempRGB[1] = TempRGB[2] = TempRGB[3] = 0;      // RESET THE RGB FOR THIS LIGHT

      // SPOTLIGHT CONTRIBUTION
      //if (Lights[i].SpotCutoff == 180)                            // NOT A SPOTLIGHT
      if (ctx->Light.light_i[i].SpotCutoff == 11796480)
	SpotlightEffect = 65536;
      else {
	Light2Vertex[0] = p[0] - ctx->Light.light_i[i].EyePosition[0];       // VECTOR FROM LIGHT -> VERTEX
	Light2Vertex[1] = p[1] - ctx->Light.light_i[i].EyePosition[1];
	Light2Vertex[2] = p[2] - ctx->Light.light_i[i].EyePosition[2];

	// DENOMINATOR FOR NORMALIZATION
	dist = ftofx(sqrt( fxtof(Mulfx2(Light2Vertex[0], Light2Vertex[0]) + \
	                         Mulfx2(Light2Vertex[1], Light2Vertex[1]) + \
		                 Mulfx2(Light2Vertex[2], Light2Vertex[2]) )  ) );

	// UNIT VECTOR NORMALIZATION
	Light2Vertex[0] = Divfx(Light2Vertex[0] , dist);
	Light2Vertex[1] = Divfx(Light2Vertex[1] , dist);
	Light2Vertex[2] = Divfx(Light2Vertex[2] , dist);
	
	dotprod = ( Mulfx2(Light2Vertex[0], ctx->Light.light_i[i].SpotDirection[0]) + \
		    Mulfx2(Light2Vertex[1], ctx->Light.light_i[i].SpotDirection[1]) + \
		    Mulfx2(Light2Vertex[2], ctx->Light.light_i[i].SpotDirection[2]) );
	if (dotprod < 0) dotprod = 0;

	// DETERMINES IF THE POINT IN IN THE CONE OF LIGHT
	SpotlightEffect = (fxtof(dotprod) < cos(fxtof(PI)* fxtof(ctx->Light.light_i[i].SpotCutoff)/180.0f)) ?\
	                   0 : ftofx(pow(fxtof(dotprod), fxtof(ctx->Light.light_i[i].SpotExponent)));
      }

      // IF THE VERTEX IS NOT IN THE SPOTLIGHT'S CONE THEN SKIP THE REST
      if (SpotlightEffect) {

	// THE DISTANCE BETWEEN LIGHT AND VERTEX
   dist = ftofx(sqrt( fxtof(ctx->Light.light_i[i].EyePosition[0]-p[0])*fxtof(ctx->Light.light_i[i].EyePosition[0]-p[0]) + \
		      fxtof(ctx->Light.light_i[i].EyePosition[1]-p[1])*fxtof(ctx->Light.light_i[i].EyePosition[1]-p[1]) + \
		      fxtof(ctx->Light.light_i[i].EyePosition[2]-p[2])*fxtof(ctx->Light.light_i[i].EyePosition[2]-p[2]) ));

	// ATTENUATION FACTOR FOR THE LIGHT, 1 IF DIRECTIONAL LIGHT
	AttenuationFactor = (ctx->Light.light_i[i].EyePosition[3] == 0) ? \
			     65536 : Divfx(65536 , (ctx->Light.light_i[i].ConstantAttenuation + \
				                    Mulfx2(ctx->Light.light_i[i].LinearAttenuation, dist) + \
				                    Mulfx2(ctx->Light.light_i[i].QuadraticAttenuation, Mulfx2(dist,dist))));
				
	if (ctx->color_matrial_flag == GL_TRUE){
	  // AMBIENT TERM, AMBIENT COLOR OF THE LIGHT * AMBIENT COLOR OF CURRENT COLOR
	  TempRGB[0] += Mulfx2(ctx->currentcolor.red  , ctx->Light.light_i[i].Ambient[0]);
	  TempRGB[1] += Mulfx2(ctx->currentcolor.green, ctx->Light.light_i[i].Ambient[1]);
	  TempRGB[2] += Mulfx2(ctx->currentcolor.blue , ctx->Light.light_i[i].Ambient[2]);
	}else{
	  // AMBIENT TERM, AMBIENT COLOR OF THE LIGHT * AMBIENT COLOR OF MATERIAL
	  TempRGB[0] += Mulfx2(ctx->Material.Ambient[0], ctx->Light.light_i[i].Ambient[0]);
	  TempRGB[1] += Mulfx2(ctx->Material.Ambient[1], ctx->Light.light_i[i].Ambient[1]);
	  TempRGB[2] += Mulfx2(ctx->Material.Ambient[2], ctx->Light.light_i[i].Ambient[2]);
	}

	  // DIFFUSE TERM, TAKES INTO ACCOUNT WHETHER THE LIGHT FALLS RIGHT ON THE SURFACE OR NOT
	  Vertex2Light[0] = ctx->Light.light_i[i].EyePosition[0] - p[0];         // VECTOR FROM VERTEX -> LIGHT
	  Vertex2Light[1] = ctx->Light.light_i[i].EyePosition[1] - p[1];
	  Vertex2Light[2] = ctx->Light.light_i[i].EyePosition[2] - p[2];

	  // DENOMINATOR FOR NORMALIZATION
	  dist = ftofx(sqrt( fxtof(Vertex2Light[0])*fxtof(Vertex2Light[0]) + 
	                     fxtof(Vertex2Light[1])*fxtof(Vertex2Light[1]) + 
			     fxtof(Vertex2Light[2])*fxtof(Vertex2Light[2]) ) );

	  // UNIT VECTOR NORMALIZATION
	  Vertex2Light[0] = ftofx(fxtof(Vertex2Light[0])/fxtof(dist));
	  Vertex2Light[1] = ftofx(fxtof(Vertex2Light[1])/fxtof(dist));
	  Vertex2Light[2] = ftofx(fxtof(Vertex2Light[2])/fxtof(dist));

	  dotprod = ftofx( fxtof(Vertex2Light[0])* fxtof(N[0]) +\
	                   fxtof(Vertex2Light[1])* fxtof(N[1]) +\
	                   fxtof(Vertex2Light[2])* fxtof(N[2]) ) ;

	  if (dotprod > 0) {
	    if (ctx->color_matrial_flag == GL_TRUE){
	      // ADD IN THE DIFFUSE TERM
	      TempRGB[0] += Mulfx2(ctx->currentcolor.red  , Mulfx2(ctx->Light.light_i[i].Diffuse[0], dotprod));
	      TempRGB[1] += Mulfx2(ctx->currentcolor.green, Mulfx2(ctx->Light.light_i[i].Diffuse[1], dotprod));
	      TempRGB[2] += Mulfx2(ctx->currentcolor.blue , Mulfx2(ctx->Light.light_i[i].Diffuse[2], dotprod));
	    }else{
	      TempRGB[0] += Mulfx2(ctx->Material.Diffuse[0], Mulfx2(ctx->Light.light_i[i].Diffuse[0], dotprod));
	      TempRGB[1] += Mulfx2(ctx->Material.Diffuse[1], Mulfx2(ctx->Light.light_i[i].Diffuse[1], dotprod));
	      TempRGB[2] += Mulfx2(ctx->Material.Diffuse[2], Mulfx2(ctx->Light.light_i[i].Diffuse[2], dotprod));
	    }

	    // SPECULAR TERM, TAKES INTO ACCOUNT WHETHER THE LIGHT FALLS RIGHT ON THE SURFACE OR NOT
	    if (ctx->Material.Shininess == 0)
	    {
	      dist = 65536;                                           // FINAL TERM WE RAISE TO POWER OF SHININESS
	    }else{
	      // SUM 2 UNIT VECTORS, VERTEX -> LIGHT, VERTEX -> VIEWPOINT
	      TempVec[0] = Vertex2Light[0] + Vertex2Viewer[0];
	      TempVec[1] = Vertex2Light[1] + Vertex2Viewer[1];
	      TempVec[2] = Vertex2Light[2] + Vertex2Viewer[2];

	      // DENOMINATOR FOR NORMALIZATION
	      dist = ftofx(sqrt( fxtof(TempVec[0])* fxtof(TempVec[0]) +\
	                         fxtof(TempVec[1])* fxtof(TempVec[1]) +\
			         fxtof(TempVec[2])* fxtof(TempVec[2]) ) );

	      TempVec[0] = Divfx(TempVec[0], dist);
	      TempVec[1] = Divfx(TempVec[1], dist);
	      TempVec[2] = Divfx(TempVec[2], dist); // UNIT VECTOR NORMALIZATION

	      dotprod = (Mulfx2(TempVec[0], N[0]) + Mulfx2(TempVec[1], N[1]) + Mulfx2(TempVec[2], N[2]));

	      if (dotprod < 0) dotprod = 0;

	      dist = ftofx(pow(fxtof(dotprod), fxtof(ctx->Material.Shininess)));  // USE dist AS TEMP VARIABLE
	    } // IF SHININESS == 0

	    // ADD THE SPECULAR CONTRIBUTION
	    TempRGB[0] += Mulfx2(ctx->Material.Specular[0], Mulfx2(ctx->Light.light_i[i].Specular[0], dist));
	    TempRGB[1] += Mulfx2(ctx->Material.Specular[1], Mulfx2(ctx->Light.light_i[i].Specular[1], dist));
	    TempRGB[2] += Mulfx2(ctx->Material.Specular[2], Mulfx2(ctx->Light.light_i[i].Specular[2], dist));

	  } // DOTPROD > 0
      } // SPOTLIGHT EFFECT

      // ADD IN THE CONTRIBUTION FROM THE CURRENT LIGHT SOURCE
      FinalVertexColor[0] += Mulfx2(AttenuationFactor, Mulfx2(SpotlightEffect, TempRGB[0]));
      FinalVertexColor[1] += Mulfx2(AttenuationFactor, Mulfx2(SpotlightEffect, TempRGB[1]));
      FinalVertexColor[2] += Mulfx2(AttenuationFactor, Mulfx2(SpotlightEffect, TempRGB[2]));

    }  // LIGHT ENABLED
  }  // NUM LIGHTS

  // CLAMP COLOR VALUES [0,1]
  for (i = 0; i < 4; i++)
  {
    if (FinalVertexColor[i] > 65536) FinalVertexColor[i] = 65536;

    //Turning precision of color for each color of vertex!!!
    //Can mark this code to inprove performance!!
    float temp=0;
    if( (fxtof(FinalVertexColor[i]) * 100) - ((int)(fxtof(FinalVertexColor[i]) * 100)) >= 0.5f  )
    {
	temp = (fxtof(FinalVertexColor[i]) * 100)+1;
    }else{
	temp = (fxtof(FinalVertexColor[i]) * 100);
    }
    FinalVertexColor[i] = ftofx((GLfloat)((GLint)(temp)) / 100);
  }
}
