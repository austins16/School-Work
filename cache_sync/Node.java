package Cache_Sync;

import java.util.LinkedList;

public class Node {
	char letter;
	int frequency;
	boolean isEnd;
	LinkedList<Node> children;
	
	public Node(char letter, int frequency){
		this.children = new LinkedList<Node>();
		this.letter = letter;
		this.frequency = frequency;
		this.isEnd = false;
	}
	
	public Node isChild(char c){
		if(children!=null){
			for(Node current : children){
				if(current.letter == c)
					return current;
			}
		}
		return null;
	}
}
