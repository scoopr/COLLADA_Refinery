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
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.event.MouseEvent;
import java.awt.geom.CubicCurve2D;
import java.awt.geom.Point2D;
import java.awt.image.ImageObserver;

import javax.swing.JFrame;

import refinery.document.Anchor;
import refinery.document.Link;

/**
 * The <code>LinkDisplay</code> class is the class implementing Display for Links
 * 
 * @author Philippe David
 */
public class LinkDisplay extends Display {
	private Link link;
	private boolean selected;
//	private int xClicked;
//	private int yClicked;
	
	public LinkDisplay(Link _parent){
		//Links don't need history yet, so we just set history to null
		super(Display.RefererType.LINK, null);
		link = _parent;
		
		selected = false;
	}
	
	public Object getReferer(){
		return link;
	}
	
	@Override
	public Point getPosition() {
		return null;
	}
	
	public Dimension getSize(){
		return new Dimension(0,0);
	}

	@Override
	public void drawOn(Graphics2D big, ImageObserver io) {
		if (selected){
			big.setColor(Color.red);
		} else {
			big.setColor(Color.black);
		}
		
		Point2D.Double p1 = new Point2D.Double(link.second.getPosition().getX(), link.second.getPosition().getY());
		Point2D.Double cp1 = new Point2D.Double(p1.getX(), p1.getY() - 30f);
		Point2D.Double p2 = new Point2D.Double(link.first.getPosition().getX(), link.first.getPosition().getY());
		Point2D.Double cp2 = new Point2D.Double(p2.getX(), p2.getY() + 30f);
		CubicCurve2D curve = new CubicCurve2D.Double();
		curve.setCurve(p1, cp1, cp2, p2);
		big.draw(curve);
		//big.drawLine((int) link.first.getPosition().getX(), (int) link.first.getPosition().getY(),
		//		(int) link.second.getPosition().getX(), (int) link.second.getPosition().getY());

	}
	
	public void select(){
		if (selected == false){
			selected = true;
			repaint();
		}
	}
	
	public void select(int _x, int _y){
		select();
		
//		xClicked = _x;
//		yClicked = _y;
	}

	public void deSelect() {
		selected = false;
		repaint();
	}
	
	// Maybe useful
	private void repaint(){
		
	}

	// Distance from (x,y) to the line plus some tricks because this is not an infinite line
	public boolean contains(int x, int y) {
		double d, x3, y3;
		final double WIDTH = 10.0; // maximum distance at which we consider a point is touching the line
		
		double x1 = x - link.first.getPosition().getX();
		double y1 = y - link.first.getPosition().getY();
		
		double x2 = link.second.getPosition().getX() - link.first.getPosition().getX(); 
		double y2 = link.second.getPosition().getY() - link.first.getPosition().getY();
		
		double dotProduct = x1*x2 + y1*y2;
		if (dotProduct < 0){ // The point is before the line, take the distance from (x, y) to the first point
			d = Math.sqrt(x1*x1+y1*y1);
		} else {
			double d2 = Math.sqrt(x2*x2+y2*y2); 
			double ratio = dotProduct/(d2*d2);
			
			if (ratio > 1.0){ // We are after the line, take the distance from (x, y) to the second point
				x3 = x - link.second.getPosition().getX();
				y3 = y - link.second.getPosition().getY();
				d = Math.sqrt(x3*x3+y3*y3);
			} else { // "Normal" case
				x3 = x1-ratio*x2;
				y3 = y1-ratio*y2;
				d = Math.sqrt(x3*x3+y3*y3);
			}
		}

		return d < WIDTH;
	}
	
	public boolean intersects(Rectangle rect){
		return rect.intersectsLine(link.first.getPosition().getX(), link.first.getPosition().getY(),
				 link.second.getPosition().getX(), link.second.getPosition().getY());
	}

	@Override
	public void mouseMove(int x, int y, Dimension box) {
		// Maybe we can move the boxes linked by this link when trying to move the link
		
	}
	
	public void setPosition(Point pos){
		// see comment in mouseMove
	}

	@Override
	public Anchor getAnchorAt(int mx, int my) {
		// We assume anchors are on the Conditioners and IObox instead of links
		return null;
	}
	
	public Point getAnchorPosition(Anchor a){
		//	We assume anchors are on the Conditioners and IObox instead of links
		return null;
	}
	

	public void displayPropertyWindows(JFrame parentWindow) {
		//Nothing to do
	}

	@Override
	public void highlightLinkEnd(Anchor linkStart, MouseEvent event) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void unHighlight() {
		// TODO Auto-generated method stub
		
	}
	

}
