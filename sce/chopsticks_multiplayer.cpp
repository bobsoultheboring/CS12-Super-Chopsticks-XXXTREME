#include "socketstream-master/socketstream.hh"
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

#define DEBUG
#ifdef DEBUG
#define D(x) x
#else
#define D(x)
#endif

using namespace std;
using namespace swoope;

int PLAYERS = 7;


//Super Chopsticks v.0.1
//Project developed by Vince Yalung and Lance Bassig

//Declarations
class Player;
class Team;
class Limb;
vector<Player> master_list;
vector<Team> team_list;
int teams_left;
Player* selected_player;
bool pre_game;
bool debugmode;


//Object Definitions
class Limb{
	public:
	Limb(bool,int);
	bool is_foot;
	bool is_dead;
	int max_fingers;
	int fingers;
	bool add_fingers(int);
};

class Player{
	public:
	//Constructor
	Player(string,int,int,int,int);
	//Player Identification
	int player_id;
	int race;
	int team_id;
	int team_slot;
	string username;
	//Turn-related Flags
	bool is_spectator;
	bool skipped;
	//Player Points
	int actions;
	int damage;
	int health;
	int kills;
	//Player Inventory
	vector<Limb> hands;
	vector<Limb> feet;
	//Player Actions
	bool attack();
	bool distribute(vector<int>);
	void turnprep();
	void checkhealth();
	void viewstats();
	void help();
	//Special Abilities
	bool steeltoes;
	bool regenerate;
	bool goodboi;
	
};

class Team{
	public:
	//Constructor
	Team();
	//Team Information
	vector<int> roster;
	vector<bool> player_is_alive;
	bool team_is_alive;
	int team_id;
	int current_player;
	//Team Actions
	void checkhealth();
};

//Definition of LIMB Functions
Limb::Limb(bool foot, int fingercap){
	is_foot = foot;
	is_dead = false;
	max_fingers = fingercap;
	fingers = 1;
}

bool Limb::add_fingers(int add){
	if(is_foot){
		fingers += add;
		if (fingers >= max_fingers){
			is_dead = true;
			fingers = max_fingers;
			return true;
		}
		return false;
	}
	else{
		fingers += add;
		if (fingers > max_fingers){
			fingers -= max_fingers;
		}
		if (fingers == max_fingers){
			is_dead = true;
			return true;
		}
	}
	return false;
}

//Definition of PLAYER functions
Player::Player(string namn, int id, int r, int team, int slot){
	cout << "Making Player " << namn << endl;
	username = namn;
	player_id = id;
	race = r;
	team_id = team;
	team_slot = slot;
	is_spectator = false;
	skipped = false;
	actions = 0;
	damage = 0;
	steeltoes = 0;
	regenerate = 0;
	goodboi = 0;
	team_list[team].roster.push_back(id);
	if (race == 0){
		//HUMAN PLAYER
		Limb hand(false, 5);
		Limb foot(true, 5);
		hands.push_back(hand);
		hands.push_back(hand);
		feet.push_back(foot);
		feet.push_back(foot);
	}
	else if (race == 1){
		//AYYLMAO
		steeltoes = 1;
		Limb hand(false, 3);
		Limb foot(true, 2);
		hands.push_back(hand);
		hands.push_back(hand);
		hands.push_back(hand);
		hands.push_back(hand);
		feet.push_back(foot);
		feet.push_back(foot);
	}
	else if (race == 2){
		//T-VIRUS
		regenerate = 1;
		Limb hand(false, 4);
		hands.push_back(hand);
	}
	else if (race == 3){
		//WOOF WOOF
		goodboi = 1;
		Limb foot(true, 4);
		feet.push_back(foot);
		feet.push_back(foot);
		feet.push_back(foot);
		feet.push_back(foot);
	}
	health = hands.size() + feet.size();
	if (race == 2){
		health = 2;
	}
}

bool Player::attack(){
	while(true){
		cout << "------------------------------------------------------------------------------" << '\n';
		cout << "Attack. Please select a target by typing their player number." << '\n';
		for(int i = 0; i < master_list.size(); i++){
			if (master_list.at(i).team_id != team_id && !master_list.at(i).is_spectator){
				cout << i + 1 << ". " << master_list.at(i).username << " (Team " << master_list.at(i).team_id + 1 << ")" << '\n';
			}
		}
		cout << "------------------------------------------------------------------------------" << '\n';
		cout << "Enter target : ";
		int targetnumber;
		
		while(true){
			cin >> targetnumber;
			cin.ignore();
			if (targetnumber <= 0 || targetnumber > master_list.size()){
				cout << "[Error] No such player exists." << '\n';
				continue;
			}
			if (master_list.at(targetnumber-1).player_id == player_id){
				cout << "[Error] You wish to attack yourself? Please select a different player." << '\n';
				continue;
			}
			if (master_list.at(targetnumber-1).team_id == team_id){
				cout << "[Error] This player is your friend. Please select a different player." << '\n';
				continue;
			}
			if (master_list.at(targetnumber-1).is_spectator){
				cout << "[Error] This player is dead. Please select a different player." << '\n';
				continue;
			}
			if (cin.fail()){
				cout << "[Error] Invalid input. Did you input the username instead of the Player ID?" << '\n';
				continue;
			}
			break;
		}
		
		Player* target = &(master_list.at(targetnumber-1));
		cout << "------------------------------------------------------------------------------"  << '\n';
		cout << target->username << "'s extremities. Select one by typing the name and number (e.g. hand 2) Use lowercase only."<< '\n';
		vector<int> validhands;
		vector<int> validfeet;
		for(int i = 0; i < target->hands.size(); i++){
			if (!target->hands.at(i).is_dead){
				cout << "hand " << i+1 << " :" << target->hands.at(i).fingers << "/" << target->hands.at(i).max_fingers << " fingers." << '\n';
				validhands.push_back(i);
			}
		}
		for(int i = 0; i < target->feet.size(); i++){
			if (!target->feet.at(i).is_dead){
				cout << "foot " << i+1 << " :" << target->feet.at(i).fingers << "/" << target->feet.at(i).max_fingers << " toes." << '\n';
				validfeet.push_back(i);
			}
		}
		cout << "------------------------------------------------------------------------------"  << '\n';
		cout << "Enter target : ";
		string targetpart;
		int targetno;
		Limb* targetlimb;
		while(true){
			cin >> targetpart;
			cin.ignore();
			cin >> targetno;
			cin.ignore();
			if (targetpart == "hand"){
				if (find(validhands.begin(),validhands.end(),targetno-1) != validhands.end()){
					targetlimb = &(target->hands.at(targetno-1));
					break;
				}
			}
			if (targetpart == "foot"){
				if (find(validfeet.begin(),validfeet.end(),targetno-1) != validfeet.end()){
					targetlimb = &(target->feet.at(targetno-1));
					break;
				}
			}
			else{
				cout << "[Error] Invalid input." << '\n';
				continue;
			}			
		}
		cout << "------------------------------------------------------------------------------"  << '\n';
		cout << "Your extremities. Choose one to attack with using the same format (e.g. hand 1)" << '\n';
		vector<int> myvalidhands;
		vector<int> myvalidfeet;
		for(int i = 0; i < hands.size(); i++){
			if (!hands.at(i).is_dead){
				cout << "hand " << i+1 << ": " << hands.at(i).fingers << "/" << hands.at(i).max_fingers << " fingers." << '\n';
				myvalidhands.push_back(i);
			}
		}
		for(int i = 0; i < feet.size(); i++){
			if (!feet.at(i).is_dead){
				cout << "foot " << i+1 << " : " << feet.at(i).fingers << "/" << feet.at(i).max_fingers << " toes." << '\n';
				myvalidfeet.push_back(i);
			}
		}
		cout << "#------------------------------------------------------------------------------"  << '\n';
		cout << "Enter weapon : ";
		string mypart;
		int myno;
		Limb* attacklimb;
		while(true){
			cin >> mypart;
			cin.ignore();
			cin >> myno;
			cin.ignore();
			if (mypart == "hand"){
				if (find(myvalidhands.begin(),myvalidhands.end(),myno-1) != myvalidhands.end()){
					attacklimb = &(hands.at(myno-1));
					break;
				}
			}
			if (mypart == "foot"){
				if (find(myvalidfeet.begin(),myvalidfeet.end(),myno-1) != myvalidfeet.end()){
					attacklimb = &(feet.at(myno-1));
					break;
				}
			}
			cout << "[Error] Invalid input." << '\n';
		}
		cout << "[Server] Attack is registered." << '\n';
		bool oof = targetlimb->add_fingers(attacklimb->fingers);
		if (oof){
			if (targetlimb->is_foot && !target->steeltoes) target->skipped = 1;
			target->damage++;
			target->checkhealth();
		}
		if (target->goodboi && !goodboi) skipped = 1;
		return true;
	}
	return false;
}

bool Player::distribute(vector<int> dist){
	for(int i = 0; i < dist.size(); i++){
		cout << dist[i] << " ";
	}
	
	vector<int> livehands;
	vector<int> currentvalues;
	int totalfingers = 0;
	
	for (int i = 0; i < hands.size(); i++){
		if (!hands.at(i).is_dead){
			livehands.push_back(i);
			currentvalues.push_back(hands.at(i).fingers);
			totalfingers += hands.at(i).fingers;
		}
	}
	

	
	if (livehands.size() == 1){
		cout << "[Error] You only have one hand left. Unable to distribute." << '\n';
		return false;
	}
	
	if (livehands.size() == 0){
		cout << "[Error] You have no hands." << '\n';
		return false;
	}
	
	if (dist.size() < livehands.size()){
		cout << "[Error] Missing parameters. You did not distribute to all live hands." << '\n';
		cout << "	     If you wish to free a hand (e.g. distribute 0 2), please say so." << '\n';
		return false;
	}
	
	if (dist.size() > livehands.size()){
		cout << "[Error] Excess parameters. Trying to cheat?" << '\n';
		return false;
	}
	
	bool different = 0;
	for (int i = 0; i < dist.size(); i++){
		if (dist.at(i) != currentvalues.at(i)){
			different = 1;
		}
	}
	
	if (!different){
		cout << "[Error] Fingers already distributed that way." << '\n';
		return false;
	}
	
	for (int i = 0; i < dist.size(); i++){
		if (dist.at(i) == hands[i].max_fingers){
		cout << "[Error] Distribution will kill a live hand." << '\n';
		return false;
		}
	}
	
	int newtotalfingers = 0;
	for (int i = 0; i < dist.size(); i++){
		newtotalfingers += dist.at(i);
	}
	
	if (newtotalfingers != totalfingers){
		cout << "[Error] Distribution has missing/extra fingers." << '\n';
		return false;
	}
	
	for (int i = 0; i < dist.size(); i++){
		if (dist.at(i) > hands[i].max_fingers){
			cout << "[Error] Distribution exceeds max fingers. You cannot overflow when distributing." << '\n';
		}
	}
	
	for(int i = 0; i < dist.size(); i++){
		hands[livehands.at(i)].fingers = dist.at(i);
	}
	return true;
}

void Player::turnprep(){
    actions++;
    if(race == 2)
        /* zombies are fast */
        actions++;
}

void Player::checkhealth(){
	if(regenerate){
		hands.push_back(Limb(false,4));
		regenerate = false;
	}
	if (damage >= health){
		is_spectator = 1;
		team_list[team_id].player_is_alive.at(team_slot) = 0;
		team_list[team_id].checkhealth();
	}
}

//Team Declarations
Team::Team(){
	team_is_alive = 1;
	current_player = 0;
}

void Team::checkhealth(){
	for (int i = 0; i < roster.size(); i++){
		if (player_is_alive[i]){
			return;
		}
	}
	team_is_alive = false;
	teams_left--;
}

void superpinter(){
	cout << "#------------------------------------------------------------------------------" << '\n';
	cout << "STATUS BOARD" << '\n';
	cout << "#------------------------------------------------------------------------------" << '\n';
	for(int i = 0; i < team_list.size(); i++){
		cout << "TEAM " << i + 1 << " : " <<'\n';
		cout << team_list.at(i).roster.size() << " members." << '\n';
		Player* nextguy = &(master_list[team_list.at(i).roster[team_list.at(i).current_player]]);
		cout << "Player " << nextguy->player_id + 1 << " : " << nextguy->username << " is next." << '\n';
		for (int j = 0; j < team_list.at(i).roster.size(); j++){
			Player* currplayer = &(master_list.at(team_list.at(i).roster[j]));
			cout << currplayer->player_id + 1 << ". " << currplayer->username << " - ";
			if (currplayer->race == 0) cout << " Human ";
			if (currplayer->race == 1) cout << " Alien ";
			if (currplayer->race == 2) cout << " Zombie ";
			if (currplayer->race == 3) cout << " Doggo ";
			if (currplayer->skipped) cout << "- STATUS: Skipped." << '\n';
			else if (currplayer->is_spectator) cout << "- STATUS: Eliminated." << '\n';
			else if (currplayer->player_id == nextguy->player_id) cout << "- STATUS: Next." << '\n';
			else cout << "STATUS: Waiting for turn." << '\n';
			for (int i = 0; i < currplayer->hands.size(); i++){
				if (currplayer->hands.at(i).fingers == 0) cout << "Hand " << i + 1 << " is free." << ". ";
				if (currplayer->hands.at(i).is_dead == 0) cout << "Hand " << i + 1 << " : " << currplayer->hands.at(i).fingers << ". ";
				else cout << "Hand " << i + 1 << " is dead." << ". ";
			}
			for (int i = 0; i < currplayer->feet.size(); i++){
				if (currplayer->feet.at(i).fingers == 0) cout << "Foot " << i + 1 << " is free." << ". ";
				else if (!currplayer->feet.at(i).is_dead) cout << "Foot " << i + 1 << " : " << currplayer->feet.at(i).fingers << ". ";
				else cout << "Foot " << i + 1 << " is dead" << ". ";
			}
			cout << '\n';
		}	
		cout << "#------------------------------------------------------------------------------" << '\n';		
	}
	if (!pre_game){
		cout << "Player " << selected_player->player_id + 1 << " " << selected_player->username << " will take this turn!" << '\n';
		cout << "#------------------------------------------------------------------------------" << '\n';
	}
}

//Game Functions
void act(int team, Player& actor){
	cin.ignore();
	string command;
	bool done = 0;
	while (!done){
		getline(cin,command);
		size_t found = command.find("disthands");
		if (command == "tap"){
			done = actor.attack();
		}
		else if (command == "help"){
			cout << "Available Actions:" << '\n';
			cout << "Attack - opens a menu which allows you to pick a target and attack. (syntax: tap)"<< '\n';
			cout << "Distribute - distributes fingers among your live hands. (syntax: disthands x x x, where x is no. of fingers) " << '\n';
			cout << "Surrender - Quit the game. (syntax: surrender)" << '\n';
		}
		else if (command == "surrender"){
			cout << "Are you sure you want to leave? Input 'yes' to confirm. Press enter to cancel." << '\n';
			string asd;
			cin >> asd;
			cin.ignore();
			if (asd == "yes"){
				actor.damage = actor.health;
				actor.checkhealth();
				done = 1;
			}
		}
		else if(found != string::npos && command.length() > 9){
			int len = command.length();
			int index = 10; //disthands X - index of X
			vector<int> distvalues;
			stringstream stream;
			while(index < len){
				if(command[index] != ' '){
					int in = command[index] - '0';
					distvalues.push_back(in);
				}
				index++; 
			}
			if(distvalues.empty()){
				cout << "[Error] Disthands failed. Are you missing parameters?" << '\n';
			}
			done = actor.distribute(distvalues);
		}
		else{
			cout << "[Error] Command not recognised." << '\n';
		}
	}
	actor.actions--;
}

void message_all(string s, socketstream sockets[]){
	for(int i = 0; i < PLAYERS; i++)
		sockets[i] << s << endl;
}
void char_create_server(int ID, socketstream sockets[]){
	int highestchoice = 0;
	int race;
	string username;
	string s;
	cout << "\n---Create your Avatar---\n" <<
			"Enter Your Name: ";
	getline(cin,username);
	username = username.substr(0, username.length());
	cout << "Choose Your Race:\n";
	cout <<	"\t(1) HUMAN : 2 hands, 5 fingers\n" <<
			"\t            2 feet,  5 toes\n" <<
			"\t            Skill: NONE\n";
	cout <<	"\t(2) ALIEN : 4 hands, 3 fingers\n" <<
			"\t            2 feet,  2 toes\n" <<
			"\t            Skill: IRONTOES - Losing feet does not make you skip a turn.\n";
	cout <<	"\t(3) ZOMBIE: 1 hands, 4 fingers\n" <<
			"\t            0 feet,  0 toes\n" <<
			"\t            Skill: VOODOO - Can act twice per turn. Regenerates once upon death.\n";
	cout <<	"\t(4) DOGGO : 0 hands, 0 fingers\n" <<
			"\t            4 feet,  4 toes\n" <<
			"\t            Skill: GOODBOYE - Will make attacking non-GOODBOYEs lose their next turn.\n";
	while(true){
		cout <<	"\n\nEnter your choice: ";
		cin >> s;
		cin.ignore();
		if(s != "1" && s != "2" && s != "3" && s != "4" )
			cout << "\n\nInput only the number of your choice.\n";
		else if(s == "1"){ race = 0; break;}
		else if(s == "2"){ race = 1; break;}
		else if(s == "3"){ race = 2; break;}
		else if(s == "4"){ race = 3; break;}
	}
	cout << "Lastly, select a team! You may only select up to Team " << PLAYERS <<".\n";
	bool team_select = true;
	int sum = 0;
	int team_choice = -1;
	while(team_select){
		highestchoice = 1;
		sum = 0;
		team_choice = -1;
		int choice;
		int choices[6] = {0,0,0,0,0,0};
		cout <<	"\n\nEnter your choice: ";
		cin >> s;
		cin.ignore();
		if(s != "1" && s != "2" && s != "3" && s != "4" && s != "5" && s != "6")
			cout << "\n\nInput only the number of your choice.\n";
		else if(s == "1") team_choice = 0;
		else if(s == "2") team_choice = 1;
		else if(s == "3" && PLAYERS >= 3) team_choice = 2;
		else if(s == "4" && PLAYERS >= 4) team_choice = 3;
		else if(s == "5" && PLAYERS >= 5) team_choice = 4;
		else if(s == "6" && PLAYERS == 6) team_choice = 5;
		
		cout << "You chose Team " << team_choice+1 << ".\nNow to wait for the rest of them...\n";
		if(team_choice >= -1){
			choice = team_choice;
			choices[choice]++;
			for(int i = 1; i < PLAYERS; i++){
				sockets[i] >> choice;
				sockets[i].ignore();
				choices[choice]++;
			}
			if(choices[0] > 0 && choices[1] > 0){
				sum = choices[0] + choices[1];
				if(sum == 0){
					cout << "Someone has to occupy Team 1.\n";
					continue;
				}
				for(int i = 2; i < 6; i++){
					if(choices[i] > 0 && choices[i-1] > 0){
						sum += choices[i];
						highestchoice = i;
					}
					else if(choices[i] == 0 && sum == PLAYERS){
						cout << "The alliances have been formed.\n";
						team_select = false;
						break;
					}
					else{
						break;
					}
				}
				if(sum == PLAYERS && team_select){
					cout << "The alliances have been formed.\n";
					team_select = false;
					break;
				}
				else if(team_select)
					cout << "It seems like you are scattered among teams. Prioritize low numbered teams over higher ones.\n";
			}
		}
		for(int i = 1; i < PLAYERS; i++){
			sockets[i] << team_select << endl;
		}
	}
	for(int i = 0; i <= highestchoice; i++){
		team_list.push_back(Team());
	}
	master_list.push_back(Player(username,ID,race,team_choice,team_list[team_choice].roster.size()));
	team_list[team_choice].player_is_alive.push_back(true);
	
	for(int i = 1; i < PLAYERS; i++){
		string userrname;
		int IDe, rac, team_choi;
		getline(sockets[i],userrname);
		sockets[i] >> IDe;
		sockets[i].ignore();
		sockets[i] >> rac;
		sockets[i].ignore();
		sockets[i] >> team_choi;
		sockets[i].ignore();
		master_list.push_back(Player(userrname,IDe,rac,team_choi,team_list[team_choi].roster.size()));
		team_list[team_choi].player_is_alive.push_back(true);
	}
}

void char_create_client(int ID, socketstream server[]){
	int race;
	string username;
	string s;
	cout << "\n---Create your Avatar---\n" <<
			"Enter Your Name: ";
	getline(cin,username);
	username = username.substr(0, username.length());
	cout << "Choose Your Race:\n";
	cout <<	"\t(1) HUMAN : 2 hands, 5 fingers\n" <<
			"\t            2 feet,  5 toes\n" <<
			"\t            Skill: NONE\n";
	cout <<	"\t(2) ALIEN : 4 hands, 3 fingers\n" <<
			"\t            2 feet,  2 toes\n" <<
			"\t            Skill: IRONTOES - Losing feet does not make you skip a turn.\n";
	cout <<	"\t(3) ZOMBIE: 1 hands, 4 fingers\n" <<
			"\t            0 feet,  0 toes\n" <<
			"\t            Skill: VOODOO - Can act twice per turn. Regenerates once upon death.\n";
	cout <<	"\t(4) DOGGO : 0 hands, 0 fingers\n" <<
			"\t            4 feet,  4 toes\n" <<
			"\t            Skill: GOODBOYE - Will make attacking non-GOODBOYEs lose their next turn.\n";
	while(true){
		cout <<	"\n\nEnter your choice: ";
		cin >> s;
		cin.ignore();
		if(s != "1" && s != "2" && s != "3" && s != "4")
			cout << "\n\nInput only the number of your choice.\n";
		else if(s == "1"){ race = 0; break;}
		else if(s == "2"){ race = 1; break;}
		else if(s == "3"){ race = 2; break;}
		else if(s == "4"){ race = 3; break;}
	}
	cout << "Lastly, select a team! You may only select up to Team " << PLAYERS <<".\n";
	bool team_select = true;
	int team_choice;
	while(team_select){
		team_choice = -1;
		cout <<	"\n\nEnter your choice: ";
		cin >> s;
		cin.ignore();
		if(s != "1" && s != "2" && s != "3" && s != "4" && s != "5" && s != "6")
			cout << "\n\nInput only the number of your choice.\n";
		else if(s == "1") team_choice = 0;
		else if(s == "2") team_choice = 1;
		else if(s == "3" && PLAYERS >= 3) team_choice = 2;
		else if(s == "4" && PLAYERS >= 4) team_choice = 3;
		else if(s == "5" && PLAYERS >= 5) team_choice = 4;
		else if(s == "6" && PLAYERS == 6) team_choice = 5;
		
		cout << "You chose Team " << team_choice+1 << ".\nNow to wait for the rest of them...\n";
		if(team_choice >= -1){
			server[0] << team_choice << endl;
			server[0] >> team_select;
			server[0].ignore();
			if(team_select)
				cout << "It seems like you are scattered among teams. Prioritize low numbered teams over higher ones.\n";
			else
				cout << "The alliances have been formed.\n";
		}
	}
	server[0] << username << endl;
	server[0] << ID << endl;
	server[0] << race << endl;
	server[0] << team_choice << endl;
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
		sockets[i] << PLAYERS << endl;
    }

	cout << "\nAll players have successfully connected.\n";

	char_create_server(0, sockets);
	cout << "finished charcreation";
}

void runClient(int argc, char* argv[]) {
	bool event_attack = false;				//turns true when a player, including this player, attacks
	bool event_distribute = false;			//turns true when a player, including this player, disthands
	bool event_turn_ready = false;			//turns true when it's this player's turns
    socketstream server[2];
    cout << "Travelling to the arena...\n";

    server[0].open(argv[1], argv[2]);

    int player_ID;
    cout << "Getting ready for battle...\n";
    server[0] >> player_ID;
	server[0].ignore();
    server[0] >> PLAYERS;
	server[0].ignore();

    cout << "\n\nWelcome to S U P E R Chopsticks X X X T R E M E, Player " << player_ID+1 << ".\n" <<
			"\n\nAll players have successfully connected.\n";

	char_create_client(player_ID, server);
	cout << "finished charcreation";
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
//cd Documents\sce
//chopsticks_multiplayer 2555
//chopsticks_multiplayer localhost 2555
