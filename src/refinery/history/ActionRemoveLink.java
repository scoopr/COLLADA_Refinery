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
package refinery.history;

import java.util.UUID;

import refinery.document.Anchor;
import refinery.document.Link;


/** An <code>ActionEditIO</code> is the undoable action representing a Link deletion.
* 
* @author Philippe David
*/
public class ActionRemoveLink extends Action {
	private UUID linkID;
	
	private UUID firstAnchorID;
	private UUID secondAnchorID;
	
	public ActionRemoveLink(ActionHistory h, Link link){
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
		Anchor anc1 = (Anchor) history.bigBag.get(firstAnchorID);
		Anchor anc2 = (Anchor) history.bigBag.get(secondAnchorID);
		
		Link link = new Link(anc1, anc2);
		link.setUID(linkID);
		history.bigBag.put(linkID, link);
		history.document.addLink(link);
	}

	@Override
	public void redo() {
		Link link = (Link) history.getFromUID(linkID);
		history.document.removeLink(link);
	}

}
