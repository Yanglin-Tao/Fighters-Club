//
//  main.cpp
//  hw02
//
//  Created by Yanglin Tao on 9/25/21.
//  Program to model a game of warrior fighting.

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

//create a struct of Warrior
struct Warrior{
    string warrior_name;
    int warrior_strength;
};

//function prototypes;
void display_warriors(vector<Warrior> vector_warriors);
void battle(Warrior& warrior_1, Warrior& warrior_2);
int warrior_loc(const string name, vector<Warrior> vector_warriors);

int main(int argc, const char * argv[]) {
    // open file warriors.txt
    ifstream warriors("warriors.txt");
    if(!warriors){
        cout << "Could not open file.\n";
        exit(1);
    }
    
    // create a vector of warriors
    vector<Warrior> vector_warriors;
    
    string warrior_string = "Warrior";
    string name;
    int strength;
    
    //fill in the vector of warriors
    while(warriors >> warrior_string >> name >> strength){
        Warrior new_warrior;
        new_warrior.warrior_name = name;
        new_warrior.warrior_strength = strength;
        vector_warriors.push_back(new_warrior);
    }
    
    warriors.close();
    
    //reopen and make a copy of file warriors.txt to retrieve the commands
    ifstream warriors_copy("warriors.txt");
    if(!warriors_copy){
        cout << "Could not open file.\n";
        exit(1);
    }

    string status = "Status";
    string battle = "Battle";
    
    //create a vector of commands
    vector<string> vector_cmds;
    string command;
    
    // fill in vector of commands
    while(warriors_copy >> command){
        vector_cmds.push_back(command);
    }
    
    // excecute each command
    for(size_t i = 0; i < vector_cmds.size(); i++){
        if(vector_cmds[i] == status){
            display_warriors(vector_warriors);
        }
        if(vector_cmds[i] == battle){
            ::battle(vector_warriors[warrior_loc(vector_cmds[i+1], vector_warriors)], vector_warriors[warrior_loc(vector_cmds[i+2], vector_warriors)]);
        }
    }
    warriors_copy.close();
    
}

// function to display the status of warriors
void display_warriors(const vector<Warrior> vector_warriors){
    cout << "There are: " << vector_warriors.size() << " warriors\n";
    for(size_t i = 0; i < vector_warriors.size(); i++){
        cout << "Warrior: " << vector_warriors[i].warrior_name << ", ";
        cout << "strength: " << vector_warriors[i].warrior_strength << endl;
    }
}

// function to execute the battle command
void battle(Warrior& warrior_1, Warrior& warrior_2){
    cout << warrior_1.warrior_name << " battles " << warrior_2.warrior_name << endl;
    // case 1: both has same strength
    if(warrior_1.warrior_strength == warrior_2.warrior_strength){
        // special case: both strength = 0
        if(warrior_1.warrior_strength == 0 && warrior_2.warrior_strength == 0){
            cout << "Oh, NO! They're both dead! Yuck!" << endl;
            return;
        }
        cout << "Mutual Annihilation: " << warrior_1.warrior_name;
        cout << " and " << warrior_2.warrior_name;
        cout << " die at each other's hands" << endl;
        warrior_1.warrior_strength = 0;
        warrior_2.warrior_strength = 0;
        return;
    }
    // case 2: warrior_1 won
    else if(warrior_1.warrior_strength > warrior_2.warrior_strength){
        if(warrior_2.warrior_strength == 0){
            cout << "He's dead, " << warrior_1.warrior_name << endl;
            return;
        }
        cout << warrior_1.warrior_name << " defeats " << warrior_2.warrior_name << endl;
        warrior_1.warrior_strength -= warrior_2.warrior_strength;
        warrior_2.warrior_strength = 0;
        return;
    }
    // case 3: warrior_2 won
    else{
        if(warrior_1.warrior_strength == 0){
            cout << "He's dead, " << warrior_2.warrior_name;
            return;
        }
        cout << warrior_2.warrior_name << " defeats " << warrior_2.warrior_name << endl;
        warrior_2.warrior_strength -= warrior_1.warrior_strength;
        warrior_1.warrior_strength = 0;
        return;
    }
}

//function to locate warriors in the collection of warriors
int warrior_loc(const string name, vector<Warrior> vector_warriors){
    for(int i = 0; i < vector_warriors.size(); i++){
        if(vector_warriors[i].warrior_name == name){
            return i;
        }
    }
    return vector_warriors.size();
    
}
