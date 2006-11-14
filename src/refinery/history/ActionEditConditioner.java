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

import java.util.Hashtable;
import java.util.Iterator;
import java.util.Map;
import java.util.UUID;
import java.util.Vector;

import refinery.document.ConditionerWrapper;


/**
 * An <code>ActionEditConditioner</code> is the undoable action representing a Conditioner edition.
 * 
 * @author Philippe David
 */
public class ActionEditConditioner extends Action {
	private UUID condID;
	String initialName;
	String newName;
	
	private Hashtable<String, Boolean> initialBoolOptions;
	private Hashtable<Integer, String> initialFileOptions;
	private Hashtable<String, String> initialStringOptions;
	private Hashtable<String, Float> initialFloatOptions;
	
	private Hashtable<String, Boolean> newBoolOptions;
	private Hashtable<Integer, String> newFileOptions;
	private Hashtable<String, String> newStringOptions;
	private Hashtable<String, Float> newFloatOptions;
	
	private Vector<UUID> inputAnchorIDs;
	private int initialInputNumber;
	private int newInputNumber;
		
	public ActionEditConditioner(ActionHistory h, ConditionerWrapper cw, String _initialName, String _newName){
		super(h);
		
		h.bigBag.put(cw.getUID(), cw);
		
		condID = cw.getUID();
		initialName = _initialName;
		newName = _newName;
		
		initialBoolOptions = new Hashtable<String, Boolean>();
		newBoolOptions  = new Hashtable<String, Boolean>();
		initialFileOptions = new Hashtable<Integer, String>();
		newFileOptions =  new Hashtable<Integer, String>();
		initialStringOptions = new Hashtable<String, String>();
		newStringOptions = new Hashtable<String, String>();
		initialFloatOptions = new Hashtable<String, Float>();
		newFloatOptions = new Hashtable<String, Float>();
		
		inputAnchorIDs = null;
	}
	
	public void addBoleanOptionChange(String key, Boolean initialValue, Boolean newValue){
		initialBoolOptions.put(key, initialValue);
		newBoolOptions.put(key, newValue);
	}
	
	public void addFileOptionChange(Integer key, String initialValue, String newValue){
		initialFileOptions.put(key, initialValue);
		newFileOptions.put(key, newValue);
	}
	
	public void addStringOptionChange(String key, String initialValue, String newValue){
		initialStringOptions.put(key, initialValue);
		newStringOptions.put(key, newValue);
	}

	public void addFloatOptionChange(String key, Float initialValue, Float newValue){
		initialFloatOptions.put(key, initialValue);
		newFloatOptions.put(key, newValue);
	}
	
	
	/**
	 * willRaiseInputNumber stores in the history a raise in the number of input. This method MUST be called BEFORE
	 * the change is done, hence its name. We need to track the uuid of the anchors that are going to be deleted.
	 *
	 */
	public void willLowerInputNumber(int initialNumber, int newNumber){
		initialInputNumber = initialNumber;
		newInputNumber = newNumber;
		inputAnchorIDs = new Vector<UUID>();
		ConditionerWrapper cw =  (ConditionerWrapper) history.getFromUID(condID);
		if (initialNumber > newNumber){ 
			// We will delete some anchors and need to recreate them with their correct id during undo. 
			for(int i = newNumber; i < initialNumber; i++){
				inputAnchorIDs.add(cw.getInputAnchor(i).getUID());
			}
		}
		
	}
	
	/**
	 * loweredInputNumber stores in the history a decrease of the number of inputs. This method MUST be called AFTER
	 * the change is done, hence its name. We need to track  the uuid of the anchors that has just been created.
	 * @param initialNumber
	 * @param newNumber
	 */
	public void raisedInputNumber(int initialNumber, int newNumber){
		initialInputNumber = initialNumber;
		newInputNumber = newNumber;
		inputAnchorIDs = new Vector<UUID>();
		ConditionerWrapper cw =  (ConditionerWrapper) history.getFromUID(condID);
		if (initialNumber < newNumber){
			// We will create some anchors, delete them during undo and we must redo them with the correct id
			for(int i = initialNumber; i < newInputNumber; i++){
				inputAnchorIDs.add(cw.getInputAnchor(i).getUID());
			}
		}
	}
	
	@Override
	public void undo() {
		ConditionerWrapper cw =  (ConditionerWrapper) history.getFromUID(condID);

		cw.setName(initialName);

		// Boolean Options
		Iterator<Map.Entry<String, Boolean>> boolOptIt = initialBoolOptions.entrySet().iterator();
		while (boolOptIt.hasNext()){
			Map.Entry<String, Boolean> entry = boolOptIt.next();
			cw.getOptionList().getOption(entry.getKey()).isSet = entry.getValue();
		}
		
		// File Options
		Iterator<Map.Entry<Integer, String>> fileOptIt = initialFileOptions.entrySet().iterator();
		while(fileOptIt.hasNext()){
			Map.Entry<Integer, String> fileEntry = fileOptIt.next();
			cw.setOutput(fileEntry.getKey(), fileEntry.getValue());
		}
		
		// String Options
		Iterator<Map.Entry<String, String>> stringOptIt = initialStringOptions.entrySet().iterator();
		while(stringOptIt.hasNext()){
			Map.Entry<String, String> stringEntry = stringOptIt.next();
			cw.getOptionList().setValue(stringEntry.getKey(), stringEntry.getValue());
		}
		
		if (inputAnchorIDs!= null){
			cw.setInputNumber(initialInputNumber);
			if (initialInputNumber > newInputNumber){
				Iterator<UUID> ancIds = inputAnchorIDs.iterator();
				for(int i = newInputNumber; i < initialInputNumber; i++){
					UUID id = ancIds.next();
					cw.getInputAnchor(i).setUID(id);
					history.bigBag.put(id, cw.getInputAnchor(i));
				}
			}
		}
		
	}

	@Override
	public void redo() {
		ConditionerWrapper cw =  (ConditionerWrapper) history.getFromUID(condID);
		
		cw.setName(newName);
		
		// Boolean Options
		Iterator<Map.Entry<String, Boolean>> boolOptIt = newBoolOptions.entrySet().iterator();
		while (boolOptIt.hasNext()){
			Map.Entry<String, Boolean> entry = boolOptIt.next();
			cw.getOptionList().getOption(entry.getKey()).isSet = entry.getValue();
		}
		
		// File Options
		Iterator<Map.Entry<Integer, String>> fileOptIt = newFileOptions.entrySet().iterator();
		while(fileOptIt.hasNext()){
			Map.Entry<Integer, String> fileEntry = fileOptIt.next();
			cw.setOutput(fileEntry.getKey(), fileEntry.getValue());
		}
		
		// String Options
		Iterator<Map.Entry<String, String>> stringOptIt = newStringOptions.entrySet().iterator();
		while(stringOptIt.hasNext()){
			Map.Entry<String, String> stringEntry = stringOptIt.next();
			cw.getOptionList().setValue(stringEntry.getKey(), stringEntry.getValue());
		}
		
		if (inputAnchorIDs!= null){
			cw.setInputNumber(newInputNumber);
			if (newInputNumber > initialInputNumber){
				Iterator<UUID> ancIds = inputAnchorIDs.iterator();
				for(int i = initialInputNumber; i < newInputNumber; i++){
					UUID id = ancIds.next();
					cw.getInputAnchor(i).setUID(id);
					history.bigBag.put(id, cw.getInputAnchor(i));
				}
			}
		}
		
	}

}
