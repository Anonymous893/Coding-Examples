/*
 * BattleShip
 * Manages a game of BattleShip
 */
import java.util.*;
public class BattleShip {

	private BattlePlayer[] players;
	private int turn;
	private String input;

	public BattleShip(String one, String two) {

		players = new BattlePlayer[2];
		players[0] = new BattlePlayer(one);
		players[1] = new BattlePlayer(two);

		turn = (int)(Math.random()*2);
	}

	//prompts the user for their input
	public void prompt() {
		
		Scanner keyboard = new Scanner(System.in);
		System.out.print(players[turn%2].getName() + " enter your coordinates: ");
		input = keyboard.nextLine();
	}

	//checks if the last input is valid
	public boolean valid() {
		return players[turn%2].checkInput(input);
	}

	//updates the board
	public void updateBoard() {
		players[turn%2].update();
	}


	public void displayBoard() {
		players[turn%2].printBoard();
	}

	//determines if someone has won and prints their name if they have
	//if not switches whose turn it is
	public boolean determineWinner() {

		if(players[turn%2].gameOver()) {
			System.out.println(players[turn%2].getName() + " won the game");
			return true;
		}else {
			turn++;
			return false;
		}
	}
	
	//prints out the rules
	public static void displayRules() {

		System.out.println("Rules:");

		System.out.print("Two playerse take alternate turns");
		System.out.print(" firing at the opposing player's board in an attempt");
		System.out.println(" to hit one of their ships.");

		System.out.println("\nIf all of one player's ships are sunk, the other is the winner");

		System.out.println("\nCoordinates must be in this format: A10 (capital letter)");

		System.out.println("\nEach player has a board showing their hits and misses");
		System.out.println("on their opponent's ships.");
		System.out.println("A '1' represents a missed attacked");
		System.out.println("while a '2' represents a hit.");
		System.out.println("A '0' represents an unattacked spot.");
		System.out.println("-------------------------------------------------------");
	}
}
