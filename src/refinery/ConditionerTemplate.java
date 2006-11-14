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
package refinery;

/**
 * The <code>ConditionerTemplate</code> is a template of Conditioner that can be manipulated through
 * the GUI. Its parameters are to be set by an external dll, loaded by ExternalLib. Then instances 
 * of a conditioner and its parameters are represented by a ConditionerWrapper.
 * 
 * @author Philippe David
 */
public class ConditionerTemplate {
	private String IDName;
	private String description;
	private String category;
	private int count;
	
	
	public ConditionerTemplate(String _name, String _description, String _category){
		IDName = _name;
		description = _description;
		category = _category;
		
		count = 0;
		//System.out.println("new conditioner "+ IDName +" : "+ description +" : "+ category);
	}

	public String getIDNameWithCount() {
		// To prevent conditioners from having the same name at creation, we add a counter at the end
		// of the name
		count++;
		return IDName+"-"+count;
	}

	public String getIDName()
	{
		return IDName;
	}

	public String getDescription()
	{
		return description;
	}

	public String getCategory()
	{
		return category;
	}	
	
	public String toString(){
		return IDName;
	}
}
