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

import java.util.Vector;

public class Option<T1>{
	public enum OptionType {BOOL, STRING, FLOAT};
	
	public String name;
	public String fullName;
	public String description;
	public OptionType type;
	public boolean isSet;
	private T1 value;
	public boolean flag;
	
	public Option(String _name, String _fullName, String _description, OptionType _type ) throws Exception{
		
		if(! _name.matches("^[a-zA-Z][a-zA-Z\\-]*$")){
			throw new Exception("Error in Option constructor: name "+ _name +" must contain only lower case letters and a non leading -");
		}
		name = _name;
		fullName  = _fullName;
		description = _description;
		isSet = false;
		type = _type;
		flag = false;
	}
	
	public boolean getFlag()
	{
		return flag;
	}

	public void setFlag( boolean f )
	{
		flag = f;
	}
	
	public T1 getValue(){
		if(isSet){
			return value;
		} else {
			return null;
		}
	}
	
	public void setValue(T1 _value){
		value = _value;
		isSet = true;
	}

	public void unset()
	{
		isSet = false;
	}
	
	public boolean matches(String value){
		if (value.equals("-"+name)){
			return true;
		} else {
			return false;
		}
	}
	
	public String toString(){
		String result =  "  -" + name +"   "+ fullName +" ";
		
		switch(type){
		case BOOL: result += "(BOOL)"; break;
		case STRING: result += "(STRING)"; break;
		case FLOAT: result += "(FLOAT)"; break;
		}
			
		result += " :\n\t" + description + "\n";
		
		return result;
	}
}
