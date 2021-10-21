//
//  main.cpp
//  hw05
//
//  Created by Yanglin Tao on 10/14/21.
//  Read a file of commands to create Nobles and Warriors using dynamic memory,
//  and sending them off to battle.

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

//create a warrior class
class Warrior{
    friend ostream& operator << (ostream& os, const Warrior& someWarrior);
public:
    Warrior(const string& warrName, double warrStrength)
    : warriorName(warrName),
    warriorStrength(warrStrength){
    }

    // get warrior name for location checking
    const string& getWarriorName() const {
        return warriorName;
    }
    // get warrior strength
    double getWarriorStrength() const {
        return warriorStrength;
    }
    
    // set warrior strength
    void setWarriorStrength(double ratio){
        warriorStrength = warriorStrength*(1-ratio);
    }
    
    // get warrior hiring status, true or false
    bool getWarriorHireStatus() const {
        return warriorHireStatus;
    }
    
    // set warrior hiring status, true or false
    void setWarriorHireStatus(bool status){
        warriorHireStatus = status;
    }
    
private:
    string warriorName;
    double warriorStrength;
    bool warriorHireStatus = false;
};

// create the noble class
class Noble{
    friend ostream& operator << (ostream& os, const Noble& someNoble);
public:
    Noble(const string& nobName) : nobleName(nobName){
    }
    
    // get noble name
    const string& getNobleName() const{
        return nobleName;
    }
    
    // method to hire a warrior
    void hire(Warrior& addWarrior){
        // the warrior must be unhired and noble must be alive
        if(addWarrior.getWarriorHireStatus() == false
           && nobleStatus == true){
            nobleArmy.push_back(&addWarrior);
            addWarrior.setWarriorHireStatus(true);
            //return true;
        }
        else{
            cout << "Hire warrior failed: warrior " << addWarrior.getWarriorName()
            << " is already hired." << endl;
        }
        //return false;
    }
    
    // check if warrior belongs to the army
    bool warriorInArmy(const Warrior& warrior) const{
        for(size_t i = 0; i < nobleArmy.size(); i++){
            if(nobleArmy[i] == &warrior){
                return true;
            }
        }
        return false;
    }
    
    // method to fire a warrior
    void fire(Warrior& deleteWarrior){
        // the warrior must be in noble's army
        if(!warriorInArmy(deleteWarrior)){
            cout << "Fire warrior failed: warrior " <<
            deleteWarrior.getWarriorName() << "does not belong to noble's army."
            << endl;
        }
        // if warrior in noble's army
        else{
            for(size_t i = 0; i < nobleArmy.size(); i++){
                // the warrior must be hired and noble should be alive
                if(nobleArmy[i] == &deleteWarrior){
                    if(deleteWarrior.getWarriorHireStatus() == true &&
                       nobleStatus == true){
                        cout << "You don't work for me anymore " <<
                        deleteWarrior.getWarriorName()
                        << "! -- " << nobleName <<"." << endl;
                        // remove warrior from noble army while maintaining order
                        for(size_t j = i; j < nobleArmy.size(); j++){
                            Warrior* latterptr = nobleArmy[j + 1];
                            nobleArmy[j] = latterptr;
                        }
                        nobleArmy.pop_back();
                        deleteWarrior.setWarriorHireStatus(false);
                    }
                    break;
                }
            }
        }
    }
    
    // get army total strength
    double getArmyStrength() const{
        double armyStrength = 0;
        for(size_t i = 0; i < nobleArmy.size(); i++){
            armyStrength += nobleArmy[i]->getWarriorStrength();
        }
        return armyStrength;
    }
    
    // set army strength by ratio
    void setArmyStrength(double ratio){
        for(size_t i = 0; i < nobleArmy.size(); i++){
            nobleArmy[i]->setWarriorStrength(ratio);
        }
        // if ratio is 1, noble is dead
        if(ratio == 1){
            nobleStatus = false;
        }
    }
    
    // method to start the battle
    void battle(Noble& noble2){
        cout << nobleName << " battles " << noble2.nobleName << endl;
        // case 1: noble and noble2 has same strength
        if(getArmyStrength() == noble2.getArmyStrength()){
            if(nobleStatus == false && noble2.nobleStatus == false){
                cout << "Oh, NO!  They're both dead!  Yuck!" << endl;
            }
            else{
                cout << "Mutual Annihalation: " << nobleName << " and "
                << noble2.nobleName << " die at each other's hands" << endl;
                setArmyStrength(1);
                noble2.setArmyStrength(1);
            }

        }
        
        // case 2: noble is dead
        else if(nobleStatus == false){
            cout << "He is dead, " << noble2.nobleName << endl;
        }
        
        // case 3: noble2 is dead
        else if(noble2.nobleStatus == false){
            cout << "He is dead, " << nobleName << endl;
        }
        
        // case 4: noble won
        else if(getArmyStrength() > noble2.getArmyStrength()){
            cout << nobleName << " defeats " << noble2.nobleName << endl;
            double ratio = noble2.getArmyStrength()/getArmyStrength();
            setArmyStrength(ratio);
            noble2.setArmyStrength(1);
        }
        
        // case 5: noble2 won
        else if(getArmyStrength() < noble2.getArmyStrength()){
            cout << noble2.nobleName << " defeats " << nobleName << endl;
            double ratio = getArmyStrength()/noble2.getArmyStrength();
            noble2.setArmyStrength(ratio);
            setArmyStrength(1);
        }
    }
    
private:
    string nobleName;
    vector<Warrior*> nobleArmy;
    bool nobleStatus = true;
};

// output operator for Warrior class
ostream& operator << (ostream& os, const Warrior& someWarrior){
    os << "     " << someWarrior.warriorName << ": "
    << someWarrior.warriorStrength;
    return os;
}

// output operator for Noble class
ostream& operator << (ostream& os, const Noble& someNoble){
    os << someNoble.nobleName << " has an army of "
    << someNoble.nobleArmy.size() << endl;
    for(size_t i = 0; i < someNoble.nobleArmy.size(); i++){
        cout << *someNoble.nobleArmy[i] << endl;
    }
    return os;
}

void clearWarriors(vector<Warrior*>& vectorWarriors);
void clearNobles(vector<Noble*>& vectorNobles);
bool findWarrior(const vector<Warrior*>& vectorWarriors, const string& warriorName);
bool findNoble(const vector<Noble*>& vectorNobles, const string& nobleName);
void addNoble(vector<Noble*>& vectorNobles, const string& nobleName);
void addWarrior(vector<Warrior*>& vectorWarriors, const string& warriorName,
                double warriorStrength);

// function to clear warriors
void clearWarriors(vector<Warrior*>& vectorWarriors){
    for(size_t i = 0; i < vectorWarriors.size(); i++){
        delete vectorWarriors[i];
    }
    vectorWarriors.clear();
}

// function to clear nobles
void clearNobles(vector<Noble*>& vectorNobles){
    for(size_t i = 0; i < vectorNobles.size(); i++){
        delete vectorNobles[i];
    }
    vectorNobles.clear();
}

// function to check whether warrior exists
bool findWarrior(const vector<Warrior*>& vectorWarriors,
                 const string& warriorName){
    for(size_t i = 0; i < vectorWarriors.size(); i++){
        if(vectorWarriors[i] -> getWarriorName() == warriorName){
            return true;
        }
    }
    return false;
}

// function to check whether noble exists
bool findNoble(const vector<Noble*>& vectorNobles, const string& nobleName){
    for(size_t i = 0; i < vectorNobles.size(); i++){
        if(vectorNobles[i] -> getNobleName() == nobleName){
            return true;
        }
    }
    return false;
}

// function to create noble on heap
void addNoble(vector<Noble*>& vectorNobles, const string& nobleName){
    Noble* noblePtr = new Noble(nobleName);
    vectorNobles.push_back(noblePtr);
}

// function to create warrior on heap
void addWarrior(vector<Warrior*>& vectorWarriors, const string& warriorName,
                double warriorStrength){
    Warrior* warriorPtr = new Warrior(warriorName, warriorStrength);
    vectorWarriors.push_back(warriorPtr);
}


int main(int argc, const char * argv[]) {
    // insert code here...
    vector<Warrior*> vectorWarriors;
    vector<Noble*> vectorNobles;
    // test h-cases
    // ifstream nobleWarriors("hcases.txt");
    ifstream nobleWarriors("nobleWarriors.txt");
    if(!nobleWarriors){
        cout << "Could not open file.\n";
        exit(1);
    }
    string nobleName;
    string warriorName;
    double warriorStrength;
    string nobleHire;
    string warriorHire;
    string nobleFire;
    string warriorFire;
    string noble1;
    string noble2;
    
    string command;
    while(nobleWarriors >> command){
        if(command == "Noble"){
            nobleWarriors >> nobleName;
            // noble must not already exist
            if(findNoble(vectorNobles, nobleName)){
                cout << "Create noble failed: noble " << nobleName <<
                " already exists." << endl;
            }
            else{
                addNoble(vectorNobles, nobleName);
            }
        }
        else if(command == "Warrior"){
            nobleWarriors >> warriorName;
            // warrior must not already exist
            nobleWarriors >> warriorStrength;
            if(findWarrior(vectorWarriors, warriorName)){
                cout << "Create warrior failed: warrior " << warriorName <<
                " already exists." << endl;
            }
            else{
                addWarrior(vectorWarriors, warriorName, warriorStrength);
            }
        }
   
        else if(command == "Status"){
            cout << "Status" << endl;
            cout << "======" << endl;
            cout << "Nobles: " << endl;
            // if all nobles are cleared
            if(vectorNobles.size() == 0){
                cout << "NONE" << endl;
            }
            else{
                for(size_t i = 0; i < vectorNobles.size(); i++){
                    cout << *vectorNobles[i];
                }
            }
            cout << "Unemployed Warriors:" << endl;
            // if all warriors are employed
            size_t count = 0;
            if(vectorWarriors.size() != 0){
                for(size_t i = 0; i < vectorWarriors.size(); i++){
                    if(vectorWarriors[i] -> getWarriorHireStatus() == true){
                        count ++;
                    }
                }
                if(count == vectorWarriors.size()){
                    cout << "NONE" << endl;
                }
            }
            
            if(vectorWarriors.size() != 0){
                for(size_t i = 0; i < vectorWarriors.size(); i++){
                    if(vectorWarriors[i] -> getWarriorHireStatus() == false){
                        cout << *vectorWarriors[i] << endl;
                    }
                }
            }
            else{
                // if all warriors are cleared
                cout << "NONE" << endl;
            }
        }
    
        else if(command == "Hire"){
            nobleWarriors >> nobleHire;
            nobleWarriors >> warriorHire;
            // noble must exist to hire
            if(!findNoble(vectorNobles, nobleHire)){
                cout << "Hire warrior failed: noble " << nobleHire <<
                " does not exist." << endl;
            }
            // warrior must exist to be hired
            else if(!findWarrior(vectorWarriors, warriorHire)){
                cout << "Hire warrior failed: warrior " << warriorHire <<
                " does not exist." << endl;
            }
            else{
                for(size_t i = 0; i < vectorNobles.size(); i++){
                    if(vectorNobles[i] -> getNobleName() == nobleHire){
                        for(size_t j = 0; j < vectorWarriors.size(); j++){
                            if(vectorWarriors[j] -> getWarriorName() ==
                               warriorHire){
                                vectorNobles[i] -> hire(*vectorWarriors[j]);
                                break;
                            }
                        }
                        break;
                    }
                }
            }
        }
        
        else if(command == "Fire"){
            nobleWarriors >> nobleFire;
            nobleWarriors >> warriorFire;
            // noble must exist to fire
            if(!findNoble(vectorNobles, nobleFire)){
                cout << "Fire warrior failed: noble " << nobleFire <<
                " does not exist." << endl;
            }
            // warrior must exist to be fired
            if(!findWarrior(vectorWarriors, warriorFire)){
                cout << "Fire warrior failed: warrior " << warriorFire <<
                " does not exist." << endl;
            }
            else{
                for(size_t i = 0; i < vectorNobles.size(); i++){
                    if(vectorNobles[i] -> getNobleName() == nobleFire){
                        for(size_t j = 0; j < vectorWarriors.size(); j++){
                            if(vectorWarriors[j] -> getWarriorName() ==
                               warriorFire){
                                vectorNobles[i] -> fire(*vectorWarriors[j]);
                                break;
                            }
                        }
                        break;
                    }
                }
            }
        }
        else if(command == "Battle"){
            nobleWarriors >> noble1;
            nobleWarriors >> noble2;
            // noble 1 must exist to battle
            if(!findNoble(vectorNobles, noble1)){
                cout << "Battle failed: " << noble1 << " does not exist." << endl;
            }
            // noble 2 must exist to battle
            else if(!findNoble(vectorNobles, noble2)){
                cout << "Battle failed: " << noble2 << " does not exist." << endl;
            }
            else{
                for(size_t i = 0; i < vectorNobles.size(); i++){
                    if(vectorNobles[i] -> getNobleName() == noble1){
                        for(size_t j = 0; j < vectorNobles.size(); j++){
                            if(vectorNobles[j] -> getNobleName() == noble2){
                                vectorNobles[i] -> battle(*vectorNobles[j]);
                                break;
                            }
                        }
                        break;
                    }
                    
                }
            }
        }
        // clear all warriors and nobles
        else if(command == "Clear"){
            clearWarriors(vectorWarriors);
            clearNobles(vectorNobles);
        }
    }
    nobleWarriors.close();
    return 0;
}

