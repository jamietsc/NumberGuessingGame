//
// Created by sauser on 24.12.2024.
//
#include <iostream>
#include <cstdlib>
#include <limits>
#include <string>
#include <fstream>
#include <random>
#include <algorithm>
#include <utility>
#include "../includes/UserInput.h"

using namespace std;

bool checkNumber(int input, int x);
bool playingAgain();
string inputOfTheUsername();
void readUsernameAndHighscore();
void saveNewUsernameAndHighscore(const vector<pair<string, int>>& highscores);
void checkIfUserHasNewHighscore(const string& username);
bool isEmpty(ifstream& myFile);
int creatingMenu();

int trys;

/**
 * the main function for the number guessing game
 * @return 1 if the user wants to exit ||0 if the user wants to go back to the main menu
 */
bool mainFunctionNumberGuessing() {
    bool endNumberGuessingGame = false;// variable to end the loop
    int input, //input is for saving the guess of the user. trys for counting the trys the user needed
    x, //x is the random number between 1 and 100
    decision; //decision is for the menu of the game
    string username;

    trys = 0;

    username = inputOfTheUsername();

    do {
        decision = creatingMenu();

        switch (decision) {
            case 0: {
                cout << "Goodbye!" << endl;
                return true;
            }
            case 1: {
                while (!endNumberGuessingGame){
                    mt19937 rng(random_device{}());
                    uniform_int_distribution<int> dist(1, 100);
                    x = dist(rng);

                    cout << "The number is set between 1 and 100, you can write your answer now." << endl;
                    while (!endNumberGuessingGame) {
                        input = checkUserInputInteger();
                        if (input > 0) {
                            endNumberGuessingGame = checkNumber(input, x);
                        }
                    }
                    checkIfUserHasNewHighscore(username);
                    endNumberGuessingGame = playingAgain();
                }
            }
            case 2: {
                readUsernameAndHighscore();
                break;
            }
            case 3: {
                return false;
            }
            default: {
                cout << "Your input was not accpted by the program, please try again." << endl;
            }
        }
        endNumberGuessingGame = false;
    } while (!endNumberGuessingGame);
    return 0;
}

/**
 * function to check the number from the user
 * @param input the input from the user
 * @param x the number that has to be guess
 * @return true if correct, false if not correct
 */
bool checkNumber(int input, int x) {
    trys++;
    if(input < x){
        cout << "The random number is bigger than your input." << endl;
        cout << "Please guess again: " << endl;
        return false;
    } else if(input > x){
        cout << "The random number is smaller than your input." << endl;
        cout << "Please guess again: " << endl;
        return false;
    } else if(input == x){
        cout << "Congratulations, you guessed the right number." << endl;
        cout << "You needed " << trys << " trys to guess the right number." << endl;
        return true;
    } else {
        cout << "An error has occurred, the program will shut down." << endl;
        return true;
    }
}

/**
 * function for the user to decide if he wants to play again or not
 * @return false if he wants to play again; true if he wants to stop
 */
bool playingAgain() {
    char decision;

    cout << "Do you want to play again? (yes = y; no = n)" << endl;

    while (true) {
        decision = checkUserInputChar();

        if (decision == 'y' || decision == 'Y') {
            trys = 0;
            return false;
        } else if (decision == 'n' || decision == 'N') {
            return true;
        } else {
            cout << "This letter was not accepted by the program. Please try again: " << endl;
        }
    }
}

/**
 * function to insert the username and accept it
 * @return the username to the function call
 */
string inputOfTheUsername(){
    string username;
    char decision;

    while(true){
        cout << "Please insert your name." << endl;
        cin >> username;
        cout << "Your inserted username is: " << username << ". Do you want to play with it (Yes = y; No = n)" << endl;
        while (true) {
            decision = checkUserInputChar();
            if (decision == 'y' || decision == 'Y') {
                return username;
            } else if (decision == 'n' || decision == 'N') {
                break;
            } else {
                cout << "This letter was not accepted by the program. Please try again: " << endl;
            }
        }
    }
}

/**
 * function to read the username and his highscore from the highscore.txt file
 */
void readUsernameAndHighscore() {
    string bestUser[6];
    ifstream myFile ("..\\highscore.txt");

    if(isEmpty(myFile)){
        cout << "Wow, you are the first one who is playing the game. There was no one before you who could be better!!" << endl;
        return;
    }

    if(myFile.is_open()) {
        for(int i = 0; i < 6; i++){
            if(!getline(myFile, bestUser[i])) {
                cout << "Error while reading the file" << endl;
            }
        }
        cout << "The first place was: " << bestUser[0] << " with " << bestUser[1] << " trys to guess the right number." << endl;
        cout << "The second place was: " << bestUser[2] << " with " << bestUser[3] << " trys to guess the right number." << endl;
        cout << "The third place was: " << bestUser[4] << " with " << bestUser[5] << " trys to guess the right number." << endl;
        myFile.clear();
    } else {
        cout << "Unable to open the highscore file." << endl;
    }
}

/**
 * function to save the new username and highscore in the text file.
 * @param username username which should be saved
 */
void saveNewUsernameAndHighscore(const vector<pair<string, int>>& highscores) {
    ofstream myFile ("..\\highscore.txt");

    if(myFile.is_open()){
        for(const auto& entry : highscores ) {
            myFile << entry.first << "\n" << entry.second << "\n";
        }
        myFile.close();
        cout << "Highscores got updated successfully." << endl;
    } else {
        cout << "Error: The file could not be open." << endl;
    }
}

/**
 * function to check if the user got the new highscore
 * @param username if the user got the highscore the function calls a function where the new username is needed
 */
void checkIfUserHasNewHighscore(const string& username) {
    vector<pair<string, int>> highscores;
    ifstream myFile("..\\highscore.txt");

    if(myFile.is_open()) {
        string line;
        while (getline(myFile, line)) {
            string user = line;
            if(getline(myFile, line)) {
                int score = atoi(line.c_str());
                highscores.emplace_back(user, score);
            }
        }
        myFile.close();
    }

    // sort every player if needed
    sort(highscores.begin(), highscores.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
        return a.second < b.second;
    });

    // Player reached the highscore
    if (highscores.empty() || trys < highscores.front().second) {
        cout << "Congratulations you reached a new highscore" << endl;
        highscores.insert(highscores.begin(), make_pair(username, trys));
    } else {
        // Add the player to the right position
        bool added = false;
        for (auto it = highscores.begin(); it != highscores.end(); ++it) {
            if (trys < it->second) {
                highscores.insert(it, make_pair(username, trys));
                added = true;
                break;
            }
        }

        // If the player is worse than the first three
        if (!added && highscores.size() < 3) {
            highscores.emplace_back(username, trys);
        }
    }

    // save only the top three
    if (highscores.size() > 3) {
        highscores.resize(3);
    }

    saveNewUsernameAndHighscore(highscores);
}

/**
 * Function to check if an excisting file is empty
 * @param myFile the path of the file
 * @return true if its empty || false if its not empty
 */
bool isEmpty(ifstream& myFile) {
    return myFile.peek() == ifstream::traits_type::eof();
}

int creatingMenu(){
    cout << "Please choose what do you want to do:" << endl;
    cout << "---------- 0. End the game ----------" << endl;
    cout << "---------- 1. Play the game ---------" << endl;
    cout << "------- 2. Show the highscore -------" << endl;
    cout << "------- 3. Return to main menu ------" << endl;

    return checkUserInputInteger();
}



