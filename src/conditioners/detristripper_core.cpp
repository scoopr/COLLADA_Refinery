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
#include "detristripper.h"
#include <dae.h>
#include <dae/daeDatabase.h>

void createTrianglesFromTristrippers( domMesh *thisMesh, domTristrips *thisTistrippers );
unsigned int getMaxOffset2( domInputLocalOffset_Array &input_array );

bool DeTriStripper::init()
{
	return true;
}

int DeTriStripper::execute()
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

		// Loop over all the tristripper elements
		int tristrippersElementCount = (int)(thisMesh->getTristrips_array().getCount());
		//if(verbose)	cerr<<"There are "<<tristrippersElementCount<<" tristripper elements in this file\n"; 
		for(int currentTristrippers = 0; currentTristrippers < tristrippersElementCount; currentTristrippers++)
		{
			// Get the tristrippers out of the mesh
			// Always get index 0 because every pass through this loop deletes the <tristrippers> element as it finishes with it
			domTristrips *thisTristrippers = thisMesh->getTristrips_array().get(0);  
			createTrianglesFromTristrippers( thisMesh, thisTristrippers );
			// Remove the tristrippers from the mesh
			thisMesh->removeChildElement(thisTristrippers);
		}
	}
	return 0;
}

void DeTriStripper::createTrianglesFromTristrippers( domMesh *thisMesh, domTristrips *thisTristrippers ) {
	// Create a new <triangles> inside the mesh that has the same material as the <tristrippers>
	domTriangles *thisTriangles = (domTriangles *)thisMesh->createAndPlace("triangles");
	thisTriangles->setCount( 0 );
	thisTriangles->setMaterial(thisTristrippers->getMaterial());
	domP* p_triangles = (domP*)thisTriangles->createAndPlace("p");

	// Give the new <triangles> the same <input> and <parameters> as the old <tristrippers>
	for(int i=0; i<(int)(thisTristrippers->getInput_array().getCount()); i++)
	{
		thisTriangles->placeElement( thisTristrippers->getInput_array()[i]->clone() );
	}

	// Get the number of inputs and primitives for the tristrippers array
	int numberOfInputs = (int)getMaxOffset2(thisTristrippers->getInput_array()) +1;
	int numberOfPrimitives = (int)(thisTristrippers->getP_array().getCount());
	//if (1) cerr<<"There are"<<numberOfInputs<<" inputs\n";
	//if (1) cerr<<"There are"<<numberOfPrimitives<<" primitives\n";

	// Triangulate all the primitives, this generates all the triangles in a single <p> element
	for(int j = 0; j < numberOfPrimitives; j++)
	{
		// Check the polygons for consistancy (some exported files have had the wrong number of indices)
		domP * thisPrimitive = thisTristrippers->getP_array()[j];
		int elementCount = (int)(thisPrimitive->getValue().getCount());
		//cerr<<"elementcount is "<<elementCount<<"\n";
		if((elementCount%numberOfInputs) != 0)
		{
			//cerr<<"Primitive "<<j<<" has an element count "<<elementCount<<" not divisible by the number of inputs "<<numberOfInputs<<"\n";
		}
		else
		{
			int triangleCount = (elementCount/numberOfInputs)-2;
			//if (1) cerr<<"There are "<<triangleCount<<" triangles\n";
			// Write out the primitives as triangles, just fan using the first element as the base
			int offset = 0;
			for(int k = 0; k < triangleCount; k++)
			{
				// First vertex
				for(int l = offset; l < numberOfInputs+offset; l++)
				{
					p_triangles->getValue().append(thisPrimitive->getValue()[l]);
				}
				// Second vertex
				for(int l = offset+numberOfInputs; l < offset+2*numberOfInputs; l++)
				{
					p_triangles->getValue().append(thisPrimitive->getValue()[l]);
				}
				// Third vertex
				for(int l = offset+2*numberOfInputs; l < offset+3*numberOfInputs; l++)
				{
					p_triangles->getValue().append(thisPrimitive->getValue()[l]);
				}
				thisTriangles->setCount(thisTriangles->getCount()+1);
				offset += numberOfInputs;
			}
		}
	}
}

unsigned int DeTriStripper::getMaxOffset2( domInputLocalOffset_Array &input_array ) {
	unsigned int maxOffset = 0;
	for ( unsigned int i = 0; i < input_array.getCount(); i++ ) {
		if ( input_array[i]->getOffset() > maxOffset ) {
			maxOffset = (unsigned int) input_array[i]->getOffset();
		}
	}
	return maxOffset;
}

Conditioner::Register< DeTriStripperProxy > triangulateProxy;
