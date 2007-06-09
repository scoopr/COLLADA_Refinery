 Copyright 2006 Sony Computer Entertainment Inc.

 Licensed under the SCEA Shared Source License, Version 1.0 (the "License"); you may not use this 
 file except in compliance with the License. You may obtain a copy of the License at:
 http://research.scea.com/scea_shared_source_license.html

 Unless required by applicable law or agreed to in writing, software distributed under the License 
 is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
 implied. See the License for the specific language governing permissions and limitations under the 
 License.

Refinery and Conditioners:
Original Author: Philippe David
Authors: Andrew Lorino, Herbert Law, Ling Liu, Steven Thomas

Prerequisites for *running* Refinery:

- Java Runtime Engine (JRE)
  Make sure the JRE bin folder is included in your path (the installer handles this automatically).

Prerequisites for *building* Refinery:

- Microsoft Visual Studio 2003 or 2005

- Java Development Kit (JDK)
  Make sure that the environment variable JDK_HOME is set to the JDK bin folder (the
  installer handles this automatically).

How to build:

- Visual Studio 2003: Open projects/VC++7/Refinery.sln and build the solution.
- Visual Studio 2005: Open projects/VC++8/Refinery.sln and build the solution.

How to run:

- Navigate to the bin directory appropriate for your platform and execute refinery.bat.

or

- Open a command prompt, navigate to the bin directory appropriate for your platform,
  and execute "java -jar ..\..\refinery.jar"

Command line options:

$ java -jar refinery.jar --help
Refinery command line usage:
        refinery [options] (--execute-lib|-x) <libname> [liboptions]
                Execute the contionner called <libname> in the lib directory
                and pass all following options to this conditionner

        refinery [options] (--execute-macro|-m) <macroname> -i <input filename> -o <output filename>
                Execute the macro called <macroname> in the macro directory

        refinery [options] (--execute-file|-f) <filename>
                Execute the pipeline saved into <filename>

        refinery --list
                List all conditioners and macros found.

        refinery --help
                Print this message.

        refinery --help-lib <libname>
                Print description and options for a particular lib.                      

example 
$ refinery -i input.dae -o output.dae -x Axisconverter -axis X_UP
or
$ java -jar refinery.jar -i input.dae -o output.dae -x Axisconverter -axis X_UP

How to test

make runtest   

- run each dae files in dae_data directory with each conditioners executable.

make pip 
- run the test cases from saved pipeline files automatically (*)
- run the test cases from saved pipeline macros automatically (*)
*make sure the input/output files specified in .pip match what you have in your environment
                           
Available conditioner list 

Axisconverter       - convert the different up_axis (X_UP, Y_UP, Z_UP)  
  - option          - axis - which axis will become up axis

bindmaterialfixer   - add missing bind_material in instance_geometry if needed

coherencytest       - test the data integrety of COLLADA document
  - option          - log - the log file to save the test result to
                          - if it is not specify, the defualt is the same path and file name with .log extension						

copyrighter         - check whether the copyright is present if not add one
  - option          - Copyright - the new string for the copyright
                    - Overwrite - overwrite the existed coyright string

daestripper         - remove a specific element
  - option          - element - the element you want to remove
                    - attribute - the profile valueof the element you want to remove
  - example	    - element=extra - remove all extra element
                    - element=technique and attribute=any - remove all techique of any profile
                    - element=technique and attribute=MAYA - remove all techique of profile="MAYA"
					
deindexer           - take COLLADA data and put it in a form that is ready for use with GL vertex arrays
                      it will deindex all primitives inside a geometries,
                      rearrange vertice array, and all skins that use these vertice arrays.

detrifanner         - take trifans and convert back to triangles

detristripper       - take tristrips and convert back to triangles

filename            - convert all the image filenames to be relative to the position of the current COLLADA file
  - option          - Prefix - the string you want to put before files of <file> and <image> element
                    - image  - modify the image element also.
								   
imageconverter      - convert all images that are referenced from a specific COLLADA document to a desire image format
  - option	    - extension - the file extension format you want the image to convert to.

Lines2linestrips    - turn all lines to linestrips

Linestrips2lines    - turn all linestrips to lines

Optimizer           - optimize the primitives to improve the vertex cache hit rate
  - option          - cache_size - cache size so that we can get max hit rate, default is 10

Packager            - take all referenced dae, image and external files and put them all into a zip file.
  - option          - archivename - filename of the zip file we want to zip to.
                                  - if it is not specify, the defualt is the same path and file name with .zip extension						

polygons2lines      - turn all polygons to lines

polygons2triangles  - turn all polygons to triangles

polygons2polylists  - turn all polygons to polylists

polylists2lines     - turn all polylists to lines

polylists2triangles - turn all polylists to triangles

polylists2polygons  - turn all polylists to polygons

Triangles2lines     - turn all triangles to lines

Trifanner           - turn all triangles to trifans

Trifanner           - turn all triangles to tristrips
