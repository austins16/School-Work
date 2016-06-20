Files:
Trie.java - This is the class file for the Trie object. 
	Attributes: The trie object has two attributes,	the root Node and a List of 
	Query objects. Obviously the root node allows us a starting point for many
	of our functions. The purpose of the Query list is to make it more convenient
	to provide a list of suggested queries for the user. This will be expanded upon later.
	Methods: 
	createTrie(File): The constructor for Trie just create a null root node. This is the
		function that truely creates a trie. It accepts a file in the correct format,
		and reads it line by line. It parse each line into the term and its frequency,
		and calls the AddPattern(term, freq) method.
	addPattern(String, int): This method converts a term in string form to nodes within a 
		trie. First, it checks for duplicates within the SearchPattern method, exiting 
		if it finds a match. Then we traverse down the trie until we reach a character
		in the string not present in the trie. From there, we add children until we reach 
		the end of the string. In the last node, we set the frequency to the term freq.
	searchPattern(String, int): This method traverses the trie for each character in the 
		passed string. It returns false if the desired character cannot be found, it
		returns false.
	printTrie(Node, String): This method recursively prints the trie, with a DFS type 
		implementation. It appends the current node's character the buffer with each
		recursion.
	predictQuery(String): This method predicts the user's desired query based upon the input
		string. It displays the four most frequently searched children of the node led
		to by the given string. It finds the start node, then calls getQuery(start Node,
		String). It returns a List of Strings.
	getQuery(Node, String): This method finds children with nonzero frequency by recursively
		searching through child nodes (DFS) and calling the addQuery(Query) method. The
		Query object contains a String term and an int frequecy. 
	addQuery(Query): This method utilizes Query List attribute of the trie mentioned before.
		It adds the query to this list if the list size is less than four our the query to
		be added has a frequency greater than the minimum of the current list. 
Node.java - This is the simple class that populates a trie.
	Attributes: These attributes help with certain functions that utilize/refrence nodes. The 
		letter indicates the value of the node within the term. If the node is the end of 
		term, its frequency will be nonzero. The isEnd attribute marks that the node is a 
		leaf (though this is redundant). The node also has a List of Nodes which are its
		child
Query.java - this file contains the Query class, which is a simple object containing a String for 
		the search term and an integer for the term frequency. 
GUI.java - this file creates the GUI and contains the GUI event handlers
	initComponents(): This method initializes all of the objects in our GUI. It also calls the
		event handlers for the buttons and the search bar. 
	setMessage(String): This method takes a string input and passes the string to our output text
		area. 
	populateList(String[]): This method takes in a list of string. It passes the containing strings
		to the autocomplete list. This method is called by the predictSearch function.
	createTrie(File): This method creates a new trie of the filename provided. This method is called
		when the browse button has been pressed and a file has been selected.
	predictSearch(String): This method is called when a button has been pressed in the search box
		The method first calls the Trie class's function: predictQuery and then populates the list
		with its output.
	jButtonConnectActionPerformed(ActionEvent): This method is called when the connect button is
		pressed. It takes in the ip and hostname values and then opens a listening socket from them.
		The method then looks for an string from the server and passes the strings the strings
		to the setMessage function.
	jButtonDisconnectActionPerformed(ActionEvent): This method is called when the disconnect button
		is pressed. It can only be called after the connect button has been clicked. The method 
		only closes the socket and allows the user to reconnect to a different socket.
	main(string args[]): This method is called on program startup and makes the GUI on a thread.
Client.java- this class is creating a server socket that finds the missing terms from the bloom filter
	Client(Socket, GUI): This is a constructor for the Client class. It initializes some the socket
		and gui variables for the class for use in other functions
	run(): This method is called for reading in the bloom filter sent from the client, decompressing
		the filter, and finding the missing terms in the server's trie. Lastly, the method sends
		the missing terms over the socket to the client.
		missing terms based on that 
	disconnect(GUI): This method closes the socket, disconnecting the client from the server
Instructions:
1) Run the ec504_cache_sync.jar file twice, to create two instances of the program.
2) In one window, which I will call c1, under 'Host a server,' enter the port you would like the other 
	program instance (which I will call c2) to connect to. Click the 'Host' button.
3) In c2, under 'Connect to a server:' enter the IP of the server you would like to connect to, and
	in the 'Port' text box, enter the port you specified in c1. Click 'Connect.' 
4) Still in c2, indicate the port you would like to host on in the textbox under 'Host a server.' Click
	the 'Host' button. 
5) In c1, enter the port you just specified under 'Connect to a server:' in the 'Port' text box. Also 
	indicate the IP of the server you will connect to. Click 'Connect.'
6) Now we will select our text files. Under the 'Input Text' textbox in c1, click Browse, then find your 
	desired textfile. Do the same in c2, browsing for your other textfile.
7) Now we will sync our textfiles. In c1 or c2, click the 'Sync' button. This will sync the text files.
8) Staying in the same window, you can use the textbox next to the 'Search' button to enter your
	searches. The textlist below will display the suggested searches.
9) We will now save the new, synced trie. Click the 'Save' button in the bottom left. This will open a 
	Browser. Navigate to your desired folder, type the name of your new file, and click 'Save.' 