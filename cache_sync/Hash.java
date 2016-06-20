package Cache_Sync;

public class Hash {
	public int hashA(String key, int max){
		int hashed = 0;
		
		for(int i=0; i<key.length(); i++){
			hashed = 37*hashed+key.charAt(i);
		}
		
		hashed %= max;
		
		if(hashed<0)
			hashed += max;
		
		return hashed;
	}
	
	public int hashB(String key, int max){
		int hashed = 0;
		
		for(int i=0; i<key.length(); i++){
			hashed += key.charAt(i);
		}
		
		return hashed % max;
	}
	
	public int hashC(String key, int max){
		int hashed = 0;
		
		for(int i=0; i<key.length(); i++){
			hashed = ((hashed<<5) + hashed)^(int)key.charAt(i);
		}
		
		return Math.abs(hashed % max);
	}
}
