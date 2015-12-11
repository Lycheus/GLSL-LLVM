//
//Copyright (C) 2002-2005  3Dlabs Inc. Ltd.
//All rights reserved.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions
//are met:
//
//    Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
//    Redistributions in binary form must reproduce the above
//    copyright notice, this list of conditions and the following
//    disclaimer in the documentation and/or other materials provided
//    with the distribution.
//
//    Neither the name of 3Dlabs Inc. Ltd. nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
//FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
//COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
//BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
//CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
//LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
//ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//POSSIBILITY OF SUCH DAMAGE.
//

#ifndef _BASICTYPES_INCLUDED_
#define _BASICTYPES_INCLUDED_
#define OpenGLES
//
// Precision qualifiers
//
enum TPrecision {
    EbpHigh,
    EbpMedium,
    EbpLow,
};

__inline const char* getPrecisionString(TPrecision p)
{
    switch (p) {
    case EbpHigh:		return "highp";		break;
      //case EbpMedium:		return "medimp";	break;//Kaymon. Now Treating medium precision is same as high precision.
    case EbpMedium:		return "highp";	break;
      //case EbpLow:		return "lowp";		break;//Kaymon. Now Treating lowp precision is same as high precision.
    case EbpLow:		return "highp";		break;
    default:			return "unknown precision";
    }
}


//
// Basic type.  Arrays, vectors, etc., are orthogonal to this.
//
enum TBasicType {
    EbtVoid,
    EbtFloat,
    EbtInt,
    EbtBool,
    EbtGuardSamplerBegin,  // non type:  see implementation of IsSampler()
    EbtSampler2D,
    EbtSamplerCube,
    EbtGuardSamplerEnd,    // non type:  see implementation of IsSampler()
    EbtStruct,
    EbtAddress,            // should be deprecated??
};

__inline bool IsSampler(TBasicType type)
{
    return type > EbtGuardSamplerBegin && type < EbtGuardSamplerEnd;
}

//
// Qualifiers and built-ins.  These are mainly used to see what can be read
// or written, and by the machine dependent translator to know which registers
// to allocate variables in.  Since built-ins tend to go to different registers
// than varying or uniform, it makes sense they are peers, not sub-classes.
//
enum TQualifier {
    EvqTemporary,     // For temporaries (within a function), read/write
    EvqGlobal,        // For globals read/write
    EvqConst,         // User defined constants and non-output parameters in functions
    EvqAttribute,     // Readonly
    EvqVaryingIn,     // readonly, fragment shaders only
    EvqVaryingOut,    // vertex shaders only  read/write
    EvqInvariantVaryingIn,     // readonly, fragment shaders only
    EvqInvariantVaryingOut,    // vertex shaders only  read/write
    EvqUniform,       // Readonly, vertex and fragment

    // pack/unpack input and output
    EvqInput,
    EvqOutput,

    // parameters
    EvqIn,
    EvqOut,
    EvqInOut,
    EvqConstReadOnly,

    // built-ins written by vertex shader
    EvqPosition,
    EvqPointSize,
#ifndef OpenGLES
    EvqClipVertex,
#else
#endif
    // built-ins read by fragment shader
    EvqFace,
    EvqFragCoord,

    // built-ins written by fragment shader
    EvqFragColor,
#ifndef OpenGLES
    EvqFragDepth,
#else
#endif
    

    // end of list
    EvqLast,
};

//
// This is just for debug print out, carried along with the definitions above.
//
__inline const char* getQualifierString(TQualifier q)
{
    switch (q) {
    case EvqTemporary:		return "Temporary";      break;
    case EvqGlobal:		return "Global";         break;
    case EvqConst:		return "const";          break;
    case EvqConstReadOnly:	return "const";          break;
    case EvqAttribute:		return "attribute";      break;
    case EvqVaryingIn:		return "varying";        break;
    case EvqVaryingOut:		return "varying";        break;
    case EvqInvariantVaryingIn:	return "invariant varying";	break;
    case EvqInvariantVaryingOut:return "invariant varying";	break;
    case EvqUniform:		return "uniform";        break;
    case EvqIn:			return "in";             break;
    case EvqOut:		return "out";            break;
    case EvqInOut:		return "inout";          break;
    case EvqInput:		return "input";          break;
    case EvqOutput:		return "output";         break;
    case EvqPosition:		return "Position";       break;
    case EvqPointSize:		return "PointSize";      break;
#ifndef OpenGLES
    case EvqClipVertex:		return "ClipVertex";     break;
#else
#endif
    case EvqFace:		return "Face";           break;
    case EvqFragCoord:		return "FragCoord";      break;
    case EvqFragColor:		return "FragColor";      break;
#ifndef OpenGLES
    case EvqFragDepth:		return "FragDepth";      break;
#else
#endif
    default:			return "unknown qualifier";
    }
}


//dondum
//This is just supporting short word to let linker know it in IR.
//
__inline const char* getQualifierShortString(TQualifier q)
{
    switch (q) {
    case EvqAttribute:		return "a";      break;
    case EvqVaryingIn:		return "v";        break;
    case EvqVaryingOut:		return "v";        break;
    case EvqInvariantVaryingIn:	return "invariant varying";	break;
    case EvqInvariantVaryingOut:return "invariant varying";	break;
    case EvqUniform:		return "u";        break;
    default:			return "";
    }
}

#endif // _BASICTYPES_INCLUDED_
