package Cache_Sync;
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


/**
 *
 * @author austins
 */
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.DataInputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.ObjectOutputStream;
import java.io.OutputStreamWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * A Simple Socket client that connects to our socket server
 * @author Austin, Larry, Everett
 *
 */
public class Client extends Thread{

    private String hostname;
    private int port;
    private int hostport;
    private Socket socketClient;
    private ServerSocket serverSocket;
    private boolean running;
    private GUI gui;
    
    public Client(Socket clientSocket, GUI gui){
    	this.socketClient = clientSocket;
    	running = true;
    	this.gui = gui;
    }
    
    public void run(){
    	try{
    		//BufferedReader reader = new BufferedReader(new InputStreamReader(socketClient.getInputStream()));
    		BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(socketClient.getOutputStream()));
    		
    		writer.write("Hello client, nice to meet you!!!\n");
    		writer.flush();
    		//writer.close();
    		//writer.write("writing more weird stuff...\n");
    		//socketClient.close();
    	} catch(IOException ex){
    		
    	}
    	
    	while(running){
    		System.out.println("I'm in the run loop!!!\n");
			try {
				//BufferedReader reader = new BufferedReader(new InputStreamReader(socketClient.getInputStream()));
				DataInputStream reader = new DataInputStream(socketClient.getInputStream());
				
				int length = reader.readInt();
				byte[] filter = new byte[length];
				reader.readFully(filter, 0, filter.length);
				
				byte[] decomp_filter = new byte[filter.length*8];
				System.out.printf("Size of decomp_filter: %d\n", decomp_filter.length);
				
				String decode = "";
				
				System.out.println("Decoding the filter received...");
				byte b;
				byte masked;
				byte mask;
				//String temp;
				/*
				for(int ii=0; ii<filter.length; ii++){
					b = filter[ii];
					temp = String.format("%8s", Integer.toBinaryString(b & 0xFF)).replace(' ', '0');
					decode += temp;
				}
								
				for(int ii=0; ii<decomp_filter.length; ii++){
					decomp_filter[ii] = (byte)Character.getNumericValue(decode.charAt(ii));
				}
				*/
				for(int ii=0; ii<filter.length; ii++){
					b = (byte) (0xFF & filter[ii]);
					mask = (byte) 0x80;
					for(int jj=0; jj<8; jj++){
						masked = (byte) (b & mask);
						if(masked>0){
							decomp_filter[8*ii + jj] = 1;
						}
						else{
							decomp_filter[8*ii + jj] = 0;
						}
						mask = (byte)(mask >> 1);
					}
				}
				
				System.out.println("Calculating missings...");
				
				gui.trie.findMissing(gui.trie.root, "", filter);
				
				//System.out.println("Successfully received data packet!\n");
				System.out.println("Sending missing back to client...\n");
				BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(socketClient.getOutputStream()));
				
				for(Query q : gui.trie.missing){
					writer.write(q.query+"\t"+q.frequency+"\n");
				}
				System.out.println("Done sending missing back to client!!!\n");
				
				writer.flush();
				writer.close();
				
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			running = false;
    	}
    	
    }
    
    public void disconnect(GUI gui) throws UnknownHostException, IOException{
    	if(socketClient!= null)
    	{
    		gui.setMessage("Disconnecting from Server");
    		socketClient.close();
    	}
    }

    /*
    public void readResponse(GUI gui) throws IOException{
        String userInput;
        BufferedReader stdIn = new BufferedReader(new InputStreamReader(socketClient.getInputStream()));

        gui.setMessage("Response from server:");
        while ((userInput = stdIn.readLine()) != null) {
            gui.setMessage(userInput);
        }
    }
    
    
    public void sendBFilter(GUI gui) throws IOException{
    	BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(socketClient.getOutputStream()));
    	gui.setMessage("Sending bloom filter...");
    	bw.write("This is when the bloom filter will be sent back");
    	bw.flush();
    	bw.close();
    }
    */

}