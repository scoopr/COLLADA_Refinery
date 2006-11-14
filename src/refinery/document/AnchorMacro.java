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


public class AnchorMacro extends Anchor {
	PipelineMacro macro;
	private int number;
	
	public AnchorMacro(PipelineMacro _macro, AnchorType _type, int num){
		macro = _macro;
		type = _type;
		number = num;
	}


	public Point getPosition(){
		return macro.getDisplay().getAnchorPosition(this);
	}

	@Override
	public Box getReferer(){
		return macro;
	}

	
	public RefererType getRefererType(){
		return RefererType.MACRO;
	}
	
	public int getNumber(){
		return number;
	}
	
	public String toString(){
		return macro.toString();
	}

}
