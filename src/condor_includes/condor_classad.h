/***************************Copyright-DO-NOT-REMOVE-THIS-LINE**
 * CONDOR Copyright Notice
 *
 * See LICENSE.TXT for additional notices and disclaimers.
 *
 * Copyright (c)1990-1998 CONDOR Team, Computer Sciences Department, 
 * University of Wisconsin-Madison, Madison, WI.  All Rights Reserved.  
 * No use of the CONDOR Software Program Source Code is authorized 
 * without the express consent of the CONDOR Team.  For more information 
 * contact: CONDOR Team, Attention: Professor Miron Livny, 
 * 7367 Computer Sciences, 1210 W. Dayton St., Madison, WI 53706-1685, 
 * (608) 262-0856 or miron@cs.wisc.edu.
 *
 * U.S. Government Rights Restrictions: Use, duplication, or disclosure 
 * by the U.S. Government is subject to restrictions as set forth in 
 * subparagraph (c)(1)(ii) of The Rights in Technical Data and Computer 
 * Software clause at DFARS 252.227-7013 or subparagraphs (c)(1) and 
 * (2) of Commercial Computer Software-Restricted Rights at 48 CFR 
 * 52.227-19, as applicable, CONDOR Team, Attention: Professor Miron 
 * Livny, 7367 Computer Sciences, 1210 W. Dayton St., Madison, 
 * WI 53706-1685, (608) 262-0856 or miron@cs.wisc.edu.
****************************Copyright-DO-NOT-REMOVE-THIS-LINE**/

#ifndef __CLASSAD_PACKAGE_H__
#define __CLASSAD_PACKAGE_H__

#include "../condor_classad.V6/common.h"
#include "../condor_classad.V6/exprTree.h"
#include "../condor_classad.V6/matchClassad.h"
#include "../condor_classad.V6/classad_collection.h"
#include "../condor_classad.V6/cedar_io.h"
using namespace classad;

#include "condor_attributes.h"
#include "condor_adtypes.h"

void printClassAdExpr( ExprTree * );
void printClassAdValue( Value & );
ClassAd* getOldClassAd( Sock& );
bool getOldClassAd( Sock&, ClassAd& );

#endif
