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
package refinery.display;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.RenderingHints;
import java.awt.event.MouseEvent;
import java.awt.geom.AffineTransform;
import java.awt.geom.Ellipse2D;
import java.awt.image.BufferedImage;
import java.awt.image.ImageObserver;
import java.util.Vector;

import javax.swing.ImageIcon;
import javax.swing.JFrame;

import refinery.document.Anchor;
import refinery.document.ConditionerWrapper;
import refinery.document.Anchor.AnchorType;
import refinery.history.ActionHistory;


/**
 * The <code>ConditionerDisplay</code> class is the class implementing Display for Conditioners
 * @author Philippe David
 *
 */
public class ConditionerDisplay extends Display{
	private ConditionerWrapper cw;
	private BufferedImage sprite;
	private int x;
	private int y;
	private int height;
	private int width;
	private boolean selected;
	private int xClicked;		// Position of the mouse relatively to the top left corner of the display when selected
	private int yClicked;
	private Rectangle rectangle;
	private Vector<Ellipse2D.Double> inputAnchorDisplays;
	private Vector<Ellipse2D.Double> outputAnchorDisplays;
	private Anchor highlightedAnchor;
	
	private final int SPACE_BEETWEEN_ANCHORS = 20; 
//	private final int GRID_WIDTH = 30;
//	private final int GRID_HEIGHT = 30;
	
	private final int ANCHORWIDTH = 11;
	private final int ANCHORHEIGHT = 11;
	
	public ConditionerDisplay(ConditionerWrapper _cw, ActionHistory h){
		super(Display.RefererType.COND, h);
		cw = _cw;
		selected = false;
		highlightedAnchor = null;
		
		// default values
		height = 80;
		width = 130;
		
		x = 10;
		y = 60;
		
		// Init the sprite
		sprite = new BufferedImage(width+20, height+20, BufferedImage.TYPE_INT_ARGB);
		
		rectangle = new Rectangle(10,10,width,height-20);
		inputAnchorDisplays = new Vector<Ellipse2D.Double>();
		outputAnchorDisplays = new Vector<Ellipse2D.Double>();//new Ellipse2D.Double(30-ANCHORWIDTH/2, height-10-ANCHORHEIGHT/2, ANCHORWIDTH,ANCHORHEIGHT); 
		repaint();
	}
	
	public ConditionerDisplay(ConditionerWrapper _cw, ActionHistory h, int _x , int _y){
		this(_cw, h);
		x = _x;
		y = _y;
	}
	
	public Object getReferer(){
		return cw;
	}
	
	public BufferedImage getSprite() {
		return sprite;
	}

	public Dimension getSize() {
		return new Dimension(width,height);
	}
	
	public int getX(){
		return x;
	}
	
	public int getY(){
		return y;
	}	
	
	public void setPosition(Point newPos){
		int oldX = x;
		int oldY = y;
		//TODO check we are still inside the box maybe
		x = newPos.x;
		y = newPos.y;
		history.addDisplayMove(this, oldX, oldY, x, y);
	}

	public Point getPosition() {
		return new Point(x, y);
	}
	
	/**
	 * Returns true if the point (mx, my) is inside the display, false otherwise
	 */
	public boolean contains(int mx, int my){
		if(rectangle.contains(mx-x, my-y) ){
			return true;
		}
		for(int i = 0; i < inputAnchorDisplays.size(); i++){
			if(inputAnchorDisplays.get(i).contains(mx-x, my-y)){
				return true;
			}
		}
		for(int i = 0; i < outputAnchorDisplays.size(); i++){
			if(outputAnchorDisplays.get(i).contains(mx-x, my-y)){
				return true;
			}
		}
		
		return false;
	}
	
	/**
	 * Returns true if the rectangle rect intersects with this diplay, false otherwise
	 */
	public boolean intersects(Rectangle rect){
		Rectangle rectIntersect = (Rectangle) rect.clone();
		rectIntersect.translate(-x, -y);
		return rectangle.intersects(rectIntersect);
	}
	
	public void setSelected(boolean _selected){
		if (selected != _selected){
			selected = _selected;
			repaint();
		}
	}
	
	public void select(){
		if (selected == false){
			selected = true;
			repaint();
		}
	}
	
	/**
	 * Same as select but remember the point of selection if we start moving
	 */
	public void select(int _x, int _y){
		select();
		
		xClicked = _x - x;
		yClicked = _y - y;
	}
	
	public void deSelect(){
		selected = false;
		
		repaint();
	}
		
	/**
	 * Move with the constraint to remain in the box
	 */
	public void mouseMove(int _x, int _y, Dimension box){
		int oldX = x;
		int oldY = y;
		
		int newX = _x - xClicked;
		int newY = _y - yClicked;
		
		// Move on the grid only
		//newX -= newX%GRID_WIDTH;
		//newY -= newY%GRID_HEIGHT;
		
		if ( newX >= 0 && newX + width <= box.width){
			x = newX;
		}
		
		if ( newY >= 0 && newY + height <= box.height){
			y = newY;
		}
		
		history.addDisplayMove(this, oldX, oldY, newX, newY);
	}
	
	public void repaint(){

		sprite = new BufferedImage(width+20, height+20, BufferedImage.TYPE_INT_ARGB);
		Graphics2D gg = sprite.createGraphics();
		gg.setRenderingHint(RenderingHints.KEY_ANTIALIASING,  RenderingHints.VALUE_ANTIALIAS_ON);
		
		gg.setColor(new Color(0,0,0,0));
		gg.fill(new Rectangle(0,0,width+20,height+20));
		
		gg.setColor(new Color(149, 191, 223, 150));
		gg.fill(rectangle);
		if (selected) {
			gg.setColor(Color.red);
		} else {
			gg.setColor(Color.black);
		}
		gg.draw(rectangle);
		
		
		gg.setColor(Color.black);
		gg.drawString(cw.getName(), 50, 35);
		gg.drawString(cw.getLibName().substring(cw.getLibName().lastIndexOf('\\') +1), 50, 55);
		
		int localVectorSize = inputAnchorDisplays.size();
		int cwInputSize = cw.getInputNumber();
		/*if( localVectorSize < cwInputSize){
			for(int i=localVectorSize; i < cwInputSize; i++){
				int xPosition = getAnchorMiddleX(i);
				inputAnchorDisplays.add(new Ellipse2D.Double(xPosition-ANCHORWIDTH/2, 10-ANCHORHEIGHT/2, ANCHORWIDTH,ANCHORHEIGHT));
			}
		} else if (localVectorSize > cwInputSize){
			for(int i = cwInputSize; i < localVectorSize; i++){
				inputAnchorDisplays.remove(cwInputSize);
			}
		}*/
		inputAnchorDisplays.clear();
		for ( int i = 0; i < cwInputSize; i++ )
		{
			int xPosition = getAnchorMiddleX(i, cwInputSize);
			inputAnchorDisplays.add(new Ellipse2D.Double(xPosition-ANCHORWIDTH/2, 10-ANCHORHEIGHT/2, ANCHORWIDTH,ANCHORHEIGHT));
		//}
		// top anchors
		//for(int i= 0; i < cw.getInputNumber(); i++){
			
			if (cw.getInputAnchor(i) == highlightedAnchor){
				gg.setColor(Color.RED);
			}
			gg.fill(inputAnchorDisplays.get(i));
			gg.setColor(Color.black);
		}

		//localVectorSize = outputAnchorDisplays.size();
		int cwOutputSize = cw.getOutputNumber();
		/*if( localVectorSize < cwOutputSize){
			for(int i=localVectorSize; i < cwOutputSize; i++){
				int xPosition = getAnchorMiddleX(i,cwOutputSize);
				outputAnchorDisplays.add(new Ellipse2D.Double(xPosition-ANCHORWIDTH/2, height-10-ANCHORHEIGHT/2, ANCHORWIDTH,ANCHORHEIGHT));
			}
		} else if (localVectorSize > cwOutputSize){
			for(int i = cwOutputSize; i < localVectorSize; i++){
				outputAnchorDisplays.remove(cwOutputSize);
			}
		}
		
		for(int i= 0; i < cw.getOutputNumber(); i++){*/
		for ( int i = 0; i < cwOutputSize; i++ )
		{
			int xPosition = getAnchorMiddleX(i, cwOutputSize);
			outputAnchorDisplays.add(new Ellipse2D.Double(xPosition-ANCHORWIDTH/2, height-10-ANCHORHEIGHT/2, ANCHORWIDTH,ANCHORHEIGHT));
			
			if (cw.getOutputAnchor(i) == highlightedAnchor){
				gg.setColor(Color.RED);
			}
			gg.fill(outputAnchorDisplays.get(i));
			gg.setColor(Color.black);
		}
		
		// Add a cool icon
		Image inImage = new ImageIcon("options.png").getImage();
		AffineTransform tx =  new AffineTransform();
		tx.translate(20, 30);
		gg.drawImage(inImage, tx, null);
	}
	
	
	/**
	 * Look for an Anchor at the point (mx, my)
	 * @return The Anchor found if there is one, null otherwise
	 */
	public Anchor getAnchorAt(int mx, int my){
		// top anchors
		for(int i= 0; i < inputAnchorDisplays.size(); i++){
			if (inputAnchorDisplays.get(i).contains(mx-x, my-y)){
				return cw.getInputAnchor(i);
			}
		}
		
		//bottom anchors
		for(int i= 0; i < outputAnchorDisplays.size(); i++){
			if (outputAnchorDisplays.get(i).contains(mx-x, my-y)){
				return cw.getOutputAnchor(i);
			}
		}
		
		return null;
	}
	
	public Point getAnchorPosition(Anchor a){
		if (a == null) return null;
		if(a.type == AnchorType.ANCHORIN){
			for(int i = 0; i < cw.getInputNumber(); i++){
				if(cw.getInputAnchor(i) == a) return new Point(x + getAnchorMiddleX(i,cw.getInputNumber()), y + 10);
			}
		} else {
			for(int i = 0; i < cw.getOutputNumber(); i++){
				if(cw.getOutputAnchor(i) == a) return new Point(x + getAnchorMiddleX(i,cw.getOutputNumber()), y + height - 10);
			} 
		}
		
		return null;
	}

	
	/**
	 * @return The X component of the center where the Anchor anchorNum is placed taking in account 
	 * the total number of Input Anchors.
	 */
	private int getAnchorMiddleX(int anchorNum, int numAnchors){
		if ( numAnchors <= 6 )
			return (30 + anchorNum*SPACE_BEETWEEN_ANCHORS);

		int initX = 30-2*(numAnchors-6);
		if ( initX < 10 )
			initX = 10;

		int sbetween;
		if ( numAnchors < 11 )
		{
			sbetween = (width)/numAnchors;
		}
		else
		{
			sbetween = (width+10)/numAnchors;
		}
		return initX + anchorNum*sbetween;
	}


	@Override
	public void drawOn(Graphics2D big, ImageObserver io) {
		big.drawImage(getSprite(), x, y, io);
	}
	
	public void displayPropertyWindows(JFrame parentWindow){
		ConditionerPropertyWindow cpw = new ConditionerPropertyWindow(parentWindow, cw, history);
	}

	@Override
	public void highlightLinkEnd(Anchor linkStart, MouseEvent event) {
		Anchor newHighlightedAnchor = null;
		Anchor a = getAnchorAt(event.getX(), event.getY());
		if (a != null && a.getType() != linkStart.getType() && a.getReferer() != linkStart.getReferer()) {
			newHighlightedAnchor = a;
		} else {
			AnchorType endType = linkStart.getType() == AnchorType.ANCHORIN ? AnchorType.ANCHOROUT : AnchorType.ANCHORIN;
			newHighlightedAnchor = cw.getLinkEndAnchor(endType);
		}
		if (newHighlightedAnchor != highlightedAnchor){
			highlightedAnchor = newHighlightedAnchor;
			repaint();
		}

		
	}

	@Override
	public void unHighlight() {
		if (highlightedAnchor != null){
			highlightedAnchor = null;
			repaint();
		}
	}


	
}
