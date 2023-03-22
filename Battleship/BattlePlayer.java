/*
 * BattlePlayer
 * Represents a single player in a game of battleship
 */
public class BattlePlayer {

	private String name;
	private BattleBoard board;

	public BattlePlayer(String n) {

		name = n;
		board = new BattleBoard();
	
	}


	public boolean checkInput(String input) {

		return board.checkInput(input);

	}

	public void update() {

		board.update();

	}

	public boolean gameOver() {

		return board.gameOver(); 

	}

	public String getName() {

		return name;

	}

	public void printBoard() {

		board.print();

	}

}
