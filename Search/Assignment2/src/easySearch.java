
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.nio.file.Paths;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Scanner;
import java.util.Set;
import java.math.*;
import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.analysis.standard.StandardAnalyzer;
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

public class easySearch {
	public static HashMap<String, Float> searchMyQuery(String query, String indexPath){
		HashMap<String,Float> docLength=new HashMap<String,Float>();  
		HashMap<String,Float> myScore=new HashMap<String,Float>(); 
		try {
			IndexReader reader = DirectoryReader.open(FSDirectory.open(Paths.get("/Users/nawazhussaink/Masters/Courses/ILS_Z534_SEARCH/Assignments/Assign2/index")));
			IndexSearcher searcher = new IndexSearcher(reader);
			Analyzer analyzer = new StandardAnalyzer();
			QueryParser parser = new QueryParser("TEXT", analyzer);
			Query query1 = parser.parse(query.trim());
			Set<Term> queryTerms = new LinkedHashSet<Term>();
			searcher.createNormalizedWeight(query1, false).extractTerms(queryTerms);
			int totalno_docs=reader.maxDoc();
			for (Term t : queryTerms)
			{
				float sum=0;
				int df=reader.docFreq(new Term("TEXT", t.text()));
				if (df == 0) 
				{
					continue;
				}
				
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
						docLength.put(searcher.doc(docId +startDocNo).get("DOCNO"),docLeng);
					}
					PostingsEnum de = MultiFields.getTermDocsEnum(leafContext.reader(),"TEXT", new BytesRef(t.text()));
					if (de != null) 
					{
						while ((de.nextDoc()) != PostingsEnum.NO_MORE_DOCS) 
						{
							String docID=searcher.doc(de.docID() +startDocNo).get("DOCNO");
							if(docLength.keySet().contains(docID) )
							{
								sum=(float) ((de.freq())/(docLength).get(docID)*Math.log(1+(float)(totalno_docs/df)));
							}
							if(myScore.keySet().contains(docID))
							{
								myScore.put(docID, myScore.get(docID)+sum);
							}
							else
							{
								myScore.put(docID, sum);
							}

						}
					}
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}

		return myScore;
	}
	public static void main(String args[]) throws ParseException, IOException
	{
		HashMap<String,Float> docLength=new HashMap<String,Float>();  
		HashMap<String,Float> myScore=new HashMap<String,Float>();  
		IndexReader reader = DirectoryReader.open(FSDirectory.open(Paths.get("/Users/nawazhussaink/Masters/Courses/ILS_Z534_SEARCH/Assignments/Assign2/index")));
		IndexSearcher searcher = new IndexSearcher(reader);
		Analyzer analyzer = new StandardAnalyzer();
		QueryParser parser = new QueryParser("TEXT", analyzer);
		Scanner ss = new Scanner(System.in);
		System.out.print("Enter query: ");
		String s = ss.nextLine(); 
		Query query = parser.parse(s);
		Set<Term> queryTerms = new LinkedHashSet<Term>();
		searcher.createNormalizedWeight(query, false).extractTerms(queryTerms);
		for (Term t : queryTerms)
		{
			float sum=0;
			File f = new File("/Users/nawazhussaink/Masters/Courses/ILS_Z534_SEARCH/Assignments/Assign2/myScore"+t+".txt");
			FileOutputStream fos = new FileOutputStream(f);
			PrintWriter out1 = new PrintWriter(fos);
			int df=reader.docFreq(new Term("TEXT", t.text()));
			if (df == 0) 
			{
				continue;
			}
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
					//length(doc)
					docLength.put(searcher.doc(docId +startDocNo).get("DOCNO"),docLeng);
				}
				System.out.println();
				PostingsEnum de = MultiFields.getTermDocsEnum(leafContext.reader(),"TEXT", new BytesRef(t.text()));
				if (de != null) 
				{
					while ((de.nextDoc()) != PostingsEnum.NO_MORE_DOCS) 
					{
						String docID=searcher.doc(de.docID() +startDocNo).get("DOCNO");
						if(docLength.keySet().contains(docID) )
						{
							sum=(float) ((de.freq())/(docLength).get(docID)*Math.log(1+(float)(totalno_docs/df)));
							out1.write("for term- "+t+"\tdoc id- "+docID+"\trelevance myScore is- " +sum+"\n");
						}
						if(myScore.keySet().contains(docID))
						{    
							myScore.put(docID, myScore.get(docID)+sum);
						}
						else
						{
							myScore.put(docID, sum);
						}

					}
				}
			}
			out1.flush();
			out1.close();
		}

		File f = new File("/Users/nawazhussaink/Masters/Courses/ILS_Z534_SEARCH/Assignments/Assign2/myScore.txt");
		FileOutputStream fos = new FileOutputStream(f);
		PrintWriter out = new PrintWriter(fos);
		for (String key:myScore.keySet())
		{
			out.write(key+"\t"+myScore.get(key)+"\n");
		}
		out.flush();
		out.close();
	}			
}