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
    //std::uniform_int_distribution indexGen {0, global::wordPool.size()}; //TODO: This range should be dependent on the length of the word list

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
        ss << global::wordPool.at(i);

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
    
    if (*argv[1] != '1' && *argv[1] != '2'){
        std::cerr << "Error: Password type must be either '1' or '2'.\n";
        exit(1);
    }
    
    // 1 for passphrase
    // 2 for passkey
    int passwordType {std::stoi(argv[1])};

    global::minLength = (passwordType == 1)? 10: 20;
    global::maxLength = (passwordType == 1)? 40: 128;
    
    if (!isNumber(static_cast<std::string>(argv[2])) || std::stoi(argv[2]) > global::maxLength || std::stoi(argv[2]) < global::minLength) {
        std::cerr << "Error: Password length must be between " << global::minLength << " and " << global::maxLength << " (inclusively) for " << (passwordType == 1? "passphrases.\n": "pass.");
        exit(1);
    }
    
    // word count for "passphrase" type
    // character count for "passkey" type
    int passwordLength {std::stoi(argv[2])};

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