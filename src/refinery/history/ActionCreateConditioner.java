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
import refinery.document.ConditionerWrapper;


/**
 * An <code>ActionCreateConditioner</code> is the undoable action representing a Conditioner creation.
 * 
 * @author Philippe David
 */
public class ActionCreateConditioner extends Action {
	private UUID condID;
	private UUID dispID;
	private int inputNumber;
	private int outputNumber;
	private boolean variableInputNumber;
	private Vector<UUID> AnchInIDs;
	private UUID AnchOutID;
	
	String name;
	String libName;
	String description;
	Point position;
	
	public ActionCreateConditioner(ActionHistory h, ConditionerWrapper cw){
		super(h);
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
			dispID = Identifiable.ZERO;
		}
		
		name = cw.getName();
		libName = cw.getLibName();
		description = cw.getDescription();
		
		inputNumber = cw.getInputNumber();
		outputNumber = cw.getOutputNumber();
		variableInputNumber = cw.isVariableInputNumber();
	}
	
	@Override
	public void undo() {
		ConditionerWrapper cw = (ConditionerWrapper) history.getFromUID(condID);
		
		//TODO once ActionEdit is fixed this should not be necessary anymore
		if(cw.isVariableInputNumber()){
			Iterator<Anchor> ancs = cw.getInputAnchors();
			while (ancs.hasNext()){
				Anchor anc = ancs.next();
				if (!AnchInIDs.contains(anc.getUID())){
					AnchInIDs.add(anc.getUID());
				}
				history.bigBag.put(anc.getUID(),anc);
			}
		}
		
		history.document.removeConditioner(cw);
	}

	@Override
	public void redo() {
		ConditionerWrapper cw = history.document.addConditioner(Refinery.instance.getAvailableCondList().getConditionerBase(libName));
		cw.setName(name);
		cw.setInputNumber(inputNumber);
		cw.setUID(condID);
		history.bigBag.put(condID, cw);
		if (! dispID.equals(Identifiable.ZERO)){
			cw.getDisplay().setUID(dispID);
			history.bigBag.put(dispID, cw.getDisplay());
			cw.getDisplay().setPosition(position);
		}
			
		
		Iterator<UUID> uuids = AnchInIDs.iterator();
		cw.setInputNumber(AnchInIDs.size());
		Iterator<Anchor> ancs = cw.getInputAnchors();
		while(uuids.hasNext()){
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

}
