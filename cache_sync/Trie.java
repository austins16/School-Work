package Cache_Sync;

import java.io.*;
import java.util.ArrayList;
import java.util.List;

public class Trie {
	Node root;
	int num_words;
	byte[] bfilter;
	List<Query> missing = new ArrayList<Query>();
	List<Query> queries = new ArrayList<Query>();

	public Trie(){
		root = new Node(' ', 0);
	}

	private void addQuery(Query q){
		Query minimum = new Query("",Integer.MAX_VALUE);
		if(!(queries.contains(q))){
			for(Query query : queries){
				if(query.frequency<minimum.frequency)
					minimum = query;
			}
			if(queries.size()<4)
				queries.add(q);
			else if(q.frequency>minimum.frequency){
				queries.remove(minimum);
				queries.add(q);
			}
		}
		return;
	}

	public boolean searchPattern(String pattern, int freq){
		Node start = root;
		for(char c : pattern.toCharArray()){
			if((start=start.isChild(c))!=null){

			}
			else{
				return false;
			}
		}
		if(start.isEnd){
			return true;
		} else{
			start.frequency = freq;
			num_words++;
			return true; // if the sequence is a part of a prefix of another pattern
		}
		//return false;
	}

	public int addPattern(String pattern, int freq){
		Node start = root;
		if(searchPattern(pattern, freq)){
			return 0;
		}
		else{
			// add new pattern to the trie
			for(char c : pattern.toCharArray()){
				Node temp = start;
				if(temp.isChild(c)!=null){
					start = temp.isChild(c);
				}
				else {
					start.children.add(new Node(c, 0));
					start = start.isChild(c);
				}
			}
			// add freq here and isEnd flag?
			start.isEnd = true;
			start.frequency = freq;
			num_words++;
			return 1;
		}
	}

	public void createTrie(File filename) throws IOException{
		BufferedReader br = new BufferedReader(new FileReader(filename));
		String[] parts;
		String line = null;
		String word = null;
		int freq = 0;

		while((line=br.readLine())!=null){
			parts = line.split("\t");
			word = parts[0];
			freq = Integer.parseInt(parts[1]);
			addPattern(word, freq);
		}
		br.close();
	}

	public void printTrie(Node node, String buffer){
		if(node.children.isEmpty()){
			System.out.printf("%s Freq: %d\n", buffer, node.frequency);
			return;
		}
		if(node.frequency!=0){
			System.out.printf("%s Freq: %d\n", buffer, node.frequency);
		}
		for(Node temp : node.children){
			printTrie(temp, buffer+temp.letter);
		}
		return;
	}
	
	public void outputTrie(Node node, FileWriter fw, String buffer) throws IOException{
		if(node.children.isEmpty()){
			String write = buffer+"\t"+node.frequency+"\n";
			fw.write(write.toString());
			return;
		}
		if(node.frequency!=0){
			String write = buffer+"\t"+node.frequency+"\n";
			fw.write(write.toString());
		}
		for(Node temp : node.children){
			outputTrie(temp, fw, buffer+temp.letter);
		}
		return;
	}

	public void getQuery(Node node, String buffer){
		if(node.children.isEmpty() || node.frequency!=0){
			this.addQuery(new Query(buffer, node.frequency));
		}
		for(Node temp : node.children){
			this.getQuery(temp, buffer+temp.letter);
		}
		return;
	}

	public List<String> predictQuery(String input){
		List<String> predictions = new ArrayList<String>(4);
		this.queries.clear();
		Node start = this.root;
		for(char c : input.toCharArray()){
			start=start.isChild(c);
		}
		if(start==null || input.isEmpty())
			return predictions;
		getQuery(start,input);
		for(Query query : queries){
			predictions.add(query.query);
		}
		return predictions;
	}
	
	public void hashTrie(Node node, String buffer){
		if(node.children.isEmpty()){
			Hash hash = new Hash();
			bfilter[hash.hashA(buffer, bfilter.length)] = 1;
			bfilter[hash.hashB(buffer, bfilter.length)] = 1;
			bfilter[hash.hashC(buffer, bfilter.length)] = 1;
			return;
		}
		if(node.frequency!=0){
			Hash hash = new Hash();
			bfilter[hash.hashA(buffer, bfilter.length)] = 1;
			bfilter[hash.hashB(buffer, bfilter.length)] = 1;
			bfilter[hash.hashC(buffer, bfilter.length)] = 1;
		}
		for(Node temp : node.children){
			hashTrie(temp, buffer+temp.letter);
		}
		return;
	}
	
	public void findMissing(Node node, String buffer, byte[] bfilter){
		if(node.children.isEmpty()){
			Hash hash = new Hash();
			if(bfilter[hash.hashA(buffer, bfilter.length)]!=1){
				missing.add(new Query(buffer, node.frequency));
			} else if(bfilter[hash.hashB(buffer, bfilter.length)]!=1){
				missing.add(new Query(buffer, node.frequency));
			} else if(bfilter[hash.hashC(buffer, bfilter.length)]!=1){
				missing.add(new Query(buffer, node.frequency));
			}
			return;
		}
		if(node.frequency!=0){
			Hash hash = new Hash();
			if(bfilter[hash.hashA(buffer, bfilter.length)]!=1){
				missing.add(new Query(buffer, node.frequency));
			} else if(bfilter[hash.hashB(buffer, bfilter.length)]!=1){
				missing.add(new Query(buffer, node.frequency));
			} else if(bfilter[hash.hashC(buffer, bfilter.length)]!=1){
				missing.add(new Query(buffer, node.frequency));
			}
		}
		for(Node temp : node.children){
			findMissing(temp, buffer+temp.letter, bfilter);
		}
		return;
	}
}
