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
package refinery.document;

import java.util.UUID;

import refinery.Couple;
import refinery.display.Display;
import refinery.display.DisplayAreaInterface;
import refinery.display.LinkDisplay;
import refinery.history.Identifiable;

/**
 * The <code>Link</code> class represent a link between two anchors of boxes in the pipeline.
 * Note: A Java class cannot extends 2 classes, so the code for History.Identifiable is duplicated here
 * 
 * @author Philippe David
 */
public class Link extends Couple<Anchor, Anchor> implements Identifiable{
	private UUID id;
	private LinkDisplay linkDisplay;
	
	public Link(Anchor _first, Anchor _second) {
		super(_first, _second);
		id = UUID.randomUUID();
		linkDisplay = null;
	}
	
	public String toString(){
		return "Link "+first+" <-> "+second;
	}
	
	/**
	 * Sets a display area and register display(s) into it.
	 */
	public void setDisplayArea(DisplayAreaInterface area){
		if (linkDisplay == null){
			linkDisplay = new LinkDisplay(this);
		}
		area.addDisplay(linkDisplay);
		
	}
	
	public Display getDisplay(){
		return linkDisplay;
	}
	
	public UUID getUID() {
		return id;
	}

	public void setUID(UUID _id) {
		id = _id;
	}

}
