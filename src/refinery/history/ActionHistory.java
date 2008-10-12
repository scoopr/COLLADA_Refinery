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

import java.util.Hashtable;
import java.util.LinkedList;
import java.util.UUID;

import refinery.display.Display;
import refinery.document.ConditionerWrapper;
import refinery.document.IOBox;
import refinery.document.Link;
import refinery.document.Pipeline;
import refinery.document.PipelineMacro;


/**
 * The <code>ActionHistory</code> class provides methods to create and store actions that can be 
 * undone and redone.
 * 
 * @author Philippe David
 */
public class ActionHistory {
	Pipeline document;
	LinkedList<Action> undoHistory; 
	LinkedList<Action> redoHistory;
	Hashtable<UUID, Identifiable> bigBag;
	
	// This is used to prevent filling the undo history when actions actually come from our undo/redo methods
	private boolean ignoreModifications;
	
	// This is to pack all move actions together instead of having many little of them.
	//  The GUI should tell when to start packing and when to stop with setPackMoves
	private boolean packMoves;
	private ActionMoveDisplay packMovesFrom;
	
	// This is used to pack different actions in a group, to undo/redo them in once
	private boolean packAll;
	private ActionGroup packAllIn;
	
	
	public ActionHistory(Pipeline _document){
		document = _document;
		bigBag = new Hashtable<UUID, Identifiable>();
		undoHistory = new LinkedList<Action>();
		redoHistory = new LinkedList<Action>();
		ignoreModifications = false;
		packMoves = false;
		packMovesFrom = null;
		packAll = false;
		packAllIn = null;
	}
	
	public Pipeline getDocument(){
		return document;
	}
	
	/**
	 * Undo last actions in the stack
	 * @throws HistoryException 
	 */
	public void undo() throws HistoryException{
		ignoreModifications = true;
		
		Action a = undoHistory.poll();
		if (a != null){
			try {
				a.undo();
			} catch (Exception e) {
				throw new HistoryException("Error while undoing : "+e.getMessage());
			}
			redoHistory.addFirst(a);
		}
		
		document.getDisplayArea().repaint();
		ignoreModifications = false;
	}
	
	/**
	 * Redo last action in the redo stack
	 * @throws Exception 
	 */
	public void redo() throws HistoryException{
		ignoreModifications = true;
		
		Action a = redoHistory.poll();
		if (a != null){
			try {
				a.redo();
			} catch (Exception e) {
				throw new HistoryException("Error while redoing : "+e.getMessage());
			}
			undoHistory.addFirst(a);
		}
		
		document.getDisplayArea().repaint();
		ignoreModifications = false;
	}
	
	/**
	 * Set the boolean that indicates that all futures moves actions have to be packed in one single action.
	 * @param pack Boolean with the new value for PackMove.
	 */
	public void setPackMoves(boolean pack){
		packMoves = pack;
		packMovesFrom = null;
	}
	
	/**
	 * All future Actions are to be packed in an ActionGroup
	 */
	public void startPackAll(){
		if (!ignoreModifications){
			packAll = true;
			packAllIn = new ActionGroup(this);
		}
		
	}
	
	/**
	 * Stop packing in the ActionGroup
	 */
	public void endPackAll(){
		if (!ignoreModifications){
			packAll = false;
			addActionToHistory(packAllIn);
		}
		
	}
	
	/**
	 * Set the <code>reversed</code> of the current pack. This will execute undo and redo of this pack in a reversed way.
	 * For sone reasons we might want to do this.
	 *
	 */
	public void reverseCurrentPack(){
		if (!ignoreModifications && packAll){
			packAllIn.reversed = true;
		}
	}
	
	/**
	 * Add a mark in the history. This is used to call packSinceMark() later. This mechanism must not be started when 
	 * packAll is active because packSinceMark will not search the mark in the packs. But you can use some packAll
	 * between a addMark - packSinceMark.
	 * @return
	 */
	public Mark addMark(){
		if (packAll){ // This is a protection because packAll should not be active at this point.
						// With a null mark, packSinceMark will not try to find the mark in the history.
			return null;
		}
		Mark m = new Mark(this);
		addActionToHistory(m);
		return m;
	}
	
	public void packSinceMark(Mark m){
		if (m == null){
			return;
		}
		
		ActionGroup groupAction = new ActionGroup(this);
		Action a;
		while( ( a = undoHistory.poll()) != null){
			if (a == m){
				undoHistory.addFirst(groupAction);
				return;
			} else {
				groupAction.addFirstAction(a);
			}
		}
		
		// Ok so we just screwed up all the history and did not find the mark... 
		// This method was not called properly but let's rebuild the history then...
		undoHistory = new LinkedList<Action>(groupAction.getActionList());
		
	}
	
	/**
	 * Get an object in the bag from its UUID
	 */
	public Object getFromUID(UUID id){
		return bigBag.get(id);
	}
	
	/****** undoable actions ******/
	
	public void addDisplayMove(Display d, int initialX, int initialY, int destinationX, int destinationY){
		if (!ignoreModifications){
			// Drop identity moves
			if (initialX == destinationX && initialY == destinationY) return;
			
			if (packMoves){
				if (packMovesFrom == null){
					// This is a first action of a "pack move" set
					ActionMoveDisplay actionMove = new ActionMoveDisplay(this, d, initialX, initialY, destinationX, destinationY);
					addActionToHistory(actionMove);
					packMovesFrom = actionMove;
				} else {
					// This is an action to pack with the previous one
					packMovesFrom.setDestination(destinationX, destinationY);
				}
			} else {
				// Single action, no packing
				ActionMoveDisplay actionMove = new ActionMoveDisplay(this, d, initialX, initialY, destinationX, destinationY);
				addActionToHistory(actionMove);
			}
		}
		
		
	}
	
	public ActionEditConditioner addEditConditioner(ConditionerWrapper cw, String oldName, String newName){
		if (!ignoreModifications){
			ActionEditConditioner actionEdit = new ActionEditConditioner(this, cw, oldName, newName);
			addActionToHistory(actionEdit);
			return actionEdit;
		}
		
		return null;
	}
	
	public void addEditIO(IOBox io, String oldName, String oldFilePath, String newName, String newFilePath){
		if (!ignoreModifications){
			ActionEditIO actionEdit = new ActionEditIO(this, io, oldName, oldFilePath, newName, newFilePath);
			addActionToHistory(actionEdit);
		}
	}
	
	public void addCreateLink(Link link){
		if (!ignoreModifications){
			ActionCreateLink actionCreate = new ActionCreateLink(this, link);
			addActionToHistory(actionCreate);
		}
	}
	
	public void addRemoveLink(Link link){
		if (!ignoreModifications){
			ActionRemoveLink actionRemove = new ActionRemoveLink(this, link);
			addActionToHistory(actionRemove);
		}
	}
	
	public void addCreateConditioner(ConditionerWrapper cw){
		if (!ignoreModifications){
			ActionCreateConditioner actionCreate = new ActionCreateConditioner(this, cw);
			addActionToHistory(actionCreate);
		}
	}
	
	public void addRemoveConditioner(ConditionerWrapper cw){
		if (!ignoreModifications){
			ActionRemoveConditioner actionRemove = new ActionRemoveConditioner(this, cw);
			addActionToHistory(actionRemove);
		}
	}
	
	public void addCreateMacro(PipelineMacro macro) throws HistoryException{
		if (!ignoreModifications){
			try{
				ActionCreateMacro actionCreate = new ActionCreateMacro(this, macro);
				addActionToHistory(actionCreate);
			} catch (Exception e){
				throw new HistoryException(e.getMessage());
			}
			
		}
	}
	
	public void addRemoveMacro(PipelineMacro macro){
			if (!ignoreModifications){
				ActionRemoveMacro actionRemove = new ActionRemoveMacro(this, macro);
				addActionToHistory(actionRemove);
			}
	}
		
	public void addCreateOutputBox(IOBox io){
		if (!ignoreModifications){
			ActionCreateOutputBox actionCreate = new ActionCreateOutputBox(this, io);
			addActionToHistory(actionCreate);
		}
	}
	
	public void addRemoveOutputBox(IOBox io){
		if (!ignoreModifications){
			ActionRemoveOutputBox actionRemove = new ActionRemoveOutputBox(this, io);
			addActionToHistory(actionRemove);
		}
	}
	
	public void addCreateInputBox(IOBox io){
		if(!ignoreModifications){
			ActionCreateInputBox actionCreate = new ActionCreateInputBox(this, io);
			addActionToHistory(actionCreate);
		}
	}
	
	public void addRemoveInputBox(IOBox io){
		if(!ignoreModifications){
			ActionRemoveInputBox actionRemove = new ActionRemoveInputBox(this, io);
			addActionToHistory(actionRemove);
		}
	}
	
	/******* private methods *****/
	
	private void addActionToHistory(Action a){
		if (!packAll){
			undoHistory.addFirst(a);
			redoHistory = new LinkedList<Action>();
		} else {
			packAllIn.addAction(a);
		}
	}
}
