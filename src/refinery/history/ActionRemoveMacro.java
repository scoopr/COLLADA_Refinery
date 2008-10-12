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
