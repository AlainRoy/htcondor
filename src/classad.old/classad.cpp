/***************************************************************
 *
 * Copyright (C) 1990-2007, Condor Team, Computer Sciences Department,
 * University of Wisconsin-Madison, WI.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License.  You may
 * obtain a copy of the License at
 * 
 *    http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ***************************************************************/

// AttrList.C
//
// Implementation of AttrList classes and AttrListList classes.
//

#include "condor_common.h"

# include "condor_debug.h"
# include "condor_ast.h"
# include "condor_registration.h"
# include "condor_attrlist.h"
# include "condor_attributes.h"
# include "condor_classad.h"
# include "condor_adtypes.h"
# include "MyString.h"
# include "condor_xml_classads.h"

static Registration regi;                   // this is the registration for 
                                            // the AttrList type names. It 
                                            // should be defined in the calling
                                            // procedure.

static	SortFunctionType SortSmallerThan;
static	void* SortInfo;
static  const char *empty_string = "";

#if defined(USE_XDR)
extern "C" int xdr_mywrapstring (XDR *, char **);
#endif

// useful when debugging (std* are macros)
FILE *__stdin__  = stdin;
FILE *__stdout__ = stdout;
FILE *__stderr__ = stderr;


//
// AdType Constructor.
//
AdType::AdType(const char *tempName)
{
    if(tempName == NULL)
    {                                       // if empty.
        name = new char[strlen("") + 1];
	if(!name)
        {
	    	EXCEPT("Out of memory!");
        }
	strcpy(name, "");
	number = -1;
    }
    else
    {
        name = new char[strlen(tempName) + 1];
	if(!name)
        {
            EXCEPT("Warning : you ran out of memory -- quitting !");
        }
	strcpy(name, tempName);
	number = regi.RegisterType(tempName);
    }   
}

//
// AdType Destructor. 
//
AdType::~AdType()
{
    if(name)
    {
        delete []name;
    }
}

//
// ClassAd constructors
//
OldClassAd::OldClassAd() : AttrList()
{
	myType = NULL;
	targetType = NULL;
	// SetRankExpr ("Rank = 0");
	// SetRequirements ("Requirements = TRUE");
}

#if 0 /* don't want to link with ProcObj class; we shouldn't need this */
OldClassAd::OldClassAd(class ProcObj* procObj) : AttrList(procObj)
{
	myType = NULL;
	targetType = NULL;
	// SetRankExpr ("Rank = 0");
	// SetRequirements ("Requirements = TRUE");
}
#endif

#if 0 // dont use CONTEXTs anymore
OldClassAd::OldClassAd(const CONTEXT* context) : AttrList((CONTEXT *) context)
{
	myType = NULL;
	targetType = NULL;
	if (!Lookup ("Requirements"))
	{
		SetRequirements ("Requirements = TRUE");
	}

	if (!Lookup ("Rank"))
	{
    	SetRankExpr ("Rank = 0");
	}
}
#endif

OldClassAd::
ClassAd(FILE* f, char* d, int& i, int &err, int &empty) 
  : AttrList(f, d, i, err, empty)
{
	myType = NULL;
	targetType = NULL;

	updateBoundVariables();
}

OldClassAd::OldClassAd(char* s, char d) : AttrList(s, d)
{
	myType = NULL;
	targetType = NULL;

	updateBoundVariables();
}

void
OldClassAd::updateBoundVariables() {
    ExprTree *tree;
    EvalResult *val;

    val = new EvalResult;
    if(val == NULL)
    {
        EXCEPT("Warning : you ran out of space -- quitting !");
    }

	// Make a parse tree that contains the variable MyType
    Parse("MyType", tree);
	// Evaluate this variable within the classad, to see if it
	// is defined.
    tree->EvalTree(this, val);

	if( myType ) {
		delete myType;
		myType = NULL;
	}

	// If it's not defined, we set the type to be blank
    if(!val || val->type!=LX_STRING)
    {
        myType = new AdType();               // undefined type.
        if(myType == NULL)
        {
            EXCEPT("Warning : you ran out of space");
		}
    }
	// otherwise it was defined, so we'll set the type to be what the
	// creator of the classad wants. Note that later on, we'll delete
	// the type from the attribute list. ('Delete("MyType")')
    else
    {
		myType = new AdType(val->s);
		if(myType == NULL)
		{
				EXCEPT("Warning : you ran out of space");
		}
    }
	delete tree;
	// I just added the next two lines: we were leaking memory 
	// because EvalResult may contain a string result that isn't
	// properly deleted if we don't call the destructor. Therefore,
	// I delete and recreate the EvalResult. --alain 23-Sep-2001
	delete val;
	val = new EvalResult;

	// Make a parse tree that contains the variable TargetType
    Parse("TargetType", tree);
	// Evaluate this variable within the classad, to see if it
	// is defined.
    tree->EvalTree(this, val);

	if( targetType ) {
		delete targetType;
		targetType = NULL;
	}

	// If it's not defined, we set the type to be blank
    if(!val || val->type!=LX_STRING)
    {
        targetType = new AdType();           // undefined type.
		if(targetType == NULL)
		{
			EXCEPT("Warning : you ran out of space");
		}
    }
	// otherwise it was defined, so we'll set the type to be what the
	// creator of the classad wants. Note that later on, we'll delete
	// the type from the attribute list. ('Delete("TargetType")')
    else
    {
		targetType = new AdType(val->s);
		if(targetType == NULL)
		{
			EXCEPT("Warning : you ran out of space");
		}
    }
    delete tree;

    if(val)
    {
        delete val;
    }

		// We no longer remove MyType and TargetType from the
		// attrlist, so they can be used in places such as
		// COLLECTOR_REQUIREMENTS.  Just beware that these values
		// should only be updated via the OldClassAd::SetXXX() functions.
	SetInvisible("MyType");
	SetInvisible("TargetType");
}

OldClassAd::OldClassAd(const OldClassAd& old) : AttrList((AttrList&) old)
{
	myType = NULL;
	targetType = NULL;

    if(old.myType)
    {
        this->myType = new AdType(old.myType->name);
		if(this->myType == NULL)
		{
			EXCEPT("Warning : you ran out of meomory");
		}
    }
    if(old.targetType)
    {
        this->targetType = new AdType(old.targetType->name);
		if(this->targetType == NULL)
		{
			EXCEPT("Warning : you ran out of meomory");
		}
	}
}

OldClassAd::~OldClassAd()
{
    if(associatedList)
    {
		associatedList->associatedAttrLists->Delete(this);
    }
    if(myType)
    {
        delete myType;
    }
    if(targetType)
    {
        delete targetType;
    }
}

OldClassAd& OldClassAd::operator=(const OldClassAd& other)
{
	if (this != &other) {
		// First, let the base class do its magic.
		AttrList::operator=(other);

		// Clean up memory that we're going to be copying over.
		if (myType != NULL) {
			delete myType;
			myType = NULL;
		}
		if (targetType != NULL) {
			delete targetType;
			targetType = NULL;
		}

		if (other.myType) {
			myType = new AdType(other.myType->name);
			if (myType == NULL) {
				EXCEPT("Warning : you ran out of meomory");
			}
		}
		if(other.targetType) {
			targetType = new AdType(other.targetType->name);
			if (targetType == NULL) {
				EXCEPT("Warning : you ran out of meomory");
			}
		}
	}
	return *this;
}

//
// This member function of class AttrList sets myType name.
//
void OldClassAd::SetMyTypeName(const char *tempName)
{
	if(!tempName)
	{
		if(myType)
		{
			delete myType;
		}
		myType = NULL;
		return;
	}
    if(myType)
    {
        delete myType;
    }
    myType = new AdType(tempName);
    if(!myType)
    {
        EXCEPT("Warning : you ran out of memory -- quitting !");
    }
		// Also set the corresponding attribute in the attrlist.
		// Note that we call GetMyTypeName() instead of passing
		// tempName directly, because of extra hackery that happens
		// in that function.
	Assign("MyType",GetMyTypeName());
	SetInvisible("MyType");
}

//
// This member function of class AttrList returns myType name.
//
const char *OldClassAd::GetMyTypeName()
{
    if(!myType)
    {
		return empty_string;
    }
    else
    {
		// We should just return myType->name here.  Instead, we have
		// a dreadful hack:  If the ad claims to be a SCHEDD_ADTYPE,
		// we see if it has a ATTR_NUM_USERS attribute.  If it does not,
		// then it is really a SUBMITTER_ADTYPE and that is what we 
		// return.  We massage the name here because in Condor v6.3.x
		// it is important to distinguish between the two types of
		// ads (because the negotiator now fetches all ads with the
		// ANY_AD query).  However, we do not *really* set Submittor
		// ads to SUBMITTER_ADTYPE.... this should have been done
		// right from the start in v6.0, but it was not.  And if we
		// do it now, then there will be lots of backwards compatibility
		// and flocking problems between v6.3 and older versions.
		// Sigh.  Yet another hack in order to preserve backwards
		// compatibility.  The sins of the father... <tannenba 9/14/01>
		if ( strcmp(SCHEDD_ADTYPE,myType->name) == 0 ) {
			if ( !Lookup(ATTR_NUM_USERS) ) {
				return SUBMITTER_ADTYPE;
			}
		}
        return myType->name;
    }
}

//
// This member function of class AttrList sets targetType name.
//
void OldClassAd::SetTargetTypeName(const char *tempName)
{
	if(!tempName)
	{
		if(targetType)
		{
			delete targetType;
		}
		targetType = NULL;
		return;
	}
    if(targetType)
    {
        delete targetType;
    }
    targetType = new AdType(tempName);
    if(!targetType)
    {
        EXCEPT("Warning : you ran out of memory -- quitting !");
    }
		// Store the value in the attrlist too.
	Assign("TargetType",tempName);
	SetInvisible("TargetType");
}

//
// This member function of class AttrList returns targetType name.
//
const char *OldClassAd::GetTargetTypeName()
{
    if(!targetType)
    {
        return empty_string;
    }
    else
    {
        return targetType->name;
    }
}

//
// This member function of class AttrList returns myType number.
//
int OldClassAd::GetMyTypeNumber()
{
    if(!myType)
    {
        return -1;                            // undefined type.
    }
    else
    {
        return myType->number;
    }
}

//
// This member function of class AttrList returns targetType number.
//
int OldClassAd::GetTargetTypeNumber()
{
    if(!targetType)
    {
        return -1;                            // undefined type.
    }
    else
    {
        return targetType->number;
    }
}


// Requirement expression management functions
#if 0
int OldClassAd::
SetRequirements (char *expr)
{
	ExprTree *tree;
	int result = Parse (expr, tree);
	if (result != 0)
	{
		delete tree;
		return -1;		
	}
	SetRequirements (tree);	
	return 0;
}

void OldClassAd::
SetRequirements (ExprTree *tree)
{
	if (!AttrList::Insert (tree))
	{
		AttrList::UpdateExpr (tree);
		delete tree;
	}
}
#endif

ExprTree *OldClassAd::
GetRequirements (void)
{
	return Lookup (ATTR_REQUIREMENTS);
}

//
// Implementation of rank expressions is same as the requirement --RR
//
#if 0
int OldClassAd::
SetRankExpr (char *expr)
{
    ExprTree *tree;
    int result = Parse (expr, tree);
    if (result != 0)
    {
        delete tree;
        return -1;     
    }
    SetRankExpr (tree); 
    return 0;
}

void OldClassAd::
SetRankExpr (ExprTree *tree)
{
	if (!AttrList::Insert (tree))
	{
		AttrList::UpdateExpr (tree);
		delete tree;
	}
}
#endif

ExprTree *OldClassAd::
GetRankExpr (void)
{
    return Lookup (ATTR_RANK);
}


//
// Set and get sequence numbers --- stored in the attrlist
//
void OldClassAd::
SetSequenceNumber (int num)
{
	seq = num;
}


int OldClassAd::
GetSequenceNumber (void)
{
	return seq;
}


//
// This member function tests whether two ClassAds match mutually.
//

// Parsing a classad from a string is slow.  Really slow.  We match
// ads frequently, so cache the "MY.Requirements" tree so we only
// need to parse it once.

ExprTree *reqsTree = 0;

int OldClassAd::IsAMatch(OldClassAd* temp)
{
    EvalResult *val;

    if(!temp)
    {
        return 0;
    }

    if( (GetTargetTypeNumber()==temp->GetMyTypeNumber()) || !stricmp(GetTargetTypeName(),ANY_ADTYPE) ) {
        /* MY.TargetType matches TARGET.MyType */
    } else {
        return 0;
    }

    if( (GetMyTypeNumber()==temp->GetTargetTypeNumber()) || !stricmp(temp->GetTargetTypeName(),ANY_ADTYPE) ) {
        /* TARGET.TargetType matches MY.MyType */
    } else {
        return 0;
    }

    val = new EvalResult;
    if(val == NULL)
    {
        EXCEPT("Warning : you ran out of memory -- quitting !");
    }

    if (reqsTree == 0) Parse("MY.Requirements", reqsTree);       // convention.

    reqsTree->EvalTree(this, temp, val);         // match in one direction.
    if(!val || val->type != LX_INTEGER)
    {
	delete val;
	return 0;
    }
    else
    {
      if(!val->i)
      {
	  delete val;
	  return 0; 
      }
    }

    reqsTree->EvalTree(temp, this, val);      // match in the other direction.
    if(!val || val->type != LX_INTEGER)
    {
	delete val;
	return 0;
    }
    else
    {
        if(!val->i)
	{
	    delete val;
	    return 0; 
        }
    }  

    delete val;
    return 1;   
}

bool operator== (OldClassAd &lhs, OldClassAd &rhs)
{
	return (lhs >= rhs && rhs >= lhs);
}


bool operator<= (OldClassAd &lhs, OldClassAd &rhs)
{
	return (rhs >= lhs);
}


bool operator>= (OldClassAd &lhs, OldClassAd &rhs)
{
	EvalResult *val;	
	
	if( (lhs.GetMyTypeNumber()!=rhs.GetTargetTypeNumber()) &&
	    stricmp(rhs.GetTargetTypeName(),ANY_ADTYPE) )
	{
		return false;
	}

	if ((val = new EvalResult) == NULL)
	{
		EXCEPT("Out of memory -- quitting");
	}

	if (reqsTree == 0) Parse ("MY.Requirements", reqsTree);
	reqsTree -> EvalTree (&rhs, &lhs, val);
	if (!val || val->type != LX_INTEGER)
	{
		delete val;
		return false;
	}
	else
	if (!val->i)
	{
		delete val;
		return false;
	}

	delete val;
	return true;
}

int OldClassAd::fPrintAsXML(FILE* f)
{
	if(!f)
	{
		return FALSE;
	}

	MyString out;
	sPrintAsXML(out);
	fprintf(f, "%s", out.Value());
	return TRUE;
}

	
////////////////////////////////////////////////////////////////////////////////
// print the whole ClassAd into a file. The expressions are in infix notation.
// Returns FALSE if the file pointer is NULL; TRUE otherwise.
////////////////////////////////////////////////////////////////////////////////
int OldClassAd::fPrint(FILE* f)
{
	if(!f)
	{
		return FALSE;
	}

    fprintf(f, "MyType = ");
    fprintf(f, "%c", '"');
	if(GetMyTypeName())
	{
		fprintf(f, "%s", GetMyTypeName());
	}
    fprintf(f, "%c\n", '"');
    fprintf(f, "TargetType = ");
    fprintf(f, "%c", '"');
	if(GetMyTypeName())
	{
		fprintf(f, "%s", GetTargetTypeName());
	}
    fprintf(f, "%c\n", '"');    
	
	return AttrList::fPrint(f);
}

int OldClassAd::sPrintAsXML(MyString &output)
{
	ClassAdXMLUnparser  unparser;
	MyString            xml;
	unparser.SetUseCompactSpacing(false);
	unparser.Unparse(this, xml);
	output += xml;
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
// Append a ClassAd to a string.
////////////////////////////////////////////////////////////////////////////////
int OldClassAd::sPrint(MyString &output)
{
	output += "MyType = \"";
	if(GetMyTypeName())
	{
		output += GetMyTypeName();
	}
	output += "\"\nTargetType = \"";
	if(GetMyTypeName())
	{
		output += GetTargetTypeName();
	}
	output += "\"\n";
	
	return AttrList::sPrint(output);
}

////////////////////////////////////////////////////////////////////////////////
// print the whole ClassAd to the given debug level. The expressions
// are in infix notation.  
////////////////////////////////////////////////////////////////////////////////
void
OldClassAd::dPrint(int level)
{
	const char* foo;
	int flag = D_NOHEADER | level;
	foo = GetMyTypeName();
	if( foo ) {
		dprintf( flag, "MyType = \"%s\"\n", foo );
	} else {
		dprintf( flag, "MyType = \"\"\n" );
	}

	foo = GetTargetTypeName();
	if( foo ) {
		dprintf( flag, "TargetType = \"%s\"\n", foo );
	} else {
		dprintf( flag, "TargetType = \"\"\n" );
	}

	AttrList::dPrint( level );
}


// shipping functions for ClassAd -- added by Lei Cao

int OldClassAd::put(Stream& s)
{

	// first send over all the attributes
	if ( !AttrList::put(s) ) {
		return 0;
	}

	// send the types; if a type does not exist, send "(unknown type)" instead
	{
		char *adType = "(unknown type)";
    	if(myType && myType->name)
		{
			if (!s.code(myType->name))
				return 0;
		}
		else
		if (!s.code (adType))
			return 0;

		if(targetType && targetType->name)
		{
    		if(!s.code(targetType->name))
        		return 0;
		}
		else
		if (!s.code(adType))
			return 0;
	}


    return 1;
}


void
OldClassAd::clear( void )
{
		// First, clear out everything in our AttrList
	AttrList::clear();

		// Now, clear out our Type fields, since those are specific to
		// ClassAd and aren't handled by AttrList::clear().
    if( myType ) {
        delete myType;
		myType = NULL;
    }
    if( targetType ) {
        delete targetType;
		targetType = NULL;
    }
}

bool
OldClassAd::initFromString(char const *str,MyString *err_msg)
{
	if( !AttrList::initFromString(str,err_msg) ) {
		return false;
	}

	updateBoundVariables();
    return true; // is this correct?
}


int
OldClassAd::initFromStream(Stream& s)
{
	char *name  = NULL;

		// First, initialize ourselves from the stream.  This will
		// delete any existing attributes in the list...
	if ( !AttrList::initFromStream(s) ) {
		return 0;
	}

		// Now, if there's also type info on the wire, read that,
		// too. 
    if(!s.code(name)) {
		dprintf(D_FULLDEBUG,"Failed to read ClassAd type.\n");
        return 0;
    }
    SetMyTypeName(name);
	if ( name != NULL ) {
		free(name);
		name = NULL;
	}

    if(!s.code(name)) {
		dprintf(D_FULLDEBUG,"Failed to read ClassAd target type.\n");
        return 0;
    }
    SetTargetTypeName(name);
	if ( name != NULL ) {
		free(name);
		name = NULL;
	}

    return 1; 
}


#if defined(USE_XDR)
int OldClassAd::put (XDR *xdrs)
{
	char*	tmp = NULL;

	xdrs->x_op = XDR_ENCODE;

	if (!AttrList::put (xdrs))
		return 0;

	if(myType)
	{
		if (!xdr_mywrapstring (xdrs, &myType->name))
			return 0;
	}
	else
	{
		if (!xdr_mywrapstring (xdrs, &tmp))
			return 0;
	}

	if(targetType)
	{
		if (!xdr_mywrapstring (xdrs, &targetType->name))
			return 0;
	}
	else
	{
		if (!xdr_mywrapstring (xdrs, &tmp))
			return 0;
	}

	return 1;
}


int OldClassAd::get (XDR *xdrs)
{
	char buf[100];
	char *line = buf;

	if (!line) return 0;

	xdrs->x_op = XDR_DECODE;

	if (!AttrList::get (xdrs)) 
		return 0;

	if (!xdr_mywrapstring (xdrs, &line)) 
		return 0;

	SetMyTypeName (line);

	if (!xdr_mywrapstring (xdrs, &line)) 
		return 0;

	SetTargetTypeName (line);

	return 1;
}
#endif

void OldClassAd::
ExchangeExpressions (OldClassAd *ad)
{
    AttrListElem *tmp1;
    AttrListList *tmp2;
    int           tmp3;
	HashTable<YourString,AttrListElem *> *tmpHash;

    // exchange variables which maintain the attribute list
    // see condor_attrlist.h  --RR

#   define SWAP(a,b,t) {t=a; a=b; b=t;}

    SWAP(associatedList, ad->associatedList, tmp2); // this is AttrListList*
    SWAP(exprList, ad->exprList, tmp1);             // these are AttrListElem*
	SWAP(hash, ad->hash, tmpHash);
    SWAP(tail, ad->tail, tmp1);
    SWAP(ptrExpr, ad->ptrExpr, tmp1);
    SWAP(ptrName, ad->ptrName, tmp1);
    SWAP(seq, ad->seq, tmp3);                      // this is an int

    // undefine macro to decrease name-space pollution
#   undef SWAP

    return;
}

OldClassAd* OldClassAdList::Lookup(const char* name)
{
	AttrList*	list;

	((AttrListList*)this)->Lookup(name, list);
	return (ClassAd*)list;
}

void OldClassAdList::
Sort(int(*SmallerThan)(AttrList*, AttrList*, void*), void* info)
{
/*
	dprintf(D_ALWAYS,"head=%08x , tail=%08x\n",head,tail);
	int count=1;
	for (AttrListAbstract* xx=head; xx; xx=xx->next, count++ ) {
	  dprintf(D_ALWAYS, "%02d: prev=%08x , cur=%08x , next=%08x\n",count,xx->prev,xx,xx->next);
    }
*/

	Sort(SmallerThan, info, head);

/*
	dprintf(D_ALWAYS,"head=%08x , tail=%08x\n",head,tail);
	count=1;
	for (AttrListAbstract* xx=head; xx; xx=xx->next, count++ ) {
	  dprintf(D_ALWAYS, "%02d: prev=%08x , cur=%08x , next=%08x\n",count,xx->prev,xx,xx->next);
    }
*/

}

int OldClassAdList::
SortCompare(const void* v1, const void* v2)
{
	AttrListAbstract** a1 = (AttrListAbstract**)v1;
	AttrListAbstract** b1 = (AttrListAbstract**)v2;
	AttrListAbstract *abstract_ad1 = *a1;
	AttrListAbstract *abstract_ad2 = *b1;
	AttrList* a;
	AttrList* b;

	// Convert AttrListAbstracts to AttrList
	if ( abstract_ad1->Type() == ATTRLISTENTITY ) {
		// this represents an AttrList in one AttrListList
		a = (AttrList *)abstract_ad1;
	} else {
		// this represents an AttrList in multiple AttrListLists
		// thus, it is an AttrListRep not an AttrList
		a = (AttrList *)((AttrListRep *)abstract_ad1)->GetOrigAttrList();
	}

	if ( abstract_ad2->Type() == ATTRLISTENTITY ) {
		// this represents an AttrList in one AttrListList
		b = (AttrList *)abstract_ad2;
	} else {
		// this represents an AttrList in multiple AttrListLists
		// thus, it is an AttrListRep not an AttrList
		b = (AttrList *)((AttrListRep *)abstract_ad2)->GetOrigAttrList();
	}
	// The user supplied SortSmallerThan() func returns a 1
	// if a is smaller than b, and that is _all_ we know about
	// SortSmallerThan().  Some tools implement a SortSmallerThan()
	// that returns a -1 on error, some just return a 0 on error,
	// it is chaos.  Just chaos I tell you.  _SO_ we only check for
	// a "1" if it is smaller than, and do not assume anything else.
	// qsort() wants a -1 for smaller.
	if ( SortSmallerThan(a,b,SortInfo) == 1 ) {
			// here we know that a is less than b
		return -1;
	} else {
			// So, now we know that a is not smaller than b, but
			// we still need to figure out if a is equal to b or not.
			// Do this by calling the user supplied compare function
			// again and ask if b is smaller than a.
		if ( SortSmallerThan(b,a,SortInfo) == 1 ) {
				// now we know that a is greater than b
			return 1;
		} else {
				// here we know a is not smaller than b, and b is not
				// smaller than a.  so they must be equal.
			return 0;
		}
	}
}

void OldClassAdList::
Sort( SortFunctionType UserSmallerThan, void* UserInfo, 
		AttrListAbstract*& listHead)
{
	AttrListAbstract* ad;
	int i;
	int len = MyLength();

	if ( len < 2 ) {
		// the list is either empty or has only one element,
		// thus it is already sorted.
		return;
	}

	// what we have is a linked list we want to sort quickly.
	// so we stash pointers to all the elements into an array and qsort.
	// then we fixup all the head/tail/next/prev pointers.

	// so first create our array
	AttrListAbstract** array = new AttrListAbstract*[len];
	ad = listHead;
	i = 0;
	while (ad) {
		array[i++] = ad;
		ad = ad->next;
	}
	ASSERT(i == len);

	// now sort it.  Note: since we must use static members, Sort() is
	// _NOT_ thread safe!!!
	SortSmallerThan = UserSmallerThan;	
	SortInfo = UserInfo;
	qsort(array,len,sizeof(AttrListAbstract*),SortCompare);

	// and finally fixup the order of the double linked list
	listHead = ad = array[0];
	ad->prev = NULL;
	for (i=1;i<len;i++) {
		ad->next = array[i];
		array[i]->prev = ad;
		ad = array[i];
	}
	tail = ad;
	tail->next = NULL;

	// and delete our array
	delete [] array;
}

OldClassAd* OldClassAd::FindNext()
{
	return (OldClassAd*)next;
}
