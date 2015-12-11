/*
 * Auther: CoolDavid <cooldavid@cooldavid.org>
 * Copyrite (C) 2008 OSLab CSE NSYSU
 * All rights reserved.
 *
 * This file holds the LLVM code generator class declaration
 *
 */

#include "llvmVar.h"
#include "myfloat.h" //dondum 2010-07-26

TllvmVar::TllvmVar(const TString& n,
		   const TType& t,
		   const TllvmVarType lvt)
{
    type = t;
    name = n;
    vartype = lvt;
}

TllvmVar::TllvmVar(bool b) : type(EbtBool),
			     vartype(TllvmVarConstant)
{
    constbool = b;
    isb.clear();
    if(b)
	isb << 1;
    else
	isb << 0;
    name = isb.c_str();
}

TllvmVar::TllvmVar(int i) : type(EbtInt),
			    vartype(TllvmVarConstant)
{
    constint = i;
    isb.clear();
    isb << i;
    name = isb.c_str();
}

TllvmVar::TllvmVar(float f) : type(EbtFloat),
			      vartype(TllvmVarConstant)
{
  char str[256];              //dondum 2010-07-26
  TransBinFloat((double)f,str);//dondum 2010-07-26
    constfloat = f;
    isb.clear();
    isb << f;
    //name = isb.c_str();
    name = str;       //dondum 2010-07-26
    //setName();
    //printf("s: %s\n",name);
}

TllvmVar::TllvmVar(const vector<bool> &bvec) : type(EbtBool,
						    EvqTemporary,
						    bvec.size()),
					       vartype(TllvmVarConstant)
{
    int i, n;
    int size=bvec.size();

    constbvec = bvec;
    isb.clear();
    isb << "<i1 ";
    n = (bvec[0])?1:0;
    isb << n;
    for(i=1;i<size;++i) {
	isb << ", i1 ";
	n = (bvec[i])?1:0;
	isb << n;
    }
    isb << ">";
    name = isb.c_str();
}

TllvmVar::TllvmVar(const vector<int> &ivec) : type(EbtInt,
						   EvqTemporary,
						   ivec.size()),
					      vartype(TllvmVarConstant)
{
    int i;
    int size=ivec.size();

    constivec = ivec;
    isb.clear();
    isb << "<i32 " << ivec[0];
    for(i=1;i<size;++i)
	isb << ", i32 " << ivec[i];
    isb << ">";
    name = isb.c_str();
}

TllvmVar::TllvmVar(const vector<float> &fvec) : type(EbtFloat,
						     EvqTemporary,
						     fvec.size()),
						vartype(TllvmVarConstant)
{
    int i;
    int size=fvec.size();

    constfvec = fvec;
    isb.clear();
    isb << "<float " << fvec[0];
    for(i=1;i<size;++i)
	isb << ", float " << fvec[i];
    isb << ">";
    name = isb.c_str();
}

TllvmVar::TllvmVar(const vector< vector<float> > &mat) : type(EbtFloat,
							      EvqTemporary,
							      mat.size(), true),
							 vartype(TllvmVarConstant)
{
    int i, j;
    int size=mat.size();

    constmat = mat;
    isb.clear();
    isb << "<";
    for(i=0;i<size;++i) {
	for(j=0;j<size;++j) {
	    if(i!=0 || j!=0)
		isb << ", ";
	    isb << "float " << mat[i][j];
	}
    }
    isb << ">";
    name = isb.c_str();
}

TllvmVar::TllvmVar(const TllvmVarList &cs,
		   const TType &t)          : type(t),
					      vartype(TllvmVarConstant)
{
    int i;
    int size=cs.size();

    conststruct = cs;

    isb.clear();
    isb << "{";
    for(i=0;i<size;++i) {
	if(i!=0)
	    isb << ", ";
	isb << cs[i].getType().getCompleteString() << " " << cs[i].getName();
    }
    isb << "}";
    name = isb.c_str();
}

TllvmVar::TllvmVar(const vector<int> &swi,
		   const TType &t)          : type(t),
					      vartype(TllvmVarSwizzle)
{
    name = "::Swizzle::";
    swizzle = swi;
}

int TllvmVar::getFirstNonAggPos(void) const {
    int firstNonAgg = indexlist.size();
    if(firstNonAgg<=0)
	return 0;

    while(
	  firstNonAgg - 1 >= 0 &&
	  indexlist[firstNonAgg-1].type.isNonAgg()
	 )
	--firstNonAgg;

    return firstNonAgg;
}
