/*
 * Copyright 2008 Red Hat, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "condor_common.h"

#include "Utils.h"

#include "condor_classad.h"

#include "condor_debug.h"

static const std::string EXPR_TYPE = "com.redhat.grid.Expression";
static const std::string TYPEMAP_KEY = "!!descriptors";

using namespace std;
using namespace qpid::types;

// cleans up the quoted values from the job log reader
string TrimQuotes(const char* str) {
	string val = str;

	size_t endpos = val.find_last_not_of("\\\"");
	if( string::npos != endpos ) {
		val = val.substr( 0, endpos+1 );
	}
	size_t startpos = val.find_first_not_of("\\\"");
	if( string::npos != startpos ) {
		val = val.substr( startpos );
	}

	return val;
}

// validate that an incoming group/user name is
// alphanumeric, underscores, or a dot separator
bool IsValidGroupUserName(const std::string& _name, std::string& _text) {
	const char* ptr = _name.c_str();
	while( *ptr ) {
		char c = *ptr++;
		if (	('a' > c || c > 'z') &&
			('A' > c || c > 'Z') &&
			('0' > c || c > '9') &&
			(c != '_' ) &&
			(c != '.' ) ) {
			_text = "Invalid name for group/user - alphanumeric, underscore and dot characters only";
			return false;
		}
	}
	return true;
}

// validate that an incoming attribute name is
// alphanumeric, or underscores
bool IsValidAttributeName(const std::string& _name, std::string& _text) {
	const char* ptr = _name.c_str();
	while( *ptr ) {
		char c = *ptr++;
		if (	('a' > c || c > 'z') &&
			('A' > c || c > 'Z') &&
			('0' > c || c > '9') &&
			(c != '_' ) ) {
			_text = "Invalid name for attribute - alphanumeric and underscore characters only";
			return false;
		}
	}
	return true;
}

bool CheckRequiredAttrs(ClassAd& ad, const char* attrs[], std::string& missing) {
	bool status = true;
	int i = 0;

        while (NULL != attrs[i]) {
		if (!ad.Lookup(attrs[i])) {
			missing += " "; missing += attrs[i];
			status = false;
		}
		i++;
	}
	return status;
}

bool
AddAttribute(ClassAd &ad, const char *name, Variant::Map &job)
{
	ExprTree *expr;
	Variant::Map* descriptors = NULL;
	Variant::Map::iterator it = job.find(TYPEMAP_KEY);
	if (it != job.end()) {
		descriptors =  &((it->second).asMap());
	}

		// All these extra lookups are horrible. They have to
		// be there because the ClassAd may have multiple
		// copies of the same attribute name! This means that
		// the last attribute with a given name will set the
		// value, but the last attribute tends to be the
		// attribute with the oldest (wrong) value. How
		// annoying is that!
	if (!(expr = ad.Lookup(name))) {
		dprintf(D_FULLDEBUG, "Warning: failed to lookup attribute '%s' from ad\n", name);
		return false;
	}

	char* value = NULL;
	switch (expr->RArg()->MyType()) {
		case LX_BOOL:
		case LX_EXPR:
			{
				if (!descriptors) {
					// start a new type map
					descriptors = new Variant::Map();
					(*descriptors)[name] = EXPR_TYPE;
					// deep copy
					job[TYPEMAP_KEY] = *descriptors;
					delete descriptors;
				}
				else {
					(*descriptors)[name] = EXPR_TYPE;
				}
				expr->RArg()->PrintToNewStr(&value);
				job[name] = TrimQuotes(value);
			}
			break;
		case LX_INTEGER:
			job[name] = ((Integer*)expr->RArg())->Value();
			break;
		case LX_FLOAT:
			job[name] = ((Float*)expr->RArg())->Value();
			break;
		case LX_FUNCTION:
			{
				Function* func = (Function*)expr->RArg();
				func->PrintToNewStr(&value);
				job[name] = TrimQuotes(value);
			}
			break;
		case LX_STRING:
		default:
			expr->RArg()->PrintToNewStr(&value);
			job[name] = TrimQuotes(value);
	}

	if (value) {
		free(value);
	}

	return true;
}

bool
PopulateVariantMapFromAd(ClassAd &ad, Variant::Map &_map)
{
	ExprTree *expr;

	ad.ResetExpr();
	while (NULL != (expr = ad.NextExpr())) {
		if (!AddAttribute(ad, ((Variable *) expr->LArg())->Name(), _map)) {
			return false;
		}
	}

	// TODO: debug
//	if (DebugFlags & D_FULLDEBUG) {
//		ad.dPrint(D_FULLDEBUG|D_NOHEADER);
//	}

	return true;
}


bool
PopulateAdFromVariantMap(Variant::Map &_map, ClassAd &ad)
{
	Variant::Map* descriptors = NULL;
	// grab the descriptor map if there is one
	Variant::Map::iterator it = _map.find(TYPEMAP_KEY);
	if (it != _map.end()) {
		descriptors =  &((it->second).asMap());
	}

	for (Variant::Map::const_iterator entry = _map.begin(); _map.end() != entry; entry++) {
		const char* name = entry->first.c_str();
		Variant value = _map[entry->first];

		// skip the hidden tag
		if (0 == strcmp(name,EXPR_TYPE.c_str())) {
			break;
		}

		// first see if the value is a expression
		// as indicated by the typemap
		if (descriptors &&
			(it = descriptors->find(entry->first)) != descriptors->end() &&
			it->second == EXPR_TYPE) {
			ad.AssignExpr(entry->first.c_str(), value.asString().c_str());
		} // TODO: Report ignored types from descriptors map
		else {
			switch (value.getType()) {
				case VAR_UINT8:
					ad.Assign(name, value.asUint8());
					break;
				case VAR_UINT16:
					ad.Assign(name, value.asUint16());
					break;
				case VAR_UINT32:
				case VAR_UINT64:
					ad.Assign(name, value.asUint32());
					break;
				case VAR_INT8:
					ad.Assign(name, value.asInt8());
					break;
				case VAR_INT16:
					ad.Assign(name, value.asInt16());
					break;
				case VAR_INT32:
				case VAR_INT64:
					ad.Assign(name, value.asInt32());
					break;
				case VAR_FLOAT:
					ad.Assign(name, value.asFloat());
					break;
				case VAR_DOUBLE:
					ad.Assign(name, value.asDouble());
					break;
				case VAR_BOOL:
				case VAR_UUID:
				case VAR_STRING:
					ad.Assign(name, value.asString().c_str());
					break;
				default:
					dprintf(D_FULLDEBUG, "Warning: Unknown/unsupported Variant type in map for attribute '%s'\n", name);
			}
		}
	}

	if (!descriptors) {
		dprintf(D_FULLDEBUG,"Warning - no type map found in this Variant::Map. Continuing...\n");
	}

	// TODO: debug
//	if (DebugFlags & D_FULLDEBUG) {
//		ad.dPrint(D_FULLDEBUG|D_NOHEADER);
//	}

	return true;
}
