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
#include "packedvertexindex.h"

packedVertexIndex::packedVertexIndex()
{
	references = 0;
	new_index_number = 0;
	index_count = 0;
}
packedVertexIndex::packedVertexIndex(int size)
{
	// Create the memory for storing the indices
	references = 0;
	new_index_number = 0;
	index_count = size;
	indices = new int[size];
}
packedVertexIndex::packedVertexIndex(const packedVertexIndex& rhs)
{
	references = 0;
	new_index_number = 0;
	indices = new int[rhs.index_count];
	memcpy(indices, rhs.indices, sizeof(int)*rhs.index_count);
	index_count = rhs.index_count;
}
packedVertexIndex& packedVertexIndex::operator=(const packedVertexIndex &rhs)
{
	if(this != &rhs)
	{
		if(index_count != rhs.index_count)
		{
			delete indices;
			indices = new int[rhs.index_count];
		}
		memcpy(indices, rhs.indices, sizeof(int)*rhs.index_count);
		references = rhs.references;
		new_index_number = rhs.new_index_number;
		index_count = rhs.index_count;
	}
	return *this;
}

packedVertexIndex::~packedVertexIndex()
{
	// free the memory containing indices
	delete indices;
	// Error marker
	indices = (int*)0x00badadd;
}
