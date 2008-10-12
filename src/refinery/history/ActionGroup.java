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
