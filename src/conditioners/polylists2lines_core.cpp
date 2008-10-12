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
#include "polylists2lines.h"
bool Polylists2lines::init()
{
	addBoolOption( "verbose", "verbose", "verbose", true);
	return true;
}

int Polylists2lines::execute()
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
		daeUInt PolylistsCount = (daeUInt) thisMesh->getPolylist_array().getCount();

//		polylists
		for(daeUInt j=0; j<PolylistsCount; j++)
		{
			// Get the polygons out of the mesh
			// Always get index 0 because every pass through this loop deletes the <polygons> element as it finishes with it
			domPolylist *polylist = thisMesh->getPolylist_array()[0];
			
			domLines *thisLines = (domLines *)thisMesh->createAndPlace("lines");
			domP*  lineP = (domP*) thisLines->createAndPlace("p");

			// add inputs
			unsigned int offsetcount = 0;
			for(unsigned int i=0; i<polylist->getInput_array().getCount(); i++)
			{
				unsigned int tempoffset = (unsigned int)polylist->getInput_array()[i]->getOffset();
				if (offsetcount < tempoffset) offsetcount = tempoffset;
				thisLines->placeElement( polylist->getInput_array()[i]->clone() );
			}
			offsetcount++; //offset is 1 start
			// add names
			if (polylist->getName())
				thisLines->setName(polylist->getName());
			// set materials
			thisLines->setMaterial(polylist->getMaterial());

			// set Ps
			domListOfUInts vcount = polylist->getVcount()->getValue();
			domListOfUInts p = polylist->getP()->getValue();
			
			unsigned int linecount = 0;

			for (daeUInt k=0; k<vcount.getCount(); k++)
			{
				unsigned int vertexcount = (unsigned int) vcount[k];
				for (daeUInt m=0; m<vertexcount-1; m++)
				{
					for (unsigned int o=0; o<offsetcount; o++)
						lineP->getValue().append( p[(linecount+m)*offsetcount+o]);
					for (unsigned int o=0; o<offsetcount; o++)
						lineP->getValue().append( p[(linecount+m+1)*offsetcount+o]);
				}
				for (unsigned int o=0; o<offsetcount; o++)
					lineP->getValue().append( p[(linecount+vertexcount-1)*offsetcount+o]);
				for (unsigned int o=0; o<offsetcount; o++)
					lineP->getValue().append( p[linecount*offsetcount+ o]);
				linecount += vertexcount;
			}
			thisLines->setCount(linecount);
			thisMesh->removeChildElement(polylist);
		}
	}
	return 0;
}

Conditioner::Register<Polylists2linesProxy> polylists2linesProxy;
