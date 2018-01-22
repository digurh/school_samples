import edu.princeton.cs.algs4.Digraph;
import edu.princeton.cs.algs4.BreadthFirstDirectedPaths;
import edu.princeton.cs.algs4.DirectedCycle;
import edu.princeton.cs.algs4.Queue;
import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import java.lang.NullPointerException;
import java.lang.IllegalArgumentException;
import java.lang.IndexOutOfBoundsException;


public class ShortestCommonAncestor {
  Digraph G;
  int root;

  // constructor takes a rooted DAG as argument
  public ShortestCommonAncestor(Digraph G) {
    if(G == null) throw new NullPointerException();


    this.G = new Digraph(G);
    int count = 0;

    //finds the root of the digraph
    for(int i = 0; i < this.G.V(); i++) {
      if(this.G.outdegree(i) == 0) {
        this.root = i;
        count++;
      }
    }
    DirectedCycle cycle = new DirectedCycle(G);
    if(count != 1) throw new IllegalArgumentException("More than one root");
    if(!bfsCheck(this.G.reverse())) throw new IllegalArgumentException("Root is not ancestor of all vertices");
    if(cycle.hasCycle()) throw new IllegalArgumentException("This has cycles");
  }

  //once root is found, ensures that digraph root is parent of all vertices
  private boolean bfsCheck(Digraph reverse) {
    Queue<Integer> q = new Queue<Integer>();
    boolean[] marked = new boolean[reverse.V()];
    int[] edgeTo = new int[reverse.V()];
    int count = 1;

    marked[this.root] = true;
    q.enqueue(this.root);
    while(!q.isEmpty()) {
      int v = q.dequeue();
      for(int x : reverse.adj(v)) {
        if(!marked[x]) {
          edgeTo[x] = v;
          marked[x] = true;
          q.enqueue(x);
          count++;
        }
      }
    }
    if(count != reverse.V()) return false;
    return true;
  }

  // length of shortest ancestral path between v and w
  public int length(int v, int w) {
    if(v < 0 || w < 0) throw new IllegalArgumentException();

    BreadthFirstDirectedPaths bfsV = new BreadthFirstDirectedPaths(this.G, v);
    BreadthFirstDirectedPaths bfsW = new BreadthFirstDirectedPaths(this.G, w);

    int ancestor = -1;

    //checks along path of v to root for a vertex shared with w
    for(Integer x : bfsV.pathTo(this.root)) {
      if(bfsW.hasPathTo(x)) {
        ancestor = x;
        break;
      }
    }
    int distance = bfsV.distTo(ancestor) + bfsW.distTo(ancestor);

    return distance;
  }

  // a shortest common ancestor of vertices v and w
  public int ancestor(int v, int w) {
    if(v < 0 || w < 0) throw new IllegalArgumentException();

    BreadthFirstDirectedPaths bfsV = new BreadthFirstDirectedPaths(this.G, v);
    BreadthFirstDirectedPaths bfsW = new BreadthFirstDirectedPaths(this.G, w);

    for(Integer x : bfsV.pathTo(this.root)) {
      if(bfsW.hasPathTo(x)) return x;
    }
    return -1;
  }

  // length of shortest ancestral path of vertex subsets A and B
  public int length(Iterable<Integer> subsetA, Iterable<Integer> subsetB) {
    if(subsetA == null || subsetB == null) throw new NullPointerException();

    BreadthFirstDirectedPaths bfsV = new BreadthFirstDirectedPaths(this.G, subsetA);
    BreadthFirstDirectedPaths bfsW = new BreadthFirstDirectedPaths(this.G, subsetB);

    return 0;
  }

  // a shortest common ancestor of vertex subsets A and B
  public int ancestor(Iterable<Integer> subsetA, Iterable<Integer> subsetB) {
    if(subsetA == null || subsetB == null) throw new NullPointerException();

    BreadthFirstDirectedPaths bfsV = new BreadthFirstDirectedPaths(this.G, subsetA);
    BreadthFirstDirectedPaths bfsW = new BreadthFirstDirectedPaths(this.G, subsetB);

    return 0;
  }

  // do unit testing of this class
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
