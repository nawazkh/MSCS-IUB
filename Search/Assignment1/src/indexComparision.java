import java.io.IOException;
import java.nio.file.Paths;
import java.util.ArrayList;

import org.apache.lucene.index.DirectoryReader;
import org.apache.lucene.index.IndexReader;
import org.apache.lucene.index.MultiFields;
import org.apache.lucene.index.Term;
import org.apache.lucene.index.Terms;
import org.apache.lucene.index.TermsEnum;
import org.apache.lucene.store.FSDirectory;
import org.apache.lucene.util.BytesRef;

public class indexComparision {
	public static void main(String[] args) {
		try {
			String indexPath = "/Users/nawazhussaink/Masters/Java_Workspace/InformationRetrieval/Indexes/";
			
			ArrayList<String> analysis = new ArrayList<String>();
			analysis.add("StandardAnalyzer");
			analysis.add("SimpleAnalyzer");
			analysis.add("KeywordAnalyzer");
			analysis.add("StopAnalyzer");
			for (String string : analysis) {
				IndexReader reader = DirectoryReader.open(FSDirectory.open(Paths.get(indexPath+string+"/")));
				System.out.println("Total number of documents in the corpus: "+reader.maxDoc());
				System.out.println("Number of documents containing the term \"new\" for field \"TEXT\": "+reader.docFreq(new Term("TEXT", "new")));
				System.out.println("Number of occurrences of \"new\" in the field \"TEXT\": "+reader.totalTermFreq(new Term("TEXT","new")));
				Terms vocabulary = MultiFields.getTerms(reader, "TEXT");
				System.out.println("Size of the vocabulary for this field: "+vocabulary.size());
				System.out.println("Number of documents that have at least one term for this field: "+vocabulary.getDocCount());
				System.out.println("Number of tokens for this field: "+vocabulary.getSumTotalTermFreq());
				System.out.println("Number of postings for this field: "+vocabulary.getSumDocFreq());
				TermsEnum iterator = vocabulary.iterator();
				BytesRef byteRef = null;
				System.out.println(" \n *******Vocabulary-Start**********");
				while((byteRef = iterator.next()) != null) {
					//String term = byteRef.utf8ToString();
					//System.out.print(term+"\n");
				}
				System.out.println("\n*******Vocabulary-End**********");        
				reader.close();
				System.out.println("--------"+string+"---------");
			}
			
		} catch (Exception e) {
			e.printStackTrace();
		}

	}
}

