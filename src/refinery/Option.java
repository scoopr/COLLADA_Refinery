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
