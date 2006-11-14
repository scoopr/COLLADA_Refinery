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
#include "bindmaterialfixer.h"
#include <dom/domConstants.h>
#include <dom/domFx_include_common.h>

bool Bindmaterialfixer::init()
{
	return true;
}

int Bindmaterialfixer::execute()
{
	set<string> materialsymbols;
	int error;
	daeString doc = getInput(0).c_str();

    daeUInt IncstanceGeometryCount = _dae->getDatabase()->getElementCount(NULL, COLLADA_TYPE_INSTANCE_GEOMETRY, doc );
    for(daeUInt i=0; i<IncstanceGeometryCount; i++)
	{
		domInstance_geometry * instance_geometry;
	    error = _dae->getDatabase()->getElement((daeElement**)&instance_geometry,i, NULL, COLLADA_TYPE_INSTANCE_GEOMETRY, doc);

		domGeometry * geometry = (domGeometry *) (domElement*) instance_geometry->getUrl().getElement();
		if (geometry == NULL)	// if geometry doesn't exist, check the next one.
			continue;

		domMesh * mesh = geometry->getMesh();
		if (mesh == NULL)		// if mesh doesn't exist, check the next one.
			continue;

		// triangles
		domTriangles_Array & triangles = mesh->getTriangles_array();
		for (daeUInt i=0; i<triangles.getCount(); i++)
		{
		  if( triangles[i]->getMaterial() )
			materialsymbols.insert(triangles[i]->getMaterial());
		}
		// polygons
		domPolygons_Array & polygons = mesh->getPolygons_array();
		for (daeUInt i=0; i<polygons.getCount(); i++)
		{
		  if( polygons[i]->getMaterial() )
			materialsymbols.insert(polygons[i]->getMaterial());
		}
		// polylist
		domPolylist_Array & polylists = mesh->getPolylist_array();
		for (daeUInt i=0; i<polylists.getCount(); i++)
		{
		  if( polylists[i]->getMaterial() )
			materialsymbols.insert(polylists[i]->getMaterial());
		}
		// tristrips
		domTristrips_Array & tristrips = mesh->getTristrips_array();
		for (daeUInt i=0; i<tristrips.getCount(); i++)
		{
		  if( tristrips[i]->getMaterial() )
			materialsymbols.insert(tristrips[i]->getMaterial());
		}
		// trifans
		domTrifans_Array & trifans = mesh->getTrifans_array();
		for (daeUInt i=0; i<trifans.getCount(); i++)
		{
		  if( trifans[i]->getMaterial() )
			materialsymbols.insert(trifans[i]->getMaterial());
		}
		// lines
		domLines_Array & lines = mesh->getLines_array();
		for (daeUInt i=0; i<lines.getCount(); i++)
		{
		  if( lines[i]->getMaterial() )
			materialsymbols.insert(lines[i]->getMaterial());
		}
		// linestrips
		domLinestrips_Array & linestrips = mesh->getLinestrips_array();
		for (daeUInt i=0; i<linestrips.getCount(); i++)
		{
		  if( linestrips[i]->getMaterial() )
			materialsymbols.insert(linestrips[i]->getMaterial());
		}

		domBind_material * bind_material = instance_geometry->getBind_material();
		if (bind_material == NULL)
			bind_material = (domBind_material *) instance_geometry->createAndPlace(COLLADA_TYPE_BIND_MATERIAL);
		domBind_material::domTechnique_common * technique_common = bind_material->getTechnique_common();
		if (technique_common == NULL)
			technique_common = (domBind_material::domTechnique_common*) bind_material->createAndPlace(COLLADA_TYPE_TECHNIQUE_COMMON);

		domInstance_material_Array & instance_material_array = technique_common->getInstance_material_array();
		set<string>::iterator iterator; 
		for (daeUInt j=0; j<instance_material_array.getCount(); j++)
		{
			string symbol = instance_material_array[j]->getSymbol();
			iterator = materialsymbols.find(symbol);
			if (iterator == materialsymbols.end()) { // this symbol doesn't exist
			} else {
				materialsymbols.erase(symbol);
			}
		}

		for (;materialsymbols.size()>0;) // these symbols are not binded
		{
			iterator = materialsymbols.begin();
			string symbol = *iterator;
			daeIDRef idref(symbol.c_str());
			idref.resolveElement();
			domElement * element = idref.getElement();
			if (element == NULL) {						// material with id == symbol NOT found, check the next one
				materialsymbols.erase(iterator);
				continue;
			}
			domInstance_material* instance_material = (domInstance_material*) technique_common->createAndPlace(COLLADA_TYPE_INSTANCE_MATERIAL);
			instance_material->setSymbol(symbol.c_str());
			string doc = instance_material->getDocument()->getDocumentURI()->getURI();
			string poundsymbol = "#" + symbol;
//			string uristr = doc + poundsymbol;
//			xsAnyURI uri(uristr.c_str());
//			uri.resolveElement();
//			element = uri.getElement();
			instance_material->getTarget().setURI(poundsymbol.c_str());
			materialsymbols.erase(iterator);
		}
	}
	return 0;
}

Conditioner::Register< BindmaterialfixerProxy > bindmaterialfixerProxy;
