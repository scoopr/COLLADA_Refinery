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
#include "triangles2lines.h"

bool Triangles2lines::init()
{
	addBoolOption( "verbose", "verbose", "verbose", true);
	return true;
}

int Triangles2lines::execute()
{
	bool verbose = false;
	getBoolOption("v", verbose);
	int error = 0;

	// How many geometry elements are there?
	int geometryElementCount = (int)(_dae->getDatabase()->getElementCount(NULL, "geometry", getInput(0).c_str()));
//	if(verbose)	cerr<<"There are "<<geometryElementCount<<" geometry elements in this file\n"; 

	for(int currentGeometry = 0; currentGeometry < geometryElementCount; currentGeometry++)
	{
		// Find the next geometry element
		domGeometry *thisGeometry;
		error = _dae->getDatabase()->getElement((daeElement**)&thisGeometry,currentGeometry, NULL, "geometry", getInput(0).c_str());

		// Get the mesh out of the geometry
		domMesh *thisMesh = thisGeometry->getMesh();
		if (thisMesh == 0) continue;

		// Loop over all the polygon elements
		int triangleCount = (int)(thisMesh->getTriangles_array().getCount());

		if (triangleCount  == 0)
			continue;

		domLines *thisLines = (domLines *)thisMesh->createAndPlace("lines");
		domP*  lineP = (domP*) thisLines->createAndPlace("p");

//		if(verbose)	cerr<<"There are "<<polygonsElementCount<<" polygon elements in this file\n"; 
		for(int currTriangle = 0; currTriangle<triangleCount; currTriangle++)
		{
			// Get the polygons out of the mesh
			// Always get index 0 because every pass through this loop deletes the <polygons> element as it finishes with it
			domTriangles *thisTriangle = thisMesh->getTriangles_array()[0];
			
			// add inputs
			unsigned int offsetcount = 0;
			domInputLocalOffset_Array & input_array = thisTriangle->getInput_array();
			for(unsigned int i=0; i<input_array.getCount(); i++)
			{
				domUint tempoffset = thisTriangle->getInput_array()[i]->getOffset();
				if (offsetcount < tempoffset) offsetcount = (unsigned int) tempoffset;
				thisLines->placeElement( thisTriangle->getInput_array()[i]->clone() );
			}
			offsetcount++; //offset is 1 start
			// add names
			if (thisTriangle->getName())
				thisLines->setName(thisTriangle->getName());
			// set materials
			thisLines->setMaterial(thisTriangle->getMaterial());
			// set P
			domListOfUInts triValue = thisTriangle->getP()->getValue();
			for(unsigned int i=0; i<thisTriangle->getCount() * 3; i=i+3)
			{
				for(unsigned int j=0; j<offsetcount; j++)
					lineP->getValue().append(triValue[i*offsetcount+j]);
				for(unsigned int j=0; j<offsetcount; j++)
					lineP->getValue().append(triValue[(i+1)*offsetcount+j]);

				for(unsigned int j=0; j<offsetcount; j++)
					lineP->getValue().append(triValue[(i+1)*offsetcount+j]);
				for(unsigned int j=0; j<offsetcount; j++)
					lineP->getValue().append(triValue[(i+2)*offsetcount+j]);

				for(unsigned int j=0; j<offsetcount; j++)
					lineP->getValue().append(triValue[(i+2)*offsetcount+j]);
				for(unsigned int j=0; j<offsetcount; j++)
					lineP->getValue().append(triValue[(i)*offsetcount+j]);
			}
			thisLines->setCount(thisTriangle->getCount() * 3);
			thisMesh->removeChildElement(thisTriangle);
		}
	}
	return 0;
}

Conditioner::Register<Triangles2linesProxy> triangles2linesProxy;
