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
import java.awt.image.BufferedImage;
import java.util.ArrayList;
import java.util.Iterator;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.Scrollable;

import refinery.Refinery;
import refinery.document.Anchor;
import refinery.document.Box;
import refinery.document.IOBox;
import refinery.document.Link;
import refinery.document.Pipeline;

public class DisplayAreaNonEditable extends JPanel implements Scrollable, DisplayAreaInterface{
	private static final long serialVersionUID = -6714944767184767903L;
	Pipeline document;					// Keep a reference to the the document this represents
	Refinery refinery;
	MainGUI gui;
	JFrame parentWindow;				// Our parent frame
	BufferedImage bi;					// The buffer we draw on (because we do doublebuffering)
	Graphics2D big;						// The buffer's Graphic2D
	
	Dimension lastDimension;			// Last known dimension of the drawing area
	Dimension preferedSize;
	ArrayList<Display> displayList;		// List of Displays. They are displayed in the order they are in this list
	
	public DisplayAreaNonEditable(Pipeline _document, JFrame _parentWindow, Refinery _refinery, MainGUI _gui) {
		document = _document;
		refinery = _refinery;
		gui = _gui;
		parentWindow = _parentWindow;
		
		// Init things
		displayList = new ArrayList<Display>();	
		
		
		lastDimension = new Dimension(0,0);
		this.setMinimumSize(new Dimension(500,500));
		preferedSize = new Dimension(700,700);
		this.setPreferredSize(preferedSize);
		
		document.setDisplayArea(this);
		document.setAutoArrange(true);
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

	public void addDisplay(Display d) {
		if(! displayList.contains((d))){
			displayList.add(d);
		}
		
	}

	public void removeAllDisplay() {
		displayList = new ArrayList<Display>();
	}

	public void removeDisplay(Display d) {
		displayList.remove(d);
	}

	public void putDisplayOnTop(Display d) {
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

	public void addDisplayAtRandomPosition(Display d) {
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

	public void setDocument(Pipeline _document) {
		removeAllDisplay();
		document = _document;
		document.setDisplayArea(this);
	}

	public void setDisplaySize(Dimension d) {
		preferedSize = d;
		setPreferredSize(d);
		setSize(d);
		repaint();
	}

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
		
		if (maxX + 200 > lastDimension.getWidth()){
			newDimension.setSize(maxX + 200, newDimension.getHeight());
		}
		
		if (maxY + 100 >lastDimension.getHeight()){
			newDimension.setSize(newDimension.getWidth(), maxY + 100);
		}
		
		if ( !lastDimension.equals(newDimension)){
			setDisplaySize(newDimension);
		}
	}
	
	public void paintComponent(Graphics g){
        update(g);
	}
	
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
		
		// Copy all that to the area
		g2.drawImage(bi, 0, 0, this);
	}
}
