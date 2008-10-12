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
