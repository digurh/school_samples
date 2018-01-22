import java.lang.IndexOutOfBoundsException;
import edu.princeton.cs.algs4.Queue;
import edu.princeton.cs.algs4.StdRandom;

public class Board {
    private int[][] tiles;
    private int manhattanNum;

    public Board(int[][] tiles) {            // construct a board from an N-by-N array of tiles
      this.tiles = tiles;                    // (where tiles[i][j] = tile at row i, column j)
      this.manhattanNum = manhattan();
    }

    public int tileAt(int i, int j) {        // return tile at row i, column j (or 0 if blank)
      if(i < 0 || i > size() - 1 || j < 0 || j > size() - 1) throw new IndexOutOfBoundsException();
      return this.tiles[i][j];
    }

    public int size() {                      // board size N
      return this.tiles.length;
    }

    public int getManhattan() {
      return this.manhattanNum;
    }

    public int hamming() {                   // number of tiles out of place
      int N = size();
      int correctNumber = 1;
      int outOfPlace = 0;

      for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
          if(this.tiles[i][j] != correctNumber && this.tiles[i][j] != 0) {
            outOfPlace++;
          }
          correctNumber++;
        }
      }
      return outOfPlace;
    }

    public int manhattan() {                 // sum of Manhattan distances between tiles and goal
      int N = size();
      int correctNumber = 1;
      int manhattanSum = 0;

      for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
          if(this.tiles[i][j] != correctNumber && this.tiles[i][j] != 0) {
            int correctI = (int)(this.tiles[i][j] / 3);
            int correctJ = (this.tiles[i][j] % 3) - 1;
            manhattanSum += Math.abs(correctI - i) + Math.abs(correctJ - j);
          }
          correctNumber++;
        }
      }
      return manhattanSum;
    }

    public boolean isGoal() {                // is this board the goal board?
      int N = size();
      int correctNumber = 1;

      for(int i = 0; i < N; i ++) {
        for(int j = 0; j < N; j++) {
          if(this.tiles[i][j] == 0 && correctNumber != (N * N)) return false;
          if(this.tiles[i][j] != correctNumber && this.tiles[i][j] != 0) return false;
          correctNumber++;
        }
      }
      return true;
    }

    public boolean isSolvable() {            // is this board solvable?
      int N = size();
      int[] inversionArray = new int[N * N];
      int converter = 0;
      int inversions = 0;
      int locationI = 0;

      //find empty space and converts 2d array to 1d
      for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
          if(this.tiles[i][j] == -1) locationI = i;
          inversionArray[converter] = this.tiles[i][j];
          converter++;
        }
      }
      //counts inversions
      //for(int x : inversionArray) System.out.print(x + " ");
      for(int i = 0; i < N * N - 1; i++) {
        for(int j = i + 1; j < N * N; j++) {
          if(inversionArray[i] > 0)
            if(inversionArray[i] > inversionArray[j] && inversionArray[j] != 0) inversions++;
        }
      }
      //System.out.println("\ninvesions = " + inversions);
      //for even board size, then odd
      if((N % 2) == 0 && (inversions + locationI) % 2 == 0) return false;
      else if((N % 2) == 1 && (inversions % 2) == 1) return false;
      else return true;
    }

    public boolean equals(Object y) {        // does this board equal y?
      int N = size();

      if(this == (Board) y) return true;
      else if(this == null) return false;
      else return false;
    }

    public Iterable<Board> neighbors() {     // all neighboring boards
      int N = size();
      int[] openSpace = new int[2];

      for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
          if(this.tiles[i][j] == 0) {
            openSpace[0] = i;
            openSpace[1] = j;
            break;
          }
        }
      }
      Queue<Board> neighborQueue = new Queue<Board>();
      //System.out.println("openSpace = " + openSpace[0] + " " + openSpace[1]);
      if(openSpace[0] > 0) neighborQueue.enqueue(exchange(openSpace, openSpace[0] - 1, openSpace[1]));
      if(openSpace[0] < N - 1) neighborQueue.enqueue(exchange(openSpace, openSpace[0] + 1, openSpace[1]));
      if(openSpace[1] > 0) neighborQueue.enqueue(exchange(openSpace, openSpace[0], openSpace[1] - 1));
      if(openSpace[1] < N - 1) neighborQueue.enqueue(exchange(openSpace, openSpace[0], openSpace[1] + 1));

      return neighborQueue;
    }

    private Board exchange(int[] openSpace, int moveRow, int moveCol) {
      int N = size();
      int row = openSpace[0];
      int col = openSpace[1];
      int[][] neighbor = new int[N][N];

      for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
          neighbor[i][j] = this.tiles[i][j];
        }
      }
      int temp = neighbor[moveRow][moveCol];
      neighbor[moveRow][moveCol] = 0;
      neighbor[row][col] = temp;

      return new Board(neighbor);
    }

    public String toString() {               // string representation of this board (in the output format specified below)
      int N = size();

      StringBuilder s = new StringBuilder();
      s.append(N + "\n");
      for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
          s.append(String.format("%2d ", tileAt(i, j)));
        }
        s.append("\n");
      }
      return s.toString();
    }

    public static void main(String[] args) { // unit testing (required)
      int[][] boardArray = new int[3][3];
      int count = 0;

      for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
          boardArray[i][j] = 0;
        }
      }

      for(int i = 0; i < 9; ++i) {
        int x = StdRandom.uniform(0, 3);
        int y = StdRandom.uniform(0, 3);
        if(boardArray[x][y] == 0) boardArray[x][y] = i;
        else i--;
      }
      Board b = new Board(boardArray);

      System.out.println(b.toString());

      System.out.println("hamming = " + b.hamming() + "\nmanhattan = " + b.manhattan());
      System.out.println("solvable = " + b.isSolvable());

      System.out.println(b.neighbors());
    }
}
