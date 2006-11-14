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

import java.util.Iterator;
import java.util.Vector;

import refinery.XMLOutput;
import refinery.document.Anchor.AnchorType;

/**
 * The <code>LinksList</code> contains a list of Links and provide methods to search in
 * that list, add new links with several check before atcually doing it, and remove links.
*/
public class LinksList {
	private Vector<Link> links;
	
	public LinksList(){
		links = new Vector<Link>();
	}

	public Iterator<Link> iterator(){
		return links.iterator();
	}
	
	/**
	 * Returns an iterator on matching links. Does not return null if nothing found, but an empty iterator.
	 * @param anc The Anchor we want to find in the links list.
	 * @return An iterator on matching links
	 */
	public Iterator<Link> getLinksFromAnchor(Anchor anc){
		Vector<Link> matchingLinks = new Vector<Link>();
		Iterator<Link> coupleIt = iterator();
		if(anc != null){
			while(coupleIt.hasNext()){
				
				Link currentCouple = coupleIt.next();
				if (currentCouple.first.equals(anc) || currentCouple.second.equals(anc)){
					matchingLinks.add(currentCouple);
				}
			}
		}
		return matchingLinks.iterator();
	}
	
	/**
	 * @return true if the link was added (accepted), flase otherwise
	 */
	public boolean add(Link link){
		// Prevent incorrect links
		if (link.first.getType() == link.second.getType() || link.first.getReferer() == link.second.getReferer()) {
			return false;
		}
		
		Link reversedLink = new Link(link.second, link.first);
		// Prevent duplicate links
		if (!contains(link) && !contains(reversedLink)){
			// We check that the link order is (ANCHOROUT, ANCHORIN)
			if (link.first.getType() == AnchorType.ANCHOROUT){
				links.add(link);
			} else {
				// No no do not add the reversed link, the calling method keeps a reference on the first one
				// We must modify the link instead of adding its reversed copy.
				Anchor tmp = link.first;
				link.first = link.second;
				link.second = tmp;
				links.add(link);
			}
			
//			//Check if a Conditioner needs to add an anchor
//			if (link.second.getRefererType() == Anchor.RefererType.COND &&
//					((AnchorConditioner) link.second).getReferer().isVariableInputNumber()){
//				((AnchorConditioner) link.second).getReferer().checkAnchorNumber();
//			}
			
			return true;
		}
		return false;
		
	}
	
	public boolean remove(Link link){
		return links.remove(link);
	}
	
	public boolean contains(Link link){
		Iterator<Link> coupleIt = iterator();
		while(coupleIt.hasNext()){
			if (coupleIt.next().equals(link)){
				return true;
			}
		}
		return false;
		
	}
	
	public void toXML(XMLOutput output){
		//String result = "";
		Link currentLink;
		Iterator<Link> linksIT = iterator();
		
		while (linksIT.hasNext()){
			currentLink = linksIT.next();
			output.startEntity("link");
			output.addAttribute("start",  currentLink.first.getReferer().getIDName());
			output.addAttribute("startNb", String.valueOf(currentLink.first.getNumber()));
			output.addAttribute("end", currentLink.second.getReferer().getIDName());
			output.addAttribute("endNb", String.valueOf(currentLink.second.getNumber()));
			output.endEntity();
		}
	}
	
}
