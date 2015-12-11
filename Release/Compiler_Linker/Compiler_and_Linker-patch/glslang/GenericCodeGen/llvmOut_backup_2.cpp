#include <iostream>
#define MAZE 1
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


//Auther: CoolDavid <cooldavid@cooldavid.org>
//Copyrite (C) 2008 OSLab CSE NSYSU
//All rights reserved.
//
// This file holds the LLVM code generator class definitions
// Modified from intermOut.cpp which originally written by 3Dlabs
//

/*
 * Extended LLVM data type:
 * 	<N x N of float> //To represent matrix
 *
 * Extended LLVM instruction:
 * 	<result> = extractelement <N x N of float> <val>, <2 x i32> <i32 idx_column, i32 idx_row>		; yields float
 * 	<result> = insertelement  <N x N of float> <val>, <ty> <elt>, <2 x i32> <i32 idx_column, i32 idx_row>	; yields <N x N float>
 * 	<result> = extractvector  <N x N of float> <val>, i32 <idx_column>					; yields <N x float>
 * 	<result> = insertvector   <N x N of float> <val>, <ty> <elt>, i32 <idx_column>				; yields <N x N float>
 *
 *
 * 	<result> = extractvector <n x <ty>> <val>, <m x i32> <mask>    ; yields <m x <ty>>
 * 		2 <= m <= n
 * 		<mask> := < <idx>{, <idx>}+ >
 * 		<idx> := i32 <number>
 * 		<number> := <digit>+
 * 		<digit> := [0-9]
 *
 * 		each <number> are between 0 and (n - 1)
 *
 * 	<result> = shufflevector <n x <ty>> <v1>, <m x <ty>> <v2>, <r x i32> <mask>    ; yields <r x <ty>>
 * 		2 <= r <= n+m;
 *
 * Built-in Function calls:
 */

/*
 * LLVM Code Generator TODO: Higher number do first
 *              1. SSA
 *              2. Solve the unused items in expression stack problem
 *              3. Variable Scope
 *
 * Low-Prio TODO:
 * 		1. Write a test bench that can test some
 * 		   pre-defined source and target code
 * 		2. Constant Declaration
 *
 * Known issue:
 * 		1. Pre/Post Increment/Decrement on vector/matrix will
 * 		   produce error. (Does it allowed?)
 *
 */

#include "llvmOut.h"

//dondum
//This is for changeMatrix[size].[argument].[s]()
int s =1 ;

string TllvmOutputTraverser::GetBinOp(const TOperator &op,
				      const TBasicType &tpy)
{
    string str;

    switch (op) {
	case EOpAdd:
	case EOpAddAssign:
	    str = "add";
	    break;
	case EOpSub:
	case EOpSubAssign:
	    str = "sub";
	    break;
	    return str;
	case EOpMul:
	case EOpMulAssign:
	    str = "mul";
	    break;
	case EOpDiv:
	case EOpDivAssign:
	    if(tpy == EbtFloat)
		str = "fdiv";
	    else if(tpy == EbtInt)
		str = "sdiv";
	    else
		str = "";
	    break;
	case EOpMod:
	case EOpModAssign:
	    if(tpy == EbtFloat)
		str = "frem";
	    else if(tpy == EbtInt)
		str = "srem";
	    else
		str = "";
	    break;
	case EOpAnd:
	case EOpAndAssign:
	    str = "and";
	    break;
	case EOpInclusiveOr:
	case EOpInclusiveOrAssign:
	    str = "or";
	    break;
	case EOpExclusiveOr:
	case EOpExclusiveOrAssign:
	    str = "xor";
	    break;
	case EOpLeftShift:
	case EOpLeftShiftAssign:
	    str = "shl";
	    break;
	case EOpRightShift:
	case EOpRightShiftAssign:
	    str = "lshr";
	    break;
	default:
	    str = "";
	    break;
    }

    return str;
}

string TllvmOutputTraverser::GetCompOp(const TOperator &op,
				       const TBasicType &tpy)
{
    string str;

    if(tpy == EbtInt || tpy == EbtBool) {
	str = "icmp ";
    }
    else if(tpy == EbtFloat) {
	str = "fcmp ";
    }
    else {
	str = "";
	return str;
    }

    switch(op) {
	case EOpEqual:
	    str += "eq,";
	    break;
	case EOpNotEqual:
	    str += "ne,";
	    break;
	case EOpLessThan:
	    str += "slt,";
	    break;
	case EOpGreaterThan:
	    str += "sgt,";
	    break;
	case EOpLessThanEqual:
	    str += "sle,";
	    break;
	case EOpGreaterThanEqual:
	    str += "sge,";
	    break;
	default:
	    str = "";
    }

    return str;
}


void TllvmOutputTraverser::GenBinOp(const string &op,
					   const TllvmVar &t,
					   const TllvmVar &s1,
					   const TllvmVar &s2)
{
    char buf[256];

    sprintf(buf, "\t%s = %s %s %s, %s\n",
	    t.getName().c_str(),
	    op.c_str(),
	    s1.getCompleteString().c_str(),
	    s1.getName().c_str(),
	    s2.getName().c_str());
    infoSink.code << buf;

}

void TllvmOutputTraverser::GenStore(const TllvmVar &ptr,
				    const TllvmVar &val)
{
    infoSink.code << "\tstore " << val.getCompleteString()
		  << " " << val.getName()
	          << ", " << ptr.getCompleteString()
		  << "* " << ptr.getName() << "\n";
}
//dondum///////////////////////////////////////////////////////////////////////////////////////////////////
void TllvmOutputTraverser::GenLoad(const TllvmVar &ptr,
				   const TllvmVar &val)
{
    infoSink.code << "\t" << ptr.getName()
		  << " = load " << val.getCompleteString()<<"*"
	          << " " << val.getName()<<"."<<val.getQualifierShortString()<< ", align "<< 4 * val.getType().getNominalSize() <<"\n";

}

//#if 0
void TllvmOutputTraverser::GenLoad_M(const TllvmVar &ts,
				     const TllvmVar &val,
				     const int size,const TString &MatrixName)
{
  //  std::cout<< MatrixName <<std::endl;
  //  MatrixName = '@' + MatrixName ;
	 infoSink.code << "\t" << ts.getName()
		  << " = load " << val.getCompleteString()<<"*"
		       << " " << MatrixName/*val.getName()*/<<"."<<val.getQualifierShortString()<<"."<<val.getNominalSize()<<"."<<size << ", align "<< 4 * val.getType().getNominalSize() <<"\n";
	   

}
//#endif
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void TllvmOutputTraverser::GenExtractSwizzle(const TllvmVar &src,
					     const TllvmVar &res,
					     const TType &origtype,
					     const vector<int> &swi)
{
    int i;
    int size = res.getType().getNominalSize();

    infoSink.code << "\t" << res.getName()
		  << " = extractvector " << origtype.getCompleteString()
		  << " " << src.getName() << ", "
		  << "<" << size << " x i32> <";

    for(i=0;i<size;++i) {
	if(i!=0)
	    infoSink.code << ", ";
	infoSink.code << "i32 " << swi[i];
    }

    infoSink.code << ">\n";
}

void TllvmOutputTraverser::GenExtractElement(const TllvmVar &vec,
					     const TllvmVar &res,
					     const TType &origtype,
					     const TllvmVar &idx)
{
    infoSink.code << "\t" << res.getName()
		  << " = extractelement " << origtype.getCompleteString()
	          << " " << vec.getName() << ", "
		  << "i32 " << idx.getName() << "\n";

}


void TllvmOutputTraverser::GenExtractVoM(const TllvmVar &mat,
					 const TllvmVar &resv,
					 const TType &origtype,
					 const TllvmVar &idx)
{
    infoSink.code << "\t" << resv.getName()
		  << " = extractvector " << origtype.getCompleteString()
	          << " " << mat.getName() << ", "
		  << "i32 " << idx.getName() << "\n";
}

void TllvmOutputTraverser::GenExtractEoM(const TllvmVar &mat,
					 const TllvmVar &rese,
					 const TType &origtype,
					 const TllvmVar &idxc,
					 const TllvmVar &idxr)
{
    infoSink.code << "\t" << rese.getName()
		  << " = extractelement " << origtype.getCompleteString()
	          << " " << mat.getName() << ", "
		  << "<2 x i32> <i32 " << idxc.getName()
		  << ", i32 " << idxr.getName() << ">\n";
}

bool TllvmOutputTraverser::GenExtractValue(const TllvmVar &agg,
					  const TllvmVar &res,
					  const unsigned int size,
					  const int line)
{
    TllvmVarIndexList il = agg.getIndex();
    unsigned int i;

    if(il.size()==0 || size > il.size()) {
	infoSink.info.message(EPrefixInternalError,
			      "Index of Aggregate type out of range",
			      line);
	return false;
    }

    infoSink.code << "\t" << res.getName()
		  << " = extractvalue " << il[0].type.getCompleteString()
	          << " " << agg.getName();

    for(i=0;i<size;++i)
	infoSink.code << ", " << il[i].index.getName();

    infoSink.code << "\n";

    return true;
}

bool TllvmOutputTraverser::GenExtractNonAgg(const TllvmVar &base,
					    const TllvmVar &res,
					    const TllvmVarIndexList &idx,
					    const int line)
{
    if(idx.size() == 1 &&
       idx[0].type.isVector() &&
       idx[0].index.isSwizzle())
    {
	GenExtractSwizzle(base, res, idx[0].type, idx[0].index.getSwizzle());
    }
    else if(idx.size() == 1 &&
	    idx[0].type.isVector() &&
	    idx[0].index.isSimpleConst())
    {
	GenExtractElement(base, res, idx[0].type, idx[0].index);
    }
    else if(idx.size() == 1 &&
	    idx[0].type.isMatrix() &&
	    idx[0].index.isSimpleType())
    {
	GenExtractVoM(base, res, idx[0].type, idx[0].index);
    }
    else if(idx.size() == 2 &&
	    idx[0].type.isMatrix() &&
	    idx[0].index.isSimpleType() &&
	    idx[1].index.isSimpleType())
    {
	GenExtractEoM(base, res, idx[0].type, idx[0].index, idx[1].index);
    }
    else
    {
	infoSink.info.message(EPrefixInternalError,
			      "No match operation found for index of Non-Aggregate type",
			      line);
	return false;
    }

    return true;
}

bool TllvmOutputTraverser::GenExtractAgg(const TllvmVar &agg,
					 const TllvmVar &res,
					 const int line)
{
    if(agg.getType() != res.getType()) {
	infoSink.info.message(EPrefixInternalError,
			 "Assigning to different type",
			 line);
	return false;
    }

    int size = agg.getIndex().size();
    int fng = agg.getFirstNonAggPos();

    if(fng<size) {
	TllvmVarIndexList idx;
	for(int i=fng;i<size;++i)
	    idx.push_back((agg.getIndex())[i]);

	if(fng>0) {
	    TllvmVar tv(allocate_tempvar().c_str(),
			(agg.getIndex())[fng].type);
	    return
		GenExtractValue(agg, tv, fng, line) &&
		GenExtractNonAgg(tv, res, idx, line);
	}
	else
	    return GenExtractNonAgg(agg, res, idx, line);
    }
    else
	return GenExtractValue(agg, res, size, line);

}

void TllvmOutputTraverser::GenInsertSwizzle(const TllvmVar &base,
					    const TllvmVar &val,
					    const TType &origtype,
					    const vector<int> &swi)
{
    int i;
    int size = val.getType().getNominalSize();
    int oldsize = origtype.getNominalSize();
    vector<int> nswi(oldsize);

    for(i=0;i<oldsize;++i)
	nswi[i] = i;

    for(i=0;i<size;++i)
	nswi[swi[i]] = i+oldsize;

    infoSink.code << "\t" << base.getName()
		  << " = shufflevector "
		  << origtype.getCompleteString()
		  << " " << base.getName()
		  << ", " << val.getCompleteString()
		  << " " << val.getName() << ", "
		  << "<" << oldsize << " x i32> <";

    for(i=0;i<oldsize;++i) {
	if(i!=0)
	    infoSink.code << ", ";
	infoSink.code << "i32 " << nswi[i];
    }

    infoSink.code << ">\n";
}

void TllvmOutputTraverser::GenInsertElement(const TllvmVar &vec,
					    const TllvmVar &val,
					    const TType &origtype,
					    const TllvmVar &idx)
{
    infoSink.code << "\t" << vec.getName()
		  << " = insertelement " << origtype.getCompleteString()
	          << " " << vec.getName()
		  << ", " << val.getCompleteString() << " " << val.getName()
		  << ", i32 " << idx.getName() << "\n";
}


void TllvmOutputTraverser::GenInsertVoM(const TllvmVar &mat,
					const TllvmVar &valv,
					const TType &origtype,
					const TllvmVar &idx)
{
    infoSink.code << "\t" << mat.getName()
		  << " = insertvector " << origtype.getCompleteString()
	          << " " << mat.getName() << ", "
		  << valv.getCompleteString() << " " << valv.getName()
		  << ", i32 " << idx.getName() << "\n";
}

void TllvmOutputTraverser::GenInsertEoM(const TllvmVar &mat,
					const TllvmVar &vale,
					const TType &origtype,
					const TllvmVar &idxc,
					const TllvmVar &idxr)
{
    infoSink.code << "\t" << mat.getName()
		  << " = insertelement " << origtype.getCompleteString()
	          << " " << mat.getName() << ", "
		  << vale.getCompleteString() << " " << vale.getName()
		  << ", <2 x i32> <i32 " << idxc.getName()
		  << ", i32 " << idxr.getName() << ">\n";
}

bool TllvmOutputTraverser::GenInsertValue(const TllvmVar &agg,
					  const TllvmVar &val,
					  const unsigned int size,
					  const int line)
{
    TllvmVarIndexList il = agg.getIndex();
    unsigned int i;

    if(il.size()==0 || size > il.size()) {
	infoSink.info.message(EPrefixInternalError,
			      "Index of Aggregate type out of range",
			      line);
	return false;
    }

    infoSink.code << "\t" << agg.getName()
		  << " = insertvalue " << il[0].type.getCompleteString()
	          << " " << agg.getName() << ", "
		  << val.getCompleteString() << " " << val.getName();

    for(i=0;i<size;++i)
	infoSink.code << ", " << il[i].index.getName();

    infoSink.code << "\n";

    return true;
}

bool TllvmOutputTraverser::GenInsertNonAgg(const TllvmVar &base,
					   const TllvmVar &val,
					   const TllvmVarIndexList &idx,
					   const int line)
{
    if(idx.size() == 1 &&
       idx[0].type.isVector() &&
       idx[0].index.isSwizzle())
    {
	GenInsertSwizzle(base, val, idx[0].type, idx[0].index.getSwizzle());
    }
    else if(idx.size() == 1 &&
	    idx[0].type.isVector() &&
	    idx[0].index.isSimpleConst())
    {
	GenInsertElement(base, val, idx[0].type, idx[0].index);
    }
    else if(idx.size() == 1 &&
	    idx[0].type.isMatrix() &&
	    idx[0].index.isSimpleType())
    {
	GenInsertVoM(base, val, idx[0].type, idx[0].index);
    }
    else if(idx.size() == 2 &&
	    idx[0].type.isMatrix() &&
	    idx[0].index.isSimpleType() &&
	    idx[1].index.isSimpleType())
    {
	GenInsertEoM(base, val, idx[0].type, idx[0].index, idx[1].index);
    }
    else
    {
	infoSink.info.message(EPrefixInternalError,
			      "No match operation found for index of Non-Aggregate type",
			      line);
	return false;
    }

    return true;
}


bool TllvmOutputTraverser::GenInsertAgg(const TllvmVar &agg,
					const TllvmVar &val,
					const int line)
{
    if(agg.getType() != val.getType()) {
	infoSink.info.message(EPrefixInternalError,
			 "Assigning to different type",
			 line);
	return false;
    }

    int size = agg.getIndex().size();
    int fng = agg.getFirstNonAggPos();

    if(fng<size) {
	TllvmVarIndexList idx;
	for(int i=fng;i<size;++i)
	    idx.push_back((agg.getIndex())[i]);

	if(fng>0) {
	    TllvmVar tv(allocate_tempvar().c_str(),
			(agg.getIndex())[fng].type);
	    return
	       GenExtractValue(agg, tv, fng, line) &&
	       GenInsertNonAgg(tv, val, idx, line) &&
	       GenInsertValue(agg, tv, fng, line);
	}
	else
	    return GenInsertNonAgg(agg, val, idx, line);
    }
    else
	return GenInsertValue(agg, val, size, line);

}

bool TllvmOutputTraverser::GenIndex(const TType &type,
				    const int line,
				    const bool isswizzle)
{
    TllvmVar basevar, v;
    IndexUnit iu;

    if(isswizzle)
    {
	int i;
	int size = type.getNominalSize();
	vector<int> swi(size);

	for(i=size-1;i>=0;--i) {
	    v = pop_expstack_rhs();
	    if(!v.isConst() || v.getBasicType() != EbtInt) {
		infoSink.info.message(EPrefixInternalError,
				 "Swizzle type error",
				 line);
		return false;
	    }
	    swi[i] = v.getAsInt();
	}

	iu.index = TllvmVar(swi, type);
    }
    else {
	v = pop_expstack_rhs();

	if(v.getName() == "") {
	    infoSink.info.message(EPrefixInternalError,
				  "Popping empty expression stack",
				  line);
	    return false;
	}

	iu.index = v;
    }

    basevar = pop_expstack();
    if(basevar.isCode())
	basevar = GenGetRHS(basevar, line);

    if(basevar.getName() == "") {
	infoSink.info.message(EPrefixInternalError,
			 "Popping empty expression stack",
			 line);
	return false;
    }

    iu.type = basevar.getType();
    basevar.pushIndex(iu);
    basevar.setType(type);
    push_expstack(basevar);

    return true;
}

bool TllvmOutputTraverser::GenAssign(const TType &type, const int line)
{
    bool rt = true;
    TllvmVar v1 = pop_expstack();
    TllvmVar v2 = pop_expstack();
    TllvmVar rhs;

    if(v1.getName() == "" || v2.getName() == "") {
	infoSink.info.message(EPrefixInternalError,
			 "Popping empty expression stack",
			 line);
	return false;
    }

    if(v1.isCode() && v2.isNormalType() && !v2.haveIndex()) {
	if(v1.getType() != v2.getType()) {
	    infoSink.info.message(EPrefixInternalError,
			     "Assigning to different type",
			     line);
	    infoSink.info << v1.getCompleteString() << " "
			  << v1.getName() << ", "
			  << v2.getCompleteString() << " "
			  << v1.getName() << "\n";
	    return false;
	}
	//dondum now -- Last assign to gl_position.
	//So far, gl_position is result of all the benchmark. When OutputTraverser generate assign to gl_Position, it finish this vertex shader.
	//We can search 'function finish' when other case occur.Maybe it is a way to do something.
	if(v2.getName() == "@gl_Position")
	  GenENDVertex(v1,v2);
	else if(v2.getName() == "@gl_FragColor")
	  GenENDFragment(v1,v2);
	else
	  infoSink.code << "\t" << v2.getName() << " = " << v1.getName();


	return true;
    }
    else if(v1.haveIndex() && v2.isNormalType() && !v2.haveIndex()) {
	return GenExtractAgg(v1, v2, line);
    }
    else {
	if(v2.isConst()) {
	    push_expstack(v2);
	    push_expstack(v1);
	    if(type.isVector()) {
		rhs = ConstructConstVec(type, line);
		v2 = pop_expstack_rhs();
	    }
	    else if(type.isMatrix()) {
		rhs = ConstructConstMat(type, line);
		v2 = pop_expstack_rhs();
	    }
	    else if(type.isStructure()) {
		rhs = ConstructConstStruct(type, line);
		v2 = pop_expstack_rhs();
	    }
	    else {
		infoSink.code << "Constant type (" << type.getCompleteString() << ") assign not implemented.\n";
		return false;
	    }
	}
	else
	    rhs = GenGetRHS(v1, line);

	if(v2.haveIndex()) {
	    rt = GenInsertAgg(v2, rhs, line);
	}
	else if(v2.isVector()) {
	  //dondum
	  GenVectorAssign(v2, rhs);
	}
	else if(v2.isMatrix()) {
	    rt = GenMatrixAssign(v2, rhs, line);
	}
	else if(v2.isStructure()) {
	    rt = GenStructAssign(v2, rhs, line);
	}
	else {
	    GenBinOp("or", v2, rhs, TllvmVar(0));
	}
    }

    if(rt)
	push_expstack(v2);

    return rt;

}

bool TllvmOutputTraverser::GenCalcAssign(const TOperator &op, const TType &type, const int line)
{
    string opstr;
    TllvmVar v1, v2;

    opstr = GetBinOp(op, type.getBasicType());
    if(opstr == "") {
	infoSink.info.message(EPrefixInternalError,
			      "unsupported operation",
			      line);
	return false;
    }

    v1 = pop_expstack_rhs();
    v2 = pop_expstack();
    if(v1.getName() == "" || v2.getName() == "") {
	infoSink.info.message(EPrefixInternalError,
			      "Popping empty expression stack",
			      line);
	return false;
    }

    if(v2.isSimpleVar()) {
	/*
	 * Don't allocate temporary variables
	 * if it can directly assign after operation
	 */
	GenBinOp(opstr, v2, v2, v1);
    }
    else {
	TllvmVar lhsv = GenGetRHS(v2, line);
	TllvmVar tgt = TllvmVar(allocate_tempvar().c_str(), type);
	GenBinOp(opstr, tgt, lhsv, v1);

	push_expstack(v2);
	push_expstack(tgt);
	return GenAssign(type, line);
    }

    return true;
}

bool TllvmOutputTraverser::GenArithBin(const TIntermBinary* const node)
{
    string opstr;
    TllvmVar tgt, ev1, ev2;

    opstr = GetBinOp(node->getOp(), node->getBasicType());
    if(opstr == "") {
	infoSink.info.message(EPrefixInternalError,
			      "unsupported operation",
			      node->getLine());
	return false;
    }

    ev1 = pop_expstack_rhs();
    ev2 = pop_expstack_rhs();
    if(ev1.getName() == "" || ev2.getName() == "") {
	infoSink.info.message(EPrefixInternalError,
			      "Popping empty expression stack",
			      node->getLine());
	return false;
    }
    tgt = TllvmVar(allocate_tempvar().c_str(), node->getType());

    GenBinOp(opstr, tgt, ev2, ev1);
    push_expstack(tgt);

    return true;
}

bool TllvmOutputTraverser::GenCompare(const TIntermBinary* const node)
{
    string opstr;
    TllvmVar tgt, ev1, ev2;

    opstr = GetCompOp(node->getOp(), node->getBasicType());
    if(opstr == "") {
	infoSink.info.message(EPrefixInternalError,
			      "unsupported operation",
			      node->getLine());
	return false;
    }

    ev1 = pop_expstack_rhs();
    ev2 = pop_expstack_rhs();
    if(ev1.getName() == "" || ev2.getName() == "") {
	infoSink.info.message(EPrefixInternalError,
			      "Popping empty expression stack",
			      node->getLine());
	return false;
    }
    tgt = TllvmVar(allocate_tempvar().c_str(), TType(EbtBool));

    GenBinOp(opstr, tgt, ev2, ev1);
    push_expstack(tgt);

    return true;

}

bool TllvmOutputTraverser::GenLogical(const TIntermBinary* const node)
{
    string opstr;
    TllvmVar tgt, ev1, ev2;

    switch(node->getOp()) {
	case EOpLogicalOr:
	    opstr = "or";
	    break;
	case EOpLogicalXor:
	    opstr = "xor";
	    break;
	case EOpLogicalAnd:
	    opstr = "and";
	    break;
	default:
	    opstr = "";
    }
    if(opstr == "") {
	infoSink.info.message(EPrefixInternalError,
			      "unsupported operation",
			      node->getLine());
	return false;
    }

    ev1 = pop_expstack_rhs();
    ev2 = pop_expstack_rhs();
    if(ev1.getName() == "" || ev2.getName() == "") {
	infoSink.info.message(EPrefixInternalError,
			      "Popping empty expression stack",
			      node->getLine());
	return false;
    }
    if(ev1.getBasicType() != EbtBool || ev2.getBasicType() != EbtBool) {
	infoSink.info.message(EPrefixInternalError,
			      "Logical operator on non-boolean variables",
			      node->getLine());
	return false;
    }
    tgt = TllvmVar(allocate_tempvar().c_str(), TType(EbtBool));

    GenBinOp(opstr, tgt, ev2, ev1);
    push_expstack(tgt);

    return true;

}

TllvmVar TllvmOutputTraverser::GenGetRHS(const TllvmVar &rhspv, const int line)
{
    if(rhspv.isCode()) {
	TllvmVar rt(allocate_tempvar().c_str(), rhspv.getType());
	infoSink.code << "\t" << rt.getName() << " = " << rhspv.getName();
	return rt;
    }
    else if(rhspv.haveIndex()) {
	TllvmVar rt(allocate_tempvar().c_str(),
		    TType(rhspv.getType()));
	GenExtractAgg(rhspv, rt, line);
	return rt;
    }
    else
	return rhspv;
}
/////////// dondum /////////////////////////////////////////////////////////////////////////////////////////////
void TllvmOutputTraverser::GenFuncOp(TllvmVar &rt,
				   const string &fnname,
			           const TllvmVarList &vec)
{
    int i,//countingMatrixSize=0, 
        size=vec.size();
    TInfoSinkBase code;
    //    std::cout<<"_____________size____________  :"<<size<<std::endl;
    //dondum
    //    for(i=size-2;i>=0;--i) {
    /*
    for(i=0;i<size;i++) {
      if(vec[i].isMatrix()){
	countingMatrixSize = vec[i].getNominalSize();
	GenLoad(rt,vec[i],countingMatrixSize);
      }
}
    */
    if(!rt.isCode()){
      //      std::cout << "...." << rt.getName() << "\n";
	code << "\t" << rt.getName() << " = ";
    }

    //dondum, This is for these two cases.
    //    code << "call " << rt.getCompleteString() << " @" << fnname <<"."<<size;
    if(!strcmp(fnname.c_str(),"OpenGLES.ChangeMatrix4")){
      code << "call " << rt.getCompleteString() << " @" << fnname <<"."<<size<<"."<<s;
    }
    else if(!strcmp(fnname.c_str(),"OpenGLES.VectorTimesMatrix")){
      code << "call " << rt.getCompleteString() << " @" << fnname <<"4."<<size;
    }
    else{
      code << "call " << rt.getCompleteString() << " @" << fnname <<"."<<size;
    }




    if(size>0) {
	code << "(";
	code << vec[size-1].getCompleteString() << " " << vec[size-1].getName();
	
	//dondum/////////////////////////////////////////////////////////////////////////////
	//std::cout <<"total [size] : "<<size<<std::endl;
	//std::cout <<"@fnname : "<<fnname<<std::endl;
	//
	//std::cout<<"About TllvmVar in call function : \n"
	//	 <<"1. getSize() : " <<vec[size-1].getType().getInstanceSize()/*getSize()*/<<"\n"
	//	 <<"2. getNominalSize() : " << vec[size-1].getNominalSize()<<"\n"
	//	 <<"3. getName() : " <<vec[size-1].getName()
	//	 <<std::endl;
	//////////////////////////////////////////////////////////////////////////////////////


	//	for(i=size-2;i>=0;--i) {
	  for(i=size-2;i>=0;--i) {
	    code << ", " << vec[i].getCompleteString()
		 << " " << vec[i].getName();

       //std::cout <<"@vec[" << i << "].getCompleteString : "<<vec[i].getCompleteString()<<std::endl;
       //std::cout <<"@vec[" << i << "].getName : "<<vec[i].getName()<<std::endl;

	}
	code << ")";
    }

    code << "\n";

    if(!rt.isCode())
	infoSink.code << code.c_str();
    else {
      //dondum
	rt.setName(code.c_str());
	push_expstack(rt);
	//std::cout<<"............222" <<rt.getName()<<"\n";
    }

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TllvmOutputTraverser::GenFunc(TllvmVar &rt,
				   const string &fnname,
				   const TllvmVarList &vec)
{
    int i, size=vec.size();
    TInfoSinkBase code;

    if(!rt.isCode())
	code << "\t" << rt.getName() << " = ";

    code << "call " << rt.getCompleteString() << " @" << fnname;

    if(size>0) {

	code << "(";

	code << vec[size-1].getCompleteString() << " " << vec[size-1].getName();

	  for(i=size-2;i>=0;--i) {
	    code << ", " << vec[i].getCompleteString()
		 << " " << vec[i].getName();

	}
	code << ")";
    }

    code << "\n";

    if(!rt.isCode())
	infoSink.code << code.c_str();
    else {
	rt.setName(code.c_str());
	push_expstack(rt);
    }

}

bool TllvmOutputTraverser::GenBinFuncAssign(string fnname, const int line)
{
    TllvmVarList varvec;
    varvec.push_back(pop_expstack_rhs());
    varvec.push_back(pop_expstack_rhs());

    if(varvec[0].getName() == "" || varvec[1].getName() == "") {
	infoSink.info.message(EPrefixInternalError,
			      "Popping empty expression stack",
			      line);
	return false;
    }

    GenFunc(varvec[1], fnname, varvec);

    push_expstack(varvec[1]);

    return true;
}

bool TllvmOutputTraverser::GenBinFuncOp(string fnname, const TType &type, const int line)
{
    TllvmVarList varvec;
    varvec.push_back(pop_expstack_rhs());
    varvec.push_back(pop_expstack_rhs());

    if(varvec[0].getName() == "" || varvec[1].getName() == "") {
	infoSink.info.message(EPrefixInternalError,
			      "Popping empty expression stack",
			      line);
	return false;
    }
    //dondum
    TllvmVar rt("", type, TllvmVarCode); //Original.
    //    TllvmVar rt(allocate_tempvar().c_str(), type, TllvmVarCode);
    GenFunc(rt, fnname, varvec);

    return true;
}

//dondum/////////////////////////////////////////////////////////////////////////////////
void TllvmOutputTraverser::GenOpenGLFuncOp(string fnname, const TType &type, const int num)
{
  TllvmVarList varvec, vartmp;//vartmp is for ChangeMatrix
  int i;
  TllvmVar v;
  
  fnname = string("OpenGLES.") + fnname;
  
  for(i=0;i<num;i++){
    //std::cout<<"v"<<i<<" : "<<v.getName()<<"\n";
    //std::cout<<"v"<<i<<" : "<<v.getCompleteString()<<"\n";
    
     v = pop_expstack_rhs();   
     if(v.isMatrix()){
       int j, k = v.getNominalSize();
       //printf("v.matrixSize : %d \n",k);
       TString MatrixName = v.getName();
       
       for(j=0;j<k;j++){
	 const TString &str = allocate_tempvar().c_str();
	 // TllvmVar ts( allocate_tempvar().c_str() , v.getType());
	 TllvmVar ts(str , v.getType());
	 GenLoad_M(ts,v,j,MatrixName);
	 v.setName(str);	
	 vartmp.push_back(v);  //-> 3 2 1 0 4 5 6


       }
       for(s=1;s<=k;s++){
	 /*	 const TString &str = allocate_tempvar().c_str();
	 TllvmVar ts( str , v.getType());

	 GenCallRegister(ts,v.getType());
	 v.setName(str);
	 varvec.insert(varvec.begin(),v); //4 5 6
 	 if(j == k-1 ){ //Check it when CallRegister's for-loop is finished.
	 */
	   
	   const TString &str = allocate_tempvar().c_str();
	   TllvmVar tm( str , v.getType());
	   
	   GenChangeMatrix(vartmp,tm);	
	   v.setName(str);
	   varvec.push_back(v);  //-> 3 2 1 0 4 5 6
	   //  varvec.insert(varvec.begin(),v);
	   //	 varvec.insert(varvec.begin(),v);    //-> 0 1 2 3 4 5 6

	   //To pop all unwanted temp_var (because we saved it in CallRegister. )
	   //	   for(j=0;j<=k;j++)
	   //  varvec.pop_back();	
	   //	   v= pop_expstack();
       }
     }
     else{
       if(v.isVector() && !v.isTemp()){
	 const TString &str = allocate_tempvar().c_str();
	 TllvmVar ts( str , v.getType());
	 if (v.getQualifier() == EvqAttribute ) // It means the vector is belong attribute.
	   GenLoadAttribute(ts,v);
	 else
	   GenLoad(ts,v);
	 v.setName(str);
	 // varvec.push_back(v);       	 
       }

       varvec.push_back(v);       
     }
    }
    
    //dondum test varvec[]
    /*    int s,k = varvec.size();
        std::cout<<"varvec list size is : "<< k << std::endl;

        for(s=0;s<k;s++){
      std::cout<<"varvec["<<s<<"].getName() : "<<varvec[s].getName()<<std::endl;
      } 
    */


    //Ask one additional CallRegister for linkers to do operations.
    const TString &str = allocate_tempvar().c_str();
    TllvmVar tt( str , v.getType());
    GenCallRegister(tt,v.getType());
    v.setName(str);
    varvec.insert(varvec.begin(),v);

    //dondum --original
    //TllvmVar tv("", type, TllvmVarCode);
      
    TllvmVar tv("", type, TllvmVarCode);
    GenFuncOp(tv, fnname, varvec);

}


void TllvmOutputTraverser::GenCallRegister(const TllvmVar &ptr ,const TType &type){

    infoSink.code << "\t" << ptr.getName()
		  << " = call " << type.getCompleteString()
	          << " " << "@OpenGLES.CallRegister()" <<"\n";


}

void TllvmOutputTraverser::GenLoadAttribute(const TllvmVar &tr ,const TllvmVar &v){

  int size = v.getType().getNominalSize();
  TString varname = v.getName();

    infoSink.code << "\t" << tr.getName()
		  << " = call <4 x float>"// << tr.getType().getCompleteString()
	          << " " << "@OpenGLES.LoadAttribute"<< size <<"."
                  // Remove the first character(@ or %). Reserve the variable name.
		  << varname.substr(1,varname.length()) 
		  <<".0()" <<"\n"; //means 0 argument
    

}

void TllvmOutputTraverser::GenLoadVarying(const TllvmVar &tr ,const TllvmVar &v){
  // all the hard code "<4 x float>" needs to be modified in the future !!!
  int size = v.getType().getNominalSize();
  TString varname = v.getName();

    infoSink.code << "\t" << tr.getName()
		  << " = call " << v.getType().getCompleteString()
	          << " " << "@OpenGLES.LoadVarying"<<size << "." //dondum did terrable things.
		  << varname.substr(1,varname.length()) 
		  <<".0()"<<"\n"; //means 0 argument 

    

}

void TllvmOutputTraverser::GenVaryingOut(const TllvmVar &dst, const TllvmVar &tr ,const TllvmVar &v){
  // all the hard code "<4 x float>" needs to be modified in the future !!!
  int size = v.getType().getNominalSize();
  TString varname = dst.getName();

    infoSink.code << "\t" << tr.getName()
		  << " = call " << "<4 x float>"//v.getType().getCompleteString()
	          << " " << "@OpenGLES.VaryingPut4." //dondum did terriable things.
		  << varname.substr(1,varname.length()) <<".1"//means 1 argument 
		  << "("<<"<4 x float>"/*v.getType().getCompleteString()*/<<" "<< v.getName()<<")" <<"\n";
    

}

void TllvmOutputTraverser::GenChangeMatrix(const TllvmVarList &varvec, const TllvmVar &v){
  //    TllvmVarList varvec;
  //    int i;
    TllvmVar var = v;
    string fnname = string("OpenGLES.ChangeMatrix") ;
    int size = var.getType().getNominalSize();
    //mark the matrix size as OpenGLES.ChangeMatrix(size)
    ss.clear();
    ss << fnname << size;
    ss >> fnname;
    
    /*    for(i=0;i<num;++i)
      varvec.push_back(v);
    */
    //          TllvmVar tv("", type, TllvmVarCode);
    GenFuncOp(var, fnname, varvec);

}

void TllvmOutputTraverser::GenENDVertex(const TllvmVar &var1, const TllvmVar &varRet){

  TllvmVarList varvec;
  string fnname = string("OpenGLES.END");
  TllvmVar var(varRet.getName(), varRet.getType());

	const TString &str = allocate_tempvar().c_str();
	TllvmVar ts( str , varRet.getType());
	ts.setName(str);
	varvec.push_back(ts); 

	infoSink.code << "\t" << ts.getName() << " = " << var1.getName();

	const TString &stv = allocate_tempvar().c_str();
	TllvmVar tr( stv , varRet.getType());
	tr.setName(stv);
	GenCallRegister(tr,varRet.getType());
	varvec.insert(varvec.begin(),tr); 

	GenFuncOp(var, fnname, varvec);	
}

void TllvmOutputTraverser::GenENDFragment(const TllvmVar &var1, const TllvmVar &varRet){

  TllvmVarList varvec;
  string fnname = string("OpenGLES.ENDFrag");
  TllvmVar var(varRet.getName(), varRet.getType());

  const TString &str = allocate_tempvar().c_str();
  TllvmVar ts( str , varRet.getType());
  ts.setName(str);
  varvec.push_back(ts); 
  
	infoSink.code << "\t" << ts.getName() << " = " << var1.getName();
	/*
	const TString &stv = allocate_tempvar().c_str();
	TllvmVar tr( stv , varRet.getType());
	tr.setName(stv);
	GenCallRegister(tr,varRet.getType());
	varvec.insert(varvec.begin(),tr); 
	*/
	GenFuncOp(var, fnname, varvec);	
}

void TllvmOutputTraverser::GenENDFragment_VectorAssign(const TllvmVar &var1, const TllvmVar &varRet){

  TllvmVarList varvec;
  string fnname = string("OpenGLES.ENDFrag");
  TllvmVar var(varRet.getName(), varRet.getType());


	//  GenENDFragment_VectorAssign(dst,tr)
  //	std::cout<< varRet.getName() <<"\n" << var1.getName()<<std::endl;
	if (varRet.getQualifier() == EvqVaryingIn ){      
	  const TString &str = allocate_tempvar().c_str();
	  TllvmVar ts( str , var1.getType());
	  ts.setName(str);
	  varvec.push_back(ts); 
	  int si;
	  si = varvec.size();
	  std::cout<<si<<std::endl;
	  GenLoadVarying(ts,varRet);
	}
	const TString &str = allocate_tempvar().c_str();
	TllvmVar tv( str , varRet.getType());
	tv.setName(str);
	varvec.push_back(tv); 
	//dondum

	TString varname = var.getName();

	int i;
	int size = var.getType().getNominalSize();
	infoSink.code << "\t" << tv.getName() << " = shufflevector "
		      << var.getCompleteString() << " "
	  //dondum do this temporary.
		      << "%0"/*<<varname.substr(1,varname.length())*/ << ", "
		      << var.getCompleteString() << " undef, "
		      << "<" << size << " x i32> <i32 0";

	for(i=1;i<size;++i)
	  infoSink.code << ", i32 " << i;
	infoSink.code << ">\n";

	//	infoSink.code << "\t" << ts.getName() << " = " << var1.getName();


	/*
	const TString &stv = allocate_tempvar().c_str();
	TllvmVar tr( stv , varRet.getType());
	tr.setName(stv);
	GenCallRegister(tr,varRet.getType());
	varvec.insert(varvec.begin(),tr); 
	*/
	TllvmVar va(var1.getName(), var1.getType());
	GenFuncOp(va, fnname, varvec);	
}



/////////////////////////////////////////////////////////////////////////////////////////


void TllvmOutputTraverser::GenOpenGLFunc(string fnname, const TType &type, const int num)
{
    TllvmVarList varvec;
    int i;

    fnname = string("OpenGLES.") + fnname;

    for(i=0;i<num;++i)
	varvec.push_back(pop_expstack_rhs());

    TllvmVar tv("", type, TllvmVarCode);
    GenFunc(tv, fnname, varvec);
}

bool TllvmOutputTraverser::GenConstructMat(const TType &t, const int line) {
    string fnname;
    TInfoSinkBase itoa;
    TllvmVarList cmvec;

    itoa << t.getNominalSize();
    fnname = "OpenGLES.ConstructMat";
    fnname += itoa.c_str();
    TllvmVar v = pop_expstack_rhs();
    while(!v.isEndConstructMatrix()) {
	cmvec.push_back(v);
	v = pop_expstack_rhs();
    }
    fnname = DepolymorphismFunctionName(fnname, cmvec);
    TllvmVar tv("", t, TllvmVarCode);
    GenFunc(tv, fnname, cmvec);
    return true;
}

bool TllvmOutputTraverser::GenConstructVec(const TType &t, const int line) {
    string fnname;
    TInfoSinkBase itoa;
    TllvmVarList cmvec;

    itoa << t.getNominalSize();
    fnname = "OpenGLES.Construct";
    if(t.getBasicType() == EbtBool)
	fnname += "B";
    else if(t.getBasicType() == EbtInt)
	fnname += "I"; 
    fnname += "Vec";
    fnname += itoa.c_str();
    TllvmVar v = pop_expstack_rhs();
    while(!v.isEndConstructVector()) {
	cmvec.push_back(v);
	v = pop_expstack_rhs();
    }
    fnname = DepolymorphismFunctionName(fnname, cmvec);
    TllvmVar tv("", t, TllvmVarCode);
    GenFunc(tv, fnname, cmvec);
    return true;
}
bool TllvmOutputTraverser::GenConstructStruct(const TType &t, const int line) {
    TTypeList* tl = t.getStruct();
    int size = tl?tl->size():0, i;
    TllvmVarList conststruct(size);
    TType curtype;
    TllvmVar v;

    for(i=size-1;i>=0;--i) {
	curtype = *((*tl)[i].type);
	v = pop_expstack_rhs();

	if(v.getType() != curtype) {
	    push_expstack(v);
	    if(v.isConst()) {
		if(curtype.isStructure())
		    v = ConstructConstStruct(curtype, line);
		else if(curtype.isVector())
		    v = ConstructConstVec(curtype, line);
		else if(curtype.isMatrix())
		    v = ConstructConstMat(curtype, line);
	    }
	}

	if(v.getType() != curtype) {
	    infoSink.code << "\n" << v.getType().getCompleteString()
			  << " " << v.getName()
			  << ", " << curtype.getCompleteString() << "\n";
	    infoSink.info.message(EPrefixInternalError,
				  "type mismatch when Constructing Structure",
				  line);
	    return false;
	}

	conststruct[i] = v;
    }
    if(!pop_expstack().isEndConstructStruct()) {
	infoSink.info.message(EPrefixInternalError,
			      "argument count mismatch when Constructing Structure",
			      line);
	return false;
    }

    push_expstack(TllvmVar(conststruct, t));
    return true;
}

// Original GenVectorAssign()
/*
void TllvmOutputTraverser::GenVectorAssign(const TllvmVar &dst, const TllvmVar &src) {
    int size = dst.getType().getNominalSize();
    int i;

    infoSink.code << "\t" << dst.getName() << " = shufflevector "
		  << src.getCompleteString() << " "
		  << src.getName() << ", "
		  << src.getCompleteString() << " undef, "
		  << "<" << size << " x i32> <i32 0";

    for(i=1;i<size;++i)
	infoSink.code << ", i32 " << i;

    infoSink.code << ">\n";

    push_expstack(dst);
}
*/
//dondum modified it.LLVM needs Load/Store instruciton when VectorAssign occurs.
void TllvmOutputTraverser::GenVectorAssign(const TllvmVar &dst, const TllvmVar &src) {

    int size = dst.getType().getNominalSize();
    int i;
    /*load add
    const TString &str = allocate_tempvar().c_str();
    TllvmVar tr(str , src.getType());
    GenLoad(tr,src);
    tr.setName(str);	
    //End of edit*/  

    //    GenENDFragment(dst,tr);



    if(dst.getName() == "@gl_FragColor"){
      GenENDFragment_VectorAssign(dst,src);
    }
    else{
  //dondum
  #ifdef MAZE
      /*    if(dst.getType().getNominalSize() != 4)
      {
	dst.getType().changeSize() = 4;
	}*/
  // all the hard code "<4 x float>, size=4 " needs to be modified in the future !!!
      size =4;

    const TString &str = allocate_tempvar().c_str();
    TllvmVar ts(str , dst.getType());
    ts.setName(str);	
    if (src.getQualifier() == EvqAttribute ){      
      GenLoadAttribute(ts,src);
    }

    const TString &stv = allocate_tempvar().c_str();
    TllvmVar tv(stv , dst.getType());
    tv.setName(stv);	
      infoSink.code << "\t" << tv.getName() << " = shufflevector "
		    << "<4 x float>" /*src.getCompleteString()*/ << " "
		    << ts.getName()/*src.getName()*/ << ", "
		    << "<4 x float>"/*src.getCompleteString()*/ << " undef, "
		    << "<" << size << " x i32> <i32 0";
      
      for(i=1;i<size;++i)
	infoSink.code << ", i32 " << i;
      
      infoSink.code << ">\n";

    if (dst.getQualifier() == EvqVaryingOut ){ 
    const TString &str = allocate_tempvar().c_str();
    TllvmVar tk(str , dst.getType());
    tk.setName(str);	
    GenVaryingOut(dst,tk,tv);
    }

      push_expstack(dst);


  #endif
  #ifndef MAZE
    const TString &str = allocate_tempvar().c_str();
    TllvmVar ts(str , dst.getType());
    ts.setName(str);	

      infoSink.code << "\t" << dst.getName() << " = shufflevector "
		      << src.getCompleteString() << " "
			   << src.getName() << ", "
		    << src.getCompleteString() << " undef, "
		    << "<" << size << " x i32> <i32 0";
      
      for(i=1;i<size;++i)
	infoSink.code << ", i32 " << i;
      
      infoSink.code << ">\n";
      
      //  push_expstack(dst);
      push_expstack(dst);
      ///*Store .
      // GenStore(src,ts);
      //End of Edit*/

  #endif
    }

}



bool TllvmOutputTraverser::GenMatrixAssign(const TllvmVar &dst, const TllvmVar &src, const int line) {
    int size = dst.getType().getNominalSize();
    int i, j, rt;

    push_expstack(TllvmVar("", TType(EbtVoid), TllvmVarEndConstructMatrix));
    if(src.isConst()) {
	vector< vector<float> > mat = src.getAsMatrix();
	for(i=0;i<size;++i)
	    for(j=0;j<size;++j)
		push_expstack(mat[i][j]);
    }
    else
	push_expstack(src);

    rt = GenConstructMat(dst.getType(), line);
    TllvmVar nsrc = pop_expstack();
    infoSink.code << "\t" << dst.getName() << " = " << nsrc.getName();

    push_expstack(dst);

    return rt;
}

bool TllvmOutputTraverser::GenStructAssign(const TllvmVar &dst, const TllvmVar &src, const int line) {

    if(src.getType() != dst.getType()) {
	infoSink.info.message(EPrefixInternalError,
			      "Mismatched structure assign",
			      line);
	return false;
    }

    GenStore(dst, src);

    return true;
}

TllvmVar TllvmOutputTraverser::ConstructConstVec(const TType &t, const int line) {
    int i;
    int size = t.getNominalSize();

    if(t.getBasicType() == EbtBool) {
	vector<bool> bvec(size, 0);
	for(i=size-1;i>=0;--i)
	    bvec[i] = pop_expstack_rhs().getAsBool();
	return TllvmVar(bvec);
    }
    else if(t.getBasicType() == EbtInt) {
	vector<int> ivec(size, 0);
	for(i=size-1;i>=0;--i)
	    ivec[i] = pop_expstack_rhs().getAsInt();
	return TllvmVar(ivec);
    }
    else if(t.getBasicType() == EbtFloat) {
	vector<float> fvec(size, 0);
	for(i=size-1;i>=0;--i)
	    fvec[i] = pop_expstack_rhs().getAsFloat();
	return TllvmVar(fvec);
    }
    else {
	infoSink.info.message(EPrefixInternalError,
			      "unsupported type for Construct Constant Vector",
			      line);
	return TllvmVar();
    }
}

TllvmVar TllvmOutputTraverser::ConstructConstMat(const TType &t, const int line) {
    int i, j;
    int size = t.getNominalSize();

    vector<float> fvec(size, 0);
    vector< vector<float> > mat(size, fvec);

    for(i=size-1;i>=0;--i) {
	for(j=size-1;j>=0;--j)
	    fvec[j] = pop_expstack().getAsFloat();
	mat[i] = fvec;
    }

    return TllvmVar(mat);

}

TllvmVar TllvmOutputTraverser::ConstructConstStruct(const TType &t, const int line) {
    TTypeList *tl = t.getStruct();
    int size = tl?tl->size():0, i;
    TType curtype;
    TllvmVarList conststruct(size);

    for(i=size-1;i>=0;--i) {
	curtype = *((*tl)[i].type);
	if(curtype.isMatrix())
	    conststruct[i] = ConstructConstMat(curtype, line);
	else if(curtype.isVector())
	    conststruct[i] = ConstructConstVec(curtype, line);
	else if(curtype.isStructure())
	    conststruct[i] = ConstructConstStruct(curtype, line);
	else if(!curtype.isArray()) {
	    TllvmVar v = pop_expstack();
	    if(!v.isSimpleConst() || v.getType() != curtype) {
		infoSink.code << "\n" << v.getType().getCompleteString()
			      << " " << v.getName()
		              << ", " << curtype.getCompleteString() << "\n";
		infoSink.info.message(EPrefixInternalError,
				      "type mismatch when Constructing Constant Structure",
				      line);
		return TllvmVar();
	    }
	    conststruct[i] = v;
	}
	else {
	    infoSink.info.message(EPrefixInternalError,
				  "unsupported type for Construct Constant Structure",
				  line);
	    return TllvmVar();
	}
    }

    return TllvmVar(conststruct, t);
}

void TllvmOutputTraverser::GenCondition(TIntermLoop* node,
				        const string &label)
{
    TllvmVar cond;

    infoSink.code << label.c_str() << ".C:\n";
    if(node->getTest()) {
	node->getTest()->traverse(this);

	cond = pop_expstack_rhs();
	if(!cond.isSimpleType() || cond.getBasicType() != EbtBool) {
	    infoSink.info.message(EPrefixInternalError,
				  "unsupported type in selection",
				  node->getLine());
	}

	infoSink.code << "\tbr " << cond.getCompleteString().c_str()
		      << " " << cond.getName() << ", label %" << label.c_str()
		      << ".B, label %" << label.c_str() << ".E\n";
    }
}

bool TllvmOutputTraverser::GenPrePostIncDec(const TOperator &op, const int line)
{
    TllvmVar v = pop_expstack();
    TllvmVar rhs;
    string opstr = (op == EOpPostIncrement ||
		    op == EOpPreIncrement)?
		    "add":"sub";
    bool ispost = op == EOpPostIncrement ||
		  op == EOpPreIncrement;

    if(v.isNormalType())
	rhs = v;
    else
	rhs = GenGetRHS(v, line);

    if(rhs.isSimpleType()) {
	if(ispost) {
	    /* Allocate a temporary variable for return */
	    TllvmVar tv(allocate_tempvar().c_str(), rhs.getType());
	    push_expstack(tv);
	    push_expstack(rhs);
	    GenAssign(tv.getType(), line);

	    /* Save the added or subtracted result */
	    if(v.isSimpleVar()) {
		GenBinOp(opstr, v, rhs, TllvmVar(1));
	    }
	    else {
		/* Allocate a temporary variable for save it back */
		TllvmVar tv2(allocate_tempvar().c_str(), rhs.getType());
		GenBinOp(opstr, tv2, rhs, TllvmVar(1));
		push_expstack(v);
		push_expstack(tv2);
		GenAssign(v.getType(), line);
	    }

	    push_expstack(tv);
	}
	else {
	    if(v.isSimpleVar()) {
		GenBinOp(opstr, v, rhs, TllvmVar(1));
	    }
	    else {
		/* Allocate a temporary variable for save it back */
		TllvmVar tv(allocate_tempvar().c_str(), rhs.getType());
		GenBinOp(opstr, tv, rhs, TllvmVar(1));
		push_expstack(v);
		push_expstack(tv);
		GenAssign(v.getType(), line);
	    }

	    push_expstack(v);
	}

	return true;

    }
    else {
	infoSink.info.message(EPrefixInternalError,
			      "unsupported type for Pre/Post Increment/Decrement",
			      line);
	return false;
    }

}

void TllvmOutputTraverser::GenConvert(const TOperator &op, const TType &type)
{
    TllvmVar v = pop_expstack_rhs();
    /*
     * Assume that after IR tree constructed,
     * the type passed here is correct.
     */
    TInfoSinkBase code;

    code << convopstr[op]
	 << " " << v.getCompleteString()
	 << " " << v.getName() << " to "
	 << type.getCompleteString() << "\n";

    TllvmVar tv(code.c_str(), type, TllvmVarCode);
    push_expstack(tv);
}

TllvmOutputTraverser::TllvmOutputTraverser(TInfoSink& i) : infoSink(i) {

    preVisit = true;
    postVisit = true;
    //dondum tempvar.
    for(n=tempvar_nr-1;n>=0;--n) {
     	ss.clear();
	ss << "%" << n;
	ss >> str;
	tempvars.push(str);
    }
    isparm = false;
    isdeclare = false;
    isinitializer = false;
    labelnr = 0;

    convopstr[EOpConvIntToBool]   = "trunc";
    convopstr[EOpConvBoolToInt]   = "zext";
    convopstr[EOpConvFloatToBool] = "fptoui";
    convopstr[EOpConvFloatToInt]  = "fptosi";
    convopstr[EOpConvBoolToFloat] = "uitofp";
    convopstr[EOpConvIntToFloat]  = "sitofp";

    typemang[EbtBool  | 1 << 5]		= ".b";
    typemang[EbtInt   | 1 << 5]		= ".i";
    typemang[EbtFloat | 1 << 5]		= ".f";
    typemang[EbtBool  | 2 << 5]		= ".bvec2";
    typemang[EbtBool  | 3 << 5]		= ".bvec3";
    typemang[EbtBool  | 4 << 5]		= ".bvec4";
    typemang[EbtInt   | 2 << 5]		= ".ivec2";
    typemang[EbtInt   | 3 << 5]		= ".ivec3";
    typemang[EbtInt   | 4 << 5]		= ".ivec4";
    typemang[EbtFloat | 2 << 5]		= ".vec2";
    typemang[EbtFloat | 3 << 5]		= ".vec3";
    typemang[EbtFloat | 4 << 5]		= ".vec4";
    typemang[EbtFloat | 2 << 5 | 1 << 8]= ".mat2";
    typemang[EbtFloat | 3 << 5 | 1 << 8]= ".mat3";
    typemang[EbtFloat | 4 << 5 | 1 << 8]= ".mat4";

    visitAggregate = llvmOutputAggregate;
    visitBinary = llvmOutputBinary;
    visitConstantUnion = llvmOutputConstantUnion;
    visitSelection = llvmOutputSelection;
    visitSymbol = llvmOutputSymbol;
    visitUnary = llvmOutputUnary;
    visitLoop = llvmOutputLoop;
    visitBranch = llvmOutputBranch;

}

string TllvmOutputTraverser::allocate_tempvar(void) {
    if(tempvars.empty()) {
	str = "";
    }
    else {
	str = tempvars.top();
	tempvars.pop();
    }
    return str;
}

const TllvmVar& TllvmOutputTraverser::pop_expstack(void) {
    if(expstack.empty()) {
	var.setName("");
	var.setType(TType(EbtVoid));
    }
    else {
	var = expstack.top();
	expstack.pop();
    }
    return var;
}

const TllvmVar& TllvmOutputTraverser::pop_expstack_rhs(void) {
    var = GenGetRHS(pop_expstack(), -1);
    return var;
}

void TllvmOutputTraverser::push_expstack(const TllvmVar &v) {
    expstack.push(v);
}

void TllvmOutputTraverser::into_function(const TllvmVar &func) {
    func_scope.push(func);
}
//dondum
void TllvmOutputTraverser::outof_function(void) {
    func_scope.pop();
}

TllvmVar TllvmOutputTraverser::get_function_var(void) {
    return func_scope.top();
}

void TllvmOutputTraverser::into_loop(string s) {
    loop_scope.push(s);
}

void TllvmOutputTraverser::outof_loop(void) {
    loop_scope.pop();
}

string TllvmOutputTraverser::get_loop_label(void) {
    if(loop_scope.empty())
	str = "";
    else
	str = loop_scope.top();

    return str;
}

void TllvmOutputTraverser::into_parameters(void) {
    firstparm = true;
    isparm = true;
}

void TllvmOutputTraverser::outof_parameters(void) {
    isparm = false;
}

bool TllvmOutputTraverser::is_parameters(void) {
    return isparm;
}

bool TllvmOutputTraverser::is_first_parameter(void) {
    bool rt = firstparm;
    firstparm = false;
    return rt;
}

const string& TllvmOutputTraverser::get_label(void) {
    ss.clear();
    ss << "L" << ++labelnr;
    ss >> str;
    return str;
}

//dondum 
//'main:void()'
string TllvmOutputTraverser::ConvertMangledFunctionName(string fnname) {
    string::size_type pos;

    if((pos = fnname.find("(")) == fnname.size() - 1)
      fnname.replace(pos, 5, ":void");
    else
	fnname.replace(pos, 1, ":");

    return fnname;
}

string TllvmOutputTraverser::DepolymorphismFunctionName(string fnname, TllvmVarList cmvec) {
    int i;
    unsigned int idx;
    TType type;

    for(i=(cmvec.size()-1);i>=0;--i) {
	type = cmvec[i].getType();
	idx = type.getBasicType() | type.getNominalSize() << 5 | type.isMatrix() << 8;
	fnname += typemang[idx];
    }
    return fnname;
}

#define OUTPUT_TEMPORARY 0

void TllvmOutputTraverser::llvmOutputSymbol(TIntermSymbol* node,
					    TIntermTraverser* it)
{
    TllvmOutputTraverser* oit = static_cast<TllvmOutputTraverser*>(it);

    if(oit->is_parameters()) {
	if(!oit->is_first_parameter())
	    oit->infoSink.code << ", ";

	oit->infoSink.code << node->getCompleteString()
			   << " %" << node->getSymbol();
    }
    else if(oit->is_declare() && !oit->is_initializer()) {

#if OUTPUT_TEMPORARY
	string prefix;
	if(node->getType().getQualifier() == EvqTemporary) {
	    prefix = "\t%";
	}
	else if(node->getType().getQualifier() == EvqGlobal) {
	    prefix = "@";
	}
	else {
	    prefix = "@";
	}
	oit->infoSink.code << prefix << node->getSymbol()
			   << " = " << node->getType().getCompleteString(true)
			   << "\n";
#else
	if(node->getType().getQualifier() == EvqTemporary)
	    return;
	//dondum
	oit->infoSink.code << "@" << node->getSymbol()
			   << " = " << node->getType().getCompleteString(true)
			   << "\n";

#endif

    }
    else {
	oit->push_expstack(
	   //dondum
	   //For llvm, every variable in function is "%" 
	   //TllvmVar(TString((node->getType().getQualifier() == EvqTemporary)?"%":"@") + node->getSymbol(),
	    TllvmVar(TString((node->getType().getQualifier() == EvqTemporary)?"%":"@") + node->getSymbol(),
		     node->getType())
	);
    }
}

bool TllvmOutputTraverser::llvmOutputBinary(bool preVisit,
					    TIntermBinary* node,
					    TIntermTraverser* it)
{
    TllvmOutputTraverser* oit = static_cast<TllvmOutputTraverser*>(it);
    TInfoSink& out = oit->infoSink;

    if(preVisit && node->getOp() == EOpAssign)
	if(oit->is_declare())
	    oit->into_initializer();


    if(!preVisit) {

	switch (node->getOp()) {
	    case EOpAssign:
		if(oit->is_declare())
		    oit->outof_initializer();
		/*
		 * NOTE:
		 *	    if node->getType() is structure,
		 *	    the actual structure information is missing (TType::structure == 0),
		 *	    using node->getLeft()->getType() for workaround.
		 */
		if(node->getType().isStructure())
		  return oit->GenAssign(node->getLeft()->getType(), node->getLine());

		else
		  return oit->GenAssign(node->getType(), node->getLine());

            //dondum
	    case EOpVectorTimesMatrixAssign:

		return oit->GenBinFuncAssign("OpenGLES.VectorTimesMatrix", node->getLine());
	    case EOpVectorTimesScalarAssign:
		return oit->GenBinFuncAssign("OpenGLES.VectorTimesScalar", node->getLine());
	    case EOpMatrixTimesScalarAssign:
		return oit->GenBinFuncAssign("OpenGLES.MatrixTimesScalar", node->getLine());
	    case EOpMatrixTimesMatrixAssign:
		return oit->GenBinFuncAssign("OpenGLES.MatrixTimesMatrix", node->getLine());


	    case EOpAddAssign:
	    case EOpSubAssign:
	    case EOpMulAssign:
	    case EOpDivAssign:
	    case EOpModAssign:
	    case EOpAndAssign:
	    case EOpInclusiveOrAssign:
	    case EOpExclusiveOrAssign:
	    case EOpLeftShiftAssign:
	    case EOpRightShiftAssign:
		return oit->GenCalcAssign(node->getOp(), node->getType(), node->getLine());

	    case EOpIndexDirect:
		return oit->GenIndex(node->getType(), node->getLine());
	    case EOpIndexIndirect:
		return oit->GenIndex(node->getType(), node->getLine());

	    case EOpIndexDirectStruct:
		return oit->GenIndex(node->getType(), node->getLine());

	    case EOpVectorSwizzle:
		return oit->GenIndex(node->getType(), node->getLine(), true);
		break;

	    case EOpAdd:
	    case EOpSub:
	    case EOpMul:
	    case EOpDiv:
	    case EOpMod:
	    case EOpRightShift:
	    case EOpLeftShift:
	    case EOpAnd:
	    case EOpInclusiveOr:
	    case EOpExclusiveOr:
		return oit->GenArithBin(node);

	    case EOpEqual:
	    case EOpNotEqual:
	    case EOpLessThan:
	    case EOpGreaterThan:
	    case EOpLessThanEqual:
	    case EOpGreaterThanEqual:
		return oit->GenCompare(node);

	    case EOpVectorTimesScalar:
	      return oit->GenBinFuncOp("OpenGLES.VectorTimesScalar", node->getType(), node->getLine());


	    //dondm
	    case EOpVectorTimesMatrix:
	      //return oit->GenBinFuncOp("OpenGLES.VectorTimesMatrix", node->getType(), node->getLine());
	      //____________________________________________________________________________________________
	      //int MatrixRow, total;
	      //MatrixRow = node->getRight()->getType().getNominalSize();     
	      //There are 5 rows with a Vector and a 4x4 Matrix
	      //                        (left)           (right)
	      //total = 1 + MatrixRow;
	      //_________________________________________I made it with other method._______________________
	      
	      oit->GenOpenGLFuncOp("VectorTimesMatrix", node->getType(),2);
	     return true;

	    case EOpMatrixTimesVector:
		/*
		 * Row vector <-> Col vector
		 * But we don't distinguish these two types, so do nothing
		 */
		return oit->GenBinFuncOp("OpenGLES.MatrixTimesVector", node->getType(), node->getLine());
	    case EOpMatrixTimesScalar:
		return oit->GenBinFuncOp("OpenGLES.MatrixTimesScalar", node->getType(), node->getLine());
	    case EOpMatrixTimesMatrix:
		return oit->GenBinFuncOp("OpenGLES.MatrixTimesMatrix", node->getType(), node->getLine());

	    case EOpLogicalOr:
	    case EOpLogicalXor:
	    case EOpLogicalAnd:
		return oit->GenLogical(node);

	    default:
		out.info.message(EPrefixInternalError,
				 "Unknown op",
				 node->getLine());
		return false;
	}

    }

    return true;
}

bool TllvmOutputTraverser::llvmOutputUnary(bool preVisit,
					   TIntermUnary* node,
					   TIntermTraverser* it)
{
    TllvmOutputTraverser* oit = static_cast<TllvmOutputTraverser*>(it);
    TInfoSink& out = oit->infoSink;

    if(!preVisit) {
	switch (node->getOp()) {
	    case EOpNegative:
		{
		    TllvmVar v = oit->pop_expstack_rhs();

		    if(v.isSimpleType()){
			    TllvmVar tv(oit->allocate_tempvar().c_str(),
					TType(v.getBasicType()));
			    oit->GenBinOp("sub", tv, TllvmVar(0), v);
			    oit->push_expstack(tv);
			    return true;
		    }
		    else if(v.isVector()) {
			oit->push_expstack(v);
			oit->GenOpenGLFunc("VectorNegate", v.getType(), 1);
			return true;
		    }
		    else {
			out.info.message(EPrefixInternalError,
					 "unsupported type for Negate",
					 node->getLine());
			return false;
		    }
		}

	    case EOpVectorLogicalNot:
		oit->GenOpenGLFunc("VectorLogicalNot", node->getType(), 1);
		return true;

	    case EOpLogicalNot:
		{
		    TllvmVar v = oit->pop_expstack_rhs();

		    if(v.isSimpleType() && v.getBasicType() == EbtBool) {
			TllvmVar tv(oit->allocate_tempvar().c_str(),
				    TType(v.getBasicType()));
			oit->GenBinOp("xor", tv, TllvmVar(true), v);
			oit->push_expstack(tv);
			return true;
		    }
		    else {
			out.info.message(EPrefixInternalError,
					 "unsupported type for Logical Not",
					 node->getLine());
			return false;
		    }
		}

	    case EOpBitwiseNot:
		{
		    TllvmVar v = oit->pop_expstack_rhs();

		    if(v.isSimpleType()){
			TllvmVar tv(oit->allocate_tempvar().c_str(),
				    TType(v.getBasicType()));
			oit->GenBinOp("xor", tv, TllvmVar(-1), v);
			oit->push_expstack(tv);
			return true;
		    }
		    else {
			out.info.message(EPrefixInternalError,
					 "unsupported type for Bitwise Not",
					 node->getLine());
			return false;
		    }
		}

	    case EOpPostIncrement:
	    case EOpPostDecrement:
	    case EOpPreIncrement:
	    case EOpPreDecrement:
		return oit->GenPrePostIncDec(node->getOp(), node->getLine());

	    case EOpConvIntToBool:
	    case EOpConvFloatToBool:
	    case EOpConvBoolToFloat:
	    case EOpConvIntToFloat:
	    case EOpConvFloatToInt:
	    case EOpConvBoolToInt:
		oit->GenConvert(node->getOp(), node->getType());
		return true;

	    case EOpRadians:
		oit->GenOpenGLFunc("Radians", node->getType(), 1);
		return true;
	    case EOpDegrees:
		oit->GenOpenGLFunc("Degrees", node->getType(), 1);
		return true;
	    case EOpSin:
		oit->GenOpenGLFunc("Sin", node->getType(), 1);
		return true;
	    case EOpCos:
		oit->GenOpenGLFunc("Cos", node->getType(), 1);
		return true;
	    case EOpTan:
		oit->GenOpenGLFunc("Tan", node->getType(), 1);
		return true;
	    case EOpAsin:
		oit->GenOpenGLFunc("Asin", node->getType(), 1);
		return true;
	    case EOpAcos:
		oit->GenOpenGLFunc("Acos", node->getType(), 1);
		return true;
	    case EOpAtan:
		oit->GenOpenGLFunc("Atan", node->getType(), 1);
		return true;

	    case EOpExp:
		oit->GenOpenGLFunc("Exp", node->getType(), 1);
		return true;
	    case EOpLog:
		oit->GenOpenGLFunc("Log", node->getType(), 1);
		return true;
	    case EOpExp2:
		oit->GenOpenGLFunc("Exp2", node->getType(), 1);
		return true;
	    case EOpLog2:
		oit->GenOpenGLFunc("Log2", node->getType(), 1);
		return true;
	    case EOpSqrt:
		oit->GenOpenGLFunc("Sqrt", node->getType(), 1);
		return true;
	    case EOpInverseSqrt:
		oit->GenOpenGLFunc("InverseSqrt", node->getType(), 1);
		return true;

	    case EOpAbs:
		oit->GenOpenGLFunc("Abs", node->getType(), 1);
		return true;
	    case EOpSign:
		oit->GenOpenGLFunc("Sign", node->getType(), 1);
		return true;
	    case EOpFloor:
		oit->GenOpenGLFunc("Floor", node->getType(), 1);
		return true;
	    case EOpCeil:
		oit->GenOpenGLFunc("Ceil", node->getType(), 1);
		return true;
	    case EOpFract:
		oit->GenOpenGLFunc("Fract", node->getType(), 1);
		return true;

	    case EOpLength:
		oit->GenOpenGLFunc("Length", node->getType(), 1);
		return true;
	    case EOpNormalize:
		oit->GenOpenGLFunc("Normalize", node->getType(), 1);
		return true;
	    case EOpDPdx:
		oit->GenOpenGLFunc("DPdx", node->getType(), 1);
		return true;
	    case EOpDPdy:
		oit->GenOpenGLFunc("DPdy", node->getType(), 1);
		return true;
	    case EOpFwidth:
		oit->GenOpenGLFunc("Fwidth", node->getType(), 1);
		return true;

	    case EOpAny:
		oit->GenOpenGLFunc("Any", node->getType(), 1);
		return true;
	    case EOpAll:
		oit->GenOpenGLFunc("All", node->getType(), 1);
		return true;

	    default:
		out.code.message(EPrefixError, "Bad unary op");
		out.code << " (" << node->getCompleteString() << ")\n";
		break;
	}

    }

    return true;
}

bool TllvmOutputTraverser::llvmOutputAggregate(bool preVisit,
					       TIntermAggregate* node,
					       TIntermTraverser* it)
{
    TllvmOutputTraverser* oit = static_cast<TllvmOutputTraverser*>(it);
    TInfoSink& out = oit->infoSink;
    string fnname;

    if(preVisit) {
	if (node->getOp() == EOpNull) {
	    out.code.message(EPrefixError, "node is still EOpNull!");
	    return true;
	}

	switch (node->getOp()) {
	    case EOpSequence:
		return true;

	    case EOpDeclaration:
		oit->into_declare();
		return true;

	    case EOpFunction:
		{
		    string fnname = oit->ConvertMangledFunctionName(node->getName().c_str());
		    out.code << "\ndefine " << node->getCompleteString() << " @" << fnname;
		    TllvmVar tv(fnname.c_str(), node->getType());
		    oit->into_function(tv);
		    return true;
		}

	    case EOpParameters:
		out.code << "(";
		oit->into_parameters();
		return true;

	    case EOpConstructFloat:
	    case EOpConstructVec2:
	    case EOpConstructVec3:
	    case EOpConstructVec4:
	    case EOpConstructBool:
	    case EOpConstructBVec2:
	    case EOpConstructBVec3:
	    case EOpConstructBVec4:
	    case EOpConstructInt:
	    case EOpConstructIVec2:
	    case EOpConstructIVec3:
	    case EOpConstructIVec4:
		oit->push_expstack(TllvmVar("", TType(EbtVoid), TllvmVarEndConstructVector));
		return true;

	    case EOpConstructMat2:
	    case EOpConstructMat3:
	    case EOpConstructMat4:
		oit->push_expstack(TllvmVar("", TType(EbtVoid), TllvmVarEndConstructMatrix));
		return true;

	    case EOpConstructStruct:
		oit->push_expstack(TllvmVar("", TType(EbtVoid), TllvmVarEndConstructStruct));
		return true;

	    case EOpComma:
	    case EOpFunctionCall:
	    case EOpLessThan:
	    case EOpGreaterThan:
	    case EOpLessThanEqual:
	    case EOpGreaterThanEqual:
	    case EOpVectorEqual:
	    case EOpVectorNotEqual:
	    case EOpMod:
	    case EOpPow:
	    case EOpAtan:
	    case EOpMin:
	    case EOpMax:
	    case EOpClamp:
	    case EOpMix:
	    case EOpStep:
	    case EOpSmoothStep:
	    case EOpDistance:
	    case EOpDot:
	    case EOpCross:
	    case EOpFaceForward:
	    case EOpReflect:
	    case EOpRefract:
	    case EOpMul:
		//Only postVisit process needed.
		return true;

	    case EOpItof:
	    case EOpFtoi:
	    case EOpSkipPixels:
	    case EOpReadInput:
	    case EOpWritePixel:
	    case EOpBitmapLsb:
	    case EOpBitmapMsb:
	    case EOpWriteOutput:
	    case EOpReadPixel:
		//Pack operations not used for now

	    default:
		out.code.message(EPrefixError, "Bad aggregation op");
		if (node->getOp() != EOpSequence && node->getOp() != EOpParameters)
		    out.code << " (" << node->getCompleteString() << ")";
		out.code << "\n";
	}

    }
    else {
	switch (node->getOp()) {
	    case EOpDeclaration:
		oit->outof_declare();
		return true;

	    case EOpFunctionCall:
		{
		    string fnname = oit->ConvertMangledFunctionName(node->getName().c_str());
		    TllvmVarList arglist;
		    string::size_type pos = 0;

		    while((pos = fnname.find_first_of(";", pos)) != string::npos) {
			arglist.push_back(oit->pop_expstack_rhs());
			++pos;
		    }

		    TllvmVar tv("", node->getType(), TllvmVarCode);

		    oit->GenFunc(tv, fnname, arglist);
		    return true;
		}

	    case EOpFunction:
	      //dondum - function finish
	      //Information of node is empty. Here is just print "}" in IR.
	   	out.code << "}\n\n";
		oit->outof_function();
		return true;

	    case EOpParameters:
		oit->outof_parameters();
		out.code << ") {\n";
		return true;

	    case EOpSequence:
		return true;

	    case EOpComma:
		{
		    /*
		     * Only preserve right-most result
		     */
		    TllvmVar v = oit->pop_expstack();
		    oit->pop_expstack();
		    oit->push_expstack(v);
		    return true;
		}

	    case EOpConstructFloat:
	    case EOpConstructBool:
	    case EOpConstructInt:
	    case EOpConstructVec2:
	    case EOpConstructBVec2:
	    case EOpConstructIVec2:
	    case EOpConstructVec3:
	    case EOpConstructBVec3:
	    case EOpConstructIVec3:
	    case EOpConstructVec4:
	    case EOpConstructBVec4:
	    case EOpConstructIVec4:
		return oit->GenConstructVec(node->getType(), node->getLine());

	    case EOpConstructMat2:
	    case EOpConstructMat3:
	    case EOpConstructMat4:
		return oit->GenConstructMat(node->getType(), node->getLine());

	    case EOpLessThan:
		oit->GenOpenGLFunc("LessThan", node->getType(), 2);
		return true;
	    case EOpGreaterThan:
		oit->GenOpenGLFunc("GreaterThan", node->getType(), 2);
		return true;
	    case EOpLessThanEqual:
		oit->GenOpenGLFunc("LessThanEqual", node->getType(), 2);
		return true;
	    case EOpGreaterThanEqual:
		oit->GenOpenGLFunc("GreaterThanEqual", node->getType(), 2);
		return true;
	    case EOpVectorEqual:
		oit->GenOpenGLFunc("VectorEqual", node->getType(), 2);
		return true;
	    case EOpVectorNotEqual:
		oit->GenOpenGLFunc("VectorNotEqual", node->getType(), 2);
		return true;

	    case EOpMod:
		oit->GenOpenGLFunc("Mod", node->getType(), 2);
		return true;
	    case EOpPow:
		oit->GenOpenGLFunc("Pow", node->getType(), 2);
		return true;
	    case EOpAtan:
		oit->GenOpenGLFunc("Atan", node->getType(), 2);
		return true;
	    case EOpMin:
		oit->GenOpenGLFunc("Min", node->getType(), 2);
		return true;
	    case EOpMax:
		oit->GenOpenGLFunc("Max", node->getType(), 2);
		return true;
	    case EOpClamp:
		oit->GenOpenGLFunc("Clamp", node->getType(), 3);
		return true;
	    case EOpMix:
		oit->GenOpenGLFunc("Mix", node->getType(), 3);
		return true;
	    case EOpStep:
		oit->GenOpenGLFunc("Step", node->getType(), 2);
		return true;
	    case EOpSmoothStep:
		oit->GenOpenGLFunc("SmoothStep", node->getType(), 3);
		return true;

	    case EOpDistance:
		oit->GenOpenGLFunc("Distance", node->getType(), 2);
		return true;
	    case EOpDot:
		oit->GenOpenGLFunc("Dot", node->getType(), 2);
		return true;
	    case EOpCross:
		oit->GenOpenGLFunc("Cross", node->getType(), 2);
		return true;
	    case EOpFaceForward:
		oit->GenOpenGLFunc("FaceForwar", node->getType(), 3);
		return true;
	    case EOpReflect:
		oit->GenOpenGLFunc("Reflect", node->getType(), 2);
		return true;
	    case EOpRefract:
		oit->GenOpenGLFunc("Refract", node->getType(), 3);
		return true;
	    case EOpMul:
		oit->GenOpenGLFunc("MatrixCompMult", node->getType(), 2);
		return true;

	    case EOpConstructStruct:
		oit->GenConstructStruct(node->getType(), node->getLine());
		break;

	    default:
		break;
	}
    }

    return true;
}

bool TllvmOutputTraverser::llvmOutputSelection(bool preVisit,
					       TIntermSelection* node,
					       TIntermTraverser* it)
{
    TllvmOutputTraverser* oit = static_cast<TllvmOutputTraverser*>(it);
    TInfoSink& out = oit->infoSink;
    string label = oit->get_label();
    TllvmVar cond;

    node->getCondition()->traverse(it);
    cond = oit->pop_expstack_rhs();
    if(cond.getType().getBasicType() != EbtBool) {
	out.info.message(EPrefixInternalError,
			 "unsupported type in selection",
			 node->getLine());
	return false;
    }

    if(node->getTrueBlock() || node->getFalseBlock())
	out.code << "\tbr " << cond.getCompleteString().c_str()
		 << " " << cond.getName() << ", label %" << label.c_str()
		 << ".T, label %" << label.c_str() << ".F\n";

    out.code << label.c_str() << ".T:\n";
    if (node->getTrueBlock())
	node->getTrueBlock()->traverse(it);

    if (node->getFalseBlock())
	out.code << "\tbr label %" << label.c_str() << ".E\n";

    out.code << label.c_str() << ".F:\n";
    if (node->getFalseBlock())
	node->getFalseBlock()->traverse(it);

    out.code << label.c_str() << ".E:\n";

    return false;
}

void TllvmOutputTraverser::llvmOutputConstantUnion(TIntermConstantUnion* node,
						   TIntermTraverser* it)
{
    TllvmOutputTraverser* oit = static_cast<TllvmOutputTraverser*>(it);
    TInfoSink& out = oit->infoSink;

    int size = node->getType().getObjectSize();

    for (int i = 0; i < size; i++) {
	switch (node->getUnionArrayPointer()[i].getType()) {
	    case EbtBool:
		oit->push_expstack(node->getUnionArrayPointer()[i].getBConst());
		break;
	    case EbtFloat: /* const float */
		oit->push_expstack(node->getUnionArrayPointer()[i].getFConst());
		break;
	    case EbtInt: /* const int */
		oit->push_expstack(node->getUnionArrayPointer()[i].getIConst());
		break;
	    default:
		out.code.message(EPrefixInternalError,
				 "Unknown constant",
				 node->getLine());
		break;
	}
    }
}

bool TllvmOutputTraverser::llvmOutputLoop(bool preVisit,
					  TIntermLoop* node,
					  TIntermTraverser* it)
{
    TllvmOutputTraverser* oit = static_cast<TllvmOutputTraverser*>(it);
    TInfoSink& out = oit->infoSink;
    string label = oit->get_label();
    oit->into_loop(label);

    if (node->testFirst())
	oit->GenCondition(node, label);

    out.code << label.c_str() << ".B:\n";
    if (node->getBody()) {
	node->getBody()->traverse(it);
    }

    out.code << label.c_str() << ".T:\n";
    if (node->getTerminal())
	node->getTerminal()->traverse(it);

    if (node->testFirst())
	out.code << "\tbr label %" << label.c_str() << ".C\n";

    if (!node->testFirst())
	oit->GenCondition(node, label);

    out.code << label.c_str() << ".E:\n";
    oit->outof_loop();

    return false;
}

bool TllvmOutputTraverser::llvmOutputBranch(bool previsit,
					    TIntermBranch* node,
					    TIntermTraverser* it)
{
    TllvmOutputTraverser* oit = static_cast<TllvmOutputTraverser*>(it);
    TInfoSink& out = oit->infoSink;
    TllvmVar var;

    if(!previsit) {
	switch (node->getFlowOp()) {
	    case EOpBreak:
		out.code << "\tbr label %" << oit->get_loop_label().c_str()
			 << ".E\n";
		break;
	    case EOpContinue:
		out.code << "\tbr label %" << oit->get_loop_label().c_str()
			 << ".T\n";
		break;
	    case EOpKill: /* Not sure what kill does, put it with return for now. */
	    case EOpReturn:
		{
		    TllvmVar funcvar = oit->get_function_var();
		    if (node->getExpression()) {
			var = oit->pop_expstack_rhs();
			if(var.getType() != funcvar.getType() && var.isConst()) {
			    oit->push_expstack(var);
			    if(funcvar.isVector())
				var = oit->ConstructConstVec(funcvar.getType(), node->getLine());
			    else if(funcvar.isMatrix())
				var = oit->ConstructConstMat(funcvar.getType(), node->getLine());
			    else if(funcvar.isStructure())
				var = oit->ConstructConstStruct(funcvar.getType(), node->getLine());
			}
		    }


		    if (node->getExpression()) {
			if(var.getType() != funcvar.getType()) {
			    out.info.message(EPrefixInternalError,
					     "Return value didn't match the function return type",
					     node->getLine());
			    return false;
			}
			out.code << "\tret " << var.getCompleteString()
				 << " " << var.getName() << "\n";
		    }
		    else {
			if(funcvar.getBasicType() != EbtVoid) {
			    out.info.message(EPrefixInternalError,
					     "Return value didn't match the function return type",
					     node->getLine());
			    return false;
			}
			out.code << "\tret void\n";
		    }
		}
		break;

	    default:
		out.code << "Branch: Unknown Branch";
		break;
	}
    }

    return true;
}


