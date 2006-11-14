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

Conditioner::Register<Triangles2linesProxy> registerObject;
