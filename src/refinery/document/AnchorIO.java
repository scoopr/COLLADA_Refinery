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
 * The <code>AnchorIO</code> class is the version of Anchor specific to IOBox.
 * @author Philippe David
 */
public class AnchorIO extends Anchor{
	private IOBox io;
	
	public AnchorIO(IOBox _io, AnchorType _type){
		io = _io;
		type = _type;
	}
	
	public IOBox getReferer() {
		return io;
	}
	
	public Point getPosition(){
		return io.getDisplay().getAnchorPosition(this);
	}
	
	public RefererType getRefererType(){
		return RefererType.IO;
	}
	
	public int getNumber(){
		return 0;
	}
	
	public String toString(){
		return io.toString();
	}
}
