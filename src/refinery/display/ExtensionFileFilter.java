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
package refinery.display;

import java.io.File;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.swing.filechooser.FileFilter;

public class ExtensionFileFilter extends FileFilter{
	Pattern pattern;
	String description;
	
	public ExtensionFileFilter(String ext, String _description){
		pattern = Pattern.compile(".*\\."+ext+"$");
		description = "*."+ext+" "+_description;
	}
	public boolean accept(File file) {
		if(file.isDirectory()) return true;
		Matcher m = pattern.matcher(file.getName());
		return m.matches();
	}
	
	public String getDescription(){
		return description;
	}
}
