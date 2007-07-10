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
#include "polylists2polygons.h"
#include <dom/domConstants.h>

bool Polylists2Polygons::init()
{
	return true;
}

int Polylists2Polygons::execute()
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

		// Loop over all the polylist elements
		while( thisMesh->getPolylist_array().getCount() > 0 )
		{
			domPolylist *plist = thisMesh->getPolylist_array()[0];

			domPolygons *polys = daeSafeCast<domPolygons>( thisMesh->createAndPlace( COLLADA_ELEMENT_POLYGONS ) );
			polys->setCount( plist->getCount() );
			polys->setMaterial( plist->getMaterial() );
			if ( plist->getName() != NULL && plist->getName()[0] != 0 )
			{
				polys->setName( plist->getName() );
			}
			int inputCount = (int)plist->getInput_array().getCount();
			domUint maxOffset = 0;
			for ( int i = 0; i < inputCount; i++ )
			{
				polys->placeElement( plist->getInput_array()[i]->clone() );
				if ( plist->getInput_array()[i]->getOffset() > maxOffset )
					maxOffset = plist->getInput_array()[i]->getOffset();
			}
			int currIdx = 0;
			for ( int poly = 0; poly < plist->getCount(); poly++ )
			{
				domP *p = daeSafeCast<domP>( polys->createAndPlace( COLLADA_ELEMENT_P ) );
				int vcount = (int)plist->getVcount()->getValue()[poly];
				for ( int v = 0; v < vcount; v++ )
				{
					domListOfUInts &inds = p->getValue();
					for ( int i = 0; i < maxOffset+1; i++ )
					{
						inds.append( plist->getP()->getValue()[currIdx] );
						currIdx++;
					}
				}
			}

			thisMesh->removeChildElement( plist );
		}
	}
	return 0;
}

Conditioner::Register< Polylists2PolygonsProxy > plist2pgonProxy;
