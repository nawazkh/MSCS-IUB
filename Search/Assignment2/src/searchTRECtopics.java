import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.nio.file.Paths;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.LinkedHashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.math.*;
import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.document.Field;
import org.apache.lucene.document.StringField;
import org.apache.lucene.index.DirectoryReader;
import org.apache.lucene.index.IndexReader;
import org.apache.lucene.index.LeafReaderContext;
import org.apache.lucene.index.MultiFields;
import org.apache.lucene.index.PostingsEnum;
import org.apache.lucene.index.Term;
import org.apache.lucene.queryparser.classic.ParseException;
import org.apache.lucene.queryparser.classic.QueryParser;
import org.apache.lucene.search.IndexSearcher;
import org.apache.lucene.search.Query;
import org.apache.lucene.search.similarities.ClassicSimilarity;
import org.apache.lucene.store.FSDirectory;
import org.apache.lucene.util.BytesRef;

public class searchTRECtopics {
	static int flag=0;
	public static void main(String args[]) throws ParseException, IOException
	{	
		String file = null;
		try {
			//read the file and saving into a string
			InputStream is = new FileInputStream("/Users/nawazhussaink/Masters/Courses/ILS_Z534_SEARCH/Assignments/Assign2/topics.51-100"); 
			BufferedReader buf = new BufferedReader(new InputStreamReader(is)); 
			String line = buf.readLine(); 
			StringBuilder sb = new StringBuilder();
			while(line != null)
			{ 
				sb.append(line);
				sb.append(" ");
				line = buf.readLine();
			}
			file = sb.toString();
		} 
		catch (IOException e) {
		}
		String[] array=file.split("<top>");
		queryParse(array,0);
		queryParse(array,1);
	}

	public static void queryParse(String[] array,int flag) throws IOException, ParseException
	{
		if(flag == 0)
		{
			File f = new File("/Users/nawazhussaink/Masters/Courses/ILS_Z534_SEARCH/Assignments/Assign2/MyAlgoShortQuery.txt");
			FileOutputStream fos = new FileOutputStream(f);
			PrintWriter printWriter = new PrintWriter(fos);
			for(int j=1;j<array.length;j++)
			{
				String id = null;
				int rank=1;
				String s=array[j];
				String name[]=null;
				Pattern pattern1 = Pattern.compile("<num>(.*?)<dom>");	
				Matcher matcher1 = pattern1.matcher(s);   
				if (matcher1.find()) 
				{
					id = matcher1.group(1);//taking only group 1
					name = id.split(":");
				}
				Pattern pattern = Pattern.compile("<title>(.*?)<desc>");	
				Matcher matcher = pattern.matcher(s);   
				if (matcher.find()) 
				{
					//extract title text
					String tagV = matcher.group(1);
					String name2[] = tagV.split(":"); 
					Map<String, Float> score=relevance_score(name2[1]);
					Object[] a = score.entrySet().toArray();
					Arrays.sort(a, new Comparator() {
						public int compare(Object o1, Object o2) {
							return ((Map.Entry<String, Float>) o2).getValue()
									.compareTo(((Map.Entry<String, Float>) o1).getValue());
						}
					});
					for (Object e : a) 
					{
						printWriter.write((name[1]+"\t"+"Q"+(j-1)+" \t"+ ((Map.Entry<String, Float>) e).getKey() + "\t"+rank +"\t"+ ((Map.Entry<String, Float >) e).getValue())+"\t"+"run-"+j+"\n");
						rank++;
						if(rank>1000) {
							break;
						}
					}
				}
			}
			printWriter.flush();
			fos.close();
			printWriter.close();
		}
		if(flag == 1)
		{
			File f = new File("/Users/nawazhussaink/Masters/Courses/ILS_Z534_SEARCH/Assignments/Assign2/MyAlgoLongQuery.txt");
			FileOutputStream fos = new FileOutputStream(f);
			PrintWriter printWriter = new PrintWriter(fos);
			for(int j=1;j<array.length;j++)
			{
				String id = null;
				int rank=1;
				String s=array[j];
				String name[]=null;
				Pattern pattern1 = Pattern.compile("<num>(.*?)<dom>");	
				Matcher matcher1 = pattern1.matcher(s);   
				if (matcher1.find()) 
				{
					id = matcher1.group(1);//taking only group 1
					name = id.split(":");
				}
				Pattern pattern = Pattern.compile("<desc>(.*?)<smry>");	
				Matcher matcher = pattern.matcher(s);   
				if (matcher.find()) 
				{
					//extract description text
					String tagV = matcher.group(1);
					String name2[] = tagV.split(":"); 
					Map<String, Float> score=relevance_score(name2[1]);
					Object[] a = score.entrySet().toArray();
					Arrays.sort(a, new Comparator() {
						public int compare(Object o1, Object o2) {
							return ((Map.Entry<String, Float>) o2).getValue()
									.compareTo(((Map.Entry<String, Float>) o1).getValue());
						}
					});
					for (Object e : a) 
					{
						printWriter.write((name[1]+"\t"+"Q"+(j-1)+"\t"+ ((Map.Entry<String, Float>) e).getKey() + "\t"+rank +"\t"+ ((Map.Entry<String, Float >) e).getValue())+"\t"+"run-"+j+"\n");
						rank++;
						if(rank>1000) {
							break;
						}
					}
				}
			}
			printWriter.flush();
			fos.close();
			printWriter.close();
		}
	}	

	//calculate relevance score of each query
	static  Map<String, Float> relevance_score(String tag_value) throws IOException, ParseException
	{
		HashMap<String,Float> docL=new HashMap<String,Float>();  
		HashMap<String,Float> relScore=new HashMap<String,Float>();  
		IndexReader reader = DirectoryReader.open(FSDirectory.open(Paths.get("/Users/nawazhussaink/Masters/Courses/ILS_Z534_SEARCH/Assignments/Assign2/index")));
		IndexSearcher searcher = new IndexSearcher(reader);
		Analyzer analyzer = new StandardAnalyzer();
		QueryParser parser = new QueryParser("TEXT", analyzer);
		Query query = parser.parse(QueryParser.escape(tag_value));
		Set<Term> queryTerms = new LinkedHashSet<Term>();
		searcher.createNormalizedWeight(query, false).extractTerms(queryTerms);
		int totalno_docs=reader.maxDoc();
		ClassicSimilarity dSimi = new ClassicSimilarity();
		List<LeafReaderContext> leafContexts = reader.getContext().reader().leaves();
		for (int i = 0; i < leafContexts.size(); i++)
		{
			LeafReaderContext leafContext = leafContexts.get(i);
			int startDocNo = leafContext.docBase;
			int numberOfDoc = leafContext.reader().maxDoc();
			for (int docId = 0; docId < numberOfDoc; docId++) 
			{
				float normDocLeng = dSimi.decodeNormValue(leafContext.reader().getNormValues("TEXT").get(docId));
				float docLeng = 1 / (normDocLeng * normDocLeng);
				docL.put(searcher.doc(docId +startDocNo).get("DOCNO"),docLeng);
			}
			for (Term t : queryTerms)
			{
				float sum=0;
				int df=reader.docFreq(new Term("TEXT", t.text()));
				if (df == 0) 
				{
					continue;
				}
				PostingsEnum de = MultiFields.getTermDocsEnum(leafContext.reader(),"TEXT", new BytesRef(t.text()));
				if (de != null) 
				{
					while ((de.nextDoc()) != PostingsEnum.NO_MORE_DOCS) 
					{
						String doc_id=searcher.doc(de.docID() +startDocNo).get("DOCNO");
						if(docL.keySet().contains(doc_id) )
						{
							//calculate tf idf
							sum=(float) ((de.freq())/(docL).get(doc_id)*Math.log(1+(float)(totalno_docs/df)));
						}
						if(relScore.keySet().contains(doc_id) )
						{    
							relScore.put(doc_id, relScore.get(doc_id)+sum);
						}
						else 
						{
							relScore.put(doc_id, sum);
						}

					}
				}
			}
		}		
		return relScore;
	}
}			
