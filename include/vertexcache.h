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
#ifndef __VERTEX_CACHE_H__
#define __VERTEX_CACHE_H__

#include <map>
using namespace std;

class simpleTriangle
{
public:
	int indices[3]; // indices for the triangle
};
class vertexCache
{
public:
	vertexCache(int size);
	virtual ~vertexCache();

	int costTriangle(simpleTriangle &tri);
	int addTriangle(simpleTriangle &tri);
	int addVertex(int vertexNumber);
	int costVertex(int vertexNumber);

	map<int,int> cacheState;
	unsigned int cacheSize;
	int cacheMisses;
	int cacheHits;
};
#endif //__VERTEX_CACHE_H__
