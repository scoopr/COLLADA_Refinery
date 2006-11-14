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
package refinery.display;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.RenderingHints;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.image.BufferedImage;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Vector;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.swing.JCheckBoxMenuItem;
import javax.swing.JFrame;
import javax.swing.JMenuItem;
import javax.swing.JPanel;
import javax.swing.JPopupMenu;
import javax.swing.JToolTip;
import javax.swing.Scrollable;
import javax.swing.ToolTipManager;

import refinery.Refinery;
import refinery.document.Anchor;
import refinery.document.Box;
import refinery.document.ConditionerWrapper;
import refinery.document.ExecuteException;
import refinery.document.IOBox;
import refinery.document.Link;
import refinery.document.Pipeline;
import refinery.document.PipelineMacro;
import refinery.document.Anchor.AnchorType;

/**
 * The <code>DisplayArea</code> class is a canvas on which we draw boxes and links representing the
 * pipeline we the user is creating. It provides features like Display list, selection, multi-
 * selection, scrollable area.
 * Note: It extends JPanel and overrides its paintComponent to have a customized drawing, and be sure it
 * is compatible with other swing objects displays (drawing on an awt Component would lead to
 * problems with other swing objects)
 * @author Philippe David
 */
public class DisplayArea extends JPanel implements MouseListener, MouseMotionListener, Scrollable, DisplayAreaInterface{
	// This is to please Jaca compiler and remove a warning, but I doubt we ever use it
	private static final long serialVersionUID = 5405027536333486483L;
	
	Pipeline document;					// Keep a reference to the the document this represents
	Refinery refinery;
	MainGUI gui;
	JFrame parentWindow;				// Our parent frame
	BufferedImage bi;					// The buffer we draw on (because we do doublebuffering)
	Graphics2D big;						// The buffer's Graphic2D
	Vector<Display> selectedDisplay;	// List of currentlty selected Displays
	Display	highlightedDisplay;
	Anchor selectedAnchor;				// The currenlty selected Anchor
	Point currentPosition;				// Current position of the mouse
	Point clickPosition;				// Start point of a draging event
	boolean selectingArea;				// Are we currenlty selecting an area. This helps for operations
										//		that last for several mouse events
	Dimension lastDimension;			// Last known dimension of the drawing area
	Dimension preferedSize;
	ArrayList<Display> displayList;		// List of Displays. They are displayed in the order they are in this list
	private JPopupMenu documentPopup;
	private JPopupMenu boxPopup;
	private JCheckBoxMenuItem  autoOrganizeCheckBox;
	
	private int lastPressedButton;
	private Display popupDisplay;
	float zoom = 1f;
	

	public DisplayArea(Pipeline _document, JFrame _parentWindow, Refinery _refinery, MainGUI _gui) {
		document = _document;
		refinery = _refinery;
		gui = _gui;
		parentWindow = _parentWindow;
		
		// Init things
		displayList = new ArrayList<Display>();	
		selectedDisplay = new Vector<Display>();
		highlightedDisplay = null;
		selectingArea = false;
		lastPressedButton = 0;
		popupDisplay = null;
		currentPosition = new Point(0,0);
		lastDimension = new Dimension(0,0);
		//this.setMinimumSize(new Dimension(500,500));
		
		// Get sizw from preferences
		Pattern p = Pattern.compile("^(\\d*),(\\d*)$");
		Matcher m = p.matcher(Refinery.instance.preferences.getValue("GUI_SIZE"));
		// Size
//		preferedSize = new Dimension(700,700);
		if(m.matches()){
			preferedSize = new Dimension(Integer.valueOf(m.group(1)), Integer.valueOf(m.group(2)));
		}
		
		
		
		this.setPreferredSize(preferedSize);
		
		documentPopup = new JPopupMenu("Document");
		JCheckBoxMenuItem  cbmenuItem = new JCheckBoxMenuItem("Align boxes in grid");
		documentPopup.add(cbmenuItem);
		JMenuItem menuItem = new JMenuItem("Organize boxes");
		menuItem.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent e) {
				document.arrangeDisplays();
				repaint();
			}
			
		});
		documentPopup.add(menuItem);
		autoOrganizeCheckBox = new JCheckBoxMenuItem("Auto-organize boxes");
		autoOrganizeCheckBox.setState(document.getAutoArrange());
		autoOrganizeCheckBox.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent e) {
				document.setAutoArrange(autoOrganizeCheckBox.getState());
				repaint();
			}
			
		});
		documentPopup.add(autoOrganizeCheckBox);
		menuItem = new JMenuItem("Properties");
		menuItem.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent e) {
				new DocumentPropertyWindow(document);
				repaint();
			}
			
		});
		documentPopup.add(menuItem);
		menuItem = new JMenuItem("Execute");
		menuItem.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent e) {
				try{
					document.execute();
				} catch (ExecuteException e1){
					refinery.addNormalMessage(e1.getMessage());
				}
			}
		});
		documentPopup.add(menuItem);
		
		
		boxPopup = new JPopupMenu();
		menuItem = new JMenuItem("Delete");
		menuItem.addActionListener(gui.getActionDelete());
		boxPopup.add(menuItem);
		menuItem = new JMenuItem("Properties");
		menuItem.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent e) {
				if (popupDisplay != null){
					popupDisplay.displayPropertyWindows(parentWindow);
					repaint();
				}
				
			}
			
		});
		boxPopup.add(menuItem);
		
		this.addMouseListener(this);
		this.addMouseMotionListener(this);
		
		document.setDisplayArea(this);
		
		
		ToolTipManager.sharedInstance().registerComponent( this);
	}
	
	public JToolTip createToolTip() {
		MultiLineToolTip m = new MultiLineToolTip();
		m.setComponent(this);
		return m;
	}
	
	public void setDocument(Pipeline _document){
		removeAllDisplay();
		document = _document;
		document.setDisplayArea(this);
		document.setAutoArrange(autoOrganizeCheckBox.getState());
		checkDisplaySize();
	}
	
	/**
	 * @return An iterator over currenlty selected displays 
	 */
	public Iterator<Display> getSelectionIterator(){
		return selectedDisplay.iterator();
	}
	
	/* (non-Javadoc)
	 * @see refinery.display.DisplayAreaInterface#addDisplay(refinery.display.Display)
	 */ 
	public void addDisplay(Display d){
		if(! displayList.contains((d))){
			displayList.add(d);
		}
	}
	
	public void addDisplayAtRandomPosition(Display d){
		int rx, ry, rw, rh; // position and size of the random box
		int px, py; // random position
		
		// Place the display somewhere at random
		rw = (int) (0.7 * getWidth());
		rh = (int) (0.3 * getHeight());
		rx = getWidth()/2 - d.getSize().width/2;
		ry = getHeight()/2 - d.getSize().height/2;
		px = (int) (rx + (Math.random()-0.5)*rw);
		py = (int) (ry + (Math.random()-0.5)*rh);
		d.setPosition(new Point(px, py));
		repaint();
		
		addDisplay(d);
	}
	
	/* (non-Javadoc)
	 * @see refinery.display.DisplayAreaInterface#removeAllDisplay()
	 */
	public void removeAllDisplay(){
		displayList = new ArrayList<Display>();
		selectedDisplay.removeAllElements();
	}
	
	/* (non-Javadoc)
	 * @see refinery.display.DisplayAreaInterface#removeDisplay(refinery.display.Display)
	 */
	public void removeDisplay(Display d){
		displayList.remove(d);
		selectedDisplay.remove(d);
	}
	
	/* (non-Javadoc)
	 * @see refinery.display.DisplayAreaInterface#putDisplayOnTop(refinery.display.Display)
	 */
	public void putDisplayOnTop(Display d){
		Anchor anc;
		
		removeDisplay(d);
		addDisplay(d);
		
		// Raise links to the top also
		switch (d.getRefererType()){
		case COND:
		case MACRO:
			// Raise input links of the conditioner
			Iterator<Anchor> ancs = ((Box) d.getReferer()).getInputAnchors();
			while (ancs.hasNext()){
				anc = ancs.next();
				if (anc != null){
					Iterator<Link> links = document.getLinksFromAnchor(anc);
					while(links.hasNext()){
						putDisplayOnTop(links.next().getDisplay());
					}
				}
			}
			
			// Raise output links of the conditioner
			ancs = ((Box) d.getReferer()).getOutputAnchors();
			while(ancs.hasNext()){
				anc = ancs.next();
				Iterator<Link> links = document.getLinksFromAnchor(anc);
				while(links.hasNext()){
					putDisplayOnTop(links.next().getDisplay());
				}
			}
			break;

		case IO:
			// Raise the anchor of the IOBox
			anc = ((IOBox) d.getReferer()).getAnchor();
			if (anc != null){
				Iterator<Link> links = document.getLinksFromAnchor(anc);
				while(links.hasNext()){
					putDisplayOnTop(links.next().getDisplay());
				}
			}
			break;
		}
		
	}

	/**
	 * mouseClicked event. Options are:
	 * - Single clicking on a display, selects it
	 * - Double clicking on a display opens the underlying PropertyWindows to edit 
	 * 		parameters of its referer
	 */
	public void mouseClicked(MouseEvent arg0) {
		Display d;
		Iterator<Display> it;
		
		switch (arg0.getClickCount()){
		case 1: // In case we are clicking a box while a group was selected that is necessary
			d = getDisplayAt(arg0.getX(), arg0.getY());
			if (d != null){
				selectedDisplay.removeAllElements();
				it = displayList.iterator();
				while (it.hasNext()) {
					it.next().deSelect();
				}
				d.select(arg0.getX(), arg0.getY());
				putDisplayOnTop(d);
				selectedDisplay.add(d);
			}
			break;
		case 2: // Double clink: open property window
			d = getDisplayAt(arg0.getX(), arg0.getY());
			if (d != null){
				d.displayPropertyWindows(parentWindow);
			}
			break;
		}
		
		repaint();
		
	}

	/**
	 * mousePressed event. Options are:
	 * - We start moving the currently selected group of display
	 * - We are selecting a display and maybe one of its anchor to start the creation a link
	 * - We are unselecting everything, and maybe start to select an area.
	 */
	public void mousePressed(MouseEvent arg0) {
		lastPressedButton = arg0.getButton();
		switch(lastPressedButton){
		case MouseEvent.BUTTON1:
			
			Iterator<Display> it;
			
			Display d = getDisplayAt(arg0.getX(), arg0.getY());
			if (d != null){ //A display is under the mouse, let's select it
				if (selectedDisplay.size() > 1 && selectedDisplay.contains(d)){ 
					//maybe we want to move a group ? If this is just a click it will be detected after in MouseClicked
					it = selectedDisplay.iterator();
					while (it.hasNext()) {
						it.next().select(arg0.getX(), arg0.getY());
					}
				} else { // We are selecting a display and possibly an anchor
					selectedDisplay.removeAllElements();
					it = displayList.iterator();
					while (it.hasNext()) {
						it.next().deSelect();
					}
					d.select(arg0.getX(), arg0.getY());
					putDisplayOnTop(d);
					selectedDisplay.add(d);
					selectedAnchor = d.getAnchorAt(arg0.getX(), arg0.getY());
//					No idea why I wrote that...
//					if (selectedAnchor!= null && selectedAnchor.getType() == Anchor.AnchorType.ANCHORIN && document.getLinksFromAnchor(selectedAnchor).hasNext()){
//						selectedAnchor = null;
//					}
				}
				
				// TODO double check that's correct
				document.getHistory().setPackMoves(true);
			} else { // No display under ? We are selecting an area
				selectedDisplay.removeAllElements();
				it = displayList.iterator();
				while (it.hasNext()) {
					it.next().deSelect();
				}
				selectingArea = true;
			}
			currentPosition = new Point(arg0.getX(), arg0.getY());
			clickPosition = new Point(arg0.getX(), arg0.getY());
			
			repaint();
			break;
			
		
		}
		
	}

	/**
	 * mouseReleased event. Options are:
	 * - We are finishing an area selection
	 * - We are finishing a link creation
	 */
	public void mouseReleased(MouseEvent arg0) {
		switch(arg0.getButton()){
		case MouseEvent.BUTTON1:
			if(selectingArea){
				// Let's find all we have to select :)
				Rectangle selectionRect = new Rectangle(Math.min(currentPosition.x, clickPosition.x),
						Math.min(currentPosition.y, clickPosition.y),
						Math.abs(currentPosition.x - clickPosition.x),
						Math.abs(currentPosition.y - clickPosition.y)
				);
				
				Iterator<Display> it = displayList.iterator();
				while (it.hasNext()) {
					Display d = it.next();
					if (d.intersects(selectionRect)){
						d.select();
						selectedDisplay.add(d);
					}
				}
			} else {
				if (selectedAnchor != null) {
					/**
					 * The user is ending a link creation at this point. The policy is to:
					 * 1 - Check if an anchor of the opposite type of the one at the other end of the link is 
					 * 		directly hit. If so the link ends here. If another link existed to this Anchor it 
					 * 		is deleted
					 * 2 - If not, and in the case of the box only has one Anchor of the type we want, the link
					 * 		will end on this anchor.
					 * 3 - In the case where we have multiple anchors of the type we want, we take the first
					 * 		available one. If none is available, the link creation is aborted.  
					 */
					if (highlightedDisplay != null){
						highlightedDisplay.unHighlight();
					}
					
					Display dTarget = getDisplayAt(arg0.getX(), arg0.getY());
					if (dTarget != null){ // First we check the user is hiting a Display
						Anchor targetAnchor = dTarget.getAnchorAt(arg0.getX(), arg0.getY());
						
						// If case 1
						if (targetAnchor != null ) { 
							Link newLink = new Link(selectedAnchor, targetAnchor);
							document.addLink(newLink);						
						} else { // case 2 and 3
							if (dTarget.getRefererType() == Display.RefererType.COND || dTarget.getRefererType() == Display.RefererType.MACRO ||
									dTarget.getRefererType() == Display.RefererType.IO){ //case 2 and 3 implemented in getLinkEndAnchor 
								targetAnchor = ((Box) dTarget.getReferer()).getLinkEndAnchor(selectedAnchor.getType().equals(AnchorType.ANCHORIN) ? AnchorType.ANCHOROUT : AnchorType.ANCHORIN);
								
								if(targetAnchor != null){
									Link newLink = new Link(selectedAnchor, targetAnchor);
									document.addLink(newLink);
										
								}
							}
						}
					}
				}
			}
			
			selectingArea = false;
			selectedAnchor = null;
			document.getHistory().setPackMoves(false);
			repaint();
			break;
			
		default:
			popupDisplay = getDisplayAt(arg0.getX(), arg0.getY());
			if(popupDisplay == null){
				if(documentPopup.isPopupTrigger(arg0)){
					documentPopup.show(arg0.getComponent(), arg0.getX(), arg0.getY());
				}
			} else {
				if(boxPopup.isPopupTrigger(arg0) && popupDisplay.getReferer() instanceof Box){
					boxPopup.setLabel(((Box)popupDisplay.getReferer()).getName());
					boxPopup.updateUI();
					boxPopup.show(arg0.getComponent(), arg0.getX(), arg0.getY());
				}
			}
			break;
		}
	}

	public void mouseEntered(MouseEvent arg0) {
		
	}

	public void mouseExited(MouseEvent arg0) {
		
	}

	/**
	 * mouseDragged event. Options are:
	 * - we are moving one or more display
	 * - update current position for link creation
	 */
	public void mouseDragged(MouseEvent arg0) {		
		switch(lastPressedButton){
		case MouseEvent.BUTTON1:
			if (selectedDisplay.size() > 0){
				if (selectedAnchor == null) { // if we are not creating a link, then we are moving (a) diplay(s)
					Iterator<Display> it = selectedDisplay.iterator();
					while(it.hasNext()){
						Display d = it.next();
						d.mouseMove(arg0.getX(), arg0.getY(), lastDimension);
					}
				} else { // We are creating a link and we want to highlight the target
					Display d = getDisplayAt(arg0.getX(), arg0.getY());
					if (highlightedDisplay != null && highlightedDisplay != d){
						highlightedDisplay.unHighlight();
					}
					highlightedDisplay = d;
					if (d!= null){
						d.highlightLinkEnd(selectedAnchor, arg0);
					}
				}
			}
			
			currentPosition.x = arg0.getX();
			currentPosition.y = arg0.getY();
			repaint();
		}
	}

	public void mouseMoved(MouseEvent arg0) {
	}
	
	public void paintComponent(Graphics g){
        update(g);
	}
/*
	public void resize(int width, int height) {
		Dimension newSize = null;
		if(width > preferedSize.width){
			if (height > preferedSize.height){ // width and height should be increased
				newSize = new Dimension(width, height);
			} else { // Increase only width
				newSize = new Dimension(width, preferedSize.height);
			}
		} else if (height > preferedSize.height){ // Increase only height
			newSize = new Dimension(preferedSize.width, height);
		}
		
		if(newSize != null){
			super.resize(newSize.width, newSize.height);
			setDisplaySize(newSize);
			//revalidate();
		}
		
	}
	*/
	/**
	 * Update the drawing area with all Displays we have.
	 */
	public void update(Graphics g){
		Graphics2D g2 = (Graphics2D)g;
		
		Dimension dim = getSize();
		if (! lastDimension.equals(dim)){
			int w = dim.width;
	        int h = dim.height;
	        lastDimension.width = w;
	        lastDimension.height = h;
			bi = (BufferedImage)createImage(w, h);
			big = bi.createGraphics();
			big.setRenderingHint(RenderingHints.KEY_ANTIALIASING,  RenderingHints.VALUE_ANTIALIAS_ON);
			
			//TODO check that no box is left outside visible area. Or create scrollable area
			//Dimension boxDim = main.getOutputBox().getDisplay().getDimension();
			//main.getOutputBox().getDisplay().setPosition(new Point(w-50-boxDim.width, h-50-boxDim.height));
		}
        
		// Draw frame and background
	    big.setColor(Color.white);
	    big.setBackground(Color.white);
		big.clearRect(0, 0, dim.width, dim.height);
		
		big.setColor(Color.black);
		big.drawRect(0, 0, dim.width-1, dim.height-1);
		
		// Draw Display things
		Iterator<Display> it = displayList.iterator();
		while (it.hasNext()) {
			it.next().drawOn(big, this);
		}
		
		// Draw the link being dragged by the user
		if (selectedAnchor != null){
			big.setColor(Color.red);
			//big.draw(new Rectangle(currentPosition, selectedAnchor.getPosition()));
			big.drawLine((int)currentPosition.getX(), (int)currentPosition.getY(), (int)selectedAnchor.getPosition().getX(), (int)selectedAnchor.getPosition().getY());
		}
		
		// Drawing the selecting area
		if (selectingArea){
			Rectangle selectionRect = new Rectangle(Math.min(currentPosition.x, clickPosition.x),
					Math.min(currentPosition.y, clickPosition.y),
					Math.abs(currentPosition.x - clickPosition.x),
					Math.abs(currentPosition.y - clickPosition.y)
					);
			big.setColor(new Color(152, 152, 190, 100));
			big.fill(selectionRect);
			big.setColor(new Color(114, 114, 143));
			big.draw(selectionRect);
		}
		
		// Copy all that to the area
		//Image biScaled = bi.getScaledInstance((new Float(getSize().width*zoom)).intValue(), (new Float(getSize().height*zoom)).intValue(), Image.SCALE_SMOOTH);
		g2.drawImage(bi, 0,0, this);
	}
	
	public void zoomIn(){
		zoom *= 1.25;
		if(zoom > 1f){
			zoom = 1f;
		}
		repaint();
	}
	
	public void zoomOut(){
		zoom *= 0.8;
		repaint();
	}
	
	/**
	 * Finds a display at the specified location
	 * @param x x of the location
	 * @param y y of the location
	 * @return The first Display found containing the point, or null if nothing found
	 */
	private Display getDisplayAt(int x, int y){
		Display result = null;
		// iterate the list at reverse
		// for some reason this is eaisier with an ArrayList than the doubly linked list LinkedList		
		for( int i = displayList.size()-1; i>=0; i--) {
			Display d = displayList.get(i);
			
			if (d.contains(x, y)){
				if (result == null || (result.getRefererType() == Display.RefererType.LINK && 
						d.getRefererType() != Display.RefererType.LINK )) {
					result = d;
				}
			}
				
		}
		return result;
	}
	
	/**
	 * For the Scrollable interface
	 */
	public Dimension getPreferredScrollableViewportSize() {
		return preferedSize;
	}

	/**
	 * For the Scrollable interface
	 */
	public int getScrollableUnitIncrement(Rectangle visibleRect, int orientation, int direction) {
		return 20;
	}

	/**
	 * For the Scrollable interface
	 */
	public int getScrollableBlockIncrement(Rectangle visibleRect, int orientation, int direction) {
		return 100;
	}

	/**
	 * For the Scrollable interface
	 */
	public boolean getScrollableTracksViewportWidth() {
		return false;
	}

	/**
	 * For the Scrollable interface
	 */
	public boolean getScrollableTracksViewportHeight() {
		return false;
	}


	
	/* No no no, a getSize method already exists
	public Dimension getSize(){
		return lastDimension;
	}
	*/
	
	public void setPreferredSize(Dimension preferredSize) {
		super.setPreferredSize(preferredSize);
	}
	
	public Dimension getPreferredSize() {
		return super.getPreferredSize();
	}
	
	
	public void setDisplaySize(Dimension d){
		preferedSize = d;
		setPreferredSize(d);
		setSize(d);
		repaint();
	}
	
	/* (non-Javadoc)
	 * @see refinery.display.DisplayAreaInterface#checkDisplaySize()
	 */
	public void checkDisplaySize() {
		double maxX = 0;
		double maxY = 0;
		Dimension newDimension = new Dimension(lastDimension);
		Iterator<Display> it = displayList.iterator();
		while (it.hasNext()) {
			Point p = it.next().getPosition();
			if (p == null) continue;
			if(maxX < p.getX()) maxX = p.getX();
			if(maxY < p.getY()) maxY = p.getY();
		}
		
		//if (maxX + 100 > lastDimension.getWidth()){
			newDimension.setSize(maxX + 150, newDimension.getHeight());
		//}
		
		//if (maxY + 100 >lastDimension.getHeight()){
			newDimension.setSize(newDimension.getWidth(), maxY + 150);
		//}
		
		if ( !lastDimension.equals(newDimension)){
			setDisplaySize(newDimension);
		}
	}

	/* (non-Javadoc)
	 * @see javax.swing.JComponent#getToolTipText(java.awt.event.MouseEvent)
	 */
	@Override
	public String getToolTipText(MouseEvent event) {
		final int WRAPLEN = 40;
		String tipText = "";
		Display d = getDisplayAt(event.getX(), event.getY());

		if (d != null){
			switch (d.getRefererType()){
			case COND:
				ConditionerWrapper cw = (ConditionerWrapper) d.getReferer();
				tipText += cw.getName() + "\n(" + cw.getLibName() + ")\n";
				for (int i = 0; i < cw.getDescription().length(); i+= WRAPLEN){
					tipText += cw.getDescription().substring(i, Math.min(cw.getDescription().length(), i+WRAPLEN)) + "\n";
				}
				break;
			case MACRO:
				PipelineMacro m = (PipelineMacro) d.getReferer();
				tipText += m.getName() + "\n(" + m.getLibName() +  ")\n";
				for (int i = 0; i < m.getDescription().length(); i+= WRAPLEN){
					tipText += m.getDescription().substring(i, Math.min(m.getDescription().length(), i+WRAPLEN)) + "\n";
				}
				break;
			case IO:
				IOBox io = (IOBox) d.getReferer();
				if (io.getType() == IOBox.IOBoxType.INPUTBOX){
					if (io.getFilename().equals("")){
						tipText += "input file not set yet";
					} else {
						tipText += io.getFilename();
					}
				} else {
					if (io.getFilename().equals("")){
						tipText += "output file not set yet";
					} else {
						tipText += io.getFilename();
					}
				}
			}
			
			return tipText;
		}
		return null;
		
	}

	private abstract class BoxActionListener implements ActionListener{
		protected Box currentBox;
		
		public BoxActionListener(){
			currentBox = null;
		}
		
		public void setCurrentBox(Box box){
			currentBox = box;
		}
	}
	
	private class DeleteBoxActionListner extends BoxActionListener{
		public void actionPerformed(ActionEvent e) {
			if (currentBox != null){
				
			}
			
		}
		
	}
}
