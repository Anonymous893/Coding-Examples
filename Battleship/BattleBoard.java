public class BattleBoard {

	private final int SMALLEST_SHIP = 2;
	private final String [] NAMES = {"Submarine","Destroyer","Cruiser","Battleship","Aircraft Carrier"};
	private final int [] DIMENSIONS = {10,10};

	private GamePiece [] ships;
	private int [][] board;
	private int [][] shipInfo;
	private int shipsSunk;
	private int [] fixedInput;

	public BattleBoard() {

		board = new int[DIMENSIONS[0]][DIMENSIONS[1]];
		shipInfo = new int[DIMENSIONS[0]][DIMENSIONS[1]];
		ships = new GamePiece[NAMES.length];
		shipsSunk = 0;

		//Initializes the gamepiece array with their names and sizes
		for(int i = 0; i < ships.length; i ++) {

			ships[i] = new GamePiece(i+SMALLEST_SHIP);
			genLoc(ships[i], i+SMALLEST_SHIP);

		}

	}

	//Checks if a spot has not been attacked yet
	private boolean spotOpen(int[] loc, int[][]matrix) {

		return matrix[loc[0]][loc[1]]==0;

	}

	//checks if all of a players ships have been sunk
	public boolean gameOver() {

		return ships.length == shipsSunk;

	}

	//checks if an input is valid
	public boolean checkInput(String input) {

		//checks if the player did not enter enough characters to have a full input  
		if(input.length() < 2) {

			return false;

		}

		String letter = input.substring(0, 1);
		int col;

		//parses the number portion of the input
		try {

			col = Integer.parseInt(input.substring(1)) - 1;

		}catch(NumberFormatException e) {

			return false;

		}

		//checks if the number portion of the input in in bounds
		if ((col < 0) || (col >= DIMENSIONS[1])){

			return false;

		}

		//checks if the letter portion of the input is in bounds
		int row = letter.compareTo("A");
		if ((row < 0) || (row >= DIMENSIONS[0])) {

			return false;

		}

		fixedInput = new int[2];
		fixedInput[0] = row;
		fixedInput[1] = col;

		return spotOpen(fixedInput,board);
	}

	//Updates the board: if a ship is hit, board value changes to 2
	//If there isn't a ship at the specified coordinates, board value changes to 1
	public void update() {

		//Checks if there is a ship at the coordinates
		if(!spotOpen(fixedInput, shipInfo)) {

			board[fixedInput[0]][fixedInput[1]]=2;
			int index = shipInfo[fixedInput[0]][fixedInput[1]]-SMALLEST_SHIP;

			System.out.println("Attack hit");

			//Checks if the ship is sunk
			if(ships[index].hit()) {

				System.out.println(NAMES[index] + " sunk");
				shipsSunk++;

			}

		}

		else {

			board[fixedInput[0]][fixedInput[1]]=1;
			System.out.println("Attack missed");

		}

	}

	//Randomly generates the location of a ship
	private void genLoc(GamePiece piece, int size) {

		int row;
		int col;
		int direction;

		//Creates a random direction and starting point
		do {

			direction = (int) (Math.random()*2);

			//Horizontal
			if(direction == 0) {

				row = calcPoint(DIMENSIONS[0]-size);
				col = calcPoint(DIMENSIONS[1]);
			}

			//Vertical
			else {

				row = calcPoint(DIMENSIONS[0]);
				col = calcPoint(DIMENSIONS[1]-size);
			}

		}while(!openSpace(size, row, col, direction));

		//Starting location
		shipInfo[row][col] = size;

		//Builds off of starting location horizontally
		if(direction==0) {

			for(int i  = row; i<row+size; i++) {

				shipInfo[i][col] = size;
			}
		}

		//Builds off of starting location vertically
		else {

			for(int i  = col; i<col+size; i++) {

				shipInfo[row][i] = size;
			}

		}

	}

	//Calculates random starting location coordinates
	private int calcPoint(int length) {

		return (int) (Math.random()*length);

	}

	//determines which open space method to go to
	private boolean openSpace(int size, int row, int col, int direction) {

		if(direction==0) {

			return openSpaceCol(size, row, col);

		}else {

			return openSpaceRow(size, row, col);

		}
	}

	//checks a given row to see if a given ship can fit
	private boolean openSpaceRow(int size, int row, int col) {

		for(int i = col; i<col+size; i++) {

			if(i>= DIMENSIONS[1]||shipInfo[row][i]!=0) {

				return false;
			}

		}

		return true;
	}

	//checks a given column to see if a given ship can fit
	private boolean openSpaceCol(int size, int row, int col) {

		for(int i = row; i<row+size; i++) {

			if(i>=DIMENSIONS[0]||shipInfo[i][col]!=0) {

				return false;
				
			}

		}

		return true;

	}

	public String toString() {

		return print(shipInfo);

	}

	public void print() {

		System.out.println(print(board));

	}

	//creates a String based off of a variation of the board to be printed later
	private String print(int[][] matrix) {

		//number heading
		String toReturn = " ";
		for(int i = 0; i<DIMENSIONS[1]; i++) {

			toReturn+= "  " + (i+1);

		}
		toReturn +="\n";

		for(int row = 0; row<DIMENSIONS[0]; row++) {

			//letters on side
			toReturn += (char)(row+65) + "  ";
			for(int col = 0; col<DIMENSIONS[1]; col++) {

				//info at a given spot
				toReturn += matrix[row][col] + "  ";
			}

			toReturn += "\n";
		}

		return toReturn;
	}

}
