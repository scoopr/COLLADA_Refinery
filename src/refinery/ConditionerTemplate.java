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
