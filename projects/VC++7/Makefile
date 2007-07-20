# Copyright 2006 Sony Computer Entertainment Inc.
#
# Licensed under the SCEA Shared Source License, Version 1.0 (the "License"); you may not use this 
# file except in compliance with the License. You may obtain a copy of the License at:
# http://research.scea.com/scea_shared_source_license.html
#
# Unless required by applicable law or agreed to in writing, software distributed under the License 
# is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
# implied. See the License for the specific language governing permissions and limitations under the 
# License. 

ROOT_DIR					:=	../..
include						$(ROOT_DIR)/build/MakeDefs
							
# visual studio C++ 2003 project files
PROJECT_FILES				+= \
							libloader.vcproj \
							bindmaterialfixer.vcproj \
							conditioners.vcproj \
							kmzcleanup.vcproj  \
							packager.vcproj \
							JavaSrc.vcproj \
							
CLEAN_DIR					+= 	../../lib \
								../../lib-dbg \
								../../tmp \
								../../bin/vc7/debug/libloader.* \
								../../bin/vc7/debug/refinery.jar \
								../../bin/vc7/debug/conditioners \
								../../bin/vc7/release/libloader.* \
								../../bin/vc7/release/refinery.jar \
								../../bin/vc7/release/conditioners \
								
PROJECT_CONFIG				:= $(BUILD)

include						$(ROOT_DIR)/build/MakeRules
