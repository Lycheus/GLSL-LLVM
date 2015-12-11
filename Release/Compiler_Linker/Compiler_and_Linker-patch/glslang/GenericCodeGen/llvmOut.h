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
// This file holds the LLVM code generator class declaration
// Modified from intermOut.cpp which originally written by 3Dlabs
//

#ifndef __LLVMOUT_H_IS_INCLUDDED__
#define __LLVMOUT_H_IS_INCLUDDED__

#include "../Include/intermediate.h"
#include "../Include/InfoSink.h"
#include <sstream>
#include <string>
#include <stack>
#include <vector>
#include <map>
using std::stringstream;
using std::string;
using std::stack;
using std::vector;
using std::map;

#include "llvmVar.h"

//
// Use this class to carry along data from node to node in
// the traversal
//
class TllvmOutputTraverser : public TIntermTraverser {
    private:
	stack<TllvmVar>		expstack;
	stack<string>		tempvars;
	stack<TllvmVar>		func_scope;
	stack<string>		loop_scope;
	//dondum
	//temporary registers
	static const int	tempvar_nr = 1024;
	int			n;
	int			labelnr;
	bool			isparm;
	bool			firstparm;
	bool			isdeclare;
	bool			isinitializer;
	string			str;
	stringstream		ss;
	TllvmVar		var;
	TInfoSinkBase		isb;
	map<TOperator, string>	convopstr;
	map<unsigned int, string>	typemang;

    public:
	TInfoSink& infoSink;

	static string GetBinOp(const TOperator &op, const TBasicType &tpy);
	static string GetCompOp(const TOperator &op, const TBasicType &tpy);

	void GenBinOp(const string &op,
		      const TllvmVar &t,
		      const TllvmVar &s1,
		      const TllvmVar &s2);

	void GenStore(const TllvmVar &ptr, const TllvmVar &val);

	//dondum
	void GenLoad(const TllvmVar &ptr, const TllvmVar &val);
	void GenLoad_M(const TllvmVar &ts, const TllvmVar &val, const int size,const TString &MatrixName);

	void GenExtractSwizzle(const TllvmVar &src,
			       const TllvmVar &res,
			       const TType &origtype,
			       const vector<int> &swi);
	void GenExtractElement(const TllvmVar &vec,
			       const TllvmVar &res,
			       const TType &origtype,
			       const TllvmVar &idx);
	void GenExtractVoM(const TllvmVar &mat,
			   const TllvmVar &resv,
			   const TType &origtype,
			   const TllvmVar &idx);
	void GenExtractEoM(const TllvmVar &mat,
			   const TllvmVar &resv,
			   const TType &origtype,
			   const TllvmVar &idxc,
			   const TllvmVar &idxr);
	bool GenExtractValue(const TllvmVar &agg,
			    const TllvmVar &val,
			    const unsigned int size,
			    const int line);
	bool GenExtractNonAgg(const TllvmVar &base,
			      const TllvmVar &res,
			      const TllvmVarIndexList &idx,
			      const int line);
	bool GenExtractAgg(const TllvmVar &agg,
			   const TllvmVar &res,
			   const int line);

	void GenInsertSwizzle(const TllvmVar &base,
			      const TllvmVar &val,
			      const TType &origtype,
			      const vector<int> &swi);
	void GenInsertElement(const TllvmVar &vec,
			      const TllvmVar &val,
			      const TType &origtype,
			      const TllvmVar &idx);
	void GenInsertVoM(const TllvmVar &mat,
	                  const TllvmVar &valv,
			  const TType &origtype,
			  const TllvmVar &idx);
	void GenInsertEoM(const TllvmVar &mat,
	                  const TllvmVar &vale,
			  const TType &origtype,
			  const TllvmVar &idxc,
			  const TllvmVar &idxr);
	bool GenInsertValue(const TllvmVar &agg,
			    const TllvmVar &val,
			    const unsigned int size,
			    const int line);
	bool GenInsertNonAgg(const TllvmVar &base,
			     const TllvmVar &val,
			     const TllvmVarIndexList &idx,
			     const int line);
	bool GenInsertAgg(const TllvmVar &agg,
			  const TllvmVar &val,
			  const int line);

	bool GenIndex(const TType &type, const int line, const bool isswizzle = false);
	bool GenArithBin(const TIntermBinary* const node);
	bool GenCompare(const TIntermBinary* const node);
	bool GenLogical(const TIntermBinary* const node);
	TllvmVar GenGetRHS(const TllvmVar &ptr_or_val, const int line);
	bool GenAssign(const TType &type, const int line);
	bool GenCalcAssign(const TOperator &op, const TType &type, const int line);

	//dondum
	void GenOpenGLFuncOp(string fnname, const TType &type, const int num);
	void GenCallRegister(const TllvmVar &ptr, const TType &type);
	void GenLoadAttribute(const TllvmVar &tr, const TllvmVar &v);
	void GenLoadUniform(const TllvmVar &tr, const TllvmVar &v);
	void GenLoadVarying(const TllvmVar &tr, const TllvmVar &v);
	void GenVaryingOut(const TllvmVar &dst, const TllvmVar &tr, const TllvmVar &v);
	void GenChangeMatrix(const TllvmVarList &varvec,const TllvmVar &v);
	void GenFuncOp(TllvmVar &rt,
		     const string &fnname,
		     const TllvmVarList &vec);
	void GenENDVertex(const TllvmVar &var1,const TllvmVar &varRet);
	void GenENDFragment(const TllvmVar &var1,const TllvmVar &varRet);
	void GenENDFragment_VectorAssign(const TllvmVar &var1,const TllvmVar &varRet);
	//Edit the end of code.

	void GenFunc(TllvmVar &rt,
		     const string &fnname,
		     const TllvmVarList &vec);
	void GenBinFunc(const TllvmVar &rt, const string &fnname,
		        const TllvmVar &v1, const TllvmVar &v2);
	bool GenBinFuncAssign(string fnname, const int line);
	bool GenBinFuncOp(string fnname, const TType &type, const int line);
	void GenOpenGLFunc(string fnname, const TType &type, const int num);
	bool GenConstructVec(const TType &t, const int line);
	bool GenConstructMat(const TType &t, const int line);
	bool GenConstructStruct(const TType &t, const int line);
	void GenVectorAssign(const TllvmVar &dst, const TllvmVar &src);
	bool GenMatrixAssign(const TllvmVar &dst, const TllvmVar &src, const int line);
	bool GenStructAssign(const TllvmVar &dst, const TllvmVar &src, const int line);
	TllvmVar ConstructConstVec(const TType &t, const int line);
	TllvmVar ConstructConstMat(const TType &t, const int line);
	TllvmVar ConstructConstStruct(const TType &t, const int line);
	void GenCondition(TIntermLoop* node, const string &label);
	bool GenPrePostIncDec(const TOperator &op, const int line);
	void GenConvert(const TOperator &op, const TType &type);

	TllvmOutputTraverser(TInfoSink& i);

	string allocate_tempvar(void);

	const TllvmVar& pop_expstack(void);
	const TllvmVar& pop_expstack_rhs(void);
	void push_expstack(const TllvmVar &v);

	void into_function(const TllvmVar &func);
	void outof_function(void);
	TllvmVar get_function_var(void);

	void into_loop(string);
	void outof_loop(void);
	string get_loop_label(void);

	void into_parameters(void);
	void outof_parameters(void);
	bool is_parameters(void);
	bool is_first_parameter(void);

	void into_declare(void) { isdeclare = true; }
	void outof_declare(void) { isdeclare = false; }
	bool is_declare(void) { return isdeclare; }

	void into_initializer(void) { isinitializer = true; }
	void outof_initializer(void) { isinitializer = false; }
	bool is_initializer(void) { return isinitializer; }

	const string& get_label(void);

	string ConvertMangledFunctionName(string fnname);
	string DepolymorphismFunctionName(string fnname, TllvmVarList cmvec);

	static void llvmOutputSymbol(TIntermSymbol* node, TIntermTraverser* it);
	static bool llvmOutputBinary(bool preVisit, TIntermBinary* node, TIntermTraverser* it);
	static bool llvmOutputUnary(bool preVisit, TIntermUnary* node, TIntermTraverser* it);
	static bool llvmOutputAggregate(bool preVisit, TIntermAggregate* node, TIntermTraverser* it);
	static bool llvmOutputSelection(bool preVisit, TIntermSelection* node, TIntermTraverser* it);
	static void llvmOutputConstantUnion(TIntermConstantUnion* node, TIntermTraverser* it);
	static bool llvmOutputLoop(bool preVisit, TIntermLoop* node, TIntermTraverser* it);
	static bool llvmOutputBranch(bool previsit, TIntermBranch* node, TIntermTraverser* it);

};

#endif

