/*
 * Auther: CoolDavid <cooldavid@cooldavid.org>
 * Copyrite (C) 2008 OSLab CSE NSYSU
 * All rights reserved.
 *
 * This file holds the LLVM code generator class declaration
 *
 */

#ifndef __LLVMVAR_H_IS_INCLUDDED__
#define __LLVMVAR_H_IS_INCLUDDED__

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

class TllvmVar;
struct IndexUnit;

typedef vector<TllvmVar> TllvmVarList;
typedef vector<IndexUnit> TllvmVarIndexList;

enum TllvmVarType {
    TllvmVarNormal,
    TllvmVarCode,
    TllvmVarConstant,
    TllvmVarSwizzle,
    TllvmVarEndConstructVector,
    TllvmVarEndConstructMatrix,
    TllvmVarEndConstructStruct
};

class TllvmVar {
    public:
	TllvmVar() : name(""), type(EbtVoid), vartype(TllvmVarNormal) { }
	TllvmVar(const TString& n,
		 const TType& t,
		 const TllvmVarType lvt = TllvmVarNormal);
	TllvmVar(bool b);
	TllvmVar(int i);
	TllvmVar(float f);
	TllvmVar(const vector<bool> &bvec);
	TllvmVar(const vector<int> &ivec);
	TllvmVar(const vector<float> &fvec);
	TllvmVar(const vector< vector<float> > &mat);
	TllvmVar(const TllvmVarList &cs, const TType &t);
	TllvmVar(const vector<int> &swi, const TType &t);
	const TString& getName() const { return name; }
	void setName(const TString& n){ name = n; }

	void setType(const TType& t) { type = t; }
	const TType& getType() const { return type; }
	TType* getTypePointer() { return &type; }

	TBasicType getBasicType() const { return type.getBasicType(); }
	TQualifier getQualifier() const { return type.getQualifier(); }
	int getNominalSize() const { return type.getNominalSize(); }
	int getSize() const { return type.getInstanceSize(); }
	bool isMatrix() const { return type.isMatrix(); }
	bool isArray()  const { return type.isArray(); }
	bool isVector() const { return type.isVector(); }
	bool isStructure() const { return type.isStructure(); }
	bool isNonAgg() const { return type.isNonAgg(); }
	bool isConst() const { return vartype == TllvmVarConstant;}
	bool isTemp() const
	{
	    return name.at(0) == '%' && name.at(1) >= '0' && name.at(1) <= '9';
	}
	const char* getBasicString() const {
	    return type.getBasicString();
	}
	const char* getQualifierString() const {
	    return type.getQualifierString();
	}

	//dondum
	const char* getQualifierShortString() const {
	    return type.getQualifierShortString();
	}
	TString getCompleteString() const { return type.getCompleteString(); }

	/* Is swizzle */
	bool isSwizzle() const { return vartype == TllvmVarSwizzle; }
	const vector<int>& getSwizzle() const { return swizzle; }

	/* Is function call code */
	bool isCode() const {return vartype == TllvmVarCode;}

	bool isEndConstructVector() const {return vartype == TllvmVarEndConstructVector;}
	bool isEndConstructMatrix() const {return vartype == TllvmVarEndConstructMatrix;}
	bool isEndConstructStruct() const {return vartype == TllvmVarEndConstructStruct;}

	/* Is simple type */
	bool isSimpleType() const
	{
	    return (vartype == TllvmVarNormal || vartype == TllvmVarConstant) &&
		   !isMatrix() &&
		   !isVector() &&
		   !isStructure();
	}
	bool isSimpleVar() const
	{
	    return vartype == TllvmVarNormal &&
		   !isMatrix() &&
		   !isVector() &&
		   !isStructure();
	}
	bool isNormalType() const
	{
	    return vartype == TllvmVarNormal;
	}
	bool isSimpleConst() const
	{
	    return vartype == TllvmVarConstant &&
		   !isMatrix() &&
		   !isVector() &&
		   !isStructure();
	}

	/* For Indices */
	bool haveIndex() const { return !indexlist.empty(); }
	void pushIndex(const IndexUnit &iu) { indexlist.push_back(iu); }
	const TllvmVarIndexList& getIndex(void) const { return indexlist; }
	int getFirstNonAggPos(void) const;

	/* For constants */
	bool getAsBool() const {return constbool;}
	int getAsInt() const {return constint;}
	float getAsFloat() const {return constfloat;}
	vector<bool> getAsBVec() const {return constbvec;}
	vector<int> getAsIVec() const {return constivec;}
	vector<float> getAsFVec() const {return constfvec;}
	vector< vector<float> > getAsMatrix() const {return constmat;}
	TllvmVarList getAsStruct() const {return conststruct;}

    protected:
	TInfoSinkBase isb;
	TString name;
	TType type;
	TllvmVarType vartype;
	vector<int> swizzle;

	bool constbool;
	int constint;
	float constfloat;
	vector<bool> constbvec;
	vector<int> constivec;
	vector<float> constfvec;
	vector< vector<float> > constmat;
	TllvmVarList conststruct;
	TllvmVarIndexList indexlist;
};

struct IndexUnit {
    TType type;
    TllvmVar index;
};

#endif

