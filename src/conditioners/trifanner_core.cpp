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
#include "trifanner.h"
class Vertex;
class Triangle;
class Side;

class Vertex
{
public:
	Vertex() 
	{
		m_maxoffset = 0;
		m_data = 0;
	};
	Vertex(Vertex & copy)
	{
		m_maxoffset = copy.m_maxoffset;
		m_data = new domUint(m_maxoffset);
		for(int i=0; i<m_maxoffset; i++)
		{
			m_data[i] = copy.m_data[i];
		}
	};
	Vertex(domUint maxoffset) 
	{
		m_maxoffset = maxoffset;
		m_data = new domUint[(size_t)m_maxoffset];
	};
	~Vertex() 
	{
		if (m_data) 
		{
			delete [] m_data;
			m_data = 0;
		}
	};
	domUint m_maxoffset;
	domUint * m_data;

	bool operator ==(Vertex & v)
	{
		for (int i=0; i<m_maxoffset; i++)
		{
			if (m_data[i] != v.m_data[i])
				return false;
		}
		return true;
	}
	Vertex * operator =(Vertex & copy)
	{
		m_maxoffset = copy.m_maxoffset;
		m_data = new domUint(m_maxoffset);
		for(int i=0; i<m_maxoffset; i++)
		{
			m_data[i] = copy.m_data[i];
		}
		return this;
	}
};

class Triangle
{
public:
	Triangle() 
	{
		v[0] = v[1] = v[2] = NULL;
		rotated = 0;
	};
	Triangle(Vertex *v0, Vertex *v1, Vertex *v2,
			 Side *s01, Side *s12, Side *s20)
	{
		v[0] = v0; v[1] = v1; v[2] = v2;
		s[0] = s01; s[1] = s12; s[2] = s20;
		rotated = 0;
	};
	~Triangle() {};
	Vertex	* v[3];
	Side	* s[3];
	void Rotate()
	{
		Vertex * temp_v = v[0];
		v[0] = v[1]; v[1] = v[2]; v[2] = temp_v;
		Side * temp_s = s[0];
		s[0] = s[1]; s[1] = s[2]; s[2] = temp_s;
		rotated++;
	}
	int rotated;
};

class Side
{
public:
	Side() {v[0]=v[1]=NULL;};
	~Side() {};
	Side(Vertex *v0, Vertex *v1) 
	{
		v[0]=v0;
		v[1]=v1;
	};
	Vertex * v[2];
	bool operator ==(Side & s)
	{
		domUint maxoffset = s.v[0]->m_maxoffset;
		for (domUint i=0; i<maxoffset; i++)
		{
			if ((v[0]->m_data[i] != s.v[0]->m_data[i]) ||
				(v[1]->m_data[i] != s.v[1]->m_data[i]))
				return false;
		}
		return true;
	}
};

Vertex * FindNextVertex(Side ** next_side, vector<Triangle*> *tri_list)
{
	vector<Triangle*>::iterator jter;
	Side reverse((*next_side)->v[1], (*next_side)->v[0]);
	Vertex * next_vertex = NULL;

	for (jter=tri_list->begin(); jter!= tri_list->end(); jter++)
	{
		Triangle * test_tri = *jter;
		if (reverse == *(test_tri->s[0]))
		{
			next_vertex = test_tri->v[2];
			*next_side = test_tri->s[2];
			break;
		}
		if (reverse == *(test_tri->s[1]))
		{
			next_vertex = test_tri->v[0];
			*next_side = test_tri->s[0];
			break;
		}
		if (reverse == *(test_tri->s[2]))
		{
			next_vertex = test_tri->v[1];
			*next_side = test_tri->s[1];
			break;
		}
	}
	if (next_vertex)
	{	// next_vertex is found
		tri_list->erase(jter);
	}
	return next_vertex;
}

void CreateP(domTrifans * trifan, vector<Vertex *> *vertex_list)
{
	domP * dom_p = (domP*) trifan->createAndPlace(COLLADA_TYPE_P);
	for (size_t i=0; i<vertex_list->size(); i++)
	{
		Vertex * v = (*vertex_list)[i];
		for (domUint j=0; j<v->m_maxoffset; j++)
		{
			dom_p->getValue().append(v->m_data[j]);
		}
	}
}

void createTrifansFromTriangles( domMesh *thisMesh, domTriangles *thisTriangles, int verbose ) {

	vector<Triangle*> Triangle_list;

	vector<Vertex *> Vertex_Container;
	vector<Triangle*> Triangle_Container;
	vector<Side*> Side_Container;

	// Create a new <trifans> inside the mesh that has the same material as the <triangles>
	domTrifans *thisTrifans = (domTrifans *)thisMesh->createAndPlace("trifans");
	thisTrifans->setCount( 0 );

	// set name
	if (thisTriangles->getName())
		thisTrifans->setName(thisTriangles->getName());		
	// set material symbol
	if (thisTriangles->getMaterial())
		thisTrifans->setMaterial(thisTriangles->getMaterial());

	// Give the new <trifans> the same <input> and <parameters> as the old <triangles>
	for(int i=0; i<(int)(thisTriangles->getInput_array().getCount()); i++)
	{
		thisTrifans->placeElement( thisTriangles->getInput_array()[i]->clone() );
	}

	// Get the number of inputs and primitives for the triangles array
	domUint offsetcount = (int)getMaxOffset(thisTriangles->getInput_array())+1;

	// set P
	domListOfUInts triangleValue = thisTriangles->getP()->getValue();

	for(size_t i=0; i<thisTriangles->getCount(); i++)
	{
		// parse the Vertices
		Vertex * newV0 = new Vertex(offsetcount);
		Vertex * newV1 = new Vertex(offsetcount);
		Vertex * newV2 = new Vertex(offsetcount);
		Vertex_Container.push_back(newV0); Vertex_Container.push_back(newV1); Vertex_Container.push_back(newV2);
		for (size_t j=0; j<offsetcount; j++)
		{
			newV0->m_data[j] = triangleValue[(size_t)(3*i*offsetcount + 0*offsetcount + j)];
			newV1->m_data[j] = triangleValue[(size_t)(3*i*offsetcount + 1*offsetcount + j)];
			newV2->m_data[j] = triangleValue[(size_t)(3*i*offsetcount + 2*offsetcount + j)];
		}

		// Set new Triangle, Side
		Side *s01 = new Side(newV0, newV1);
		Side *s12 = new Side(newV1, newV2);
		Side *s20 = new Side(newV2, newV0);
		Side_Container.push_back(s01); Side_Container.push_back(s12); Side_Container.push_back(s20);
		Triangle * newTri = new Triangle(newV0, newV1, newV2, s01, s12, s20);
		Triangle_Container.push_back(newTri);
		// put Triangles into list
		Triangle_list.push_back(newTri);
	}

	domUint TriFanCount = 0;
	vector<Triangle*>::iterator iter;
	for (iter = Triangle_list.begin(); !Triangle_list.empty(); iter = Triangle_list.begin())
	{
		vector<Vertex *> vertex_list;

		Triangle *start_tri = *iter;
		Side * back_side = start_tri->s[0];
		Side reverse_back_side(back_side->v[1], back_side->v[0]);
		Side * next_side = start_tri->s[2];
		vertex_list.push_back(start_tri->v[0]);
		vertex_list.push_back(start_tri->v[1]);
		vertex_list.push_back(start_tri->v[2]);
		Triangle_list.erase(iter);

		vector<Triangle*>::iterator jter;
		for (jter=Triangle_list.begin(); jter!= Triangle_list.end(); jter=Triangle_list.begin())
		{
			// find the next vertex, might update next_side and Triangle_list
			Vertex * next_vertex = FindNextVertex(&next_side, &Triangle_list);
			if (next_vertex)
			{	// next_vertex found, add to vertex_list, check if fan is looped
				vertex_list.push_back(next_vertex);
				if (reverse_back_side == *next_side)
				{	// fan looped
					break;
				}
			} else 
			{	// next_vertex not found, complete one TriFan, but not looped				
				break;
			}
		}
		
		if (vertex_list.size() == 3 && start_tri->rotated < 2)
		{
			start_tri->Rotate();
			Triangle_list.push_back(start_tri);
			vertex_list.clear();
			continue;
		}

		CreateP(thisTrifans, &vertex_list);
		TriFanCount++;
		vertex_list.clear();
	}

	thisTrifans->setCount(TriFanCount);

	//cleanup
	for(size_t i=0; i<Triangle_Container.size(); i++)
		delete Triangle_Container[i];
	Triangle_Container.clear();
	for(size_t i=0; i<Side_Container.size(); i++)
		delete Side_Container[i];
	Side_Container.clear();
	for(size_t i=0; i<Vertex_Container.size(); i++)
		delete Vertex_Container[i];
	Vertex_Container.clear();
}

bool Trifanner::init()
{
	addBoolOption( "verbose", "verbose", "verbose", true);
	return true;
}

int Trifanner::execute()
{
	bool verbose;
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

		if (thisMesh == NULL) continue;
		// Loop over all the triangle elements
		int trianglesElementCount = (int)(thisMesh->getTriangles_array().getCount());
		//if(verbose)	cerr<<"There are "<<trianglesElementCount<<"triangle elements in this file\n"; 
		for(int currentTriangles = 0; currentTriangles < trianglesElementCount; currentTriangles++)
		{
			// Create Trifans from Triangles
			domTriangles *thisTriangles = thisMesh->getTriangles_array().get(currentTriangles);  
			createTrifansFromTriangles( thisMesh, thisTriangles, verbose );
		}  
		while(thisMesh->getTriangles_array().getCount() != 0)
		{
			domTriangles *thisTriangles = thisMesh->getTriangles_array().get(0);  
			thisMesh->removeChildElement(thisTriangles);
		}
	} 
	return 0; 
}

Conditioner::Register<TrifannerProxy> trifannerProxy;


