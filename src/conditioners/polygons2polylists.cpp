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
#include "polygons2polylists.h"
#include <dom/domConstants.h>

bool Polygons2Polylists::init()
{
	return true;
}

int Polygons2Polylists::execute()
{
	int error = 0;

	// How many geometry elements are there?
	int geometryElementCount = (int)(_dae->getDatabase()->getElementCount(NULL, "geometry", getInput(0).c_str()));

	for(int currentGeometry = 0; currentGeometry < geometryElementCount; currentGeometry++)
	{
		// Find the next geometry element
		domGeometry *thisGeometry;
		error = _dae->getDatabase()->getElement((daeElement**)&thisGeometry,currentGeometry, NULL, "geometry", getInput(0).c_str());

		// Get the mesh out of the geometry
		domMesh *thisMesh = thisGeometry->getMesh();

		if (thisMesh == NULL) continue;

		// Loop over all the polygon elements
		while( thisMesh->getPolygons_array().getCount() > 0 )
		{
			domPolygons *polys = thisMesh->getPolygons_array()[0];
			
			domPolylist *plist = daeSafeCast<domPolylist>( thisMesh->createAndPlace( COLLADA_ELEMENT_POLYLIST ) );
			domPolylist::domVcount *vcount = daeSafeCast<domPolylist::domVcount>( plist->createAndPlace( COLLADA_ELEMENT_VCOUNT ) );
			domListOfUInts &vcs = vcount->getValue();
			plist->createAndPlace( COLLADA_ELEMENT_P );
			
			plist->setCount( polys->getCount() );
			plist->setMaterial( polys->getMaterial() );
			if ( polys->getName() != NULL && polys->getName()[0] != 0 )
			{
				plist->setName( polys->getName() );
			}
			int inputCount = (int)polys->getInput_array().getCount();
			domUint maxOffset = 0;
			for ( int i = 0; i < inputCount; i++ )
			{
				plist->placeElement( polys->getInput_array()[i]->clone() );
				if ( polys->getInput_array()[i]->getOffset() > maxOffset )
					maxOffset = polys->getInput_array()[i]->getOffset();
			}
			for ( int poly = 0; poly < polys->getCount(); poly++ )
			{
				domP *p = polys->getP_array()[poly];
								
				plist->getP()->getValue().appendArray( p->getValue() );
				vcs.append( p->getValue().getCount() / (maxOffset+1) );
			}

			thisMesh->removeChildElement( polys );
		}
	}
	return 0;
}

Conditioner::Register< Polygons2PolylistsProxy > p2pProxy;
