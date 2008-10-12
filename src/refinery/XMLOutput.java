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

import java.util.EmptyStackException;
import java.util.Iterator;
import java.util.Stack;
import java.util.Vector;

public class XMLOutput {
	private Stack<Entity> entities;
	private Entity currentEntity;
	
	private StringBuffer output;
	
	
	public XMLOutput(){
		entities = new Stack<Entity>();
		currentEntity = null;
		output = new StringBuffer();
		output.append("<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n");
	}
	
	public void startEntity(String name){
		Entity newEntity = new Entity(name, entities.size() + (currentEntity == null ? 0 : 1));
		if (currentEntity!= null){
			currentEntity.flushBeginingForNewEntity();
			entities.push(currentEntity);
		}
		currentEntity = newEntity;
	}
	
	public void endEntity(){
		if (currentEntity != null){
			currentEntity.end();
			try{
				currentEntity = entities.pop();
			} catch (EmptyStackException e){
				currentEntity = null;
			}
		}
	}
	
	public void addAttribute(String name, String value){
		currentEntity.addAttribute(name, value);
	}
	
	public void addBodyText(String bodyText){
		if (currentEntity!= null){
			currentEntity.flushBeginingForBodyText();
		}
		currentEntity.addBodyText(bodyText);
	}
	
	public void newLine(){
		output.append('\n');
	}
	
	public String toString(){
		return output.toString();
	}
	
	private class Entity{
		String name;
		Vector<Couple<String, String>> attributes;
		boolean attributesFlushed;
		boolean hasBodyText;
		String body;
		int indent;
		
		public Entity(String _name, int _indent){
			name = _name;
			body = "";
			indent = _indent;
			attributesFlushed = false;
			hasBodyText = false;
			attributes = new Vector<Couple<String, String>>();
		}
		
		public void addAttribute(String name, String value){
			//TODO escape text
			if (!attributesFlushed){
				attributes.add(new Couple<String, String>(name, value));
			}
		}
		
		public void flushBeginingForNewEntity(){
			if (!attributesFlushed){
				flushBeginingForBodyText();
				output.append('\n');
			}
		}
		
		public void flushBeginingForBodyText(){
			if (!attributesFlushed){
				for (int i = 0; i < indent; i++){
					output.append('\t');
				}
				output.append("<"+ name);
				Iterator<Couple<String, String>> attrIt = attributes.iterator();
				while(attrIt.hasNext()){
					Couple<String, String> attr = attrIt.next();
					output.append(" " + attr.first + "=\"" + attr.second + "\""); 
				}
				output.append(">");
				
				attributesFlushed = true;
			}
		}
		
		public void addBodyText(String bodyText){
			output.append(bodyText);
			hasBodyText = true;
		}

		public void end(){
			if ( !hasBodyText){
				for (int i = 0; i < indent; i++){
					output.append('\t');
				}
			}
			if (!attributesFlushed){
				// Write all in a line
				output.append("<"+ name);
				Iterator<Couple<String, String>> attrIt = attributes.iterator();
				while(attrIt.hasNext()){
					Couple<String, String> attr = attrIt.next();
					output.append(" " + attr.first + "=\"" + attr.second + "\""); 
				}
				output.append("/>\n");
			} else {
				output.append("</" + name + ">\n");
			}
		}
	}
}
