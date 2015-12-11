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

#ifndef _TYPES_INCLUDED
#define _TYPES_INCLUDED

#include "../Include/Common.h"
#include "../Include/BaseTypes.h"
#include "../Include/InfoSink.h"
#include <iostream>
#define NewMatrix
#define trick_for_bool
//
// Need to have association of line numbers to types in a list for building structs.
//

class TType;
struct TTypeLine {
    TType* type;
    int line;
};
typedef TVector<TTypeLine> TTypeList;

inline TTypeList* NewPoolTTypeList()
{
	void* memory = GlobalPoolAllocator.allocate(sizeof(TTypeList));
	return new(memory) TTypeList;
}

//
// This is a workaround for a problem with the yacc stack,  It can't have
// types that it thinks have non-trivial constructors.  It should
// just be used while recognizing the grammar, not anything else.  Pointers
// could be used, but also trying to avoid lots of memory management overhead.
//
// Not as bad as it looks, there is no actual assumption that the fields
// match up or are name the same or anything like that.
//
class TPublicType {
public:
    TBasicType type;
    TQualifier qualifier;
    TPrecision precision;
    int size;          // size of vector or matrix, not size of array
    bool matrix;
    bool array;
    int arraySize;
    TType* userDef;
    int line;

    void setBasic(TBasicType bt, TQualifier q, int ln = 0)
    {
        type = bt;
        qualifier = q;
	precision = EbpHigh;
        size = 1;
        matrix = false;
        array = false;
        arraySize = 0;
        userDef = 0;
        line = ln;
    }

    void setPrecision(TPrecision pcs)
    {
	    precision = pcs;
    }

    void setAggregate(int s, bool m = false)
    {
        size = s;
        matrix = m;
    }

    void setArray(bool a, int s = 0)
    {
        array = a;
        arraySize = s;
    }
};

typedef std::map<TTypeList*, TTypeList*> TStructureMap;
typedef std::map<TTypeList*, TTypeList*>::iterator TStructureMapIterator;
//
// Base class for things that have a type.
//
class TType {
public:
    POOL_ALLOCATOR_NEW_DELETE(GlobalPoolAllocator)
    explicit TType(TBasicType t, TQualifier q = EvqTemporary, int s = 1, bool m = false, bool a = false, TPrecision p = EbpHigh) :
                            type(t), precision(p), qualifier(q), size(s), matrix(m), array(a), arraySize(0),
                            structure(0), structureSize(0), maxArraySize(0), arrayInformationType(0), fieldName(0), mangled(0), typeName(0)
			      { /*printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");fflush(stdout);*/}
    explicit TType(const TPublicType &p) :
                            type(p.type), precision(p.precision), qualifier(p.qualifier), size(p.size), matrix(p.matrix), array(p.array), arraySize(p.arraySize),
                            structure(0), structureSize(0), maxArraySize(0), arrayInformationType(0), fieldName(0), mangled(0), typeName(0)
			      { //printf("BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\n");fflush(stdout);
                              if (p.userDef) {
                                  structure = p.userDef->getStruct();
                                  typeName = NewPoolTString(p.userDef->getTypeName().c_str());
                              }
                            }
    explicit TType(TTypeList* userDef, const TString& n, TPrecision p = EbpHigh) :
                            type(EbtStruct), precision(p), qualifier(EvqTemporary), size(1), matrix(false), array(false), arraySize(0),
			      structure(userDef), maxArraySize(0), arrayInformationType(0), fieldName(0), mangled(0) {//printf("CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\n");fflush(stdout);
								typeName = NewPoolTString(n.c_str());
                            }
	explicit TType() {}
    virtual ~TType() {}

	TType(const TType& type) { *this = type; }

	void copyType(const TType& copyOf, TStructureMap& remapper)
	{
		type = copyOf.type;
		precision = copyOf.precision;
		qualifier = copyOf.qualifier;
		size = copyOf.size;
		matrix = copyOf.matrix;
		array = copyOf.array;
		arraySize = copyOf.arraySize;

		TStructureMapIterator iter;
		if (copyOf.structure) {
	        if ((iter = remapper.find(structure)) == remapper.end()) {
				// create the new structure here
				structure = NewPoolTTypeList();
				for (unsigned int i = 0; i < copyOf.structure->size(); ++i) {
					TTypeLine typeLine;
					typeLine.line = (*copyOf.structure)[i].line;
					typeLine.type = (*copyOf.structure)[i].type->clone(remapper);
					structure->push_back(typeLine);
				}
			} else {
				structure = iter->second;
			}
		} else
			structure = 0;

		fieldName = 0;
		if (copyOf.fieldName)
			fieldName = NewPoolTString(copyOf.fieldName->c_str());
		typeName = 0;
		if (copyOf.typeName)
			typeName = NewPoolTString(copyOf.typeName->c_str());

		mangled = 0;
		if (copyOf.mangled)
			mangled = NewPoolTString(copyOf.mangled->c_str());

		structureSize = copyOf.structureSize;
		maxArraySize = copyOf.maxArraySize;
		assert(copyOf.arrayInformationType == 0);
		arrayInformationType = 0; // arrayInformationType should not be set for builtIn symbol table level
	}

	TType* clone(TStructureMap& remapper)
	{
		TType *newType = new TType();
		newType->copyType(*this, remapper);

		return newType;
	}

    virtual void setType(TBasicType t, int s, bool m, bool a, int aS = 0)
                            { type = t; size = s; matrix = m; array = a; arraySize = aS; }
    virtual void setType(TBasicType t, int s, bool m, TType* userDef = 0)
                            { type = t;
                              size = s;
                              matrix = m;
                              if (userDef)
                                  structure = userDef->getStruct();
                              // leave array information intact.
                            }
    virtual void setTypeName(const TString& n) { typeName = NewPoolTString(n.c_str()); }
    virtual void setFieldName(const TString& n) { fieldName = NewPoolTString(n.c_str()); }
    virtual const TString& getTypeName() const
    {
		assert(typeName);
    	return *typeName;
    }

    virtual const TString& getFieldName() const
    {
    	assert(fieldName);
		return *fieldName;
    }

    virtual TBasicType getBasicType() const { return type; }
    virtual TPrecision getPrecision() const { return precision; }
    virtual TQualifier getQualifier() const { return qualifier; }
    virtual void changePrecision(TPrecision p) { precision = p; }
    virtual void changeQualifier(TQualifier q) { qualifier = q; }

    // One-dimensional size of single instance type
    virtual int getNominalSize() const { 
      //printf("No problem here\n");
      //fflush(stdout);
      //printf("Size is: %d   (%d)\n",size,sizeof(int));
      //fflush(stdout);
      //printf("-----------------------------------------\n");
      //fflush(stdout);
return size; }

    // Full-dimensional size of single instance of type
    virtual int getInstanceSize() const
    {
      if (matrix) //dondum
	  return size * size ;
        else
            return size;
    }

    virtual bool isMatrix() const { return matrix ? true : false; }
    virtual bool isArray() const  { return array ? true : false; }
    int getArraySize() const { return arraySize; }
    void setArraySize(int s) { array = true; arraySize = s; }
    void setMaxArraySize (int s) { maxArraySize = s; }
    int getMaxArraySize () const { return maxArraySize; }
    void clearArrayness() { array = false; arraySize = 0; maxArraySize = 0; }
    void setArrayInformationType(TType* t) { arrayInformationType = t; }
    TType* getArrayInformationType() { return arrayInformationType; }
    virtual bool isVector() const { return size > 1 && !matrix; }
    virtual bool isStructure() const { return type == EbtStruct; }
    virtual bool isNonAgg() const { return !isArray() && !isStructure(); };
    static const char* getBasicString(TBasicType t) {
        switch (t) {
        case EbtVoid:              return "void";              break;
        case EbtFloat:             return "float";             break;
        case EbtInt:               return "i32";               break;
#ifdef trick_for_bool
        case EbtBool:              return "i32";                break;//Change the bool to int. It will be easy to implement. Kaymon
#else
	case EbtBool:              return "i1";                break;
#endif
        case EbtSampler2D:         return "sampler2D";         break;
        case EbtSamplerCube:       return "samplerCube";       break;
        case EbtStruct:            return "structure";         break;
        default:                   return "unknown type";
        }
    }
    const char* getBasicString() const { return TType::getBasicString(type); }
    const char* getPrecisionString() const { return ::getPrecisionString(precision); }
    const char* getQualifierString() const { return ::getQualifierString(qualifier); }
    //dondum
    const char* getQualifierShortString() const { return ::getQualifierShortString(qualifier); }

    TTypeList* getStruct() { return structure; }

    int getObjectSize() const
    {
        int totalSize;

        if (getBasicType() == EbtStruct)
            totalSize = getStructSize();
        else if (matrix)
            totalSize = size * size;
        else
            totalSize = size;

        if (isArray())
            totalSize *= Max(getArraySize(), getMaxArraySize());

        return totalSize;
    }

    TTypeList* getStruct() const { return structure; }
    TString& getMangledName() {
        if (!mangled) {
			mangled = NewPoolTString("");
            buildMangledName(*mangled);
            *mangled += ';' ;
        }

        return *mangled;
    }
    bool sameElementType(const TType& right) const {
        return      type == right.type   &&
                    size == right.size   &&
                  matrix == right.matrix &&
               structure == right.structure;
    }
    bool operator==(const TType& right) const {
        return      type == right.type   &&
                    size == right.size   &&
                  matrix == right.matrix &&
                   array == right.array  && (!array || arraySize == right.arraySize) &&
               structure == right.structure;
        // don't check the qualifier, it's not ever what's being sought after
    }
    bool operator!=(const TType& right) const {
        return !operator==(right);
    }


    //    bool JJJ() const {printf("LLLLLLLLLLLLLLLLLLL\n");} //const TType& right) const {printf("LLLLLLLLLLLLLLLLLLL\n");}

    TString getCompleteString(const bool outputQualifier = false) const
    {
	char buf[128];
	char *p = buf;
	TInfoSinkBase typestr;
        //static int GGG =0;
        //printf("GGG@@ : %d\n",++GGG);fflush(stdout);
	//if(GGG==199)	  { JJJ();	    printf("KKKKKKK\n");	  }

	//printf("[[%d]]\n",getBasicType() == EbtStruct);fflush(stdout);


	if(getBasicType() == EbtStruct && structure && structure->size() > 0)
	{
	  //printf("@@@@@@@@@@@@@@@@@@@@HHH : %d\n",GGG);fflush(stdout);
	    if(array)
		typestr << "[" << arraySize << " x ";

	    typestr << "{" << (*structure)[0].type->getCompleteString();
	    for(unsigned int i = 1 ; i < structure->size() ; ++i)
		typestr << ", " << (*structure)[i].type->getCompleteString();
	    typestr << "}";

	    if(array)
		typestr << "]";

            //printf("HHH : %d\n",GGG);fflush(stdout);
	    return TString(typestr.c_str());
	}

#define PRINT_PRECISION 1
#if	PRINT_PRECISION
	p += sprintf(p, "%s ", getPrecisionString());
#endif

        //printf("##########HHH : %d\n",GGG);fflush(stdout);
	if (qualifier != EvqTemporary && qualifier != EvqGlobal && outputQualifier)
	    p += sprintf(p, "%s ", getQualifierString());

	if (array)
	    p += sprintf(p, "[%d x ", arraySize);

	if (matrix){
	  //dondum marked it.
#ifndef NewMatrix
	  p += sprintf(p, "<%d x %d ", size, size);
#else
	  p += sprintf(p, "[%d x <%d x ", size, size);//Kaymon change this type to vector array.
#endif
	}
	else if (size > 1)
	    p += sprintf(p, "<%d x ", size);



	p += sprintf(p, "%s", getBasicString());


#ifndef NewMatrix
	if(matrix || size > 1)//prevously
	  p += sprintf(p, ">"); 
#else
	if( matrix )
	  p += sprintf(p, ">]"); //Kaymon
	else if(  size > 1)
	  p += sprintf(p, ">"); 
#endif
	if(array)
	    p += sprintf(p, "]");
	
	if( qualifier == EvqInOut || qualifier  == EvqOut) //For the parameter which qualifier is EvqInOut or EvqOut. Because pass its address into the function.
	  p += sprintf(p, "*");
	//printf("%d\n",qualifier);
        //printf("HHH : %d\n",GGG);fflush(stdout);
	return TString(buf);
    }
#ifdef NewMatrix
    TString getBasicTypeFromAggType()const{//get the basic unit type from array. e.g. [4 x float] => show float. [4 x <4 x float>] => show <4 x float>, Kaymon
      char buf[128];
      
      if(matrix)
	sprintf(buf,"<%d x %s>", size ,getBasicString() );
      else if(array)
	sprintf(buf,"%s", getBasicString() );
      
      return TString(buf);
    }
#endif

protected:
    void buildMangledName(TString&);
    int getStructSize() const;

	TBasicType type      : 6;
	TPrecision precision;
	TQualifier qualifier : 7;
	int size             : 8; // size of vector or matrix, not size of array
	unsigned int matrix  : 1;
	unsigned int array   : 1;

    int arraySize;

    TTypeList* structure;      // 0 unless this is a struct
    mutable int structureSize;
    int maxArraySize;
    TType* arrayInformationType;
	TString *fieldName;         // for structure field names
    TString *mangled;
	TString *typeName;          // for structure field type name
};

#endif // _TYPES_INCLUDED_
