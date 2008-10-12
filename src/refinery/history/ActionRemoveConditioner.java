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
import refinery.document.ConditionerWrapper;


/** An <code>ActionEditIO</code> is the undoable action representing a Conditioner deletion.
* 
* @author Philippe David
*/
public class ActionRemoveConditioner extends Action {
	private UUID condID;
	private UUID dispID;
	private Vector<UUID> AnchInIDs;
	private UUID AnchOutID;
	private int inputNumber;
	private int outputNumber;
	private boolean variableInputNumber;
	
	
	String name;
	String description;
	String libName;
	Point position;
	
	public ActionRemoveConditioner(ActionHistory h, ConditionerWrapper cw){
		super(h);
		
		name = cw.getName();
		description = cw.getDescription();
		libName = cw.getLibName();
		inputNumber = cw.getInputNumber();
		outputNumber = cw.getOutputNumber();
		variableInputNumber = cw.isVariableInputNumber();
		
		AnchInIDs = new Vector<UUID>();
		
		condID = cw.getUID();
		h.bigBag.put(condID, cw);
		
		
		Iterator<Anchor> ancs = cw.getInputAnchors();
		while (ancs.hasNext()){
			Anchor anc = ancs.next();
			AnchInIDs.add(anc.getUID());
			h.bigBag.put(anc.getUID(),anc);
		}
		
		if (cw.getOutputNumber() == 1){
			AnchOutID = cw.getOutputAnchor(0).getUID();
			h.bigBag.put(AnchOutID, cw.getOutputAnchor(0));
		}
		
		if (cw.getDisplay() != null){
			dispID = cw.getDisplay().getUID();
			h.bigBag.put(dispID, cw.getDisplay());
			position = cw.getDisplay().getPosition();
		} else {
			dispID = IdentifiableImpl.ZERO;
		}
	}
	

	public void undo() {
		ConditionerWrapper cw = history.document.addConditioner(Refinery.instance.getAvailableCondList().getConditionerBase(libName));
		cw.setName(name);
		cw.setInputNumber(inputNumber);
		cw.setUID(condID);
		history.bigBag.put(condID, cw);
		if (! dispID.equals(IdentifiableImpl.ZERO)){
			cw.getDisplay().setUID(dispID);
			history.bigBag.put(dispID, cw.getDisplay());
			cw.getDisplay().setPosition(position);
		}
		
		Iterator<UUID> uuids = AnchInIDs.iterator();
		Iterator<Anchor> ancs = cw.getInputAnchors();
		while(uuids.hasNext() && ancs.hasNext()){
			Anchor anc =  ancs.next();
			UUID uuid = uuids.next();
			anc.setUID(uuid);
			history.bigBag.put(uuid, anc);
		}
		if (cw.getOutputNumber() == 1){
			cw.getOutputAnchor(0).setUID(AnchOutID);
			history.bigBag.put(AnchOutID, cw.getOutputAnchor(0));
		}
	}

	@Override
	public void redo() {
		ConditionerWrapper cw = (ConditionerWrapper) history.getFromUID(condID);
		history.document.removeConditioner(cw);
	}

}
