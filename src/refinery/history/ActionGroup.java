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

import java.util.ArrayList;
import java.util.Iterator;

/**
 * An <code>ActionGroup</code> is the undoable action composed of several other Actions. 
 * Undoing or Redoing this Action will undo or redo all the nested actions in the correct order.
 * 
 * @author Philippe David
 */
public class ActionGroup extends Action {
	private ArrayList<Action> actions;
	protected boolean reversed;
	
	public ActionGroup(ActionHistory h){
		super(h);
		actions = new ArrayList<Action>();
		reversed = false;
	}

	public void addAction(Action a){
		actions.add(a);
	}
	
	public void addFirstAction(Action a){
		actions.add(0, a);
	}
	
	public ArrayList<Action> getActionList(){
		return actions;
	}
	
	@Override
	public void undo() throws Exception {
		if (reversed){
			// Special case for some situations, we may want to undo the actions in the same order we created them
			Iterator<Action> actionsIt = actions.iterator();
			while(actionsIt.hasNext()){
				actionsIt.next().undo();
			}
		} else {
			// undo actions in reverse order
			for( int i = actions.size()-1; i>=0; i--) {
				Action a = actions.get(i); 
				a.undo();
			}
		}

	}

	@Override
	public void redo() throws Exception {
		if (reversed){
			// Special case for some situations, we may want to redo the actions in the reversed order we created them
			Iterator<Action> actionsIt = actions.iterator();
			while(actionsIt.hasNext()){
				actionsIt.next().redo();
			}
		} else {
			// redo actions in normal order
			Iterator<Action> it = actions.iterator();
			while (it.hasNext()){
				Action a = it.next();
				a.redo();
			}
		}

	}

}
