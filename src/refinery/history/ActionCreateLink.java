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
package refinery.history;

import java.util.UUID;

import refinery.document.Anchor;
import refinery.document.Link;


/**
 * An <code>ActionCreateLink</code> is the undoable action representing a Link creation.
 * 
 * @author Philippe David
 */
public class ActionCreateLink extends Action {
	private UUID linkID;
	
	private UUID firstAnchorID;
	private UUID secondAnchorID;
	
	public ActionCreateLink(ActionHistory h, Link link){
		super(h);
		
		linkID = link.getUID();
		firstAnchorID = link.first.getUID();
		secondAnchorID = link.second.getUID();
		
		h.bigBag.put(linkID, link);
		h.bigBag.put(firstAnchorID, link.first);
		h.bigBag.put(secondAnchorID, link.second);
	}

	@Override
	public void undo() {
		Link link = (Link) history.getFromUID(linkID);
		history.document.removeLink(link);
	}

	@Override
	public void redo() {
		Anchor anc1 = (Anchor) history.bigBag.get(firstAnchorID);
		Anchor anc2 = (Anchor) history.bigBag.get(secondAnchorID);
		
		Link link = new Link(anc1, anc2);
		link.setUID(linkID);
		history.bigBag.put(linkID, link);
		history.document.addLink(link);
	}

}
