import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.util.Collections;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Set;
import java.util.stream.Collectors;

import org.apache.commons.collections15.Transformer;
import org.apache.commons.collections15.functors.MapTransformer;
import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.document.Document;
import org.apache.lucene.index.DirectoryReader;
import org.apache.lucene.index.IndexReader;
import org.apache.lucene.queryparser.classic.QueryParser;
import org.apache.lucene.search.IndexSearcher;
import org.apache.lucene.search.Query;
import org.apache.lucene.search.ScoreDoc;
import org.apache.lucene.search.TopDocs;
import org.apache.lucene.search.similarities.BM25Similarity;
import org.apache.lucene.store.FSDirectory;

import edu.uci.ics.jung.algorithms.scoring.PageRank;
import edu.uci.ics.jung.algorithms.scoring.PageRankWithPriors;
import edu.uci.ics.jung.graph.DirectedSparseGraph;
import edu.uci.ics.jung.graph.util.EdgeType;
import edu.uci.ics.jung.graph.util.Pair;

public class AuthorRankwithQuery {
	//my code here
	public static String authorIndex = "/Users/nawazkh/Masters/Courses/ILS_Z534_SEARCH/Assignments/Assignment 3/author_index/";
	public static String graphFileName = "/Users/nawazkh/Masters/Courses/ILS_Z534_SEARCH/Assignments/Assignment 3/author.net";
	public static Map<String, Double> authorMap;

	public static void calculatePriorProbabilities(String queryString) {
		try {
			IndexReader reader = DirectoryReader.open(FSDirectory.open(new File(authorIndex)));
			IndexSearcher searcher = new IndexSearcher(reader);
			Analyzer analyzer = new StandardAnalyzer();
			searcher.setSimilarity(new BM25Similarity());

			QueryParser parser = new QueryParser("content", analyzer);
			Query query = parser.parse(queryString);
			System.out.println("Searching for: " + query.toString("content"));
			TopDocs results = searcher.search(query, 300);
			//int matchedValue = results.totalHits;
			//System.out.println(matchedValue);

			ScoreDoc[] matches = results.scoreDocs;

			authorMap = new HashMap<String, Double>();
			double priorSum = 0;
			for(int i = 0; i < matches.length; i++) {
				//System.out.println("doc == "+matches[i].doc+" Score= "+matches[i].score);
				Document doc = searcher.doc(matches[i].doc);
				//System.out.println("Paper ID: " + doc.get("paperid"));
				//System.out.println("Author ID: " + doc.get("authorid"));
				priorSum += matches[i].score;
				if (authorMap.containsKey(doc.get("authorid"))) {
					// key exists
					Double id = authorMap.get(doc.get("authorid"));
					double total = matches[i].score + id.doubleValue();
					authorMap.put(doc.get("authorid"), new Double(total));
				} else {
					// key does not exists
					authorMap.put(doc.get("authorid"), new Double(matches[i].score));
				}
				//System.out.println("Author Name: "+doc.get("authorName"));
				//System.out.println("Body: "+doc.get("content"));
			}
			// Get a set of the entries
			Set<Map.Entry<String, Double>> set = authorMap.entrySet();
			// Get an iterator
			Iterator<Map.Entry<String, Double>> iterator = set.iterator();
			// Display elements
			while (iterator.hasNext()) {
				Map.Entry<String, Double> me = (Map.Entry<String, Double>) iterator.next();
				double value = me.getValue().doubleValue();
				value /= priorSum;
				authorMap.put(me.getKey(), value);
			}
			reader.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	public static void main(String[] args) {
		//String ourQuery = "Data Mining";
		String ourQuery = "Information Retrieval";
		calculatePriorProbabilities(ourQuery);
		FileReader fileReader = null;
		BufferedReader bufferedReader = null;
		DirectedSparseGraph<String, String> authorGraph = new DirectedSparseGraph<String, String>();
		System.out.println("Looking into:"+graphFileName);
		String line = "";
		int count = 0;
		try {
			fileReader = new FileReader(graphFileName);
			bufferedReader = new BufferedReader(fileReader);
			Map<String, String> verticesMap = new HashMap<String,String>();
			
			String lineOne = bufferedReader.readLine();
			String[] lineOneParts = lineOne.split("\\s+");
			//System.out.println(lineOneParts[0]+":::: These are the split parts of line one ::::"+lineOneParts[1]);
			int numberOfVertices = Integer.parseInt(lineOneParts[1].trim());
			for(int i = 0; i< numberOfVertices; i++) {
				String s = bufferedReader.readLine();
				//System.out.println(s);
				String[] parts = s.split("\\s+");
				verticesMap.put(parts[0], parts[1].substring(1, parts[1].length()-1));
				authorGraph.addVertex(parts[1].substring(1, parts[1].length()-1));
				if(authorMap.containsKey(parts[1].substring(1, parts[1].length()-1)) == false)
				{
					authorMap.put(parts[1].substring(1, parts[1].length()-1), new Double(0.0));
				}
			}
			
			Map<String, String> edgesMap = new HashMap<String, String>();

			String edgesCount = bufferedReader.readLine();
			String[] edgesCountSplit = edgesCount.split("\\s+");
			//System.out.println(edgesCountSplit[0]+":::: These are the split parts of edges count ::::"+edgesCountSplit[1]);
			int numberOfedges = Integer.parseInt(edgesCountSplit[1].trim());
			for(int i = 0; i< numberOfedges; i++) {
				String s = bufferedReader.readLine();
				//System.out.println(s);
				String[] parts = s.split("\\s+");
				//System.out.println(verticesMap.get(parts[0].trim())+"   "+verticesMap.get(parts[1].trim()));
				edgesMap.put(verticesMap.get(parts[0].trim()), verticesMap.get(parts[1].trim()));
				Pair<String> pair = new Pair<String>(verticesMap.get(parts[0].trim()), verticesMap.get(parts[1].trim()));
				authorGraph.addEdge(Integer.toString(i),pair,EdgeType.DIRECTED);
			}
			
			double alpha = 0.15;
//			PageRank<String, String> ranker = new PageRank<String, String>(authorGraph, alpha);
			Transformer<String, Double> authorMapTransformer = MapTransformer.getInstance(authorMap);
			PageRankWithPriors<String, String> ranker = new PageRankWithPriors<String, String>(authorGraph,authorMapTransformer, alpha);
			ranker.evaluate();
			
			Map<String, Double> output = new HashMap<String, Double>();
			for (String v : authorGraph.getVertices()) {
				output.put(v, ranker.getVertexScore(v));
			}
			Map<String, Double> sortedValues = new HashMap<String, Double>();
			sortedValues = sortByValue(output);
			Set<Map.Entry<String, Double>> set = sortedValues.entrySet();
			Iterator<Map.Entry<String, Double>> iterator = set.iterator();
			System.out.println("The top 10 ranked authors according to \""+ourQuery+"\", are: ");
			System.out.println("Author ID\t\tPage Rank Score");
			for (int j = 0; j < 10; j++) {
				Map.Entry<String, Double> top10 = (Map.Entry<String, Double>) iterator.next();
				//System.out.println(verticesMap.get(top10.getKey()).toString());
				System.out.println(top10.getKey()+ "\t\t\t" + top10.getValue());
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	//sortByValue referenced from https://stackoverflow.com/questions/109383/sort-a-mapkey-value-by-values-java
	public static <K, V extends Comparable<? super V>> Map<K, V> sortByValue(Map<K, V> map) {
		return map.entrySet().stream().sorted(Map.Entry.comparingByValue(Collections.reverseOrder()))
				.collect(Collectors.toMap(Map.Entry::getKey, Map.Entry::getValue, (e1, e2) -> e1, LinkedHashMap::new));
	}
}
