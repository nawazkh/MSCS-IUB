import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.stream.Collectors;
import java.io.IOException;

import org.apache.lucene.document.Document;
import org.apache.lucene.index.DirectoryReader;
import org.apache.lucene.index.IndexReader;
import org.apache.lucene.index.MultiFields;
import org.apache.lucene.index.Term;
import org.apache.lucene.index.Terms;
import org.apache.lucene.index.TermsEnum;
import org.apache.lucene.store.FSDirectory;
import org.apache.lucene.util.BytesRef;

import edu.uci.ics.jung.algorithms.scoring.PageRank;
import edu.uci.ics.jung.graph.DirectedSparseGraph;
import edu.uci.ics.jung.graph.util.EdgeType;
import edu.uci.ics.jung.graph.util.Pair;

public class AuthorRank {
	//mycode here

	public static void main(String[] args) {
		FileReader fileReader = null;
		BufferedReader bufferedReader = null;
		String graphFileName = "/Users/nawazkh/Masters/Courses/ILS_Z534_SEARCH/Assignments/Assignment 3/author.net";

		DirectedSparseGraph<String, String> authorGraph = new DirectedSparseGraph<String, String>();
		System.out.println("Looking into:"+graphFileName);
		String line = "";
		int count = 0;

		try {
			fileReader = new FileReader(graphFileName);
			bufferedReader = new BufferedReader(fileReader);
			Map<String, String> verticesMap = new HashMap<String,String>();

			//			while ((line = bufferedReader.readLine())!= null) {
			//				System.out.println(line);
			//				if(!(line.contains("Vertices")))
			//					count++;
			//			}
			//			System.out.println("Total count of the authors is:"+count);

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
			PageRank<String, String> ranker = new PageRank<String, String>(authorGraph, alpha);
			ranker.evaluate();

			Map<String, Double> output = new HashMap<String, Double>();
			for (String v : authorGraph.getVertices()) {
				output.put(v, ranker.getVertexScore(v));
			}
			
			Map<String, Double> sortedValues = new HashMap<String, Double>();
			sortedValues = sortByValue(output);
			Set<Map.Entry<String, Double>> set = sortedValues.entrySet();
			Iterator<Map.Entry<String, Double>> iterator = set.iterator();
			System.out.println("The top 10 ranked authors are:");
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
