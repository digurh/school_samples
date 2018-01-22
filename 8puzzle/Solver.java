import edu.princeton.cs.algs4.MinPQ;
import edu.princeton.cs.algs4.Queue;
import java.lang.IllegalArgumentException;
import java.lang.NullPointerException;


public class Solver {
  MinPQ<SearchNode> boardPriority;
  Board initial;
  int moves;

  public Solver(Board initial) {           // find a solution to the initial board (using the A* algorithm)
    if(initial.isSolvable() == false) throw new IllegalArgumentException();
    if(initial.equals(null)) throw new NullPointerException();

    boardPriority = new MinPQ<SearchNode>();
    moves = 0;
    this.initial = initial;
    Iterable<Board> solutionBoards = solution();
    for(Board b : solutionBoards) System.out.println(b);
  }

  public int moves() {                    // min number of moves to solve initial board
    return moves;
  }

  public Iterable<Board> solution() {     // sequence of boards in a shortest solution
    SearchNode node = new SearchNode(this.initial, 0, null);

    while(!node.board.isGoal()) {
      Iterable<Board> it = node.board.neighbors();

      //have to use for-each with type iterable
      //optimization to make sure previous board not enqueued
      for(Board b : it) {
        if(node.previous == null)
            boardPriority.insert(new SearchNode(b, node.moves + 1, node));
        else
          if(!b.equals(node.previous.board))
            boardPriority.insert(new SearchNode(b, node.moves + 1, node));
      }
      node = boardPriority.delMin();
    }
    Queue<Board> sb = new Queue<Board>();
    sb.enqueue(node.board);

    this.moves = node.moves;

    while(node.previous != null) {
      node = node.previous;
      sb.enqueue(node.board);
    }
    return sb;
  }

  //represents steps along solution
  private class SearchNode implements Comparable<SearchNode> {
    Board board;
    int moves;
    int priority;
    SearchNode previous;

    public SearchNode(Board board, int moves, SearchNode previous) {
      this.board = board;
      this.moves = moves;
      this.previous = previous;
      this.priority = moves + board.getManhattan();
    }

    public int compareTo(SearchNode sn) {
      // if(this.board.getManhattan() == sn.board.getManhattan()) return 0;
      // if(this.board.getManhattan() > sn.board.getManhattan()) return 1;
      if(this.priority == sn.priority) return 0;
      if(this.priority > sn.priority) return 1;
      else return -1;
    }
  }

  public static void main(String[] args) { // unit testing


  }
}
