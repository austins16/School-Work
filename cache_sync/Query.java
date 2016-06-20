package Cache_Sync;

public class Query {
	public String query;
	public int frequency;
	
	public Query(){
		this.query = "";
		this.frequency = 0;
	}
	
	public Query(String query, int frequency){
		this.query = query;
		this.frequency = frequency;
	}
	
	//@Override
	public boolean equals(Query object)
	{
	    boolean isEqual= false;

	    if (object != null && object instanceof Query)
	    {
	        isEqual = ((this.query == ((Query) object).query)
	        		&& (this.frequency == ((Query) object).frequency));
	    }

	    return isEqual;
	}
}
