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
package refinery.document;

import java.awt.Point;


/**
 * The <code>AnchorConditioner</code> class is the version of Anchor specific to Conditioners.
 * @author Philippe David
 */
public class AnchorConditioner extends Anchor {
	protected ConditionerWrapper cw;
	// The Anchor position is the ConditionerWrapper
	private int number;
	
	public AnchorConditioner(ConditionerWrapper _cw, AnchorType _type, int _number){
		cw = _cw;
		type = _type;
		number = _number;
	}
	
	public RefererType getRefererType(){
		return RefererType.COND;
	}
	
	public ConditionerWrapper getReferer() {
		return cw;
	}
	
	public Point getPosition(){
		return cw.getDisplay().getAnchorPosition(this);
		/*
		int dx, dy;
		
		if (type == AnchorType.ANCHORIN){
			Point p = cw.getDisplay().getInputAnchorPosition(number);
			dx = p.x;
			dy = p.y;
		} else {
			//dy = (int) cw.getDisplay().getSize().getHeight() - 3;
			dy = (int) cw.getDisplay().getSize().getHeight();
			dx = (int) cw.getDisplay().getSize().getWidth()/2;
		}

		return new Point(cw.getDisplay().getX()+dx, cw.getDisplay().getY()+dy);
		*/
	}
	
	public int getNumber(){
		return number;
	}
	
	public String toString(){
		return cw.toString();
	}
}
