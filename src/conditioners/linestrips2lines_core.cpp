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
#include "linestrips2lines.h"

bool Linestrips2lines::init()
{
	addBoolOption( "verbose", "verbose", "verbose", true);
	return true;
}

int Linestrips2lines::execute()
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

		// Loop over all the linestrip elements
		int linestripCount = (int)(thisMesh->getLinestrips_array().getCount());

		if (linestripCount  == 0)
			continue;

		domLines *thisLines = (domLines *)thisMesh->createAndPlace("lines");
		domP*  lineP = (domP*) thisLines->createAndPlace("p");

//		if(verbose)	cerr<<"There are "<<linestripCount<<" linestrip elements in this file\n"; 
		for(int currLinestrip = 0; currLinestrip<linestripCount; currLinestrip++)
		{
			// Get the linestrip out of the mesh
			// Always get index 0 because every pass through this loop deletes the <linestrips> element as it finishes with it
			domLinestrips *thisLinestrip = thisMesh->getLinestrips_array().get(0);
			
			// add inputs
			unsigned int offsetcount = 0;
			for(unsigned int i=0; i<thisLinestrip->getInput_array().getCount(); i++)
			{
				unsigned int tempoffset = (unsigned int) thisLinestrip->getInput_array()[i]->getOffset();
				if (offsetcount < tempoffset) offsetcount = tempoffset;
				thisLines->placeElement( thisLinestrip->getInput_array()[i]->clone() );
			}
			offsetcount++; //offset is 1 start
			// add names
			if (thisLinestrip->getName())
				thisLines->setName(thisLinestrip->getName());
			// set materials
			thisLines->setMaterial(thisLinestrip->getMaterial());
			// set P
			int pArrayCount = (int)thisLinestrip->getP_array().getCount();
			unsigned int totalLineCount = 0;
			for (int i = 0; i < pArrayCount; i++) {
				domListOfUInts linestripValue = thisLinestrip->getP_array()[i]->getValue();
				unsigned int lineCount = (unsigned int) thisLinestrip->getP_array()[i]->getValue().getCount()/offsetcount - 1;
				for(unsigned int i=0; i<lineCount; i=i+1)
				{
					for(unsigned int j=0; j<offsetcount; j++)
						lineP->getValue().append(linestripValue[i*offsetcount+j]);

					for(unsigned int j=0; j<offsetcount; j++)
						lineP->getValue().append(linestripValue[(i+1)*offsetcount+j]);
				}
				totalLineCount += lineCount;
			}
			thisLines->setCount(totalLineCount);
			thisMesh->removeChildElement(thisLinestrip);
		}
	}
	return 0;
}

Conditioner::Register<Linestrips2linesProxy> linestrips2linesProxy;
