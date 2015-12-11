#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#define MAZE 1
#define deal_store_instr
#define new_convert 
#define optimization
//#define trick_for_bool
//#define parameter_with_addr_inside_function //move this definition to header file.
//#define DEBUG_FOR_STORED_VARIABLE           //Store variable before entering a new basic block.
//#define PUSH                                //Debug message for push or pop data.
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
	    // /* add for checking variable need to be add is float or int or else // Goda 2011-03-23
	    if(tpy == EbtFloat)
	        str = "fadd";
	    else if(tpy == EbtInt)
	    //  */
		str = "add";
	    // /* add for checking variable need to be add is float or int or else // Goda 2011-03-23
	    else
		str = "";
	    //  */
	    break;
	case EOpSub:
	case EOpSubAssign:
	    // /* add for checking variable need to be sub is float or int or else // Goda 2011-03-23
	    if(tpy == EbtFloat)
	        str = "fsub";
	    else if(tpy == EbtInt)
	    //  */
	        str = "sub";
	    // /* add for checking variable need to be sub is float or int or else // Goda 2011-03-23
	    else
		str = "";
	    //  */
	    break;
	    return str;
	case EOpMul:
	case EOpMulAssign:
	    // /* add for checking variable need to be mul is float or int or else // Goda 2011-03-23
	    if(tpy == EbtFloat)
	        str = "fmul";
	    else if(tpy == EbtInt)
	    //  */
	        str = "mul";
	    // /* add for checking variable need to be mul is float or int or else // Goda 2011-03-23
	    else
		str = "";
	    //  */
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
	  /*
	  if( tpy == EbtFloat ){
	    str += "eq";
	    break;
	    }*/
	    str += "eq";
	    break;
	case EOpNotEqual:
	  if( tpy == EbtFloat ){
	    str += "one";
	    break;
	  }
	    str += "ne";
	    break;
	case EOpLessThan:
	  if( tpy == EbtFloat ){
	    str += "olt";
	    break;
	  }
	    str += "slt";
	    break;
	case EOpGreaterThan:
	  if( tpy == EbtFloat ){
	    str += "ogt";
	    break;
	  }
	    str += "sgt";
	    break;
	case EOpLessThanEqual:
	  if( tpy == EbtFloat ){
	    str += "ole";
	    break;
	  }
	    str += "sle";
	    break;
	case EOpGreaterThanEqual:
	  if( tpy == EbtFloat ){
	    str += "uge";
	    break;
	  }
	    str += "sge";
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
{                   //arithmetical operation
    char buf[256];

    sprintf(buf, "\t%s = %s %s %s, %s\n",
	    t.getName().c_str(),
	    op.c_str(),
	    s1.getCompleteString().c_str(),
	    s1.getName().c_str(),
	    s2.getName().c_str());
    infoSink.code << buf;
    

}
void TllvmOutputTraverser::GenStoreForInOut(const TllvmVar &ptr,
					    const TllvmVar &val)
{
  TType NewType( EbtFloat , EvqTemporary, ptr.getNominalSize() , false , false , EbpHigh);
  infoSink.code << "\tstore " << NewType.getCompleteString()
		<< " " << val.getName()
		<< ", " << ptr.getCompleteString()
		<< " " << ptr.getName() << "\n"; //disable print star sign ( * ).
  
}
void TllvmOutputTraverser::GenStore(const TllvmVar &ptr,
				    const TllvmVar &val)
{
  //infoSink.code <<" ptr'name: "<< ptr.getName()<<" val's name: " << val.getName() <<" haveIndex?: " <<ptr.haveIndex() << " " << val.haveIndex() <<"\n";
  if( ptr.haveIndex() && val.haveIndex() ){
    TType Newtype( EbtFloat , EvqTemporary, ptr.getIndex()[0].type.getNominalSize() , false , false , EbpHigh);
    if( ptr.getIndex()[0].type.isMatrix() ){
      infoSink.code << "\tstore " << Newtype.getCompleteString()
		    << " " << val.getName()
		    << ", " << Newtype.getCompleteString()
		    << "* " << ptr.getName() << "\n";
    }
    else{
      infoSink.code << "\tstore " << val.getIndex()[0].type.getCompleteString()
		    << " " << val.getName()
		    << ", " << ptr.getIndex()[0].type.getCompleteString()
		    << "* " << ptr.getName() << "\n";
    }
  }
  else{
#ifdef parameter_with_addr_inside_function
    if( FlagForGenstore && ( ptr.getQualifier() == EvqInOut || ptr.getQualifier() == EvqOut ) ){ //For inout or out qualifier.Don't print a load instruction before storing the parameters to new variable's name
      const TString &str = allocate_tempvar().c_str(); 
      TllvmVar tempvar(str,ptr.getType());
      GenLoad( tempvar ,  ptr );
      infoSink.code << "\tstore " << val.getCompleteString()
		    << " " << val.getName()
		    << ", " << ptr.getCompleteString()
		    << " " << tempvar.getName() << "\n"; //Do not print the "*" sign because it already have star sign.
    }
    else{
      infoSink.code << "\tstore " << val.getCompleteString()
		    << " " << val.getName()
		    << ", " << ptr.getCompleteString()
		    << "* " << ptr.getName() << "\n";
    }
    if( !ptr.isVector() && ptr.isNormalType() ){ //only one kind of statements can enter this if statement.Statements is that assigning float or int to variable 
      if( DoesNameExist(ptr.getName()) )
	ret_vector.erase(it); //erase the record of a variable.
    }
#else
    infoSink.code << "\tstore " << val.getCompleteString()
		  << " " << val.getName()
		  << ", " << ptr.getCompleteString()
		  << "* " << ptr.getName() << "\n";
    if( !ptr.isVector() && ptr.isNormalType() ){ //only one kind of statements can enter this if statement.Statements is that assigning float or int to variable 
      if( DoesNameExist(ptr.getName()) )
	ret_vector.erase(it); //erase the record of a variable.
    }
#endif
  }
}
//dondum///////////////////////////////////////////////////////////////////////////////////////////////////
void TllvmOutputTraverser::GenLoad(const TllvmVar &ptr,
				   const TllvmVar &val)
{
  //mark
    int size = val.getType().getNominalSize();
    if( size == 2 || size ==3 )
      size = 4;
    infoSink.code << "\t" << ptr.getName()
		  << " = load " << val.getCompleteString()<<"*"
	          << " " << val.getName()//<<"." Kaymon
		  <<val.getQualifierShortString()<< ", align "<< 4 * size//val.getType().getNominalSize() //Kaymon fix alignment. 2&4=> vector4: align 16 
		  <<"\n";
    
}
#ifdef parameter_with_addr_inside_function
void TllvmOutputTraverser::GenLoadForInOut( TllvmVar &ptr,
					    TllvmVar &val)
{
  //mark
    int size = val.getType().getNominalSize();
    if( size == 2 || size ==3 )
      size = 4;
    infoSink.code << "\t" << ptr.getName()
		  << " = load " << val.getCompleteString()<<"*"// Add one more star sign.
	          << " " << val.getName()//<<"." Kaymon
		  <<val.getQualifierShortString()<< ", align "<< 4 * size //val.getType().getNominalSize() Kaymon 
		  <<"\n";

    val.setName(ptr.getName());
    
    TString str1 = allocate_tempvar().c_str();
    ptr.setName( str1 );

    infoSink.code << "\t" << ptr.getName()
		  << " = load " << val.getCompleteString() 
	          << " " << val.getName()//<<"." Kaymon
		  <<val.getQualifierShortString()<< ", align "<< 4 * size //val.getType().getNominalSize() Kaymon 
		  <<"\n";

    //ptr.changeQualifier( EvqIn ); //Change the qualifier. Do not print with star sign.
}
#else
void TllvmOutputTraverser::GenLoadForInOut(TllvmVar &ptr,
					   TllvmVar &val)
{
  //mark
    int size = val.getType().getNominalSize();
    if( size == 2 || size ==3 )
      size = 4;
    infoSink.code << "\t" << ptr.getName()
		  << " = load " << val.getCompleteString()
	          << " " << val.getName()//<<"." Kaymon
		  <<val.getQualifierShortString()<< ", align "<< 4 * size//val.getType().getNominalSize() //Kaymon fix alignment. 2&4=> vector4: align 16 
		  <<"\n";
    
}
#endif
//#if 0
void TllvmOutputTraverser::GenLoad_M(const TllvmVar &ts,
				     const TllvmVar &val,
				     const int size,const TString &MatrixName)
{
  //  std::cout<< MatrixName <<std::endl;
  //  MatrixName = '@' + MatrixName ;
	 infoSink.code << "\t" << ts.getName()
		  << " = load " << val.getCompleteString()<<"*"
		       << " " << MatrixName/*val.getName()*/<<"."<<val.getQualifierShortString()<<"."<<size <<"."<<val.getNominalSize()
		       << ", align 16"/*<< 4 * val.getType().getNominalSize() */<<"\n";
	   

}
//#endif
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void TllvmOutputTraverser::GenExtractSwizzle(const TllvmVar &src,
					     const TllvmVar &res,
					     const TType &origtype,
					     const vector<int> &swi)
{/* Original code.
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
    */

#ifdef optimization
    int i;
    //int size = res.getType().getNominalSize();
    int size = swi.size();
    //format : @OpenGLES.Extractelement."size"."swizzle".
    //8 bits for "swizzle". each 2 bits indicate x,y,z,w.
    TString fnname = "@OpenGLES.Extractelement."; 
    /*
    for( i = 0 ; i < size ; i++ ){
      fnname += swi[i] + 48;
      if( i == size-1 )
	break;
      fnname += '.';
    }
    */
    fnname += size + 48;
    fnname += '.';

    char swizzle=0;
    for( i = 0 ; i < 4 ; i++ ){
      if( i < size )
	swizzle |= swi[i];
      else 
	swizzle |= 0;
      if( i != 3 ) //Maximum left shift is three.
	swizzle = swizzle << 2;
    }
//infoSink.code << swizzle<<"\n";
    //infoSink.code << (int)size<<"\n";
    char num[4];
    unsigned int kk = 0;
    kk =swizzle&0x000000FF; //avoid neg number.
    sprintf(num,"%d",kk);
    fnname = fnname + num ;
    infoSink.code << "\t" << res.getName()
		  << " = call " << origtype.getCompleteString() << " " << fnname << "(" << src.getCompleteString() << " " <<src.getName() << ")" << "\n";
    tmp_register_counter = res.getName(); //save the final dst's register number. 

#else 
    int i;
    int size = res.getType().getNominalSize();
    //infoSink.code <<"Here\n";
    //infoSink.code <<(int )res.getIndex().size() <<"\n";
    //TllvmVar dstforextract(res);
    TllvmVar dstforinsert(res);
    //TString registername1 = "%Get";
    TString registername2 = "%TmpVar";
    //dstforextract.setName(registername1);
    dstforinsert.setName(registername2);
    DeclareVariable(dstforinsert);

    GenLoad( res , dstforinsert );      
    dstforinsert.setName(res.getName());
    //infoSink.code <<"size:"<<size<<"\n";

    
    for(i=0;i<size;i++){
      const TString &str = allocate_tempvar().c_str(); //for extractelement
      const TString &str1 = allocate_tempvar().c_str(); //for insertelement

      infoSink.code << "\t" << str 
		    << " = extractelement " << origtype.getCompleteString()
		    << " " << src.getName() << ", "
		    << "i32 " << swi[i]<<"\n";

      if( i != size -1 ){
	infoSink.code << "\t" << str1 
		      << " = insertelement " << origtype.getCompleteString()
		      << " " << dstforinsert.getName() << ", ";
      }
      else{
	infoSink.code << "\t" << str1 
		      << " = insertelement " << origtype.getCompleteString()
		      << " " << dstforinsert.getName() << ", ";
      }  

      dstforinsert.setName(str1);
      tmp_register_counter = str1; //save the final dst's register number. 
      if(res.getCompleteString()[11] == 'f' ) // check whether the vector's type
	infoSink.code << "float ";
      else
	infoSink.code << "il ";

      infoSink.code << str
	//<< ", i32 " << swi[i]<<"\n";
                    << ", i32 " << i <<"\n"; // for vec3 t = a.gbr;  We should save the elements sequentially.
    }
    
#endif
    
}

void TllvmOutputTraverser::GenExtractElement(const TllvmVar &vec,
					     const TllvmVar &res,
					     const TType &origtype,
					     const TllvmVar &idx)
{
  tmp_register_counter = res.getName(); //I think add this variable might be ok.
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
      //<< " = extractvector " << origtype.getCompleteString() 
		  << " = extractelement " << origtype.getCompleteString() //replace the instruction opcode "extractvector" with "extractelement". Kaymon 
	          << " " << mat.getName() << ", "
		  << "i32 " << idx.getName() << "\n";
}
//e.g: m[0][1] m[2][1]...etc.
void TllvmOutputTraverser::GenExtractEoM(const TllvmVar &mat,
					 const TllvmVar &rese,
					 const TType &origtype,
					 const TllvmVar &idxc,
					 const TllvmVar &idxr)
{
  /* // *************************Original code ************************************
    infoSink.code << "\t" << rese.getName()
		  << " = extractelement " << origtype.getCompleteString()
	          << " " << mat.getName() << ", "
		  << "<2 x i32> <i32 " << idxc.getName()
		  << ", i32 " << idxr.getName() << ">\n";
  */
  //*******************Fixed size ****************************//
#ifndef NewMatrix
  if( mat.getIndex()[0].type.getQualifier() == EvqUniform ){
    /*
    infoSink.code << mat.getName() 
		  << " " <<mat.getQualifier() 
		  << " " << mat.getCompleteString()
		  << " " << mat.isMatrix()
		  << " " << mat.getIndex()[0].type.isMatrix() << "\n";
    */
    const TString &str = allocate_tempvar().c_str(); // For extractelement instruction.
    TllvmVar temp( str , mat.getType());
    tmp_register_counter = str;

    infoSink.code << "\t" << rese.getName() << " = load <4 x float>* "
		  << mat.getName() << ".u." << idxc.getName() << ".4, align 16\n";

    infoSink.code << "\t" << temp.getName()
		  << " = extractelement " << "<4 x float>"/*origtype.getCompleteString()*/ //Fix the size to vec4 Kaymon
	          << " " << rese.getName() << ", "
		  << "i32 " << idxr.getName() << "\n";
 
  }
  else if( mat.getIndex()[0].type.getQualifier() == EvqTemporary ){
    /*
    TString VectorNumber;
    
    TString NameOfMatrix = mat.getName();
    NameOfMatrix += '.';
    NameOfMatrix += idxc.getName();
    TllvmVar temp( NameOfMatrix , mat.getType() );
    
    infoSink.code << rese.getName() <<" type:"<< rese.getCompleteString() << " matrix??:" << rese.isMatrix() << "\n";
    infoSink.code << temp.getName() <<" type:"<< temp.getCompleteString() << " matrix??:" << temp.isMatrix() << "\n";
    
    LoadReserveVariableFirstTime( temp );

    if( DoesNameExist( NameOfMatrix ) ){
      VectorNumber = rese.getName();
    }
    else {
      const TString &str = allocate_tempvar().c_str(); // For extractelement instruction.
      VectorNumber = str;
      tmp_register_counter = str;
    }
      
    tmp_register_counter = VectorNumber;
*/
    //infoSink.code << "\t" << VectorNumber

    TllvmVar temp( mat.getName() , mat.getType() );
    tmp_register_counter = rese.getName();
    infoSink.code << "\t" << rese.getName()
		  << " = extractelement " << "<4 x float>"/*origtype.getCompleteString()*/ //Fix the size to vec4 Kaymon
	          << " " << temp.getName() << ", "
		  << "i32 " << idxr.getName() << "\n";
  }
#else
  //infoSink.code << mat.getName() <<"\n"; 
  //infoSink.code << mat.getIndex()[0].type.getCompleteString() <<"\n"; 
  //infoSink.code << mat.getIndex()[1].type.getCompleteString() <<"\n"; 
  //infoSink.code << rese.getName() <<"\n";
  //**************Level 1***************
  //TllvmVar Newmat(  mat );
  //LoadReserveVariableFirstTime( Newmat );
  if(mat.getIndex()[0].type.getQualifier()==EvqUniform){
    infoSink.code << "\t" << rese.getName()
		  << " = getelementptr inbounds " << mat.getIndex()[0].type.getCompleteString() //Change the Name from extractvalue to  getelementptr
		  << "* " << mat.getName() <<".u.0."<<mat.getIndex()[0].type.getNominalSize()//fixing this part kaymon by adding the attribute or uniform. 
		  <<", i32 0, i32 " << mat.getIndex()[0].index.getName(); // Add the type of index.
    infoSink.code << "\n";  
  }
  else{
    infoSink.code << "\t" << rese.getName()
		  << " = getelementptr inbounds " << mat.getIndex()[0].type.getCompleteString() //Change the Name from extractvalue to  getelementptr
		  << "* " << mat.getName() 
		  <<", i32 0, i32 " << mat.getIndex()[0].index.getName(); // Add the type of index.
    infoSink.code << "\n";  
  }
  const TString &str = allocate_tempvar().c_str(); // dst'register of load instruction.
  TllvmVar temp(  rese.getName() , mat.getIndex()[1].type );
  TllvmVar temp1(  str , mat.getIndex()[1].type );

  GenLoad(temp1,temp);

  //*************Level 2****************
  const TString &str1 = allocate_tempvar().c_str(); // dst'register of extractelement instruction.
  TllvmVar temp2(  str1 , mat.getIndex()[1].type );
  tmp_register_counter = temp2.getName();
  infoSink.code << "\t" << temp2.getName()
		<< " = extractelement " << mat.getIndex()[1].type.getCompleteString()
		<< " " << temp1.getName() << ", "
		<< "i32 " << mat.getIndex()[1].index.getName() << "\n";
#endif

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
		  << " = getelementptr inbounds " << il[0].type.getCompleteString() //Change the Name from extractvalue to  getelementptr
	          << "* " << agg.getName(); //fixing this part kaymon by adding the attribute or uniform. 


    if( il[0].type.getQualifier() == EvqUniform && il[0].type.isArray() ){ //only handle one dimetional.
      if(il[0].type.isMatrix())
	infoSink.code <<  ".u.a." << il[0].type.getArraySize()*agg.getNominalSize();//each row of matrix is vector. There are many rows in a matrix. So the size is array size* rows.
      else
	infoSink.code <<  ".u.a." << il[0].type.getArraySize();
    }
    else if( il[0].type.getQualifier() == EvqAttribute && il[0].type.isArray() )
      infoSink.code <<  ".a.a." << il[0].type.getArraySize();

    for(i=0;i<size;++i){
      infoSink.code << ", i32 0, i32 " << il[i].index.getName(); // Add the type of index.
    }

    infoSink.code << "\n";
    
    

    if( agg.getIndex()[0].type.isStructure() && agg.getIndex().size() == 2 ){
      tmp_register_counter = allocate_tempvar().c_str();  
      TllvmVar tgt( tmp_register_counter.c_str(), res.getType() ); 
      TllvmVar tmp_var( res.getName() , agg.getIndex()[1].type );
      GenLoad( tgt , tmp_var  ); //We should load the variable before we excute the mathematical operations.
    }
#ifdef NewMatrix
    else if(agg.getIndex()[0].type.isArray() && agg.getIndex()[0].type.isMatrix() ){//For matrix array.
      tmp_register_counter =res.getName();  
    }
#endif
    else{
      tmp_register_counter = allocate_tempvar().c_str();  
      TllvmVar tgt( tmp_register_counter.c_str(), res.getType() ); 
      GenLoad( tgt , res ); //We should load the variable before we excute the mathematical operations.
    }
    //infoSink.code << tmp_register_counter<<"qqqqqqqqqq\n";
    //infoSink.code << res.getType().getCompleteString()<<" " << agg.getType().getCompleteString() <<"\n";
    return true;
}
bool TllvmOutputTraverser::GenStructLoadValue(const TllvmVar &agg,
					  const TllvmVar &res,
					  const unsigned int size,
					  const int line)
{
    TllvmVarIndexList il = agg.getIndex();
    unsigned int i;
    unsigned int SizeofIndex = il.size() ;
    //infoSink.code <<"\n\nIn GenstructLoadValue: " <<"\n";
    //infoSink.code << " Total size : " << SizeofIndex << " fng:" << size <<"\n";
    
    if(il.size()==0 || size > il.size()) {
	infoSink.info.message(EPrefixInternalError,
			      "Index of Aggregate type out of range",
			      line);
	return false;
    }
    
    if( SizeofIndex > size ){
      TllvmVar tempv(res);
      infoSink.code << "\t" << tempv.getName()
		    << " = getelementptr inbounds " << il[0].type.getCompleteString() //Change the Name from extractvalue to  getelementptr
		    << "* " << agg.getName() //fixing this part kaymon by adding the attribute or uniform. 
		    <<", i32 0, i32 " << il[0].index.getName(); // Add the type of index.
      infoSink.code << "\n";
      TllvmVar Tmp_tempv(tempv) ;
      
      for( i = 1 ; i <= size ; i++ ){
	
	if( i < size ){
	  tempv.setName( allocate_tempvar().c_str() );  
	  infoSink.code << "\t" << tempv.getName()
			<< " = getelementptr inbounds " << il[i].type.getCompleteString() //Change the Name from extractvalue to  getelementptr
			<< "* " <<  Tmp_tempv.getName()//fixing this part kaymon by adding the attribute or uniform. 
			<<", i32 0, i32 " << il[i].index.getName(); // Add the type of index.
	  infoSink.code << "\n";
	}
	else{ //e.g. s.a[0]. indicate the first [0]. But might be two dimensional array ( Matrix type). e.g. s.a[0][0].

	  if( il[i].type.isMatrix() ){ //e.g. s.a[0][0]
	    
	    Tmp_tempv.setName( tempv.getName() );
	    tempv.setName( allocate_tempvar().c_str() );  
	    infoSink.code << "\t" << tempv.getName()
			  << " = getelementptr inbounds " << il[i].type.getCompleteString() //Change the Name from extractvalue to  getelementptr
			  << "* " <<  Tmp_tempv.getName()//fixing this part kaymon by adding the attribute or uniform. 
			<<", i32 0, i32 " << il[i].index.getName(); // Add the type of index.
	    infoSink.code << "\n";
	    i++; //indicate the next indexunit. the second [] of two dimensional.
	    Tmp_tempv.setName( tempv.getName() );
	  }
	  
	  tempv.setName( allocate_tempvar().c_str() );//register's number + 1  
	  Tmp_tempv.setType( il[i].type ); //Only set up the type, so the register's number is old.
	  GenLoad( tempv , Tmp_tempv );
	
	  Tmp_tempv.setName( tempv.getName() );  //Save the previous register's number.
	  tempv.setName( allocate_tempvar().c_str() );//register's number + 1  
	  GenExtractElement( Tmp_tempv , tempv, il[i].type ,il[i].index );
	  
	
	
	}
	Tmp_tempv.setName(  tempv.getName());
    
      }
    }
    else{
      TllvmVar tempv(res);
      TllvmVar Tmp_tempv(tempv) ;


      infoSink.code << "\t" << tempv.getName()
		    << " = getelementptr inbounds " << il[0].type.getCompleteString() //Change the Name from extractvalue to  getelementptr
		    << "* " << agg.getName() //fixing this part kaymon by adding the attribute or uniform. 
		    <<", i32 0, i32 " << il[0].index.getName(); // Add the type of index.
      infoSink.code << "\n";
      
      for( i = 1 ; i < size ; i++ ){
	

	tempv.setName( allocate_tempvar().c_str() );  
	  infoSink.code << "\t" << tempv.getName()
			<< " = getelementptr inbounds " << il[i].type.getCompleteString() //Change the Name from extractvalue to  getelementptr
			<< "* " <<  Tmp_tempv.getName()//fixing this part kaymon by adding the attribute or uniform. 
			<<", i32 0, i32 " << il[i].index.getName(); // Add the type of index.
	  infoSink.code << "\n";

	  Tmp_tempv.setName(  tempv.getName());
    
      }
      tempv.setName( allocate_tempvar().c_str() );//register's number + 1  
      Tmp_tempv.setType( res.getType() ); //Only set up the type, so the register's number is old.
      GenLoad( tempv , Tmp_tempv );

      tmp_register_counter = tempv.getName();  
    }
    
    return true;

}
bool TllvmOutputTraverser::GenLoadValueFromArrayofMatrix(const TllvmVar &agg,
							 const TllvmVar &res,
							 const unsigned int size,
							 const int line)
{
  /******* Load value from array of matrix. Now just get the vector element. See below.
   
	   mat3 testmat3[2]; 
	   result += testmat3[0][j] + testmat3[1][j];]

   ***/
  TllvmVarIndexList il = agg.getIndex();
  
  infoSink.code << "\t" << res.getName()
		<< " = getelementptr inbounds " << il[0].type.getCompleteString() //Change the Name from extractvalue to  getelementptr
		<< "* " << agg.getName(); //fixing this part kaymon by adding the attribute or uniform. 
  if(il[0].type.isArray()&&il[0].type.getQualifier()==EvqUniform)
    infoSink.code<<".u.a."<<il[0].type.getArraySize()*agg.getNominalSize()<<", i32 0, i32 " << il[0].index.getName(); // Add the type of index.
  else
    infoSink.code<<", i32 0, i32 " << il[0].index.getName(); // Add the type of index.
  infoSink.code << "\n";

  TllvmVar tempv(res);
  LoadReserveVariableFirstTime(il[1].index); //load the index first. e.g. testmat3[0][j] <= load j first.

  tempv.setName( allocate_tempvar().c_str() );  
  infoSink.code << "\t" << tempv.getName()
		<< " = getelementptr inbounds " << il[1].type.getCompleteString() //Change the Name from extractvalue to  getelementptr
		<< "* " << res.getName() //fixing this part kaymon by adding the attribute or uniform. 
		<<", i32 0, i32 " << il[1].index.getName(); // Add the type of index.
  infoSink.code << "\n";
  

  TllvmVar Tmp_tempv(tempv);
  Tmp_tempv.setType( res.getType() ); //Only set up the type, so the register's number is old.
  tempv.setName( allocate_tempvar().c_str() );//register's number + 1  
  GenLoad( tempv , Tmp_tempv );
  
  tmp_register_counter = tempv.getName();  

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
	} // Kaymon change the type.
  //see
    int size = agg.getIndex().size();
    int fng = agg.getFirstNonAggPos(); //This function means to get first position of non agg from this variable's member called indexlist.
                                       // Note: Array and structure are aggregate types. Matrix is not agge type.(even implement matrix by vector array.) 
    //infoSink.code <<"ExtractAgg=> size: "<< size <<" fng: "<< fng <<"\n";
    //infoSink.code <<"reg(dst): " << res.getName() <<"\n";

    if(fng<size) {
	TllvmVarIndexList idx;
	for(int i=fng;i<size;++i)
	    idx.push_back((agg.getIndex())[i]);

	if(fng>0) {    //e.g.  s.a[0].   s is a structure.
	  /*                //original code.
	  TllvmVar tv(allocate_tempvar().c_str(),
		      (agg.getIndex())[fng].type);
	    return
	      GenExtractValue(agg, tv, fng, line) && //original code.
	      GenExtractNonAgg(tv, res, idx, line);
	  */
	  /*
	  infoSink.code << "res: " << res.getName() <<" res.type: " << res.getCompleteString() <<" res.haveindex(): "<< res.haveIndex() <<"\n";
	  infoSink.code << "agg.name: " << agg.getName() <<" agg.type: " << agg.getCompleteString() <<" agg.haveindex(): "<< agg.haveIndex() <<"\n";
	  infoSink.code << "agg.index.0 : " << agg.getIndex()[0].index.getName() <<" agg.type.0: " << agg.getIndex()[0].type.getCompleteString() <<"\n";
	  if( size == 2 )
	    infoSink.code << "agg.index.1 : " << agg.getIndex()[1].index.getName() <<" agg.type.1: " << agg.getIndex()[1].type.getCompleteString() <<"\n";
	  if( size == 3 ){
	    infoSink.code << "agg.index.1 : " << agg.getIndex()[1].index.getName() <<" agg.type.1: " << agg.getIndex()[1].type.getCompleteString() <<"\n";
	    infoSink.code << "agg.index.2 : " << agg.getIndex()[2].index.getName() <<" agg.type.2: " << agg.getIndex()[2].type.getCompleteString() <<"\n";
	  }
	  if( size == 4 ){
	    infoSink.code << "agg.index.1 : " << agg.getIndex()[1].index.getName() <<" agg.type.1: " << agg.getIndex()[1].type.getCompleteString() <<"\n";
	    infoSink.code << "agg.index.2 : " << agg.getIndex()[2].index.getName() <<" agg.type.2: " << agg.getIndex()[2].type.getCompleteString() <<"\n";
	    infoSink.code << "agg.index.3 : " << agg.getIndex()[3].index.getName() <<" agg.type.3: " << agg.getIndex()[3].type.getCompleteString() <<"\n";
	  }
	  */
	  bool valuereturned;
	  if( agg.getIndex()[0].type.isArray() && agg.getIndex()[0].type.isMatrix() && size == 2){
	    return GenLoadValueFromArrayofMatrix(agg, res, fng, line);  
	  }
	  else if( size == 2 ){ //for s2.a[0]. s2 is a structure.
	    valuereturned = GenExtractValue(agg, res, fng, line);  
	    //TllvmVar tv(allocate_tempvar().c_str(),(agg.getIndex())[fng].type); //old 
	    //TllvmVar tmp_var( tmp_register_counter ,(agg.getIndex())[fng].type);
	    TllvmVar tv(allocate_tempvar().c_str(), agg.getType() ); //for texure[0].xy
	    TllvmVar tmp_var( tmp_register_counter , agg.getType() );//
	    return valuereturned && GenExtractNonAgg( tmp_var, tv , idx, line);
	  }
	  else{
	    valuereturned = GenStructLoadValue(agg, res, fng, line);  
	    //TllvmVar tv(allocate_tempvar().c_str(),(agg.getIndex())[fng].type);
	    //TllvmVar tmp_var( tmp_register_counter ,(agg.getIndex())[fng].type);
	    return valuereturned;// && GenExtractNonAgg( tmp_var, tv , idx, line);
	  }
	}
	else{
	  /*
	  infoSink.code << "agg.name: " << agg.getName() <<" agg.type: " << agg.getCompleteString() <<" agg.haveindex(): "<< agg.haveIndex() <<"\n";
	  infoSink.code << "agg.index.0 : " << agg.getIndex()[0].index.getName() <<" agg.type.0: " << agg.getIndex()[0].type.getCompleteString() <<"\n";
	  if( size == 2 ){
	    infoSink.code << "agg.index.1 : " << agg.getIndex()[1].index.getName() <<" agg.type.1: " << agg.getIndex()[1].type.getCompleteString() <<"\n";
	    infoSink.code << agg.getIndex()[1].type.isNonAgg() <<"\n";
	    infoSink.code << agg.getIndex()[0].type.isNonAgg() <<"\n";
	    infoSink.code << agg.getIndex()[1].index.isSwizzle() <<"\n";
	    infoSink.code << agg.getIndex()[0].index.isSwizzle() <<"\n";
	    }*/
	  return GenExtractNonAgg(agg, res, idx, line);
	}
    }
    else{

      /*
      infoSink.code << "res: " << res.getName() <<" res.type: " << res.getCompleteString() <<" res.haveindex(): "<< res.haveIndex() <<"\n";
      infoSink.code << "agg.name: " << agg.getName() <<" agg.type: " << agg.getCompleteString() <<" agg.haveindex(): "<< agg.haveIndex() <<"\n";
      infoSink.code << "agg.index.0 : " << agg.getIndex()[0].index.getName() <<" agg.type.0: " << agg.getIndex()[0].type.getCompleteString() <<"\n";
      infoSink.code << " Ononagg: " << agg.getType().isNonAgg() <<"\n";
      infoSink.code << " OArray: " << agg.getType().isArray() <<"\n";
      infoSink.code << " OMatrix: " << agg.getType().isMatrix() <<"\n";
      infoSink.code << " OStruc: " << agg.getType().isStructure() <<"\n";

      infoSink.code << " nonagg: " << agg.getIndex()[0].type.isNonAgg() <<"\n";
      infoSink.code << " Array: " << agg.getIndex()[0].type.isArray() <<"\n";
      infoSink.code << " Matrix: " << agg.getIndex()[0].type.isMatrix() <<"\n";
      infoSink.code << " Struc: " << agg.getIndex()[0].type.isStructure() <<"\n";
      if( size == 2 ){
	infoSink.code << "agg.index.1 : " << agg.getIndex()[1].index.getName() <<" agg.type.1: " << agg.getIndex()[1].type.getCompleteString() <<"\n";
	infoSink.code << agg.getIndex()[1].index.isSwizzle() <<"\n";
	infoSink.code << agg.getIndex()[0].index.isSwizzle() <<"\n";
      }
	if( size == 3 ){
	infoSink.code << "agg.index.1 : " << agg.getIndex()[1].index.getName() <<" agg.type.1: " << agg.getIndex()[1].type.getCompleteString() <<"\n";
	infoSink.code << "agg.index.2 : " << agg.getIndex()[2].index.getName() <<" agg.type.2: " << agg.getIndex()[2].type.getCompleteString() <<"\n";
      }
      if( size == 4 ){
	infoSink.code << "agg.index.1 : " << agg.getIndex()[1].index.getName() <<" agg.type.1: " << agg.getIndex()[1].type.getCompleteString() <<"\n";
	infoSink.code << "agg.index.2 : " << agg.getIndex()[2].index.getName() <<" agg.type.2: " << agg.getIndex()[2].type.getCompleteString() <<"\n";
	infoSink.code << "agg.index.3 : " << agg.getIndex()[3].index.getName() <<" agg.type.3: " << agg.getIndex()[3].type.getCompleteString() <<"\n";
      }
      */
      if( size < 2 ) // for s2.a;
	return GenExtractValue(agg, res, size, line);
      else{ //for s2.a.b. (A structure contains  a structure.)
	return  GenStructLoadValue(agg, res, size, line);  
      }
    }

}
//For temp.xyz = temp1.xyz;
void TllvmOutputTraverser::GenInsertSwizzle(const TllvmVar &base,
					    const TllvmVar &val,
					    const TType &origtype,
					    const vector<int> &swi)
{
#ifdef optimization
    int i;
    //int size = res.getType().getNominalSize();
    int size = swi.size();
    //format : @OpenGLES.Insertelement."size"."swizzle".
    //8 bits for "swizzle". each 2 bits indicate x,y,z,w.
    TString fnname = "@OpenGLES.Insertelement."; 
    /*
    for( i = 0 ; i < size ; i++ ){
      fnname += swi[i] + 48;
      if( i == size-1 )
	break;
      fnname += '.';
    }
    */
    fnname += size + 48;
    fnname += '.';

    char swizzle=0;
    for( i = 0 ; i < 4 ; i++ ){
      if( i < size )
	swizzle |= swi[i];
      else 
	swizzle |= 0;
      if( i != 3 ) //Maximum left shift is three.
	swizzle = swizzle << 2;
    }
//infoSink.code << swizzle<<"\n";
    //infoSink.code << (int)size<<"\n";
    char num[4];
    unsigned int kk = 0;
    kk =swizzle&0x000000FF; //avoid neg number.
    sprintf(num,"%d",kk);
    fnname = fnname + num ;
    const TString &str = allocate_tempvar().c_str();
    TllvmVar rd( str , origtype);
    tmp_register_counter = str; //save the last virtual register.

    infoSink.code << "\t" << rd.getName()
		  << " = call " << origtype.getCompleteString() << " " << fnname << "(" <<base.getCompleteString()<<" "<<base.getName()<<", "<< val.getCompleteString() << " " <<val.getName() << ")" << "\n";
    //tmp_register_counter = val.getName(); //save the final dst's register number. 

#else 
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
#endif
}

void TllvmOutputTraverser::GenInsertElement(const TllvmVar &vec,
					    const TllvmVar &val,
					    const TType &origtype,
					    const TllvmVar &idx)
{
  
  TllvmVar ChangedVar = val; // Add for handling the variable which was not loaded.

  if( !strncmp( val.getName().c_str() , "%" , 1 ) ||  !strncmp( val.getName().c_str() , "@" , 1 ) ) //Filter out the string of IEEE 754.) //Filter out the string of IEEE 754.  
    LoadReserveVariableFirstTime( ChangedVar );



  const TString &str = allocate_tempvar().c_str();
  TllvmVar rd( str , origtype);
  tmp_register_counter = str;

  
  

  infoSink.code << "\t" << rd.getName()//vec.getName()
		<< " = insertelement " << origtype.getCompleteString()
		<< " " << vec.getName()
		<< ", " << val.getCompleteString() << " " << ChangedVar.getName() //val.getName() Original code
		<< ", i32 " << idx.getName() << "\n";
}


void TllvmOutputTraverser::GenInsertVoM(const TllvmVar &mat,
					const TllvmVar &valv,
					const TType &origtype,
					const TllvmVar &idx)
{
  /*//original code
    infoSink.code << "\t" << mat.getName()
		  << " = insertvector " << origtype.getCompleteString()
	          << " " << mat.getName() << ", "
		  << valv.getCompleteString() << " " << valv.getName()
		  << ", i32 " << idx.getName() << "\n";
  */
#ifndef NewMatrix
  TllvmVar temp(valv);
  //infoSink.code << "\t" << mat.getName()<<" "<< mat.isMatrix() << " " <<mat.haveIndex() <<"\n";
  //infoSink.code << mat.getIndex()[0].index.getName()<<"\n";
  /*
  if( v.getQualifier() == EvqTemporary ){ //For the local matrix. Local matrix is defined by three vec4s, so we need to change the matrix type to vec4.
    TString NameOfMatrix = MatrixName;
    NameOfMatrix += '.';
    NameOfMatrix += j+48;
    
    //infoSink.code << "\t" << ts.getName() << " = load <"<< v.getCompleteString()[7] << " x float>* " << NameOfMatrix<<", align 16\n";
    
    TType Newtype( EbtFloat , EvqTemporary, k , false , false , EbpHigh);
    //infoSink.code << Newtype.getCompleteString() << "\n";
    v.setType( Newtype );
    //infoSink.code << NameOfMatrix <<"\n";
    v.setName( NameOfMatrix );
    LoadReserveVariableFirstTime( v );

    }*/
  TString NameOfMatrix = mat.getName() + "." + mat.getIndex()[0].index.getName();
  //infoSink.code << NameOfMatrix <<"\n";
  LoadReserveVariableFirstTime( temp );
  //infoSink.code << temp.getName() <<"\n";
  NameMappingRegister(NameOfMatrix,temp.getName());
#else
#endif

}

void TllvmOutputTraverser::GenInsertEoM(const TllvmVar &mat,
					const TllvmVar &vale,
					const TType &origtype,
					const TllvmVar &idxc,
					const TllvmVar &idxr)
{
   //*********************Original code*************************
  /*
    infoSink.code << "\t" << mat.getName()
		  << " = insertelement " << origtype.getCompleteString()
	          << " " << mat.getName() << ", "
		  << vale.getCompleteString() << " " << vale.getName()
		  << ", <2 x i32> <i32 " << idxc.getName()
		  << ", i32 " << idxr.getName() << ">\n";
  */
    //**********************Fixed size **************
  
#ifdef NewMatrix
  //infoSink.code << mat.getName() << " " << vale.getName() << " " << "\n";
  //infoSink.code << idxc.getName() << " " << idxr.getName() << " " << "\n";
  //infoSink.code << (int)mat.getIndex().size() <<"\n";
  //infoSink.code << mat.getIndex()[0].type.getCompleteString() <<"\n";
  //infoSink.code << mat.getIndex()[1].type.getCompleteString() <<"\n";
  
  const TString &str = allocate_tempvar().c_str(); //For first instruction
  TllvmVar tempv(mat.getName(),mat.getIndex()[1].type);
  tempv.setName(str);
  infoSink.code << "\t" << tempv.getName()
		<< " = getelementptr inbounds " << origtype.getCompleteString()  //Change the Name from extractvalue to  getelementptr
		<< "* " << mat.getName() //fixing this part kaymon by adding the attribute or uniform. 
		<<", i32 0, i32 " << idxc.getName(); // Add the type of index.
  infoSink.code << "\n";

  const TString &str1 = allocate_tempvar().c_str(); //For first instruction
  TllvmVar Tmp_tempv(mat);
  Tmp_tempv.setName(str1);
  GenLoad( Tmp_tempv ,tempv );
  tempv.setName(str1);

  const TString &str2 = allocate_tempvar().c_str(); //For first instruction
  Tmp_tempv.setName(str2);  
  infoSink.code << "\t" << Tmp_tempv.getName()
		<< " = insertelement " << "<4 x float>"/*origtype.getCompleteString()*/
  		<< " " << tempv.getName() << ", "
		<< vale.getCompleteString() << " " << vale.getName()
		<< ", i32 " << idxr.getName() << "\n";
  
#else
  if( mat.getIndex()[0].type.getQualifier() == EvqUniform ){
    //infoSink.code << "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHere U?\n";
      const TString &str = allocate_tempvar().c_str(); //For first instruction
      TllvmVar temp( mat);
      temp.setName(str);
      const TString &str1 = allocate_tempvar().c_str(); //For second instruction
      TllvmVar temp1( mat );
      temp1.setName(str1);
      //tmp_register_counter = str;
      TllvmVar temp2( vale);

      LoadReserveVariableFirstTime( temp2 );
      //infoSink.code << mat.getName() << " " << vale.getName() <<"\n";
      
      infoSink.code << "\t" << temp.getName() <<" = load <4 x float>* "
		    << mat.getName() << ".u." << idxc.getName() << ".4, align 16\n";
      
      infoSink.code << "\t" << temp1.getName()
		    << " = insertelement " << "<4 x float>"/*origtype.getCompleteString()*/
		    << " " << temp.getName() << ", "
		    << vale.getCompleteString() << " " << temp2.getName()
		    << ", i32 " << idxr.getName() << "\n";

    
    
      infoSink.code <<"\tstore <4 x float> " << temp1.getName()
		    <<", <4 x float>* " << mat.getName()
		    << ".u." << idxc.getName() << ".4\n";
    }
  else if( mat.getIndex()[0].type.getQualifier() == EvqTemporary ){ 
    //infoSink.code << "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHere T?\n";
      //const TString &str = allocate_tempvar().c_str(); 
      TString NameOfMatrix;
      NameOfMatrix = mat.getName();
      NameOfMatrix += "."; 
      NameOfMatrix += idxc.getName();
      //Local matrix is composed of many vec4. So we need to change the type from mat type to vector n.
      //infoSink.code << mat.getNominalSize() << mat.getIndex()[0].type.getNominalSize() <<"\n";
      //TType Newtype( EbtFloat , EvqTemporary, mat.getIndex()[0].type.getNominalSize() , false , false , EbpHigh);
      TllvmVar temp( mat ); //Create new variable  
      temp.setName( NameOfMatrix );
      //temp.setType( Newtype );
      //infoSink.code <<"I think it's here.\n";
      LoadReserveVariableFirstTime( temp );
      

      const TString &str1 = allocate_tempvar().c_str(); //For second instruction
      TllvmVar temp1( mat );
      temp1.setName(str1);
      TllvmVar temp2( vale );
      //infoSink.code << vale.getName() << "\n";
      LoadReserveVariableFirstTime( temp2 );
      //infoSink.code << mat.isMatrix() << mat.getIndex()[0].index.getName() << "\n";
      //infoSink.code << vale.getName() << "\n";



      infoSink.code << "\t" << temp1.getName()
		    << " = insertelement " << "<4 x float>"/*origtype.getCompleteString()*/
		    << " " << temp.getName() << ", "
		    << vale.getCompleteString() << " " << temp2.getName()
		    << ", i32 " << idxr.getName() << "\n";

      NameMappingRegister( NameOfMatrix, str1 );


      //For store instruction.
      temp.setName( NameOfMatrix );
      //temp.setType( Newtype );
      //infoSink.code << Newtype.getCompleteString() << "\n";
      //infoSink.code << temp.getName() <<" "<<temp.getCompleteString() <<" "<< temp.getIndex()[0].type.getCompleteString() <<"\n";
      //infoSink.code << temp.haveIndex() <<"\n";
      store_vars.push_back( temp );     

      tmp_register_counter = str1;
      //infoSink.code << NameOfMatrix <<" "<<ret_vector[NameOfMatrix]<< "\n";
    }
#endif
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
    /*
    infoSink.code << "\t" << agg.getName()
		  << " = insertvalue " << il[0].type.getCompleteString()
	          << " " << agg.getName() << ", "
		  << val.getCompleteString() << " " << val.getName();

    for(i=0;i<size;++i)
	infoSink.code << ", " << il[i].index.getName();

    infoSink.code << "\n";
    */
    if( il[0].type.isArray() && il[0].type.isMatrix() && il.size() == 1 ){ //For assigning array of matrix type.

      int SizeofMatrix = val.getNominalSize();
      int SizeofParameters = (int)RecordParametersforStructure.size();
      //infoSink.code << SizeofParameters <<"\n";
      bool AssignFloatToMatrix = false;
      if( SizeofMatrix != SizeofParameters ){
	AssignFloatToMatrix = true;
	//SizeofParameters > SizeofMatrix ? SizeofMatrix = SizeofParameters : SizeofMatrix ;
      }
      const TString &str = allocate_tempvar().c_str();
      TType Newcurtype(EbtFloat,EvqTemporary,SizeofMatrix,false,false,EbpHigh); //Creat vec4 type.
      TllvmVar rt( val );
      rt.setName( str );
      
      infoSink.code << "\t" << rt.getName() << " = " 
		    << "getelementptr inbounds " << il[0].type.getCompleteString() << "* "
		    << agg.getName() << ", i32 0, i32 " << il[0].index.getName() <<"\n";

      const TString &str1 = allocate_tempvar().c_str();
      TllvmVar rt1( val );
      rt1.setName( str1 );

      for( int IndexofMatrix = SizeofMatrix-1 ; IndexofMatrix >= 0 ; IndexofMatrix-- ){
	
	infoSink.code << "\t" << rt1.getName() << " = " 
		      << "getelementptr inbounds " << agg.getCompleteString() << "* "
		      << rt.getName() << ", i32 0, i32 " << IndexofMatrix <<"\n";
	TllvmVar tempvar(rt1);
	if( AssignFloatToMatrix ){
	  
	  TType TmpType( Newcurtype );
	  string FnnameforConstruct;
	  TInfoSinkBase InttoAscii;
	  TllvmVarList VarList;
	  int indexforsize = 0;
	  InttoAscii <<  Newcurtype.getNominalSize();
	  //infoSink.code << Newcurtype.getNominalSize()<<"\n";
	  //infoSink.code << SizeofParameters<<"\n";
	  FnnameforConstruct = "OpenGLES.Construct";
	  if(TmpType.getBasicType() == EbtBool)
	    FnnameforConstruct += "B";
	  else if(TmpType.getBasicType() == EbtInt)
	    FnnameforConstruct += "I"; 
	  FnnameforConstruct += "Vec";
	  FnnameforConstruct += InttoAscii.c_str();
	  //out.code <<"curtype: " << curtype.getCompleteString()  <<" size : " << curtype.getNominalSize() << "\n";
	  
	  while( indexforsize < Newcurtype.getNominalSize() ) {
	    VarList.push_back( RecordParametersforStructure[0] );
	    RecordParametersforStructure.erase( RecordParametersforStructure.begin() ); //Remove the register which is already assgined.
	    indexforsize++;
	    //infoSink.code << v.getName() <<" "<<v.isEndConstructVector() <<"after pop1\n";
	  }
	  FnnameforConstruct = DepolymorphismFunctionName(FnnameforConstruct, VarList);
	  //infoSink.code << FnnameforConstruct <<"\n";
	  
	  TllvmVar tmpv("", TmpType, TllvmVarCode);      
	  GenFunc(tmpv, FnnameforConstruct, VarList);      
	  
	  tempvar.setType( Newcurtype );
	  GenStore( tempvar , pop_expstack_rhs()  ); 
	  if( IndexofMatrix  != 0 )
	    rt1.setName( allocate_tempvar().c_str() );
	  

	}
	else{
	  rt1.setType( Newcurtype );
	  GenStore( rt1 , RecordParametersforStructure[0] );
	  RecordParametersforStructure.erase( RecordParametersforStructure.begin() ); //Remove the register which is already assgined.
	  if( IndexofMatrix  != 0 )
	    rt1.setName( allocate_tempvar().c_str() );
	}
      }
    }
    else {  //e.g. a[1]=2.0;
      const TString &str = allocate_tempvar().c_str();
      TllvmVar dst( str , val.getType() ); //Dst's type should be the same as the val's type.
      
      
      infoSink.code << "\t" << dst.getName() << " = getelementptr inbounds" << il[0].type.getCompleteString() << "* " <<agg.getName() << " , i32 0 ";
      
      for(i=0;i<size;++i)
	infoSink.code << ", i32 " << il[i].index.getName();
      
      infoSink.code << "\n";
      
      TllvmVar temp_var( val.getName() , val.getType() );

      //DDDDDDDDDDDDDD
      LoadReserveVariableFirstTime( temp_var );


      //infoSink.code <<agg.getName() <<" "<<agg.getType().getCompleteString() <<" "<< val.getName()<<" "<<val.getType().getCompleteString()<<"\n";
#ifdef deal_store_instr
      const TString &str1 = allocate_tempvar().c_str();
      TllvmVar var1(str1,temp_var.getType());
      GenLoad(var1,dst);
      const TString &str2 = allocate_tempvar().c_str();
      TllvmVar var2(str2,temp_var.getType());
      const TString &str3 = allocate_tempvar().c_str();
      TllvmVar var3(str3,temp_var.getType());
      if(temp_var.getCompleteString() == "highp i1"||temp_var.getCompleteString() == "highp i32"){
	infoSink.code << "\t" << var2.getName()<< "= mul " << temp_var.getCompleteString() <<" "
		      << str1 <<", " << "0"<<"\n";
	infoSink.code << "\t"<<var3.getName() << "= add " << temp_var.getCompleteString() <<" "
		      << var2.getName()  <<", " << temp_var.getName()<<"\n";
      }
      else{
	infoSink.code << "\t" << var2.getName()<< "= fmul " << temp_var.getCompleteString() <<" "
		      << str1 <<", " << "0x0000000000000000"<<"\n";
	infoSink.code << "\t"<<var3.getName() << "= fadd " << temp_var.getCompleteString() <<" "
		      << var2.getName()  <<", " << temp_var.getName()<<"\n";
      }
      temp_var.setName(str3);
      GenStore( dst , temp_var );
#else
      GenStore( dst , temp_var );
#endif

      if( agg.getQualifier() == EvqVaryingOut ){

	const TString &str = allocate_tempvar().c_str();
	TllvmVar tk(str , dst.getType());
	  
	//NameMappingRegister( v2.getName() , tk.getName() );
	tk.setName(str);	

	GenLoad( tk , dst );

	const TString &str1 = allocate_tempvar().c_str();
	TllvmVar tk2(str1 , dst.getType());
	
	GenVaryingOut(agg,tk2,tk);
	
      }
    }

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
  //infoSink.code << agg.getType().getCompleteString() <<"\n";
  //infoSink.code << val.getType().getCompleteString() <<"\n";

      if(agg.getType() != val.getType()) {
	infoSink.info.message(EPrefixInternalError,
			 "Assigning to different type",
			 line);
	return false;
	}// Kaymon 
  
    int size = agg.getIndex().size();
    int fng = agg.getFirstNonAggPos();
    //infoSink.code <<"InsertAgg=> size: "<< size <<" fng: "<< fng <<"\n";
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
	else{
	  return GenInsertNonAgg(agg, val, idx, line);
	}
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
    //infoSink.code << "IsSwizzle: "<<isswizzle<<"\n";
    if(isswizzle)
    {
	int i;
	int size = type.getNominalSize();
	vector<int> swi(size);
	//infoSink.code << "Sizeof(type): "<<size<<"\n";

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
    //infoSink.code << "basevar.isCode() :" << basevar.isCode() <<"\n";
    //infoSink.code << "Type :" << type.getCompleteString() <<"\n";
    //infoSink.code << "basevar: "<<basevar.getName()<<"\n";
    //infoSink.code << "basevar type: "<<basevar.getCompleteString()<<"\n";
    
    if(basevar.isCode())
	basevar = GenGetRHS(basevar, line);

    if(basevar.getName() == "") {
	infoSink.info.message(EPrefixInternalError,
			 "Popping empty expression stack",
			 line);
	return false;
    }
    //infoSink.code << "basevar.isCode() :" << basevar.isCode() <<"\n";
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
    //TllvmVar v3 = pop_expstack();
    TllvmVar rhs;
    size_t index;
    //infoSink.code << "line: " <<line <<"\n";
    //infoSink.code << (int)store_vars_vector.size() <<"\n";
    //infoSink.code<<"v1 name = "<<v1.getName() << "\nv1 type = "<< v1.getCompleteString() <<"\n"; //debug message
    //infoSink.code<<"v2 name = "<<v2.getName() << "\nv2 type = "<< v2.getCompleteString() <<"\n"; //destination's vector.
    //infoSink.code<<"v3 name = "<<v3.getName()<<"\n"; //destination's vector.
    //infoSink.code<<"v1 type: "<<v1.getType().getCompleteString()<<" Q: "<<v1.getQualifier()<<" V: "<<v1.isVector()<<" C: "<<v1.isCode()<<" N: "<<v1.isNormalType()<<" haveIndex: "<<v1.haveIndex() <<" isMatrix: "<< v1.isMatrix()<<" isStructure: "<<v1.isStructure()<<" isArray: "<< v1.isArray()<<"\n";
    //infoSink.code<<"v2 type: "<<v2.getType().getCompleteString()<<" Q: "<<v2.getQualifier()<<" V: "<<v2.isVector()<<" C: "<<v2.isCode()<<" N: "<<v2.isNormalType()<<" haveIndex: "<<v2.haveIndex() <<" isMatrix: "<< v2.isMatrix()<<" isStructure: "<<v2.isStructure()<<" isArray: "<< v2.isArray()<<"\n";
    /*
    if( v1.haveIndex() ){
      infoSink.code <<"Size of v1.getIndex(): "<<(int )v1.getIndex().size() << "  isMatrix() in v1.getIndex()[0].index: " << v1.getIndex()[0].type.isMatrix() <<" Array: " <<v1.getIndex()[0].type.isArray() <<" v1.getIndex()[0].type: "<< v1.getIndex()[0].type.getCompleteString() << "\n";
    }
    if( v2.haveIndex() ){
      infoSink.code <<"Size of v2.getIndex(): "<<(int )v2.getIndex().size() << "  isMatrix() in v2.getIndex()[0].index: " << v2.getIndex()[0].type.isMatrix() <<" Array: " <<v2.getIndex()[0].type.isArray() <<" v2.getIndex()[0].type: "<< v2.getIndex()[0].type.getCompleteString() << "\n";
      }*/
    
    //infoSink.code <<" v1.haveIndex():  " <<v1.haveIndex() << " v2.haveIndex():" << v2.haveIndex() <<"\n"; 
    //Kaymon

    if( v2.getQualifier() == EvqGlobal && IsGlobalOutsideFunction == 1){ //Might change parser better than below thing. For initailize global vec4.

      index = infoSink.code.sink.find(v2.getName().c_str());
      infoSink.code.sink.erase(index,100); //just delete last declaration. It is the same as variable I will create. It might remove the debug message of v1.getName().
      TllvmVarList temp = v1.getAsStruct();
      int ii=0;
      int Ssize= temp.size();
      infoSink.code <<v2.getName() <<" = global " << v1.getCompleteString()<<"<";
      
      while(ii < 4){
	if(ii<Ssize)
	  infoSink.code <<"float "<<temp[ii].getName()<<", ";
	else if(ii<3)
	  infoSink.code <<"float 0x0000000000000000,";
	else
	  infoSink.code <<"float 0x0000000000000000";
	ii++;
      }
      infoSink.code << ">\n";
      return true;
    }
    //cout<< "print:\n\n" << infoSink.code.c_str();
    //cout.flush();

    it = ret_name.find( v2.getName() );
    if( it == ret_name.end() ){
      stringstream insertstring;
      //infoSink.code << Recordfnname<<"() {\n";
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
      size_t sssize = strlen(infoSink.code.c_str());
	
      //infoSink.code<<"index: "<<(int)index<<infoSink.code.sink[index]<<"\n";
      //infoSink.code <<Recordfnname<<"\n";
      //infoSink.code<<"v2 name = "<<v2.getName()<<" v2.qualifier(): "<<v2.getQualifier()<<"\n"; //destination's vector.
      if( v2.haveIndex() && strncmp( v2.getName().c_str() , "@" , 1 ) && v2.getIndex()[0].type.isMatrix() ) {//Allocate the variable which have indexes and the name start character of "%". Check variable "v2" whether it is matrix or not.
	//insertstring <<"\t"<<v2.getName()<<" = alloca "<<v2.getIndex()[0].type.getCompleteString()<<"\n";
#ifdef NewMatrix
	insertstring <<"\t"<<v2.getName()<<" = alloca "<<v2.getIndex()[0].type.getCompleteString()<<"\n";
#else
	//infoSink.code<<"allocate v2 name = "<<v2.getName()<<"\n"; //destination's vector.
	int SizeofMatrix = v2.getIndex()[0].type.getCompleteString()[7] - 48 ;
	while( --SizeofMatrix >= 0 ){
	  insertstring <<"\t"<<v2.getName()<< "." << SizeofMatrix << " = alloca <"<< v2.getIndex()[0].type.getCompleteString()[7] <<" x float>"<<"\n";
	  TString MatrixName = v2.getName();
	  MatrixName += '.';
	  MatrixName += SizeofMatrix+48;
	  //infoSink.code << MatrixName <<"\n";
	  ret_name[ MatrixName ] = "Yes";
	}
#endif
      }
      else if( v2.haveIndex() && strncmp( v2.getName().c_str() , "@" , 1 ) )//Allocate the variable which have indexes and the name start character of "%". 
	insertstring <<"\t"<<v2.getName()<<" = alloca "<<v2.getIndex()[0].type.getCompleteString()<<"\n";
      else if( !strncmp( v2.getName().c_str() , "%" , 1 ) ) //Distinguish the first character of variable names between "@" and "%". Allocate the variable which name start character of "%".
	insertstring <<"\t" <<v2.getName() << " = alloca " << type.getCompleteString() <<"\n"; 
	
      if(sssize < index ){ //error message!!!!!!!!!!!!
	infoSink.code << "String insertion error!!!!!!!!!!!!\n";
	return false;
      }
      infoSink.code.sink.insert( index+2 , insertstring.str() );
      ret_name[ v2.getName() ] = "Yes";
      //store_vars.push( v2 );
      //infoSink.code << v2.getName() << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++-vars\n";
      //if( strncmp( v2.getName().c_str() , "0" , 1 ) && !( v2.haveIndex() && v2.getIndex()[0].type.isMatrix() )&& !(  v2.haveIndex() && v2.getIndex()[0].type.isArray() ) && (!v2.isNormalType()||v2.isVector()||v2.haveIndex() ) ){ // Filter out the string of IEEE754,array , normal type  and matrix type for v2. note: normaltype => int a = 1; a is normaltype.
      if( strncmp( v2.getName().c_str() , "0" , 1 ) && !( v2.haveIndex() && v2.getIndex()[0].type.isMatrix() )&& !(  v2.haveIndex() && v2.getIndex()[0].type.isArray() ) && (!v2.isSimpleVar()||v2.haveIndex()||v1.isCode()||v1.haveIndex() ) && (!v2.isStructure()) ){ // Filter out the string of IEEE754,array , normal type  and matrix type for v2. note: normaltype => int a = 1; a is normaltype.
#ifdef NewMatrix
	if( !v2.isMatrix())
#endif
	  store_vars.push_back( v2 );   
	//infoSink.code << "Come in: "<<v2.getName() <<"\n";
      }
      //size_t size = store_vars.size();
      //infoSink.code <<"store_ vars: "<< (int) size <<"\n";


    }
    else {
      
      //store_vars.push( v2 );
      //infoSink.code << v2.getName() << " haveIndex(): " << v2.haveIndex() <<"\n";
      if( strncmp( v2.getName().c_str() , "0" , 1 ) && !( v2.haveIndex() && v2.getIndex()[0].type.isMatrix() ) && !( v2.haveIndex() &&  v2.getIndex()[0].type.isArray() ) && (!v2.isSimpleVar()||v2.haveIndex()||v1.isCode()||v1.haveIndex() ) && ( !v2.isStructure() ) ) {//As above
	//infoSink.code << "Come else: "<<v2.getName() << "\n";
	if( ( v2.getQualifier() == EvqTemporary ) && v2.isMatrix() ){  //e.g. NormalMatrix = NormalMatrix / detM.

	  TType Newtype( EbtFloat , EvqTemporary, v2.getNominalSize() , false , false , EbpHigh);
#ifdef NewMatrix
#else
	  v2.setType( Newtype );
#endif
	  TString NameOfMatrix = v2.getName();
	  NameOfMatrix += '.';
	  NameOfMatrix += v2.getNominalSize()-1+48;
#ifdef NewMatrix
#else
	  v2.setName( NameOfMatrix );
#endif
	}
#ifdef NewMatrix
	if( !v2.isMatrix())
#endif
	store_vars.push_back( v2 );
      }
      //size_t size = store_vars.size();
      //infoSink.code <<"store_ vars: "<< (int) size <<"\n";  
    }
    //=============== old ==============
    /*
    if( it == ret_name.end() && !strncmp( v2.getName().c_str() , "%" , 1 ) || it == ret_name.end() && !strncmp( v2.getName().c_str() , "@gl" , 3 ) ){
      stringstream insertstring;
      index= infoSink.code.sink.find("{");
      if( v2.haveIndex())
	insertstring <<"\t"<<v2.getName()<<" = alloca "<<v2.getIndex()[0].type.getCompleteString()<<"\n";
      else
	insertstring <<"\t" <<v2.getName() << " = alloca " << type.getCompleteString() <<"\n"; 
      infoSink.code.sink.insert( index+2 , insertstring.str() );
      ret_name[ v2.getName() ] = "Yes";
      //store_vars.push( v2 );
      store_vars.push_back( v2 );
      //size_t size = store_vars.size();
      //infoSink.code <<"store_ vars: "<< (int) size <<"\n";

    }
    else if( it != ret_name.end() && !strncmp( v2.getName().c_str() , "%" , 1 ) || it != ret_name.end() && !strncmp( v2.getName().c_str() , "@gl" , 3 ) ){
      
      //store_vars.push( v2 );
      store_vars.push_back( v2 );
      //size_t size = store_vars.size();
      //infoSink.code <<"store_ vars: "<< (int) size <<"\n";  
      }
    */
    //Done


    if(v1.getName() == "" || v2.getName() == "") {
	infoSink.info.message(EPrefixInternalError,
			 "Popping empty expression stack",
			 line);

	return false;
    }

    if(v1.isCode() && v2.isNormalType() && !v2.haveIndex()) {
      // Disable this condition because do it previous.
      //if( v2.isMatrix() && ( v2.getQualifier() == EvqTemporary ) ){ // Bypass this condition.For example : NormalMatrix = NormalMatrix / detM. The dst's type is defferent from the src's type. Kaymon 
	//TType Newtype( EbtFloat , EvqTemporary, v2.getNominalSize() , false , false , EbpHigh);
	//v2.setType( Newtype );
      //}

       if(v1.getType() != v2.getType()) {
	 if (v2.isMatrix() && SizeofMatrixCompMult){
	   //skip the checking condition.
	 }
	 else{
	    infoSink.info.message(EPrefixInternalError,
			     "Assigning to different type",
			     line);
	    infoSink.info << v1.getCompleteString() << " "
			  << v1.getName() << ", "
			  << v2.getCompleteString() << " "
	                  //<< v1.getName() << "\n";
	                  << v2.getName() << "\n";
	    return false;
	 }
       }
	
	//dondum now -- Last assign to gl_position.
	//So far, gl_position is result of all the benchmark. When OutputTraverser generate assign to gl_Position, it finish this vertex shader.
	//We can search 'function finish' when other case occur.Maybe it is a way to do something.
	if(v2.getName() == "@gl_Position")
	  GenENDVertex(v1,v2);
	else if(v2.getName() == "@gl_FragColor")
	  GenENDFragment(v1,v2);
	else{
	  //Kaymon
	  const TString &str = allocate_tempvar().c_str();
	  TString tmp_string;
	  tmp_string = v2.getName();
	  //infoSink.code << v2.getCompleteString() <<" "<<v2.getName() <<"+++++++"<<str<<"++++++++++\n";
	  //infoSink.code << v2.isStructure() <<"\n";
	  if(v2.isStructure()){
	    TllvmVar tempVar(v2);
	    tempVar.setName(str);
	    infoSink.code << "\t" << str << " = " << v1.getName();
	    GenStore(v2,tempVar);
	  }
#ifdef NewMatrix

	  else if (v2.isMatrix() && SizeofMatrixCompMult){

	    TType Newcurtype(EbtFloat,EvqTemporary,SizeofMatrixCompMult,false,false,EbpHigh); //Creat vec4 type.
	    TllvmVar rt( str , Newcurtype );
	    infoSink.code << "\t" << rt.getName()  << " = " << v1.getName();
	    Recordparameters.push_back( rt );
	    const TString &str1 = allocate_tempvar().c_str();
	    rt.setName(str1);
	    for( int j = 0 ; j < SizeofMatrixCompMult ; j++ ){

	      infoSink.code << "\t" << rt.getName() << " = " 
			    << "getelementptr inbounds " << v2.getCompleteString() << "* "
			    << v2.getName() << ", i32 0, i32 " << j <<"\n";
	      	      
	      GenStore( rt , Recordparameters[j] );
	      if( j != SizeofMatrixCompMult-1 ){
		const TString &str2 = allocate_tempvar().c_str();
		rt.setName(str2);	      
	      }
	    }

	  }
	  else if( v2.isMatrix() ){ //Assign matrix.

	    int SizeofMatrix = v2.getNominalSize();
	    int SizeofParameters = (int)RecordParametersforStructure.size();
	    //infoSink.code << SizeofParameters <<"\n";
	    bool AssignFloatToMatrix = false;
	    if( SizeofMatrix != SizeofParameters ){
	      AssignFloatToMatrix = true;
	      //SizeofParameters > SizeofMatrix ? SizeofMatrix = SizeofParameters : SizeofMatrix ;
	    }
	    TType Newcurtype(EbtFloat,EvqTemporary,SizeofMatrix,false,false,EbpHigh); //Creat vec4 type.
	    TllvmVar rt( v2 );
	    rt.setName( str );
	     
	    for( int IndexofMatrix = SizeofMatrix-1 ; IndexofMatrix >= 0 ; IndexofMatrix-- ){
	     
	      infoSink.code << "\t" << rt.getName() << " = " 
			    << "getelementptr inbounds " << v2.getCompleteString() << "* "
			    << v2.getName() << ", i32 0, i32 " << IndexofMatrix <<"\n";
	      TllvmVar tempvar(rt);
	      if( AssignFloatToMatrix ){
		
		  TType TmpType( Newcurtype );
		  string FnnameforConstruct;
		  TInfoSinkBase InttoAscii;
		  TllvmVarList VarList;
		  int indexforsize = 0;
		  InttoAscii <<  Newcurtype.getNominalSize();
		  //infoSink.code << Newcurtype.getNominalSize()<<"\n";
		  //infoSink.code << SizeofParameters<<"\n";
		  FnnameforConstruct = "OpenGLES.Construct";
		  if(TmpType.getBasicType() == EbtBool)
		    FnnameforConstruct += "B";
		  else if(TmpType.getBasicType() == EbtInt)
		    FnnameforConstruct += "I"; 
		  FnnameforConstruct += "Vec";
		  FnnameforConstruct += InttoAscii.c_str();
		  //out.code <<"curtype: " << curtype.getCompleteString()  <<" size : " << curtype.getNominalSize() << "\n";
		  
		  while( indexforsize < Newcurtype.getNominalSize() ) {
		    VarList.push_back( RecordParametersforStructure[0] );
		    RecordParametersforStructure.erase( RecordParametersforStructure.begin() ); //Remove the register which is already assgined.
		    indexforsize++;
		    //infoSink.code << v.getName() <<" "<<v.isEndConstructVector() <<"after pop1\n";
		  }
		  FnnameforConstruct = DepolymorphismFunctionName(FnnameforConstruct, VarList);
		  //infoSink.code << FnnameforConstruct <<"\n";
		  
		  TllvmVar tmpv("", TmpType, TllvmVarCode);      
		  GenFunc(tmpv, FnnameforConstruct, VarList);      
		  
		  tempvar.setType( Newcurtype );
		  GenStore( tempvar , pop_expstack_rhs()  ); 
		  if( IndexofMatrix  != 0 )
		    rt.setName( allocate_tempvar().c_str() );
		  

	      }
	      else{
		rt.setType( Newcurtype );
		GenStore( rt , RecordParametersforStructure[0] );
		RecordParametersforStructure.erase( RecordParametersforStructure.begin() ); //Remove the register which is already assgined.
		if( IndexofMatrix  != 0 )
		  rt.setName( allocate_tempvar().c_str() );
	      }
	    }


	  }
#endif
	  else{
	    NameMappingRegister( v2.getName() , str );
	    //infoSink.code << ret_vector[v2.getName()]<<"+++++++++++++++++\n";
	    //std::cout<< "[YY]:" << v2.getName()<<"\n";
	    v2.setName(str);
	    //store_vars_vector.push( v2 );
	    //if( !strncmp( tmp_string.c_str() , "%" , 1 ) ) //Kaymon save the the variable which is beginning "@".
	    store_vars_vector.push_back( v2 );
	    //size_t size = store_vars_vector.size();
	    //infoSink.code <<"store_vars_vector: "<< (int) size <<"\n";
	    
	    //Done
	    infoSink.code << "\t" << v2.getName() << " = " << v1.getName();
	  }
	}

	return true;
    }
    else if(v1.haveIndex() && v2.isNormalType() && !v2.haveIndex()) {
      //return GenExtractAgg(v1, v2, line); Original code 
      TString tmp_string = v2.getName(); // Save the v2.getName(). Get the name before running GenExtracttAGG.
#ifndef NewMatrix
      if( v1.getIndex()[0].type.isMatrix() ){
	TString NameOfMatrix = v1.getName();
	NameOfMatrix += '.';
	NameOfMatrix += v1.getIndex()[0].index.getName();

	v1.setName( NameOfMatrix );
      }
      /*v1.setType( v1.getIndex()[0].type );
      if( ( v1.getQualifier() == EvqTemporary ) && v1.isMatrix() ){
	TType Newtype( EbtFloat , EvqTemporary, v1.getNominalSize() , false , false , EbpHigh);
	v1.setType( Newtype );
      }      
      */
      LoadReserveVariableFirstTime( v1 ); //Kaymon
#else
      if(v1.getIndex()[0].type.isMatrix())     //if variable is array or matrix, skip it. e.g. vector T = k[1]; 
	;
      else if(v1.getIndex()[0].type.isArray())//if variable is array or matrix, skip it. e.g. vector T = k[1]; 
	;
      else
	LoadReserveVariableFirstTime( v1 ); //Kaymon
#endif
      
      TllvmVar rt( allocate_tempvar().c_str() , TType(v2.getType()) ); //This vector will be assigned. Dst vector.

      if( GenExtractAgg(v1, rt, line) ){

	//infoSink.code <<" rt.getName(): " <<rt.getName() <<" tmp_register: " << tmp_register_counter <<"\n";
	if( strcmp( tmp_register_counter.c_str() , "unused" ) ){
	  //infoSink.code << v2.getName() <<" " <<rt.getName() <<"\n";
	  NameMappingRegister( tmp_string , tmp_register_counter );
	  //NameMappingRegister( v2.getName() , rt.getName() );
	  v2.setName( tmp_register_counter );
	}
	else{
	  //infoSink.code << v2.getName() <<" " <<rt.getName() <<"\n";
	  NameMappingRegister( tmp_string , rt.getName() );
	  v2.setName( rt.getName() );
	}
	store_vars_vector.push_back( v2 );
	//infoSink.code << ret_vector[ tmp_string ]<<"\n";
	return true;
      }
      else 
	return false;
    }
    else {

      
      if(v2.isConst()) {
	//infoSink.code << v1.getName() <<"HIIIIIIIIIIIII\n";
	//infoSink.code << v2.getName() <<"HIIIIIIIIIIIII\n";
	    push_expstack(v2);
	    push_expstack(v1);
	    if(type.isVector()) { //Generate the shufflevector's content. ex: vec4 example( 1.0 , 1.0 , 1.0 , 1.0  );
		rhs = ConstructConstVec(type, line);
		v2 = pop_expstack_rhs();
		//infoSink.code << v2.getName() <<" "<<rhs.getName()<<"HIIIIIIIIIIIII\n";
		//************************Declare the local variable for this special case. ex: %VARIABLE_NAME = allca <TYPE>*****************************
		it = ret_name.find( v2.getName() );
		if( it == ret_name.end() ){
		  stringstream insertstring;
		  index= infoSink.code.sink.find("{");
		  
		  if( v2.haveIndex() && strncmp( v2.getName().c_str() , "@" , 1 ) )//Allocate the variable which have indexes and the name start character of "%".  
		    insertstring <<"\t"<<v2.getName()<<" = alloca "<<v2.getIndex()[0].type.getCompleteString()<<"\n";
		  else if( !strncmp( v2.getName().c_str() , "%" , 1 ) ) //Distinguish the first character of variable names between "@" and "%". Allocate the variable which name start character of "%".
		    insertstring <<"\t" <<v2.getName() << " = alloca " << type.getCompleteString() <<"\n"; 
	
      

		  infoSink.code.sink.insert( index+2 , insertstring.str() );
		  ret_name[ v2.getName() ] = "Yes";
		  //store_vars.push( v2 );
		  //infoSink.code << v2.getName() << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++-vars\n";
		  if( strncmp( v2.getName().c_str() , "0" , 1 ) ) // Filter out the string of IEEE754  
		    store_vars.push_back( v2 );      
		  //size_t size = store_vars.size();
		  //infoSink.code <<"store_ vars: "<< (int) size <<"\n";

		}
		else {
      
		  //store_vars.push( v2 );
		  //infoSink.code << v2.getName() << "----------------------------------------------------------vars\n";
		  if( strncmp( v2.getName().c_str() , "0" , 1 ) ) // Filter out the string of IEEE754  
		    store_vars.push_back( v2 );
		  //size_t size = store_vars.size();
		  //infoSink.code <<"store_ vars: "<< (int) size <<"\n";  
		}
		//store_vars.push_back( v2 ); //Old //Kaymon 
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
      else{
#ifdef NewMatrix
	if( v1.getName() == "ConstructorMat" )
	  ;
	else
#endif
	rhs = GenGetRHS(v1, line);//get the rhs variable name 
      }
      if(v2.haveIndex()) { //For example: (vector) temp_vertex.x = (float) a !!.
	
	//infoSink.code <<"IIIIIIIIIIIIIIIn v2.haveIndex() \n";
	//dondum 20110727 try; ????????????????
	TString tmp_string; // Save the v2.getName(). Get the name before running GenInsertAGG.
	
	//const TString &str = allocate_tempvar().c_str();
	//infoSink.code<<"ret_name = "<<ret_vector[v2.getName()]<<"\n";
	//infoSink.code<<"rhs name = "<<rhs.getName()<<"\n";
	//NameMappingRegister( v2.getName() , str );
	//std::cout<< "[YO]:" << v2.getName()<<"\n";
	 //v2.setName(str);
	 //store_vars_vector.push_back( v2 ); 
	 //End the edit.
	//infoSink.code<<"type_name = "<<v2.getType().getCompleteString()<<"\n";
	 tmp_string = v2.getName();
	 //v2.setType( v2.getIndex()[0].type ); //mark here. Because  fixed the index.
	 //infoSink.code<<"v2 index: "<<v2.getIndex()[0].type.getCompleteString()<<" "<<v2.getIndex()[0].type.getQualifier()<<"\n";
	 
	 //TType tmp_type = v2.getIndex()[0].type; //old
	 //TType tmp_type1( v2.getBasicType() , v2.getQualifier() , tmp_type.getNominalSize() , 0 , 0  , EbpHigh);
	 //infoSink.code <<"Test :" <<tmp_type1.getCompleteString()<<"\n";
	 //v2.setType( tmp_type1 );
	 //infoSink.code << v2.getName() <<" "<<v2.getType().getCompleteString()<<" " <<v2.getIndex()[0].index.getName() <<" "<< v2.getIndex()[0].type.getCompleteString() <<"\n";
	 //v2.setType( v2.getIndex()[0].type ); // new
	 //infoSink.code << v2.getCompleteString() <<"\n";
	 //infoSink.code << v1.getCompleteString() <<"\n";
	 //infoSink.code << rhs.getName()  <<"\n";
	 //infoSink.code << rhs.getCompleteString() <<"\n";
	 //infoSink.code << v1.isConst() <<"\n";
	 //infoSink.code << v2.getIndex()[0].type.getCompleteString() <<"\n";
	 //infoSink.code << v2.getIndex()[0].type.isArray() <<"\n";
	 //infoSink.code << v2.getIndex()[0].type.isMatrix() <<"\n";
	 //infoSink.code << (int )v2.getIndex().size() <<"\n";
	 
	 if( v2.getIndex()[0].type.isArray() && v2.getIndex()[0].type.isMatrix() ){
	   rhs = v1;
	 }
	 if( v2.getIndex()[0].type.isArray() )
	   ;//infoSink.code <<"aaaaaaFFFFFFFFFFFFFFFF\n";
	 else if( v2.getIndex()[0].type.isMatrix() )
	   ;//infoSink.code <<"mmmmmmmmmFFFFFFFFFFFFFFFF\n";
	 else if( DoesNameExist( v2.getName() ))
	   v2.setName( ret_vector[v2.getName()] );
	 else{
	   //infoSink.code <<"elseeeeeeeeeeeeeeeeeeeeeeeeeeeeee\n";
	   bool IsNum = true; // Make sure of the string which is number or string.
	   const char *temp;
	   temp = v2.getName().c_str();
	   temp++;
	   while( *temp != '\0' ){
	     if( *temp < '0' || *temp > '9' )
	       IsNum = false;
	     temp++;
	   }// Done 
	   //infoSink.code << IsNum << "\n"; 
	   if( !IsNum ){
	     const TString &str = allocate_tempvar().c_str();
	     TllvmVar ts(v2);
	     ts.setName(str);
	     GenLoadVariablefromIndex(ts,v2);
	     
	     v2.setName(str);
	   }  
	 }
	 //infoSink.code << "++++++++++++++++++++"<<v2.getName() <<" " << rhs.getName() <<"\n";
	 rt = GenInsertAgg(v2, rhs, line);
	 //infoSink.code << "++++++++++++++++++++"<<v2.getName() <<" " << rhs.getName() <<"\n";
	 v2.setName( tmp_register_counter );
	 //infoSink.code << tmp_register_counter <<"\n";
	 NameMappingRegister( tmp_string , tmp_register_counter );
	 //infoSink.code <<v2.getName()<<"+++\n";
	 if( !v2.getIndex()[0].type.isArray() ) //do not save the variable for array.
	   store_vars_vector.push_back( v2 );
	 //#endif	 
	 //infoSink.code << tmp_string<<"\n";
	 //infoSink.code<<" after ret_name = "<<ret_vector[tmp_string]<<"\n";
	}
      else if(v2.isVector()) {
	//dondum
	//infoSink.code<<"v2  name = "<<v2.getName()<<"\n";
	//infoSink.code<<"rhs name = "<<rhs.getName()<<"\n";
	GenVectorAssign(v2, rhs);
      }
      else if(v2.isMatrix()) {

	rt = GenMatrixAssign(v2, rhs, line);
      }
      else if(v2.isStructure()) {
	//infoSink.code <<"Structure\n";
	rt = GenStructAssign(v2, rhs, line);
      }
      else {
	//Initialize the variable but we can just store value in the variable.
	//infoSink.code << "hihihihihihihihihih\n";	
	//infoSink.code << "v2 : "<<v2.getName()<<"\n";
	//infoSink.code << "rhs : "<<rhs.getName()<<"\n";
	if( !strncmp( rhs.getName().c_str() , "%" , 1 ) ||  !strncmp( rhs.getName().c_str() , "@" , 1 ) ) //Filter out the string of IEEE 754.) //Filter out the string of IEEE 754.
	  LoadReserveVariableFirstTime( rhs );
	NameMappingRegister( v2.getName() , rhs.getName() );
	//infoSink.code << "rhs : "<<rhs.getName()<<"\n";
	if( !v2.isNormalType() || v2.isVector() ) // Don't save the var for a statement which is that int a = 1; because it will call store instruction immediately.
	  store_vars_vector.push_back( rhs );


	//GenBinOp("or", v2, rhs, TllvmVar(0));

	if (v2.getQualifier() == EvqVaryingOut ){

	  const TString &str = allocate_tempvar().c_str();
	  TllvmVar tk(str , v2.getType());
	  
	  NameMappingRegister( v2.getName() , tk.getName() );
	  tk.setName(str);	
	  GenVaryingOut(v2,tk,rhs);
	}
	else{
#ifdef deal_store_instr
	  
	  if(rhs.getName()[0] != '0' || !strcmp(rhs.getName().c_str(),"0x0000000000000000") || rhs.getCompleteString() == "highp i32" ){ //temporary method. bypass the storing IEEE754 to variable. e.g. store float 0x3EE0000000000000, %c.  
	    //infoSink.code << rhs.getName() <<"\n";
	    //infoSink.code << v2.getName() <<"\n";
	    //infoSink.code << AfterGenCompareInstruction <<"\n";
	    //infoSink.code << (rhs.getType().getBasicType()==EbtBool) <<"\n";
	    if(rhs.getType().getBasicType()==EbtBool&&AfterGenCompareInstruction==1){ //Because we change i1 to i32, we have to do one more operation to convert the type from i1 to i32.
	                                                                              //e.g. %1 = icmp eq highp i32 %0, 102
	                                                                              //e.g. store highp i32 %1, highp i32* %equalto ;%1 is i1 type.
	      const TString &str = allocate_tempvar().c_str();
	      infoSink.code <<"\t"<<str <<" = call i32"  <<" @OpenGLES.Convert.BtoI"
			    << "(" <<"i1 " << rhs.getName()
			    << ")\n";
	      rhs.setName(str);
	      AfterGenCompareInstruction=0;
	    }
	    else
	      AfterGenCompareInstruction=0;
	    if(v2.getQualifier() == EvqInOut || v2.getQualifier() == EvqOut) //e.g. store highp float 0x0000000000000000, highp float* %par. par's qualifier is inout or out. A star in its type.
	      GenStoreForInOut(v2,rhs);
	    else
	      GenStore(v2,rhs);
	  }
	  else{
	    //infoSink.code << rhs.getName() <<"\n";
	    //infoSink.code << v2.getName() <<"\n";
	    const TString &str = allocate_tempvar().c_str();
	    TllvmVar var1(str,v2.getType());
	    GenLoad(var1,v2);
	    const TString &str1 = allocate_tempvar().c_str();
	    TllvmVar var2(str1,v2.getType());
	    const TString &str2 = allocate_tempvar().c_str();
	    TllvmVar var3(str2,v2.getType());
	    if(v2.getCompleteString() == "highp i1"||v2.getCompleteString() == "highp i32"){
	      infoSink.code << "\t" << var2.getName()<< "= mul " << v2.getCompleteString() <<" "
			    << str <<", " << "0"<<"\n";
	      infoSink.code << "\t"<<var3.getName() << "= add " << v2.getCompleteString() <<" "
			    << var2.getName()  <<", " << rhs.getName()<<"\n";
	    }
	    else{
	      infoSink.code << "\t" << var2.getName()<< "= fmul " << v2.getCompleteString() <<" "
			    << str <<", " << "0x0000000000000000"<<"\n";
	      infoSink.code << "\t"<<var3.getName() << "= fadd " << v2.getCompleteString() <<" "
			    << var2.getName()  <<", " << rhs.getName()<<"\n";
	    }
	    rhs.setName(str2);
	    GenStore(v2,rhs);
	  }
#else
	  GenStore(v2,rhs);
#endif
	}
	//infoSink.code << ret_vector[ v2.getName() ] <<"\n";
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
    //infoSink.code << "GencalAssign\n";
    opstr = GetBinOp(op, type.getBasicType());
    //infoSink.code << opstr << "\n";
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
        
      if(v2.haveIndex()){//e.g. c[0] *= 0.1 ;
	//infoSink.code << v2.getName() <<" "<< v2.getIndex()[0].index.getName() <<" "<< v2.getIndex()[0].type.getCompleteString()<<"\n";

	TllvmVar src(v2);
	TllvmVar dst(v2);
	//infoSink.code << v1.getName() <<"\n";
	//infoSink.code << src.getName() <<"\n";
	store_vars.push_back(v2); // save the dst's name.
	//const TString &str = allocate_tempvar().c_str();
	//dst.setName( str );	
	//GenLoadVariablefromIndex( dst,src );
	//NameMappingRegister( v2.getName() , dst.getName() );
	LoadReserveVariableFirstTime(src);
	
	//src.setName(str);
	const TString &str1 = allocate_tempvar().c_str();
	dst.setName( str1 );	
	GenExtractAgg(src,dst,line);
	
	src.setName(str1);
	const TString &str2 = allocate_tempvar().c_str();
	dst.setName( str2 );	
	GenBinOp(opstr, dst , src, v1);

	src.setName(ret_vector[v2.getName()]);
	GenInsertAgg(src,dst,line);
	NameMappingRegister( v2.getName() , tmp_register_counter );
	dst.setName(tmp_register_counter);
	store_vars_vector.push_back(dst); // save the dst's register number.
	
      }
      else{  //e.g. c*=0.1;
	TString recordv2name = v2.getName();
	store_vars.push_back(v2); // save the dst's name.
	LoadReserveVariableFirstTime(v2);
	LoadReserveVariableFirstTime(v1);
	TllvmVar target(v2);
	if(opstr=="sdiv"){ //Our spec do not deal integer devide. So convert type first.
	  opstr = "fdiv";

	  TInfoSinkBase code;
	  TInfoSinkBase code1;
	  TInfoSinkBase code3;
	  code << "call float @OpenGLES.Convert."
			<<  "sitofp(" << v2.getCompleteString() <<" " << v2.getName()
			<< ")\n";
	  
	  TllvmVar tv1(code.c_str(), type, TllvmVarCode);

	  code1 << "call float @OpenGLES.Convert."
			<<  "sitofp(" << v1.getCompleteString() <<" " << v1.getName()
			<< ")\n";

	  TllvmVar tv2(code1.c_str(), type, TllvmVarCode);

	  push_expstack(tv1);
	  push_expstack(tv2);
	  v1 = pop_expstack_rhs();
	  v2 = pop_expstack_rhs();
	  TType NewType( EbtFloat , EvqTemporary, 1 , false , false , EbpHigh);
	  v2.setType(NewType);
	  
	  const TString &str = allocate_tempvar().c_str();
	  target.setName( str );
	  GenBinOp(opstr, target , v2, v1);

	  code3 << "call i32 @OpenGLES.Convert."
			<<  "fptosi(" << v2.getCompleteString() <<" " << target.getName()
			<< ")\n";
	  TllvmVar tv3(code3.c_str(), type, TllvmVarCode);

	  push_expstack(tv3);
	  target = pop_expstack_rhs();
	  //xinfoSink.code << recordv2name<<" "<<target.getName()<<"\n";

	  store_vars_vector.push_back(target); // save the dst's register number.
	  NameMappingRegister( recordv2name , target.getName() );


	}
	else{
	  const TString &str = allocate_tempvar().c_str();
	  target.setName( str );
	  store_vars_vector.push_back(target); // save the dst's register number.
	  NameMappingRegister( recordv2name , str );
	  GenBinOp(opstr, target , v2, v1);
	}

      }
	//GenBinOp(opstr, v2, v2, v1); original
    }
    else {//for vector
	TllvmVar lhsv = GenGetRHS(v2, line);
	//infoSink.code << lhsv.getName()<<"\n";
	//infoSink.code << v1.getName()<<"\n";
	//Kaymon
	LoadReserveVariableFirstTime(lhsv);
	LoadReserveVariableFirstTime(v1);
	HandleArithmetic( lhsv , v1 , opstr );  //Handle the vec4 type for arithmetic. Now only deal the fdiv instruction
	//Doneg
	TllvmVar tgt = TllvmVar(allocate_tempvar().c_str(), type);

	if(opstr == "fdiv"){//Now just this instruction failed. e.g: gl_FragCorlor/=2.0 =>  fdiv <4 x float> dividend , <float 0x4000000000000000, float 0x4000000000000000, float 0x4000000000000000, float 0x4000000000000000>;
	   TllvmVar temp = pop_expstack();
	   infoSink.code <<"\t"<<tgt.getName() << " = "<< temp.getName();
	}
	else
	  GenBinOp(opstr, tgt, lhsv, v1);

	push_expstack(v2);
	push_expstack(tgt);
	//infoSink.code <<v2.getName() << " "<< tgt.getName()<<"\n";
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

// /* add for loading global variables if need // Goda 2011-03-23
bool TllvmOutputTraverser::GenArithBin_2(const TIntermBinary* const node, const int num)
{
  TllvmVarList varvec,tmpvarvec;
    int i;
    string opstr;
    TllvmVar tgt, ev[8], v;
    //std::cout<< "GenArithBin\n";
    //infoSink.code << "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHi\n";
    opstr = GetBinOp(node->getOp(), node->getBasicType());
    if(opstr == "") {
        infoSink.info.message(EPrefixInternalError,
			      "unsupported operation",
			      node->getLine());
	return false;
    }

    for(i=0;i<num;i++){
      //std::cout<<"v"<<i<<" : "<<v.getCompleteString()<<"\n";                                         
      //infoSink.code <<"pop?\n";
      v = pop_expstack_rhs();
      //infoSink.code <<"Start GenArithBin??\n";
      //infoSink.code <<"v"<<i<<" : "<<v.getName()<<" isMatrix(): "<<v.isMatrix()<< " HaveIndex(): "<< v.haveIndex() <<"\n";                                                   
      //infoSink.code <<"v"<<i<<" : "<<v.getName()<<" "<<v.getType().getCompleteString()<<"\n";                                                   
      //infoSink.code << v.getQualifier() <<"\n";
      /* suppose variable cannot be a matrix here                                                      
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
	  //      varvec.insert(varvec.begin(),v);    //-> 0 1 2 3 4 5 6                               
	  vartmp.push_back(v);  //-> 3 2 1 0 4 5 6                                                     
         
        }                                                                                              
	for(s=1;s<=k;s++){                                                                             
	//      const TString &str = allocate_tempvar().c_str();                                     
	  TllvmVar ts( str , v.getType(2));                                                             
	  GenCallRegister(ts,v.getType());                                                             
	  v.setName(str);                                                                              
	  varvec.insert(varvec.begin(),v); //4 5 6                                                     
	  if(j == k-1 ){ //Check it when CallRegister's for-loop is finished.                          
	  //                                                                                           
         
	  const TString &str = allocate_tempvar().c_str();                                             
          TllvmVar tm( str , v.getType());                                                             
                                                                                                       
          GenChangeMatrix(vartmp,tm);                                                                  
          v.setName(str);                                                                              
          varvec.push_back(v);  //-> 3 2 1 0 4 5 6                                                     
          //  varvec.insert(varvec.begin(),v);                                                         
                                                                                                       
          //To pop all unwanted temp_var (because we saved it in CallRegister. )                       
          //      for(j=0;j<=k;j++)                                                                    
          //  varvec.pop_back();                                                                       
          //      v= pop_expstack();                                                                   
	}                                                                                              
      }                                                                                                
      else{                                                                                            
      */
      //infoSink.code << "+++++++++"<<v.getName() <<"\n";
      if( !v.isMatrix() ) // Except For the instruction about matrix/float.
	LoadReserveVariableFirstTime( v );
      varvec.push_back(v);

      ev[i]=v;
    }

    if(ev[0].getName() == "" || ev[1].getName() == "") {
      infoSink.info.message(EPrefixInternalError,
			    "Popping empty expression stack",
			    node->getLine());
      return false;
    }

    if( opstr == "fdiv" && ev[1].isMatrix() && ev[1].getType() != ev[0].getType() ){  //  For example: ( mat ) Matrix / (float ) detm
      if( ev[0].getType() != ev[1].getType() ){ //maybe the ev[1]'s type is less than ev[0] 
	ev[0].setType( ev[1].getType() );
      }
      //infoSink.code <<"Come here\n";
      //infoSink.code <<ev[0].getName() << " "<< ev[1].getName()<<"\n";
      //infoSink.code <<ev[0].getCompleteString() << " "<< ev[1].getCompleteString()<<"\n";
      int SizeOfMatrix = ev[1].getNominalSize();
      TString SaveTheNameOfMatrix = ev[1].getName();
      TString SaveTheNameOfDivisor = ev[0].getName();
      char IndexOfMatrix = '0';
      //infoSink.code << SizeOfMatrix << "\n";
      
      
  
      TType Newtype( EbtFloat , EvqTemporary, SizeOfMatrix , false , false , EbpHigh);
      ev[1].setType( Newtype );
      ev[0].setType( Newtype );
      
      //Generate the ConstructVector instruction.
      const TString &str = allocate_tempvar().c_str();
      infoSink.code << "\t" << str
		    << " = call " << Newtype.getCompleteString()
		    << " @OpenGLES.ConstructVec4.f.f.f.f(" 
		    << "float" << SaveTheNameOfDivisor 
		    << " ,float" << SaveTheNameOfDivisor 
		    << " ,float" << SaveTheNameOfDivisor 
		    << " ,float" << SaveTheNameOfDivisor 
		    << ")\n";
      ev[0].setName( str );
      
      while( SizeOfMatrix-- > 0 ){


	TString NameOfMatrix = SaveTheNameOfMatrix;
	NameOfMatrix += ".";
	NameOfMatrix += IndexOfMatrix++;
	ev[1].setName( NameOfMatrix );

	push_expstack(ev[1]);
	push_expstack(ev[0]);
	GenOpenGLFunc( "FloatDiv" , ev[1].getType() , 2 ); // Note : this assigning type will be different from the dst's variable
	
	if( IndexOfMatrix-48 < ev[1].getNominalSize() ){ //print the assignment of matrix. The last assignment is in the genassign function.
	  //infoSink.code << NameOfMatrix << "\n";
	  const TString &str = allocate_tempvar().c_str();
	  TllvmVar temp = pop_expstack(); 
	  infoSink.code << "\t" << str << " = "<< temp.getName() ;
	 
	  TllvmVar temp1( NameOfMatrix , Newtype );  //Create these variables for its store instruction.
	  TllvmVar temp2( str , Newtype ); 
	  store_vars.push_back( temp1 );
	  store_vars_vector.push_back( temp2 );
	  NameMappingRegister( NameOfMatrix , str );
	}
      }
      return true;
    }
    else if( opstr == "fdiv" && ev[1].getType() != ev[0].getType() ){ //produce the content which is like <float IEEE 754 presentation, float IEEE , ......>. The counter is  the dividend's vector type. Kaymon

      /*if( ev[0].getType() != ev[1].getType() ){ //maybe the ev[1]'s type is less than ev[0] 
	ev[0].setType( ev[1].getType() );
	}*/
      //infoSink.code << "Here??\n";
      //infoSink.code <<"ev[0]: "<< ev[0].getNominalSize() << "  ev[1]: " <<ev[1].getNominalSize()  <<"\n"; //ev[1]:dividend, ev[0]:divisor
      int max = 0;
      if( ev[1].getNominalSize() > ev[0].getNominalSize() ){// divisor's type is less than dividend.
	ev[0].setType( ev[1].getType() );
      }
      else{ // dividend's type is less than divisor.
	ev[1].setType( ev[0].getType() );
	max = 1;
      }
      //infoSink.code << "max?"<<max<<"\n";
      const char *temp; 
      /*
      if(!max){
	bool IsNum = true; // Make sure that the string is number or string. 

	temp = ev[0].getName().c_str();
	temp++;
	while( *temp != '\0' ){
	  if( *temp < '0' || *temp > '9'){
	    IsNum = false;
	    break;
	  }
	  temp++;
	}
	if( IsNum ){
	  const TString &str = allocate_tempvar().c_str();
	  infoSink.code << "\t" << str
			<< " = call " << ev[1].getCompleteString()
			<< " @OpenGLES.ConstructVec4.f.f.f.f(" 
			<< "float " << ev[0].getName()
			<< " ,float " << ev[0].getName()
			<< " ,float " << ev[0].getName()
			<< " ,float " << ev[0].getName()
			<< ")\n";
	  ev[0].setName( str );
	}
	else{
	  //int  i =  ev[0].getType().getCompleteString()[7] - 48;  //
	  int i = 4;
	  TString name,tempname;
	  tempname = ev[0].getName();
	  name = "<";
	  while( i > 0 ){
	    name += "float ";
	    name += tempname;
	    if( i == 1)
	      break;
	    name += ", ";
	    i--;
	  }
	  name += ">";
	  
	  ev[0].setName( name );
	  push_expstack(ev[1]);
	  push_expstack(ev[0]);
	  GenOpenGLFunc( "FloatDiv" , node->getType() , 2 );
	  return true;
	}
      }
      else{
	//int  i =  ev[0].getType().getCompleteString()[7] - 48;  

	int i = 4;
	TString name,tempname;
	tempname = ev[1].getName();
	name = "<";
	while( i > 0 ){
	  name += "float ";
	  name += tempname;
	  if( i == 1)
	    break;
	  name += ", ";
	  i--;
	}
	name += ">";
	ev[1].setName( name );
	
      }
      */
      //**********IR for division*********** Kaymon
	bool IsNum = true; // Make sure that the string is number or string.  

	temp = ev[max].getName().c_str();
	temp++;
	while( *temp != '\0' ){
	  if( *temp < '0' || *temp > '9'){
	    IsNum = false;
	    break;
	  }
	  temp++;
	}
	if( IsNum ){
	  const TString &str = allocate_tempvar().c_str();
	  infoSink.code << "\t" << str
			<< " = call " << ev[max].getCompleteString()
			<< " @OpenGLES.ConstructVec4.f.f.f.f(" 
			<< "float " << ev[max].getName()
			<< " ,float " << ev[max].getName()
			<< " ,float " << ev[max].getName()
			<< " ,float " << ev[max].getName()
			<< ")\n";
	  ev[max].setName( str );
	  push_expstack(ev[1]); //Kaymon add 2013/6/5
	  push_expstack(ev[0]);
	  GenOpenGLFunc( "FloatDiv" , node->getType() , 2 );
	  return true;
	}
	else{
	  //int  i =  ev[0].getType().getCompleteString()[7] - 48;  //
	  int i = 4;
	  TString name,tempname;
	  tempname = ev[max].getName();
	  name = "<";
	  while( i > 0 ){
	    name += "float ";
	    name += tempname;
	    if( i == 1)
	      break;
	    name += ", ";
	    i--;
	  }
	  name += ">";
	  
	  ev[max].setName( name );
	  push_expstack(ev[1]);
	  push_expstack(ev[0]);
	  GenOpenGLFunc( "FloatDiv" , node->getType() , 2 );
	  return true;
	}

    }
    else if(opstr == "fdiv" ){
      push_expstack(ev[1]);
      push_expstack(ev[0]);
      GenOpenGLFunc( "FloatDiv" , node->getType() , 2 );
      return true;
    }
    else if( opstr == "fadd" && ev[1].getType() != ev[0].getType() ){ // To solve the problem that the operand is not the same type. e.g. fadd <4 x float> %6, 0x4fffffffffff. => fadd <4 x float> %6 , <float 0x4ffffffffff,float 0x4ff ..> 
      //infoSink.code << ev[0].getName() <<" "<<ev[0].getCompleteString() <<" "<<ev[0].getNominalSize() <<"\n";
      //infoSink.code << ev[1].getName() <<" "<<ev[1].getCompleteString() <<" "<<ev[1].getNominalSize() <<"\n";
      //infoSink.code <<ev[0].getType().getCompleteString()[7] <<"\n";
      //infoSink.code <<ev[1].getType().getCompleteString()[7] <<"\n";
      int size;
      TString name,temp;
      int max = 0;
      if( ev[0].getType().getCompleteString()[7] != 'l'){
	ev[1].setType( ev[0].getType() );
	temp = ev[1].getName();
	max = 1;
      }
      else{
	ev[0].setType( ev[1].getType() );
	temp = ev[0].getName();
	max = 0;
      }
      
      bool IsNum = true; // Make sure that the string is number or string.                                                                                                                                       
      const char *p_temp;
      p_temp = ev[max].getName().c_str();
      p_temp++;
      while( *p_temp != '\0' ){
	if( *p_temp < '0' || *p_temp > '9'){
	  IsNum = false;
	  break;
	}
	p_temp++;
      }
      if( IsNum ){
	const TString &str = allocate_tempvar().c_str();
	infoSink.code << "\t" << str
		      << " = call " << ev[max].getCompleteString()
		      << " @OpenGLES.ConstructVec4.f.f.f.f("
		      << "float " << ev[max].getName()
		      << " ,float " << ev[max].getName()
		      << " ,float " << ev[max].getName()
		      << " ,float " << ev[max].getName()
		      << ")\n";
	ev[max].setName( str );
      }
      else{
	size = ev[max].getType().getCompleteString()[7] - 48;
	if( size < 4 ) // <3 x float >, <2 x float> -> <4 x flpat>
	  size = 4;
	name = "<";
	while( size > 0 ){
	  name += "float ";
	  name += temp;
	  if( size == 1)
	    break;
	  name += ", ";
	  size--;
	}
	name += ">";
	ev[max].setName( name );
      
	}


    }
    else if( opstr == "fsub" && ev[1].getType() != ev[0].getType() ){ // To solve the problem that the operand is not the same type. e.g. fadd <4 x float> %6, 0x4fffffffffff. => fsub <4 x float> %6 , <float 0x4ffffffffff,float 0x4ff ..> 
      
      int size,path;
      TString name,temp;
      if( ev[0].getType().getCompleteString()[7] != 'l'){
	ev[1].setType( ev[0].getType() );
	temp = ev[1].getName();
	path = 1;
      }
      else{
	ev[0].setType( ev[1].getType() );
	temp = ev[0].getName();
	path = 2;
      }
      size = ev[0].getType().getCompleteString()[7] - 48;
      if( size < 4 ) // <3 x float >, <2 x float> -> <4 x flpat>
	size = 4;
      name = "<";
      while( size > 0 ){
	name += "float ";
	name += temp;
	if( size == 1)
	  break;
	name += ", ";
	size--;
      }
      name += ">";
      if( path == 1 )
	ev[1].setName( name );
      else
	ev[0].setName( name );

    }
    else if ( ev[0].isMatrix() && ev[1].isMatrix() ){ //Handle the matrix operation. e.g matrix + matrix.
      if(ev[0].getName() == "Matrix" && ev[1].getName() == "Matrix"){ //Come a TllvmVar class called Matrix. It contains the expanded vectors. First add them. Finally pack them into a new TllvmVar class. 
	int rows = ev[0].getNominalSize();                            //It is also called "Matrix".
	for(int i=0;i<rows;i++){
	  tgt = TllvmVar(allocate_tempvar().c_str(), node->getType());
	  GenBinOp(opstr, tgt, ev[0].getVectorsOfMatrix()[i], ev[1].getVectorsOfMatrix()[i]);
	  tmpvarvec.push_back(tgt);
	}
	TllvmVar Matrix=tgt; //Pack $(rows) vectors into a TllvmVar class. It means this class contains $(rows) vectors. 
	Matrix.setName("Matrix");
	for(int index=0;index<rows;index++){
	  Matrix.SaveExpandedVectors(tmpvarvec[index]);
	}
	push_expstack(Matrix);
	return true;
      }
      else
	infoSink.code << "Now it's another matrix operation\n";
    }
    else if(opstr=="sdiv"){ //Our spec do not deal integer devide. So convert type first.

      TllvmVar v1=ev[0],v2=ev[1];
      TllvmVar target;
      opstr = "fdiv";
      
      TInfoSinkBase code;
      TInfoSinkBase code1;
      TInfoSinkBase code3;
      code << "call float @OpenGLES.Convert."
			<<  "sitofp(" << v2.getCompleteString() <<" " << v2.getName()
	   << ")\n";
      
      TllvmVar tv1(code.c_str(), ev[0].getType(), TllvmVarCode);
      
      code1 << "call float @OpenGLES.Convert."
	    <<  "sitofp(" << v1.getCompleteString() <<" " << v1.getName()
	    << ")\n";
      
      TllvmVar tv2(code1.c_str(), ev[1].getType(), TllvmVarCode);
      
      push_expstack(tv1);
      push_expstack(tv2);
      v1 = pop_expstack_rhs();
      v2 = pop_expstack_rhs();
      TType NewType( EbtFloat , EvqTemporary, 1 , false , false , EbpHigh);
      v2.setType(NewType);
      
      const TString &str = allocate_tempvar().c_str();
      target.setName( str );
      GenBinOp(opstr, target , v2, v1);
	  
      code3 << "call i32 @OpenGLES.Convert."
	    <<  "fptosi(" << v2.getCompleteString() <<" " << target.getName()
	    << ")\n";
      TllvmVar tv3(code3.c_str(), ev[0].getType(), TllvmVarCode);
      
      push_expstack(tv3);
      //target = pop_expstack_rhs();
	  //infoSink.code << recordv2name<<" "<<target.getName()<<"\n";
      
      //store_vars_vector.push_back(target); // save the dst's register number.
      //NameMappingRegister( recordv2name , target.getName() );
      
      return true;
    }
    

    tgt = TllvmVar(allocate_tempvar().c_str(), node->getType());
    
    GenBinOp(opstr, tgt, ev[1], ev[0]);

    push_expstack(tgt);

    return true;
}
//  */

bool TllvmOutputTraverser::GenCompare(const TIntermBinary* const node)
{
    string opstr;
    TllvmVar tgt, ev1, ev2;
    //infoSink.code <<"Come in GenCompare\n";
    //added by sheng-chih
    if(node->getLeft()->getType()!=node->getRight()->getType())
      {
	infoSink.info.message(EPrefixInternalError,
			      "Children aren't the same. Need to fix",
			      node->getLine());
	return false;
      }
    //end added
    //fixed by sheng-chih
    //opstr = GetCompOp(node->getOp(), node->getBasicType());
    opstr = GetCompOp(node->getOp(), node->getLeft()->getBasicType());
    //end fixed
    if(opstr == "") {
	infoSink.info.message(EPrefixInternalError,
			      "unsupported operation",
			      node->getLine());
	return false;
    }

    ev1 = pop_expstack_rhs();
    if( ev1.getName() == "ConstructorMat"){
      CompareMatrix = 1;
    }
    ev2 = pop_expstack_rhs();


    if(ev1.getName() == "" || ev2.getName() == "") {
	infoSink.info.message(EPrefixInternalError,
			      "Popping empty expression stack",
			      node->getLine());
	return false;
    }

    /*
    if( DoesNameExist( ev1.getName() ))
      ev1.setName( ret_vector[ev1.getName()] );
    else{ 
      bool IsNum = true; // Make sure that the string is number or string.
      const char *temp;
      temp =ev1.getName().c_str();
      temp++;
      while( *temp != '\0' ){
	if( *temp < '0' || *temp > '9' )
	  IsNum = false;
	temp++;
      }// Done 
	//infoSink.code << IsNum << "\n";
      if( ev1.getQualifier() == EvqTemporary && !IsNum && !strncmp( ev1.getName().c_str() , "%" , 1 ) ){
	//infoSink.code << "============= "<<i<<varvec[i].getName() << "\n";
	const TString &str = allocate_tempvar().c_str();
	TllvmVar ts( str , ev1.getType());
	GenLoad( ts , ev1 );
	NameMappingRegister( ev1.getName() , str );
	ev1.setName( str );
      }
      else if( ev1.getQualifier() == EvqUniform && !IsNum && !strncmp( ev1.getName().c_str() , "@" , 1 ) ){
	//infoSink.code << "============= "<<i<<varvec[i].getName() << "\n";
	const TString &str = allocate_tempvar().c_str();
	TllvmVar ts( str , ev1.getType());
	GenLoadUniform( ts , ev1 );
	NameMappingRegister( ev1.getName() , str );
	ev1.setName( str );
      }
      else if( ev1.getQualifier() == EvqVaryingIn && !IsNum && !strncmp( ev1.getName().c_str() , "@" , 1 ) ){
	//infoSink.code << "============= "<<i<<varvec[i].getName() << "\n";
	const TString &str = allocate_tempvar().c_str();
	TllvmVar ts( str , ev1.getType());
	GenLoadVarying( ts , ev1 );
	NameMappingRegister( ev1.getName() , str );
	ev1.setName( str );
      }
    }
    if( DoesNameExist( ev2.getName() ))
      ev2.setName( ret_vector[ev2.getName()] );
    else{ 
      bool IsNum = true; // Make sure that the string is number or string.
      const char *temp;
      temp =ev2.getName().c_str();
      temp++;
      while( *temp != '\0' ){
	if( *temp < '0' || *temp > '9' )
	  IsNum = false;
	temp++;
      }// Done 
      //infoSink.code << IsNum << "\n";
      if( ev2.getQualifier() == EvqTemporary && !IsNum && !strncmp( ev2.getName().c_str() , "%" , 1 ) ){
	//infoSink.code << "============= "<<i<<varvec[i].getName() << "\n";
	const TString &str = allocate_tempvar().c_str();
	TllvmVar ts( str , ev2.getType());
	GenLoad( ts , ev2 );
	NameMappingRegister( ev2.getName() , str );
	ev2.setName( str );
      }
      else if( ev2.getQualifier() == EvqUniform && !IsNum && !strncmp( ev2.getName().c_str() , "@" , 1 ) ){
	//infoSink.code << "============= "<<i<<varvec[i].getName() << "\n";
	const TString &str = allocate_tempvar().c_str();
	TllvmVar ts( str , ev2.getType());
	GenLoadUniform( ts , ev2 );
	NameMappingRegister( ev2.getName() , str );
	ev2.setName( str );
      }
      else if( ev2.getQualifier() == EvqVaryingIn && !IsNum && !strncmp( ev2.getName().c_str() , "@" , 1 ) ){
	//infoSink.code << "============= "<<i<<varvec[i].getName() << "\n";
	const TString &str = allocate_tempvar().c_str();
	TllvmVar ts( str , ev2.getType());
	GenLoadVarying( ts , ev2 );
	NameMappingRegister( ev2.getName() , str );
	ev2.setName( str );
      }
      }*/

    if( ev1.getName() == "ConstructorMat" && ev2.isMatrix() ){

      
      push_expstack( GenCompareMatrix(ev1,ev2) );
      
      return true;
    }
    else{
      //infoSink.code << ev2.getQualifier() << "\n";
      //infoSink.code << ev2.getCompleteString() << "\n";
      //infoSink.code << ev1.getQualifier() << "\n";
      //infoSink.code << ev1.getCompleteString() << "\n";
      //infoSink.code << "ev1: "<<ev1.getName() <<" ev2: "<< ev2.getName() << "\n";
      LoadReserveVariableFirstTime(ev1);
      LoadReserveVariableFirstTime(ev2);
      //infoSink.code << "ev1: "<<ev1.getName() <<" ev2: "<< ev2.getName() << "\n";
      //infoSink.code << ev2.getQualifier() << "\n";
      //infoSink.code << ev2.getCompleteString() << "\n";
      //infoSink.code << ev1.getQualifier() << "\n";
      //infoSink.code << ev1.getCompleteString() << "\n";
      tgt = TllvmVar(allocate_tempvar().c_str(), TType(EbtBool));
      //infoSink.code << opstr << "\n";
      GenBinOp(opstr, tgt, ev2, ev1);
      push_expstack(tgt);
      AfterGenCompareInstruction=1;//To avoid to generate another conversion instruction.      
      return true;
    }
}
TllvmVar TllvmOutputTraverser::GenCompareMatrix(TllvmVar rhs,TllvmVar lhs)
{

  TllvmVarList varvec;
  TllvmVarIndexList il = lhs.getIndex();
  int SizeofMatrix = lhs.getNominalSize();
  int SizeofParameters = (int)RecordParametersforStructure.size();
  //infoSink.code << SizeofParameters <<"\n";
  bool AssignFloatToMatrix = false;
  if( SizeofMatrix != SizeofParameters ){
    AssignFloatToMatrix = true;
    //SizeofParameters > SizeofMatrix ? SizeofMatrix = SizeofParameters : SizeofMatrix ;
  }
  const TString &str = allocate_tempvar().c_str();
  TType Newcurtype(EbtFloat,EvqTemporary,SizeofMatrix,false,false,EbpHigh); //Creat vec4 type.
  TllvmVar rt( lhs );
  rt.setName( str );
  
  infoSink.code << "\t" << rt.getName() << " = " 
		<< "getelementptr inbounds " << il[0].type.getCompleteString() << "* "
		<< lhs.getName() << ", i32 0, i32 " << il[0].index.getName() <<"\n";
  
  const TString &str1 = allocate_tempvar().c_str();
  TllvmVar rt1( lhs );
  rt1.setName( str1 );
  
  for( int IndexofMatrix = SizeofMatrix-1 ; IndexofMatrix >= 0 ; IndexofMatrix-- ){
	
    infoSink.code << "\t" << rt1.getName() << " = " 
		  << "getelementptr inbounds " << lhs.getCompleteString() << "* "
		  << rt.getName() << ", i32 0, i32 " << IndexofMatrix <<"\n";

    const TString &str2 = allocate_tempvar().c_str();
    TllvmVar load_dst(str2,Newcurtype);
    rt1.setType( Newcurtype );
    GenLoad(load_dst,rt1);

    TllvmVar tempvar(load_dst);
    if( AssignFloatToMatrix ){
      
      TType TmpType( Newcurtype );
      string FnnameforConstruct;
      TInfoSinkBase InttoAscii;
      TllvmVarList VarList;
      int indexforsize = 0;
      InttoAscii <<  Newcurtype.getNominalSize();
      //infoSink.code << Newcurtype.getNominalSize()<<"\n";
      //infoSink.code << SizeofParameters<<"\n";
      FnnameforConstruct = "OpenGLES.Construct";
      if(TmpType.getBasicType() == EbtBool)
	FnnameforConstruct += "B";
      else if(TmpType.getBasicType() == EbtInt)
	    FnnameforConstruct += "I"; 
      FnnameforConstruct += "Vec";
      FnnameforConstruct += InttoAscii.c_str();
      //out.code <<"curtype: " << curtype.getCompleteString()  <<" size : " << curtype.getNominalSize() << "\n";
      
      while( indexforsize < Newcurtype.getNominalSize() ) {
	VarList.push_back( RecordParametersforStructure[0] );
	RecordParametersforStructure.erase( RecordParametersforStructure.begin() ); //Remove the register which is already assgined.
	indexforsize++;
	//infoSink.code << v.getName() <<" "<<v.isEndConstructVector() <<"after pop1\n";
      }
      FnnameforConstruct = DepolymorphismFunctionName(FnnameforConstruct, VarList);
      //infoSink.code << FnnameforConstruct <<"\n";
      
      TllvmVar tmpv("", TmpType, TllvmVarCode);      
      GenFunc(tmpv, FnnameforConstruct, VarList);      
      
      tempvar.setType( Newcurtype );
      TllvmVar tmpv1 = pop_expstack_rhs();
      
      const TString &str3 = allocate_tempvar().c_str();
      TType bvec4(EbtBool,EvqTemporary,SizeofMatrix,false,false,EbpHigh); //Creat bvec4 type.
      TllvmVar final_dst(str3,bvec4); 
      
      varvec.push_back(final_dst);

      GenBinOp("fcmp eq", final_dst, tempvar, tmpv1);
      //GenStore( tempvar ,tmpv1  ); 
      if( IndexofMatrix  != 0 )
	rt1.setName( allocate_tempvar().c_str() );
      

    }
    else{
      rt1.setType( Newcurtype );
      GenStore( rt1 , RecordParametersforStructure[0] );
      RecordParametersforStructure.erase( RecordParametersforStructure.begin() ); //Remove the register which is already assgined.
      if( IndexofMatrix  != 0 )
	rt1.setName( allocate_tempvar().c_str() );
    }
  }

  //infoSink.code << (int)varvec.size() <<"\n";

  TllvmVar return_dst( TllvmVar(allocate_tempvar().c_str(), TType(EbtBool)) );
  //GenBinOp("and", return_dst, varvec[0], varvec[1]);
  infoSink.code << "\t" << return_dst.getName() <<" = call "
		<< return_dst.getCompleteString() << " @OpenGLES.Compare."
		<< SizeofMatrix <<"(<4 x i1> "<< varvec[0].getName()
		<< ", <4 x i1> " << varvec[1].getName();
  if( SizeofMatrix >= 3 )
    infoSink.code << ", <4 x i1> " << varvec[2].getName();
  if( SizeofMatrix == 4)
    infoSink.code << ", <4 x i1> " << varvec[3].getName();

  infoSink.code << ")\n"; 

  return return_dst;
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
    //infoSink.code << "ev1: "<<ev1.getName()<<" ev2: "<<ev2.getName()<<"\n";
    const char *temp;
    bool IsNum=true;
    bool IsNum2=true;
    temp = ev2.getName().c_str();
    temp++;
    while( *temp != '\0' ){
      if( *temp < '0' || *temp > '9'){
	IsNum = false;
	break;
      }
      temp++;
    }// 
    temp = ev1.getName().c_str();
    temp++;
    while( *temp != '\0' ){
      if( *temp < '0' || *temp > '9'){
	IsNum2 = false;
	break;
      }
      temp++;
    }
    //infoSink.code << IsNum <<" "<<IsNum2<<"\n";
    LoadReserveVariableFirstTime(ev1); //load the register number mapping to variables. 
    LoadReserveVariableFirstTime(ev2);
    //infoSink.code << "ev1: "<<ev1.getName()<<" ev2: "<<ev2.getName()<<"\n";
    //infoSink.code << "ev1: "<<ev1.getCompleteString()<<" ev2: "<<ev2.getCompleteString()<<"\n";
    tgt = TllvmVar(allocate_tempvar().c_str(), TType(EbtBool));

    //GenBinOp(opstr, tgt, ev2, ev1);
    char buf[200];
    if(IsNum&&IsNum2){
      //e.g. and i1 Num Num
      //%34 = icmp sge i32 %33, 11
      //%35 = icmp sle i32 %33, 13
      //%36 = and i1 %34, %35
      if(AfterGenCompareInstruction == 1 ){
	sprintf(buf, "\t%s = %s i1 %s, %s\n",
		tgt.getName().c_str(),
		opstr.c_str(),
		ev2.getName().c_str(),
		ev1.getName().c_str());
      }
      else{
	sprintf(buf, "\t%s = %s i32 %s, %s\n",
		tgt.getName().c_str(),
		opstr.c_str(),
		ev2.getName().c_str(),
		ev1.getName().c_str());
      }
      infoSink.code << buf;
    }
    else{ //e.g. and type name num
          //e.g. %3 = load i32* %result, align 4 
          //     %4 = and i32 %3, 1
      sprintf(buf, "\t%s = %s %s %s, %s\n",
	      tgt.getName().c_str(),
	      opstr.c_str(),
	      ev2.getCompleteString().c_str(),
	      ev2.getName().c_str(),
	      ev1.getName().c_str());
      infoSink.code << buf;
    }
    push_expstack(tgt);

    return true;

}

TllvmVar TllvmOutputTraverser::GenGetRHS(const TllvmVar &rhspv, const int line)
{
  //infoSink.code <<"rhspv: "<<rhspv.getName() << " "<<rhspv.getType().getCompleteString()<<" "<<rhspv.getAsFloat() <<" isvec?:  "<< rhspv.isEndConstructVector() <<" "<<rhspv.isVector() <<" haveIndex:"<<rhspv.haveIndex()<<"\n";
  //infoSink.code << rhspv.getName() <<" line:" << line <<" q: " << rhspv.getQualifier() << "\n";
  //infoSink.code << rhspv.getName() <<" "<<(int)rhspv.getVectorsOfMatrix().size()<<" 111111before\n";
  //infoSink.code <<rhspv.DoesItExpand()<<"DDDDDDDDDDDDDDDd\n";
  //infoSink.code << rhspv.getVectorsOfMatrix()[0].getName() <<" 222222before\n";
    if(rhspv.isCode()) {

      if( rhspv.getName() == "ConstructorMat" ){
	return rhspv;
      }
      else{
	//TllvmVar rt(allocate_tempvar().c_str(), rhspv.getType()); //past declaration.just drop it. 
	TllvmVar rt(rhspv);
	rt.setName(allocate_tempvar().c_str());
	infoSink.code << "\t" << rt.getName() << " = " << rhspv.getName();
	rt.setvartype(TllvmVarNormal); //because it is not a TllvmVarCode.
	return rt;
      }
    }
    else if(rhspv.haveIndex()) {
        //infoSink.code <<"ret_vector: "<<ret_vector[rhspv.getName()]<<"\n";
	TllvmVar var ( rhspv ); //Kaymon
	//infoSink.code <<"rhspv: "<<rhspv.getName() <<" Type: "<<rhspv.getType().getCompleteString()<<" Q after getindex: "<<rhspv.getIndex()[0].type.getQualifier() << "\n";
	//infoSink.code <<"isArray(): "<< rhspv.getIndex()[0].type.isArray() << "\n";
	//infoSink.code <<"isStructure(): "<< rhspv.getIndex()[0].type.isStructure() << "\n";
	//infoSink.code <<"isMatrix(): "<< rhspv.getIndex()[0].type.isMatrix() << "\n";
	//infoSink.code <<"var name: " <<var.getName() << " "<< var.getQualifier() <<" "<<var.isMatrix()<<" "<<var.isEndConstructMatrix() <<"\n";
	//infoSink.code << rhspv.getIndex()[0].type.getCompleteString() << " "<<rhspv.getIndex()[0].type.getQualifier() <<" "<< rhspv.getIndex()[0].type.isMatrix() <<"\n";
	
	//infoSink.code <<"Size: " <<(int )rhspv.getIndex().size() <<  "\n";
	//infoSink.code << rhspv.getIndex()[0].index.getName() <<  "\n";
	//infoSink.code << rhspv.getIndex()[0].type.isArray() <<  "\n";
	//infoSink.code << rhspv.getIndex()[0].type.isMatrix() <<  "\n";
	//infoSink.code << rhspv.getIndex()[1].index.getName() <<  "\n";
	//infoSink.code <<"Size: " <<(int )var.getIndex().size() <<  "\n";
	//infoSink.code << var.getIndex()[0].index.getName() <<  "\n";
	//infoSink.code << var.getIndex()[1].index.getName() <<  "\n";
	/*
	int size = (int) rhspv.getIndex().size();
	int IndexofIndexList = 0;
	TllvmVarIndexList list = rhspv.getIndex(); //Because this variable is protected in llvmvar class. we need create new one to fix it.
	//infoSink.code << list[0].index.getName() <<  "\n";
	LoadReserveVariableFirstTime( list[0].index ); //Assign the index number to a vector before a getelementptr instruction. ex: the index of "i" in a while loop.
	
	while( size-- > 0 ){
	  var.pushIndex( list[IndexofIndexList++] ); // Put the types of TllvnVarIndexList into the stack because the var is a new variable.
	}

	//TType tmp( rhspv.getBasicType() , rhspv.getQualifier() , rhspv.getIndex()[0].type.getNominalSize() , 0  , 0 , EbpHigh ); //old
	//var.setType( tmp );
	var.setType( rhspv.getIndex()[0].type ); // New
	*/
	
	if( CompareMatrix ) //For comparing matrix.
	  return rhspv;
	if( ( rhspv.getIndex()[0].type.isArray() && rhspv.getIndex()[0].type.isMatrix() ) || ( rhspv.getIndex()[0].type.isArray() && rhspv.getIndex()[1].index.isSwizzle() ) ){ //for matrix array or array[1].xy.
	  ;
	}
	else if( rhspv.getIndex()[0].type.isArray() ){ // Identify  the variable is array.
	  //infoSink.code << var.getName() <<" " << var.getIndex()[0].index.getName() <<" qualifier in index: " << var.getIndex()[0].type.getQualifier() <<" qualifier not in index: " << var.getQualifier()<<"\n";
	  //infoSink.code << var.getType().getArraySize() <<" " << var.getIndex()[0].type.getArraySize() <<" qualifier in index: " << var.getIndex()[0].type.isArray() <<" qualifier not in index: " << var.isArray()<<"\n";
	  //infoSink.code << " type in type: " << var.getIndex()[0].type.getCompleteString() <<" type in index: " << var.getIndex()[0].index.getCompleteString()<<"\n";
	  TllvmVarIndexList list = rhspv.getIndex();     //Because this variable is protected in llvmvar class. we need create new one to fix it.
	  LoadReserveVariableFirstTime( list[0].index ); //Assign the index number to a vector before a getelementptr instruction. ex: the index of "i" in a while 
	  var.popIndex();                                //Can not modify the data directly. so creat a new one and push into the variable.
	  var.pushIndex(list[0]);
	}
	else if( rhspv.getIndex()[0].type.isMatrix() && rhspv.getIndex()[0].type.getQualifier() == EvqTemporary ){ //Identify the variable is matrix.
	  //infoSink.code <<"HERE\n";
#ifndef NewMatrix	  
	  TString NameOfMatrix = rhspv.getName();
	  NameOfMatrix += '.';
	  NameOfMatrix += rhspv.getIndex()[0].index.getName();
	  var.setName( NameOfMatrix );
	  
	  TType Newtype( EbtFloat , EvqTemporary, var.getNominalSize() , false , false , EbpHigh);
	  var.setType( Newtype );
	  
	  LoadReserveVariableFirstTime( var );
#else
	  //infoSink.code << var.haveIndex() <<" " << (int)var.getIndex().size() <<"\n";
	  //infoSink.code << var.getCompleteString() <<"\n";
	  //infoSink.code <<"New matrix\n";
#endif
	}
	else if( rhspv.getIndex()[0].type.isMatrix() )
	  ;//infoSink.code << "nothing to do \n";
	else if( rhspv.getIndex()[0].type.isStructure())
	  ;
	else if( DoesNameExist( rhspv.getName() ))
	  var.setName( ret_vector[ rhspv.getName() ] );
	else{
	  //infoSink.code << "else\n";
	  bool IsNum = true; // Make sure that the string is number or string.
	  const char *temp;
	  temp = var.getName().c_str();
	  temp++;
	  while( *temp != '\0' ){
	    if( *temp < '0' || *temp > '9' )
	      IsNum = false;
	    temp++;
	  }// Done 
	  //infoSink.code << IsNum << "\n";
	  if( !IsNum ){
	    if( !IsNum && strncmp( rhspv.getName().c_str() , "0x" , 2 ) && strncmp( rhspv.getName().c_str() , "::" , 2 )  )
	      DeclareVariable( var );
	    const TString &str = allocate_tempvar().c_str();
	    TllvmVar ts( rhspv );
	    ts.setName(str);
	    GenLoadVariablefromIndex( ts , var );
	    NameMappingRegister( var.getName() , str );
	    var.setName( str );
	  }
	  
	} //Done

	//infoSink.code <<"var_vector: "<<var.getName()<<"\n";
	//infoSink.code <<"ret_vector: "<<ret_vector[rhspv.getName()]<<"\n";
	
	TllvmVar rt(allocate_tempvar().c_str(),
		    TType(rhspv.getType())); //This vector will be assigned. Dst vector.

	//infoSink.code <<"var :"<<var.getName()<<"\n";
	//infoSink.code <<"rt  :"<<rt.getName()<<"\n";

	
	//GenExtractAgg(rhspv, rt, line); //Original code.
	
	GenExtractAgg( var, rt, line);
	//infoSink.code << rt.getName() <<"isstructure: "<<rhspv.getIndex()[0].type.isStructure() <<"before\n";
	if( rhspv.getIndex()[0].type.isArray() || rhspv.getIndex()[0].type.isMatrix() || rhspv.getIndex()[0].index.isSwizzle() || rhspv.getIndex()[0].type.isStructure()) // Identify  the variable is array or matrix
	  rt.setName( tmp_register_counter.c_str() );
	//infoSink.code <<"rt  :"<<rt.getName()<<"\n";
	//infoSink.code << rt.getName() <<"after\n";
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
    if(!strncmp(fnname.c_str(),"OpenGLES.ChangeMatrix",21)){//Kaymon
      code << "call " << rt.getCompleteString() << " @" << fnname <<"."<<s;
    }
    else if(!strcmp(fnname.c_str(),"OpenGLES.VectorTimesMatrix")){
      TString matrix_subscript;
      matrix_subscript = vec[size-1].getCompleteString();
      code << "call " << rt.getCompleteString() << " @" << fnname << matrix_subscript[7] <<"."<<size;
    }//mark
    else{
      code << "call " << rt.getCompleteString() << " @" << fnname <<"."<<size;//s;
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
      //infoSink.code <<  "discorvery\n";
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
    //infoSink.code << size <<" "<< rt.isCode() <<" ++++++++++++++\n";
    //cout<< vec.size()<<"Come here\n";
    //infoSink.code << fnname <<"\n";

    //if(!rt.isCode()) 
	//code << "\t" << rt.getName() << " = ";
    if(!rt.isCode() ) //Kaymon add the one more condition to this if statement. If returning type is void , we don't push it into stack. Just print it.
	code << "\t" << rt.getName() << " = ";

    code << "call " << rt.getCompleteString() << " @" << fnname;
    //infoSink.code <<"size: "<<size<<"\n";
    if(size>0) {
      
	code << "(";
	
	
	code << vec[size-1].getCompleteString() << " " << vec[size-1].getName();
	
	for(i=size-2;i>=0;--i) {
	  code << ", " << vec[i].getCompleteString()
	       << " " << vec[i].getName();
	  
	}
	code << ")";
    }
    else //size: 0  => no parameters.
      code <<"()";

    code << "\n";
    //infoSink.code << code.c_str();
    //cout << size <<" "<<code.c_str()<<"@@@@@\n";
    //if(!rt.isCode())
    //infoSink.code << code.c_str();
    if(!rt.isCode()|| rt.getCompleteString() == "highp void") //Kaymon add the one more condition to this if statement. If returning type is void , we don't push it into stack. Just print it.
      infoSink.code <<"\t"<< code.c_str();
    else {
      //infoSink.code << code.c_str();
      rt.setVarList(vec); //set the vec var list. In orde to initialize the global variables.
      rt.setName(code.c_str());
      push_expstack(rt);
    }
    //infoSink.code << "END------------------\n";
}

bool TllvmOutputTraverser::GenBinFuncAssign(string fnname, const int line)
{

    if(fnname == "OpenGLES.ScalarTimesVector"){ //Kaymon change the name from VectorTimesScalar to ScalarTimesVector.

      TllvmVarList varvec;
      TllvmVar v1,v2;
      v1 = pop_expstack_rhs();
      v2 = pop_expstack_rhs();
      if( v1.getType().isVector() ){ //Match the function to scalar times vector.
	varvec.push_back( v1 );
	varvec.push_back( v2 );
      }
      else{
	varvec.push_back( v2 );
	varvec.push_back( v1 );
      }
      //infoSink.code << v1.getName()<<"\n";
      //infoSink.code << v2.getName()<<"\n";
      for( int i = 0 ; i < 2 ; i++ ){
	LoadReserveVariableFirstTime( varvec[i] );
      }



      if(varvec[0].getName() == "" || varvec[1].getName() == "") {
	infoSink.info.message(EPrefixInternalError,
			      "Popping empty expression stack",
			      line);
	return false;
      }
      const TString &str = allocate_tempvar().c_str();
      TllvmVar rt( str , varvec[0].getType() );
      GenFunc( rt , fnname, varvec);
      NameMappingRegister(v2.getName(),rt.getName());//record the dst'register
      store_vars.push_back( v2 );      
      store_vars_vector.push_back( rt );      
      push_expstack( rt );

      return true;
    }
    else{
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
}

bool TllvmOutputTraverser::GenBinFuncOp(string fnname, const TType &type, const int line)
{
  //dondum 2011-01-05. Modify for Texture_Cube_Fragment
  //add the special case for VectorTimesScalar
  //Check the argument in function is global or not.
  //If it is global, we load it to a temp variable first.
  if(fnname == "OpenGLES.ScalarTimesVector"){ //Kaymon change the name from VectorTimesScalar to ScalarTimesVector.
    
    TllvmVarList varvec;
    TllvmVar v1,v2;
    v1 = pop_expstack_rhs();
    v2 = pop_expstack_rhs();
    
    //infoSink.code << "v1 : "<<v1.getName() <<"\n";
    //infoSink.code << "V2 : "<<v2.getName() <<"\n";
    //infoSink.code << "V2 : "<<v2.getQualifier() <<"\n";
    //infoSink.code << "v1 : "<<v1.getType().isVector() <<"\n";
    //infoSink.code << "v2 : "<<v2.getType().isVector() <<"\n";
    //infoSink.code << "v1 : "<<v1.getType().getNominalSize() <<"\n";
    //infoSink.code << "v2 : "<<v2.getType().getNominalSize() <<"\n";
    /*    if( v1.getName().compare(0,1,"@")== 0){
      const TString &str = allocate_tempvar().c_str();
      TllvmVar ts( str , v1.getType());
      if( v1.getQualifier() == EvqUniform )
	GenLoadUniform(ts,v1);
      else
	GenLoad(ts,v1);
      v1.setName(str);

      }*/
    if( v1.getType().isVector() ){ //Match the function to scalar times vector.
      varvec.push_back( v1 );
      varvec.push_back( v2 );
    }
    else{
      varvec.push_back( v2 );
      varvec.push_back( v1 );
    }
    //varvec.push_back(v1);
    /*
    if( v2.getName().compare(0,1,"@") == 0){
      const TString &str = allocate_tempvar().c_str();
      TllvmVar ts( str , v2.getType());
	if( v2.getQualifier() == EvqUniform )
	  GenLoadUniform(ts,v2);
	else
	  GenLoad(ts,v2);
      v2.setName(str);

      }*/

    //varvec.push_back(v2);
    
    //Kaymon
    int i = 0;
    while( i < 2 ){
      //infoSink.code <<"v" << i <<": "<<varvec[i].getQualifier()<<"\n";
      if( DoesNameExist( varvec[i].getName() ))
	varvec[i].setName( ret_vector[ varvec[i].getName() ] );
      else{ 
	bool IsNum = true; // Make sure that the string is number or string.
	const char *temp;
	temp = varvec[i].getName().c_str();
	temp++;
	while( *temp != '\0' ){
	  if( *temp < '0' || *temp > '9' )
	    IsNum = false;
	  temp++;
	}// Done 
	//infoSink.code << IsNum << "\n";
	if( varvec[i].getQualifier() == EvqTemporary && !IsNum && !strncmp( varvec[i].getName().c_str() , "%" , 1 ) ){
	  //infoSink.code << "============= "<<i<<varvec[i].getName() << "\n";
	  const TString &str = allocate_tempvar().c_str();
	  TllvmVar ts( str , varvec[i].getType());
	  GenLoad( ts , varvec[i] );
	  NameMappingRegister( varvec[i].getName() , str );
	  varvec[i].setName( str );
	}
	else if( varvec[i].getQualifier() == EvqUniform && !IsNum && !strncmp( varvec[i].getName().c_str() , "@" , 1 ) ){
	  //infoSink.code << "============= "<<i<<varvec[i].getName() << "\n";
	  const TString &str = allocate_tempvar().c_str();
	  TllvmVar ts( str , varvec[i].getType());
	  GenLoadUniform( ts , varvec[i] );
	  NameMappingRegister( varvec[i].getName() , str );
	  varvec[i].setName( str );
	}
	else if( varvec[i].getQualifier() == EvqVaryingIn && !IsNum && !strncmp( varvec[i].getName().c_str() , "@" , 1 ) ){
	  //infoSink.code << "============= "<<i<<varvec[i].getName() << "\n";
	  const TString &str = allocate_tempvar().c_str();
	  TllvmVar ts( str , varvec[i].getType());
	  GenLoadVarying( ts , varvec[i] );
	  NameMappingRegister( varvec[i].getName() , str );
	  varvec[i].setName( str );
	}
	else{
	  LoadReserveVariableFirstTime(varvec[i]);
	}
	  
      } //Done
      i++;
    }
    if(varvec[0].getName() == "" || varvec[1].getName() == "") {
      infoSink.info.message(EPrefixInternalError,
                            "Popping empty expression stack",
                            line);
      return false;
    }




    TllvmVar rt("", type, TllvmVarCode);

    GenFunc(rt, fnname, varvec);

    return true;

  }
  else if(fnname == "OpenGLES.MatrixTimesVector"){  
    TllvmVarList varvec;
    varvec.push_back(pop_expstack_rhs());
    varvec.push_back(pop_expstack_rhs());
    if(varvec[0].getName() == "" || varvec[1].getName() == "") {
      infoSink.info.message(EPrefixInternalError,
			    "Popping empty expression stack",
			    line);
      return false;
    }
    //infoSink.code << varvec[0].getName() <<"\n";
    //infoSink.code << varvec[1].getName() <<"\n";
#ifndef NewMatrix
    if(varvec[1].isMatrix()){
      int SizeofMatrix = varvec[1].getNominalSize();
      fnname += SizeofMatrix+48;
      TString MatrixName = varvec[1].getName();
      varvec.pop_back();
      TllvmVar ts("" , varvec[1].getType());
      for(int index=0;index<SizeofMatrix;index++){
	
	if( varvec[1].getQualifier() == EvqUniform ){
	  const TString &str = allocate_tempvar().c_str();
	  ts.setName(str);
	  GenLoad_M(ts,varvec[1],index,MatrixName);
	  varvec.push_back(ts);
	}
	else if( varvec[1].getQualifier() == EvqTemporary ){ //For the local matrix. Local matrix is defined by three vec4s, so we need to change the matrix type to vec4.
	  TString NameOfMatrix = MatrixName;
	  NameOfMatrix += '.';
	  NameOfMatrix += index+48;
	  
	  //infoSink.code << "\t" << ts.getName() << " = load <"<< v.getCompleteString()[7] << " x float>* " << NameOfMatrix<<", align 16\n";
	  
	  TType Newtype( EbtFloat , EvqTemporary, SizeofMatrix , false , false , EbpHigh);
	  //infoSink.code << Newtype.getCompleteString() << "\n";
	  varvec[1].setType( Newtype );
	  //infoSink.code << NameOfMatrix <<"\n";
	  varvec[1].setName( NameOfMatrix );
	  LoadReserveVariableFirstTime( varvec[1] );
	  

	}
	else
	  infoSink.code << "Need to fix here(GenBinFunOp)\n";

      }
    }
#else
    if(varvec[1].isMatrix()){
      TllvmVar tmp = varvec.back();
      varvec.pop_back();
      int SizeofMatrix = varvec[1].getNominalSize();
      fnname+= SizeofMatrix+48;
      for(int index = 0;index<SizeofMatrix;index++){
	if(tmp.getQualifier() == EvqUniform ){
	  
	  const TString &str = allocate_tempvar().c_str();
	  const TString &str1 = allocate_tempvar().c_str();
	  TType NewType( EbtFloat , EvqTemporary, SizeofMatrix , false , false , EbpHigh);//To generate the type of each row of matrix
	  TllvmVar ts(str , NewType);
	  TllvmVar dst(str1 , NewType);
	  GenRowofMatrix(str,tmp,index);
	  GenLoad(dst,ts);
	  varvec.push_back(dst);
	}
	else{
	  infoSink.code << "Need to fix here(GenBinFuncOp)\n";
	}
      }
    }
#endif

    TllvmVar rt("", type, TllvmVarCode); 
    
    GenFunc(rt, fnname, varvec);
    
    return true;
  }
#ifdef NewMatrix
  else if(fnname == "OpenGLES.MatrixTimesScalar"){  //e.g. matrix * scalar ,but this function shoud expand as follows.
                                                    //Functions will expand to 3 vectors and use 3 ScalarTimesVector function instead.
    TllvmVarList varvec,tmpvar;
      varvec.push_back(pop_expstack_rhs());
      varvec.push_back(pop_expstack_rhs());
      tmpvar = varvec;
      if(varvec[0].getName() == "" || varvec[1].getName() == "") {
	infoSink.info.message(EPrefixInternalError,
			      "Popping empty expression stack",
			      line);
	return false;
      }
      fnname = "OpenGLES.ScalarTimesVector";
      int size = varvec[0].getNominalSize();
      TType Vec4_type(EbtFloat,varvec[0].getQualifier(),4,false,false,EbpHigh); //Creat vec4 type.

      for(int i = 0; i < size ; i++){
	const TString &str = allocate_tempvar().c_str();
	const TString &str1 = allocate_tempvar().c_str();
	//const TString &str2 = allocate_tempvar().c_str();
	TllvmVar ld(str1, Vec4_type);              //later print the register number.
	TllvmVar rt("",type,TllvmVarCode);         //later print the register number.
	rt.ChangeType(Vec4_type);
	GenRowofMatrix(str,varvec[0],i);           //varvec should be fixed.
	tmpvar[0].setName(str);
	tmpvar[0].ChangeType(Vec4_type);
	GenLoad(ld,tmpvar[0]);
	tmpvar[0].setName(str1);
	GenFunc(rt, fnname, tmpvar);
	//infoSink.code << "hi\n";
      }

      tmpvar.clear();
      for(int j=0;j<size;j++){    
	tmpvar.push_back(pop_expstack_rhs());  //Print $(rows) ScalarTimesVector functions.
      }

      TllvmVar Matrix = varvec[0]; //Original Type. Indicate which is matrix type.
      Matrix.setName("Matrix");
      for(int index=0;index<size;index++){
	Matrix.SaveExpandedVectors(tmpvar[index]);  //Pack $(rows) ScalarTimesVector into a new  TllvmVar class called "Matrix".
      }
      push_expstack(Matrix);
      
      return true;
      
    }
#endif
  else  //Original function's code. 
        //bool TllvmOutputTraverser::GenBinFuncOp(string fnname, const TType &type, const int line)
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
      

      
      TllvmVar rt("", type, TllvmVarCode); 

      GenFunc(rt, fnname, varvec);
      
      return true;
      
    }
}
void TllvmOutputTraverser::GenRowofMatrix(TString str,TllvmVar row, int number){
  if(row.getQualifier() == EvqUniform)
    infoSink.code << "\t" << str << " = getelementptr inbounds "<< row.getCompleteString() <<"*"<<row.getName()<<".u.0."<<row.getNominalSize()<<", i32 0, i32 "<<number<<"\n";
  else
    infoSink.code << "\t" << str << " = getelementptr inbounds "<< row.getCompleteString() <<"*"<<row.getName()<<", i32 0, i32 "<<number<<"\n";
}
//dondum/////////////////////////////////////////////////////////////////////////////////
void TllvmOutputTraverser::GenOpenGLFuncOp(string fnname, const TType &type, const int num)
{
  TllvmVarList varvec,vartmp;//tmp is for ChangeMatrix
    int i;
    TllvmVar v;
    fnname = string("OpenGLES.") + fnname;

    for(i=0;i<num;i++){

     v = pop_expstack_rhs();   
     //infoSink.code << v.getName() <<" isMatrix(): "<< v.isMatrix() <<" Index??:"<< v.haveIndex() <<" num: "<< num <<" fname: "<< fnname <<"\n";
     //infoSink.code << v.getName() <<" type:"<< v.getCompleteString()<<"\n";
     if(v.isMatrix()){
       int j, k = v.getNominalSize();
      
       
       //printf("v.matrixSize : %d \n",k);
       TString MatrixName = v.getName();
#ifdef NewMatrix
       //These operations means we should save two different types for getelementptr and load instrs. Newtype is for load instr. OldType is for getelement. 
       TType Newtype( EbtFloat , v.getQualifier(), k , false , false , EbpHigh);
       v.ChangeType(Newtype);	
#endif
       for(j=0;j<k;j++){
	 
	 // TllvmVar ts( allocate_tempvar().c_str() , v.getType());
#ifndef NewMatrix	 
	 if( v.getQualifier() == EvqUniform ){
	   const TString &str = allocate_tempvar().c_str();
	   TllvmVar ts(str , v.getType());
	   GenLoad_M(ts,v,j,MatrixName);
	   v.setName(str);	
	 }
	 else if( v.getQualifier() == EvqTemporary ){ //For the local matrix. Local matrix is defined by three vec4s, so we need to change the matrix type to vec4.
	   TString NameOfMatrix = MatrixName;
	   NameOfMatrix += '.';
	   NameOfMatrix += j+48;

	   //infoSink.code << "\t" << ts.getName() << " = load <"<< v.getCompleteString()[7] << " x float>* " << NameOfMatrix<<", align 16\n";
	  
	   TType Newtype( EbtFloat , EvqTemporary, k , false , false , EbpHigh);
	   //infoSink.code << Newtype.getCompleteString() << "\n";
	   v.setType( Newtype );
	   //infoSink.code << NameOfMatrix <<"\n";
	   v.setName( NameOfMatrix );
	   LoadReserveVariableFirstTime( v );


	 }
	 else
	   infoSink.code << "Need to fix here\n";
#else
	 if( v.getQualifier() == EvqUniform ){//Enable the newmatrix mechanism. Use getelementptr to get the address for indicated vector. Load the vector.
	   const TString &str = allocate_tempvar().c_str();
	   const TString &str1 = allocate_tempvar().c_str();
	   TllvmVar ts(str , v.getOldType());
	   infoSink.code << "\t"<<str << "= getelementptr inbounds " << v.getOldType().getCompleteString() << "* " <<MatrixName <<".u.0."<<k<<", i32 0, i32 "<< j<<"\n";
	   TType Newtype( EbtFloat , EvqUniform, k , false , false , EbpHigh);
	   infoSink.code << "\t"<<str1 << "= load " << Newtype.getCompleteString() << "* " << str <<"\n";
	   v.setName(str1);	
	 }
	 else if( v.getQualifier() == EvqTemporary ){//Compare with above output. This statement lack the "u" because its qualifier is not Uniform.
	   const TString &str = allocate_tempvar().c_str();
	   const TString &str1 = allocate_tempvar().c_str();
	   TllvmVar ts(str , v.getOldType());
	   //infoSink.code << "\t"<<str << "= getelementptr inbounds " << v.getOldType().getCompleteString() << "* " <<MatrixName <<".0."<<k<<", i32 0, i32 "<< j<<"\n";//Expand one name to k name.
	   infoSink.code << "\t"<<str << "= getelementptr inbounds " << v.getOldType().getCompleteString() << "* " <<MatrixName <<", i32 0, i32 "<< j<<"\n"; //Don't print the .0.$k.Only one name in NewMatrix method.
	   infoSink.code << "\t"<<str1 << "= load " << Newtype.getCompleteString() << "* " << str <<"\n";
	   v.setName(str1);

	 }
#endif
	 
	 //	 varvec.insert(varvec.begin(),v);    //-> 0 1 2 3 4 5 6
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
	   /*
	   if( v.getQualifier() == EvqUniform )
	     GenChangeMatrix(vartmp,tm);	
	   else if( v.getQualifier() == EvqTemporary ){
	     infoSink.code << "\t" << tm.getName() << " = call <" << v.getCompleteStrung()[7]
			   << " x float>@OpenGLES.ChangeMatrix"
	   }
	   else
	     infoSink.code << "Need to fix here\n";
*/
	   GenChangeMatrix(vartmp,tm);	
	   v.setName(str);
	   varvec.push_back(v);  //-> 3 2 1 0 4 5 6
	   //  varvec.insert(varvec.begin(),v);

	   //To pop all unwanted temp_var (because we saved it in CallRegister. )
	   //	   for(j=0;j<=k;j++)
	   //  varvec.pop_back();	
	   //	   v= pop_expstack();
       }
     }
     else{
       if(v.isVector() && !v.isTemp()){
	 /*// ------------------------old------------
	 const TString &str = allocate_tempvar().c_str();
	 TllvmVar ts( str , v.getType());

	 if (v.getQualifier() == EvqAttribute ) // It means the vector is belong attribute.
	   GenLoadAttribute(ts,v);
	 else
	   GenLoad(ts,v);
	 v.setName(str);
	 // varvec.push_back(v);       	 
	 */
	 LoadReserveVariableFirstTime(v);
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
    //infoSink.code << "11111111111111111111\n";
    GenFuncOp(tv, fnname, varvec);

}


void TllvmOutputTraverser::GenCallRegister(const TllvmVar &ptr ,const TType &type){

    infoSink.code << "\t" << ptr.getName()
		  << " = call " << type.getCompleteString() //Our register is always vector4. //it will be always vec4.
	          << " " << "@OpenGLES.CallRegister()" <<"\n";


}
void TllvmOutputTraverser::GenCallFloatRegister(const TllvmVar &ptr ,const TType &type){

    infoSink.code << "\t" << ptr.getName()
		  << " = call " << type.getCompleteString()
	          << " " << "@OpenGLES.CallFloatRegister()" <<"\n";


}

void TllvmOutputTraverser::GenLoadVariablefromIndex(const TllvmVar &dst,const TllvmVar &src){ // This function should be called after you make sure wheather var.haveIndex() is true

  /*infoSink.code << "\t" << dst.getName()
		  << " = load " << src.getIndex()[0].type.getCompleteString()
	          << "* " << src.getName()
		  <<"\n"; 
  */
  int size = dst.getIndex()[0].type.getNominalSize();
  //infoSink.code << dst.getIndex()[0].type.getNominalSize() <<" "<< dst.getType().getNominalSize() ;
  //infoSink.code << dst.getName() <<" " << src.getName() <<" qualifier in index: " << dst.getIndex()[0].type.getQualifier() <<" qualifier not in index: " << dst.getQualifier()<<"\n";

  if (src.getIndex()[0].type.getQualifier() == EvqAttribute &&  !strncmp( src.getName().c_str() , "@" , 1 ) ){ // It means the vector's qualifier  is attribute.           
    infoSink.code << "\t" << dst.getName()
		  << " = call " << dst.getIndex()[0].type.getCompleteString()
		  << " " << "@OpenGLES.LoadAttribute"<< size <<"."
		  << src.getName().substr(1)
		  <<".0()\n"; 
    }
  else if ( src.getIndex()[0].type.getQualifier() == EvqUniform && !strncmp( src.getName().c_str() , "@" , 1 ) ){ //Modify by Kaymon
    
    infoSink.code << "\t" << dst.getName()
		  << " = load " << dst.getIndex()[0].type.getCompleteString()
		  << "* " << src.getName() << ".u.0." <<size/4 //mark 
		  << ", align 16"<<"\n";

  }
  else if ( src.getIndex()[0].type.getQualifier() == EvqTemporary &&  !strncmp( src.getName().c_str() , "%" , 1 ) ){

    if( size == 2 || size ==3 )
      size = 1;
    if( src.getIndex()[0].type.isMatrix() ){
      TType Newtype( EbtFloat , EvqTemporary, src.getIndex()[0].type.getNominalSize() , false , false , EbpHigh);
      infoSink.code << "\t" << dst.getName()
		    << " = load " << Newtype.getCompleteString()
		    << "* " << src.getName()
		    <<", align "<< 4*size <<"\n"; 
    }
    else{
      infoSink.code << "\t" << dst.getName()
		    << " = load " << dst.getIndex()[0].type.getCompleteString()
		    << "* " << src.getName()
		    <<", align "<< 4*size <<"\n"; 
    }
  }
  else if( src.getIndex()[0].type.getQualifier()  == EvqVaryingIn &&  !strncmp( src.getName().c_str() , "@" , 1 ) ){

    infoSink.code << "\t" << dst.getName()
		  << " = call " << dst.getIndex()[0].type.getCompleteString()
	          << " " << "@OpenGLES.LoadVarying" << size << "."
		  << src.getName().substr(1)
		  <<".0()"<<"\n"; 
  }
  else if( src.getIndex()[0].type.getQualifier()  == EvqGlobal &&  !strncmp( src.getName().c_str() , "@" , 1 ) ){

    if( size == 2 || size ==3 )
      size = 1;
    
    infoSink.code << "\t" << dst.getName()
		  << " = load " << dst.getIndex()[0].type.getCompleteString()
		  << "* " << src.getName()
		  <<", align "<< 4*size <<"\n"; 
  }
  else if( src.getIndex()[0].type.getQualifier()  == EvqFragCoord &&  !strncmp( src.getName().c_str() , "@" , 1 ) ){

    if( size == 2 || size ==3 )
      size = 1;
    
    infoSink.code << "\t" << dst.getName()
		  << " = load " << dst.getIndex()[0].type.getCompleteString()
		  << "* " << src.getName()
		  <<", align "<< 4*size <<"\n"; 
  }
  else if ( src.getIndex()[0].type.getQualifier() == EvqIn &&  !strncmp( src.getName().c_str() , "%" , 1 ) ){

    if( size == 2 || size ==3 )
      size = 1;
    infoSink.code << "\t" << dst.getName()
		  << " = load " << dst.getIndex()[0].type.getCompleteString()
		  << "* " << src.getName()
		  <<", align "<< 4*size <<"\n"; 
  }
  else {
    infoSink.code << src.getIndex()[0].type.getQualifier() << "\n";
    infoSink.code << "Load variable from Index failed\n";
  }
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

void TllvmOutputTraverser::GenLoadUniform(const TllvmVar &tr ,const TllvmVar &v){
  // all the hard code "<4 x float>" needs to be modified in the future !!!
  int size = v.getType().getNominalSize();
  TString varname = v.getName();
  //Kaymon
  //infoSink.code << v.getNominalSize()<<"\n" ;

  if( size < 4 )
    size = 4;
    infoSink.code << "\t" << tr.getName()
		  << " = load " << v.getType().getCompleteString() <<"*"
	          << " " << v.getName() << ".u.0." <<size/4; //mark 
    if(v.getCompleteString() == "highp sampler2D")
      infoSink.code << ".s, align 16\n";
    else if(v.getCompleteString() == "highp samplerCube")
      infoSink.code << ".c, align 16\n";
    else
      infoSink.code << ", align 16"<<"\n";


    

}

void TllvmOutputTraverser::GenLoadVarying(const TllvmVar &tr ,const TllvmVar &v){
  // all the hard code "<4 x float>" needs to be modified in the future !!!
  int size = v.getType().getNominalSize();
  TString varname = v.getName();

    infoSink.code << "\t" << tr.getName()
		  << " = call " << v.getType().getCompleteString()
	          << " " << "@OpenGLES.LoadVarying" << size << "."
		  << varname.substr(1,varname.length()) 
		  <<".0()"<<"\n"; //means 0 argument 

    

}

void TllvmOutputTraverser::GenVaryingOut(const TllvmVar &dst, const TllvmVar &tr ,const TllvmVar &v){
  // all the hard code "<4 x float>" needs to be modified in the future !!!
  //int size = v.getType().getNominalSize();
  TString varname = dst.getName();

    infoSink.code << "\t" << tr.getName()
		  << " = call " << /*"<4 x float>"*/v.getType().getCompleteString()
	          << " " << "@OpenGLES.VaryingPut4" <<"."
		  << varname.substr(1,varname.length()) <<".1"//means 1 argument 
		  << "("<</*"<4 x float>"*/v.getType().getCompleteString()<<" "<< v.getName()<<")" <<"\n";
    

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
    //infoSink.code << "222222222222222222222\n";
    GenFuncOp(var, fnname, varvec);

}

void TllvmOutputTraverser::GenENDVertex(const TllvmVar &var1, const TllvmVar &varRet){

  TllvmVarList varvec;
  string fnname = string("OpenGLES.END");
  TllvmVar var(varRet.getName(), varRet.getType());

      if( var1.isCode() ){
	const TString &str = allocate_tempvar().c_str();
	TllvmVar ts( str , varRet.getType());
	ts.setName(str);
	varvec.push_back(ts); 
	infoSink.code << "\t" << ts.getName() <<" = " << var1.getName();
      }
      else{
	//TllvmVar ts( var1.getName() , var1.getType());
	//LoadReserveVariableFirstTime( ts );
	//varvec.push_back(ts);
	varvec.push_back( var1 );
      }

	const TString &stv = allocate_tempvar().c_str();
	TllvmVar tr( stv , varRet.getType());
	tr.setName(stv);
	GenCallRegister(tr,varRet.getType());
	varvec.insert(varvec.begin(),tr); 

	const TString &stv2 = allocate_tempvar().c_str(); //Kaymom
	TllvmVar tr2( stv2 , varRet.getType());
	tr2.setName(stv2);
	GenCallRegister(tr2,varRet.getType());
	varvec.insert(varvec.begin(),tr2); 
	
	//infoSink.code<<varRet.getName(); //print "@gl_position"
	//infoSink.code << "33333333333333\n";
	const TString &str1 = allocate_tempvar().c_str(); //vector number replaces the word named gl_position.
	NameMappingRegister( var.getName() , str1 );
	//infoSink.code <<var.getName() <<ret_vector[var.getName()]<<"xxx\n";
	var.setName(str1);
	store_vars_vector.push_back( var );
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
	//infoSink.code << "4444444444444444444444\n";
	//infoSink.code <<"var "<<var.getName()<<"\n";
	const TString &str1 = allocate_tempvar().c_str(); //Kaymon ----------old-----------
	NameMappingRegister( var.getName() , str1 );
	//infoSink.code <<var.getName() <<ret_vector[var.getName()]<<"xxx\n";
	var.setName(str1);
	store_vars_vector.push_back( var );
	GenFuncOp(var, fnname, varvec);	
	/*
	NameMappingRegister( var.getName() , str ); //-----------New------------
	var.setName(str);
	store_vars_vector.push_back( var );*/
	
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
	  //int si;
	  //si = varvec.size();
	  //std::cout<<si<<std::endl;
	  GenLoadVarying(ts,varRet);

	  
	  const TString &stk = allocate_tempvar().c_str();
	  TllvmVar tk( stk , var1.getType());
	  tk.setName(stk);
	  varvec.insert(varvec.begin(),tk); 
	  //	  varvec.push_back(tk); 
	  
	  TString varname = var.getName();
	  
	  int i;
	  int size = var.getType().getNominalSize();
	  infoSink.code << "\t" << tk.getName() << " = shufflevector "
			<< var.getCompleteString() << " "
	    //dondum do this temporary.
			<< ts.getName() << ", "//"%"<<varname.substr(1,varname.length()) << ", "
			<< var.getCompleteString() << " undef, "
			<< "<" << size << " x i32> <i32 0";
	  
	  for(i=1;i<size;++i)
	  infoSink.code << ", i32 " << i;
	  infoSink.code << ">\n";
	  
	  //	infoSink.code << "\t" << ts.getName() << " = " << var1.getName();
	  	  varvec.pop_back(); 
	  
	}else if(varRet.getQualifier() == EvqUniform ){      
	  const TString &str = allocate_tempvar().c_str();
	  TllvmVar ts( str , var1.getType());
	  ts.setName(str);
	  varvec.push_back(ts); 
	  //int si;
	  //si = varvec.size();
	  //std::cout<<si<<std::endl;
	  GenLoadUniform(ts,varRet);
	  
	  
	  const TString &stk = allocate_tempvar().c_str();
	  TllvmVar tk( stk , var1.getType());
	  tk.setName(stk);
	  varvec.insert(varvec.begin(),tk); 
	  //	  varvec.push_back(tk); 
	  
	  TString varname = var.getName();
	  
	  int i;
	  int size = var.getType().getNominalSize();
	  infoSink.code << "\t" << tk.getName() << " = shufflevector "
			<< var.getCompleteString() << " "
	    //dondum do this temporary.
			<< ts.getName() << ", "//"%"<<varname.substr(1,varname.length()) << ", "
			<< var.getCompleteString() << " undef, "
			<< "<" << size << " x i32> <i32 0";
	  
	  for(i=1;i<size;++i)
	    infoSink.code << ", i32 " << i;
	  infoSink.code << ">\n";
	  varvec.pop_back(); 
	}	  
	//	infoSink.code << "\t" << ts.getName() << " = " << var1.getName();
	else{
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
			<< varname /*"%"<<varname.substr(1,varname.length()) */<< ", "
			<< var.getCompleteString() << " undef, "
			<< "<" << size << " x i32> <i32 0";
	  
	  for(i=1;i<size;++i)
	    infoSink.code << ", i32 " << i;
	  infoSink.code << ">\n";

	  //	infoSink.code << "\t" << ts.getName() << " = " << var1.getName();
	  
	}
       	
	/*
	  const TString &stv = allocate_tempvar().c_str();
	  TllvmVar tr( stv , varRet.getType());
	  tr.setName(stv);
	  GenCallRegister(tr,varRet.getType());
	  varvec.insert(varvec.begin(),tr); 
	*/
	TllvmVar va(var1.getName(), var1.getType());
	//infoSink.code << fnname<<" 5555555555555555555555555555\n";
	const TString &str1 = allocate_tempvar().c_str(); //Kaymon
	NameMappingRegister( va.getName() , str1 );
	//infoSink.code <<va.getName() <<ret_vector[va.getName()]<<"xxx\n";
	va.setName(str1);
	store_vars_vector.push_back( va );
	GenFuncOp(va, fnname, varvec);	
	
}



/////////////////////////////////////////////////////////////////////////////////////////


void TllvmOutputTraverser::GenOpenGLFunc(string fnname, const TType &type, const int num)
{
    TllvmVarList varvec;
    int i;
    //mark

    fnname = string("OpenGLES.") + fnname;
    //infoSink.code << fnname << "\n";
    int num2 = num;

     for(i=0;i<num2;++i){
       varvec.push_back(pop_expstack_rhs());
       //cout<< "varvec[ "<<i<<" ]:"<<varvec[i].getName() <<" "<<varvec[i].getQualifier()<<" "<<fnname<<" "<<varvec[i].isEndConstructVector()<<"\n";
       //infoSink.code << "index:  "<<i<<" GetName(): "<<varvec[i].getName() <<" Type: "<<varvec[i].getType().getCompleteString()<<" "<< varvec[i].isMatrix()<< "\n";
       //infoSink.code <<"varvec[ "<<i<<" ]:"<<varvec[i].getName() <<" "<<varvec[i].getQualifier()<<" "<<fnname<<" "<<varvec[i].isEndConstructVector()<<"\n";
       //infoSink.code << "See here "<<ret_vector[varvec[i].getName()]<<"\n";
       //infoSink.code << varvec[i].getQualifier() <<"\n";
       if( varvec[i].isMatrix() ) //filter out matrix type.
	 ;
       else if( DoesNameExist( varvec[i].getName() ))
	 varvec[i].setName( ret_vector[ varvec[i].getName() ] );
       else{ //Kaymon
	 bool IsNum = true; // Make sure that the string is number or string.
	 const char *temp;
	 temp = varvec[i].getName().c_str();
	 temp++;
	 while( *temp != '\0' ){
	   if( *temp < '0' || *temp > '9' )
	     IsNum = false;
	   temp++;
	 }// Done 
	 //infoSink.code <<i <<" "<<IsNum << "\n";
	 if(!IsNum && strncmp( varvec[i].getName().c_str() , "0x" , 2 ) && strncmp( varvec[i].getName().c_str() , "::" , 2 ))
	    DeclareVariable( varvec[i] ); //might be wrong.

	 if( varvec[i].getQualifier() == EvqTemporary && !IsNum && !strncmp( varvec[i].getName().c_str() , "%" , 1 ) ){
	   const TString &str = allocate_tempvar().c_str();
	   TllvmVar ts( str , varvec[i].getType());
	   GenLoad( ts , varvec[i] );
	   NameMappingRegister( varvec[i].getName() , str );
	   varvec[i].setName( str );
	 }
	 if( varvec[i].getQualifier() == EvqUniform && !IsNum && !strncmp( varvec[i].getName().c_str() , "@" , 1 ) ){//&& !strcmp( fnname.c_str() , "OpenGLES.Normalize" ) ){
	   const TString &str = allocate_tempvar().c_str();
	   TllvmVar ts( str , varvec[i].getType());
	   GenLoadUniform( ts , varvec[i] );
	   NameMappingRegister( varvec[i].getName() , str );
	   //infoSink.code << ret_vector[varvec[0].getName()] << "\n";
	   varvec[i].setName( str );
	 }
	 if( varvec[i].getQualifier() == EvqAttribute && !IsNum && !strncmp( varvec[i].getName().c_str() , "@" , 1 ) ){//&& !strcmp( fnname.c_str() , "OpenGLES.Normalize" ) ){
	   const TString &str = allocate_tempvar().c_str();
	   TllvmVar ts( str , varvec[i].getType());
	   GenLoadAttribute( ts , varvec[i] );
	   NameMappingRegister( varvec[i].getName() , str );
	   //infoSink.code << ret_vector[varvec[0].getName()] << "\n";
	   varvec[i].setName( str );
	 }
	 else if( varvec[i].getQualifier() == EvqVaryingIn && !IsNum && !strncmp( varvec[i].getName().c_str() , "@" , 1 ) ){//&& !strcmp( fnname.c_str() , "OpenGLES.Normalize" ) ){
	   const TString &str = allocate_tempvar().c_str();
	   TllvmVar ts( str , varvec[i].getType());
	   GenLoadVarying( ts , varvec[i] );	
	   NameMappingRegister( varvec[i].getName() , str );
	   varvec[i].setName( str );
	 }
	 else if( varvec[i].getQualifier() == EvqIn && !IsNum && !strncmp( varvec[i].getName().c_str() , "%" , 1 ) ){//&& !strcmp( fnname.c_str() , "OpenGLES.Normalize" ) ){
	   const TString &str = allocate_tempvar().c_str();
	   TllvmVar ts( str , varvec[i].getType());
	   GenLoad( ts , varvec[i] );	
	   NameMappingRegister( varvec[i].getName() , str );
	   varvec[i].setName( str );
	 }
       
       } //Done
       
       //LoadReserveVariableFirstTime( varvec[i] );
     } //Done
	 //infoSink.code << ret_vector[varvec[i].getName()]<<"----------\n";
	 //infoSink.code << varvec[i].getName()<<"\n";


     TllvmVar tv("", type, TllvmVarCode);


     if( !strcmp( fnname.c_str() , "OpenGLES.Normalize" ) ){ 
       TType Newtype( EbtFloat , EvqTemporary, 4 , false , false , EbpHigh);
       const TString &str = allocate_tempvar().c_str();
       TllvmVar t1( str , Newtype );
       const TString &str2 = allocate_tempvar().c_str();
       TllvmVar t2( str2 , Newtype );
       GenCallRegister( t1 , Newtype );
       GenCallRegister( t2 , Newtype );
       TllvmVar temp = varvec.back();
       varvec.pop_back();
       varvec.push_back( t2 );
       varvec.push_back( t1 );
       varvec.push_back( temp );

       string RecordType;
       unsigned int idx = type.getBasicType() | type.getNominalSize() << 5 | type.isMatrix() << 8;
       RecordType = typemang[idx];
       fnname += RecordType;

     }
     else if( !strcmp( fnname.c_str() , "OpenGLES.Pow" ) ){
       const TString &str = allocate_tempvar().c_str();
       TType t2( EbtFloat , EvqTemporary , 4 , 0 , 0 ); // Generate the string "<4 x float>".
       //infoSink.code << t2.getPrecision() <<" "<<t2.getCompleteString()<<"\n";
       fnname = DepolymorphismFunctionName( fnname , varvec );
       TllvmVar t1( str , t2 );
       GenCallRegister( t1 , t2 );
       TllvmVar temp = varvec.back();
       varvec.pop_back();
       TllvmVar temp1 = varvec.back();
       varvec.pop_back();
       varvec.push_back(t1);
       varvec.push_back(temp1);
       varvec.push_back(temp);

     }
     else if( !strcmp( fnname.c_str() , "OpenGLES.FloatDiv" )   ){
       TType Vec4_type(EbtFloat,EvqTemporary,4,false,false,EbpHigh); //Creat vec4 type.
       const TString &str = allocate_tempvar().c_str();
       TllvmVar t1( str , Vec4_type );
       const TString &str2 = allocate_tempvar().c_str();
       TllvmVar t2( str2 , Vec4_type );
       GenCallRegister( t1 , Vec4_type );
       GenCallRegister( t2 , Vec4_type );
       TllvmVar temp = varvec.back();
       varvec.pop_back();
       TllvmVar temp1 = varvec.back();
       /*if( temp.getType() != temp1.getType() ){
	 temp1.setType( temp.getType() );
	 }*/
       if( temp.getCompleteString() == "highp float" ){
	 fnname += '1';
       }
       else{
	 fnname += temp.getType().getCompleteString()[7];
       }
       varvec.pop_back();
       varvec.push_back(t2);
       varvec.push_back(t1);
       varvec.push_back(temp1);
       varvec.push_back(temp);
     }//Done
     else if( !strcmp( fnname.c_str() , "OpenGLES.Sin") ){
	  TType Newtype( EbtFloat , EvqTemporary, 4 , false , false , EbpHigh);
	   const TString &str = allocate_tempvar().c_str();
	   TllvmVar t1( str , Newtype );
	   const TString &str2 = allocate_tempvar().c_str();
	   TllvmVar t2( str2 , Newtype );
	   const TString &str3 = allocate_tempvar().c_str();
	   TllvmVar t3( str3 , Newtype );
	   const TString &str4 = allocate_tempvar().c_str();
	   TllvmVar t4( str4 , Newtype );
	   GenCallRegister( t1 , Newtype );
	   GenCallRegister( t2 , Newtype );
	   GenCallRegister( t3 , Newtype );
	   GenCallRegister( t4 , Newtype );
	   TllvmVar temp = varvec.back();



	   string RecordType;
	   unsigned int idx = type.getBasicType() | type.getNominalSize() << 5 | type.isMatrix() << 8;
	   RecordType = typemang[idx];
	   fnname += RecordType;

	   varvec.pop_back();
	   varvec.push_back( t4 );
	   varvec.push_back( t3 );
	   varvec.push_back( t2 );
	   varvec.push_back( t1 );
	   varvec.push_back( temp );
     }
     else if( !strcmp( fnname.c_str() , "OpenGLES.Cos" ) ){
           TType Newtype( EbtFloat , EvqTemporary, 4 , false , false , EbpHigh);
	   const TString &str = allocate_tempvar().c_str();
	   TllvmVar t1( str , Newtype );
	   const TString &str2 = allocate_tempvar().c_str();
	   TllvmVar t2( str2 , Newtype );
	   const TString &str3 = allocate_tempvar().c_str();
	   TllvmVar t3( str3 , Newtype );
	   const TString &str4 = allocate_tempvar().c_str();
	   TllvmVar t4( str4 , Newtype );
	   const TString &str5 = allocate_tempvar().c_str();
	   TllvmVar t5( str5 , Newtype );
	   GenCallRegister( t1 , Newtype );
	   GenCallRegister( t2 , Newtype );
	   GenCallRegister( t3 , Newtype );
	   GenCallRegister( t4 , Newtype );
	   GenCallRegister( t5 , Newtype );
	   TllvmVar temp = varvec.back();


	   string RecordType;
	   unsigned int idx = type.getBasicType() | type.getNominalSize() << 5 | type.isMatrix() << 8;
	   RecordType = typemang[idx];

	   fnname += RecordType;


	   varvec.pop_back();
	   varvec.push_back( t5 );
	   varvec.push_back( t4 );
	   varvec.push_back( t3 );
	   varvec.push_back( t2 );
	   varvec.push_back( t1 );
	   varvec.push_back( temp );
     }
     else if( !strcmp( fnname.c_str() , "OpenGLES.Sqrt" )   ){
       TType Vec4_type(EbtFloat,EvqTemporary,4,false,false,EbpHigh); //Creat vec4 type.
       const TString &str = allocate_tempvar().c_str();
       TllvmVar t1( str , Vec4_type ); 
       const TString &str2 = allocate_tempvar().c_str();
       TllvmVar t2( str2 , Vec4_type );
       GenCallRegister( t1 , Vec4_type );
       GenCallRegister( t2 , Vec4_type );
       fnname = DepolymorphismFunctionName( fnname , varvec );
       TllvmVar temp = varvec.back();
       varvec.pop_back();
       varvec.push_back(t2);
       varvec.push_back(t1);
       varvec.push_back(temp);
     }//Done
     else if( !strcmp( fnname.c_str() , "OpenGLES.Log" )  || !strcmp( fnname.c_str() , "OpenGLES.Length" ) ){
       TType Vec4_type(EbtFloat,EvqTemporary,4,false,false,EbpHigh); //Creat vec4 type.
       const TString &str = allocate_tempvar().c_str();
       TllvmVar t1( str , Vec4_type ); 
       const TString &str2 = allocate_tempvar().c_str();
       TllvmVar t2( str2 , Vec4_type );
       GenCallRegister( t1 , Vec4_type );
       GenCallRegister( t2 , Vec4_type );
       fnname = DepolymorphismFunctionName( fnname , varvec );
       TllvmVar temp = varvec.back();
       varvec.pop_back();
       varvec.push_back(t2);
       varvec.push_back(t1);
       varvec.push_back(temp);
     }//Done
     else if( !strcmp( fnname.c_str() , "OpenGLES.Distance" ) ){
       TType Vec4_type(EbtFloat,EvqTemporary,4,false,false,EbpHigh); //Creat vec4 type.
       const TString &str = allocate_tempvar().c_str();
       TllvmVar t1( str , Vec4_type ); 
       const TString &str2 = allocate_tempvar().c_str();
       TllvmVar t2( str2 , Vec4_type );
       GenCallRegister( t1 , Vec4_type );
       GenCallRegister( t2 , Vec4_type );
       fnname = DepolymorphismFunctionName( fnname , varvec );
       TllvmVar temp = varvec.back();
       varvec.pop_back();
       TllvmVar temp1 = varvec.back();
       varvec.pop_back();
       varvec.push_back(t2);
       varvec.push_back(t1);
       varvec.push_back(temp1);
       varvec.push_back(temp);
     }//Done
     else if( !strcmp( fnname.c_str() , "OpenGLES.Step" )  ){
       TType Vec4_type(EbtFloat,EvqTemporary,4,false,false,EbpHigh); //Creat vec4 type.
       const TString &str = allocate_tempvar().c_str();
       TllvmVar t1( str , Vec4_type ); 
       GenCallRegister( t1 , Vec4_type );
       fnname = DepolymorphismFunctionName( fnname , varvec );
       TllvmVar temp = varvec.back();
       varvec.pop_back();
       TllvmVar temp1 = varvec.back();
       varvec.pop_back();
       varvec.push_back(t1);
       varvec.push_back(temp1);
       varvec.push_back(temp);
     }//Done
     else if( !strcmp( fnname.c_str() , "OpenGLES.Cross" )  ){
       TType Vec4_type(EbtFloat,EvqTemporary,4,false,false,EbpHigh); //Creat vec4 type.
       const TString &str = allocate_tempvar().c_str();
       TllvmVar t1( str , Vec4_type ); 
       GenCallRegister( t1 , Vec4_type );
       fnname = DepolymorphismFunctionName( fnname , varvec );
       TllvmVar temp = varvec.back();
       varvec.pop_back();
       TllvmVar temp1 = varvec.back();
       varvec.pop_back();
       varvec.push_back(t1);
       varvec.push_back(temp1);
       varvec.push_back(temp);
     }//Done
     else if( !strcmp( fnname.c_str() , "OpenGLES.Fract" )  ){
       TType Vec4_type(EbtFloat,EvqTemporary,4,false,false,EbpHigh); //Creat vec4 type.
       const TString &str = allocate_tempvar().c_str();
       TllvmVar t1( str , Vec4_type ); 
       GenCallRegister( t1 , Vec4_type );
       fnname = DepolymorphismFunctionName( fnname , varvec );
       TllvmVar temp = varvec.back();
       varvec.pop_back();
       varvec.push_back(t1);
       varvec.push_back(temp);
     }//Done
     else
       fnname = DepolymorphismFunctionName( fnname , varvec );

#ifdef NewMatrix     
     //infoSink.code <<"Start\n";
     if( !strcmp( fnname.c_str() , "OpenGLES.MatrixCompMult" ) ){
      
       int size =  varvec[0].getNominalSize();
       //infoSink.code << size << "\n";
       const TString &str = allocate_tempvar().c_str();
       TType Newcurtype(EbtFloat,EvqTemporary,size,false,false,EbpHigh); //Creat vec4 type.
       TllvmVar tempv(str,Newcurtype);
       TllvmVarList newvarvec; 
       tv.setType(Newcurtype);
       SizeofMatrixCompMult = size;      

       for( int j = 0 ; j < size ; j ++ ){
	 for( int k = 0; k < 2; k++ ){
	   infoSink.code << "\t" << tempv.getName()
			 << " = getelementptr inbounds " << varvec[k].getCompleteString() 
			 << "* " <<  varvec[k].getName()
			 <<", i32 0, i32 " << j; 
	   infoSink.code << "\n";

	   const TString &str1 = allocate_tempvar().c_str();
	   TllvmVar tempv2(str1, Newcurtype);
	   newvarvec.push_back(tempv2);
	   GenLoad(tempv2,tempv);
	   if( k != 1 ){
	     const TString &str2 = allocate_tempvar().c_str();
	     tempv.setName( str2 );
	   }

	 }
	 string newfnname;
	 ss.clear();
	 ss << fnname <<"."<<size<<'.'<<j;
	 ss >> newfnname;
	 //point1
	 GenFunc(tv, newfnname, newvarvec); //create function name.
	 newvarvec.clear(); //clear the parameters of function.

	 if( j != size-1 ){
	   Recordparameters.push_back( pop_expstack_rhs() ) ; //last one should stay in var's stack.
	   const TString &str2 = allocate_tempvar().c_str();
	   tempv.setName( str2 );
	 }
       }

     }
     else
       GenFunc(tv, fnname, varvec);
     //cout<<"~~~~~~~~~~~~~~~~~~~~~\n" <<infoSink.code.c_str()<<"~~~~~~~~~~~~~~~~~~" << endl;

     //infoSink.code << (int)Recordparameters.size() <<"\n";
#else
     GenFunc(tv, fnname, varvec);
#endif

}

 bool TllvmOutputTraverser::GenConstructMat(const TType &t, const int line) {

#ifndef NewMatrix
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
#else
     TllvmVar v = pop_expstack_rhs();
     //gggggggggggggggggg
     int SizeofMatrix = 0;
     int i = 0;

     if( line == -999 ){
       SizeofMatrix = t.getNominalSize() * t.getNominalSize(); 
       //infoSink.code << SizeofMatrix <<"\n";

       
       while( SizeofMatrix-1 > i ) {
	 LoadReserveVariableFirstTime( v );
	 RecordParametersforStructure.push_back( v ) ;
	 v = pop_expstack_rhs();
	 i++;
       }
       LoadReserveVariableFirstTime( v );
       RecordParametersforStructure.push_back( v ) ;
       
     }
     else {
       while(!v.isEndConstructMatrix()) { 
	 LoadReserveVariableFirstTime( v );
	 RecordParametersforStructure.push_back( v ) ;
	 v = pop_expstack_rhs();

       }
     }
     //infoSink.code << (int)RecordParametersforStructure.size()<<"\n";
     TllvmVar tempvar( TString("ConstructorMat"),t,TllvmVarCode);
     push_expstack( tempvar );
     
     return true;
#endif
     
 }

 bool TllvmOutputTraverser::GenConstructVec(const TType &t, const int line) {
     string fnname;
     TInfoSinkBase itoa;
     TllvmVarList cmvec;
     int index = 0;
     itoa << t.getNominalSize();
     fnname = "OpenGLES.Construct";
     if(t.getBasicType() == EbtBool)
	 fnname += "B";
     else if(t.getBasicType() == EbtInt)
	 fnname += "I"; 
     fnname += "Vec";
     fnname += itoa.c_str();
     TllvmVar v = pop_expstack_rhs();
     //infoSink.code << v.getName() <<" "<<v.isEndConstructVector() <<"after pop1\n";
     while(!v.isEndConstructVector()) {
	 cmvec.push_back(v);
	 LoadReserveVariableFirstTime( cmvec[index++] );
	 v = pop_expstack_rhs();
	 //infoSink.code << v.getName() <<" "<<v.isEndConstructVector() <<"after pop1\n";
	 //infoSink.code << v.getName() <<"after pop2\n";
     }
     fnname = DepolymorphismFunctionName(fnname, cmvec);
     TllvmVar tv("", t, TllvmVarCode);

     //infoSink.code <<"++++++++++++++++++++" << (int )cmvec.size() <<"\n";
     //infoSink.code <<"----------------------" << cmvec[0].getName() <<"\n";
     //LoadReserveVariableFirstTime( cmvec[0] );

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
		 else if(curtype.isVector()){
		     v = ConstructConstVec(curtype, line);
		 }
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


     //infoSink.code << " src name : " << src.getName() <<" "<<src.getQualifier()<<"\n";
     //infoSink.code << " dst name : " << dst.getName() <<" "<<dst.getQualifier()<<"\n";
     //Modify by Kaymon

     if(dst.getName() == "@gl_FragColor"){
       TllvmVar ts(src.getName(), src.getType()); //New
       LoadReserveVariableFirstTime( ts );
       GenENDFragment_VectorAssign(dst,ts);
       //GenENDFragment_VectorAssign(dst,src); //old
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
       /*
       TllvmVar ts(src.getName(), dst.getType());




       if( DoesNameExist( src.getName() ) )
	 ts.setName( ret_vector[ src.getName() ] );
       else if ( src.getQualifier() == EvqGlobal ){      //Kaymon
	 const TString &str = allocate_tempvar().c_str();
	 ts.setName(str);
	 NameMappingRegister( src.getName() , str );
	 GenLoad(ts,src);
       }
       else if (src.getQualifier() == EvqAttribute ){      //Kaymon
	 const TString &str = allocate_tempvar().c_str();
	 ts.setName(str);
	 NameMappingRegister( src.getName() , str );
	 GenLoadAttribute(ts,src);
       }
       else if (src.getQualifier() == EvqUniform ){      //Kaymon
	 const TString &str = allocate_tempvar().c_str();
	 ts.setName(str);
	 NameMappingRegister( src.getName() , str );
	 GenLoadUniform(ts,src);
       }
       */
	 TllvmVar ts(src);
	 //infoSink.code <<dst.getName()<<"\n";
	 //infoSink.code <<src.getName()<<"\n";
	 LoadReserveVariableFirstTime(ts);
	 //infoSink.code <<dst.getName()<<"\n";
	 //infoSink.code <<src.getName()<<"\n";
       //infoSink.code <<ts.getName()<<"\n";

       const TString &stv = allocate_tempvar().c_str();
       TllvmVar tv(stv , dst.getType());
       tv.setName(stv);
       NameMappingRegister( dst.getName() , stv );
       //store_vars_vector.push( tv );
       //infoSink.code <<"Here i come \n";
       store_vars_vector.push_back( tv );
       //size_t gg = store_vars_vector.size();
       //infoSink.code <<"store_vars_vector: "<< (int) gg <<"\n";
       //infoSink.code <<"src:"<< src.getName()<<"  "<<ret_vector[ src.getName() ] <<"        \n";
       //infoSink.code <<"dst:"<< dst.getName()<<"  "<<ret_vector[ dst.getName() ] <<"        \n";

       //Done 
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

     if( dst.getName() == "@gl_Position" )
       GenENDVertex(tv,dst);

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
#ifndef NewMatrix
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
#else
     //infoSink.code << dst.getName()<<"\n";
     //infoSink.code << src.getName()<<"\n";
     //Compare the src's name to "Matrix". If yes,it means The TllvmVar class contains the virtual register numbers. We don't load it again.
     int SizeofMatrix = src.getNominalSize();
     TType Newcurtype(EbtFloat,EvqTemporary,SizeofMatrix,false,false,EbpHigh); //Creat vec4 type.
     TllvmVar rt( src );
     TllvmVar newdst( dst );
     newdst.setName( allocate_tempvar().c_str() );
	     
     for( int IndexofMatrix = SizeofMatrix-1 ; IndexofMatrix >= 0 ; IndexofMatrix-- ){
       
       if(src.getName()!="Matrix"){
	 rt.setName( newdst.getName() );
 	 infoSink.code << "\t" << rt.getName() << " = " 
		       << "getelementptr inbounds " << src.getCompleteString() << "* "
		       << src.getName() << ", i32 0, i32 " << IndexofMatrix <<"\n";
	 newdst.setName( allocate_tempvar().c_str() );
       }

       infoSink.code << "\t" << newdst.getName() << " = " 
		     << "getelementptr inbounds " << newdst.getCompleteString() << "* "
		     << dst.getName() << ", i32 0, i32 " << IndexofMatrix <<"\n";

       TType Newcurtype(EbtFloat,EvqTemporary,SizeofMatrix,false,false,EbpHigh);
       if(src.getName()!="Matrix"){
	 TllvmVar loadst( allocate_tempvar().c_str(), Newcurtype );
	 rt.setType( Newcurtype );
	 GenLoad(loadst,rt);

	 TllvmVar stored_dst( newdst.getName() , Newcurtype );
	 
	 GenStore( stored_dst , loadst );
       }
       else{

	 TllvmVar stored_dst( newdst.getName() , Newcurtype );
	 TllvmVarList tmp = rt.getVectorsOfMatrix();
	 tmp[SizeofMatrix-IndexofMatrix-1].setType(Newcurtype);
	 GenStore( stored_dst , tmp[SizeofMatrix-IndexofMatrix-1] ); //Matrix save the reverse sqeuence, so be carefully.

       }       
       if( IndexofMatrix != 0 ){
	 //if(src.getName()!="Matrix")
	 //rt.setName( allocate_tempvar().c_str() );
	 newdst.setName( allocate_tempvar().c_str() );
       }
     }
     return true;
#endif
 }

 bool TllvmOutputTraverser::GenStructAssign(const TllvmVar &dst, const TllvmVar &src, const int line) {

   
   //infoSink.code <<dst.getName()<<" src: "<<src.getName() <<"\n";
   //infoSink.code << dst.haveIndex() << "\n";
   //infoSink.code << src.haveIndex() << "\n";
   //infoSink.code << "RecordParametersforStructure.size(): "<<(int)RecordParametersforStructure.size()<<"\n";
   //infoSink.code << "RecordStructureLevel: "<< RecordStructureSize <<"\n";

     if(src.getType() != dst.getType()) {
	 infoSink.info.message(EPrefixInternalError,
			       "Mismatched structure assign",
			       line);
	 return false;
     }
     
     
     //infoSink.code << 	      RecordStructureSize <<" number of structure\n";
     int size = ( int ) RecordParametersforStructure.size();
     int maxsize = ( int ) RecordParametersforStructure.size();     
     
     if( RecordStructureSize > 1 ){ //mutiple structure inside structure.
       //infoSink.code <<dst.getCompleteString() <<" " << src.getCompleteString() <<"\n";
       
       TTypeList* tl = dst.getType().getStruct();
       int size = tl?tl->size():0;	
       TType curtype;
       //infoSink.code <<" Size of dst structer: " <<size <<"\n";
       //infoSink.code <<" is dst structure? " <<dst.isStructure() <<"\n";
              
       for( int index = size-1; index >=0 ; index--){
	 curtype = *((*tl)[index].type);
	 if( curtype.isStructure() ){
	   //infoSink.code << "In the first isStructure() statement.\n";
	   //infoSink.code << curtype.getCompleteString() <<"\n";
	   
	   TllvmVar rt( dst );
	   rt.setName( allocate_tempvar().c_str() );
	   infoSink.code << "\t" << rt.getName() << " = " 
			 << "getelementptr inbounds " << dst.getCompleteString() << "* "
			 << dst.getName() << ", i32 0, i32 " << index <<"\n";
	   
	   //TTypeList* t2 = curtype.getStruct();
	   //int ssize = t2?t2->size():0;
	   //infoSink.code <<"Print the size of next structure: " <<ssize <<"\n";
	   //curtype = 
	   GenStructAssignPart2( rt,curtype ); //load the address of first structure. 
	 }
	 else{
	   if( curtype.isMatrix() ){

	     
	     int SizeofMatrix = curtype.getNominalSize();
	     TType Newcurtype(EbtFloat,EvqTemporary,SizeofMatrix,false,false,EbpHigh); //Creat vec4 type.
	     TllvmVar rt( dst.getName() ,Newcurtype );
	     rt.setName( allocate_tempvar().c_str() );
	     
	     for( int IndexofMatrix = SizeofMatrix-1 ; IndexofMatrix >= 0 ; IndexofMatrix-- ){
	     
	       infoSink.code << "\t" << rt.getName() << " = " 
			     << "getelementptr inbounds " << curtype.getCompleteString() << "* "
			     << dst.getName() << ", i32 0, i32 " << IndexofMatrix <<"\n";

	       
	       GenStore( rt , RecordParametersforStructure[0] );
	       RecordParametersforStructure.erase( RecordParametersforStructure.begin() ); //Remove the register which is already assgined.
	       if( IndexofMatrix  != 0 )
		 rt.setName( allocate_tempvar().c_str() );
	     }

	     
	   }
	   else{
	     TllvmVar rt( dst );
	     rt.setName( allocate_tempvar().c_str() );
	     infoSink.code << "\t" << rt.getName() << " = " 
			   << "getelementptr inbounds " << dst.getCompleteString() << "* "
			   << dst.getName() << ", i32 0, i32 " << index <<"\n";
	   
	     TllvmVar newdst( rt.getName() , RecordParametersforStructure[0].getType() );
	 
      
	     GenStore( newdst , RecordParametersforStructure[0] );
	     RecordParametersforStructure.erase( RecordParametersforStructure.begin() ); //Remove the register which is already assgined.
	     //infoSink.code <<"Terminal node.\n\n";
	   }
	 
	 }
	 
     
       }
     }

     else if( size > 0 ){ //for sabcd s1 = sabcd(1, 2, 4, 8);
       TTypeList* tl = src.getType().getStruct();
       int typesize = tl?tl->size():0;
       TType curtype;
       for( int i = typesize -1 ; i >= 0 ; i -- ){
	 /*//I wrote this code previously.
	 while( size > 0){
	   TllvmVar rt( dst );
	   rt.setName( allocate_tempvar().c_str() );
	   infoSink.code << "\t" << rt.getName() << " = " 
			 << "getelementptr inbounds " << dst.getCompleteString() << "* "
			 << dst.getName() << ", i32 0, i32 " << --size <<"\n";
	   TllvmVar newdst( rt.getName() , RecordParametersforStructure[size].getType() );
	   
	   
	   GenStore( newdst , RecordParametersforStructure[maxsize-size-1] );
	   RecordParametersforStructure.clear();
       }*/
	 curtype = *((*tl)[i].type);
	 if( curtype.isVector() ){
	   TllvmVar rt( dst );
	   rt.setName( allocate_tempvar().c_str() );
	   infoSink.code << "\t" << rt.getName() << " = " 
			 << "getelementptr inbounds " << dst.getCompleteString() << "* "
			 << dst.getName() << ", i32 0, i32 " << i <<"\n";
	   TllvmVar newdst( rt.getName() , RecordParametersforStructure[i].getType() );
	   
	   
	   GenStore( newdst , RecordParametersforStructure[maxsize-i-1] );
	 }
	 else if( curtype.isMatrix() ){
	   
	   int SizeofMatrix = curtype.getNominalSize();
	   
	   
	   TllvmVar rt( dst );
	   rt.setName( allocate_tempvar().c_str() );
	   infoSink.code << "\t" << rt.getName() << " = " 
			 << "getelementptr inbounds " << dst.getCompleteString() << "* "
			 << dst.getName() << ", i32 0, i32 " << i <<"\n";
	   
	   while( SizeofMatrix > 0 ){ 
	     //int index = 0;
	     TllvmVar rt1( rt );
	     rt1.setName( allocate_tempvar().c_str() );
	     infoSink.code << "\t" << rt1.getName() << " = " 
			   << "getelementptr inbounds " << curtype.getCompleteString() << "* "
			   << rt.getName() << ", i32 0, i32 " << --SizeofMatrix <<"\n";

	     TllvmVar newdst( rt1.getName() , RecordParametersforStructure[i].getType() );
	   
	   
	     GenStore( newdst , RecordParametersforStructure[maxsize-SizeofMatrix-1] );
	     /*
	     int SizeofMatrix2 = curtype.getNominalSize();
	     TllvmVar loadst( allocate_tempvar().c_str(), curtype );
	     GenLoad(loadst,rt1); 
	     
	     while( SizeofMatrix2 > 0 ){
	       infoSink.code << "\t" <<loadst.getName() << " = insertelement " << loadst.getCompleteString()
			     << " " << loadst.getName() << ", " << RecordParametersforStructure[index].getCompleteString() 
			     << " " << RecordParametersforStructure[index++].getName() << ", i32 "<< --SizeofMatrix2 <<"\n";
			     }*/
	     
	   }
	   
	 }
	 else{//sabcd s = sabcd(1.0, 2.0, 4.0, 8.0);
	   int SizeofMatrix = curtype.getNominalSize();
	   TllvmVar rt( dst );
	   rt.setName( allocate_tempvar().c_str() );
	   infoSink.code << "\t" << rt.getName() << " = " 
			 << "getelementptr inbounds " << dst.getCompleteString() << "* "
			 << dst.getName() << ", i32 0, i32 " << i <<"\n";
	   
	   TllvmVar newdst( rt.getName() , RecordParametersforStructure[i].getType() );
	   
#ifdef deal_store_instr
	   const TString &str1 = allocate_tempvar().c_str();
	   TllvmVar var1(str1,newdst.getType());
	   GenLoad(var1,newdst);
	   const TString &str2 = allocate_tempvar().c_str();
	   TllvmVar var2(str2,newdst.getType());
	   const TString &str3 = allocate_tempvar().c_str();
	   TllvmVar var3(str3,newdst.getType());
	   if(newdst.getCompleteString() == "highp i1"||newdst.getCompleteString() == "highp i32"){
	     infoSink.code << "\t" << var2.getName()<< "= mul " << newdst.getCompleteString() <<" "
			   << str1 <<", " << "0"<<"\n";
	     infoSink.code << "\t"<<var3.getName() << "= add " << newdst.getCompleteString() <<" "
			   << var2.getName()  <<", " << RecordParametersforStructure[maxsize-i-1].getName()<<"\n";
	   }
	   else{
	     infoSink.code << "\t" << var2.getName()<< "= fmul " << newdst.getCompleteString() <<" "
			   << str1 <<", " << "0x0000000000000000"<<"\n";
	     infoSink.code << "\t"<<var3.getName() << "= fadd " << newdst.getCompleteString() <<" "
			   << var2.getName()  <<", " << RecordParametersforStructure[maxsize-i-1].getName()<<"\n";
	   }
	   newdst.setName(str3);
	   rt.setType(newdst.getType());
	   GenStore( rt , var3);
#else
	   GenStore( newdst , RecordParametersforStructure[maxsize-i-1]);
#endif	   

	 }
       }
       
       RecordParametersforStructure.clear();
     }
     else{
       TTypeList* tl = src.getType().getStruct();
       int typesize = tl?tl->size():0, i;
       //TllvmVarList variables(size);
       TType curtype;
       //TllvmVar v;
       i = 0;

       while( typesize >  i ){ //for s2 = s1;
	 
	 curtype = *((*tl)[i].type);

	 if( curtype.isMatrix() ){

	   TllvmVar rt( dst.getName() ,curtype );
	   rt.setName( allocate_tempvar().c_str() );
	   infoSink.code << "\t" << rt.getName() << " = " 
			 << "getelementptr inbounds " << dst.getCompleteString() << "* "
			 << dst.getName() << ", i32 0, i32 " << i  <<"\n";
	 
	   TllvmVar st( src.getName() ,curtype);
	   st.setName( allocate_tempvar().c_str() );
	   infoSink.code << "\t" << st.getName() << " = " 
			 << "getelementptr inbounds " << src.getCompleteString() << "* "
			 << src.getName() << ", i32 0, i32 " << i++ <<"\n";

	   int SizeofMatrix = curtype.getNominalSize();
	   TllvmVar PtrofVectorAssigned( rt.getName() ,curtype );
	   TllvmVar GetPtrofVectorAssigning( st.getName() ,curtype);
	   
	   for( int index = 0 ; index < SizeofMatrix ; index ++ ){


	     PtrofVectorAssigned.setName( allocate_tempvar().c_str() );
	     infoSink.code << "\t" << PtrofVectorAssigned.getName() << " = " 
			   << "getelementptr inbounds " << curtype.getCompleteString() << "* "
			   << rt.getName() << ", i32 0, i32 " << index  <<"\n";
	 

	     GetPtrofVectorAssigning.setName( allocate_tempvar().c_str() );
	     infoSink.code << "\t" << GetPtrofVectorAssigning.getName() << " = " 
			   << "getelementptr inbounds " << curtype.getCompleteString() << "* "
			   << st.getName() << ", i32 0, i32 " << index <<"\n";
	     //rt.setName( PtrofVectorAssigned.getName() );
	     //st.setName( GetPtrofVectorAssigning.getName() );
	     TType Newcurtype(EbtFloat,EvqTemporary,SizeofMatrix,false,false,EbpHigh);
	     TllvmVar loadst( allocate_tempvar().c_str(), Newcurtype );
	     GetPtrofVectorAssigning.setType( Newcurtype );
	     GenLoad(loadst,GetPtrofVectorAssigning);
	     TllvmVar newdst( PtrofVectorAssigned.getName() , Newcurtype );
	     
	     GenStore( newdst , loadst );
	     
	   }

	 }
	 else{
	   TllvmVar rt( dst.getName() ,curtype );
	   rt.setName( allocate_tempvar().c_str() );
	   infoSink.code << "\t" << rt.getName() << " = " 
			 << "getelementptr inbounds " << dst.getCompleteString() << "* "
			 << dst.getName() << ", i32 0, i32 " << i  <<"\n";
	 
	   TllvmVar st( src.getName() ,curtype);
	   st.setName( allocate_tempvar().c_str() );
	   infoSink.code << "\t" << st.getName() << " = " 
			 << "getelementptr inbounds " << src.getCompleteString() << "* "
			 << src.getName() << ", i32 0, i32 " << i++ <<"\n";
	   TllvmVar loadst( allocate_tempvar().c_str(), curtype );
	   GenLoad(loadst,st);
	   TllvmVar newdst( rt.getName() , curtype );

	   GenStore( newdst , loadst );
	 }
       }
     }
     
     return true;
 }
bool TllvmOutputTraverser::GenStructAssignPart2(const TllvmVar & dst, const TType &t) {
  //infoSink.code <<"\n\n\nIn GenStructAssignPart2 function\n";
  //infoSink.code <<t.getCompleteString() <<"\n";
       
  TTypeList* tl = t.getStruct();
  int size = tl?tl->size():0;	
  TType curtype;
  //infoSink.code <<" Size of Structure: " <<size <<"\n";
  //infoSink.code <<" Is type structure? "<< t.isStructure() <<"\n";
              
  for( int index = size-1; index >=0 ; index--){
    curtype = *((*tl)[index].type);
    //infoSink.code << curtype.getCompleteString() <<" \n";
    if( curtype.isStructure() ){
      //infoSink.code << " In isStructure statment \n";
      //infoSink.code << curtype.getCompleteString() <<"\n";
      
      TllvmVar rt( dst );
      rt.setName( allocate_tempvar().c_str() );
      infoSink.code << "\t" << rt.getName() << " = " 
		    << "getelementptr inbounds " << t.getCompleteString() << "* "
		    << dst.getName() << ", i32 0, i32 " << index <<"\n";
	   
      //TTypeList* t2 = curtype.getStruct();
      //int ssize = t2?t2->size():0;
      //infoSink.code <<" Print the size of next structure: " <<ssize <<"\n";

      GenStructAssignPart2( rt , curtype );
    }
    else{

      if( curtype.isMatrix() ){


	int SizeofMatrix = curtype.getNominalSize();
	TType Newcurtype(EbtFloat,EvqTemporary,SizeofMatrix,false,false,EbpHigh); //Creat vec4 type.
	TllvmVar rt( dst.getName() ,Newcurtype );
	rt.setName( allocate_tempvar().c_str() );
	TllvmVar st( dst.getName() ,Newcurtype );
	st.setName( allocate_tempvar().c_str() );
	
	infoSink.code << "\t" << rt.getName() << " = " 
		      << "getelementptr inbounds " << t.getCompleteString() << "* "
		      << dst.getName() << ", i32 0, i32 " << index <<"\n";

	for( int IndexofMatrix = SizeofMatrix-1 ; IndexofMatrix >= 0 ; IndexofMatrix-- ){
	     
	  infoSink.code << "\t" << st.getName() << " = " 
			<< "getelementptr inbounds " << curtype.getCompleteString() << "* "
			<< rt.getName() << ", i32 0, i32 " << IndexofMatrix <<"\n";

		  
	  GenStore( st , RecordParametersforStructure[0] );
	  RecordParametersforStructure.erase( RecordParametersforStructure.begin() ); //Remove the register which is already assgined.
	  if( IndexofMatrix  != 0 )
	    st.setName( allocate_tempvar().c_str() );
	}


      }
      else{
	TllvmVar rt( dst );
	rt.setName( allocate_tempvar().c_str() );
	infoSink.code << "\t" << rt.getName() << " = " 
		      << "getelementptr inbounds " << t.getCompleteString() << "* "
		      << dst.getName() << ", i32 0, i32 " << index <<"\n";
	
	TllvmVar newdst( rt.getName() , RecordParametersforStructure[0].getType() );
	
	
	GenStore( newdst , RecordParametersforStructure[0] );
	RecordParametersforStructure.erase( RecordParametersforStructure.begin() ); //Remove the register which is already assgined.
	//infoSink.code <<" Terminal node.\n\n";
	
      }
    }
	 
  }
  return true;
}
 TllvmVar TllvmOutputTraverser::ConstructConstVec(const TType &t, const int line) {
     int i;
     int size = t.getNominalSize();
     //infoSink.code <<size<<" Come here\n";
     if( FloattoVec )
       FloattoVec = 0;
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
#ifdef trick_for_bool
	 infoSink.code << "\tbr i1" 
		       << " " << cond.getName() << ", label %" << label.c_str()
		       << ".B, label %" << label.c_str() << ".E\n";
#else
	 infoSink.code << "\tbr " << cond.getCompleteString().c_str()
		       << " " << cond.getName() << ", label %" << label.c_str()
		       << ".B, label %" << label.c_str() << ".E\n";
#endif
     }
 }

 bool TllvmOutputTraverser::GenPrePostIncDec(const TOperator &op, const int line)
 {
   //infoSink.code<<"GenPrePostIncDec\n";
     TllvmVar v = pop_expstack();
     TllvmVar rhs;
     string opstr = (op == EOpPostIncrement ||
		     op == EOpPreIncrement)?
		     "add":"sub";
     //bool ispost = op == EOpPostIncrement ||  //original code.
     //		   op == EOpPreIncrement;
     bool ispost = op == EOpPostIncrement ||
		   op == EOpPostDecrement;

     if(v.isNormalType())
	 rhs = v;
     else
	 rhs = GenGetRHS(v, line);

     if(rhs.isSimpleType()) {
	 if(ispost) {
	     /* Allocate a temporary variable for return  //Original code.
	     TllvmVar tv(allocate_tempvar().c_str(), rhs.getType());
	     push_expstack(tv);
	     push_expstack(rhs);

	     GenAssign(tv.getType(), line);

	     //Save the added or subtracted result 
	     if(v.isSimpleVar()) {

		 GenBinOp(opstr, v, rhs, TllvmVar(1));

		 }*/
	     /* Allocate a temporary variable for return */ //original code.
	     //TllvmVar tv(allocate_tempvar().c_str(), rhs.getType());
	     //push_expstack(rhs); // Reverse the putting order. Compare with the original code. I mean the rhs is the dst vector,but I put it in the position of src vector.
	     //push_expstack(tv); 

	     //GenAssign(tv.getType(), line); //Disable this function.****************note here****************
	   //infoSink.code<<ispost<<"\n";
	     /* Save the added or subtracted result */
	     if(v.isSimpleVar()) {
	       /*
	       store_vars.push_back( rhs ); //Because I disable the genassign function, I should save the vector and the name. Record the mapping table.
	       LoadReserveVariableFirstTime( rhs );
	       //GenAssign(rhs.getType(), line);
	       TllvmVar tv(allocate_tempvar().c_str(), rhs.getType());
	       infoSink.code << "v: "<<v.getName() <<" rhs: "<<rhs.getName() <<" tv: "<< tv.getName() <<"\n";
	       GenBinOp(opstr, tv, rhs, TllvmVar(1)); // tv : destination's vector . rhs : source's vector.
	       store_vars_vector.push_back( tv );//Because I disable the genassign function, I should save the vector and the name.
	       NameMappingRegister( v.getName() , tv.getName() );
	       push_expstack(tv);
	       */
	       //store_vars.push_back( rhs ); //Because I disable the genassign function, I should save the vector and the name. Record the mapping table.
	       LoadReserveVariableFirstTime( rhs );
	       push_expstack(rhs);//Push the old value to stack. e.g. int k = i ++; k is assigned by old value of i.	       
	       //GenAssign(rhs.getType(), line);
	       TllvmVar tv(allocate_tempvar().c_str(), rhs.getType());
	       //infoSink.code << "v: "<<v.getName() <<" rhs: "<<rhs.getName() <<" tv: "<< tv.getName() <<"\n";
	       GenBinOp(opstr, tv, rhs, TllvmVar(1)); // tv : destination's vector . rhs : source's vector.
	       GenStore(v,tv);
	       //store_vars_vector.push_back( tv );//Because I disable the genassign function, I should save the vector and the name.
	       //NameMappingRegister( v.getName() , tv.getName() );
	       //push_expstack(v);//push original variable and it will be assigned by tv. but numbers of stack is more than before. more one. Note this.
	       //push_expstack(tv);
	     }
	     else {
		 /* Allocate a temporary variable for save it back */
		 TllvmVar tv2(allocate_tempvar().c_str(), rhs.getType());

		 GenBinOp(opstr, tv2, rhs, TllvmVar(1));
		 push_expstack(v);
		 push_expstack(tv2);

		 GenAssign(v.getType(), line);
		 push_expstack(tv2);
	     }
	     //push_expstack(tv);
	     
	 }
	 else {
	   //infoSink.code<<ispost<<"\n";
	     if(v.isSimpleVar()) {
	       //store_vars.push_back( rhs ); //Because I disable the genassign function, I should save the vector and the name. Record the mapping table.
	       LoadReserveVariableFirstTime( rhs );
	       //GenAssign(rhs.getType(), line);
	       TllvmVar tv(allocate_tempvar().c_str(), rhs.getType());
	       //infoSink.code << "v: "<<v.getName() <<" rhs: "<<rhs.getName() <<" tv: "<< tv.getName() <<"\n";
	       GenBinOp(opstr, tv, rhs, TllvmVar(1)); // tv : destination's vector . rhs : source's vector.
	       GenStore(v,tv);
	       //store_vars_vector.push_back( tv );//Because I disable the genassign function, I should save the vector and the name.
	       //NameMappingRegister( v.getName() , tv.getName() );
	       push_expstack(tv);
	       
	     }
	     else {
		 /* Allocate a temporary variable for save it back */
		 TllvmVar tv(allocate_tempvar().c_str(), rhs.getType());

		 GenBinOp(opstr, tv, rhs, TllvmVar(1));
		 push_expstack(v);
		 push_expstack(tv);

		 GenAssign(v.getType(), line);
	     }

	     //push_expstack(v);
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
     //infoSink.code << v.getName() <<"\n";
     LoadReserveVariableFirstTime( v );
     //infoSink.code << v.getName() <<"\n";
#ifndef new_convert
     code << convopstr[op]
	  << " " << v.getCompleteString()
	  << " " << v.getName() << " to "
	  << type.getCompleteString() << "\n";
#else
     code << "call " << type.getCompleteString() <<" @OpenGLES.Convert."
	  << convopstr[op] << "(" << v.getCompleteString() <<" " << v.getName()
	  << ")\n";
#endif
     TllvmVar tv(code.c_str(), type, TllvmVarCode);
     push_expstack(tv);
 }

 TllvmOutputTraverser::TllvmOutputTraverser(TInfoSink& i) : infoSink(i) {
     
     RecordStructureSize = 1;  //defaut is 1; At least we have one structure. 
     SizeofMatrixCompMult = 0;  
     tmp_register_counter="unused";
     CompareMatrix = 0;
     BreakorContinue = 0;
     lockfnname = 0;
     ReturnVarInTrueBlock = 0;
     ReturnVarInFalseBlock = 0;
     FlagForGenstore = 1;
     IsGlobalOutsideFunction = 1;
     ReturnFlag = 0;
     AfterGenCompareInstruction =0;

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
#ifdef PUSH
       infoSink.code << "Pop but empty: "  <<"\n";
       //cout<< "empty:\n";
#else
#endif
	 var.setName("");
	 var.setType(TType(EbtVoid));
     }
     else {

	 var = expstack.top();
#ifdef PUSH
	 infoSink.code << "Pop: " << var.getName() <<"\n";
	 //cout<< var.getName() << "\n";
#else
#endif
	 expstack.pop();

     }
     return var;
 }

 const TllvmVar& TllvmOutputTraverser::pop_expstack_rhs(void) {
     var = GenGetRHS(pop_expstack(), -1);
     return var;
 }

 void TllvmOutputTraverser::push_expstack(const TllvmVar &v) {
 #ifdef PUSH
   infoSink.code << "Push:" << v.getName() <<"\n";
 #else
 #endif
     expstack.push(v);
 }

 void TllvmOutputTraverser::into_function(const TllvmVar &func) {
     func_scope.push(func);
 }
 //dondum
void TllvmOutputTraverser::outof_function(void) { // if out of this function, the beginning register number should be  zero. Kaymon
     func_scope.pop();
     for(n=tempvar_nr-(int)tempvars.size()-1;n>=0;--n) { 
	 ss.clear();
	 ss << "%" << n;
	 ss >> str;
	 tempvars.push(str);
     }
     //infoSink.code<< (int)tempvars.size() << "\n";
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
     //infoSink.code << fnname <<"\n";
     if((pos = fnname.find("(")) == fnname.size() - 1)
       fnname.replace(pos, 5, ":void");
     else
	 fnname.replace(pos, 1, ":");
     if( lockfnname == 0 )
       Recordfnname = fnname; //Record the function name;
     IsGlobalOutsideFunction = 0;
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
     //oit->infoSink.code << "come into symbol\n";
     if(oit->is_parameters()) {   //Print the parameter of user defining function.
	 if(!oit->is_first_parameter())
	     oit->infoSink.code << ", ";
	 
	 //oit->infoSink.code << node->getType().getQualifier() <<"\n";
	 if( node->getType().getQualifier() == EvqInOut || node->getType().getQualifier() == EvqOut){ //For qualifier attribute "inout" and "out".
	   oit->infoSink.code << node->getCompleteString()
			      << " %" << node->getSymbol();
	   TllvmVar parameter("%"+node->getSymbol(),node->getType()); //Add a star sign.
	   oit->Recordparameters.push_back(parameter);
	   //oit->infoSink.code << (int)oit->Recordparameters.size()<<"\n";
	 }
	 else{ //for default or in.( Qualifier attribute )
	   oit->infoSink.code << node->getCompleteString()
			      << " %" << node->getSymbol();

	   TllvmVar parameter("%"+node->getSymbol(),node->getType());
	   oit->Recordparameters.push_back(parameter);
	   //oit->infoSink.code << (int)oit->Recordparameters.size()<<"\n";
	 }


     }
     else if(oit->is_declare() && !oit->is_initializer()) { //Print the global variables. Except for initailize the vec4.

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
//	 oit->infoSink.code << prefix << node->getSymbol()
//			    << " = " << node->getType().getCompleteString(true)
//			    << "\n";
	 oit->infoSink.code << prefix << node->getSymbol()
			    << " = alloca " << node->getType().getCompleteString(true)
			    << "\n";
 #else
	 if(node->getType().getQualifier() == EvqTemporary || node->getType().getQualifier() == EvqConst )
	   return;
	 //dondum
	 //if(node->getType().getQualifier() == EvqGlobal || node->getType().)
	 oit->infoSink.code << "@" << node->getSymbol() //print gloabal variables. e.g. uniform,attribute,etc.
			    << " = " << node->getType().getCompleteString(true)
			    << "\n";
#ifdef getSamplerIndex
	 if( node->getType().getQualifier() == EvqUniform && node->getType().getCompleteString() == "highp sampler2D"){
	   std::fstream outfile;
	   outfile.open("sampler_id_of_uniform",std::fstream::out | std::fstream::app);
	   TString temp = node->getSymbol();
	   temp+='\n';
	   outfile.write(temp.c_str(),(int)strlen(temp.c_str()));
	   
	   }
#endif

 #endif

     }
     else {//if the var is temporary or EvqIn (function parameter),prefix is %. and the variable's name should be saved with another name. Append the _addr to original name.
       //oit->infoSink.code << node->getType().getQualifier()<<" "<<EvqIn<<" "<<node->getSymbol()<<"\n";
       /*
	 oit->push_expstack(
	    //dondum
	    //For llvm, every variable in function is "%" 
	    //TllvmVar(TString((node->getType().getQualifier() == EvqTemporary)?"%":"@") + node->getSymbol(),
	    TllvmVar(TString(((node->getType().getQualifier() == EvqTemporary)||(node->getType().getQualifier() == EvqIn ))?"%":"@") + node->getSymbol()+TString((node->getType().getQualifier() == EvqIn)?"_addr":""),
		      node->getType())
	 ); 
       */
#ifdef parameter_with_addr_inside_function
	 TString prefix;
	 TString postfix;
	 TType TmpType( node->getType() );
	 if( node->getType().getQualifier() == EvqTemporary || node->getType().getQualifier() == EvqIn || TmpType.getQualifier() == EvqInOut || TmpType.getQualifier() == EvqOut )
	   prefix = "%";
	 else
	   prefix = "@";

	 if( node->getType().getQualifier() == EvqIn  || TmpType.getQualifier() == EvqInOut || TmpType.getQualifier() == EvqOut ){
	   postfix = "_addr ";
	   TllvmVar NewVar( TString( prefix + node->getSymbol() + postfix ) , node->getType() );
	   oit->DeclareVariable( NewVar );
	 }
	 else
	   postfix = "";
	 
	 oit->push_expstack( TllvmVar( TString( prefix + node->getSymbol() + postfix ) , node->getType() ) );
#else
	 //oit->push_expstack( TllvmVar( TString( ((node->getType().getQualifier() == EvqTemporary)?"%":"@")+ node->getSymbol()  ) , node->getType() ) );
	 if( node->getType().getQualifier() == EvqIn) //EvqIn parameter should save to another variable.
	   oit->push_expstack( TllvmVar( TString( oit->PrefixofVariable( node->getType().getQualifier() )+ node->getSymbol() +"_addr" ) , node->getType() ) );
	 else
	   oit->push_expstack( TllvmVar( TString( oit->PrefixofVariable( node->getType().getQualifier() )+ node->getSymbol()  ) , node->getType() ) );
#endif
     }

 }
char TllvmOutputTraverser::PrefixofVariable(TQualifier t){
#ifndef OpenGLES
  char a[24]="%@ @@@@@@@@%%% @@@@@@@@";
#else
  char a[22]="%@ @@@@@@@@%%% @@@@@@";
#endif
  return a[t];
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
		 if(node->getType().isStructure()){
		   //oit->infoSink.code<<"entry1\n";
		   return oit->GenAssign(node->getLeft()->getType(), node->getLine());
		 }
		 else{
		   //oit->infoSink.code<<"entry\n";
		   //oit->infoSink.code<<node->getLeft()->getCompleteString()<<"\n";
		   //oit->infoSink.code<<node->getRight()->getCompleteString()<<"\n";
		   //oit->infoSink.code<<node->getCompleteString()<<"\n";
		   return oit->GenAssign(node->getType(), node->getLine());
		 }
	     //dondum
	     case EOpVectorTimesMatrixAssign:
		 return oit->GenBinFuncAssign("OpenGLES.VectorTimesMatrix", node->getLine());
	     case EOpVectorTimesScalarAssign:
		 return oit->GenBinFuncAssign("OpenGLES.ScalarTimesVector", node->getLine()); //Kaymon
		 //return oit->GenBinFuncAssign("OpenGLES.VectorTimesScalar", node->getLine());
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
		 // return oit->GenArithBin(node); // Goda 2011-03-23
		 return oit->GenArithBin_2(node, 2);

	     case EOpEqual:
	     case EOpNotEqual:
	     case EOpLessThan:
	     case EOpGreaterThan:
	     case EOpLessThanEqual:
	     case EOpGreaterThanEqual:
		 return oit->GenCompare(node);

	     case EOpVectorTimesScalar:
		 return oit->GenBinFuncOp("OpenGLES.ScalarTimesVector", node->getType(), node->getLine()); //Kaymon
		 //return oit->GenBinFuncOp("OpenGLES.VectorTimesScalar", node->getType(), node->getLine());


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
		     //oit->infoSink.code << v.getName() <<" "<< v.getType().getCompleteString() <<" "<<v.isVector()<<"\n";
		     //oit->infoSink.code << v.getType().getBasicType() <<"\n";
		     if(v.isSimpleType()){
		       oit->LoadReserveVariableFirstTime(v);//Check wheather the variable is loaded or not. 

			     TllvmVar tv(oit->allocate_tempvar().c_str(),
					 TType(v.getBasicType()));
			     if( v.getType().getBasicType() == EbtInt )  //Identify the type which is integer,
			       oit->GenBinOp("sub", tv, TllvmVar(0), v);
			     else if( v.getType().getBasicType() == EbtFloat ) 
			       oit->GenBinOp("fsub", tv, TllvmVar( (float)0.0 ), v);

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
		     
		     if(v.isSimpleType() && v.getBasicType() == EbtBool) {//for !boovariable
		         oit->LoadReserveVariableFirstTime(v);//Load variables first.
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
	 case EOpPreDecrement:{
	   //std::cout<<"1234\n";
		 return oit->GenPrePostIncDec(node->getOp(), node->getLine());
	 }
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
	       //oit->infoSink.code <<"print normalize\n";
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
     //out.code <<"OutputAgg start\n";
     if(preVisit) {
	 if (node->getOp() == EOpNull) {
	     out.code.message(EPrefixError, "node is still EOpNull!");
	     return true;
	 }

	 switch (node->getOp()) {
	     case EOpSequence:
		 return true;

	     case EOpDeclaration:
		 oit->into_declare(); //set the flag of the global variable.
		 return true;

	    case EOpFunction:
		{
		  //out.code << "===================function start======================\n";
		    string fnname = oit->ConvertMangledFunctionName(node->getName().c_str());
		    out.code << "\ndefine " << node->getCompleteString() << " @" << fnname;//add one more space
		    TllvmVar tv(fnname.c_str(), node->getType());
		    oit->into_function(tv);
		    return true;
		}

	    case EOpParameters:
	        out.code << "("; //Print the left brace of parameters.
		oit->lockfnname = 1; //lock fnname,therefore you can't assign new fnname. Kaymon
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
     else {//out.code << node->getOp() <<" else\n";
      switch (node->getOp()) { 
	    case EOpDeclaration:
		oit->outof_declare();
		return true;

	    case EOpFunctionCall:
		{
		  //Function call( not built-in ).
		    string fnname = oit->ConvertMangledFunctionName(node->getName().c_str());
		    //oit->infoSink.code <<fnname<<"\n";//Textrue2D is generated here if you want to modify its name.

		    TllvmVarList arglist;
		    string::size_type pos = 0;
		    //out.code << "another place\n";
		    //int i = 0;
		    while((pos = fnname.find_first_of(";", pos)) != string::npos) {
		      arglist.push_back(oit->pop_expstack_rhs());//Qualifier will disapear.
			//arglist[i].changeQualifier( TQualifier(node->getAsAggregate()->getQualifier()[i++]) ); //can not modify the qualifier.
			++pos;
		    }
		    //out.code << "q:"<<node->getAsAggregate()->getQualifier()[0] <<"\n"; //get the qualifier of function. e.g. EvqInOut,EvqOut and EvqIn
		    //Modify by Kaymon
		    //size_t sizeoffnname = fnname.size(); //Delete the ";" character. The last char of fnname string.
		    //fnname[sizeoffnname-1] = ' ';
		    int size = arglist.size();
		    //out.code << size <<" hi\n";
		    //out.code << "name: " << arglist[0].getCompleteString()<<"\n";
		    while( size > 0 ){
		      
		      /*const TString &str = oit->allocate_tempvar().c_str();
		      TllvmVar ts( str , arglist[size-1].getType());
		      out.code << arglist[size-1].getQualifier() <<"\n";
		      if( arglist[size-1].getQualifier() == EvqUniform ){
			oit->GenLoadUniform( ts , arglist[size-1] );
			arglist[size-1].setName( str );
		      }
		      else if( arglist[size-1].getQualifier() == EvqVaryingIn ){
			oit->GenLoadVarying( ts , arglist[size-1] );	
			arglist[size-1].setName( str );
			}*/
		      //out.code << arglist[size-1].getQualifier() <<"before coming\n";
		      //Because qualifier of inout or out  pass its address into the argument list. 
		      if( !(node->getAsAggregate()->getQualifier()[size-1] == EvqInOut || node->getAsAggregate()->getQualifier()[size-1] == EvqOut) ){
			oit->LoadReserveVariableFirstTime( arglist[size-1] );
		      }
		      else{
			//out.code << node->getAsAggregate()->getQualifier()[size-1] <<" changed qualifier()\n";
			arglist[size-1].changeQualifier( TQualifier(node->getAsAggregate()->getQualifier()[size-1]) ); 
			//Store the matched variable first before pass its address. //mmmmmmmmmmark here
			size_t size2 = oit->store_vars.size(); 
#ifdef DEBUG_FOR_STORED_VARIABLE
			//out.code <<"store_vars size:"<<(int) size <<"\n";
			//size = oit->store_vars_vector.size();
			//out.code <<"store_vars_vector "<<(int) size <<"\n";
			//**************************List all the store instruction*************************
			for(int k =0; k < (int)oit->store_vars.size() ;k++)
			  out.code <<"if  vars:"<<(int)oit->store_vars.size()<<" "<<oit->store_vars[k].getName() << "\n";
			if(oit->store_vars.size() != oit->store_vars_vector.size()) //Generate the store instruction in  the label.T block. Store the the numbers of vars to memory. 
			  out.code << "Store_vars's size is not equal to store_vars_vector\n";
			for( int f = 0; f< (int)oit->store_vars_vector.size();f++)
			  out.code <<"if2 vectors:"<<(int) oit->store_vars_vector.size()<<" "<< oit->store_vars_vector[f].getName() <<"\n";
#else
#endif    
			for( size_t i = size2 ; i >0 ; i-- ){  //Delete the arguments from the record map.
			  if( oit->store_vars[i-1].getName() == arglist[size-1].getName() ){
			    oit->GenStore( oit->store_vars[i-1] , oit->store_vars_vector[i-1] ); 
			    oit->it = oit->ret_vector.find(arglist[size-1].getName());
			    oit->ret_vector.erase(oit->it);
			    oit->store_vars.erase(oit->store_vars.begin()+i-1);
			    oit->store_vars_vector.erase(oit->store_vars_vector.begin()+i-1);
			  }
			}

		      }
		      size--;
		    }//Done
		    
		    TllvmVar tv("", node->getType(), TllvmVarCode);
		    oit->GenFunc(tv, fnname, arglist);
		    return true;
		}

	    case EOpFunction:
	      //dondum - function end 
	      //Information of node is empty. Here is just print "}" in IR.
	      //out.code << oit->Recordfnname <<"\n";
	      if( oit->Recordfnname == "main:void"){ //specially deal for metadata and return variable.
		out.code << "main function end\n}\nadd_metadata\n\n";
		//delete the saved variables.
		oit->ret_vector.clear();
		oit->store_vars.clear();
		oit->store_vars_vector.clear();
	      }
	      else{
		if(!oit->ReturnFlag){
		  for( size_t i = oit->Recordparameters.size() ; i >0 ; i-- ){
		    TllvmVar temp = oit->Recordparameters[i-1];
		    if(oit->DoesNameExist(oit->Recordparameters[i-1].getName()))
		      temp.setName(oit->ret_vector[oit->Recordparameters[i-1].getName()]);
		    else
		      temp.setName("Name does not exsist!!!!!!!!");
		    oit->GenStoreForInOut( oit->Recordparameters[i-1] , temp ); 
		  }
		  out.code<<"\tret void\n";
		}
	   	out.code << "}\n\n"; //temporary add

	      }


#ifdef DEBUG_FOR_STORED_VARIABLE
	      //out.code <<"store_vars size:"<<(int) size <<"\n";
	      //size = oit->store_vars_vector.size();
	      //out.code <<"store_vars_vector "<<(int) size <<"\n";
	      //**************************List all the store instruction*************************
	      for(int k =0; k < (int)oit->store_vars.size() ;k++)
		out.code <<"if  vars:"<<(int)oit->store_vars.size()<<" "<<oit->store_vars[k].getName() << "\n";
	      if(oit->store_vars.size() != oit->store_vars_vector.size()) 
		out.code << "Store_vars's size is not equal to store_vars_vector\n";
	      for( int f = 0; f< (int)oit->store_vars_vector.size();f++)
		out.code <<"if2 vectors:"<<(int) oit->store_vars_vector.size()<<" "<< oit->store_vars_vector[f].getName() <<"\n";
#else
#endif	      
		oit->lockfnname = 0;              //unlock the fnname because exit the function.
		oit->IsGlobalOutsideFunction = 1; //Set 1 to this variable because exit the function.
		oit->ret_vector.clear();          //Exit the function. Clear the record of this.
		oit->ret_name.clear();          //Exit the function. Clear the record of this.
		oit->store_vars.clear();          //Exit the function. Clear the record of this.
		oit->store_vars_vector.clear();   //Exit the function. Clear the record of this.
		oit->Recordparameters.clear();    //Exit the function. Clear the record of this.
		oit->outof_function();
		return true;

	    case EOpParameters: 
	      //parameters's right brace
		oit->outof_parameters();
		if( oit->Recordfnname == "main:void")
		  out.code << ") {\n";
		else
		  out.code << ") {\nentry:\n";
#ifdef parameter_with_addr_inside_function		
		for(int i = 0; i < (int)oit->Recordparameters.size() ; i++){
		  TllvmVar appendpostfix(oit->Recordparameters[i].getName()+"_addr",oit->Recordparameters[i].getType());
		  if( oit->Recordparameters[i].getQualifier() == EvqInOut || oit->Recordparameters[i].getQualifier() == EvqOut ){
		    oit->FlagForGenstore = 0;
		  }
		  oit->GenStore( appendpostfix , oit->Recordparameters[i] );
		}
		oit->Recordparameters.clear();
		oit->FlagForGenstore = 1; //default value.
#else
		for(int i=0;i<(int)oit->Recordparameters.size();i++){ //only for EvqIn
		  if( oit->Recordparameters[i].getQualifier() == EvqIn ){
		    TllvmVar appendpostfix(oit->Recordparameters[i].getName()+"_addr",oit->Recordparameters[i].getType());
		    oit->GenStore( appendpostfix , oit->Recordparameters[i] );
		  }
		}
		for(int i = 0; i < (int)oit->Recordparameters.size() ; i++){
		  if(oit->Recordparameters[i].getQualifier() == EvqIn ) //Allow to declare EvqIn variables.
		    ;
		  else
		    oit->ret_name[oit->Recordparameters[i].getName()]="Yes"; //Avoid to declare parameter of function.
		}
		//oit->Recordparameters.clear(); //Clear when exit this function. Not beginning.
#endif
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
	         oit->GenOpenGLFunc( "Dot" , node->getType(), 2);
		 return true;
	    case EOpCross:
		 oit->GenOpenGLFunc("Cross", node->getType(), 2);
		 return true;
	    case EOpFaceForward:
		 oit->GenOpenGLFunc("FaceForward", node->getType(), 3);
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
    
    size_t size = oit->store_vars.size(); //Generate the store instruction before the if statement. Store the the numbers of vars to memory. ====== vector version ======= Kaymon
    //out.code <<"store_vars size:"<<(int) size <<"\n";
    //size = oit->store_vars_vector.size();
    //out.code <<"store_vars_vector "<<(int) size <<"\n";
#ifdef DEBUG_FOR_STORED_VARIABLE
    //out.code <<"store_vars size:"<<(int) size <<"\n";
    //size = oit->store_vars_vector.size();
    //out.code <<"store_vars_vector "<<(int) size <<"\n";
    //**************************List all the store instruction*************************
    for(int k =0; k < (int)size ;k++)
      out.code <<"if  vars:"<<(int)size<<" "<<oit->store_vars[k].getName() << "\n";
    size = oit->store_vars_vector.size(); //Generate the store instruction in  the label.T block. Store the the numbers of vars to memory. 
    for( int f = 0; f< (int)oit->store_vars_vector.size();f++)
      out.code <<"if2 vectors:"<<(int) oit->store_vars_vector.size()<<" "<< oit->store_vars_vector[f].getName() <<"\n";
#else
#endif    

    
    //Kaymon
    //**********************************************Note**************************************
    //This loop generate the store instruction which is mapping from store_vars to stor_vars_vector.
    //The vector number of matrix is wrong. Because I don't save the numbers in the function called "GenInsertEOM".
    //Because it shoulde be saved immediately. Can't put it below this basic block.
    //The vector number of matrix is the last record of variable "tmp_register_counter".
    
    for( size_t i = 0 ; i < size ; i++ ){
      //if( !oit->store_vars_vector[i].isMatrix() ) //Don't print the store instuction of matrix.
	oit->GenStore( oit->store_vars[i] , oit->store_vars_vector[i] ); //0727-- dondum; bug! store_vars_vector missing.
      //oit->ret_vector.erase( oit->store_vars[i].getName() );
    }



    oit->ret_vector.clear();
    oit->store_vars.clear();
    oit->store_vars_vector.clear();
    
    /*//Kaymon
    size_t size = oit->store_vars.size(); // Store the the  registers of vars to memory. ====== stack version ======
    // out.code <<"if :"<<(int) size <<"\n";
    for( size_t i = 0 ; i < size ; i++ ){
      TllvmVar src,dst;
      dst = oit->store_vars.top();
      oit->store_vars.pop();
      src = oit->store_vars_vector.top();
      oit->store_vars_vector.pop();
      oit->ret_vector.erase( dst.getName() );
      oit->GenStore( dst , src );
    }
    //Done*/
    
    oit->LoadReserveVariableFirstTime(cond);
    if(node->getTrueBlock() || node->getFalseBlock()){
#ifdef trick_for_bool
      
      if(cond.getType().getBasicType()==EbtBool&&oit->AfterGenCompareInstruction!=1){ //Because we change i1 to i32, we have to do one more operation to convert the type from i32 to i1.
                                                                                      //e.g. %1 = icmp eq highp i32 %0, 102
	                                                                              //e.g. br i1 %1, label %L1.T, label %L1.F ;we don't need to do type conversion.
	const TString &str = oit->allocate_tempvar().c_str();
	out.code <<"\t"<<str <<" = call i1"  <<" @OpenGLES.Convert.ItoB"
		 << "(" << cond.getCompleteString() <<" " << cond.getName()
		 << ")\n";
	cond.setName(str);
	oit->AfterGenCompareInstruction=0;
      }
      else
	oit->AfterGenCompareInstruction=0;

	out.code << "\tbr i1" 
		 << " " << cond.getName() << ", label %" << label.c_str()
		 << ".T," ;
#else
	out.code << "\tbr " << cond.getCompleteString().c_str()
		 << " " << cond.getName() << ", label %" << label.c_str()
		 << ".T," ;
#endif
	if (node->getFalseBlock())
	  out.code << " label %" << label.c_str() << ".F\n";
	else
	  out.code << " label %" << label.c_str() << ".E\n";
    }
    //out.code <<     node->getTrueBlock() <<"  "<< node->getFalseBlock()<<"\n";
    
    //Label True begin
    oit->ReturnVarInTrueBlock = 1;
    out.code << label.c_str() << ".T:\n";
    if (node->getTrueBlock())
	node->getTrueBlock()->traverse(it);


    size = oit->store_vars.size(); //Generate the store instruction in  the label.T block. Store the the registers of vars to memory. ====== vector version ======= Kaymon
#ifdef DEBUG_FOR_STORED_VARIABLE
    //out.code <<"store_vars size:"<<(int) size <<"\n";
    //size = oit->store_vars_vector.size();
    //out.code <<"store_vars_vector "<<(int) size <<"\n";
    //**************************List all the store instruction*************************
    for(int k =0; k < (int)size ;k++)
      out.code <<"if  :"<<(int)size<<" "<<oit->store_vars[k].getName() << "\n";
    size = oit->store_vars_vector.size(); //Generate the store instruction in  the label.T block. Store the the numbers of vars to memory. 
    for( int f = 0; f< (int)oit->store_vars_vector.size();f++)
      out.code <<"if2 :"<<(int) oit->store_vars_vector.size()<<" "<< oit->store_vars_vector[f].getName() <<"\n";
#else
#endif    
    //out.code <<"if :"<<(int) size <<"\n";
    //size = oit->store_vars_vector.size(); //Generate the store instruction in  the label.T block. Store the the registers of vars to memory. 
    //out.code <<"if2 :"<<(int) oit->store_vars_vector.size() <<"\n";

    
    for( size_t i = 0 ; i < size ; i++ ){
      //if( !oit->store_vars_vector[i].isMatrix() ) //Don't print the store instuction of matrix.
	oit->GenStore( oit->store_vars[i] , oit->store_vars_vector[i] );
      //oit->ret_vector.erase( oit->store_vars[i].getName() );
    }
    //***********************List the mapping relationship************************
    /*
    map< TString , TString >::iterator t;
    for ( t = oit->ret_vector.begin() ; t != oit->ret_vector.end(); t++ )
    out.code << t->first << " => " << t->second << "\n";**/
    /*
    for ( oit->it = oit->ret_vector.begin() ; oit->it != oit->ret_vector.end(); it++ )
      //      out.code << *(oit->it).first << " => " << *(oit->it).second << "\n";
            out.code << oit->it->first << " => " << oit->it->second << "\n";
    */
    oit->ret_vector.clear();
    oit->store_vars.clear();
    oit->store_vars_vector.clear();
    //Done

    //if (node->getFalseBlock()) //Kaymon done. We always need this instruction "br label.E" even though the label.F blocak doesn't have any content. 
    if( !oit->BreakorContinue && oit->ReturnVarInTrueBlock ) //if "break" come out, do not print this message. When returning var in true block,not to print
      out.code << "\tbr label %" << label.c_str() << ".E\n";
    oit->BreakorContinue = 0; //set the defaut value to this flag.


    oit->ReturnVarInFalseBlock = 1;
    //Label False begin
    if (node->getFalseBlock())
      out.code << label.c_str() << ".F:\n";
    if (node->getFalseBlock())
	node->getFalseBlock()->traverse(it);


    size = oit->store_vars.size(); //Generate the store instruction in  the label.F block. Store the the registers of vars to memory. ====== vector version ======= Kaymon
#ifdef DEBUG_FOR_STORED_VARIABLE
    //out.code <<"store_vars size:"<<(int) size <<"\n";
    //size = oit->store_vars_vector.size();
    //out.code <<"store_vars_vector "<<(int) size <<"\n";
    //**************************List all the store instruction*************************
    for(int k =0; k < (int)size ;k++)
      out.code <<"if  :"<<(int)size<<" "<<oit->store_vars[k].getName() << "\n";
    size = oit->store_vars_vector.size(); //Generate the store instruction in  the label.T block. Store the the numbers of vars to memory. 
    for( int f = 0; f< (int)oit->store_vars_vector.size();f++)
      out.code <<"if2 :"<<(int) oit->store_vars_vector.size()<<" "<< oit->store_vars_vector[f].getName() <<"\n";
#else
#endif    
    //out.code <<"if :"<<(int) size <<"\n";
    //size = oit->store_vars_vector.size(); //Generate the store instruction in  the label.T block. Store the the registers of vars to memory. ====== vector version ======= Kaymon
    //out.code <<"if2 :"<<(int) oit->store_vars_vector.size() <<"\n";


    
    for( size_t i = 0 ; i < size ; i++ ){
      //if( !oit->store_vars_vector[i].isMatrix() ) //Don't print the store instuction of matrix.
	oit->GenStore( oit->store_vars[i] , oit->store_vars_vector[i] );
      //oit->ret_vector.erase( oit->store_vars[i].getName() );
    }
    oit->ret_vector.clear();
    oit->store_vars.clear();
    oit->store_vars_vector.clear();
    //Done
    //out.code << oit->BreakorContinue << "\n";
    if (node->getFalseBlock() && !oit->BreakorContinue && oit->ReturnVarInTrueBlock && oit->ReturnVarInFalseBlock ) //Follow above true condition.
      out.code << "\tbr label %"<<label.c_str() << ".E\n";//Kaymon
    
    //out.code << oit->ReturnVarInTrueBlock << "\n";
    //out.code << oit->ReturnVarInFalseBlock << "\n";

    if( oit->ReturnVarInTrueBlock && oit->ReturnVarInFalseBlock )
      out.code << label.c_str() << ".E:\n";

    
    oit->BreakorContinue = 0; //set the defaut value to this flag.
    //oit->ReturnVarInTrueBlock = 0;//set the defaut value to this flag.
    //oit->ReturnVarInFalseBlock = 0;//set the defaut value to this flag.
    return false;
}

void TllvmOutputTraverser::llvmOutputConstantUnion(TIntermConstantUnion* node,
						   TIntermTraverser* it)
{
    TllvmOutputTraverser* oit = static_cast<TllvmOutputTraverser*>(it);
    TInfoSink& out = oit->infoSink;
    int size = node->getType().getObjectSize();
    //out.code <<"Type: "<<node->getType().getCompleteString()<<"\n";

    for (int i = 0; i < size; i++) {
      //out.code <<"size: " <<size<<" "<<node->getType().isStructure() <<"\n";
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
    
    //jump
    if( node->getType().isStructure() ){ //for struct initializing. e.g. struct_type( vec2(0.0),vec2(0.0) );
      TTypeList* tl = node->getType().getStruct();
      int size = tl?tl->size():0, i;
      TllvmVarList variables(size);
      TType curtype;
      TllvmVar v;
      
      for(i=size-1;i>=0;--i) {
	curtype = *((*tl)[i].type);
	v = oit->pop_expstack_rhs();
	//out.code << size <<" v.type: "<<v.getType().getCompleteString() <<" " << curtype.getCompleteString() <<" caller\n";
	

	if(v.getType() != curtype) {
	  oit->push_expstack(v);

	  
	  if(v.isConst()) {
	    if(curtype.isStructure()){
	      oit->RecordStructureSize++;
	      /*
	      TTypeList* t2 = curtype.getStruct();
	      int size2 = t2?t2->size():0;
	      out.code << size2 <<"+\n";
	      TType newcurtype = *((*t2)[0].type);
	      out.code << size2 <<" v.type: "<<v.getType().getCompleteString() <<" " << newcurtype.getCompleteString() <<" caller\n";
	      newcurtype = *((*t2)[1].type);
	      out.code << size2 <<" v.type: "<<v.getType().getCompleteString() <<" " << newcurtype.getCompleteString() <<" caller\n";
	      */

	      oit->ConstructorforStructure( curtype );
	      
	      
	      //out.code <<"isStructure in constantunion\n";
	      //v = oit->ConstructConstStruct(curtype, line);
	    }
	    else if(curtype.isVector() ){
	      TType TmpType( curtype );
	      string FnnameforConstruct;
	      TInfoSinkBase InttoAscii;
	      TllvmVarList VarList;
	      int indexforsize = 0;
	      InttoAscii << curtype.getNominalSize();
	      FnnameforConstruct = "OpenGLES.Construct";
	      if(TmpType.getBasicType() == EbtBool)
		FnnameforConstruct += "B";
	      else if(TmpType.getBasicType() == EbtInt)
		FnnameforConstruct += "I"; 
	      FnnameforConstruct += "Vec";
	      FnnameforConstruct += InttoAscii.c_str();
	      //out.code <<"curtype: " << curtype.getCompleteString()  <<" size : " << curtype.getNominalSize() << "\n";
	      while( indexforsize < curtype.getNominalSize() ) {
		TllvmVar v = oit->pop_expstack_rhs();
		VarList.push_back(v);
		oit->LoadReserveVariableFirstTime( VarList[indexforsize++] );
		//infoSink.code << v.getName() <<" "<<v.isEndConstructVector() <<"after pop1\n";
	      }
	      FnnameforConstruct = oit->DepolymorphismFunctionName(FnnameforConstruct, VarList);
	      //out.code << FnnameforConstruct <<"\n";
	      TllvmVar tmpv("", TmpType, TllvmVarCode);      
	      oit->GenFunc(tmpv, FnnameforConstruct, VarList);      
	      variables[i] =  oit->pop_expstack_rhs() ; 
	      oit->RecordParametersforStructure.push_back( variables[i] ) ;
	      
	      
	      //v = oit->ConstructConstVec(curtype, line);
	    }
	    else if(curtype.isMatrix()){

	      int SizeofMatrix = curtype.getNominalSize();
	      for(int j = 0; j < SizeofMatrix ; j++ ){
		TType TmpType( EbtFloat,EvqTemporary,SizeofMatrix,false,false,EbpHigh );//Because Matrix is composed of SizeofMatrix vector.Now we indicate one vector.
		string FnnameforConstruct;
		TInfoSinkBase InttoAscii;
		TllvmVarList VarList;
		int indexforsize = 0;
		TllvmVarList VariablesforMatrix(SizeofMatrix);
		InttoAscii << curtype.getNominalSize();
		FnnameforConstruct = "OpenGLES.Construct";
		if(TmpType.getBasicType() == EbtBool)
		  FnnameforConstruct += "B";
		else if(TmpType.getBasicType() == EbtInt)
		  FnnameforConstruct += "I"; 
		FnnameforConstruct += "Vec";
		FnnameforConstruct += InttoAscii.c_str();
		//out.code <<"curtype: " << curtype.getCompleteString()  <<" size : " << curtype.getNominalSize() << "\n";
		while( indexforsize < curtype.getNominalSize() ) {
		  TllvmVar v = oit->pop_expstack_rhs();
		  VarList.push_back(v);
		  oit->LoadReserveVariableFirstTime( VarList[indexforsize++] );
		  //infoSink.code << v.getName() <<" "<<v.isEndConstructVector() <<"after pop1\n";
		}
		FnnameforConstruct = oit->DepolymorphismFunctionName(FnnameforConstruct, VarList);
		//out.code << FnnameforConstruct <<"\n";
		TllvmVar tmpv("", TmpType, TllvmVarCode);      
		oit->GenFunc(tmpv, FnnameforConstruct, VarList);      
		VariablesforMatrix[j] =  oit->pop_expstack_rhs() ; 
	      //out.code << variables[i].getName() <<"\n";
		oit->RecordParametersforStructure.push_back( VariablesforMatrix[j] ) ;
	      /*
	      int SizeofMatrix = curtype.getObjectSize();
	      for(int j = 0; j < SizeofMatrix ; j++ ){
		oit->RecordParametersforStructure.push_back( oit->pop_expstack_rhs() ) ;
		}*/
	      //out.code <<"constant union isMatrix in constantunion\n";
	      //v = oit->ConstructConstMat(curtype, line);
	      }
	    }
	    else {
	      out.code<<"not matrix ,const and vector ??\n";
	    }
	  }
	
	  
	  //oit->RecordParametersforStructure[i] = variables[i];
	  //out.code << variables[i].getName() <<" i:"<< i <<"\n";
	}
	else {//e.g. sabcd s = sabcd(1.0, 2.0, 4.0, 8.0)
	  //if  (curtype.getBasicType() == EbtInt ){
	    TllvmVar tmpv( v.getName() , curtype );   
	    variables[i] =  tmpv ;    
	    oit->RecordParametersforStructure.push_back( variables[i] ) ;
	    //out.code << v.getName() <<"\n";
	}

	//out.code << variables[0].getName() << " out of loop \n";
	
	/*		
	if(v.getType() != curtype) {
	  out.code << "\n" << v.getType().getCompleteString()
			<< " " << v.getName()
			<< ", " << curtype.getCompleteString() << "\n";
	  out.info.message(EPrefixInternalError,
				"type mismatch when Constructing Structure",
				line);
	  // return false;
	}
	
	conststruct[i] = v;
	*/
      }
      TllvmVar tempvar( TString("StructureConstructor"),node->getType() );
      oit->push_expstack( tempvar );

      /*
      if(!oit->pop_expstack().isEndConstructStruct()) {
	out.info.message(EPrefixInternalError,
			      "argument count mismatch when Constructing Structure",
			      line);
	//return false;
      }
      out.code <<"end\n";
      oit->push_expstack(TllvmVar(conststruct, node->getType() ));
      //return true;
      */
      
    }
#ifdef NewMatrix
    if( node->getType().isMatrix() ){
      //gggggggggggg
      //out.code << node->getType().getNominalSize() <<"\n";
	oit->GenConstructMat( node->getType() , -999);
    }
    else if( size > 1 && !node->getType().isStructure() ){  //make the vec3(1.0,1.2,1.3) is OK.
#else
    if( size > 1 && !node->getType().isStructure() ){  //make the vec3(1.0,1.2,1.3) is OK.
#endif
      //num2 = FloattoVec+1;
      TType type(node->getType());
      string fnname1;
      TInfoSinkBase itoa;
      TllvmVarList cmvec;
      int index = 0;
      itoa << size;
      fnname1 = "OpenGLES.Construct";
      if(type.getBasicType() == EbtBool)
	fnname1 += "B";
      else if(type.getBasicType() == EbtInt)
       fnname1 += "I"; 
      fnname1 += "Vec";
      fnname1 += itoa.c_str();
      
      while( index < size ) {
	TllvmVar v = oit->pop_expstack_rhs();
	cmvec.push_back(v);
	oit->LoadReserveVariableFirstTime( cmvec[index++] );
	//infoSink.code << v.getName() <<" "<<v.isEndConstructVector() <<"after pop1\n";
      }
      fnname1 = oit->DepolymorphismFunctionName(fnname1, cmvec);
      //out.code << fnname1 <<"\n";
      TllvmVar tv("", type, TllvmVarCode);      
      oit->GenFunc(tv, fnname1, cmvec);      
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

    
    //----------------------------print the store instruction last block befor loop block.----------------------------------
    size_t size;
    size = oit->store_vars.size();
    //out.code<<"size:"<<(int )size<<"\n";
    //size = oit->store_vars_vector.size();
    //out.code<<"size2:"<<(int )size<<"\n";

    for( size_t i = 0 ; i < size ; i++ ){
      //std::cout<<" oit->store_vars[" << i <<"].getName :" << oit->store_vars[i].getName() <<"\n";
      //std::cout<<" oit->store_vars_vector["<< i <<"].getName :" << oit->store_vars_vector[i].getName() <<"\n";
      oit->GenStore( oit->store_vars[i] , oit->store_vars_vector[i] ); 
      //oit->ret_vector.erase( oit->store_vars[i].getName() );
    }
    oit->ret_vector.clear();
    oit->store_vars.clear();
    oit->store_vars_vector.clear();
    out.code << "\tbr " << "label %" <<label.c_str() << ".C\n";
    //size = oit->store_vars.size();
    //std::cout<<(int) size <<"\n";
    //out.code<<(int) size<<"\n";
    //size = oit->store_vars_vector.size();
    //std::cout<<(int) size <<"\n";
    //oit->infoSink.code << (int)size<<"\n";

    if (node->testFirst()) // ----------------print the loop condition------------------------------.
	oit->GenCondition(node, label);
    oit->ret_vector.clear(); //  Just remove the previous data.
    
    //size = oit->store_vars.size();
    //std::cout<<(int) size <<"\n";
    //out.code<<(int) size<<"\n";
    //size = oit->store_vars_vector.size();
    //std::cout<<(int) size <<"\n";
    //oit->infoSink.code << (int)size<<"\n";
    /*
     for( size_t i = 0 ; i < size ; i++ ){
      //std::cout<<" oit->store_vars[" << i <<"].getName :" << oit->store_vars[i].getName() <<"\n";
      //std::cout<<" oit->store_vars_vector["<< i <<"].getName :" << oit->store_vars_vector[i].getName() <<"\n";
      oit->GenStore( oit->store_vars[i] , oit->store_vars_vector[i] ); 
      oit->ret_vector.erase( oit->store_vars[i].getName() );
    }

    oit->store_vars.clear();
    oit->store_vars_vector.clear();
    */
    
    
    out.code << label.c_str() << ".B:\n";
    if (node->getBody()) {
	node->getBody()->traverse(it);
    }

    
        
    //out.code << label.c_str() << ".T:\n"; //we don't need this label.
    if (node->getTerminal()) // e.g. i++
	node->getTerminal()->traverse(it);
    
    size = oit->store_vars.size();
    //std::cout<<(int) size <<"\n";
    //out.code<<(int) size<<"\n";
    //size = oit->store_vars_vector.size();
    //std::cout<<(int) size <<"\n";
    //oit->infoSink.code << (int)size<<"\n";

    for( size_t i = 0 ; i < size ; i++ ){
      //std::cout<<" oit->store_vars[" << i <<"].getName :" << oit->store_vars[i].getName() <<"\n";
      //std::cout<<" oit->store_vars_vector["<< i <<"].getName :" << oit->store_vars_vector[i].getName() <<"\n";
      oit->GenStore( oit->store_vars[i] , oit->store_vars_vector[i] ); 
      //oit->ret_vector.erase( oit->store_vars[i].getName() );
    }
    oit->ret_vector.clear(); 
    oit->store_vars.clear();
    oit->store_vars_vector.clear();


    if (node->testFirst())
	out.code << "\tbr label %" << label.c_str() << ".C\n";

    if (!node->testFirst()) // maybe for do-while
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
		oit->BreakorContinue = 1; // set 1 to this flag. Indicate the "break" come out.
		break;
	    case EOpContinue:
	        out.code << "\tbr label %L" << oit->labelnr //Go to the i++ or ++i section.
		  //<< oit->get_loop_label().c_str() //original code.
		  //<< ".T\n";
			 << ".E\n";
		
		oit->BreakorContinue = 1; // set 1 to this flag. Indicate the "continue" come out.
		break;
	    case EOpKill: /* Not sure what kill does, put it with return for now. */
	    case EOpReturn:
		{
		    oit->ReturnFlag = 1; //Function has return something. temporary method.
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
			if( oit->ReturnVarInTrueBlock ) // set -1 to this variable if returning variables in true bloack
			  oit->ReturnVarInTrueBlock = 0;
			else if( oit->ReturnVarInFalseBlock )// set -1 to this variable if returning variables in false bloack
			  oit->ReturnVarInFalseBlock = 0;
			oit->LoadReserveVariableFirstTime(var); //e.g. load variable first before return this variable.
			//out.code << funcvar.getName() << " " << funcvar.getBasicType()<<"\n";
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
			//out.code << "\tret void\n";//For discard. original code.
			oit->allocate_tempvar().c_str();//ret void is counted for one register.
			out.code << "\tbr label %return\n";//For discard. original code.
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
bool TllvmOutputTraverser::HandleArithmetic( TllvmVar &dst, TllvmVar &src , string opstr ){ 

     TllvmVar ev[2];
     ev[0] = src;
     ev[1] = dst;
     //infoSink.code<< src.getName() << " src \n";
     //infoSink.code<< dst.getName() << " dst \n";
     if( opstr == "fdiv" && ev[1].getType() != ev[0].getType() ){ //produce the content which is like <float IEEE 754 presentation, float IEEE , ......>. The counter is  the dividend's vector type. Kaymon
       /*if( ev[0].getType() != ev[1].getType() ){ //maybe the ev[1]'s type is less than ev[0] 
	 ev[0].setType( ev[1].getType() );
	 }*/
       //infoSink.code << "Here??";
       int max = 0;
       if( ev[1].getNominalSize() > ev[0].getNominalSize() ){// divisor's type is less than dividend.
	 ev[0].setType( ev[1].getType() );
       }
       else{ // dividend's type is less than divisor.
	 ev[1].setType( ev[0].getType() );
	 max = 1;
       }
       //infoSink.code << max <<"\n";
       const char *temp; 
       if(!max){
	 bool IsNum = true; // Make sure that the string is number or string. 

	 temp = ev[0].getName().c_str();
	 temp++;
	 while( *temp != '\0' ){
	   if( *temp < '0' || *temp > '9'){
	     IsNum = false;
	     break;
	   }
	   temp++;
	 }
	 //infoSink.code << ev[0].getName()<<" "<<IsNum<<"\n";
	 //if(ev[0].getName()[0]=='0'&&ev[0].getName()[1]=='x')//for IEEE754
	 //IsNum=true;
	 if( IsNum ){
	   const TString &str = allocate_tempvar().c_str();
	   infoSink.code << "\t" << str
			 << " = call " << ev[1].getCompleteString()
			 << " @OpenGLES.ConstructVec4.f.f.f.f(" 
			 << "float " << ev[0].getName()
			 << " ,float " << ev[0].getName()
			 << " ,float " << ev[0].getName()
			 << " ,float " << ev[0].getName()
			 << ")\n";
	   src.setName( str );
	 }
	 else{
	   //int  i =  ev[0].getType().getCompleteString()[7] - 48;  //
	   int i = 4;
	   TString name,tempname;
	   tempname = ev[0].getName();
	   name = "<";
	   while( i > 0 ){
	     name += "float ";
	     name += tempname;
	     if( i == 1)
	      break;
	     name += ", ";
	     i--;
	  }
	   name += ">";
	   
	   ev[0].setName( name );
	   src.setName( name );
	   push_expstack(ev[1]);
	   push_expstack(ev[0]);
	   GenOpenGLFunc( "FloatDiv" , ev[0].getType() , 2 );
	   return true;
	 }
       }
       else{
	 //int  i =  ev[0].getType().getCompleteString()[7] - 48;  //
	 int i = 4;
	 TString name,tempname;
	 tempname = ev[1].getName();
	 name = "<";
	 while( i > 0 ){
	  name += "float ";
	  name += tempname;
	  if( i == 1)
	    break;
	  name += ", ";
	  i--;
	 }
	 name += ">";
	 dst.setName( name );
	 return true;
       }
     }

     return false;
}

bool TllvmOutputTraverser::ConstructorforStructure( TType curtype ){
      

  //jump
  //infoSink.code <<"\n\nCome into function**********************************\n";
  TTypeList* tl = curtype.getStruct();
  int size = tl?tl->size():0, i;
  TllvmVarList variables(size);
  TType newcurtype;
  TllvmVar v;
  //infoSink.code <<" size of structure type: "<<size<<"\n";
  for(i=size-1;i>=0;--i) {
    newcurtype = *((*tl)[i].type);
    v = pop_expstack_rhs();
    //infoSink.code << size <<" v.type: "<<v.getType().getCompleteString() <<" " << newcurtype.getCompleteString() <<" callee first\n";
    //infoSink.code << "index of loop: " << i << "\n";

    if(v.getType() != newcurtype) {
      push_expstack(v);


      if(v.isConst()) {
	if(newcurtype.isStructure()){
	  RecordStructureSize++;
	  /*
	  TTypeList* t2 = newcurtype.getStruct();
	  int size2 = t2?t2->size():0;
	  TType newcurtype2 = *((*t2)[0].type);
	  infoSink.code << size2 <<"++++++\n";
	  infoSink.code << newcurtype2.getCompleteString() <<"==============\n";
	  infoSink.code <<"\n\nrecursive start\n";
	  infoSink.code << newcurtype2.getCompleteString() <<" "<< newcurtype2.isVector()<<" newcurtype\n";
	  */
	  ConstructorforStructure( newcurtype );
	  //infoSink.code <<"recursive end\n\n\n"; 
	  //v = oit->ConstructConstStruct(curtype, line);
	}
	else if(newcurtype.isVector() ){
	  TType TmpType( newcurtype );
	  string FnnameforConstruct;
	  TInfoSinkBase InttoAscii;
	  TllvmVarList VarList;
	  int indexforsize = 0;
	  InttoAscii << newcurtype.getNominalSize();
	  FnnameforConstruct = "OpenGLES.Construct";
	  if(TmpType.getBasicType() == EbtBool)
	    FnnameforConstruct += "B";
	  else if(TmpType.getBasicType() == EbtInt)
	    FnnameforConstruct += "I"; 
	  FnnameforConstruct += "Vec";
	  FnnameforConstruct += InttoAscii.c_str();
	  //out.code <<"curtype: " << curtype.getCompleteString()  <<" size : " << curtype.getNominalSize() << "\n";
	  while( indexforsize < newcurtype.getNominalSize() ) {
	    TllvmVar v = pop_expstack_rhs();
	    VarList.push_back(v);
	    LoadReserveVariableFirstTime( VarList[indexforsize++] );
	    //infoSink.code << v.getName() <<" "<<v.isEndConstructVector() <<"after pop1\n";
	  }
	  FnnameforConstruct = DepolymorphismFunctionName(FnnameforConstruct, VarList);
	  //out.code << FnnameforConstruct <<"\n";
	  TllvmVar tmpv("", TmpType, TllvmVarCode);      
	  GenFunc(tmpv, FnnameforConstruct, VarList);      
	  variables[i] =  pop_expstack_rhs() ; 
	  
	  RecordParametersforStructure.push_back( variables[i] ) ;
	      
	      //v = oit->ConstructConstVec(curtype, line);
	}
	else if(newcurtype.isMatrix()){
	  //infoSink.code <<"isMatrix in constantunion\n";
	  int SizeofMatrix = newcurtype.getNominalSize();
	  for(int j = 0; j < SizeofMatrix ; j++ ){
	    TType TmpType( EbtFloat,EvqTemporary,SizeofMatrix,false,false,EbpHigh );//Because Matrix is composed of SizeofMatrix vector.Now we indicate one vector.
	    string FnnameforConstruct;
	    TInfoSinkBase InttoAscii;
	    TllvmVarList VarList;
	    int indexforsize = 0;
	    TllvmVarList VariablesforMatrix(SizeofMatrix);
	    InttoAscii << newcurtype.getNominalSize();
	    FnnameforConstruct = "OpenGLES.Construct";
	    if(TmpType.getBasicType() == EbtBool)
	      FnnameforConstruct += "B";
	    else if(TmpType.getBasicType() == EbtInt)
	      FnnameforConstruct += "I"; 
	    FnnameforConstruct += "Vec";
	    FnnameforConstruct += InttoAscii.c_str();
	    //out.code <<"curtype: " << curtype.getCompleteString()  <<" size : " << curtype.getNominalSize() << "\n";
	    while( indexforsize < newcurtype.getNominalSize() ) {
	      TllvmVar v = pop_expstack_rhs();
	      VarList.push_back(v);
	      LoadReserveVariableFirstTime( VarList[indexforsize++] );
	      //infoSink.code << v.getName() <<" "<<v.isEndConstructVector() <<"after pop1\n";
	    }
	    FnnameforConstruct = DepolymorphismFunctionName(FnnameforConstruct, VarList);
	    //out.code << FnnameforConstruct <<"\n";
	    TllvmVar tmpv("", TmpType, TllvmVarCode);      
	    GenFunc(tmpv, FnnameforConstruct, VarList);      
	    VariablesforMatrix[j] =  pop_expstack_rhs() ; 
	    //out.code << variables[i].getName() <<"\n";
	    RecordParametersforStructure.push_back( VariablesforMatrix[j] ) ;
	    /*
	      int SizeofMatrix = curtype.getObjectSize();
	      for(int j = 0; j < SizeofMatrix ; j++ ){
	      oit->RecordParametersforStructure.push_back( oit->pop_expstack_rhs() ) ;
	      }*/
	    //out.code <<"constant union isMatrix in constantunion\n";
	    //v = oit->ConstructConstMat(curtype, line);
	  }
	  //v = oit->ConstructConstMat(curtype, line);
	}
      }
      //oit->RecordParametersforStructure[i] = variables[i];
      //out.code << variables[i].getName() <<" i:"<< i <<"\n";
    }
    else { //for int , float.
      //if  (curtype.getBasicType() == EbtInt ){
      TllvmVar tmpv( v.getName() , newcurtype );   
      variables[i] =  tmpv ;    
      RecordParametersforStructure.push_back( variables[i] ) ;    
      //out.code << v.getName() <<"\n";
    }
    
    //out.code << variables[0].getName() << " out of loop \n";
	
    /*		
		if(v.getType() != curtype) {
		out.code << "\n" << v.getType().getCompleteString()
		<< " " << v.getName()
		<< ", " << curtype.getCompleteString() << "\n";
		out.info.message(EPrefixInternalError,
		"type mismatch when Constructing Structure",
		line);
		// return false;
		}
		
		conststruct[i] = v;
    */
  }
  /*
  if( size == 0 ){


    infoSink.code <<"terminal\n";

  }
  */
  //TllvmVar tempvar( TString("StructureConstructor"),curtype );
  //push_expstack( tempvar );
  

  return true;
} 

