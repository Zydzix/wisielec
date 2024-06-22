#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <locale>

std::vector<std::string> loadWordsFromFile(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<std::string> words;
    std::string word;

    if (file.is_open()) {
        while (std::getline(file, word)) {
            words.push_back(word);
        }
        file.close();
    }
    else {
        std::cerr << "Error opening file: " << filename << std::endl;
    }

    return words;
}

void input(char& guess) {
    std::cout << "Enter your guess: ";
    std::cin >> guess;
    guess = tolower(guess);
}

void update(const std::string& secretWord, std::string& guessedWord, char guess, int& remainingAttempts, std::vector<char>& wrongGuesses) {
    bool correctGuess = false;
    for (size_t i = 0; i < secretWord.size(); ++i) {
        if (tolower(secretWord[i]) == guess) {
            guessedWord[i] = secretWord[i];
            correctGuess = true;
        }
    }

    if (!correctGuess) {
        wrongGuesses.push_back(guess);
        --remainingAttempts;
    }
}

void drawHangman(int remainingAttempts) {
    switch (remainingAttempts) {
    case 6:
        std::cout << "  +---+\n"
            "  |   |\n"
            "      |\n"
            "      |\n"
            "      |\n"
            "      |\n"
            "=========\n";
        break;
    case 5:
        std::cout << "  +---+\n"
            "  |   |\n"
            "  O   |\n"
            "      |\n"
            "      |\n"
            "      |\n"
            "=========\n";
        break;
    case 4:
        std::cout << "  +---+\n"
            "  |   |\n"
            "  O   |\n"
            "  |   |\n"
            "      |\n"
            "      |\n"
            "=========\n";
        break;
    case 3:
        std::cout << "  +---+\n"
            "  |   |\n"
            "  O   |\n"
            " /|   |\n"
            "      |\n"
            "      |\n"
            "=========\n";
        break;
    case 2:
        std::cout << "  +---+\n"
            "  |   |\n"
            "  O   |\n"
            " /|\  |\n"
            "      |\n"
            "      |\n"
            "=========\n";
        break;
    case 1:
        std::cout << "  +---+\n"
            "  |   |\n"
            "  O   |\n"
            " /|\  |\n"
            " /    |\n"
            "      |\n"
            "=========\n";
        break;
    case 0:
        std::cout << "  +---+\n"
            "  |   |\n"
            "  O   |\n"
            " /|\  |\n"
            " / \  |\n"
            "      |\n"
            "=========\n";
        break;
    }
}

void render(const std::string& guessedWord, const std::vector<char>& wrongGuesses, int remainingAttempts) {
    drawHangman(remainingAttempts);
    std::cout << "Current word: " << guessedWord << std::endl;
    std::cout << "Wrong guesses: ";
    for (char c : wrongGuesses) {
        std::cout << c << " ";
    }
    std::cout << std::endl;
    std::cout << "Remaining attempts: " << remainingAttempts << std::endl;
}

int main() {
    std::locale::global(std::locale("en_US.UTF-8"));

    std::srand(std::time(0));

    std::vector<std::string> words = loadWordsFromFile("words.txt");
    if (words.empty()) {
        std::cerr << "No words loaded. Exiting." << std::endl;
        return 1;
    }

    const std::string secretWord = words[std::rand() % words.size()];
    std::string guessedWord(secretWord.size(), '_');
    int remainingAttempts = 6;
    std::vector<char> wrongGuesses;

    while (remainingAttempts > 0 && guessedWord != secretWord) {
        char guess;
        input(guess);
        update(secretWord, guessedWord, guess, remainingAttempts, wrongGuesses);
        render(guessedWord, wrongGuesses, remainingAttempts);
    }

    if (guessedWord == secretWord) {
        std::cout << "Congratulations! You guessed the word: " << secretWord << std::endl;
    }
    else {
        std::cout << "Game over! The word was: " << secretWord << std::endl;
    }

    return 0;
}
