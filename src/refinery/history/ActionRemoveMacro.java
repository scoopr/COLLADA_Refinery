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

import java.awt.Point;
import java.util.Iterator;
import java.util.UUID;
import java.util.Vector;

import refinery.Refinery;
import refinery.document.Anchor;
import refinery.document.PipelineMacro;

public class ActionRemoveMacro extends Action {
	private UUID macroID;
	private UUID displayID;
	private Vector<UUID> anchInID;
	private Vector<UUID> anchOutID;
	
	private String IDName;
	private Point position;
	
	public ActionRemoveMacro(ActionHistory h, PipelineMacro macro){
		super(h);
		
		macroID = macro.getUID();
		h.bigBag.put(macroID, macro);
		
		anchInID = new Vector<UUID>();
		Iterator<Anchor> anchInIt = macro.getInputAnchors();
		while(anchInIt.hasNext()){
			Anchor anc = anchInIt.next();
			anchInID.add(anc.getUID());
			h.bigBag.put(anc.getUID(), anc);
		}
				
		anchOutID = new Vector<UUID>();
		Iterator<Anchor> anchOutIt = macro.getOutputAnchors();
		while(anchOutIt.hasNext()){
			Anchor anc = anchOutIt.next();
			anchOutID.add(anc.getUID());
			h.bigBag.put(anc.getUID(), anc);
		}
		
		
		if(macro.getDisplay() != null){
			displayID = macro.getDisplay().getUID();
			h.bigBag.put(displayID, macro.getDisplay());
			position = macro.getDisplay().getPosition();
		} else {
			displayID = Identifiable.ZERO;
		}
		
		IDName = macro.getLibName();
	}
	
	@Override
	public void undo() throws Exception {
		PipelineMacro macro = history.document.addMacro(Refinery.instance.getAvailableMacroList().getMacroBaseFromLibName(IDName));
		macro.setUID(macroID);
		history.bigBag.put(macroID, macro);

		if (! displayID.equals(Identifiable.ZERO)){
			macro.getDisplay().setUID(displayID);
			history.bigBag.put(displayID, macro.getDisplay());
			macro.getDisplay().setPosition(position);
		}
		
		Iterator<Anchor> anchInIt = macro.getInputAnchors();
		Iterator<UUID> uuids = anchInID.iterator();
		while(anchInIt.hasNext()){
			Anchor anc = anchInIt.next();
			anc.setUID(uuids.next());
		}
		
		
		Iterator<Anchor> anchOutIt = macro.getOutputAnchors();
		uuids = anchOutID.iterator();
		while(anchOutIt.hasNext()){
			Anchor anc = anchOutIt.next();
			anc.setUID(uuids.next());
		}

	}

	@Override
	public void redo() {
		PipelineMacro macro = (PipelineMacro) history.getFromUID(macroID);
		history.document.removeMacro(macro);

	}

}
