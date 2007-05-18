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
#include "lines2linestrips.h"

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

class Line
{
public:
	Line() {v[0]=v[1]=NULL;};
	~Line() {};
	Line(Vertex *v0, Vertex *v1) 
	{
		v[0]=v0;
		v[1]=v1;
	};
	Vertex * v[2];
	bool operator ==(Line & s)
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

bool Lines2linestrips::init()
{
	addBoolOption( "verbose", "verbose", "verbose", true);
	return true;
}

void ConvertLinesToLine(domMesh * thisMesh, domLines * lines)
{
	vector<Line *> Line_Container;
	vector<Vertex *> Vertex_Container;

	vector<Line *> Line_List;
	domLinestrips *linestrips = (domLinestrips *)thisMesh->createAndPlace("linestrips");

	// Get the number of inputs and primitives for the triangles array
	domUint offsetcount = (int)getMaxOffset(lines->getInput_array())+1;

	// set inputs
	for (size_t j=0; j<lines->getInput_array().getCount(); j++)
		linestrips->placeElement(lines->getInput_array()[j]->clone());
	// set names
	if (lines->getName())
		linestrips->setName(lines->getName());
	// set materials
	if (lines->getMaterial())
		linestrips->setMaterial(lines->getMaterial());

	domUint line_count = lines->getCount();

	domP * dom_p = lines->getP();
	if (dom_p==NULL) return;
	domListOfUInts p_value = dom_p->getValue();

	// parsing lines into Line and Vertex
	for (size_t j=0; j<line_count; j++)
	{
		Vertex * v0 = new Vertex(offsetcount);
		Vertex * v1 = new Vertex(offsetcount);
		for (size_t k=0; k<offsetcount; k++)
		{
			v0->m_data[k] = p_value[(size_t)(2*j*offsetcount + 0*offsetcount + k)];
			v1->m_data[k] = p_value[(size_t)(2*j*offsetcount + 1*offsetcount + k)];
		}
		Line * l = new Line(v0, v1);
		Line_List.push_back(l);
		Line_Container.push_back(l);
		Vertex_Container.push_back(v0);
		Vertex_Container.push_back(v1);
	}

	// create linestrips
	domUint linestrips_count=0;
	vector<Line*>::iterator iter;
	for (iter = Line_List.begin(); !Line_List.empty(); iter = Line_List.begin())
	{
		vector<Vertex *> vertex_list;

		Line *start_line = *iter;
		vertex_list.push_back(start_line->v[0]);
		vertex_list.push_back(start_line->v[1]);
		Line_List.erase(iter);

		Vertex *find_vertex = start_line->v[1];

		vector<Line*>::iterator jter;
		for (jter=Line_List.begin(); jter!= Line_List.end(); jter=Line_List.begin())
		{
			vector<Line*>::iterator kter;
			Vertex *match_vertex = NULL;
			for (kter=Line_List.begin(); kter!= Line_List.end(); kter++)
			{
				// find the next vertex, might update next_side and Triangle_list
				Line * test_line = *kter;
				if (*(test_line->v[0]) == (*find_vertex))
				{	// next vertex found
					match_vertex = test_line->v[1];
					vertex_list.push_back(match_vertex);
					Line_List.erase(kter);
					break;
				}
			}
			if (match_vertex)
			{	// matching vertext found, continue finding the next match_vertex
				find_vertex = match_vertex; 
				continue;
			} else
			{	// match_vertex not found, line_strips complete
				break;
			}
		}

		domP * newP = (domP *) linestrips->createAndPlace("p");
		for (size_t k=0; k<vertex_list.size(); k++)
		{
			Vertex * v = vertex_list[k];
			for (size_t l=0; l<offsetcount; l++)
			{
				newP->getValue().append(v->m_data[l]);
			}
		}
		vertex_list.clear();
		linestrips_count++;
	}

	linestrips->setCount(linestrips_count);

	// cleanup
	for (size_t i=0; i<Vertex_Container.size(); i++)
	{
		delete Vertex_Container[i];
	}
	for (size_t i=0; i<Line_Container.size(); i++)
	{
		delete Line_Container[i];
	}
	Line_Container.clear();
	Vertex_Container.clear();	
}


int Lines2linestrips::execute()
{
	string axis;
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

		// Loop over all the lines elements
		size_t lineCount = (int)(thisMesh->getLines_array().getCount());

		if (lineCount == 0)
			continue;

		for (size_t i=0; i<lineCount; i++)
		{
			domLines * lines = thisMesh->getLines_array()[i];
			ConvertLinesToLine(thisMesh, lines);
		}
		for (size_t i=0; i<lineCount; i++)
		{
			domLines * lines = thisMesh->getLines_array()[i];
			thisMesh->removeChildElement(lines);
		}
	}
	return 0;
}

Conditioner::Register<Lines2linestripsProxy> registerObject;
