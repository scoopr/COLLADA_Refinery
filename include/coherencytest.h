/*
 * Copyright 2006 Sony Computer Entertainment Inc.
 *
 * Licensed under the SCEA Shared Source License, Version 1.0 (the "License"); you may not use this 
 * file except in compliance with the License. You may obtain a copy of the License at:
 * http://research.scea.com/scea_shared_source_license.html
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License 
 * is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
 * implied. See the License for the specific language governing permissions and limitations under the 
 * License. 
 */
#ifndef _COHERENCYTEST_H_
#define _COHERENCYTEST_H_

#include "conditionerBase.h"

// Collada API includes

#include "dae.h" 
#include "dae/daeSIDResolver.h"
#include "dom/domTypes.h"
#include "dom/domCOLLADA.h"
#include "dom/domConstants.h"
#include "dom/domElements.h"
#include "dom/domProfile_GLES.h"
#include "dom/domProfile_COMMON.h"
#include "dom/domFx_include_common.h"
#include "libxml/xmlschemas.h"
#include "libxml/schemasInternals.h"
#include "libxml/schematron.h"
#include "libxml/xmlreader.h"
#include "defines.h"
#include <map>
#include <set>
#include <string>
#include <vector>
using namespace std;

#define MAX_NAME_SIZE 512 

struct _xmlDoc;

class Coherencytest : public ConditionerBase
{
public:
	REF_EXPORT bool init();
	REF_EXPORT int execute();

	REF_EXPORT std::string getBaseName() const { return "coherencytest"; }
	REF_EXPORT std::string getDescription() const { return "COLLADA dae document coherency test"; }
	REF_EXPORT std::string getCategory() const  { return "Misc."; }
	void PRINTF(const char * log);

private:
	void print_name_id(domElement * element);
	domUint CHECK_error(domElement * element, bool b,  const char * message= NULL);
	void CHECK_warning(domElement * element, bool b, const char *message = NULL);
	domUint CHECK_uri(xsAnyURI & uri);
	domUint CHECK_count(domElement * element, domInt expected, domInt result, const char *message = NULL);
	bool CHECK_fileexist(const char * filename);
	domUint CHECK_file(domElement *element, xsAnyURI & fileuri);
	domUint	GetMaxOffsetFromInputs(domInputLocalOffset_Array & inputs);
	domUint CHECK_Triangles(domTriangles *triangles);
	domUint CHECK_Polygons(domPolygons *polygons);
	domUint CHECK_Polylists(domPolylist *polylist);
	domUint CHECK_Tristrips(domTristrips *tristrips);
	domUint CHECK_Trifans(domTrifans *trifans);
	domUint CHECK_Lines(domLines *lines);
	domUint CHECK_Linestrips(domLinestrips *linestrips);
	domUint CHECK_Geometry(domGeometry *geometry);
	domUint CHECK_InstanceGeometry(domInstance_geometry * instance_geometry);
	domUint CHECK_Controller(domController *controller);
	domUint CHECK_InstanceElementUrl(daeDatabase *db, const char * instance_element);
	domUint GetSizeFromType(xsNMTOKEN type);
	domUint CHECK_Source(domSource * source);
//	void _XMLSchemaValidityErrorFunc(void* ctx, const char* msg, ...);
//	void _XMLSchemaValidityWarningFunc(void* ctx, const char* msg, ...);
	domUint CHECK_validateDocument(_xmlDoc *LXMLDoc);
	domUint CHECK_xmlfile(daeString filename);

	domUint CHECK_counts (DAE *input, int verbose = 0);
	domUint CHECK_links (DAE *input, int verbose = 0);
	domUint CHECK_files (DAE *input, int verbose = 0);
	domUint CHECK_unique_id (DAE *input, int verbose = 0);
	domUint CHECK_texture (DAE *input, int verbose = 0);
	domUint CHECK_schema (DAE *input, int verbose = 0);
	domUint CHECK_skin (DAE *input, int verbose = 0);
	domUint CHECK_inputs (domInputLocal_Array & inputs, const char * semantic);
	domUint CHECK_inputs (domInputLocalOffset_Array & inputs, const char * semantic);

	string file_name;
	FILE * file;
};

class CoherencytestProxy : public ConditionerCreator
{
	REF_EXPORT std::string getBaseName() const { return "coherencytest"; }
	REF_EXPORT std::string getDescription() const { return "COLLADA dae document coherency test"; }
	REF_EXPORT std::string getCategory() const  { return "Misc."; }
	REF_EXPORT Conditioner *create() const { return new Coherencytest(); }
};

#endif
