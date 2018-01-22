import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.Digraph;
import edu.princeton.cs.algs4.AVLTreeST;
import edu.princeton.cs.algs4.Queue;
import java.lang.NullPointerException;
import java.lang.IllegalArgumentException;


public class WordNet {
  private Digraph hypernyms;
  private AVLTreeST<Integer, String> intToNoun;
  private AVLTreeST<String, Integer> nounToInt;

  // constructor takes the name of the two input files
  public WordNet(String synsets, String hypernyms) {
    if(synsets == null || hypernyms == null) throw new NullPointerException();

    String[] ln;
    int v = 0;

    In inHN = new In(hypernyms);

    while(inHN.hasNextLine()) {
      inHN.readLine();
      v++;
    }
    inHN.close();
    In newinHN = new In(hypernyms);
    this.hypernyms = new Digraph(v);
    while(newinHN.hasNextLine()) {
      ln = newinHN.readLine().split(",");
      for(int i = 1; i < ln.length; i ++) {
        this.hypernyms.addEdge(Integer.parseInt(ln[0]), Integer.parseInt(ln[i]));
      }
    }
    //System.out.println(this.hypernyms.V() + " " + this.hypernyms.E());
    newinHN.close();

    In inSS = new In(synsets);
    intToNoun = new AVLTreeST<Integer, String>();
    nounToInt = new AVLTreeST<String, Integer>();

    while(inSS.hasNextLine()) {
      ln = inSS.readLine().split(",");
      intToNoun.put(Integer.parseInt(ln[0]), ln[1]);
      nounToInt.put(ln[1], Integer.parseInt(ln[0]));
    }
    inSS.close();
  }

  // all WordNet nouns
  public Iterable<String> nouns() {
    return nounToInt.keys();
  }

  // is the word a WordNet noun?
  public boolean isNoun(String word) {
    if(word == null) throw new NullPointerException();

    return nounToInt.contains(word);
  }

  // a synset (second field of synsets.txt) that is a shortest common ancestor
  // of noun1 and noun2 (defined below)
  public String sca(String noun1, String noun2) {
    if(noun1 == null || noun2 == null) throw new NullPointerException();

    ShortestCommonAncestor shortestCommon = new ShortestCommonAncestor(this.hypernyms);
    int ancestor = shortestCommon.ancestor(nounToInt.get(noun1), nounToInt.get(noun2));
    return intToNoun.get(ancestor);
  }

  // distance between noun1 and noun2 (defined below)
  public int distance(String noun1, String noun2) {
    if(noun1 == null || noun2 == null) throw new NullPointerException();

    ShortestCommonAncestor shortestCommon = new ShortestCommonAncestor(this.hypernyms);
    //System.out.println(nounToInt.get(noun1) + " " + nounToInt.get(noun2));
    return shortestCommon.length(nounToInt.get(noun1), nounToInt.get(noun2));
  }

  // do unit testing of this class
  public static void main(String[] args) {
    WordNet wn = new WordNet("wordnet/synsets.txt", "wordnet/hypernyms.txt");

    while (!StdIn.isEmpty()) {
      String noun1 = StdIn.readString();
      String noun2 = StdIn.readString();
      int length = wn.distance(noun1, noun2);
      String ancestor = wn.sca(noun1, noun2);
      if(wn.isNoun(noun1) && wn.isNoun(noun2))
        StdOut.printf("length = %d, ancestor = %s\n", length, ancestor);
    }
  }
}
