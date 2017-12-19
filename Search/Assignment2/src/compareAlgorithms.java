import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.nio.file.Paths;
import java.util.Arrays;
import java.util.Comparator;
import java.util.HashMap;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.document.Document;
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
import org.apache.lucene.search.ScoreDoc;
import org.apache.lucene.search.TopDocs;
import org.apache.lucene.search.similarities.BM25Similarity;
import org.apache.lucene.search.similarities.ClassicSimilarity;
import org.apache.lucene.search.similarities.LMDirichletSimilarity;
import org.apache.lucene.search.similarities.LMJelinekMercerSimilarity;
import org.apache.lucene.search.similarities.Similarity;
import org.apache.lucene.store.FSDirectory;
import org.apache.lucene.util.BytesRef;

public class compareAlgorithms {
	static int flag=0;
	public static void main(String args[]) throws ParseException, IOException
	{	
		String fileAsString = null;
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
			fileAsString = sb.toString();
		} 
		catch (IOException e) {
		}
		String[] array=fileAsString.split("<top>");
		queryParseD(array,0,"DefaultShortQuery.txt");
		queryParseD(array,1,"DefaultLongQuery.txt");
		queryParseB(array,0,"BM25SimilarityShortQuery.txt");
		queryParseB(array,1,"BM25SimilarityLongQuery.txt");
		queryParseDD(array,0,"DirichletSimilarityShortQuery.txt");
		queryParseDD(array,1,"DirichletSimilarityLongQuery.txt");
		queryParseDD(array,0,"JelinekMercerSimilarityShortQuery.txt");
		queryParseDD(array,1,"JelinekMercerSimilarityLongQuery.txt");

	}

	public static void queryParseJ(String[] array,int flag,String fileName) throws IOException, ParseException
	{
		IndexReader reader = DirectoryReader.open(FSDirectory.open(Paths.get("/Users/nawazhussaink/Masters/Courses/ILS_Z534_SEARCH/Assignments/Assign2/index")));
		IndexSearcher searcher = new IndexSearcher(reader);
		//for short query of each model
		if(flag==0)
		{
			File f = new File("/Users/nawazhussaink/Masters/Courses/ILS_Z534_SEARCH/Assignments/Assign2/"+fileName);
			FileOutputStream fos = new FileOutputStream(f);
			PrintWriter printWriter = new PrintWriter(fos);
			//for each <top> in file
			for(int j=1;j<array.length;j++)
			{
				String id = null;
				int rank=1;
				String s=array[j];
				String name1[]=null;
				Pattern pattern1 = Pattern.compile("<num>(.*?)<dom>");	
				Matcher matcher1 = pattern1.matcher(s);   
				if (matcher1.find()) 
				{
					id = matcher1.group(1);
					name1 = id.split(":");//taking only group 1
				}
				//take title of each top from the topics.51-100 as short query
				Pattern pattern = Pattern.compile("<title>(.*?)<desc>");		
				Matcher matcher = pattern.matcher(s);   
				if (matcher.find()) 
				{		
					//extract title text
					String tagV = matcher.group(1);//taking only group 1
					String name2[] = tagV.split(":"); 
					//send to calculate relevance score
					// Get the preprocessed query terms
					//use models to calculate relevance score.
					searcher.setSimilarity(new LMJelinekMercerSimilarity((float)0.7));//  , JelinekMercerSimilarity((float)0.7).
					Analyzer analyzer = new StandardAnalyzer();
					QueryParser parser = new QueryParser("TEXT", analyzer);
					Query query = parser.parse(QueryParser.escape(name2[1]));
					TopDocs topDocs = searcher.search(query, 1000);
					//get relevance score
					ScoreDoc[] docs = topDocs.scoreDocs;
					for (int i = 0; i < docs.length; i++) 
					{
						Document doc = searcher.doc(docs[i].doc);
						printWriter.write((name1[1]+"\tQ"+(j-1)+"\t"+ doc.get("DOCNO") + "\t"+rank +"\t"+ docs[i].score+"\t"+"run-"+j+"\n"));
						rank++;
					}
				}
			}
			printWriter.flush();
			fos.close();
			printWriter.close();
		}
		//for long query of each model
		else 
		{
			File f = new File("/Users/nawazhussaink/Masters/Courses/ILS_Z534_SEARCH/Assignments/Assign2/"+fileName);
			FileOutputStream fos = new FileOutputStream(f);
			PrintWriter printWriter = new PrintWriter(fos);
			for(int j=1;j<array.length;j++)
			{
				String id = null;
				int rank=1;
				String s=array[j];
				String name1[]=null;
				Pattern pattern1 = Pattern.compile("<num>(.*?)<dom>");	
				Matcher matcher1 = pattern1.matcher(s);   
				if (matcher1.find()) 
				{
					id = matcher1.group(1);//taking only group 1
					name1 = id.split(":");
				}
				//take description of each top from the topics.51-100 as long query
				Pattern pattern = Pattern.compile("<desc>(.*?)<smry>");	
				Matcher matcher = pattern.matcher(s);   
				if (matcher.find()) 
				{
					String tagV = matcher.group(1);//taking only group 1
					String name2[] = tagV.split(":");
					// Get the preprocessed query terms
					//use models to calculate relevance score.
					searcher.setSimilarity(new LMJelinekMercerSimilarity((float)0.7));
					Analyzer analyzer = new StandardAnalyzer();
					QueryParser parser = new QueryParser("TEXT", analyzer);
					Query query = parser.parse(QueryParser.escape(name2[1]));
					//get relevance score
					TopDocs topDocs = searcher.search(query, 1000);
					ScoreDoc[] docs = topDocs.scoreDocs;
					for (int i = 0; i < docs.length; i++)
					{
						Document doc = searcher.doc(docs[i].doc);
						printWriter.write((name1[1]+"\tQ"+(j-1)+"\t"+ doc.get("DOCNO") + "\t"+rank +"\t"+ docs[i].score+"\t"+"run-"+j+"\n"));
						rank++;
					}				
				}
			}
			printWriter.flush();
			fos.close();
			printWriter.close();
		}
	}

	public static void queryParseDD(String[] array,int flag,String fileName) throws IOException, ParseException
	{
		IndexReader reader = DirectoryReader.open(FSDirectory.open(Paths.get("/Users/nawazhussaink/Masters/Courses/ILS_Z534_SEARCH/Assignments/Assign2/index")));
		IndexSearcher searcher = new IndexSearcher(reader);
		//for short query of each model
		if(flag==0)
		{
			File f = new File("/Users/nawazhussaink/Masters/Courses/ILS_Z534_SEARCH/Assignments/Assign2/"+fileName);
			FileOutputStream fos = new FileOutputStream(f);
			PrintWriter printWriter = new PrintWriter(fos);
			//for each <top> in file
			for(int j=1;j<array.length;j++)
			{
				String id = null;
				int rank=1;
				String s=array[j];
				String name1[]=null;
				Pattern pattern1 = Pattern.compile("<num>(.*?)<dom>");	
				Matcher matcher1 = pattern1.matcher(s);   
				if (matcher1.find()) 
				{
					id = matcher1.group(1);
					name1 = id.split(":");//taking only group 1
				}
				//take title of each top from the topics.51-100 as short query
				Pattern pattern = Pattern.compile("<title>(.*?)<desc>");		
				Matcher matcher = pattern.matcher(s);   
				if (matcher.find()) 
				{		
					//extract title text
					String tagV = matcher.group(1);//taking only group 1
					String name2[] = tagV.split(":"); 
					//send to calculate relevance score
					// Get the preprocessed query terms
					//use models to calculate relevance score.
					searcher.setSimilarity(new LMDirichletSimilarity());//  , JelinekMercerSimilarity((float)0.7).
					Analyzer analyzer = new StandardAnalyzer();
					QueryParser parser = new QueryParser("TEXT", analyzer);
					Query query = parser.parse(QueryParser.escape(name2[1]));
					TopDocs topDocs = searcher.search(query, 1000);
					//get relevance score
					ScoreDoc[] docs = topDocs.scoreDocs;
					for (int i = 0; i < docs.length; i++) 
					{
						Document doc = searcher.doc(docs[i].doc);
						printWriter.write((name1[1]+"\tQ"+(j-1)+"\t"+ doc.get("DOCNO") + "\t"+rank +"\t"+ docs[i].score+"\t"+"run-"+j+"\n"));
						rank++;
					}
				}
			}
			printWriter.flush();
			fos.close();
			printWriter.close();
		}
		//for long query of each model
		else 
		{
			File f = new File("/Users/nawazhussaink/Masters/Courses/ILS_Z534_SEARCH/Assignments/Assign2/"+fileName);
			FileOutputStream fos = new FileOutputStream(f);
			PrintWriter printWriter = new PrintWriter(fos);
			for(int j=1;j<array.length;j++)
			{
				String id = null;
				int rank=1;
				String s=array[j];
				String name1[]=null;
				Pattern pattern1 = Pattern.compile("<num>(.*?)<dom>");	
				Matcher matcher1 = pattern1.matcher(s);   
				if (matcher1.find()) 
				{
					id = matcher1.group(1);//taking only group 1
					name1 = id.split(":");
				}
				//take description of each top from the topics.51-100 as long query
				Pattern pattern = Pattern.compile("<desc>(.*?)<smry>");	
				Matcher matcher = pattern.matcher(s);   
				if (matcher.find()) 
				{
					String tagV = matcher.group(1);//taking only group 1
					String name2[] = tagV.split(":");
					// Get the preprocessed query terms
					//use models to calculate relevance score.
					searcher.setSimilarity(new LMDirichletSimilarity());// , JelinekMercerSimilarity((float)0.7).
					Analyzer analyzer = new StandardAnalyzer();
					QueryParser parser = new QueryParser("TEXT", analyzer);
					Query query = parser.parse(QueryParser.escape(name2[1]));
					//get relevance score
					TopDocs topDocs = searcher.search(query, 1000);
					ScoreDoc[] docs = topDocs.scoreDocs;
					for (int i = 0; i < docs.length; i++)
					{
						Document doc = searcher.doc(docs[i].doc);
						printWriter.write((name1[1]+"\tQ"+(j-1)+"\t"+ doc.get("DOCNO") + "\t"+rank +"\t"+ docs[i].score+"\t"+"run-"+j+"\n"));
						rank++;
					}				
				}
			}
			printWriter.flush();
			fos.close();
			printWriter.close();
		}
	}
	public static void queryParseB(String[] array,int flag,String fileName) throws IOException, ParseException
	{
		IndexReader reader = DirectoryReader.open(FSDirectory.open(Paths.get("/Users/nawazhussaink/Masters/Courses/ILS_Z534_SEARCH/Assignments/Assign2/index")));
		IndexSearcher searcher = new IndexSearcher(reader);
		//for short query of each model
		if(flag==0)
		{
			File f = new File("/Users/nawazhussaink/Masters/Courses/ILS_Z534_SEARCH/Assignments/Assign2/"+fileName);
			FileOutputStream fos = new FileOutputStream(f);
			PrintWriter printWriter = new PrintWriter(fos);
			//for each <top> in file
			for(int j=1;j<array.length;j++)
			{
				String id = null;
				int rank=1;
				String s=array[j];
				String name1[]=null;
				Pattern pattern1 = Pattern.compile("<num>(.*?)<dom>");	
				Matcher matcher1 = pattern1.matcher(s);   
				if (matcher1.find()) 
				{
					id = matcher1.group(1);
					name1 = id.split(":");//taking only group 1
				}
				//take title of each top from the topics.51-100 as short query
				Pattern pattern = Pattern.compile("<title>(.*?)<desc>");		
				Matcher matcher = pattern.matcher(s);   
				if (matcher.find()) 
				{		
					//extract title text
					String tagV = matcher.group(1);//taking only group 1
					String name2[] = tagV.split(":"); 
					//send to calculate relevance score
					// Get the preprocessed query terms
					//use models to calculate relevance score.
					searcher.setSimilarity(new BM25Similarity());// , DirichletSimilarity() , JelinekMercerSimilarity((float)0.7).
					Analyzer analyzer = new StandardAnalyzer();
					QueryParser parser = new QueryParser("TEXT", analyzer);
					Query query = parser.parse(QueryParser.escape(name2[1]));
					TopDocs topDocs = searcher.search(query, 1000);
					//get relevance score
					ScoreDoc[] docs = topDocs.scoreDocs;
					for (int i = 0; i < docs.length; i++) 
					{
						Document doc = searcher.doc(docs[i].doc);
						printWriter.write((name1[1]+"\tQ"+(j-1)+"\t"+ doc.get("DOCNO") + "\t"+rank +"\t"+ docs[i].score+"\t"+"run-"+j+"\n"));
						rank++;
					}
				}
			}
			printWriter.flush();
			fos.close();
			printWriter.close();
		}
		//for long query of each model
		else 
		{
			File f = new File("/Users/nawazhussaink/Masters/Courses/ILS_Z534_SEARCH/Assignments/Assign2/"+fileName);
			FileOutputStream fos = new FileOutputStream(f);
			PrintWriter printWriter = new PrintWriter(fos);
			for(int j=1;j<array.length;j++)
			{
				String id = null;
				int rank=1;
				String s=array[j];
				String name1[]=null;
				Pattern pattern1 = Pattern.compile("<num>(.*?)<dom>");	
				Matcher matcher1 = pattern1.matcher(s);   
				if (matcher1.find()) 
				{
					id = matcher1.group(1);//taking only group 1
					name1 = id.split(":");
				}
				//take description of each top from the topics.51-100 as long query
				Pattern pattern = Pattern.compile("<desc>(.*?)<smry>");	
				Matcher matcher = pattern.matcher(s);   
				if (matcher.find()) 
				{
					String tagV = matcher.group(1);//taking only group 1
					String name2[] = tagV.split(":");
					// Get the preprocessed query terms
					//use models to calculate relevance score.
					searcher.setSimilarity(new BM25Similarity());//DirichletSimilarity() , JelinekMercerSimilarity((float)0.7).
					Analyzer analyzer = new StandardAnalyzer();
					QueryParser parser = new QueryParser("TEXT", analyzer);
					Query query = parser.parse(QueryParser.escape(name2[1]));
					//get relevance score
					TopDocs topDocs = searcher.search(query, 1000);
					ScoreDoc[] docs = topDocs.scoreDocs;
					for (int i = 0; i < docs.length; i++)
					{
						Document doc = searcher.doc(docs[i].doc);
						printWriter.write((name1[1]+"\tQ"+(j-1)+"\t"+ doc.get("DOCNO") + "\t"+rank +"\t"+ docs[i].score+"\t"+"run-"+j+"\n"));
						rank++;
					}				
				}
			}
			printWriter.flush();
			fos.close();
			printWriter.close();
		}
	}

	public static void queryParseD(String[] array,int flag,String fileName) throws IOException, ParseException
	{
		IndexReader reader = DirectoryReader.open(FSDirectory.open(Paths.get("/Users/nawazhussaink/Masters/Courses/ILS_Z534_SEARCH/Assignments/Assign2/index")));
		IndexSearcher searcher = new IndexSearcher(reader);
		//for short query of each model
		if(flag==0)
		{
			File f = new File("/Users/nawazhussaink/Masters/Courses/ILS_Z534_SEARCH/Assignments/Assign2/"+fileName);
			FileOutputStream fos = new FileOutputStream(f);
			PrintWriter printWriter = new PrintWriter(fos);
			//for each <top> in file
			for(int j=1;j<array.length;j++)
			{
				String id = null;
				int rank=1;
				String s=array[j];
				String name1[]=null;
				Pattern pattern1 = Pattern.compile("<num>(.*?)<dom>");	
				Matcher matcher1 = pattern1.matcher(s);   
				if (matcher1.find()) 
				{
					id = matcher1.group(1);
					name1 = id.split(":");//taking only group 1
				}
				//take title of each top from the topics.51-100 as short query
				Pattern pattern = Pattern.compile("<title>(.*?)<desc>");		
				Matcher matcher = pattern.matcher(s);   
				if (matcher.find()) 
				{		
					//extract title text
					String tagV = matcher.group(1);//taking only group 1
					String name2[] = tagV.split(":"); 
					//send to calculate relevance score
					// Get the preprocessed query terms
					//use models to calculate relevance score.
					searcher.setSimilarity(new ClassicSimilarity());//BM25Similarity() , DirichletSimilarity() , JelinekMercerSimilarity((float)0.7).
					Analyzer analyzer = new StandardAnalyzer();
					QueryParser parser = new QueryParser("TEXT", analyzer);
					Query query = parser.parse(QueryParser.escape(name2[1]));
					TopDocs topDocs = searcher.search(query, 1000);
					//get relevance score
					ScoreDoc[] docs = topDocs.scoreDocs;
					for (int i = 0; i < docs.length; i++) 
					{
						Document doc = searcher.doc(docs[i].doc);
						printWriter.write((name1[1]+"\tQ"+(j-1)+"\t"+ doc.get("DOCNO") + "\t"+rank +"\t"+ docs[i].score+"\t"+"run-"+j+"\n"));
						rank++;
					}
				}
			}
			printWriter.flush();
			fos.close();
			printWriter.close();
		}
		//for long query of each model
		else 
		{
			File f = new File("/Users/nawazhussaink/Masters/Courses/ILS_Z534_SEARCH/Assignments/Assign2/"+fileName);
			FileOutputStream fos = new FileOutputStream(f);
			PrintWriter printWriter = new PrintWriter(fos);
			for(int j=1;j<array.length;j++)
			{
				String id = null;
				int rank=1;
				String s=array[j];
				String name1[]=null;
				Pattern pattern1 = Pattern.compile("<num>(.*?)<dom>");	
				Matcher matcher1 = pattern1.matcher(s);   
				if (matcher1.find()) 
				{
					id = matcher1.group(1);//taking only group 1
					name1 = id.split(":");
				}
				//take description of each top from the topics.51-100 as long query
				Pattern pattern = Pattern.compile("<desc>(.*?)<smry>");	
				Matcher matcher = pattern.matcher(s);   
				if (matcher.find()) 
				{
					String tagV = matcher.group(1);//taking only group 1
					String name2[] = tagV.split(":");
					// Get the preprocessed query terms
					//use models to calculate relevance score.
					searcher.setSimilarity(new ClassicSimilarity());//BM25Similarity() , DirichletSimilarity() , JelinekMercerSimilarity((float)0.7).
					Analyzer analyzer = new StandardAnalyzer();
					QueryParser parser = new QueryParser("TEXT", analyzer);
					Query query = parser.parse(QueryParser.escape(name2[1]));
					//get relevance score
					TopDocs topDocs = searcher.search(query, 1000);
					ScoreDoc[] docs = topDocs.scoreDocs;
					for (int i = 0; i < docs.length; i++)
					{
						Document doc = searcher.doc(docs[i].doc);
						printWriter.write((name1[1]+"\tQ"+(j-1)+"\t"+ doc.get("DOCNO") + "\t"+rank +"\t"+ docs[i].score+"\t"+"run-"+j+"\n"));
						rank++;
					}				
				}
			}
			printWriter.flush();
			fos.close();
			printWriter.close();
		}
	}
}