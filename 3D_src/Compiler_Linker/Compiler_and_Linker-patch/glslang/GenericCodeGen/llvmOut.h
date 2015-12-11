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
#include <iostream>
using namespace std;
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
//#define parameter_with_addr_inside_function //Unfished, Drop method.
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
	//stack<TllvmVar>         store_vars;
	//stack<TllvmVar>         store_vars_vector;
	TllvmVarList            store_vars;
	TllvmVarList            store_vars_vector;
	string                  Recordfnname; //Record the function name.
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
	TString                 tmp_register_counter; // Record the register for saving the register's number in GenInsertElement function.
	int                     FloattoVec;           // e.g. vec3(0.333) => push(0.333) 3 times. Deal this condition. I set this flag.
	map<TOperator, string>	convopstr;
	map<unsigned int, string> typemang;
	map< TString , TString > ret_vector; //Record the name mapping register's number.
	map< TString , TString > ret_name; //Record the name which was declared. 
	map< TString , TString >::iterator it;
	int                     FlagForGenstore;     //Do not print the load instruction for qualifier "inout" or "out". Set 0 to default value.
	int                     lockfnname;     //lockfnname == 0 , can assign new name, otherwise cannot.
	TllvmVarList            Recordparameters; //Record the parameter of function.
	TllvmVarList            RecordParametersforStructure; //Record the parameter of function.
	int                     BreakorContinue;      //"break" come out. set 1 to this flag.
	int                     RecordStructureSize;      //count the number of structures.
	int                     SizeofMatrixCompMult;   //record the size of parameters for matrixcompmult().
	int                     CompareMatrix;   //record to compare matrix elements.
	int                     ReturnVarInTrueBlock; //record whether returning the variable in the true block 
	int                     ReturnVarInFalseBlock;//record whether returning the variable in the flase block 
	int                     ReturnFlag;           //Does it return value. 
	int                     IsGlobalOutsideFunction;//record if we initailize the global variables. default is 1.
	int                     AfterGenCompareInstruction;// default is 0.
    public:
	TInfoSink& infoSink;
	
	static string GetBinOp(const TOperator &op, const TBasicType &tpy);
	static string GetCompOp(const TOperator &op, const TBasicType &tpy);

	void GenBinOp(const string &op,
		      const TllvmVar &t,
		      const TllvmVar &s1,
		      const TllvmVar &s2);

	void GenStore(const TllvmVar &ptr, const TllvmVar &val);
	void GenStoreForInOut(const TllvmVar &ptr, const TllvmVar &val);

	//dondum
	void GenLoad(const TllvmVar &ptr, const TllvmVar &val);
	void GenLoadForInOut( TllvmVar &ptr, TllvmVar &val); //Kaymon

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
	bool GenStructLoadValue(const TllvmVar &agg, //Kaymon
			     const TllvmVar &val,
			     const unsigned int size,
			     const int line);
	bool GenLoadValueFromArrayofMatrix(const TllvmVar &agg, //Kaymon
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
	bool GenArithBin_2(const TIntermBinary* const node, const int num); // Goda 2011-03-23
	bool GenCompare(const TIntermBinary* const node);
	TllvmVar GenCompareMatrix( TllvmVar rhs,TllvmVar lhs); //Kaymon
	void GenRowofMatrix(TString str,TllvmVar row, int number); //Kaymon
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
	bool GenStructAssignPart2(const TllvmVar &dst,const TType &t);
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

	//Kaymon
	char PrefixofVariable(TQualifier t);
	void NameMappingRegister( const TString name , const TString reg ){ 
	  //infoSink.code << "name: " <<name <<" reg: "<<reg<<"\n";
	  ret_vector[ name ] = reg;
	}
	bool DoesNameExist( TString name ){
	  //cout<< "======Does\n";
	  it = ret_vector.find( name );
	  if( it != ret_vector.end() )
	    return true;
	  else
	    return false;
	}//Done
	void LoadReserveVariableFirstTime( TllvmVar &v ){
	  //infoSink.code <<"v.getName : "<<v.getName()<<"\n";
	  //cout<< " +++++++++++Load entry+++++++++\n";
	  //cout <<"v.getName : "<<v.getName()<<"\n";
	  
	  if( DoesNameExist( v.getName() ) ){
	    //cout<< v.getName() <<"\n";
	    v.setName( ret_vector[ v.getName() ] );
	  }
	  else{
	    //cout<< v.getName() << "      Not Exist\n";
	    bool IsNum = true; // Make sure that the string is number or string.                                                                                                                                                    
	    const char *temp;
	    temp = v.getName().c_str();
	    temp++;
	    while( *temp != '\0' ){
	      if( *temp < '0' || *temp > '9'){
		IsNum = false;
		break;
	      }
	      temp++;
	    }// Done                                                                                                                                                                                        
	    //infoSink.code <<"v.getName: "<<v.getName() <<" IsNum? " <<IsNum <<" v.haveIndex(): "<< v.haveIndex() <<" Q:"<< v.getQualifier()<< "\n";     
	    if( !IsNum && strncmp( v.getName().c_str() , "0x" , 2 ) && strncmp( v.getName().c_str() , "::" , 2 )  ){
	      DeclareVariable( v );
	      const TString &str = allocate_tempvar().c_str();
	      TllvmVar ts( v );
	      ts.setName(str);
	      //infoSink.code << ts.getName() <<"\n";
	      if (v.getQualifier() == EvqAttribute && !strncmp( v.getName().c_str() , "@" , 1 ) ){ // It means the vector is belong attribute.
		GenLoadAttribute( ts , v );
	      }
	      else if ( v.getQualifier() == EvqUniform && !strncmp( v.getName().c_str() , "@" , 1 ) ){ //Modify by Kaymon                                                                                                          
		GenLoadUniform( ts , v );
	      }
	      else if ( (v.getQualifier() == EvqTemporary )&& (!strncmp( v.getName().c_str() , "%" , 1 ) ) && (!v.haveIndex())){
		GenLoad( ts , v );
	      }
	      else if( v.getQualifier() == EvqVaryingIn && !strncmp( v.getName().c_str() , "@" , 1 ) ){ //Modify by Kaymon                        
		GenLoadVarying( ts , v );                                                                                                                                                                          
	      }
	      else if( v.getQualifier() == EvqGlobal && !strncmp( v.getName().c_str() , "@" , 1 ) ){ //Modify by Kaymon                                                                                                                       
                GenLoad( ts , v );
              }
	      else if( v.getQualifier() == EvqFace && !strncmp( v.getName().c_str() , "@" , 1 ) ){ //Modify by Kaymon                                                                                                                       
                GenLoad( ts , v );
              }
	      else if( v.getQualifier() == EvqIn && !strncmp( v.getName().c_str() , "%" , 1 ) ){ //Modify by Kaymon
	        GenLoad( ts , v );
	      }
	      else if( v.getQualifier() == EvqOut && !strncmp( v.getName().c_str() , "%" , 1 ) ){ //Modify by Kaymon
	        GenLoadForInOut( ts , v );
		v.changeQualifier( EvqIn );//need to change the qualifier.
		//v.setType( ts.getType() ); 
	      }
#ifdef parameter_with_addr_inside_function
	      else if( v.getQualifier() == EvqInOut && !strncmp( v.getName().c_str() , "%" , 1 ) ){ //Modify by Kaymon
	        GenLoadForInOut( ts , v );
		v.changeQualifier( EvqIn );//need to change the qualifier.
	      }
#else
	      else if( v.getQualifier() == EvqInOut && !strncmp( v.getName().c_str() , "%" , 1 ) ){ //Modify by Kaymon
	        GenLoadForInOut( ts , v );
		v.changeQualifier( EvqIn );//need to change the qualifier. Because qualifier of EvqInOut and EvqOut will generate a star sign.
	      }
#endif
	      else if( v.haveIndex() )
		GenLoadVariablefromIndex( ts, v );
	      else
		infoSink.code <<"Not load successfully\n";
	      //else
	      //GenLoad( ts , v );
	      //infoSink.code << v.getName() <<" "<< str<<"\n";
	      
	      //NameMappingRegister( v.getName() , str ); //Old ,Kaymon , 2013/3/11
	      //v.setName(str);

	      NameMappingRegister( v.getName() , ts.getName() ); 
	      v.setName( ts.getName() );
	    }
	    //cout<< ".............leave load function...........\n";
	  }
	}
	bool DeclareVariable( TllvmVar &V ){

	  size_t index;

	  it = ret_name.find( V.getName() );
	  if( it == ret_name.end() ){


	    //infoSink.code <<"Come in " << V.getName() <<" V.Qualifier(): "<<V.getQualifier()<<"\n";
	    stringstream insertstring;
	    //index = infoSink.code.sink.find("{");
	    index = infoSink.code.sink.find(Recordfnname); //Allocate the space for variables correctly. Indicate the correct position in function. Not the top function.                                                
	    int i = 1;
	    while( infoSink.code.sink[index+i]!='{')
	      i++;
	    if( Recordfnname == "main:void")
	      index += i;
	    else{
	      while( infoSink.code.sink[index+i]!=':')
		i++;
	      index+=i;
	    }


	    if( V.haveIndex() && strncmp( V.getName().c_str() , "@" , 1 ) && V.getIndex()[0].type.isMatrix() ) {//Allocate the variable which have indexes and the name start character of "%". Check variable "v2" whether it is matrix.
	      //insertstring <<"\t"<<V.getName()<<" = alloca "<<V.getIndex()[0].type.getCompleteString()<<"\n";
	      int SizeofMatrix = V.getIndex()[0].type.getCompleteString()[7] - 48 ;
	      while( --SizeofMatrix >= 0 ){
		insertstring <<"\t"<< V.getName()<< "." << SizeofMatrix << " = alloca <"<< V.getIndex()[0].type.getCompleteString()[7] <<" x float>"<<"\n";
		TString MatrixName = V.getName();
		MatrixName += '.';
		MatrixName += SizeofMatrix+48;
		ret_name[ MatrixName ] = "Yes";
	      }
	    }
	    else if( V.haveIndex() && strncmp( V.getName().c_str() , "@" , 1 ) )//Allocate the variable which have indexes and the name start character of "%".                                                                            
	      insertstring <<"\t"<<V.getName()<<" = alloca "<< V.getIndex()[0].type.getCompleteString()<<"\n";
	    else if( !strncmp( V.getName().c_str() , "%" , 1 ) ) //Distinguish the first character of variable names between "@" and "%". Allocate the variable which name start character of "%".                                         
	      insertstring <<"\t" << V.getName() << " = alloca " << V.getCompleteString() <<"\n";



	    infoSink.code.sink.insert( index+2 , insertstring.str() );
	    ret_name[ V.getName() ] = "Yes";
	    
	    //if( strncmp( V.getName().c_str() , "0" , 1 ) && !( V.haveIndex() && V.getIndex()[0].type.isMatrix() ) ) // Filter out the string of IEEE754  and matrix type for v2.             
		//store_vars.push_back( V );

	    return true;
	  }
	  else
	    return false;

	}
	void GenCallFloatRegister(const TllvmVar &ptr, const TType &type); //Kaymon
	void GenLoadVariablefromIndex(const TllvmVar &dst, const TllvmVar &src); //Kaymon
	bool HandleArithmetic( TllvmVar &dst, TllvmVar &src , string opstr ); //Kaymon
	bool ConstructorforStructure( TType curtype ); //Kaymon


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

