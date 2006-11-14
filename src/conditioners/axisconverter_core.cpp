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
#include "axisconverter.h"

daeString docName;
char debugmessage[1024];
#define PRINTF(X, Y)	if (verbose) {sprintf(debugmessage, X, Y); Axisconverter::printExecutionMessage(std::string(debugmessage));}
//#define PRINTF(X, Y) if (verbose) {sprintf(debugmessage, X, Y); Axisconverter::printExecutionMessage(std::string(X));}
//#define PRINTF(X) if (verbose) Axisconverter::printExecutionMessage(std::string(X));}
void float3_swap(domFloat3 &trans, int code, int flag, int verbose)
{
	domFloat3 trans1;
	trans1.setCount(3);
	trans1[0] = trans[0];
	trans1[1] = trans[1];
	trans1[2] = trans[2];
	PRINTF("inside float3 swap code is %d\n", code);
	if (flag == 0) {
		switch (code) 
		{
			case 1:
				trans[0] = trans1[1];
				trans[1] = -trans1[0];
				trans[2] = trans1[2];
				break;
			case 2:
				trans[0] = trans1[2];
				trans[1] = -trans1[0];
				trans[2] = -trans1[1];
				break;
			case 3:
				trans[0] = -trans1[1];
				trans[1] = trans1[0];
				trans[2] = trans1[2];
				break;
			case 4:
				trans[0] = trans1[0];
				trans[1] = trans1[2];
				trans[2] = -trans1[1];
				break;
			case 5:
				trans[0] = -trans1[1];
				trans[1] = -trans1[2];
				trans[2] = trans1[0];
				break;
			case 6:
				trans[0] = trans1[0];
				trans[1] = -trans1[2];
				trans[2] = trans1[1];
				break;
			default:
				PRINTF("invalid code=%d\n", code);
		}
	} else if (flag == 1) {
		switch (code) 
		{
			case 1:
				trans[0] = trans1[1];
				trans[1] = trans1[0];
				trans[2] = trans1[2];
				break;
			case 2:
				trans[0] = trans1[2];
				trans[1] = trans1[0];
				trans[2] = trans1[1];
				break;
			case 3:
				trans[0] = trans1[1];
				trans[1] = trans1[0];
				trans[2] = trans1[2];
				break;
			case 4:
				trans[0] = trans1[0];
				trans[1] = trans1[2];
				trans[2] = trans1[1];
				break;
			case 5:
				trans[0] = trans1[1];
				trans[1] = trans1[2];
				trans[2] = trans1[0];
				break;
			case 6:
				trans[0] = trans1[0];
				trans[1] = trans1[2];
				trans[2] = trans1[1];
				break;
			default:
				PRINTF("invalid code=%d\n", code);
		}
	}
}

void float4_swap(domFloat4 &rot, int code, int verbose)
{
    domFloat4 rot1;
	rot1.setCount(4);
	rot1[0] = rot[0];
	rot1[1] = rot[1];
	rot1[2] = rot[2];
	switch (code) 
	{
		case 1:
			rot[0] = rot1[1];
			rot[1] = -rot1[0];
			rot[2] = rot1[2];
			break;
		case 2:
			rot[0] = rot1[2];
			rot[1] = -rot1[0];
			rot[2] = -rot1[1];
			break;
		case 3:
			rot[0] = -rot1[1];
			rot[1] = rot1[0];
			rot[2] = rot1[2];
			break;
		case 4:
			rot[0] = rot1[0];
			rot[1] = rot1[2];
			rot[2] = -rot1[1];
			break;
		case 5:
			rot[0] = -rot1[1];
			rot[1] = -rot1[2];
			rot[2] = rot1[0];
			break;
		case 6:
			rot[0] = rot1[0];
			rot[1] = -rot1[2];
			rot[2] = rot1[1];
			break;
		default:
			PRINTF("invalid code=%d\n", code);
	}
}

void doublearray_swap(daeDoubleArray &array, int code, int verbose)
{
	daeDoubleArray array1;
	array1 = array;
	switch (code)
	{
		case 1:
			array[0] = array1[1];
			array[1] = -array1[0];
			array[2] = array1[2];
			break;
		case 2:
			array[0] = array1[2];
			array[1] = -array1[0];
			array[2] = -array1[1];
			break;
		case 3:
			array[0] = -array1[1];
			array[1] = array1[0];
			array[2] = array1[2];
			break;
		case 4:
			array[0] = array1[0];
			array[1] = array1[2];
			array[2] = -array1[1];
			break;
		case 5:
			array[0] = -array1[1];
			array[1] = -array1[2];
			array[2] = array1[0];
			break;
		case 6:
			array[0] = array1[0];
			array[1] = -array1[2];
			array[2] = array1[1];
			break;
		default:
			PRINTF("invalid code=%d\n", code);
	}
}

void process_node(domNode *node, int code, int verbose)
{
    // translate
	unsigned int translateCount = (unsigned int)node->getTranslate_array().getCount();
	PRINTF("translate count is %d\n", translateCount);
	for (unsigned i = 0; i < translateCount; i++ )
	{
		PRINTF("inside translate i=%d\n", i);
		domFloat3 &trans = node->getTranslate_array()[i]->getValue();
		float3_swap(trans, code, 0, verbose);
	}
	//rotate
	unsigned int rotateCount = (unsigned int)node->getRotate_array().getCount();
    PRINTF("rotate count is %d\n", rotateCount);
	for (unsigned int i = 0; i < rotateCount; i++ )
	{
		PRINTF("inside rotate i=%d\n", i);
		domFloat4 &rot = node->getRotate_array()[i]->getValue();
		float4_swap(rot, code, verbose);
	}

	//scale
	unsigned int scaleCount = (unsigned int)node->getScale_array().getCount();
	PRINTF("scale count is %d\n", scaleCount);
	for (unsigned int i = 0; i < scaleCount; i++ )
	{
		PRINTF("inside scale i=%d\n", i);
		domFloat3 &scale = node->getScale_array()[i]->getValue();
        float3_swap(scale, code, 1, verbose);
	} // scale
}

template<class T>
void process_mesh(T mesh, int code, int verbose)
{
	set <string> strset;
	set <string> strvertexset;
	set <string>::iterator si;

	// Loop over all the triangle assets in this mesh
	int trianglesAssetCount = (int) mesh->getTriangles_array().getCount();
	//PRINTF("there are %d triangles\n", trianglesAssetCount);
	for(int currentTriangles = 0; currentTriangles < trianglesAssetCount; currentTriangles++) {
		domTriangles *thisTriangles = mesh->getTriangles_array().get(currentTriangles);
		if (thisTriangles != NULL) {
			for(int j=0; j<(int)(thisTriangles->getInput_array().getCount()); j++)
			{
				xsNMTOKEN semantic = thisTriangles->getInput_array()[j]->getSemantic();
				PRINTF("input semantic value is %s\n", semantic);
				if (strcmp(semantic, "VERTEX") == 0) {
					daeString vertexSource = thisTriangles->getInput_array()[j]->getSource().getID();
					PRINTF("input vertext source value is %s\n", vertexSource);
					strvertexset.insert(vertexSource);
				}
				if ( (strcmp(semantic, "NORMAL") == 0)      ||
					    (strcmp(semantic, "TANGENT") == 0)     ||
					    (strcmp(semantic, "BINORMAL") == 0)    ||
					    (strcmp(semantic, "TEXTANGENT") == 0)  ||
					    (strcmp(semantic, "TEXBINORMAL") == 0) ) {
					daeString source = thisTriangles->getInput_array()[j]->getSource().getID();
					PRINTF("input normal source value is %s\n", source);
					strset.insert(source);
				}
			} // for
		} // if
	} // for

	// Loop over all the polygon assets in this mesh
	int polygonsAssetCount = (int)mesh->getPolygons_array().getCount();
	//PRINTF("there are %d polygons\n", polygonsAssetCount);
	for(int currentPolygons = 0; currentPolygons < polygonsAssetCount; currentPolygons++) {
		domPolygons *thisPolygons = mesh->getPolygons_array().get(currentPolygons);
		if (thisPolygons != NULL) {
			for(int j=0; j<(int)(thisPolygons->getInput_array().getCount()); j++)
			{
				xsNMTOKEN semantic = thisPolygons->getInput_array()[j]->getSemantic();
				PRINTF("input semantic value is %s\n", semantic);
				if (strcmp(semantic, "VERTEX") == 0) {
					daeString vertexSource = thisPolygons->getInput_array()[j]->getSource().getID();
					PRINTF("input vertex source value is %s\n", vertexSource);
					strvertexset.insert(vertexSource);
				}
				if ( (strcmp(semantic, "NORMAL") == 0)      ||
					    (strcmp(semantic, "TANGENT") == 0)     ||
					    (strcmp(semantic, "BINORMAL") == 0)    ||
					    (strcmp(semantic, "TEXTANGENT") == 0)  ||
					    (strcmp(semantic, "TEXBINORMAL") == 0) ) {
					daeString source = thisPolygons->getInput_array()[j]->getSource().getID();
					PRINTF("input normal source value is %s\n", source);
					strset.insert(source);
				}
			} // for input loop
		} // if
	} // for polygon loop

	// Loop over all the polylist assets in this mesh
	int polylistAssetCount = (int)mesh->getPolylist_array().getCount();
	//PRINTF("there are %d polylist\n", polylistAssetCount);
	for(int currentPolylist = 0; currentPolylist < polylistAssetCount; currentPolylist++) {
		domPolylist *thisPolylist = mesh->getPolylist_array().get(currentPolylist);
		if (thisPolylist != NULL) {
			for(int j=0; j<(int)(thisPolylist->getInput_array().getCount()); j++)
			{
				xsNMTOKEN semantic = thisPolylist->getInput_array()[j]->getSemantic();
				PRINTF("input semantic value is %s\n", semantic);
				if (strcmp(semantic, "VERTEX") == 0) {
					daeString vertexSource = thisPolylist->getInput_array()[j]->getSource().getID();
					PRINTF("input vertex source value is %s\n", vertexSource);
					strvertexset.insert(vertexSource);
				}
				if ( (strcmp(semantic, "NORMAL") == 0)      ||
					    (strcmp(semantic, "TANGENT") == 0)     ||
					    (strcmp(semantic, "BINORMAL") == 0)    ||
					    (strcmp(semantic, "TEXTANGENT") == 0)  ||
					    (strcmp(semantic, "TEXBINORMAL") == 0) ) {
					daeString source = thisPolylist->getInput_array()[j]->getSource().getID();
					PRINTF("input normal source value is %s\n", source);
					strset.insert(source);
				}
			} // for input loop
		} // if
	} // for polylist loop

	// Loop over all the tristrips assets in this mesh
	int tristripsAssetCount = (int)mesh->getTristrips_array().getCount();
	//PRINTF("there are %d tristrips\n", tristripsAssetCount);
	for(int currentTristrips = 0; currentTristrips < tristripsAssetCount; currentTristrips++) {
		domTristrips *thisTristrips = mesh->getTristrips_array().get(currentTristrips);
		if (thisTristrips != NULL) {
			for(int j=0; j<(int)(thisTristrips->getInput_array().getCount()); j++)
			{
				xsNMTOKEN semantic = thisTristrips->getInput_array()[j]->getSemantic();
				PRINTF("input semantic value is %s\n", semantic);
				if (strcmp(semantic, "VERTEX") == 0) {
					daeString vertexSource = thisTristrips->getInput_array()[j]->getSource().getID();
					PRINTF("input vertext source value is %s\n", vertexSource);
					strvertexset.insert(vertexSource);
				}
				if ( (strcmp(semantic, "NORMAL") == 0)      ||
					    (strcmp(semantic, "TANGENT") == 0)     ||
					    (strcmp(semantic, "BINORMAL") == 0)    ||
					    (strcmp(semantic, "TEXTANGENT") == 0)  ||
					    (strcmp(semantic, "TEXBINORMAL") == 0) ) {
					daeString source = thisTristrips->getInput_array()[j]->getSource().getID();
					PRINTF("input normal source value is %s\n", source);
					strset.insert(source);
				}
			} // for
		} // if
	} // for

	// Loop over all the trifans assets in this mesh
	int trifansAssetCount = (int)mesh->getTrifans_array().getCount();
	//PRINTF("there are %d trifans\n", trifansAssetCount);
	for(int currentTrifans = 0; currentTrifans < trifansAssetCount; currentTrifans++) {
		domTrifans *thisTrifans = mesh->getTrifans_array().get(currentTrifans);
		if (thisTrifans != NULL) {
			for(int j=0; j<(int)(thisTrifans->getInput_array().getCount()); j++)
			{
				xsNMTOKEN semantic = thisTrifans->getInput_array()[j]->getSemantic();
				PRINTF("input semantic value is %s\n", semantic);
				if (strcmp(semantic, "VERTEX") == 0) {
					daeString vertexSource = thisTrifans->getInput_array()[j]->getSource().getID();
					PRINTF("input vertext source value is %s\n", vertexSource);
					strvertexset.insert(vertexSource);
				}
				if ( (strcmp(semantic, "NORMAL") == 0)      ||
					    (strcmp(semantic, "TANGENT") == 0)     ||
					    (strcmp(semantic, "BINORMAL") == 0)    ||
					    (strcmp(semantic, "TEXTANGENT") == 0)  ||
					    (strcmp(semantic, "TEXBINORMAL") == 0) ) {
					daeString source = thisTrifans->getInput_array()[j]->getSource().getID();
					PRINTF("input normal source value is %s\n", source);
					strset.insert(source);
				}
			} // for
		} // if
	} // for

	// Loop over all the lines assets in this mesh
	int linesAssetCount = (int)mesh->getLines_array().getCount();
	//PRINTF("there are %d lines\n", linesAssetCount);
	for(int currentLines = 0; currentLines < linesAssetCount; currentLines++) {
		domLines *thisLines = mesh->getLines_array().get(currentLines);
		if (thisLines != NULL) {
			for(int j=0; j<(int)(thisLines->getInput_array().getCount()); j++)
			{
				xsNMTOKEN semantic = thisLines->getInput_array()[j]->getSemantic();
				PRINTF("input semantic value is %s\n", semantic);
				if (strcmp(semantic, "VERTEX") == 0) {
					daeString vertexSource = thisLines->getInput_array()[j]->getSource().getID();
					PRINTF("input vertext source value is %s\n", vertexSource);
					strvertexset.insert(vertexSource);
				}
				if ( (strcmp(semantic, "NORMAL") == 0)      ||
					    (strcmp(semantic, "TANGENT") == 0)     ||
					    (strcmp(semantic, "BINORMAL") == 0)    ||
					    (strcmp(semantic, "TEXTANGENT") == 0)  ||
					    (strcmp(semantic, "TEXBINORMAL") == 0) ) {
					daeString source = thisLines->getInput_array()[j]->getSource().getID();
					PRINTF("input normal source value is %s\n", source);
					strset.insert(source);
				}
			} // for
		} // if
	} // for

	// Loop over all the linestrips assets in this mesh
	int linestripsAssetCount = (int)mesh->getLinestrips_array().getCount();
	//PRINTF("there are %d linestrips\n", linestripsAssetCount);
	for(int currentLinestrips = 0; currentLinestrips < linestripsAssetCount; currentLinestrips++) {
		domLinestrips *thisLinestrips = mesh->getLinestrips_array().get(currentLinestrips);
		if (thisLinestrips != NULL) {
			for(int j=0; j<(int)(thisLinestrips->getInput_array().getCount()); j++)
			{
				xsNMTOKEN semantic = thisLinestrips->getInput_array()[j]->getSemantic();
				PRINTF("input semantic value is %s\n", semantic);
				if (strcmp(semantic, "VERTEX") == 0) {
					daeString vertexSource = thisLinestrips->getInput_array()[j]->getSource().getID();
					PRINTF("input vertext source value is %s\n", vertexSource);
					strvertexset.insert(vertexSource);
				}
				if ( (strcmp(semantic, "NORMAL") == 0)      ||
					    (strcmp(semantic, "TANGENT") == 0)     ||
					    (strcmp(semantic, "BINORMAL") == 0)    ||
					    (strcmp(semantic, "TEXTANGENT") == 0)  ||
					    (strcmp(semantic, "TEXBINORMAL") == 0) ) {
					daeString source = thisLinestrips->getInput_array()[j]->getSource().getID();
					PRINTF("input normal source value is %s\n", source);
					strset.insert(source);
				}
			} // for
		} // if
	} // for

	// check <vertice>
	domVerticesRef vertice = mesh->getVertices();
	if (vertice != NULL) {
		daeString verticeId = vertice->getID();
		for (si=strvertexset.begin(); si!=strvertexset.end(); si++)  {
			if (strcmp(verticeId, (*si).c_str()) == 0) {
				for(int j=0; j<(int)(vertice->getInput_array().getCount()); j++) {
					xsNMTOKEN semantic = vertice->getInput_array()[j]->getSemantic();
					if ( (strcmp(semantic, "POSITION") == 0) || (strcmp(semantic, "NORMAL") == 0) ) {
						daeString source = vertice->getInput_array()[j]->getSource().getID();
						PRINTF("vertex input normal source value is %s\n", source);
						strset.insert(source);
					}
				} // for input loop
				break;
			}
		} // for set iteration
	} // if

	// check <source> in <mesh>
	int sourceCount = (int)mesh->getSource_array().getCount();
	for (int currentSource = 0; currentSource < sourceCount; currentSource++) {
		daeString sourceId = mesh->getSource_array()[currentSource]->getID();
		PRINTF("source id is %s\n", sourceId);
		for (si=strset.begin(); si!=strset.end(); si++)  {
			if (strcmp(sourceId, (*si).c_str()) == 0) {
				PRINTF("find match %s\n", sourceId);
				daeDoubleArray &float_array = mesh->getSource_array()[currentSource]->getFloat_array()->getValue();
				int count = (int)float_array.getCount();
				PRINTF("float array count is %d\n", count);
				daeDoubleArray float_array_1 = float_array;
//				PRINTF("copy value\n");
                int number = count/3;
				for (int i = 0; i < number; i++) {
					switch (code) 
					{
						case 1:
							float_array[i*3] = float_array_1[i*3+1];
							float_array[i*3+1] = -float_array_1[i*3];
							float_array[i*3+2] = float_array_1[i*3+2];
							break;
						case 2:
							float_array[i*3] = float_array_1[i*3+2];
							float_array[i*3+1] = -float_array_1[i*3];
							float_array[i*3+2] = -float_array_1[i*3+1];
							break;
						case 3:
							float_array[i*3] = -float_array_1[i*3+1];
							float_array[i*3+1] = float_array_1[i*3];
							float_array[i*3+2] = float_array_1[i*3+2];
							break;
						case 4:
							float_array[i*3] = float_array_1[i*3];
							float_array[i*3+1] = float_array_1[i*3+2];
							float_array[i*3+2] = -float_array_1[i*3+1];
							break;
						case 5:
							float_array[i*3] = -float_array_1[i*3+1];
							float_array[i*3+1] = -float_array_1[i*3+2];
							float_array[i*3+2] = float_array_1[i*3];
							break;
						case 6:
							float_array[i*3] = float_array_1[i*3];
							float_array[i*3+1] = -float_array_1[i*3+2];
							float_array[i*3+2] = float_array_1[i*3+1];
							break;
						default:
							PRINTF("invalid code=%d\n", code);
					}						
				} // for
//				PRINTF("done\n");
			} // if
		} // for
	} // for
}

void process_shape(domRigid_body::domTechnique_common::domShape *shape, int code, int verbose)
{
	// change the equation value for plane
	domPlaneRef plane = shape->getPlane();
	if (plane != NULL) {
		domPlane::domEquationRef equation = plane->getEquation();
		domFloat4 &value = equation->getValue();
		float4_swap(value, code, verbose);
	}
	// change the half_extent value for box
	domBoxRef box = shape->getBox();
	if (box != NULL) {
		domBox::domHalf_extentsRef half = box->getHalf_extents();
		domFloat3 &value = half->getValue();
		float3_swap(value, code, 1, verbose);
	}

	// translate
	unsigned int translateCount = (unsigned int)shape->getTranslate_array().getCount();
	PRINTF("translate count is %d\n", translateCount);
	for (unsigned i = 0; i < translateCount; i++ )
	{
		PRINTF("inside translate i=%d\n", i);
		domFloat3 &trans = shape->getTranslate_array()[i]->getValue();
        float3_swap(trans, code, 0, verbose);
	}
	// rotate
	// for capsule/cylinder/tapered_capsule/tapered_cylinder, need to rotate first since they are aligned along local y axis by default
    bool extra_rotate = false;
	domCapsuleRef capsule = shape->getCapsule();
	domCylinderRef cylinder = shape->getCylinder();
	domTapered_capsuleRef tapered_capsule = shape->getTapered_capsule();
	domTapered_cylinderRef tapered_cylinder = shape->getTapered_cylinder();
	if ((capsule != NULL) || (cylinder != NULL) || (tapered_capsule != NULL) || (tapered_cylinder != NULL)) {
		extra_rotate = true;
	}
	unsigned int rotateCount = (unsigned int)shape->getRotate_array().getCount();
    PRINTF("rotate count is %d\n", rotateCount);
	for (unsigned int i = 0; i < rotateCount; i++ )
	{
		PRINTF("inside shape rotate i=%d\n", i);
		domFloat4 &rot = shape->getRotate_array()[i]->getValue();
		float4_swap(rot, code, verbose);
		if (extra_rotate) {
//			PRINTF("extra rotate\n");
			switch (code) {
				case 1:
					if (rot[2] == 1)
						rot[3] -= 90.0;
					else if (rot[2] == -1)
						rot[3] += 90.0;
					break;
				case 2:
					if (rot[0] == 1)
						rot[3] -= 90.0;
					else if (rot[0] == -1)
						rot[3] += 90.0;
					break;
				case 3:
					if (rot[2] == 1) 
						rot[3] += 90.0;
					else if (rot[2] == -1)
						rot[3] -= 90.0;
					break;
				case 4:
					if (rot[0] == 1)
						rot[3] -= 90.0;
					else if (rot[0] == -1)
						rot[3] += 90.0;
					break;
				case 5:
					if (rot[2] == 1)
						rot[3] += 90.0;
					else if (rot[2] == -1)
						rot[3] -= 90.0;
					break;
				case 6:
					if (rot[0] == 1)
						rot[3] = rot[3] + 90.0;
					else if (rot[0] < 0) 
						rot[3] = rot[3] - 90.0;					
					break;
				default:
					PRINTF("invalid code=%d\n", code);
			}
		}
	}
}

void process_rigid_constraint(domRigid_constraint *constraint, int code, int verbose)
{
    // check ref_attachment
	// translate
	unsigned int translateCount = (unsigned int)constraint->getRef_attachment()->getTranslate_array().getCount();
	PRINTF("translate count is %d\n", translateCount);
	for (unsigned i = 0; i < translateCount; i++ )
	{
		PRINTF("inside translate i=%d\n", i);
		domFloat3 &trans = constraint->getRef_attachment()->getTranslate_array()[i]->getValue();
        float3_swap(trans, code, 0, verbose);
	}
	//rotate
	unsigned int rotateCount = (unsigned int)constraint->getRef_attachment()->getRotate_array().getCount();
    PRINTF("rotate count is %d\n", rotateCount);
	for (unsigned int i = 0; i < rotateCount; i++ )
	{
		PRINTF("inside rotate i=%d\n", i);
		domFloat4 &rot = constraint->getRef_attachment()->getRotate_array()[i]->getValue();
        float4_swap(rot, code, verbose);
	}
	// check attachment
	// translate
	translateCount = (unsigned int)constraint->getAttachment()->getTranslate_array().getCount();
	PRINTF("translate count is %d\n", translateCount);
	for (unsigned i = 0; i < translateCount; i++ )
	{
		PRINTF("inside translate i=%d\n", i);
		domFloat3 &trans = constraint->getAttachment()->getTranslate_array()[i]->getValue();
        float3_swap(trans, code, 0, verbose);
	}
	//rotate
	rotateCount = (unsigned int)constraint->getAttachment()->getRotate_array().getCount();
    PRINTF("rotate count is %d\n", rotateCount);
	for (unsigned int i = 0; i < rotateCount; i++ )
	{
		PRINTF("inside rotate i=%d\n", i);
		domFloat4 &rot = constraint->getAttachment()->getRotate_array()[i]->getValue();
        float4_swap(rot, code, verbose);
	}
	// swing_cone_and_twist/linear
	daeDoubleArray &maxSwingArray = constraint->getTechnique_common()->getLimits()->getSwing_cone_and_twist()->getMax()->getValue();
	doublearray_swap(maxSwingArray, code, verbose);
	daeDoubleArray &minSwingArray = constraint->getTechnique_common()->getLimits()->getSwing_cone_and_twist()->getMin()->getValue();
	doublearray_swap(minSwingArray, code, verbose);

	daeDoubleArray &maxLinearArray = constraint->getTechnique_common()->getLimits()->getLinear()->getMax()->getValue();
	doublearray_swap(maxLinearArray, code, verbose);
	daeDoubleArray &minLinearArray = constraint->getTechnique_common()->getLimits()->getLinear()->getMin()->getValue();
	doublearray_swap(minLinearArray, code, verbose);
}

void process_spline(domSpline *spline, int code, int verbose)
{
	set <string> strpositionset;
	set <string>::iterator si;

	domSpline::domControl_verticesRef cv = spline->getControl_vertices();

	int count = (int)cv->getInput_array().getCount();
	for (int i = 0; i < count; i++) {
		xsNMTOKEN semantic = cv->getInput_array()[i]->getSemantic();
		if (strcmp(semantic, "POSITION") == 0) {
			daeString positionSource = cv->getInput_array()[i]->getSource().getID();
			strpositionset.insert(positionSource);
		}
	}

	int splineSourceCount = (int)spline->getSource_array().getCount();
	for (int currentSource = 0; currentSource < splineSourceCount; currentSource++) {
		daeString sourceId = spline->getSource_array()[currentSource]->getID();
		PRINTF("spline source id is %s\n", sourceId);
		for (si=strpositionset.begin(); si!=strpositionset.end(); si++)  {
			if (strcmp(sourceId, (*si).c_str()) == 0) {
//				PRINTF("find match in spline\n");
				daeDoubleArray &float_array = spline->getSource_array()[currentSource]->getFloat_array()->getValue();
				int count = (int)float_array.getCount();
				PRINTF("float array count is %d\n", count);
				daeDoubleArray float_array_1 = float_array;
//				PRINTF("copy value\n");
				int number = count/3;
				for (int i = 0; i < number; i++) {
					switch (code) 
					{
						case 1:
							float_array[i*3] = float_array_1[i*3+1];
							float_array[i*3+1] = -float_array_1[i*3];
							float_array[i*3+2] = float_array_1[i*3+2];
							break;
						case 2:
							float_array[i*3] = float_array_1[i*3+2];
							float_array[i*3+1] = -float_array_1[i*3];
							float_array[i*3+2] = -float_array_1[i*3+1];
							break;
						case 3:
							float_array[i*3] = -float_array_1[i*3+1];
							float_array[i*3+1] = float_array_1[i*3];
							float_array[i*3+2] = float_array_1[i*3+2];
							break;
						case 4:
							float_array[i*3] = float_array_1[i*3];
							float_array[i*3+1] = float_array_1[i*3+2];
							float_array[i*3+2] = -float_array_1[i*3+1];
							break;
						case 5:
							float_array[i*3] = -float_array_1[i*3+1];
							float_array[i*3+1] = -float_array_1[i*3+2];
							float_array[i*3+2] = float_array_1[i*3];
							break;
						case 6:
							float_array[i*3] = float_array_1[i*3];
							float_array[i*3+1] = -float_array_1[i*3+2];
							float_array[i*3+2] = float_array_1[i*3+1];
							break;
						default:
							PRINTF("invalid code=%d\n", code);
					}						
				} // for
//				PRINTF("done\n");
			} // if
		} // for
	} // for
}

void common_convert(DAE *input, int code, int verbose)
{
	domNode *thisNode;
    domTranslateRef currTranslate;
	domRotateRef _currRotateX;
	domRotateRef _currRotateY;
	domRotateRef _currRotateZ;
	domScaleRef _currScale;
	set <string> stranimationset;
	set <string>::iterator si;
    map <string, string> sourcemap;
	map <string, string>::iterator mi;

	int error = 0;
	unsigned int nodeCount = input->getDatabase()->getElementCount(NULL, COLLADA_ELEMENT_NODE, docName);
    PRINTF("node count is %d\n", nodeCount);    
	// node
	for (unsigned int j = 0; j < nodeCount; j++) {
		error = input->getDatabase()->getElement((daeElement**)&thisNode, j, NULL, COLLADA_ELEMENT_NODE, docName);
		PRINTF("get node %d\n", j);
		if (error != DAE_OK)
		{
			exit(-1);
		}
		process_node(thisNode, code, verbose);
	} // for

	// check <geometry>
	unsigned int geometryCount = input->getDatabase()->getElementCount(NULL, COLLADA_ELEMENT_GEOMETRY, docName);
    PRINTF("geometry count is %d\n", geometryCount); 
	for (unsigned int i = 0; i < geometryCount; i++) {
		domGeometry *thisGeometry;
		error = input->getDatabase()->getElement((daeElement**)&thisGeometry,i, NULL, "geometry", docName);
		if (error != DAE_OK) {
			//exit(-1);
		}

		// Get the geometry element's mesh
		domMesh *thisMesh = thisGeometry->getMesh();
		if (thisMesh != NULL)
			process_mesh(thisMesh, code, verbose);

		domSpline *thisSpline = thisGeometry->getSpline();
		if (thisSpline != NULL)
			process_spline(thisSpline, code, verbose);

        domConvex_mesh *thisConvexMesh = thisGeometry->getConvex_mesh();
        if (thisConvexMesh == NULL)
			continue;
		else if (thisConvexMesh->hasAttribute("attrConvex_hull_of") == false) {
//			PRINTF("process convex_mesh\n");
			process_mesh(thisConvexMesh, code, verbose);
		}
	} // for geometry

	daeDoubleArray float_array_x1_1;
	daeDoubleArray float_array_x2_1;
	daeDoubleArray float_array_x3_1;
	daeDoubleArray float_array_y1_1;
	daeDoubleArray float_array_y2_1;
	daeDoubleArray float_array_y3_1;
	daeDoubleArray float_array_z1_1;
	daeDoubleArray float_array_z2_1;
	daeDoubleArray float_array_z3_1;

	// check <animation>
	unsigned int animationCount = input->getDatabase()->getElementCount(NULL, COLLADA_ELEMENT_ANIMATION, docName);
    PRINTF("animation count is %d\n", animationCount); 
	for (unsigned int i = 0; i < animationCount; i++) {
		domAnimation *thisAnimation;
		error = input->getDatabase()->getElement((daeElement**)&thisAnimation,i, NULL, "animation", docName);
		if (error != DAE_OK) {
			//exit(-1);
		}
		int channelCount = (int)thisAnimation->getChannel_array().getCount();
		PRINTF("channel count is %d\n", channelCount);
        // one channel per animation?
		xsToken target = thisAnimation->getChannel_array()[0]->getTarget();
		// check to see it is tranalate or rotate
        char *targetValue = const_cast<char*>(strrchr( target, '/' ));
		if (targetValue != NULL) {
			PRINTF("target value is %s\n", targetValue);
			if ( (strcmp(targetValue, "/translate") == 0) || (strcmp(targetValue, "/scale") == 0) ) {
				// translate/scale animation
				int samplerCount = (int)thisAnimation->getSampler_array().getCount();
				PRINTF("sampler count is %d\n", samplerCount);
				for (int j = 0; j < samplerCount; j++) {
					int inputCount = (int)thisAnimation->getSampler_array()[j]->getInput_array().getCount();
					for (int k = 0; k < inputCount; k++) {
						xsNMTOKEN semantic = thisAnimation->getSampler_array()[j]->getInput_array()[k]->getSemantic();
						if ( (strcmp(semantic, "OUTPUT") == 0) ||
							(strcmp(semantic, "IN_TANGENT") == 0) ||
							(strcmp(semantic, "OUT_TANGENT") == 0) ) {
								daeString source = thisAnimation->getSampler_array()[j]->getInput_array()[k]->getSource().getID();
								PRINTF("sampler input source value is %s\n", source);
								stranimationset.insert(source);
						} // if
					} //for
				} // for

				// check <source> in <animation>
				int animationSourceCount = (int)thisAnimation->getSource_array().getCount();
				for (int currentSource = 0; currentSource < animationSourceCount; currentSource++) {
					daeString sourceId = thisAnimation->getSource_array()[currentSource]->getID();
					PRINTF("animation source id is %s\n", sourceId);
					for (si=stranimationset.begin(); si!=stranimationset.end(); si++)  {
						if (strcmp(sourceId, (*si).c_str()) == 0) {
//							PRINTF("find match in animation\n");
							daeDoubleArray &float_array = thisAnimation->getSource_array()[currentSource]->getFloat_array()->getValue();
							int count = (int)float_array.getCount();
							PRINTF("float array count is %d\n", count);
							daeDoubleArray float_array_1 = float_array;
//							PRINTF("copy value\n");
							int number = count/3;
							for (int i = 0; i < number; i++) {
								switch (code) 
								{
									case 1:
										float_array[i*3] = float_array_1[i*3+1];
										float_array[i*3+1] = -float_array_1[i*3];
										float_array[i*3+2] = float_array_1[i*3+2];
										break;
									case 2:
										float_array[i*3] = float_array_1[i*3+2];
										float_array[i*3+1] = -float_array_1[i*3];
										float_array[i*3+2] = -float_array_1[i*3+1];
										break;
									case 3:
										float_array[i*3] = -float_array_1[i*3+1];
										float_array[i*3+1] = float_array_1[i*3];
										float_array[i*3+2] = float_array_1[i*3+2];
										break;
									case 4:
										float_array[i*3] = float_array_1[i*3];
										float_array[i*3+1] = float_array_1[i*3+2];
										float_array[i*3+2] = -float_array_1[i*3+1];
										break;
									case 5:
										float_array[i*3] = -float_array_1[i*3+1];
										float_array[i*3+1] = -float_array_1[i*3+2];
										float_array[i*3+2] = float_array_1[i*3];
										break;
									case 6:
										float_array[i*3] = float_array_1[i*3];
										float_array[i*3+1] = -float_array_1[i*3+2];
										float_array[i*3+2] = float_array_1[i*3+1];
										break;
									default:
//										PRINTF("invalid code\n");
										break;
								}						
							} // for
//							PRINTF("done\n");
						} // if
					} // for
				} // for
			} else {
				// rotate animation
				if (strcmp(targetValue, "/rotateX.ANGLE") == 0) {
					int samplerCount = (int)thisAnimation->getSampler_array().getCount();
					PRINTF("rotateX sampler count is %d\n", samplerCount);
					// one sampler per animation?
					int inputCount = (int)thisAnimation->getSampler_array()[0]->getInput_array().getCount();
					for (int k = 0; k < inputCount; k++) {
						xsNMTOKEN semantic = thisAnimation->getSampler_array()[0]->getInput_array()[k]->getSemantic();
						daeString source = thisAnimation->getSampler_array()[0]->getInput_array()[k]->getSource().getID();
						PRINTF("rotateX sampler input source value is %s\n", source);	
						if (strcmp(semantic, "OUTPUT") == 0) {
							sourcemap.insert(pair<string,string>("OUTPUT_X", source));
						} else if (strcmp(semantic, "IN_TANGENT") == 0) {
                            sourcemap.insert(pair<string, string>("IN_TANGENT_X", source));
						} else if (strcmp(semantic, "OUT_TANGENT") == 0) {
                            sourcemap.insert(pair<string, string>("OUT_TANGENT_X", source));
						} // if
					} //for input
				} else if (strcmp(targetValue, "/rotateY.ANGLE") == 0) {
					int samplerCount = (int)thisAnimation->getSampler_array().getCount();
					PRINTF("rotateX sampler count is %d\n", samplerCount);
					// one sampler per animation?
					int inputCount = (int)thisAnimation->getSampler_array()[0]->getInput_array().getCount();
					for (int k = 0; k < inputCount; k++) {
						xsNMTOKEN semantic = thisAnimation->getSampler_array()[0]->getInput_array()[k]->getSemantic();
						daeString source = thisAnimation->getSampler_array()[0]->getInput_array()[k]->getSource().getID();
						PRINTF("rotateX sampler input source value is %s\n", source);	
						if (strcmp(semantic, "OUTPUT") == 0) {
							sourcemap.insert(pair<string, string>("OUTPUT_Y", source));
						} else if (strcmp(semantic, "IN_TANGENT") == 0) {
                            sourcemap.insert(pair<string, string>("IN_TANGENT_Y", source));
						} else if (strcmp(semantic, "OUT_TANGENT") == 0) {
                            sourcemap.insert(pair<string, string>("OUT_TANGENT_Y", source));
						} // if
					} //for input				
				} else if (strcmp(targetValue, "/rotateZ.ANGLE") == 0) {
					int samplerCount = (int)thisAnimation->getSampler_array().getCount();
					PRINTF("rotateX sampler count is %d\n", samplerCount);
					// one sampler per animation?
					int inputCount = (int)thisAnimation->getSampler_array()[0]->getInput_array().getCount();
					for (int k = 0; k < inputCount; k++) {
						xsNMTOKEN semantic = thisAnimation->getSampler_array()[0]->getInput_array()[k]->getSemantic();
						daeString source = thisAnimation->getSampler_array()[0]->getInput_array()[k]->getSource().getID();
						PRINTF("rotateX sampler input source value is %s\n", source);	
						if (strcmp(semantic, "OUTPUT") == 0) {
							sourcemap.insert(pair<string, string>("OUTPUT_Z", source));
						} else if (strcmp(semantic, "IN_TANGENT") == 0) {
                            sourcemap.insert(pair<string, string>("IN_TANGENT_Z", source));
						} else if (strcmp(semantic, "OUT_TANGENT") == 0) {
                            sourcemap.insert(pair<string, string>("OUT_TANGENT_Z", source));
						} // if
					} //for input				
				}
				int animationSourceCount = (int)thisAnimation->getSource_array().getCount();
				for (int currentSource = 0; currentSource < animationSourceCount; currentSource++) {
					daeString sourceId = thisAnimation->getSource_array()[currentSource]->getID();
					PRINTF("animation source id is %s\n", sourceId);
					for (mi = sourcemap.begin(); mi != sourcemap.end(); mi++) {
						//PRINTF("map entry is %s : %s\n", (*mi).first.c_str(), (*mi).second.c_str());
						if (strcmp(sourceId, (*mi).second.c_str()) == 0) {
							if (strcmp((*mi).first.c_str(), "OUTPUT_X") == 0) {
								daeDoubleArray &float_array_x1= thisAnimation->getSource_array()[currentSource]->getFloat_array()->getValue();
								float_array_x1_1 = float_array_x1;
							} else if (strcmp((*mi).first.c_str(), "IN_TANGENT_X") == 0) {
								daeDoubleArray &float_array_x2= thisAnimation->getSource_array()[currentSource]->getFloat_array()->getValue();
								float_array_x2_1 = float_array_x2;
							} else if (strcmp((*mi).first.c_str(), "OUT_TANGENT_X") == 0) {
								daeDoubleArray &float_array_x3= thisAnimation->getSource_array()[currentSource]->getFloat_array()->getValue();								
								float_array_x3_1 = float_array_x3;
							} else if (strcmp((*mi).first.c_str(), "OUTPUT_Y") == 0) {
								daeDoubleArray &float_array_y1= thisAnimation->getSource_array()[currentSource]->getFloat_array()->getValue();	
								float_array_y1_1 = float_array_y1;
					        } else if (strcmp((*mi).first.c_str(), "IN_TANGENT_Y") == 0) {
								daeDoubleArray &float_array_y2= thisAnimation->getSource_array()[currentSource]->getFloat_array()->getValue();
								float_array_y2_1 = float_array_y2;
							} else if (strcmp((*mi).first.c_str(), "OUT_TANGENT_Y") == 0) {
								daeDoubleArray &float_array_y3= thisAnimation->getSource_array()[currentSource]->getFloat_array()->getValue();
								float_array_y3_1 = float_array_y3;
							} else if (strcmp((*mi).first.c_str(), "OUTPUT_Z") == 0) {
								daeDoubleArray &float_array_z1= thisAnimation->getSource_array()[currentSource]->getFloat_array()->getValue();	
								float_array_z1_1 = float_array_z1;
						    } else if (strcmp((*mi).first.c_str(), "IN_TANGENT_Z") == 0) {
							    daeDoubleArray &float_array_z2= thisAnimation->getSource_array()[currentSource]->getFloat_array()->getValue();
								float_array_z2_1 = float_array_z2;
							} else if (strcmp((*mi).first.c_str(), "OUT_TANGENT_Z") == 0) {
								daeDoubleArray &float_array_z3= thisAnimation->getSource_array()[currentSource]->getFloat_array()->getValue();	
								float_array_z3_1 = float_array_z3;
							} // if
						} // if
					} // for map
				} // for source
			} //else
		} // if
	} // for animation

	for (unsigned int i = 0; i < animationCount; i++) {
		domAnimation *thisAnimation;
		error = input->getDatabase()->getElement((daeElement**)&thisAnimation,i, NULL, "animation", docName);
		if (error != DAE_OK) {
			//exit(-1);
		}
		int animationSourceCount = (int)thisAnimation->getSource_array().getCount();
		for (int currentSource = 0; currentSource < animationSourceCount; currentSource++) {
			daeString sourceId = thisAnimation->getSource_array()[currentSource]->getID();
			PRINTF("animation source id is %s\n", sourceId);
			for (mi = sourcemap.begin(); mi != sourcemap.end(); mi++) {
				if (sourceId == (*mi).second) {
					if (strcmp((*mi).first.c_str(), "OUTPUT_X") == 0) {
						daeDoubleArray &float_array_x1= thisAnimation->getSource_array()[currentSource]->getFloat_array()->getValue();
						switch (code) 
						{
							case 1:
								for ( unsigned int a = 0; a < float_array_y1_1.getCount(); a++) {
										float_array_x1[a] = float_array_y1_1[a];
								}
								break;
							case 2:
								for ( unsigned int a = 0; a < float_array_z1_1.getCount(); a++) {
										float_array_x1[a] = float_array_z1_1[a];
								}
								break;
							case 3:
								for ( unsigned int a = 0; a < float_array_y1_1.getCount(); a++) {
										float_array_x1[a] = -float_array_y1_1[a];
								}
								break;
							case 4:
//								PRINTF("do nothing\n");
								break;
							case 5:
								for ( unsigned int a = 0; a < float_array_y1_1.getCount(); a++) {
										float_array_x1[a] = -float_array_y1_1[a];
								}
								break;
							case 6:
//								PRINTF("do nothing\n");
								break;
							default :
								PRINTF("invalid code=%d\n", code);
						}
					} else if (strcmp((*mi).first.c_str(), "IN_TANGENT_X") == 0) {
						daeDoubleArray &float_array_x2= thisAnimation->getSource_array()[currentSource]->getFloat_array()->getValue();
						switch (code) 
						{
							case 1:
								for ( unsigned int a = 0; a < float_array_y2_1.getCount(); a++) {
										float_array_x2[a] = float_array_y2_1[a];
								}
								break;
							case 2:
								for ( unsigned int a = 0; a < float_array_z2_1.getCount(); a++) {
										float_array_x2[a] = float_array_z2_1[a];
								}
								break;
							case 3:
								for ( unsigned int a = 0; a < float_array_y2_1.getCount(); a++) {
										float_array_x2[a] = -float_array_y2_1[a];
								}
								break;
							case 4:
//								PRINTF("do nothing\n");
								break;
							case 5:
								for ( unsigned int a = 0; a < float_array_y2_1.getCount(); a++) {
										float_array_x2[a] = -float_array_y2_1[a];
								}
								break;
							case 6:
//								PRINTF("do nothing\n");
								break;
							default :
								PRINTF("invalid code=%d\n", code);
						}
					} else if (strcmp((*mi).first.c_str(), "OUT_TANGENT_X") == 0) {
						daeDoubleArray &float_array_x3= thisAnimation->getSource_array()[currentSource]->getFloat_array()->getValue();								
						switch (code) 
						{
							case 1:
								for ( unsigned int a = 0; a < float_array_y3_1.getCount(); a++) {
										float_array_x3[a] = float_array_y3_1[a];
								}
								break;
							case 2:
								for ( unsigned int a = 0; a < float_array_z3_1.getCount(); a++) {
										float_array_x3[a] = float_array_z3_1[a];
								}
								break;
							case 3:
								for ( unsigned int a = 0; a < float_array_y3_1.getCount(); a++) {
										float_array_x3[a] = -float_array_y3_1[a];
								}
								break;
							case 4:
//								PRINTF("do nothing\n");
								break;
							case 5:
								for ( unsigned int a = 0; a < float_array_y3_1.getCount(); a++) {
										float_array_x3[a] = -float_array_y3_1[a];
								}
								break;
							case 6:
//								PRINTF("do nothing\n");
								break;
							default :
//								PRINTF("invalid code\n");
										break;
						}						
					} else if (strcmp((*mi).first.c_str(), "OUTPUT_Y") == 0) {
						daeDoubleArray &float_array_y1= thisAnimation->getSource_array()[currentSource]->getFloat_array()->getValue();	
						switch (code) 
						{
							case 1:
								for ( unsigned int a = 0; a < float_array_x1_1.getCount(); a++) {
										float_array_y1[a] = -float_array_x1_1[a];
								}
								break;
							case 2:
								for ( unsigned int a = 0; a < float_array_x1_1.getCount(); a++) {
										float_array_y1[a] = -float_array_x1_1[a];
								}
								break;
							case 3:
								for ( unsigned int a = 0; a < float_array_x1_1.getCount(); a++) {
										float_array_y1[a] = float_array_x1_1[a];
								}
								break;
							case 4:
								for ( unsigned int a = 0; a < float_array_z1_1.getCount(); a++) {
										float_array_y1[a] = float_array_z1_1[a];
								}
								break;
							case 5:
								for ( unsigned int a = 0; a < float_array_z1_1.getCount(); a++) {
										float_array_y1[a] = -float_array_z1_1[a];
								}
								break;
							case 6:
								for ( unsigned int a = 0; a < float_array_z1_1.getCount(); a++) {
										float_array_y1[a] = -float_array_z1_1[a];
								}
								break;
							default :
//								PRINTF("invalid code\n");
										break;
						}
					} else if (strcmp((*mi).first.c_str(), "IN_TANGENT_Y") == 0) {
						daeDoubleArray &float_array_y2= thisAnimation->getSource_array()[currentSource]->getFloat_array()->getValue();
						switch (code) 
						{
							case 1:
								for ( unsigned int a = 0; a < float_array_x2_1.getCount(); a++) {
										float_array_y2[a] = -float_array_x2_1[a];
								}
								break;
							case 2:
								for ( unsigned int a = 0; a < float_array_x2_1.getCount(); a++) {
										float_array_y2[a] = -float_array_x2_1[a];
								}
								break;
							case 3:
								for ( unsigned int a = 0; a < float_array_x2_1.getCount(); a++) {
										float_array_y2[a] = float_array_x2_1[a];
								}
								break;
							case 4:
								for ( unsigned int a = 0; a < float_array_z2_1.getCount(); a++) {
										float_array_y2[a] = float_array_z2_1[a];
								}
								break;
							case 5:
								for ( unsigned int a = 0; a < float_array_z2_1.getCount(); a++) {
										float_array_y2[a] = -float_array_z2_1[a];
								}
								break;
							case 6:
								for ( unsigned int a = 0; a < float_array_z2_1.getCount(); a++) {
										float_array_y2[a] = -float_array_z2_1[a];
								}
								break;
							default :
//								PRINTF("invalid code\n");
										break;
						}
					} else if (strcmp((*mi).first.c_str(), "OUT_TANGENT_Y") == 0) {
						daeDoubleArray &float_array_y3= thisAnimation->getSource_array()[currentSource]->getFloat_array()->getValue();
						switch (code) 
						{
							case 1:
								for ( unsigned int a = 0; a < float_array_x3_1.getCount(); a++) {
										float_array_y3[a] = -float_array_x3_1[a];
								}
								break;
							case 2:
								for ( unsigned int a = 0; a < float_array_x3_1.getCount(); a++) {
										float_array_y3[a] = -float_array_x3_1[a];
								}
								break;
							case 3:
								for ( unsigned int a = 0; a < float_array_x3_1.getCount(); a++) {
										float_array_y3[a] = float_array_x3_1[a];
								}
								break;
							case 4:
								for ( unsigned int a = 0; a < float_array_z3_1.getCount(); a++) {
										float_array_y3[a] = float_array_z3_1[a];
								}
								break;
							case 5:
								for ( unsigned int a = 0; a < float_array_z3_1.getCount(); a++) {
										float_array_y3[a] = -float_array_z3_1[a];
								}
								break;
							case 6:
								for ( unsigned int a = 0; a < float_array_z3_1.getCount(); a++) {
										float_array_y3[a] = -float_array_z3_1[a];
								}
								break;
							default :
//								PRINTF("invalid code\n");
										break;
						}
					} else if (strcmp((*mi).first.c_str(), "OUTPUT_Z") == 0) {
						daeDoubleArray &float_array_z1= thisAnimation->getSource_array()[currentSource]->getFloat_array()->getValue();	
						switch (code) 
						{
							case 1:
//								PRINTF("do nothing\n");
								break;
							case 2:
								for ( unsigned int a = 0; a < float_array_y1_1.getCount(); a++) {
										float_array_z1[a] = -float_array_y1_1[a];
								}
								break;
							case 3:
//								PRINTF("do nothing\n");
								break;
							case 4:
								for ( unsigned int a = 0; a < float_array_y1_1.getCount(); a++) {
										float_array_z1[a] = -float_array_y1_1[a];
								}
								break;
							case 5:
								for ( unsigned int a = 0; a < float_array_x1_1.getCount(); a++) {
										float_array_z1[a] = float_array_x1_1[a];
								}
								break;
							case 6:
								for ( unsigned int a = 0; a < float_array_y1_1.getCount(); a++) {
									float_array_z1[a] = float_array_y1_1[a];
								}
								break;
							default :
//								PRINTF("invalid code\n");
										break;
						}
					} else if (strcmp((*mi).first.c_str(), "IN_TANGENT_Z") == 0) {
						daeDoubleArray &float_array_z2= thisAnimation->getSource_array()[currentSource]->getFloat_array()->getValue();
						switch (code) 
						{
							case 1:
//								PRINTF("do nothing\n");
								break;
							case 2:
								for ( unsigned int a = 0; a < float_array_y2_1.getCount(); a++) {
										float_array_z2[a] = -float_array_y2_1[a];
								}
								break;
							case 3:
//								PRINTF("do nothing\n");
								break;
							case 4:
								for ( unsigned int a = 0; a < float_array_y2_1.getCount(); a++) {
										float_array_z2[a] = -float_array_y2_1[a];
								}
								break;
							case 5:
								for ( unsigned int a = 0; a < float_array_x2_1.getCount(); a++) {
										float_array_z2[a] = float_array_x2_1[a];
								}
								break;
							case 6:
								for ( unsigned int a = 0; a < float_array_y2_1.getCount(); a++) {
									float_array_z2[a] = float_array_y2_1[a];
								}
								break;
							default :
//								PRINTF("invalid code\n");
										break;
						}
					} else if (strcmp((*mi).first.c_str(), "OUT_TANGENT_Z") == 0) {
						daeDoubleArray &float_array_z3= thisAnimation->getSource_array()[currentSource]->getFloat_array()->getValue();	
						switch (code) 
						{
							case 1:
//								PRINTF("do nothing\n");
								break;
							case 2:
								for ( unsigned int a = 0; a < float_array_y3_1.getCount(); a++) {
										float_array_z3[a] = -float_array_y3_1[a];
								}
								break;
							case 3:
//								PRINTF("do nothing\n");
								break;
							case 4:
								for ( unsigned int a = 0; a < float_array_y3_1.getCount(); a++) {
									float_array_z3[a] = -float_array_y3_1[a];
								}
								break;
							case 5:
								for ( unsigned int a = 0; a < float_array_x3_1.getCount(); a++) {
									float_array_z3[a] = float_array_x3_1[a];
								}
								break;
							case 6:
								for ( unsigned int a = 0; a < float_array_y3_1.getCount(); a++) {
									float_array_z3[a] = float_array_y3_1[a];
								}
								break;
							default :
//								PRINTF("invalid code\n");
										break;
						}
					} // if
				} // if
			} // for map
		} // for source
	} // for animation

    // physics
	unsigned int shapeCount = input->getDatabase()->getElementCount(NULL, COLLADA_ELEMENT_SHAPE, docName);
    PRINTF("shape count is %d\n", shapeCount);
	for (unsigned int i = 0; i < shapeCount; i++) {
		domRigid_body::domTechnique_common::domShape *thisShape;
		error = input->getDatabase()->getElement((daeElement**)&thisShape,i, NULL, "shape", docName);
		if (error != DAE_OK) {
			//exit(-1);
		}
        process_shape(thisShape, code, verbose);
	} // shape
	unsigned int constraintCount = input->getDatabase()->getElementCount(NULL, COLLADA_ELEMENT_RIGID_CONSTRAINT, docName);
    PRINTF("rigid constraint count is %d\n", constraintCount);
	for (unsigned int i = 0; i < constraintCount; i++) {
		domRigid_constraint *thisConstraint;
		error = input->getDatabase()->getElement((daeElement**)&thisConstraint,i, NULL, "rigid_constraint", docName);
		if (error != DAE_OK) {
			//exit(-1);
		}
		// seem not needed, comment out for now
		process_rigid_constraint(thisConstraint, code, verbose);
	}
	unsigned int physicsSceneCount = input->getDatabase()->getElementCount(NULL, COLLADA_ELEMENT_PHYSICS_SCENE, docName);
    PRINTF("physics scene count is %d\n", physicsSceneCount);
	for (unsigned int i = 0; i < physicsSceneCount; i++) {
		domPhysics_scene *thisPhysicsScene;
		error = input->getDatabase()->getElement((daeElement**)&thisPhysicsScene,i, NULL, "physics_scene", docName);
		if (error != DAE_OK) {
			//exit(-1);
		}
		domPhysics_scene::domTechnique_commonRef tech = thisPhysicsScene->getTechnique_common();
		daeDoubleArray &gravity = tech->getGravity()->getValue();
        doublearray_swap(gravity, code, verbose);
	}


}

void y_to_x_convert(DAE *input, int verbose)
{
	common_convert(input, 1, verbose);
}

void z_to_x_convert(DAE *input, int verbose)
{
	common_convert(input, 2, verbose);
}

void x_to_y_convert(DAE *input, int verbose)
{
	common_convert(input, 3, verbose);
}

void z_to_y_convert(DAE *input, int verbose)
{
	common_convert(input, 4, verbose);
}

void x_to_z_convert(DAE *input, int verbose)
{
	common_convert(input, 5, verbose);
}

void y_to_z_convert(DAE *input, int verbose)
{
	common_convert(input, 6, verbose);
}

void x_up_converter(DAE *input, int verbose)
{
	int error = 0;
	domAsset * thisAsset;
	error = input->getDatabase()->getElement((daeElement**)&thisAsset,0, NULL, COLLADA_ELEMENT_ASSET, docName);
	if(error != DAE_OK)
	{
		//cerr<<"error "<<daeErrorString(error)<<" getting asset"<<i<<"\n";
		exit(-1);
	}
	domAsset::domUp_axisRef up_axis = thisAsset->getUp_axis();
	if (up_axis != NULL) {
		if (up_axis->getValue() == UPAXISTYPE_Y_UP)
			y_to_x_convert(input, verbose);
		else if (up_axis->getValue() == UPAXISTYPE_Z_UP)
			z_to_x_convert(input, verbose);
			up_axis->setValue(UPAXISTYPE_X_UP);
	} else {
		y_to_x_convert(input, verbose);
		domAsset::domUp_axis * up_axis = (domAsset::domUp_axis *)thisAsset->createAndPlace("up_axis");
		up_axis->setValue(UPAXISTYPE_X_UP);
	}
}

void y_up_converter(DAE *input, int verbose)
{
	int error = 0;
	domAsset * thisAsset;
	error = input->getDatabase()->getElement((daeElement**)&thisAsset,0, NULL, COLLADA_ELEMENT_ASSET, docName);
	if(error != DAE_OK)
	{
		//cerr<<"error "<<daeErrorString(error)<<" getting asset"<<i<<"\n";
		exit(-1);
	}
	domAsset::domUp_axisRef up_axis = thisAsset->getUp_axis();
	if (up_axis != NULL) {
		if (up_axis->getValue() == UPAXISTYPE_X_UP)
			x_to_y_convert(input, verbose);
		else if (up_axis->getValue() == UPAXISTYPE_Z_UP)
			z_to_y_convert(input, verbose);
		up_axis->setValue(UPAXISTYPE_Y_UP);
	}
}

void z_up_converter(DAE *input, int verbose)
{
	int error = 0;
	domAsset * thisAsset;
	error = input->getDatabase()->getElement((daeElement**)&thisAsset,0, NULL, COLLADA_ELEMENT_ASSET, docName);
	if(error != DAE_OK)
	{
		exit(-1);
	}
	domAsset::domUp_axisRef up_axis = thisAsset->getUp_axis();
	if (up_axis != NULL) {
		if (up_axis->getValue() == UPAXISTYPE_X_UP)
			x_to_z_convert(input, verbose);
		else if (up_axis->getValue() == UPAXISTYPE_Y_UP)
			y_to_z_convert(input, verbose);
			up_axis->setValue(UPAXISTYPE_Z_UP);
	} else {
		y_to_z_convert(input, verbose);
		domAsset::domUp_axis * up_axis = (domAsset::domUp_axis *)thisAsset->createAndPlace("up_axis");
		up_axis->setValue(UPAXISTYPE_Z_UP);
	}
}

bool Axisconverter::init()
{
	addBoolOption( "verbose", "verbose", "verbose", true);
	addStringOption( "axis", "axis", "axis", "Y_UP");
	return true;
}

int Axisconverter::execute()
{
	string axis;
	bool verbose = false;
	getBoolOption("v", verbose);
	getStringOption("axis", axis);

	docName = getInput(0).c_str();

	if ( stricmp(axis.c_str(), "X_UP") == 0 || stricmp(axis.c_str(), "X") == 0 ) 
		x_up_converter(_dae, verbose);
	else if (stricmp(axis.c_str(), "Y_UP") == 0 || stricmp(axis.c_str(), "Y") == 0) 
		y_up_converter(_dae, verbose);
	else if (stricmp(axis.c_str(), "Z_UP") == 0 || stricmp(axis.c_str(), "Z") == 0)
		z_up_converter(_dae, verbose);

	return 0;
}

Conditioner::Register<AxisconverterProxy> registerObject;
