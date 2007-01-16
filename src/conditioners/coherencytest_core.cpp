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
#include "coherencytest.h"
#define MAX_PATH 260

#include "libxml/xmlschemas.h"
#include "libxml/schemasInternals.h"
#include "libxml/schematron.h"
#include "iconv.h"

Coherencytest * globalpointer;
//#define PRINTF if(VERBOSE) printf
#define MAX_LOG_BUFFER 1024
void Coherencytest::PRINTF(const char * log)
{
	printExecutionMessage(std::string(log));
	printf(log);
	if (file)
		fwrite(log, sizeof(char), strlen(log), file);
}
int VERBOSE;

// check links
// check unique id
// check file path
// check counts
// check schema
// check texture

bool Coherencytest::init()
{
	addStringOption( "log", "log file option", "log file for test result", "", true );
	return true;
}

int Coherencytest::execute()
{
	file = NULL;
	getStringOption("log", file_name);
	if (file_name == "") //use default name 
	{
		string input = getInput(0);
		string temp = input.substr(1,input.length()-5);
		file_name = temp + ".log";
	}

	file = fopen(file_name.c_str(), "w+");

	//VERBOSE = verbose;
	domUint errorcount = 0;
	errorcount += CHECK_schema(_dae);
	errorcount += CHECK_unique_id(_dae);
	errorcount += CHECK_counts(_dae);
	errorcount += CHECK_links(_dae);
	errorcount += CHECK_texture(_dae);
	errorcount += CHECK_files(_dae);
	errorcount += CHECK_skin(_dae);

	if (file) fclose(file);
//	return (int) errorcount;
	return 0;
}

void Coherencytest::print_name_id(domElement * element)
{
	domElement * e = element;
	while ( e->getID() == NULL)
		e = e->getParentElement();
	char temp[MAX_LOG_BUFFER];
	sprintf(temp, "(type=%s,id=%s)", e->getTypeName(), e->getID());
	PRINTF(temp);
}
domUint Coherencytest::CHECK_error(domElement * element, bool b,  const char * message)
{
	if (b == false) {
		PRINTF("CHECK_error   Failed ");
		print_name_id(element);
		if (message) PRINTF(message);
		return 1;
	}
	return 0;
}
void Coherencytest::CHECK_warning(domElement * element, bool b, const char *message)
{
	if (b == false) {
		PRINTF("CHECK_warning Failed ");
		print_name_id(element);
		if (message) PRINTF(message);
	}
}
domUint Coherencytest::CHECK_uri(xsAnyURI & uri)
{
	uri.resolveElement();
	if (uri.getElement() == NULL)
	{
		char temp[MAX_LOG_BUFFER];
		sprintf(temp, "CHECK_uri     Failed uri=%s not resolved\n",uri.getURI());
		PRINTF(temp);
		return 1;
	}
	return 0;
}
domUint Coherencytest::CHECK_count(domElement * element, domInt expected, domInt result, const char *message)
{
	if (expected != result)
	{
		char temp[MAX_LOG_BUFFER];
		sprintf(temp, "CHECK_count   Failed: expected=%d, result=%d", expected, result);
		PRINTF(temp);
		print_name_id(element);
		if (message) PRINTF(message);
		return 1;
	}
	return 0;
}
bool Coherencytest::CHECK_fileexist(const char * filename)
{
	xmlTextReader * reader = xmlReaderForFile(filename, 0, 0);
	if (!reader)
		return false;
	return true;
}
domUint Coherencytest::CHECK_file(domElement *element, xsAnyURI & fileuri)
{
	daeURI * uri = element->getDocumentURI();
	daeString TextureFilePrefix = uri->getFilepath();

	// Build a path using the scene name ie: images/scene_Textures/boy.tga
	daeChar newTexName[MAX_NAME_SIZE]; 	
	sprintf(newTexName, "%s%s", TextureFilePrefix, fileuri.getURI() ); 
	
	// Build a path for the Shared texture directory ie: images/Shared/boy.tga
	daeChar sharedTexName[MAX_NAME_SIZE];
	sprintf(sharedTexName, "%sShared/%s",TextureFilePrefix, fileuri.getFile() );

	if (!CHECK_fileexist(fileuri.getURI()))
		if(!CHECK_fileexist(newTexName))
			if(!CHECK_fileexist(sharedTexName))
			{
				char temp[MAX_LOG_BUFFER];
				sprintf(temp, "CHECK_file   failed, %s not found\n", fileuri.getURI());
				PRINTF(temp);
				return 1;
			}
	return 0;
}

domUint	Coherencytest::GetMaxOffsetFromInputs(domInputLocalOffset_Array & inputs)
{
	domUint maxoffset = 0;
	domUint count = (domUint) inputs.getCount();
	for(size_t i=0; i< count ;i++)
	{
		domUint thisoffset  = inputs[i]->getOffset();
		if (maxoffset < thisoffset) maxoffset++;
	}
	return maxoffset + 1;
}
domUint Coherencytest::CHECK_Triangles(domTriangles *triangles)
{
	domUint errorcount = 0;
	domUint count = triangles->getCount();
	domInputLocalOffset_Array & inputs = triangles->getInput_array();
	errorcount += CHECK_inputs(inputs, "VERTEX");
	domUint maxoffset = GetMaxOffsetFromInputs(inputs);
	domPRef p = triangles->getP();
	domListOfUInts & ints = p->getValue();

	// check count
	errorcount += CHECK_count(triangles, 3 * count * maxoffset, (domInt) ints.getCount(),
		                      "triangles, count doesn't match\n");
	return errorcount;
}
domUint Coherencytest::CHECK_Polygons(domPolygons *polygons)
{
	domUint errorcount = 0;
	domUint count = polygons->getCount();
	domInputLocalOffset_Array & inputs = polygons->getInput_array();
	errorcount += CHECK_inputs(inputs, "VERTEX");
	domUint maxoffset = GetMaxOffsetFromInputs(inputs);
	domP_Array & parray = polygons->getP_array();

	// check count
	errorcount += CHECK_count(polygons, count, (domInt) parray.getCount(),
		                      "polygons, count doesn't match\n");
	return errorcount;
}
domUint Coherencytest::CHECK_Polylists(domPolylist *polylist)
{
	domUint errorcount = 0;
	domUint count = polylist->getCount();
	domInputLocalOffset_Array & inputs = polylist->getInput_array();
	errorcount += CHECK_inputs(inputs, "VERTEX");
	domUint maxoffset = GetMaxOffsetFromInputs(inputs);
	domPRef p = polylist->getP();
	domPolylist::domVcountRef vcount = polylist->getVcount();

	// check vcount
	errorcount += CHECK_count(polylist, count, (domInt) vcount->getValue().getCount(),
		                      "polylists, count doesn't match\n");
	// check p count
	domUint vcountsum = 0;
	for (size_t i=0; i<count; i++)
	{
		vcountsum += vcount->getValue()[i];
	}
	errorcount += CHECK_count(polylist, (domInt) p->getValue().getCount(), vcountsum * maxoffset,
		                      "polylists, total vcount and p count doesn't match\n");
	return errorcount;
}
domUint Coherencytest::CHECK_Tristrips(domTristrips *tristrips)
{
	domUint errorcount = 0;
	domUint count = tristrips->getCount();
	domInputLocalOffset_Array & inputs = tristrips->getInput_array();
	errorcount += CHECK_inputs(inputs, "VERTEX");
	domUint maxoffset = GetMaxOffsetFromInputs(inputs);
	domP_Array & parray = tristrips->getP_array();

	// check vcount
	errorcount += CHECK_count(tristrips, count, (domInt) parray.getCount(),
		                      "tristrips, count doesn't match\n");
	// check p count
	for (size_t i=0; i<count; i++)
	{
		errorcount += CHECK_count(tristrips, 3 * maxoffset <= parray[i]->getValue().getCount(), 1,
			                      "tristrips, this p has less than 3 vertices\n");		
		errorcount += CHECK_count(tristrips, (domInt) parray[i]->getValue().getCount() % maxoffset, 0,
			                      "tristrips, this p count is not in multiple of maxoffset\n");		
	}
	return errorcount;
}
domUint Coherencytest::CHECK_Trifans(domTrifans *trifans)
{
	domUint errorcount = 0;
	domUint count = trifans->getCount();
	domInputLocalOffset_Array & inputs = trifans->getInput_array();
	errorcount += CHECK_inputs(inputs, "VERTEX");
	domUint maxoffset = GetMaxOffsetFromInputs(inputs);
	domP_Array & parray = trifans->getP_array();

	// check vcount
	errorcount += CHECK_count(trifans, count, (domInt) parray.getCount(),
		                      "trifan, count doesn't match\n");
	// check p count
	for (size_t i=0; i<count; i++)
	{
		errorcount += CHECK_count(trifans, 3 * maxoffset <= parray[i]->getValue().getCount(), 1,
			                      "trifan, this p has less than 3 vertices\n");		
		errorcount += CHECK_count(trifans, (domInt) parray[i]->getValue().getCount() % maxoffset, 0,
			                      "trifan, this p count is not in multiple of maxoffset\n");		
	}
	return errorcount;
}
domUint Coherencytest::CHECK_Lines(domLines *lines)
{
	domUint errorcount = 0;
	domUint count = lines->getCount();
	domInputLocalOffset_Array & inputs = lines->getInput_array();
	errorcount = CHECK_inputs(inputs, "VERTEX");
	domUint maxoffset = GetMaxOffsetFromInputs(inputs);
	domP * p = lines->getP();
	// check p count
	errorcount += CHECK_count(lines, 2 * count * maxoffset, (domInt) p->getValue().getCount(),
		                      "lines, count doesn't match\n");
	return errorcount;
}

domUint Coherencytest::CHECK_Linestrips(domLinestrips *linestrips)
{
	domUint errorcount = 0;
	domUint count = linestrips->getCount();
	domInputLocalOffset_Array & inputs = linestrips->getInput_array();
	errorcount += CHECK_inputs(inputs, "VERTEX");
	domUint maxoffset = GetMaxOffsetFromInputs(inputs);
	domP_Array & parray = linestrips->getP_array();

	// check p count
	errorcount += CHECK_count(linestrips, count, (domInt) parray.getCount(),
		                      "linestrips, count doesn't match\n");
	// check inputs
	for (size_t i=0; i<count; i++)
	{
		errorcount += CHECK_count(linestrips, 2 * maxoffset <= parray[i]->getValue().getCount(), 1,
			                  "linestrips, this p has less than 2 vertices\n");		
		errorcount += CHECK_count(linestrips, (domInt) parray[i]->getValue().getCount() % maxoffset, 0,
			                  "linestrips, this p is not in mutiple of maxoffset\n");		
	}
	return errorcount;
}

domUint Coherencytest::CHECK_Geometry(domGeometry *geometry)
{
	domUint errorcount = 0;
	domMesh * mesh = geometry->getMesh();
	if (mesh == NULL)
		return 0;

	// check vertices
	domVertices *vertices = mesh->getVertices();
	CHECK_error(geometry, vertices != NULL, "geometry, no vertices in this mesh\n");
	if (vertices)
	{
		domInputLocal_Array & inputs = vertices->getInput_array();
		errorcount += CHECK_inputs(inputs, "POSITION");
	}
	// triangles
	domTriangles_Array & triangles = mesh->getTriangles_array();
	for (size_t i=0; i<triangles.getCount(); i++)
	{
		errorcount += CHECK_Triangles(triangles[i]);
	}
	// polygons
	domPolygons_Array & polygons = mesh->getPolygons_array();
	for (size_t i=0; i<polygons.getCount(); i++)
	{
		errorcount += CHECK_Polygons(polygons[i]);
	}
	// polylist
	domPolylist_Array & polylists = mesh->getPolylist_array();
	for (size_t i=0; i<polylists.getCount(); i++)
	{
		errorcount += CHECK_Polylists(polylists[i]);
	}
	// tristrips
	domTristrips_Array & tristrips = mesh->getTristrips_array();
	for (size_t i=0; i<tristrips.getCount(); i++)
	{
		errorcount += CHECK_Tristrips(tristrips[i]);
	}
	// trifans
	domTrifans_Array & trifans = mesh->getTrifans_array();
	for (size_t i=0; i<trifans.getCount(); i++)
	{
		errorcount += CHECK_Trifans(trifans[i]);
	}
	// lines
	domLines_Array & lines = mesh->getLines_array();
	for (size_t i=0; i<lines.getCount(); i++)
	{
		errorcount += CHECK_Lines(lines[i]);
	}
	// linestrips
	domLinestrips_Array & linestrips = mesh->getLinestrips_array();
	for (size_t i=0; i<linestrips.getCount(); i++)
	{
		errorcount += CHECK_Linestrips(linestrips[i]);
	}
	return errorcount;
}

domUint Coherencytest::CHECK_InstanceGeometry(domInstance_geometry * instance_geometry)
{
	domUint errorcount = 0;
	// check material binding to geometry symbols
	std::set<string> material_symbols;
	domBind_material * bind_material = instance_geometry->getBind_material();
	if (bind_material == NULL) {
		PRINTF("CHECK_InstanceGeometry failed ");
		print_name_id(instance_geometry);
		PRINTF("\n  no bind materials in instance geometry \n");
		return 0;
	}
	if (bind_material->getTechnique_common() == NULL) {
		PRINTF("CHECK_InstanceGeometry failed ");
		print_name_id(instance_geometry);
		PRINTF("\n  no technique_common in bind materials \n");
		return 0;
	}
	domInstance_material_Array & imarray = bind_material->getTechnique_common()->getInstance_material_array();
	for (size_t i=0; i<imarray.getCount(); i++)
	{
		material_symbols.insert(string(imarray[i]->getSymbol()));		
	}
	xsAnyURI & uri = instance_geometry->getUrl();
	domGeometry * geometry = (domGeometry *) (domElement*) uri.getElement();
	if (geometry == NULL) return errorcount ;
	domMesh * mesh = geometry->getMesh();
	if (mesh) {
		domTriangles_Array & triangles = mesh->getTriangles_array();
		for (size_t i=0; i<triangles.getCount(); i++)
		{
			daeString material_group = triangles[i]->getMaterial();
			if (material_group)
				CHECK_warning(instance_geometry, 
						material_symbols.find(material_group) != material_symbols.end(),
						"binding not found for material symbol");
		}
		domPolygons_Array & polygons = mesh->getPolygons_array();
		for (size_t i=0; i<polygons.getCount(); i++)
		{
			daeString material_group = polygons[i]->getMaterial();
			if (material_group)
				CHECK_warning(instance_geometry, 
						material_symbols.find(material_group) != material_symbols.end(),
						"binding not found for material symbol");
		}
		domPolylist_Array & polylists = mesh->getPolylist_array();
		for (size_t i=0; i<polylists.getCount(); i++)
		{
			daeString material_group = polylists[i]->getMaterial();
			if (material_group)
				CHECK_warning(instance_geometry, 
						material_symbols.find(material_group) != material_symbols.end(),
						"binding not found for material symbol");
		}
		domTristrips_Array & tristrips = mesh->getTristrips_array();
		for (size_t i=0; i<tristrips.getCount(); i++)
		{
			daeString material_group = tristrips[i]->getMaterial();
			if (material_group)
				CHECK_warning(instance_geometry, 
						material_symbols.find(material_group) != material_symbols.end(),
						"binding not found for material symbol");
		}
		domTrifans_Array & trifans = mesh->getTrifans_array();
		for (size_t i=0; i<trifans.getCount(); i++)
		{
			daeString material_group = trifans[i]->getMaterial();
			if (material_group)
				CHECK_warning(instance_geometry, 
						material_symbols.find(material_group) != material_symbols.end(),
						"binding not found for material symbol");
		}
		domLines_Array & lines = mesh->getLines_array();
		for (size_t i=0; i<lines.getCount(); i++)
		{
			daeString material_group = lines[i]->getMaterial();
			if (material_group)
				CHECK_warning(instance_geometry, 
						material_symbols.find(material_group) != material_symbols.end(),
						"binding not found for material symbol");
		}
		domLinestrips_Array & linestrips = mesh->getLinestrips_array();
		for (size_t i=0; i<linestrips.getCount(); i++)
		{
			daeString material_group = linestrips[i]->getMaterial();
			if (material_group)
				CHECK_warning(instance_geometry, 
						material_symbols.find(material_group) != material_symbols.end(),
						"binding not found for material symbol");
		}
	}
	return 0;
}

domUint Coherencytest::CHECK_Controller(domController *controller)
{
	domUint errorcount = 0;
	domSkin * skin = controller->getSkin();
	if (skin)
	{
		xsAnyURI & uri = skin->getSource();

		// get vertex count from skin
		domSkin::domVertex_weights * vertex_weights = skin->getVertex_weights();
		domUint vertex_weights_count = vertex_weights->getCount();
		domGeometry * geometry = (domGeometry*) (domElement*) uri.getElement();
		domMesh * mesh = geometry->getMesh();				
		if (mesh)
		{	// get vertex count from geometry
			domVertices * vertices = mesh->getVertices();
			CHECK_error(geometry, vertices != NULL, "geometry, vertices in this mesh\n");
			if (vertices)
			{
				xsAnyURI src = vertices->getInput_array()[0]->getSource();
				domSource * source = (domSource*) (domElement*) src.getElement();
				domUint vertices_count = source->getTechnique_common()->getAccessor()->getCount();
				errorcount += CHECK_count(controller, vertices_count, vertex_weights_count,
										"controller, vertex weight count != mesh vertex count\n");
			}
		}	// TODO: it could be convex_mesh and spline
		domUint vcount_count = (domUint) vertex_weights->getVcount()->getValue().getCount();
		errorcount += CHECK_count(controller, vcount_count, vertex_weights_count,
			                      "controller, vcount count != vertex weight count\n");	
		domInputLocalOffset_Array & inputs = vertex_weights->getInput_array();
		domUint sum = 0;
		for (size_t i=0; i<vcount_count; i++)
		{
			sum += vertex_weights->getVcount()->getValue()[i];
		}
		errorcount += CHECK_count(controller, sum * inputs.getCount(), (domInt) vertex_weights->getV()->getValue().getCount(), 
			                      "controller, total vcount doesn't match with numbers of v\n");
	}	// TODO : we will support morph later
	return errorcount;
}

domUint Coherencytest::CHECK_InstanceElementUrl(daeDatabase *db, const char * instance_element) {
	domUint errorcount = 0;
	for (daeInt i=0; i<(daeInt)db->getElementCount(NULL, instance_element, getInput(0).c_str() ); i++)
	{
		domInstanceWithExtra *element;
		domInt error = db->getElement((daeElement**)&element, i, NULL, instance_element, getInput(0).c_str());
		if (error == DAE_OK) {
			errorcount += CHECK_uri(element->getUrl());
		}
	}
	return errorcount;
}

domUint Coherencytest::GetSizeFromType(xsNMTOKEN type)
{
	if (stricmp(type, "bool2")==0)
		return 2;
	else if (stricmp(type, "bool3")==0)
		return 3;
	else if (stricmp(type, "bool4")==0)
		return 4;
	else if (stricmp(type, "int2")==0)
		return 2;
	else if (stricmp(type, "int3")==0)
		return 3;
	else if (stricmp(type, "int4")==0)
		return 4;
	else if (stricmp(type, "float2")==0)
		return 2;
	else if (stricmp(type, "float3")==0)
		return 3;
	else if (stricmp(type, "float4")==0)
		return 4;
	else if (stricmp(type, "float2x2")==0)
		return 4;
	else if (stricmp(type, "float3x3")==0)
		return 9;
	else if (stricmp(type, "float4x4")==0)
		return 16;
	return 1;
}

domUint Coherencytest::CHECK_Source(domSource * source)
{
	domUint errorcount = 0;
	// check if this is a source with children
	daeTArray<daeSmartRef<daeElement> >  children;
	source->getChildren(children);
	if (children.getCount() <= 0) return 0;
	// prepare technique_common 
	domSource::domTechnique_common * technique_common = source->getTechnique_common();
	domAccessor * accessor = 0;
	domUint accessor_count = 0;
	domUint accessor_stride = 0;
	domUint accessor_size = 0;
	domUint array_count = 0;
	domUint array_value_count = 0;
	if (technique_common)
	{
		accessor = technique_common->getAccessor();
		if (accessor)
		{
			accessor_count = accessor->getCount();
			accessor_stride = accessor->getStride();
			domParam_Array & param_array = accessor->getParam_array();
			for(size_t i=0; i<param_array.getCount(); i++)
			{
				xsNMTOKEN type = param_array[i]->getType();
				accessor_size += GetSizeFromType(type);
			}
		}
	}

	// float_array
	domFloat_array * float_array = source->getFloat_array();
	if (float_array)
	{
		array_count = float_array->getCount(); 
		array_value_count = (domUint) float_array->getValue().getCount(); 
	}
	
	// int_array
	domInt_array * int_array = source->getInt_array();
	if (int_array)
	{
		array_count = int_array->getCount(); 
		array_value_count = (domUint) int_array->getValue().getCount(); 
	} 
	
	// bool_array
	domBool_array * bool_array = source->getBool_array();
	if (bool_array)
	{
		array_count = bool_array->getCount(); 
		array_value_count = (domUint) bool_array->getValue().getCount(); 
	}
	
	// idref_array
	domIDREF_array * idref_array = source->getIDREF_array();
	if (idref_array)
	{
		array_count = idref_array->getCount(); 
		array_value_count = (domUint) idref_array->getValue().getCount(); 
	}
	
	// name_array
	domName_array * name_array = source->getName_array();
	if (name_array)
	{
		array_count = name_array->getCount(); 
		array_value_count = (domUint) name_array->getValue().getCount(); 
	}  
	errorcount += CHECK_count(source, array_count, array_value_count,
			                    "array count != number of name in array value_count\n");
	if (accessor)
	{
		if (accessor_stride >= accessor_size)
		{
			errorcount += CHECK_count(source, array_count, accessor_count * accessor_size, 
								"accessor_stride >= accessor_size but array_count != accessor_count * accessor_size\n");
		} else {
			errorcount += CHECK_count(source, array_count, accessor_size + (accessor_stride * (accessor_count - 1)), 
			                    "accessor_stride < accessor_size but array_count != accessor_size + (accessor_stride * (accessor_count - 1))\n");	
		}
	}
	
	return errorcount ;
}

domUint Coherencytest::CHECK_counts (DAE *input, int verbose)
{
//	checklist = new std::map<daeString, domElement *>;
	domInt error = 0;
	domUint errorcount = 0;
	daeDatabase *db = input->getDatabase();

	// check geometry
	daeInt count = (daeInt) db->getElementCount(NULL, "geometry", getInput(0).c_str() );
	for (daeInt i=0; i<count; i++)
	{
		domGeometry *geometry;
		error = db->getElement((daeElement**)&geometry, i, NULL, "geometry", getInput(0).c_str());
		errorcount += CHECK_Geometry(geometry);			
	}
	// check controller
	count = (daeInt)db->getElementCount(NULL, "controller", getInput(0).c_str() );
	for (daeInt i=0; i<count; i++)
	{
		domController *controller;
		error = db->getElement((daeElement**)&controller, i, NULL, "controller", getInput(0).c_str());
		errorcount += CHECK_Controller(controller);			
	}
	// check instance_geometry
	count = (daeInt)db->getElementCount(NULL, "instance_geometry", getInput(0).c_str() );
	for (daeInt i=0; i<count; i++)
	{
		domInstance_geometry *instance_geometry;
		error = db->getElement((daeElement**)&instance_geometry, i, NULL, "instance_geometry", getInput(0).c_str() );
		errorcount += CHECK_InstanceGeometry(instance_geometry);
	}
	// check source
	count = (daeInt)db->getElementCount(NULL, "source", getInput(0).c_str() );
	for (daeInt i=0; i<count; i++)
	{
		domSource *source;
		error = db->getElement((daeElement**)&source, i, NULL, "source", getInput(0).c_str() );
		errorcount += CHECK_Source(source);
	}
	return errorcount; 
}

domUint Coherencytest::CHECK_links (DAE *input, int verbose)
{
	domInt error = 0;
	domUint errorcount = 0;
	daeDatabase *db = input->getDatabase();

	// check links
	daeInt count = (daeInt)db->getElementCount(NULL, "accessor", getInput(0).c_str() );
	for (daeInt i=0; i<count; i++)
	{
		domAccessor *accessor;
		error = db->getElement((daeElement**)&accessor, i, NULL, "accessor", getInput(0).c_str() );
		xsAnyURI & uri = accessor->getSource();
		errorcount += CHECK_uri(uri);
	}
	count = (daeInt)db->getElementCount(NULL, "channel", getInput(0).c_str());
	for (daeInt i=0; i<count; i++)
	{
		domChannel *channel;
		error = db->getElement((daeElement**)&channel, i, NULL, "channel", getInput(0).c_str());
		xsAnyURI & uri = channel->getSource();
		errorcount += CHECK_uri(uri);
	}
	count = (daeInt)db->getElementCount(NULL, "IDREF_array", getInput(0).c_str());
	for (daeInt i=0; i<count; i++)
	{
		domIDREF_array *IDREF_array;
		error = db->getElement((daeElement**)&IDREF_array, i, NULL, "IDREF_array", getInput(0).c_str());
		for (size_t j=0; j<IDREF_array->getCount(); j++) 
		{
			daeIDRef idref = IDREF_array->getValue()[j];
			idref.resolveElement();
			domElement * element = idref.getElement();
			if (element == NULL)
			{
				char temp[MAX_LOG_BUFFER];
				sprintf(temp, "IDREF_array value %s not referenced\n", idref.getID());
				PRINTF(temp);
				errorcount += CHECK_error(IDREF_array, element!=NULL, temp);
			}
		}
	}
	count = (daeInt)db->getElementCount(NULL, "input", getInput(0).c_str());
	for (daeInt i=0; i<count; i++)
	{
		domInputLocalOffset *input;
		error = db->getElement((daeElement**)&input, i, NULL, "input", getInput(0).c_str());
		xsAnyURI & uri = input->getSource();
		errorcount += CHECK_uri(uri);
	}

	count = (daeInt)db->getElementCount(NULL, "skeleton", getInput(0).c_str());
	for (daeInt i=0; i<count; i++)
	{
		domInstance_controller::domSkeleton *skeleton;
		error = db->getElement((daeElement**)&skeleton, i, NULL, "skeleton", getInput(0).c_str());
		xsAnyURI & uri = skeleton->getValue();
		errorcount += CHECK_uri(uri);
	}
	count = (daeInt)db->getElementCount(NULL, "skin", getInput(0).c_str());
	for (daeInt i=0; i<count; i++)
	{
		domSkin *skin;
		error = db->getElement((daeElement**)&skin, i, NULL, "skin", getInput(0).c_str());
		xsAnyURI & uri = skin->getSource();
		errorcount += CHECK_uri(uri);
	}
	// physics
/*	for (size_t i=0; i<db->getElementCount(NULL, "program", NULL); i++)
	{
		domProgram *program;
		error = db->getElement((daeElement**)&program, i, NULL, "program");
		xsAnyURI & uri = program->getSource();
		errorcount += CHECK_uri(uri);
	}
*/
	count = (daeInt)db->getElementCount(NULL, "instance_rigid_body", getInput(0).c_str());
	for (daeInt i=0; i<count; i++)
	{
		domInstance_rigid_body *instance_rigid_body;
		error = db->getElement((daeElement**)&instance_rigid_body, i, NULL, "instance_rigid_body", getInput(0).c_str());
		xsAnyURI & uri = instance_rigid_body->getTarget();
		errorcount += CHECK_uri(uri);
	}
	count = (daeInt)db->getElementCount(NULL, "ref_attachment", getInput(0).c_str());
	for (daeInt i=0; i<count; i++)
	{
		domRigid_constraint::domRef_attachment *ref_attachment;
		error = db->getElement((daeElement**)&ref_attachment, i, NULL, "ref_attachment", getInput(0).c_str());
		xsAnyURI & uri = ref_attachment->getRigid_body();
		errorcount += CHECK_uri(uri);
	}

	// FX, todo: color_target, connect_param, depth_target, param, stencil_target
	count = (daeInt)db->getElementCount(NULL, "instance_material", getInput(0).c_str());
	for (daeInt i=0; i<count; i++)
	{
		domInstance_material *instance_material;
		error = db->getElement((daeElement**)&instance_material, i, NULL, "instance_material", getInput(0).c_str());
		xsAnyURI & uri = instance_material->getTarget();
		errorcount += CHECK_uri(uri);
	}


	// urls
	domUint instance_elments_max = 12;
	const char * instance_elments[] = { "instance_animation",
										"instance_camera", 
										"instance_controller", 
										"instance_geometry", 
										"instance_light", 
										"instance_node", 
										"instance_visual_scene", 
										"instance_effect",
										"instance_force_field", 
										"instance_physics_material",
										"instance_physics_model",
										"instance_physics_scene"};
	
	for (size_t i=0; i<instance_elments_max ; i++) 
	{
		errorcount += CHECK_InstanceElementUrl(db, instance_elments[i]);
	}
	return 0;
}


domUint Coherencytest::CHECK_files (DAE *input, int verbose)
{
	domInt error = 0;
	domUint errorcount = 0;
	daeDatabase *db = input->getDatabase();

	// files
	daeInt count = (daeInt) db->getElementCount(NULL, "image", getInput(0).c_str());
	for (daeInt i=0; i<count; i++)
	{
		domImage *image;
		error = db->getElement((daeElement**)&image, i, NULL, "image", getInput(0).c_str());
		domImage::domInit_from * init_from = image->getInit_from();
		xsAnyURI & uri = init_from->getValue();
		errorcount += CHECK_file(init_from, uri);
	}
	count = (daeInt) db->getElementCount(NULL, "include", getInput(0).c_str());
	for (daeInt i=0; i<count; i++)
	{
		domFx_include_common *include;
		error = db->getElement((daeElement**)&include, i, NULL, "include", getInput(0).c_str());
		xsAnyURI & uri = include->getUrl();
		errorcount += CHECK_file(include, uri);
	}
	return errorcount;
}

domUint Coherencytest::CHECK_unique_id (DAE *input, int verbose)
{
	std::pair<std::set<string>::iterator, bool> pair;
	std::set<string> ids;
	domInt error = 0;
	domUint errorcount = 0;
	daeDatabase *db = input->getDatabase();
	daeInt count = (daeInt) db->getElementCount(NULL, NULL, getInput(0).c_str());
	for (daeInt i=0; i<count; i++)
	{
		domElement *element;
		error = db->getElement((daeElement**)&element, i, NULL, NULL, getInput(0).c_str());
		daeString id = element->getID();
		daeString docURI = element->getDocumentURI()->getURI();
		if (id == NULL) continue;
		pair = ids.insert(string(id) + string(docURI));
		if (pair.second == false)
		{
			errorcount++;
			char temp[MAX_LOG_BUFFER];
			sprintf(temp, "Unique ID conflict id=%s, docURI=%s\n", id, docURI);
			PRINTF(temp);
		}
	}
	return errorcount;
}

domEffect *TextureGetEffect(domCommon_color_or_texture_type::domTexture *texture)
{
	for (domElement * element = texture; element; element = element->getParentElement())
	{
		if (element->getTypeName())
			if (stricmp(element->getTypeName(), "effect") == 0)
				return (domEffect *)element;
	}
	return NULL;
}

domUint Coherencytest::CHECK_texture (DAE *input, int verbose)
{
	std::pair<std::set<string>::iterator, bool> pair;
	std::set<string> ids;
	domInt error = 0;
	domUint errorcount = 0;
	daeDatabase *db = input->getDatabase();

	daeInt count = (daeInt) db->getElementCount(NULL, "texture", getInput(0).c_str());
	for (daeInt i=0; i<count; i++)
	{
		domCommon_color_or_texture_type::domTexture *texture;
		error = db->getElement((daeElement**)&texture, i, NULL, "texture", getInput(0).c_str());
		xsNCName texture_name = texture->getTexture();
		char * target = new char[ strlen( texture_name ) + 3 ];
		strcpy( target, "./" );
		strcat( target, texture_name );
		domEffect * effect = TextureGetEffect(texture);
		if (effect==NULL) 
			continue;
		daeSIDResolver sidRes( effect, target );
		delete[] target;
		target = NULL;
		if ( sidRes.getElement() != NULL )
		{	// it is doing the right way
			continue;
		}
		daeIDRef ref(texture_name);
		ref.setContainer(texture);
		ref.resolveElement();
		daeElement * element = ref.getElement();
		if (element)
		{	// it is directly linking the image
			char temp[MAX_LOG_BUFFER];
			sprintf(temp, "CHECK_texture failed, texture=%s is direct linking to image\n", texture_name);
			PRINTF(temp);
		} else {
			char temp[MAX_LOG_BUFFER];
			sprintf(temp, "CHECK_texture failed, texture=%s is not link to anything\n", texture_name);
			PRINTF(temp);
		}
	}
	return errorcount;
}

void _XMLSchemaValidityErrorFunc(void* ctx, const char* msg, ...)
{
	va_list      LVArgs;
	char         LTmpStr[8192];    // FIXME: this is not buffer-overflow safe
//	DAEDocument* LDAEDocument = (DAEDocument*)ctx;
//	xmlSchemaValidCtxt* LXMLSchemaValidContext = (xmlSchemaValidCtxt*) ctx;
//	xmlNode*     LXMLNode = xmlSchemaValidCtxtGetNode(ctx);
//	xmlDoc *	 doc = (xmlDoc *) ctx;

	va_start(LVArgs, msg);
	vsprintf(LTmpStr, msg, LVArgs);
	va_end(LVArgs);
//	PRINTF("%s:%d  Schema validation error:\n%s", LDAEDocument->Name, xmlGetLineNo(LXMLNode), LTmpStr);
//	PRINTF("CHECK_schema Error msg=%c ctx=%p\n", msg, ctx);
	char temp[MAX_LOG_BUFFER];
	sprintf(temp, "CHECK_schema Error   msg=%s\n", LTmpStr);
	globalpointer->PRINTF(temp);
}


void _XMLSchemaValidityWarningFunc(void* ctx, const char* msg, ...)
{
	va_list      LVArgs;
	char         LTmpStr[8192];    // FIXME: this is not buffer-overflow safe
//	DAEDocument* LDAEDocument = (DAEDocument*)ctx;
//	xmlNode*     LXMLNode = xmlSchemaValidCtxtGetNode(LDAEDocument->XMLSchemaValidContext);
//	xmlDoc *	 doc = (xmlDoc *) ctx;

	va_start(LVArgs, msg);
	vsprintf(LTmpStr, msg, LVArgs);
	va_end(LVArgs);
//	PRINTF("%s:%d  Schema validation warning:\n%s", LDAEDocument->Name, xmlGetLineNo(LXMLNode), LTmpStr);
	char temp[MAX_LOG_BUFFER];
	sprintf(temp, "CHECK_schema Warning msg=%s\n", LTmpStr);
	globalpointer->PRINTF(temp);
}

//void dae_ValidateDocument(DAEDocument* LDAEDocument, xmlDocPtr LXMLDoc)
domUint Coherencytest::CHECK_validateDocument(xmlDocPtr LXMLDoc)
{
//	const char * dae_SchemaURL = "C:\\svn\\COLLADA_DOM\\doc\\COLLADASchema.xsd";
	const char * dae_SchemaURL = "http://www.collada.org/2005/11/COLLADASchema.xsd";

	xmlSchemaParserCtxt*  Ctxt = xmlSchemaNewDocParserCtxt(LXMLDoc);
	xmlSchemaParserCtxt*    LXMLSchemaParserCtxt = xmlSchemaNewParserCtxt(dae_SchemaURL);

	if(LXMLSchemaParserCtxt)
	{
		xmlSchema*    LXMLSchema = xmlSchemaParse(LXMLSchemaParserCtxt);

		if(LXMLSchema)
		{
			xmlSchemaValidCtxt*    LXMLSchemaValidContext = xmlSchemaNewValidCtxt(LXMLSchema);

			if(LXMLSchemaValidContext)
			{	
				int    LSchemaResult;

//				LDAEDocument->XMLSchemaValidContext = LXMLSchemaValidContext;

/*				xmlSchemaSetParserErrors(LXMLSchemaParserCtxt,
							_XMLSchemaValidityErrorFunc,
							_XMLSchemaValidityWarningFunc,
							LXMLDoc);
*/
				globalpointer = this;
				xmlSchemaSetValidErrors(LXMLSchemaValidContext,
							_XMLSchemaValidityErrorFunc,
							_XMLSchemaValidityWarningFunc,
							LXMLDoc);

				LSchemaResult = xmlSchemaValidateDoc(LXMLSchemaValidContext, LXMLDoc);
//				switch(LSchemaResult)
//				{
//				case 0:    PRINTF("Document validated\n");break;
//				case -1:   PRINTF("API error\n");break;
//				default:   PRINTF("Error code: %d\n", LSchemaResult);break;
//				}

//			LDAEDocument->XMLSchemaValidContext = NULL;
			xmlSchemaFreeValidCtxt(LXMLSchemaValidContext);
			}

			xmlSchemaFree(LXMLSchema);
		}
		else {
			char temp[MAX_LOG_BUFFER];
			sprintf(temp, "Could not parse schema from %s\n", dae_SchemaURL);
			PRINTF(temp);
		}
		xmlSchemaFreeParserCtxt(LXMLSchemaParserCtxt);
	}
	else {
		char temp[MAX_LOG_BUFFER];
		sprintf(temp, "Could not load schema from '%s\n", dae_SchemaURL);
		PRINTF(temp);
	}
	return 0;
}


domUint Coherencytest::CHECK_xmlfile(daeString filename)
{
	xmlDocPtr doc;		
	doc = xmlReadFile(filename, NULL, 0);
    if (doc == NULL) {
		char temp[MAX_LOG_BUFFER];
        sprintf(temp, "Failed to parse %s\n", filename);
		PRINTF(temp);
		return 1;
    } else {
		// load okay
		CHECK_validateDocument(doc);
		/* free up the parser context */
		xmlFreeDoc(doc);
	}
	return 0;
}

domUint Coherencytest::CHECK_schema (DAE *input, int verbose)
{
	domInt errorcount = 0;
	daeDatabase* db = input->getDatabase();
	daeInt count = (daeInt) db->getDocumentCount();
	for (daeInt i=0; i<count; i++)
	{
		daeDocument* doc = db->getDocument(i);
		daeString xmldoc = doc->getDocumentURI()->getURI();
		errorcount += CHECK_xmlfile(xmldoc);
	}
	return errorcount;
}

domUint Coherencytest::CHECK_inputs (domInputLocalOffset_Array & inputs, const char * semantic)
{
	domUint count = (domUint) inputs.getCount();
	for (size_t i=0; i<count; i++) 
	{
		if (stricmp(semantic, inputs[i]->getSemantic()) == 0)
			return 0;
	}
	PRINTF("CHECK inputs Failed ");
	print_name_id(inputs[0]);
	char temp[MAX_LOG_BUFFER];
	sprintf(temp, " input with semantic=%s not found", semantic);
	PRINTF(temp);
	return 1;
}

domUint Coherencytest::CHECK_inputs (domInputLocal_Array & inputs, const char * semantic)
{
	domUint count = (domUint) inputs.getCount();
	for (size_t i=0; i<count; i++) 
	{
		if (stricmp(semantic, inputs[i]->getSemantic()) == 0)
			return 0;
	}
	PRINTF("CHECK inputs Failed ");
	print_name_id(inputs[0]);
	char temp[MAX_LOG_BUFFER];
	sprintf(temp, " input with semantic=%s not found", semantic);
	PRINTF(temp);
	return 1;
}

domUint Coherencytest::CHECK_skin (DAE *input, int verbose)
{
	std::vector<domNode *> nodeset;
	domInt error = 0;
	domUint errorcount = 0;
	daeDatabase *db = input->getDatabase();

	daeInt count = (daeInt) db->getElementCount(NULL, "instance_controller", getInput(0).c_str());
	for (daeInt i=0; i<count; i++)
	{
		domInstance_controller *instance_controller;
		error = db->getElement((daeElement**)&instance_controller, i, NULL, "instance_controller", getInput(0).c_str());

		// get all skeletons
		domInstance_controller::domSkeleton_Array & skeleton_array = instance_controller->getSkeleton_array();
		domUint skeleton_count = (domUint) skeleton_array.getCount();
		for (size_t i=0; i<skeleton_count; i++)
		{
			domNode * node = (domNode*) (domElement*) skeleton_array[i]->getValue().getElement();
			nodeset.push_back(node);
		}

		//get joints from skin
		domController * controller = (domController*) (domElement*) instance_controller->getUrl().getElement();
		domSkin * skin = controller->getSkin();
		if (skin)
		{
			// get source of name_array or IDREF_array
			domSource * source = NULL;
			domSkin::domJoints * joints = skin->getJoints();
			domInputLocal_Array &input_array = joints->getInput_array();
			for(size_t i=0; i<input_array.getCount(); i++)
			{
				if (stricmp(input_array[i]->getSemantic(), "JOINT") == 0)
				{
					source = (domSource *) (domElement*) input_array[i]->getSource().getElement();
					if (source) break;
				}
			}
			if (source == NULL) continue;

			//name_array
			domName_array * name_array = source->getName_array();
			domIDREF_array * idref_array = source->getIDREF_array();
			if (name_array)
			{
				domListOfNames &list_of_names = name_array->getValue();
				domUint name_count = (domUint) list_of_names.getCount();
				for (domUint j=0; j<name_count; j++)
				{
					char jointpath[MAX_PATH];
					strcpy( jointpath, "./" );
					strcat( jointpath, list_of_names[(size_t)j] );
					domElement * e = NULL;
					for (size_t k=0; k<nodeset.size(); k++)
					{
						daeSIDResolver sidRes( nodeset[k], jointpath );
						e = sidRes.getElement();
						if (e) break;
					}
					if (e==NULL) // this joint name is not match with any sid of skeleton nodes
					{
						char tempstr[MAX_PATH];
						sprintf(tempstr, "instance_controller, can't find node with sid=%s of controller=%s\n", list_of_names[(size_t)j], controller->getId());
						errorcount += CHECK_error(instance_controller, false, tempstr);								
					}
				}
			} else if (idref_array)
			{
				xsIDREFS & list_of_idref = idref_array->getValue();
				domUint idref_count = (domUint) list_of_idref.getCount();
				for (domUint j=0; j<idref_count; j++)
				{
					list_of_idref[(size_t)j].resolveElement();
					domElement * element = list_of_idref[(size_t)j].getElement();
					if (element == 0)
					{
						char tempstr[MAX_PATH];
						sprintf(tempstr, "skin, idref=%s can't resolve\n", list_of_idref[(size_t)j].getID());
						errorcount +=CHECK_error(source, false, tempstr);
					}
				}
			} else { // name_array and IDREF_array not found
				errorcount +=CHECK_error(skin, false, "skin, both name_array and IDREF_array are not found");
			}			
		} else // TODO: for other non-skin controllers
		{
		}
	}

	return errorcount;
}

Conditioner::Register< CoherencytestProxy > coherencytestProxy;

