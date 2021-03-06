#include <iostream>
#include <string>
#include <cstdlib> // used to include the die function
using namespace std;

bool die( const string & msg ){
    cout <<msg <<endl;
    exit( EXIT_FAILURE );
}

class Room {
	private:
		bool trap; 
		
	public:
		Room() {
			trap = false;
		}
		
		void setTrapStatus( bool trap ) {
			this->trap = trap;
		}
		
		bool getTrapStatus() {
			return trap;
		}
};

class Map {
	private:
		Room rooms[20][20];
		int width;  // how many boxes the grid has vertically
		int length; // how many boxes the grid has horizantally
	
	public:
		Map() {	
			width = 1;
			length = 1;
		}
		
		Map ( int width, int length ) {
			this->width = width;
			this->length = length;
		}
		
		void setLimits ( int width, int length  ) {
			this->width = width;
			this->length = length;
		}
		
		int getWidth() {
			return width;
		}
		
		int getLength() {
			return length;
		}
		
		void setTrapOn ( int y, int x ) {
			rooms[y][x].setTrapStatus(true);
		}
		
		bool getTrap ( int y, int x ) {
			return rooms[y][x].getTrapStatus();
		}
		
		bool levelCleared() {
			for ( int y = 0; y < width; y++ ) {
				for (int x = 0; x < length; x++ ) {
					// if there exists a room where a trap is not set, then the map is not cleared.
					if ( rooms[y][x].getTrapStatus() == false ) return false;
				}
			}
			return true;
		}
		
		void display ( int y , int x ) { 
			string player_row = ""; // row in which player is in.
	
			// Update position x,y in accordance to the grid so that 'O' is displayed properly.
			int display_x = 1 + ( 4 * x ); 
			int display_y = 2 + ( 3 * y ); 
	
			cout << "\nYou are currently here: \n";
			// top line:
			for ( int i = 0; i < length; i++ ) {
				cout << "___ ";
			}
			cout << "\n";

			for ( int current_line = 1; current_line < (width * 3 + 1) ; current_line++ ) { 
				// first line in every row of boxes. 
				if ( current_line % 3 == 1 ) { 
					for ( int i = 0; i < length; i++ ) {
						cout << "   |";
					}
					cout << "\n";
				}
				
				// second line every row of boxes
				else if ( current_line % 3 == 2 ) {
					player_row.clear();
					for ( int i = 0; i < length; i++ ) { // iterate through every '   |' block
						// each block only consists of 4 characters: ' ? |'
						player_row.append(" ");
						if ( current_line == 2 && length/2 == i ) player_row.append("1");
						else if ( rooms[current_line/3][i].getTrapStatus() == true ) player_row.append("X");
						else player_row.append(" ");
						player_row.append(" |");
					}	
					// Rewrite designated box with player 'O'
					if (current_line == display_y) player_row[display_x] = 'O'; 
					cout << player_row << "\n";
				} 
				
				// third line in every row of boxes
				else { 
					for ( int i = 0; i < length; i++ ) {
						cout << "___|";
					}
					cout << "\n";
				}
			} // end of for loop
			cout << "\n";
		} 
}; 

class Position {
	private:
		int y;
		int x;
	
	public:
		Position() {
			x = 0;
			y = 0;
		}
		
		Position(Map maps) {
			// player is initially located at the bottom row in the middle of the grid
			y = maps.getWidth() - 1; 
			x = maps.getLength() / 2;
		}
		
		void setPosition(Map maps) {
			y = maps.getWidth() - 1; 
			x = maps.getLength() / 2;
		}
		
		int getX() {
			return x;
		}
		
		int getY() {
			return y;
		}
		
		void setTrap ( int y, int x, Map &maps ) {
			if ( maps.getTrap ( y, x ) == true ) die ( "Game Over" );
			else maps.setTrapOn( y, x);
		}
		
		void goNorth( Map &map ) { 
			if ( y > 0 && y < map.getWidth() ) {
				y = y - 1;
				map.display(y,x);
				setTrap(y,x,map);
			}
			else cout << "\nYou can't go that way.\nPosition has not been changed.\n\n";
		}
		
		void goSouth( Map &map ) {
			if ( y >= 0 && y < map.getWidth() - 1 ) {
				y = y + 1;
				map.display(y,x);
				setTrap(y,x,map);
			}
			else cout << "\nYou can't go that way.\nPosition has not been changed.\n\n";
		}
		
		void goEast( Map &map) {
			if ( x >= 0 && x < map.getLength() - 1 ) {
				x = x + 1;
				map.display(y,x);
				setTrap(y,x,map);
			}
			else cout << "\nYou can't go that way.\nPosition has not been changed.\n\n";
		}
		
		void goWest( Map &map ) {
			if ( x > 0 && x < map.getLength() ) {
				x = x - 1;
				map.display(y,x);
				setTrap(y,x,map);
			}
			else cout << "\nYou can't go that way.\nPosition has not been changed.\n\n";
		}
};

void options() {
	cout	<< "Here are your options:\n"
	 		<< "W: Go North\n" << "S: Go South\n" << "D: Go East\n" << "A: Go West\n"
	 		<< "M: Map\n"
	 		<< "O: Options\n"
		 	<< "Q: Quit the program\n";
}

void introduction( int y, int x, Map map ) {
	cout << "*******************************************\n";
	cout << "Watch Out!\n\n";
	cout  << "*******************************************\n";
	cout << "Watch Out! is a puzzle game where you must maneuver yourself ('O') to where the '1' is located at.";
	cout << "\nThe catch though is that you can only walk through each room once and only once.\n";
	cout << "Rooms labeled with an 'X' are rooms that you can not visit.\n";
	cout << "You must have an 'X' in every room before you walk to the room where the '1' is at.\n";
	map.display(y,x);
	options();
}

int main () {
	
	// create variables to hold user responses.
	char answer;
	char finalAnswer;
	
	// create the first level
	Map firstLevel(3,3);
	Position me(firstLevel);
	
	// set traps
	firstLevel.setTrapOn( me.getY() , me.getX() ); // set trap at initial position.
	firstLevel.setTrapOn( 2 , 0 );
	firstLevel.setTrapOn( 0 , 2 );
	
	introduction( me.getY(), me.getX(), firstLevel );
	
	// first level loop
	while ( firstLevel.levelCleared() == false ) {
		cout << "What would you like to do? ";
		cin >> answer;
		answer = toupper(answer);
		
		if ( answer == 'M') firstLevel.display( me.getY() , me.getX() );
		else if (answer == 'W')  me.goNorth( firstLevel ); 
		else if ( answer == 'A') me.goWest( firstLevel );
		else if ( answer == 'S') me.goSouth( firstLevel );
		else if ( answer == 'D') me.goEast( firstLevel );
		else if ( answer == 'O') options();
		else if ( answer == 'Q') {
			cout << "Are you sure you want to quit playing? Answer: [Y] / [N]" << "\t";
			cin >> finalAnswer;
			finalAnswer = toupper(finalAnswer);
			if ( finalAnswer == 'Y') {
				die ( "Thank you for playing this game. I hoped you enjoyed it :)\n" );
			}
			else cout << "\n"; continue;
		}
		else cout << "Invalid Input. Try again.\n";
	}
	
	cout << "1st Level Cleared!";
	
	// create second level
	Map secondLevel(3,7);
	me.setPosition(secondLevel);
	
	// set traps
	secondLevel.setTrapOn( me.getY() , me.getX() ); // set trap at initial position.
	secondLevel.setTrapOn( 1 , 1 );
	secondLevel.setTrapOn( 2 , 4 );
	
	secondLevel.display( me.getY() , me.getX() );
	
	// second level loop
	while ( secondLevel.levelCleared() == false ) {
		cout << "What would you like to do? ";
		cin >> answer;
		answer = toupper(answer);
		
		if ( answer == 'M') secondLevel.display( me.getY() , me.getX() );
		else if (answer == 'W')  me.goNorth( secondLevel ); 
		else if ( answer == 'A') me.goWest( secondLevel );
		else if ( answer == 'S') me.goSouth( secondLevel );
		else if ( answer == 'D') me.goEast( secondLevel );
		else if ( answer == 'O') options();
		else if ( answer == 'Q') {
			cout << "Are you sure you want to quit playing? Answer: [Y] / [N]" << "\t";
			cin >> finalAnswer;
			finalAnswer = toupper(finalAnswer);
			if ( finalAnswer == 'Y') {
				die ( "Thank you for playing this game. I hoped you enjoyed it :)\n" );
			}
			else cout << "\n"; continue;
		}
		else cout << "Invalid Input. Try again.\n";
	}
	
	cout << "2nd Level Cleared!";
	
	// create third level
	Map thirdLevel(4,11);
	me.setPosition(thirdLevel);
	
	// set traps
	thirdLevel.setTrapOn( me.getY() , me.getX() ); // set trap at initial position.
	thirdLevel.setTrapOn( 3 , 0 );
	thirdLevel.setTrapOn( 1 , 5 );
	thirdLevel.setTrapOn( 3 , 8 );
	thirdLevel.setTrapOn( 1 , 9 );
	
	thirdLevel.display( me.getY() , me.getX() );
	
	// third level loop
	while ( thirdLevel.levelCleared() == false ) {
		cout << "What would you like to do? ";
		cin >> answer;
		answer = toupper(answer);
		
		if ( answer == 'M') thirdLevel.display( me.getY() , me.getX() );
		else if (answer == 'W')  me.goNorth( thirdLevel ); 
		else if ( answer == 'A') me.goWest( thirdLevel );
		else if ( answer == 'S') me.goSouth( thirdLevel );
		else if ( answer == 'D') me.goEast( thirdLevel );
		else if ( answer == 'O') options();
		else if ( answer == 'Q') {
			cout << "Are you sure you want to quit playing? Answer: [Y] / [N]" << "\t";
			cin >> finalAnswer;
			finalAnswer = toupper(finalAnswer);
			if ( finalAnswer == 'Y') {
				die ( "Thank you for playing this game. I hoped you enjoyed it :)\n" );
			}
			else cout << "\n"; continue;
		}
		else cout << "Invalid Input. Try again.\n";
	}
	
	cout << "CONGRATULATIONS YOU BEAT THE GAME :)";
	
} // end of main function
