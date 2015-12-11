/*
 * frag_define.h
 *
 *  Created on: 2009/12/1
 *      Author: Jason
 */

#ifndef FRAG_DEFINE_H
#define FRAG_DEFINE_H

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

#define NEVER_BITS    "000"
#define ALWAYS_BITS   "001"
#define LESS_BITS     "010"
#define LEQUAL_BITS   "011"
#define EQUAL_BITS    "100"
#define GEQUAL_BITS   "101"
#define GREATER_BITS  "110"
#define NOTEQUAL_BITS "111"

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

#define KEEP_BITS      "000"
#define ZERO_BITS      "001"
#define REPLACE_BITS   "010"
#define INCR_BITS      "011"
#define DECR_BITS      "100"
#define INVERT_BITS    "101"
#define INCR_WRAP_BITS "110"
#define DECR_WRAP_BITS "111"


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


#define BLEND_ZERO_BITS                  "0000"
#define BLEND_ONE_BITS                   "0001"
#define BLEND_SRCCOLOR_BITS              "0010"
#define BLEND_ONEMINUSSRCCOLOR_BITS      "0011"
#define BLEND_DSTCOLOR_BITS              "0100"
#define BLEND_ONEMINUSDSTCOLOR_BITS      "0101"
#define BLEND_SRCALPHA_BITS              "0110"
#define BLEND_ONEMINUSSRCALPHA_BITS      "0111"
#define BLEND_DSTALPHA_BITS              "1000"
#define BLEND_ONEMINUSDSTALPHA_BITS      "1001"
#define BLEND_CONSTANTCOLOR_BITS         "1010"//not implement
#define BLEND_ONEMINUSCONSTANTCOLOR_BITS "1011"//not implement
#define BLEND_CONSTANTALPHA_BITS         "1100"//not implement
#define BLEND_ONEMINUSCONSTANTALPHA_BITS "1101"//not implement
#define BLEND_SRCALPHASATURATE_BITS      "1110"

//add by sheng-chih

#endif /* FRAG_DEFINE_H */
