

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.commons.io.FileUtils;
import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.analysis.core.KeywordAnalyzer;
import org.apache.lucene.analysis.core.SimpleAnalyzer;
import org.apache.lucene.analysis.core.StopAnalyzer;
import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.document.Document;
import org.apache.lucene.document.StringField;
import org.apache.lucene.document.TextField;
import org.apache.lucene.index.IndexWriter;
import org.apache.lucene.index.IndexWriterConfig;
import org.apache.lucene.index.IndexWriterConfig.OpenMode;
import org.apache.lucene.store.Directory;
import org.apache.lucene.store.FSDirectory;
import org.apache.lucene.store.FSDirectory.*;
import org.apache.lucene.document.Field;
import org.apache.lucene.document.StringField;
import org.apache.lucene.document.TextField;

public class generateIndex {

	public static void main(String[] args) {
		String folderName = "/Users/nawazhussaink/Masters/Courses/ILS_Z534_SEARCH/corpus/";
		String indexPath = "/Users/nawazhussaink/Masters/Java_Workspace/InformationRetrieval/Indexes/";
		String fileName = "";
		String completeName = "";
		String line ="";
		String DOCNO = "";
		String HEAD = "";
		String BYLINE = "";
		String DATELINE = "";
		String TEXT = "";
		FileReader fileReader = null;
		BufferedReader bufferedReader = null;
		ArrayList<String> docToDoc = null;
		HashSet<HashMap<String,String>> documents = null;
		HashMap<String, String> document = null;
		IndexWriter writer = null;

		try {
			Pattern TAG_REGEX = Pattern.compile("<DOC>(.+?)</DOC>");
			Pattern TAG_DOCNO = Pattern.compile("<DOCNO>(.+?)</DOCNO>");
			Pattern TAG_HEAD = Pattern.compile("<HEAD>(.+?)</HEAD>");
			Pattern TAG_BYLINE = Pattern.compile("<BYLINE>(.+?)</BYLINE>");
			Pattern TAG_DATELINE = Pattern.compile("<DATELINE>(.+?)</DATELINE>");
			Pattern TAG_TEXT = Pattern.compile("<TEXT>(.+?)</TEXT>");
			Matcher matcher = null;
			documents = new HashSet<HashMap<String,String>>();
			Iterator fileIterator = FileUtils.iterateFiles(new File(folderName), null, false);
			while (fileIterator.hasNext()) {
				File inputFile = (File) fileIterator.next();
				fileName = inputFile.getName();
				completeName = folderName.trim() + fileName.trim();
				if(!fileName.equalsIgnoreCase(".DS_Store")) {
					System.out.println("Looking into: "+fileName);
					fileReader = new FileReader(completeName);
					bufferedReader = new BufferedReader(fileReader);
					StringBuilder docInLine = new StringBuilder();
					while((line = bufferedReader.readLine())!= null) {
						docInLine.append(line);
					}

					matcher = TAG_REGEX.matcher(docInLine.toString());
					docToDoc = new ArrayList<String>();

					while (matcher.find()) {
						//System.out.println("Working ?");
						//System.out.println(matcher.group(1));
						docToDoc.add(matcher.group(1));
						//System.out.println(matcher.group(1).trim());
					}
					//System.out.println("Doc appended");
					for (String string : docToDoc) {
						//System.out.println("whast the problem");
						document = new HashMap<String, String>();
						//System.out.println(string);
						document.put("DOCNO", readInternalTags(string,"DOCNO"));
						//System.out.println("Step 1");
						document.put("HEAD", readInternalTags(string,"HEAD"));
						//System.out.println("Step 2");
						document.put("BYLINE", readInternalTags(string,"BYLINE"));
						document.put("DATELINE", readInternalTags(string,"DATELINE"));
						document.put("TEXT", readInternalTags(string,"TEXT"));

						documents.add(document);
					}

				}
				else {
					continue;
				}
			}
			bufferedReader.close();
			//System.out.println("Are we here ?");
			Analyzer analyzer4 = new StandardAnalyzer();
			Directory dir4 = FSDirectory.open(Paths.get(indexPath+"StandardAnalyzer/"));//opens this path
			Analyzer analyzer2 = new SimpleAnalyzer();
			Directory dir2 = FSDirectory.open(Paths.get(indexPath+"SimpleAnalyzer/"));//opens this path
			Analyzer analyzer1 = new KeywordAnalyzer();
			Directory dir1= FSDirectory.open(Paths.get(indexPath+"KeywordAnalyzer"));//opens this path
			Analyzer analyzer3 = new StopAnalyzer();
			Directory dir3 = FSDirectory.open(Paths.get(indexPath+"StopAnalyzer"));//opens this path
			//--------
			IndexWriterConfig iwc4 = new IndexWriterConfig(analyzer4);
			//iwc.setOpenMode(OpenMode.CREATE_OR_APPEND);
			iwc4.setOpenMode(OpenMode.CREATE);
			//System.out.println("Are we here ?");
			writer = new IndexWriter(dir4, iwc4);
			for (HashMap<String, String> storedDocument : documents) {
				Document luceneDoc = new Document();
				luceneDoc.add(new StringField("DOCNO", storedDocument.get("DOCNO"),Field.Store.YES));
				luceneDoc.add(new TextField("HEAD", storedDocument.get("HEAD"),Field.Store.YES));
				luceneDoc.add(new TextField("BYLINE", storedDocument.get("BYLINE"),Field.Store.YES));
				luceneDoc.add(new TextField("DATELINE", storedDocument.get("DATELINE"),Field.Store.YES));
				luceneDoc.add(new TextField("TEXT", storedDocument.get("TEXT"),Field.Store.YES));
				writer.addDocument(luceneDoc);
			}
			writer.forceMerge(1);
			writer.commit();
			writer.close();
			System.out.println("--Indexed StandardAnalyzer--");
			
			IndexWriterConfig iwc3 = new IndexWriterConfig(analyzer3);
			//iwc.setOpenMode(OpenMode.CREATE_OR_APPEND);
			iwc3.setOpenMode(OpenMode.CREATE);
			//System.out.println("Are we here ?");
			writer = new IndexWriter(dir3, iwc3);
			for (HashMap<String, String> storedDocument : documents) {
				Document luceneDoc = new Document();
				luceneDoc.add(new StringField("DOCNO", storedDocument.get("DOCNO"),Field.Store.YES));
				luceneDoc.add(new TextField("HEAD", storedDocument.get("HEAD"),Field.Store.YES));
				luceneDoc.add(new TextField("BYLINE", storedDocument.get("BYLINE"),Field.Store.YES));
				luceneDoc.add(new TextField("DATELINE", storedDocument.get("DATELINE"),Field.Store.YES));
				luceneDoc.add(new TextField("TEXT", storedDocument.get("TEXT"),Field.Store.YES));
				writer.addDocument(luceneDoc);
			}
			writer.forceMerge(1);
			writer.commit();
			writer.close();
			System.out.println("--Indexed StopAnalyzer--");
			
			IndexWriterConfig iwc2 = new IndexWriterConfig(analyzer2);
			//iwc.setOpenMode(OpenMode.CREATE_OR_APPEND);
			iwc2.setOpenMode(OpenMode.CREATE);
			//System.out.println("Are we here ?");
			writer = new IndexWriter(dir2, iwc2);
			for (HashMap<String, String> storedDocument : documents) {
				Document luceneDoc = new Document();
				luceneDoc.add(new StringField("DOCNO", storedDocument.get("DOCNO"),Field.Store.YES));
				luceneDoc.add(new TextField("HEAD", storedDocument.get("HEAD"),Field.Store.YES));
				luceneDoc.add(new TextField("BYLINE", storedDocument.get("BYLINE"),Field.Store.YES));
				luceneDoc.add(new TextField("DATELINE", storedDocument.get("DATELINE"),Field.Store.YES));
				luceneDoc.add(new TextField("TEXT", storedDocument.get("TEXT"),Field.Store.YES));
				writer.addDocument(luceneDoc);
			}
			writer.forceMerge(1);
			writer.commit();
			writer.close();
			System.out.println("--Indexed SimpleAnalyzer--");
			IndexWriterConfig iwc1 = new IndexWriterConfig(analyzer1);
			//iwc.setOpenMode(OpenMode.CREATE_OR_APPEND);
			iwc1.setOpenMode(OpenMode.CREATE);
			//System.out.println("Are we here ?");
			writer = new IndexWriter(dir1, iwc1);
			for (HashMap<String, String> storedDocument : documents) {
				Document luceneDoc = new Document();
				luceneDoc.add(new StringField("DOCNO", storedDocument.get("DOCNO"),Field.Store.YES));
				luceneDoc.add(new TextField("HEAD", storedDocument.get("HEAD"),Field.Store.YES));
				luceneDoc.add(new TextField("BYLINE", storedDocument.get("BYLINE"),Field.Store.YES));
				luceneDoc.add(new TextField("DATELINE", storedDocument.get("DATELINE"),Field.Store.YES));
				luceneDoc.add(new TextField("TEXT", storedDocument.get("TEXT"),Field.Store.YES));
				writer.addDocument(luceneDoc);
			}
			writer.forceMerge(1);
			writer.commit();
			writer.close();
			System.out.println("--Indexed KeywordAnalyzer--");
		} catch (Exception e) {
			e.printStackTrace();
		}


	}
	private static String readInternalTags(String oneFile,String tag) {
		String tagToBeFound = "";
		Pattern TAG_REGEX = Pattern.compile("<"+tag+">(.+?)</"+tag+">");
		Matcher matcher = TAG_REGEX.matcher(oneFile);
		while (matcher.find()) {
			//System.out.println("Working ?");
			//System.out.println(matcher.group(1));
			//docToDocTemp.add(matcher.group(1).trim());
			tagToBeFound = tagToBeFound+(matcher.group(1).trim());
		}
		return tagToBeFound;
	}

}
