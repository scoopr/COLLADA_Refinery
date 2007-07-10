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
#include "polygons2lines.h"
bool Polygons2lines::init()
{
	addBoolOption( "verbose", "verbose", "verbose", true);
	return true;
}

int Polygons2lines::execute()
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
		daeUInt PolygonsCount = (daeUInt) thisMesh->getPolygons_array().getCount();

//		polygons
		for(daeUInt j=0; j<PolygonsCount; j++)
		{
			// Get the polygons out of the mesh
			// Always get index 0 because every pass through this loop deletes the <polygons> element as it finishes with it
			domPolygons *polygons = thisMesh->getPolygons_array()[0];

			domLines *thisLines = (domLines *)thisMesh->createAndPlace("lines");
			domP*  lineP = (domP*) thisLines->createAndPlace("p");

			// add inputs
			unsigned int offsetcount = 0;
			for(unsigned int i=0; i<polygons->getInput_array().getCount(); i++)
			{
				unsigned int tempoffset = (unsigned int ) polygons->getInput_array()[i]->getOffset();
				if (offsetcount < tempoffset) offsetcount = tempoffset;
				thisLines->placeElement( polygons->getInput_array()[i]->clone() );
			}
			offsetcount++; //offset is 1 start
			// add names
			if (polygons->getName())
				thisLines->setName(polygons->getName());
			// set materials
			thisLines->setMaterial(polygons->getMaterial());

			// set Ps
			daeUInt	linecount = 0;
			for (daeUInt k=0; k<polygons->getP_array().getCount(); k++)
			{
				domP * p = polygons->getP_array()[k];
				domListOfUInts & plist = p->getValue();
				daeUInt vertexcount = (daeUInt) plist.getCount() / offsetcount;

				for (daeUInt l=0; l<vertexcount-1; l++)
				{
					for(daeUInt m=0; m<offsetcount; m++)
						lineP->getValue().append(plist[l*offsetcount+m]);
					for(daeUInt m=0; m<offsetcount; m++)
						lineP->getValue().append(plist[(l+1)*offsetcount+m]);
				}
				for(daeUInt m=0; m<offsetcount; m++)
					lineP->getValue().append(plist[(vertexcount-1)*offsetcount+m]);
				for(daeUInt m=0; m<offsetcount; m++)
					lineP->getValue().append(plist[m]);
				linecount += vertexcount;
			}
			thisLines->setCount(linecount);
			thisMesh->removeChildElement(polygons);
		}
	}
	return 0;
}

Conditioner::Register<Polygons2linesProxy> polygons2linesProxy;
