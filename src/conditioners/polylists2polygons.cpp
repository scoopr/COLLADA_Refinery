/*
The MIT License

Copyright 2006 Sony Computer Entertainment Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE. 
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
			for ( daeUInt poly = 0; poly < plist->getCount(); poly++ )
			{
				domP *p = daeSafeCast<domP>( polys->createAndPlace( COLLADA_ELEMENT_P ) );
				int vcount = (int)plist->getVcount()->getValue()[poly];
				for ( int v = 0; v < vcount; v++ )
				{
					domListOfUInts &inds = p->getValue();
					for ( daeUInt i = 0; i < maxOffset+1; i++ )
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
