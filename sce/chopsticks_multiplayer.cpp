#include "socketstream-master/socketstream.hh"
#include <iostream>
#include <sstream>

#define DEBUG
#ifdef DEBUG
#define D(x) x
#else
#define D(x)
#endif

using namespace std;
using namespace swoope;

int PLAYERS = 7; 

void tutorial(){
	string s;
	cout << "\nAre you familiar with the rules of S U P E R Chopsticks X X X T R E M E?\n"<<
			"1. Yes, I am very well versed in slaying my enemies.\n"<<
			"2. No, it's my first time playing something this violent.\n";
	while(true){
		cin >> s;
		cin.ignore();
		if(s == "1" || s == "1." || s == "Yes" || s == "yes"){
			cout << "\nGood. I won't keep you, then.\n"
		}
	}
	
}
void runServer(int argc, char* argv[]) {
    socketstream sockets[PLAYERS];
    socketstream listeningSocket;
	
	cout << string("\n\nWelcome to S U P E R Chopsticks X X X T R E M E, Player 1.\n") +
			"How many players should I prepare for?\n";
	
	int flavor_count = 0 ;
	while(PLAYERS > 6 || PLAYERS < 2){
		if(flavor_count < 3)
			cout << "<Enter a number from 2 - 6>\n";
		else if(flavor_count < 5)
			cout << "<2 3 4 5 6 only.>\n";
		else if(flavor_count < 7)
			cout << "<Seriously?>\n";
		else
			cout << "<...>\n";
		cin >> PLAYERS;
		cin.ignore();
		flavor_count++;
	}
	
	if(flavor_count < 5)
		cout << "\n";
	else if(flavor_count < 7)
		cout << "<Thank goodness>\n";
	else
		cout << "< :c finally >\n\n";
	flavor_count = 0;
	
	cout << PLAYERS << " players shall enter the arena...\n";
    listeningSocket.open(argv[1], PLAYERS);

	//Waiting for players...
    for (int i = 1; i < PLAYERS; i++) {
        listeningSocket.accept(sockets[i]);
		if(i == 1)
			cout << "Player 2 drops in from an overhead helicopter.\n";
		if(i == 2)
			cout << "Player 3 teleports behind you.\n";
		if(i == 3)
			cout << "Player 4 emerges from beneath the earth.\n";
		if(i == 4)
			cout << "Player 5 materializes from thin air.\n";
		if(i == 5)
			cout << "Player 6 comes in through the front door.\n";
    }

    for (int i = 1; i < PLAYERS; i++) {
        sockets[i] << i << endl;
    }
	
	cout << "\nAll players have successfully connected.\n";
	
	
}

void runClient(int argc, char* argv[]) {
	bool event_attack = false;				//turns true when a player, including this player, attacks
	bool event_distribute = false;			//turns true when a player, including this player, disthands
	bool event_turn_ready = false;			//turns true when it's this player's turnsS
    socketstream server;
    cout << "Travelling to the arena...\n";

    server.open(argv[1], argv[2]);

    int player_ID;
    cout << "Getting ready for battle...\n";
    server >> player_ID;
	server.ignore();

    cout << "\n\nWelcome to S U P E R Chopsticks X X X T R E M E, Player " << player_ID+1 << ".\n" <<
			"\n\nAll players have successfully connected.\n";


}

int main(int argc, char* argv[]) {
    string s;

    if (argc == 2) {
		if(atoi(argv[1]) < 1024 || atoi(argv[1]) > 65535){
			cout << "ERROR: Port listed is invalid. Please select a port between 1024-65535.\n";
			return 0;
		}
        runServer(argc, argv);
    } 
	else {
		if(atoi(argv[2]) < 1024 || atoi(argv[1]) > 65535){
			cout << "ERROR: Port listed is invalid. Please select a port between 1024-65535.\n";
			return 0;
		}
        runClient(argc, argv);
    }
	return 0;
}


//g++ -D _WIN32 -o chopsticks_multiplayer.exe chopsticks_multiplayer.cpp -lws2_32
//g++  your_file.cpp -o your_program
//cd Documents\soketstrm
//chopsticks_multiplayer 2555
//chopsticks_multiplayer localhost 2555