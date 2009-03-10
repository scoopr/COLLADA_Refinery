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
#include "template.h"
#include <dae.h>
#include <dae/daeDatabase.h>


bool Template::init()
{
	return true;
}

int Template::execute()
{
	int error = 0;

/*	std::string doc = getInput( 0 );

	// How many geometry elements are there?
	int geometryElementCount = (int)(_dae->getDatabase()->getElementCount(NULL, "geometry", doc.c_str() ));
//	if(verbose)	cerr<<"There are "<<geometryElementCount<<" geometry elements in this file\n"; 

	for(int currentGeometry = 0; currentGeometry < geometryElementCount; currentGeometry++)
	{
		// Find the next geometry element
		domGeometry *thisGeometry;
		error = _dae->getDatabase()->getElement((daeElement**)&thisGeometry,currentGeometry, NULL, "geometry", doc.c_str() );

		// Get the mesh out of the geometry
		domMesh *thisMesh = thisGeometry->getMesh();

		if (thisMesh == 0) continue;

		// Loop over all the polygon elements
		for(size_t currentPolygons = 0; currentPolygons < thisMesh->getPolygons_array().getCount(); currentPolygons++)
		{
			// Get the polygons out of the mesh
			// Always get index 0 because every pass through this loop deletes the <polygons> element as it finishes with it
			domPolygons *thisPolygons = thisMesh->getPolygons_array()[currentPolygons];  
			createTrianglesFromPolygons( thisMesh, thisPolygons );
			// Remove the polygons from the mesh
			thisMesh->removeChildElement(thisPolygons);
		}
		while(thisMesh->getPolygons_array().getCount() > 0)
		{
			domPolygons *thisPolygons = thisMesh->getPolygons_array()[0];  
			thisMesh->removeChildElement(thisPolygons);
		}
	}
*/	
	return 0;
}


Conditioner::Register< TemplateProxy > TemplateProxy;

