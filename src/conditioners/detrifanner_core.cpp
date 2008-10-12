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
#include "detrifanner.h"

bool Detrifanner::init()
{
	return true;
}

int Detrifanner::execute()
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

		// Loop over all the trifan elements
		int trifansElementCount = (int)(thisMesh->getTrifans_array().getCount());
		//if(verbose)	cerr<<"There are "<<trifansElementCount<<"trifan elements in this file\n"; 
		for(int currentTrifans = 0; currentTrifans < trifansElementCount; currentTrifans++)
		{
			// Get the trifans out of the mesh
			// Always get index 0 because every pass through this loop deletes the <polygons> element as it finishes with it
			domTrifans *thisTrifans = thisMesh->getTrifans_array().get(0);  
			createTrianglesFromTrifans( thisMesh, thisTrifans );
			// Remove the polygons from the mesh
			thisMesh->removeChildElement(thisTrifans);
		}
	}
	return 0;
}

void Detrifanner::createTrianglesFromTrifans( domMesh *thisMesh, domTrifans *thisTrifans ) {
	// Create a new <triangles> inside the mesh that has the same material as the <trifans>
	domTriangles *thisTriangles = (domTriangles *)thisMesh->createAndPlace("triangles");
	thisTriangles->setCount( 0 );
	thisTriangles->setMaterial(thisTrifans->getMaterial());
	domP* p_triangles = (domP*)thisTriangles->createAndPlace("p");

	// Give the new <triangles> the same <input> and <parameters> as the old <trifans>
	for(int i=0; i<(int)(thisTrifans->getInput_array().getCount()); i++)
	{
		thisTriangles->placeElement( thisTrifans->getInput_array()[i]->clone() );
	}

	// Get the number of inputs and primitives for the trifans array
	int numberOfInputs = (int)getMaxOffset(thisTrifans->getInput_array()) +1;
	int numberOfPrimitives = (int)(thisTrifans->getP_array().getCount());

	thisTriangles->setCount(0);
	// go through all the primitives, this generates all the triangles in a single <p> element
	for(int j = 0; j < numberOfPrimitives; j++)
	{
		// Check the trifans for consistancy (some exported files have had the wrong number of indices)
		domP * thisPrimitive = thisTrifans->getP_array()[j];
		int elementCount = (int)(thisPrimitive->getValue().getCount());
		if((elementCount%numberOfInputs) != 0)
		{
			//cerr<<"Primitive "<<j<<" has an element count "<<elementCount<<" not divisible by the number of inputs "<<numberOfInputs<<"\n";
		}
		else
		{
			int triangleCount = (elementCount/numberOfInputs)-2;
			// Write out the primitives as triangles
			// first triangle
			for (int i = 0; i < numberOfInputs; i++) {
				p_triangles->getValue().append(thisPrimitive->getValue()[i]);
			}
			int idx = numberOfInputs;
			for (int i = 0; i < numberOfInputs; i++) {
				p_triangles->getValue().append(thisPrimitive->getValue()[idx + i]);
			}
			idx += numberOfInputs;
 			for (int i = 0; i < numberOfInputs; i++) {
				p_triangles->getValue().append(thisPrimitive->getValue()[idx + i]);
			}
			//thisTriangles->setCount(1);
		    // second triangle etc.
			for(int k = 0; k < triangleCount-1; k++)
			{
				// First vertex
				for(int l = 0; l < numberOfInputs; l++)
				{
					p_triangles->getValue().append(thisPrimitive->getValue()[l]);
				}
				// Second vertex
				for(int l = 0; l < numberOfInputs; l++)
				{
					p_triangles->getValue().append(thisPrimitive->getValue()[numberOfInputs*(k+2) + l]);
				}
				// Third vertex
				for(int l = 0; l < numberOfInputs; l++)
				{
					p_triangles->getValue().append(thisPrimitive->getValue()[numberOfInputs*(k+3) + l]);
				}
				//thisTriangles->setCount(thisTriangles->getCount()+1);
			}
			thisTriangles->setCount(thisTriangles->getCount()+triangleCount);
		}
	}
}

Conditioner::Register< DetrifannerProxy > detrifannerProxy;
