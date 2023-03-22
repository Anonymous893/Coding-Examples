/*
 * BattleDriver
 * Runs a game of battleship using the BattleShip class
 */
import java.util.*;
public class BattleDriver {

	public static void main(String [] args) {

		BattleShip.displayRules();

		//prompts both players for their names
		Scanner keyboard = new Scanner(System.in);
		System.out.println("\n\nPlayer one enter your name: ");
		String one = keyboard.nextLine();

		System.out.println("Player two enter your name: ");
		String two = keyboard.nextLine();

		BattleShip game = new BattleShip(one, two); 

		//runs the game of battleship
		do {
			System.out.println("\n\n");
			game.displayBoard();

			//prompts the user for an input until a valid input is received
			do {

				game.prompt();

			}while(!game.valid());


			game.updateBoard();
			game.displayBoard();
			System.out.println("--------------------------------\n");

		}while(!game.determineWinner());
	}

}
