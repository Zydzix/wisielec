#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <locale>

#ifdef _WIN32
#include <cstdlib>
#define CLEAR_SCREEN "cls"
#else
#include <cstdlib>
#define CLEAR_SCREEN "clear"
#endif

// Function to load words from a file
std::vector<std::wstring> loadWordsFromFile(const std::string& filename) {
    std::vector<std::wstring> words;
    std::wifstream file(filename);
    if (file.is_open()) {
        std::wstring word;
        while (std::getline(file, word)) {
            words.push_back(word);
        }
        file.close();
    }
    else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
    return words;
}

// Function to draw the hangman based on the number of wrong attempts
void drawHangman(int wrongGuessCount) {
    switch (wrongGuessCount) {
    case 0:
        std::wcout << "  +---+\n"
            "      |\n"
            "      |\n"
            "      |\n"
            "      |\n"
            "      |\n"
            "=========" << std::endl;
        break;
    case 1:
        std::wcout << "  +---+\n"
            "  |   |\n"
            "      |\n"
            "      |\n"
            "      |\n"
            "      |\n"
            "=========" << std::endl;
        break;
    case 2:
        std::wcout << "  +---+\n"
            "  |   |\n"
            "  O   |\n"
            "      |\n"
            "      |\n"
            "      |\n"
            "=========" << std::endl;
        break;
    case 3:
        std::wcout << "  +---+\n"
            "  |   |\n"
            "  O   |\n"
            "  |   |\n"
            "      |\n"
            "      |\n"
            "=========" << std::endl;
        break;
    case 4:
        std::wcout << "  +---+\n"
            "  |   |\n"
            "  O   |\n"
            " /|  |\n"
            "      |\n"
            "      |\n"
            "=========" << std::endl;
        break;
    case 5:
        std::wcout << "  +---+\n"
            "  |   |\n"
            "  O   |\n"
            " /|\\  |\n"
            "      |\n"
            "      |\n"
            "=========" << std::endl;
        break;
    case 6:
        std::wcout << "  +---+\n"
            "  |   |\n"
            "  O   |\n"
            " /|\\  |\n"
            " /    |\n"
            "      |\n"
            "=========" << std::endl;
        break;
    case 7:
        std::wcout << "  +---+\n"
            "  |   |\n"
            "  O   |\n"
            " /|\\  |\n"
            " / \\  |\n"
            "      |\n"
            "=========" << std::endl;
        break;
    default:
        std::wcout << "Invalid hangman state!" << std::endl;
        break;
    }
}

// Function to check if a character is an alphabet letter
bool isAlphabetLetter(wchar_t letter) {
    return (letter >= L'a' && letter <= L'z') || (letter >= L'A' && letter <= L'Z');
}

// Function to convert a character to lowercase
wchar_t toLowerCase(wchar_t letter) {
    if (letter >= L'A' && letter <= L'Z') {
        return letter + (L'a' - L'A');
    }
    return letter;
}

// Function to check if a letter has already been guessed
bool isAlreadyGuessed(const std::vector<wchar_t>& guessedLetters, wchar_t letter) {
    for (wchar_t guessedLetter : guessedLetters) {
        if (toLowerCase(guessedLetter) == toLowerCase(letter)) {
            return true;
        }
    }
    return false;
}

// Function to check if a letter is part of the word
bool isPartOfWord(const std::wstring& word, wchar_t letter) {
    for (wchar_t w : word) {
        if (toLowerCase(w) == toLowerCase(letter)) {
            return true;
        }
    }
    return false;
}

int main() {
    // Set the global locale for handling Polish characters
    std::locale::global(std::locale("pl_PL.UTF-8"));

    // Load words from the file
    std::vector<std::wstring> words = loadWordsFromFile("words.txt");
    if (words.empty()) {
        std::cerr << "No words loaded. Exiting..." << std::endl;
        return 1;
    }

    // Select a random word to guess
    srand(time(nullptr));
    std::wstring secretWord = words[rand() % words.size()];

    // Initialize the vector for the letters the player has guessed
    std::vector<wchar_t> guessedLetters;

    // Initialize the number of wrong attempts
    int wrongAttempts = 0;
    int maxAttempts = 7;


    // ------------------------
    // |     Main game loop    |
    // ------------------------
    while (true) {
        // Clear the console
        std::system(CLEAR_SCREEN);

        // Display the state of the hangman and the guessed word
        std::wcout << std::endl;
        drawHangman(wrongAttempts);
        std::wcout << "Current word: ";
        for (wchar_t letter : secretWord) {
            if (isAlreadyGuessed(guessedLetters, letter)) {
                std::wcout << letter << " ";
            }
            else {
                std::wcout << "_ ";
            }
        }
        std::wcout << std::endl;

        // Display wrong attempts
        std::wcout << "Wrong guesses: ";
        bool anyWrongGuesses = false;
        for (wchar_t letter : guessedLetters) {
            if (!isPartOfWord(secretWord, letter)) {
                std::wcout << letter << " ";
                anyWrongGuesses = true;
            }
        }
        if (!anyWrongGuesses) {
            std::wcout << "None";
        }
        std::wcout << std::endl;

        // Display remaining attempts
        std::wcout << "Remaining attempts: " << (maxAttempts - wrongAttempts) << std::endl;

        // Check losing condition
        if (wrongAttempts >= maxAttempts) {
            std::wcout << "Game over! You ran out of attempts. The word was: " << secretWord << std::endl;
            std::wcout << "Wanna play another round? y/n" << std::endl;
            char choice;
            std::cin >> choice;
            if (toLowerCase(choice) == 'y') {
                main();
            }
            break;
        }

        // Check winning condition
        bool allLettersGuessed = true;
        for (wchar_t letter : secretWord) {
            if (!isAlreadyGuessed(guessedLetters, letter)) {
                allLettersGuessed = false;
                break;
            }
        }
        if (allLettersGuessed) {
            std::wcout << "Congratulations! You guessed the word: " << secretWord << std::endl;
            std::wcout << "Wanna play another round? y/n" << std::endl;
            char choice;
            std::cin >> choice;
            if (toLowerCase(choice) == 'y') {
                main();
            }

            break;
        }

        // Get a letter guess from the player
        std::wcout << "Enter your guess: ";
        std::wstring input;
        std::getline(std::wcin, input);

        // Check input validity
        if (input.length() != 1 || !isAlphabetLetter(input[0])) {
            std::wcout << "Invalid input. Please enter a single valid letter." << std::endl;
            std::wcin.clear(); // Clear input error
            std::wcin.ignore(std::numeric_limits<std::streamsize>::max(), L'\n');
            continue;
        }

        // Convert the letter to lowercase
        wchar_t guess = toLowerCase(input[0]);

        // Check if the letter has already been guessed
        if (isAlreadyGuessed(guessedLetters, guess)) {
            std::wcout << "You already guessed that letter. Try again." << std::endl;
            std::wcin.clear(); // Clear input error
            std::wcin.ignore(std::numeric_limits<std::streamsize>::max(), L'\n');
            continue;
        }

        // Add the letter to the guessed letters set
        guessedLetters.push_back(guess);

        // Check if the letter is part of the word
        if (!isPartOfWord(secretWord, guess)) {
            wrongAttempts++;
        }
    }

    return 0;
}
