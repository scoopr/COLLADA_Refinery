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
			for ( daeUInt poly = 0; poly < polys->getCount(); poly++ )
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

Conditioner::Register< Polygons2PolylistsProxy > pgon2plistProxy;
