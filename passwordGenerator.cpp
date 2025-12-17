#include <iostream>
#include <random>
#include <sstream>
#include <fstream>
#include <vector>

//TODO: implement cli arguments capabilites
//TODO: add entropy calculator
//TODO: add return objects to "generatePassphrase" and "generatePasskey" functions

namespace global{
    std::ifstream defaultWordList{"defaultWordList.txt"};
    std::vector<std::string> wordPool{};

    void loadWordPool(std::ifstream& wordList = global::defaultWordList){
        std::string tmp;
        while (std::getline(wordList, tmp)){
            global::wordPool.push_back(tmp);
        }
    }
    
    std::mt19937 generateSeededMT(){
        std::random_device rd{};
        std::seed_seq ss{rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd()};

        return std::mt19937 {ss};
    }

    std::mt19937 mt {generateSeededMT()};

    //TODO: make this initialized run time based on the length of the wordList
    std::uniform_int_distribution indexGen {0, 1000 - 1}; //TODO: This range should be dependent on the length of the word list

    std::uniform_int_distribution<int> charGen {33, 126};

    // maximum length of password (derived from keepassxc)
    int maxLength{};
    
    // minimum length of password (derived from keepassxc)
    int minLength{};
}

std::string generatePassphrase(int wordCount){
    std::string passphrase;

    //easier to use than a string array
    std::stringstream ss{};

    for (int i {}; i < wordCount; i++){
        ss << global::wordPool.at(global::indexGen(global::mt));

        if (i != wordCount - 1) ss << ' ';
    }

    std::getline(ss, passphrase);
    return passphrase;
}

std::string generatePasskey(int length){
    std::string passkey;

    // i dont want to use a character array
    std::stringstream ss;

    for (int i{0}; i < length; i++){
        ss << static_cast<char>(global::charGen(global::mt));
    }

    // using  getline and not operator>> incase a character in the stream is a space
    std::getline(ss, passkey);

    return passkey;
}

bool isNumber(std::string str){
    for (char ch : str){
        if (!std::isdigit(ch)) return false;
    }
    
    return true;
}

int main(int argc, char *argv[]){
    // 1 for passphrase
    // 2 for passkey
    int passwordType{}; 
    
    while (true) {
    // this variable is created inside the loop block because it is temporary
    // it is assigned and not initialized because if it was assigned inside the loop the program would attempt to initialize it at the start of every loop
    std::string input = ""; // this is a string (and not a char) to prevent std::cin issues for multicharacter input

    std::cout << "Password Type Options\n-----------------------\n1.) Passphrase (words only)\n2.) Passkey (letters, symbols, and numbers)\n\nEnter an option: ";
    std::cin >> input;
    
    // exit condition
    if (input == "1" || input == "2") {passwordType = std::stoi(input); break;}
    
    system("clear");
    }

    global::minLength = (passwordType == 1)? 10: 20;
    global::maxLength = (passwordType == 1)? 40: 128;

    // word count for "passphrase" type
    // character count for "passkey" type
    int passwordLength{};

    while (true) {
    // this variable is created inside the loop block because it is temporary
    // it is assigned and not initialized because if it was assigned inside the loop the program would attempt to initialize it at the start of every loop
    std::string input = ""; // this is a string (and not a char) to prevent std::cin issues for multicharacter input

    std::cout << "Password length (" << global::minLength << " to " << global::maxLength << " inclusive): ";
    std::cin >> input;

    // exit condition
    if (isNumber(input) && (std::stoi(input) >= global::minLength && std::stoi(input) <= global::maxLength)) {passwordLength = std::stoi(input); break;}
    
    system("clear");

    // this is only here to make it look like only the password length prompt is being refreshed
    std::cout << "Password Type Options\n-----------------------\n1.) Passphrase (words only)\n2.) Passkey (letters, symbols, and numbers)\n\nEnter an option: " << passwordType << '\n';
    }

    std::string password {};

    switch (passwordType){
        case 1:
            //TODO: add a way to input a custom word list
            global::loadWordPool();
            password = generatePassphrase(passwordLength);
            break;
        case 2:
            password = generatePasskey(passwordLength);
            break;
    }

    system("clear");
    std::cout << "Password: \"" << password << "\"\n";

    return 0;
}