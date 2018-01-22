import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.Digraph;


public class TestClient {
  public static void main(String[] args) {
    In in = new In(args[0]);
    Digraph G = new Digraph(in);
    ShortestCommonAncestor sca = new ShortestCommonAncestor(G);
    while (!StdIn.isEmpty()) {
      int v = StdIn.readInt();
      int w = StdIn.readInt();
      int length   = sca.length(v, w);
      int ancestor = sca.ancestor(v, w);
      StdOut.printf("length = %d, ancestor = %d\n", length, ancestor);
    }
  }
}
