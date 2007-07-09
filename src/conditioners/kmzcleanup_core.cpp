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
#include "kmzcleanup.h"
#include "dom/domConstants.h"

//#define PRINTF if (verbose) printf
template<class T>
int Kmzcleanup::processprimitive(T primitive, int verbose)
{
	int input_count = (int)primitive->getInput_array().getCount();
	printf("input count in primitive is %d\n", input_count);
	int input_set;
	for (int i = 0; i < input_count; i++) {
		xsNMTOKEN semantic = primitive->getInput_array()[i]->getSemantic();
		if (strcmp(semantic, "TEXCOORD") == 0) {
			input_set = (int)primitive->getInput_array()[i]->getSet();
			return input_set;
		} //if 
	} // for
	return 0;
}

bool Kmzcleanup::checkbindneeded(DAE* input, daeString material, int verbose)
{
	unsigned int index = 0;
	domCOLLADA *root = (domCOLLADA*)input->getDatabase()->getDocument(index)->getDomRoot();
	daeElementRefArray elementArray;
	for ( unsigned int j = 0; j < root->getLibrary_materials_array().getCount(); j++ )
	{
		domLibrary_materials *lm = root->getLibrary_materials_array()[j];
		for ( unsigned int k = 0; k < lm->getMaterial_array().getCount(); k++ )
		{
			elementArray.append( lm->getMaterial_array()[k] );
		}
	}

	for ( unsigned int j = 0; j < elementArray.getCount(); j++ )
	{
		daeString id = elementArray[j]->getID();
		if ( (id != NULL) && (strcmp(material, id) == 0)) {
			domMaterial *thisMaterial = (domMaterial *)(daeElement *)elementArray[j];
			thisMaterial->getInstance_effect()->getUrl().resolveElement();
			daeElement *thisElement = thisMaterial->getInstance_effect()->getUrl().getElement();
			domEffect *thisEffect = (domEffect *)thisElement;
			//int count = (int)thisEffect->getProfile_common()->getNewparam_array().getCount();
			//int count = (int)thisEffect->getNewparam_array().getCount();
			int count = (int)thisEffect->getContents().getCount();
			for (int i = 0; i < count; i++) {
				domProfile_COMMON *profile = daeSafeCast< domProfile_COMMON >( thisEffect->getContents()[i] );
				if (profile != NULL) {
					if (profile->getTechnique()) {
						domProfile_COMMON::domTechnique::domPhong * phong = profile->getTechnique()->getPhong();
						if (phong == NULL) continue;
						domCommon_color_or_texture_type_complexType::domTexture *thisTexture = phong->getDiffuse()->getTexture();
						if (thisTexture != NULL)
							return true;
						else 
							continue;
					}
				}
			}
		}
	}
	return false;
}

bool Kmzcleanup::init()
{
	addBoolOption( "verbose", "verbose", "verbose print out", true );
	return true;
}

int Kmzcleanup::execute()
{
	int error = 0;
	bool effect_change = false;

	map <string, int> strmap;
    map <string, int>::iterator si;

	bool verbose;
	getBoolOption( "verbose", verbose );

	unsigned int assetCount = _dae->getDatabase()->getElementCount(NULL, COLLADA_TYPE_ASSET, getInput(0).c_str());
    //cerr<<"There are "<<assetCount<<" assets in this file\n"; 
	domAsset * thisAsset;
	for(unsigned int i=0; i<assetCount; i++)
	{
		// Get the next asset
		error = _dae->getDatabase()->getElement((daeElement**)&thisAsset,i, NULL, COLLADA_TYPE_ASSET, getInput(0).c_str());
		if(error != DAE_OK)
			continue;

		domAsset::domCreated *pCreated = thisAsset->getCreated();
		if (pCreated == 0) {
			// need to add created
			domAsset::domCreated *newCreated;
			newCreated = (domAsset::domCreated *)thisAsset->createAndPlace(COLLADA_TYPE_CREATED);
			newCreated->setValue("2005-11-14T02:16:38Z");
		}
		domAsset::domModified *pModified = thisAsset->getModified();
		if (pModified == 0) {
			// need to add modified
			domAsset::domModified *newModified;
			newModified = (domAsset::domModified *)thisAsset->createAndPlace(COLLADA_TYPE_MODIFIED);
			newModified->setValue("2005-11-15T11:36:38Z");
		}
		domAsset::domContributor_Array & contributors = thisAsset->getContributor_array();
		for (unsigned int i=0; i< contributors.getCount(); i++)
		{
			domAsset::domContributorRef contrib = contributors[i];
			domAsset::domContributor::domAuthoring_toolRef ref = contrib->getAuthoring_tool();
			if (ref)
			{
				printf("authoring tool is %s\n", ref->getValue());
				if ( strstr(ref->getValue(), "KMZ fix") != NULL) {
					// check for "TEXTCOORD" and fix it
					unsigned int geometryInstanceCount = _dae->getDatabase()->getElementCount(NULL, "instance_geometry", getInput(0).c_str());
					//printf("there are %d instance_geometry\n", geometryInstanceCount);

					for (unsigned int currentGeometryInstance = 0; currentGeometryInstance < geometryInstanceCount; currentGeometryInstance++) {
						domInstance_geometry *thisGeometryInstance;
						error = _dae->getDatabase()->getElement((daeElement**)&thisGeometryInstance,currentGeometryInstance, NULL, "instance_geometry", getInput(0).c_str());
						if (error != DAE_OK) {
							return(-1);
						}
						domBind_materialRef bindMaterial = thisGeometryInstance->getBind_material();
						if (bindMaterial != NULL) {
						    domInstance_material_Array &instanceMaterialArray = bindMaterial->getTechnique_common()->getInstance_material_array();
							int materialArrayCount = (int)instanceMaterialArray.getCount();
							for (int i = 0; i < materialArrayCount; i++) {
								domInstance_material::domBind_vertex_input_Array &bindInputArray = instanceMaterialArray[i]->getBind_vertex_input_array();
								int bindInputArrayCount = (int)bindInputArray.getCount();
								for (int j = 0; j < bindInputArrayCount; j++) {
									xsNCName inputSemantic = bindInputArray[j]->getInput_semantic();
									if (strcmp(inputSemantic, "TEXTCOORD") == 0)
										bindInputArray[j]->setInput_semantic("TEXCOORD");
								} // for
							} // for
						} // if
					} // for
				} // if KMZ fix
				if ( 1 /*(strstr(ref->getValue(), "SketchUp") != NULL) || (strstr(ref->getValue(), "Google") != NULL) || (strstr(ref->getValue(), "PhotoModeler") != NULL)*/ )
				{
					printf("start kmz cleaning...\n");
					// change /library_effects/effect/profile_COMMON/technique/phong/diffuse/texture@texcoord="1" to texture@texcoord="UVSET0"
					unsigned int phongCount = _dae->getDatabase()->getElementCount(NULL, COLLADA_TYPE_PHONG, getInput(0).c_str());
					//cerr<<"There are "<<phongCount<<" phongs in this file\n"; 
					domProfile_COMMON::domTechnique::domPhong * thisPhong;
					domCommon_color_or_texture_type_complexType::domTexture * thisTexture;

					for (unsigned int i = 0; i < phongCount; i++) 
					{
						//Get the next phong
						error = _dae->getDatabase()->getElement((daeElement**)&thisPhong, i, NULL, COLLADA_TYPE_PHONG, getInput(0).c_str());
						if (error != DAE_OK)
							continue;

						domCommon_color_or_texture_type * thisDiffuse;
						thisDiffuse = thisPhong->getDiffuse();
						if (thisDiffuse != NULL) {
							thisTexture = thisDiffuse->getTexture();
							if (thisTexture != NULL) {
								xsNCName  attribute = thisTexture->getTexcoord();
								//cerr<<"texture coordinate : "<<attribute<<"\n";
								if (strcmp(attribute, "1") == 0) {
									//cerr<<"set texture coordinate\n";
									thisTexture->setTexcoord("UVSET0");
								}
								xsNCName texture = thisTexture->getTexture();
								char *texture_1;
								if (texture) {
									if (texture[0] >= '0' && texture[0] <= '9') {
										texture_1 = new char[strlen(texture) + 1 + 1];
										sprintf(texture_1, "a%s", texture);
									} else {
										texture_1 = new char[strlen(texture) + 1];
										strcpy(texture_1, texture);
									}
									//printf("texture image is %s\n", texture);
									daeElement *parent = thisPhong->getParentElement();
									domProfile_COMMON::domTechnique *thisTechnique = (domProfile_COMMON::domTechnique *)parent;
									//domCommon_newparam_type * thisNewParam = (domCommon_newparam_type *)thisTechnique->createAndPlace("newparam");
									daeElement *parent2 = thisTechnique->getParentElement();
									domProfile_COMMON *thisProfile = (domProfile_COMMON *)parent2;
									int newparam_count = (int)thisProfile->getNewparam_array().getCount();
									if (newparam_count == 0 ) {
										domCommon_newparam_type * thisNewParam = (domCommon_newparam_type *)thisProfile->createAndPlace("newparam");

										char *surface_id = new char[strlen(texture_1) + 1 + 8];
										sprintf(surface_id, "%s-surface", texture_1);
										//printf("surface id is %s\n", surface_id);
										thisNewParam->setSid(surface_id);
										domFx_surface_common * thisSurface = (domFx_surface_common *)thisNewParam->createAndPlace("surface");
										thisSurface->setType(FX_SURFACE_TYPE_ENUM_2D);
										domFx_surface_init_from_common * thisInitFrom = (domFx_surface_init_from_common *)thisSurface->createAndPlace("init_from");
										thisInitFrom->setValue(texture_1);

										domCommon_newparam_type * thisNewParam2 = (domCommon_newparam_type *)thisProfile->createAndPlace("newparam");
										char *sampler_id = new char[strlen(texture_1) + 1 + 8];
										sprintf(sampler_id, "%s-sampler", texture_1);
										//printf("sampler id is %s\n", sampler_id);
										thisNewParam2->setSid(sampler_id);
										domFx_sampler2D_common * thisSampler2D = (domFx_sampler2D_common *)thisNewParam2->createAndPlace("sampler2D");
										domFx_sampler2D_common::domSource * thisSource = (domFx_sampler2D_common::domSource *)thisSampler2D->createAndPlace("source");
										//printf("before source set id\n");
										thisSource->setValue(surface_id);
										//printf("after source set id\n");
										thisTexture->setTexture(sampler_id);

										delete [] surface_id;
										delete [] sampler_id;
									}
								}
							} //if
						}//if
					} //for


					// remove library_geometries/geometry/mesh/triangles@material="BackColor"
					// remove library_geometries/geometry/mesh/lines@material="EdgeColor"
					unsigned int geometryElementCount = _dae->getDatabase()->getElementCount(NULL, "geometry", getInput(0).c_str());

					// Iterate over all the geometry assets
					for(unsigned int currentGeometry = 0; currentGeometry < geometryElementCount; currentGeometry++) {
						// Get the next geometry element
						domGeometry *thisGeometry;
						error = _dae->getDatabase()->getElement((daeElement**)&thisGeometry,currentGeometry, NULL, "geometry", getInput(0).c_str());
						if (error != DAE_OK) {
							return(-1);
						}

						// Get the geometry element's mesh
						domMesh *thisMesh = thisGeometry->getMesh();

						if (thisMesh==NULL) continue; // if there are no mesh in this geometry, skip to next one

						// Loop over all the triangle assets in this mesh
						int trianglesAssetCount = (int) thisMesh->getTriangles_array().getCount();
						//printf("there are %d triangles\n", trianglesAssetCount);
						for(int currentTriangles = 0; currentTriangles < trianglesAssetCount; currentTriangles++) {
							domTriangles *thisTriangles = thisMesh->getTriangles_array().get(currentTriangles);
							if (thisTriangles != NULL) {
								xsNCName triMaterial = thisTriangles->getMaterial();
								if (triMaterial) {
									if (strcmp(triMaterial, "BackColor") == 0) {
										//printf("remove the triangle with backcolor\n");
										thisMesh->removeChildElement(thisTriangles);
										currentTriangles--;
										trianglesAssetCount--;
									} // if
								}
							} // if
						} // for

						/*int linesCount = (int) thisMesh->getLines_array().getCount();
						printf("there are %d lines\n", linesCount);
						for (int currentLines = 0; currentLines < linesCount; currentLines++) {
							domLines *thisLines = thisMesh->getLines_array().get(currentLines);
							if (thisLines != NULL) {
								xsNCName lineMaterial= thisLines->getMaterial();
								if (strcmp(lineMaterial, "EdgeColor") == 0) {
									printf("remove the line with edgecolor\n");
									thisMesh->removeChildElement(thisLines);
									currentLines--;
									linesCount--;
								} // if
							} // if
						} // for*/
					} // for

					// change the id/name of images/materials/effects if they start with a numerical number
					int imageElementCount = (int)(_dae->getDatabase()->getElementCount(NULL, "image", getInput(0).c_str()));

					for(int currentImg = 0; currentImg < imageElementCount; currentImg++)
					{
						domImage *thisImage;
						error = _dae->getDatabase()->getElement((daeElement**)&thisImage,currentImg, NULL, "image", getInput(0).c_str());
						xsID image_id = thisImage->getId();
						if (image_id) {
							if (image_id[0] >= '0' && image_id[0] <= '9') {
								char *image_id_1 = new char[strlen(image_id) + 1 + 1];
								sprintf(image_id_1, "a%s", image_id);
								thisImage->setId(image_id_1);
								delete [] image_id_1;
							}
						}
						xsNCName image_name = thisImage->getName();
						if (image_name) {
							if (image_name[0] >= '0' && image_name[0] <= '9') {
								char *image_name_1 = new char[strlen(image_name) + 1 + 1];
								sprintf(image_name_1, "a%s", image_name);
								thisImage->setName(image_name_1);
								delete [] image_name_1;
							}
						}
					}

				    int materialElementCount = (int)(_dae->getDatabase()->getElementCount(NULL, "material", getInput(0).c_str()));

					for(int currentMat = 0; currentMat < materialElementCount; currentMat++)
					{
						domMaterial *thisMaterial;
						error = _dae->getDatabase()->getElement((daeElement**)&thisMaterial,currentMat, NULL, "material", getInput(0).c_str());
						xsID material_id = thisMaterial->getId();
						if (material_id) {
							if (material_id[0] >= '0' && material_id[0] <= '9') {
								char *material_id_1 = new char[strlen(material_id) + 1 + 1];
								sprintf(material_id_1, "a%s", material_id);
								thisMaterial->setId(material_id_1);
								delete [] material_id_1;
							}
						}
						xsNCName material_name = thisMaterial->getName();
						if (material_name) {
							if (material_name[0] >= '0' && material_name[0] <= '9') {
								char *material_name_1 = new char[strlen(material_name) + 1 + 1];
								sprintf(material_name_1, "a%s", material_name);
								thisMaterial->setName(material_name_1);
								delete [] material_name_1;
							}
						}
						domInstance_effectRef instance_ref = thisMaterial->getInstance_effect();
						daeString uri = instance_ref->getUrl().getURI();
						printf("uri is %s\n", uri);
						daeString id = instance_ref->getUrl().getID();
						printf("uri id is %s\n", id);
						if (id[0] >= '0' && id[0] <= '9') {
							char *uri_1 = new char[strlen(id) + 1 + 1];
							sprintf(uri_1, "#a%s", id);
							daeURI thisUri = daeURI((daeString)uri_1, false);
							instance_ref->setUrl(thisUri);
						}
					}

					int effectElementCount = (int)(_dae->getDatabase()->getElementCount(NULL, "effect", getInput(0).c_str()));

					for(int currentEffect = 0; currentEffect < effectElementCount; currentEffect++)
					{
						domEffect *thisEffect;
						error = _dae->getDatabase()->getElement((daeElement**)&thisEffect,currentEffect, NULL, "effect", getInput(0).c_str());
						xsID effect_id = thisEffect->getId();
						if (effect_id) {
							if (effect_id[0] >= '0' && effect_id[0] <= '9') {
								effect_change = true;
								char *effect_id_1 = new char[strlen(effect_id) + 1 + 1];
								sprintf(effect_id_1, "a%s", effect_id);
								thisEffect->setId(effect_id_1);
								delete [] effect_id_1;
							}
						}
						xsNCName effect_name = thisEffect->getName();
						if (effect_name) {
							if (effect_name[0] >= '0' && effect_name[0] <= '9') {
								char *effect_name_1 = new char[strlen(effect_name) + 1 + 1];
								sprintf(effect_name_1, "a%s", effect_name);
								thisEffect->setName(effect_name_1);
								delete [] effect_name_1;
							}
						}

					}

					// add bind_material
					unsigned int geometryInstanceCount = _dae->getDatabase()->getElementCount(NULL, "instance_geometry", getInput(0).c_str());
					//printf("there are %d instance_geometry\n", geometryInstanceCount);

					for (unsigned int currentGeometryInstance = 0; currentGeometryInstance < geometryInstanceCount; currentGeometryInstance++) {
						domInstance_geometry *thisGeometryInstance;
						error = _dae->getDatabase()->getElement((daeElement**)&thisGeometryInstance,currentGeometryInstance, NULL, "instance_geometry", getInput(0).c_str());
						if (error != DAE_OK) {
							return(-1);
						}
						daeElementRef thisElement = thisGeometryInstance->getUrl().getElement();
						domGeometry *thisGeometry = (domGeometry *)(daeElement*)thisElement;
						//printf("get the geometry\n");

						// Get the geometry element's mesh
						domMesh *thisMesh = thisGeometry->getMesh();
						//printf("get the mesh\n");
						if (thisMesh==NULL) continue; // if there are no mesh in this geometry, skip to next one

						// Loop over all the polygon assets in this mesh
						int polygonsAssetCount = (int) thisMesh->getPolygons_array().getCount();
						//printf("there are %d polygons\n", polygonsAssetCount);
						for(int currentPolygons = 0; currentPolygons < polygonsAssetCount; currentPolygons++) {
							domPolygons *thisPolygons = thisMesh->getPolygons_array().get(currentPolygons);
							if (thisPolygons) {
								xsNCName polygonMaterial = thisPolygons->getMaterial();
								if (polygonMaterial) {
									if (polygonMaterial[0] >= '0' && polygonMaterial[0] <= '9') {
										char *polygonMaterial_1 = new char[strlen(polygonMaterial) + 1 + 1];
										sprintf(polygonMaterial_1, "a%s", polygonMaterial);
										thisPolygons->setMaterial(polygonMaterial_1);
                                        int set = processprimitive(thisPolygons, verbose);
										strmap.insert(pair<string, int>(polygonMaterial_1, set));
										delete [] polygonMaterial_1;
									} else {
                                        int set = processprimitive(thisPolygons, verbose);
										strmap.insert(pair<string, int>(polygonMaterial, set));	
									}
								}
							}
						}

						// Loop over all the polylist assets in this mesh
						int polylistsAssetCount = (int) thisMesh->getPolylist_array().getCount();
						//printf("there are %d polylists\n", polylistsAssetCount);
						for(int currentPolylists = 0; currentPolylists < polylistsAssetCount; currentPolylists++) {
							domPolylist *thisPolylists = thisMesh->getPolylist_array().get(currentPolylists);
							if (thisPolylists) {
								xsNCName polylistMaterial = thisPolylists->getMaterial();
								if (polylistMaterial) {
									if (polylistMaterial[0] >= '0' && polylistMaterial[0] <= '9') {
										char *polylistMaterial_1 = new char[strlen(polylistMaterial) + 1 + 1];
										sprintf(polylistMaterial_1, "a%s", polylistMaterial);
										thisPolylists->setMaterial(polylistMaterial_1);
                                        int set = processprimitive(thisPolylists, verbose);
										strmap.insert(pair<string, int>(polylistMaterial_1, set));
										delete [] polylistMaterial_1;
									} else {
                                        int set = processprimitive(thisPolylists, verbose);
										strmap.insert(pair<string, int>(polylistMaterial, set));
									}
								}
							}
						}

						// Loop over all the triangle assets in this mesh
						int trianglesAssetCount = (int) thisMesh->getTriangles_array().getCount();
						//printf("there are %d triangles\n", trianglesAssetCount);
						for(int currentTriangles = 0; currentTriangles < trianglesAssetCount; currentTriangles++) {
							domTriangles *thisTriangles = thisMesh->getTriangles_array().get(currentTriangles);
							if (thisTriangles) {
								xsNCName triangleMaterial = thisTriangles->getMaterial();
								if (triangleMaterial) {
									if (triangleMaterial[0] >= '0' && triangleMaterial[0] <= '9') {
										char *triangleMaterial_1 = new char[strlen(triangleMaterial) + 1 + 1];
										sprintf(triangleMaterial_1, "a%s", triangleMaterial);
										thisTriangles->setMaterial(triangleMaterial_1);
                                        int set = processprimitive(thisTriangles, verbose);
										strmap.insert(pair<string, int>(triangleMaterial_1, set));
										delete [] triangleMaterial_1;
									} else {
                                        int set = processprimitive(thisTriangles, verbose);
										strmap.insert(pair<string, int>(triangleMaterial, set));										
									}
								}	
							}
						}

						// Loop over all the tristrips assets in this mesh
						int tristripsAssetCount = (int) thisMesh->getTristrips_array().getCount();
						//printf("there are %d tristrips\n", tristripsAssetCount);
						for(int currentTristrips = 0; currentTristrips < tristripsAssetCount; currentTristrips++) {
							domTristrips *thisTristrips = thisMesh->getTristrips_array().get(currentTristrips);
							if (thisTristrips) {
								xsNCName tristripMaterial = thisTristrips->getMaterial();
								if (tristripMaterial) {
									if (tristripMaterial[0] >= '0' && tristripMaterial[0] <= '9') {
										char *tristripMaterial_1 = new char[strlen(tristripMaterial) + 1 + 1];
										sprintf(tristripMaterial_1, "a%s", tristripMaterial);
										thisTristrips->setMaterial(tristripMaterial_1);
                                        int set = processprimitive(thisTristrips, verbose);
										strmap.insert(pair<string, int>(tristripMaterial_1, set));
										delete [] tristripMaterial_1;
									} else {
                                        int set = processprimitive(thisTristrips, verbose);
										strmap.insert(pair<string, int>(tristripMaterial, set));
									}
								}
							}
						}

						// Loop over all the trifans assets in this mesh
						int trifansAssetCount = (int) thisMesh->getTrifans_array().getCount();
						//printf("there are %d trifans\n", trifansAssetCount);
						for(int currentTrifans = 0; currentTrifans < trifansAssetCount; currentTrifans++) {
							domTrifans *thisTrifans = thisMesh->getTrifans_array().get(currentTrifans);
							if (thisTrifans) {
								xsNCName trifanMaterial = thisTrifans->getMaterial();
								if (trifanMaterial) {
									if (trifanMaterial[0] >= '0' && trifanMaterial[0] <= '9') {
										char *trifanMaterial_1 = new char[strlen(trifanMaterial) + 1 + 1];
										sprintf(trifanMaterial_1, "a%s", trifanMaterial);
										thisTrifans->setMaterial(trifanMaterial_1);
                                        int set = processprimitive(thisTrifans, verbose);
										strmap.insert(pair<string, int>(trifanMaterial_1, set));										
										delete [] trifanMaterial_1;
									} else {
                                        int set = processprimitive(thisTrifans, verbose);
										strmap.insert(pair<string, int>(trifanMaterial, set));
									}								
								}
									
							}
						}

						// Loop over all the lines assets in this mesh
						int linesAssetCount = (int) thisMesh->getLines_array().getCount();
						//printf("there are %d lines\n", linesAssetCount);
						for(int currentLines = 0; currentLines < linesAssetCount; currentLines++) {
							domLines *thisLines = thisMesh->getLines_array().get(currentLines);
							if (thisLines) {
								xsNCName lineMaterial = thisLines->getMaterial();
								if (lineMaterial) {
									if (lineMaterial[0] >= '0' && lineMaterial[0] <= '9') {
										char *lineMaterial_1 = new char[strlen(lineMaterial) + 1 + 1];
										sprintf(lineMaterial_1, "a%s", lineMaterial);
										thisLines->setMaterial(lineMaterial_1);
                                        int set = processprimitive(thisLines, verbose);
										strmap.insert(pair<string, int>(lineMaterial_1, set));
										delete [] lineMaterial_1;
									} else {
                                        int set = processprimitive(thisLines, verbose);
										strmap.insert(pair<string, int>(lineMaterial, set));
									}
								}	
							}
						}

						// Loop over all the linestrips assets in this mesh
						int linestripsAssetCount = (int) thisMesh->getLinestrips_array().getCount();
						//printf("there are %d linestrips\n", linestripsAssetCount);
						for(int currentLinestrips = 0; currentLinestrips < linestripsAssetCount; currentLinestrips++) {
							domLinestrips *thisLinestrips = thisMesh->getLinestrips_array().get(currentLinestrips);
							if (thisLinestrips) {
								xsNCName linestripMaterial = thisLinestrips->getMaterial();
								if (linestripMaterial) {
									if (linestripMaterial[0] >= '0' && linestripMaterial[0] <= '9') {
										char *linestripMaterial_1 = new char[strlen(linestripMaterial) + 1 + 1];
										sprintf(linestripMaterial_1, "a%s", linestripMaterial);
										thisLinestrips->setMaterial(linestripMaterial_1);
                                        int set = processprimitive(thisLinestrips, verbose);
										strmap.insert(pair<string, int>(linestripMaterial_1, set));										
										delete [] linestripMaterial_1;
									} else {
                                        int set = processprimitive(thisLinestrips, verbose);
										strmap.insert(pair<string, int>(linestripMaterial, set));										
									}
								}	
							}
						}

						/*for (si=strmap.begin(); si!=strmap.end(); si++)  {
							cout << (*si).first << " ";
							cout << endl;
						}*/

						domBind_materialRef bindMaterial = thisGeometryInstance->getBind_material();
						if (bindMaterial == NULL) {
							if (strmap.size() != 0) {
								domBind_material *thisBindMaterial = (domBind_material *)thisGeometryInstance->createAndPlace("bind_material");
								domSource::domTechnique_common *thisTechniqueCommon = (domSource::domTechnique_common *)thisBindMaterial->createAndPlace("technique_common");
								for (si=strmap.begin(); si!=strmap.end();si++) {
									domInstance_material * thisInstanceMaterial = (domInstance_material *)thisTechniqueCommon->createAndPlace("instance_material");
									string targetValue("#");
									targetValue += (*si).first.c_str();
									//printf("symbol value is %s\n", (*si).c_str());
									//printf("target value is %s\n", targetValue.c_str());
									thisInstanceMaterial->setSymbol((*si).first.c_str());
									daeURI thisUri = daeURI((daeString)targetValue.c_str(), false);
									thisInstanceMaterial->setTarget(thisUri);
									//check if it needs <bind_vertex_input> 
									//printf("material name is %s\n", (*si).first.c_str());
									if (effect_change == true) {
										//printf("bind vertex is needed\n");
										domInstance_material::domBind_vertex_input * thisBindVertexInput = (domInstance_material::domBind_vertex_input *)thisInstanceMaterial->createAndPlace("bind_vertex_input");
										thisBindVertexInput->setSemantic("UVSET0");
										thisBindVertexInput->setInput_semantic("TEXCOORD");
										thisBindVertexInput->setInput_set((*si).second);
									} else if (checkbindneeded(_dae, (*si).first.c_str(), verbose) == true) {
										//printf("bind vertex is needed\n");
										domInstance_material::domBind_vertex_input * thisBindVertexInput = (domInstance_material::domBind_vertex_input *)thisInstanceMaterial->createAndPlace("bind_vertex_input");
										thisBindVertexInput->setSemantic("UVSET0");
										thisBindVertexInput->setInput_semantic("TEXCOORD");
										thisBindVertexInput->setInput_set((*si).second);
									} else { 
										printf("bind vertex is not needed\n");
									}
								}
								strmap.clear();
							}
						} else {
						    domInstance_material_Array &instanceMaterialArray = bindMaterial->getTechnique_common()->getInstance_material_array();
							int materialArrayCount = (int)instanceMaterialArray.getCount();
							printf("material array count is %d\n", materialArrayCount);
							for (int i = 0; i < materialArrayCount; i++) {
								domInstance_material::domBind_Array &bindArray = instanceMaterialArray[i]->getBind_array();
								int bindArrayCount = (int)bindArray.getCount();
								printf("bind array count is %d\n", bindArrayCount);
								if (bindArrayCount != 0) {
                                    xsNCName symbol = instanceMaterialArray[i]->getSymbol();
									for (int j = 0; j < bindArrayCount; j++) {
										instanceMaterialArray[i]->removeChildElement(bindArray[j]);
									}
									domInstance_material::domBind_vertex_input * thisBindVertexInput = (domInstance_material::domBind_vertex_input *)instanceMaterialArray[i]->createAndPlace("bind_vertex_input");
									thisBindVertexInput->setSemantic("UVSET0");
									thisBindVertexInput->setInput_semantic("TEXCOORD");
									for (si = strmap.begin(); si != strmap.end(); si++) {
										if (strcmp(symbol, (*si).first.c_str()) == 0) {
											thisBindVertexInput->setInput_set((*si).second);
										}
									}
								}
							}
						} // else
					}
                    //ref->setValue("KMZ fix");
					unsigned int index = 0;
				    daeDocument * thisDocument = (daeDocument *)_dae->getDatabase()->getDocument(index);
					domCOLLADA *thisCollada = (domCOLLADA *)thisDocument->getDomRoot();
					thisCollada->setAttribute("version", "1.4.1");
					break;
				}
			}
		}
	} // for

	return 0;
}

Conditioner::Register< KmzcleanupProxy > kmzcleanupProxy;
