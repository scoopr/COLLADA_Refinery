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
