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
package refinery.document;

import java.awt.Point;
import java.util.Iterator;
import java.util.Vector;


/**
 * The <code>GraphNode</code> class is used by the Refinery.makeCallList method 
 * to build the sequence of calls, as well as checking the consistency of the graph.
 * @author Philippe David
 *
 */
public class GraphNode {
	Box box;
	int level;
	Vector<GraphNode> linksOut;
	boolean changing;
	Vector<Vector<GraphNode>> nodesOrderedPerLevels;
	int width;
	int xShift;
	
	final int GRID_WIDTH = 150;
	final int GRID_HEIGHT = 125; 
	
	
	public GraphNode(Box _box, int _level, Vector<Vector<GraphNode>> _nodesOrderedPerLevels) throws GraphException{
		box = _box;
		changing = false;
		linksOut = new Vector<GraphNode>();
		level = _level;
		nodesOrderedPerLevels = _nodesOrderedPerLevels;
		nodesOrderedPerLevels.get(level).add(this);
	}
	
	/**
	 * Change the level of a node and its sub nodes. Check the consistency of the Graph, 
	 * i.e. if their is no cycle in the recurrent level change.
	 * The method is also responsible fo updating the list nodesOrderedPerLevels.
	 * @param newLevel The new level for this node
	 * @return True if succeeded, false if an error was detected.
	 */
	public void changeLevel(int newLevel)
				throws GraphException{
		if (changing) throw new GraphException("Cycle detected !");
		
		nodesOrderedPerLevels.get(level).remove(this);
		level = newLevel;
		try{
			nodesOrderedPerLevels.get(level).add(this);
		} catch (ArrayIndexOutOfBoundsException e){
			nodesOrderedPerLevels.add(new Vector<GraphNode>());
			nodesOrderedPerLevels.get(level).add(this);
		}
		
		changing = true;
		
		Iterator<GraphNode> lOutsIt = linksOut.iterator();
		while (lOutsIt.hasNext()){
			GraphNode lOut = lOutsIt.next();
			lOut.changeLevel(level+1);
			
		}
		changing = false;
	}
	
	/**
	 * Add a link from this node to newLink, and change the level of the sub node if needed
	 * @param newLink New sub node of the current node.
	 * @return True if succeeded, false if an error was detected. 
	 */
	public void addLinkTo(GraphNode newLink) throws GraphException{
		linksOut.add(newLink);
		if (newLink.level <= level){
			newLink.changeLevel(level+1);
		}
	}
	
	/**
	 * Find a box in the graph, starting from the current node.
	 * @param searchedBox The box we want to find
	 * @return The GraphNode containing the Box if found, null otherwise
	 */
	public GraphNode findBox(Box searchedBox){
		if (box != null && box.equals(searchedBox)) return this;
		
		Iterator<GraphNode> sonsIt = linksOut.iterator();
		while(sonsIt.hasNext()){
			GraphNode son = sonsIt.next();
			GraphNode result = son.findBox(searchedBox);
			if (result != null) return result;
		}
		
		return null;
	}
	
	/**
	 * Calculates the width of this node based on its children one. It is based on the formula
	 * width = max(1, sum(child.width, child being a child of this node) ).
	 * This method is to be called once the tree is built, or else the width is always one because the node has no child yet.
	 * Once the width is computed its value can be accessed through width, but you have to be aware that if you modify
	 * the tree, you have to call this method on the root node to have correct values. The width member is only a cache.
	 * @return The width for this nodes and all its sub nodes.
	 */
	public int computeWidth(){
		width = 0;
		for (Iterator<GraphNode> it = linksOut.iterator(); it.hasNext();){
			GraphNode child = it.next();
			width += child.computeWidth();
			//width++;
		}
		
		if (width < 1){
			width = 1;
		}
		return width;
	}
	
	
	/**
	 * computeXShift will recompute the x position of the boxes in this document and set the boxes position.
	 * Prerequisites are :
	 * - levels must be computed already.
	 * - we assume there is no cycle in the graph, the check must be done before
	 */
	public void computeXShift(){
		resetXShift();
		computeWidth();
		computeXShift(0);
	}
	
	public void resetXShift(){
		xShift = -1;
		for (Iterator<GraphNode> it = linksOut.iterator(); it.hasNext();){
			GraphNode child = it.next();
			child.resetXShift();
		}
	}
	
	public void computeXShift(int parentXShift){
		// We want to set the position only once even if this node has more than one parent.
		if (xShift != -1) return;
		
		xShift = parentXShift;
		int brotherShift = 0;
		for (Iterator<GraphNode> it = linksOut.iterator(); it.hasNext();){
			GraphNode child = it.next();
			child.computeXShift(parentXShift + brotherShift);
			if (child.xShift >= parentXShift){
				brotherShift += child.width;
				//brotherShift += child.computeWidth();
				//brotherShift++;
			}
		}
		
		if(box != null){
			box.getDisplay().setPosition(new Point(xShift*GRID_WIDTH+50, (level-1)*GRID_HEIGHT+50));
		}
	}
	
	
	
}
