#ifndef FRAGMENT_H
#define FRAGMENT_H


/**** test circuit parameter ****/
//for <zcompare func> , <stencil_func>
#define NEVER    0
#define ALWAYS   1
#define LESS     2
#define LEQUAL   3
#define EQUAL    4
#define GEQUAL   5
#define GREATER  6
#define NOTEQUAL 7

/**** stencil operation type ****/
//for <fail> , <zfail> , <zpass>
#define KEEP      0
#define ZERO      1
#define REPLACE   2
#define INCR      3
#define DECR      4
#define INVERT    5
#define INCR_WRAP 6
#define DECR_WRAP 7

/**** factor selector parameter ****/
//for <blending_srcfunc> , <blending_dstfunc>
#define BLEND_ZERO                  0
#define BLEND_ONE                   1
#define BLEND_SRCCOLOR              2
#define BLEND_ONEMINUSSRCCOLOR      3
#define BLEND_DSTCOLOR              4
#define BLEND_ONEMINUSDSTCOLOR      5
#define BLEND_SRCALPHA              6
#define BLEND_ONEMINUSSRCALPHA      7
#define BLEND_DSTALPHA              8
#define BLEND_ONEMINUSDSTALPHA      9
#define BLEND_CONSTANTCOLOR         10//not implement
#define BLEND_ONEMINUSCONSTANTCOLOR 11//not implement
#define BLEND_CONSTANTALPHA         12//not implement
#define BLEND_ONEMINUSCONSTANTALPHA 13//not implement
#define BLEND_SRCALPHASATURATE      14


//----------------------------------------------------
struct F_info
{
    unsigned int color;	
	unsigned int x,y,z;
};


struct F_FIFO
{
    int           full,empty;
	int           R_ptr,W_ptr;
	unsigned int  counter;
	struct F_info list[7];//FS max num = 7
};


struct FRAGMENT
{
    /**** z compare parameters ****/
 	int           zcompare_enable;
 	unsigned int  zcompare_func;
 	/**** stencil test parameters ****/
 	int           stencil_enable;
    unsigned int  stencil_func;
    unsigned int  stencil_ref,stencil_mask;
    unsigned int  fail,zfail,zpass;
 	/**** blending parameters ****/
 	int           blending_enable;
 	unsigned int  blending_srcfunc,blending_dstfunc;    
	//------------------------
	int           state;//0: IDLE , 1: BUSY
	int           penalty_count;
	//------------------------
	struct F_FIFO *fifo;
	struct F_info *out_pool;
}; 


#endif
