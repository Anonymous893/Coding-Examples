/*
 * GamePiece
 * Represents a piece in a game of BattleShip
 */
public class GamePiece{
        private int size;
        private int status;
  
        public GamePiece(int s){
                size = s;
                status = s;
        }
  
        //Updates ship after it has been ship
        public boolean hit(){
              status -= 1;
          
              //Returns true if ship has been sunk
              if(status == 0){
                       return true;
              }
              return false;
        }
}
