#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <unordered_map>
#include <algorithm>

// This function takes in a string and returns a copy with non-alphabetical characters removed.
std::string make_alphabetical(const std::string& word) {
    // Declare a new string to hold the alphabetized word.
    std::string alpha_word;
    // Loop through each character in the word.
    for (char c : word) {
        // If it is alphabetical, add it to the alphabetical word.
        if (std::isalpha(c)) {
            alpha_word += c;
        }
    }
    // Return the alphabetized word.
    return alpha_word;
}

int main() {
    // Print out a welcome message to the user.
    std::cout << "Welcome to Word Counter!" << std::endl;
    // Explain what this program is and what it does.
    std::cout << "This utility program counts word frequency in a given file." << std::endl;
    std::cout << "It will save its results to a file you choose." << std::endl;
    std::cout << "A word consists of alphabetical characters surrounded by whitespace." << std::endl;
    std::cout << "This program will automatically compare all words case insensitively." << std::endl;

    // Declare two strings for the filenames that the user will pass in.
    std::string filename;
    std::string save_filename;

    // Store the current directory of the system into a convenient constant.
    const std::filesystem::path current_directory = std::filesystem::current_path();
    // Display the current directory to the user, so they can enter in relative file path names.
    std::cout << "Current directory: " << current_directory << std::endl;

    // Get the file to analyze from user input.
    std::cout << "Enter file to analyze:";
    std::getline(std::cin, filename);
    // Display the full absolute path for the user to see if it is correct.
    std::cout << "Path: " << current_directory / filename << std::endl;

    // Create and open the file.
    std::ifstream file;
    file.open(current_directory / filename);

    // Check to see if the file is valid.
    if (!file) {
        // If the file is not valid, display an error message for the user.
        std::cout << "File couldn't be opened!" << std::endl;
        // Then write a better error message to the standard error stream.
        std::cerr << "Error opening file:" << current_directory / filename;
        // Finally, exit with return code 1 to signify that something went wrong.
        return 1;
    }

    // Get the name of the save file from user input.
    std::cout << "Enter file to store results:";
    std::getline(std::cin, save_filename);
    // Display the full absolute path for the user to see if it is correct.
    std::cout << "Path: " << current_directory / save_filename << std::endl;

    // Create and open the file.
    std::ofstream save_file;
    save_file.open(current_directory / save_filename, std::fstream::out);

    // Check to see if the file is valid.
    if (!save_file) {
        // If the file is not valid, display an error message for the user.
        std::cout << "File couldn't be opened!" << std::endl;
        // Then write a better error message to the standard error stream.
        std::cerr << "Error opening file:" << current_directory / save_filename;
        // Finally, exit with return code 1 to signify that something went wrong.
        return 1;
    }

    // Report to the user that the file has been opened successfully.
    std::cout << "File opened!" << std::endl;

    // Declare the contents variable to hold the current word from the file.
    std::string contents;
    // Declare the map variable to store each unique word along with a frequency count.
    // The keys of the map are lowercase, alphabetical strings for the words.
    // The values of the map are unsigned integers for the frequency counts.
    std::unordered_map<std::string, unsigned int> word_counts;

    // Enter a loop to process the file.
    // The loop terminates when the file has reached the end.
    while (!file.eof()) {
        // Store the next word into the contents variable.
        // The >> operator separates the contents of the file by whitespace.
        file >> contents;
        // Transform the word into lowercase for case-insensitive counting.
        std::transform(contents.begin(), contents.end(), contents.begin(), ::tolower);
        // Use the make_alphabetical() function from earlier to strip the word of all non-alphabetic characters.
        contents = make_alphabetical(contents);

        // The word will be empty if it contained only non-alphabetical characters.
        // Check to see if the word is empty.
        if (!contents.empty()) {
            // If the word isn't empty, we can count it.
            // Check to see if the word isn't already within the map.
            if (word_counts.count(contents) == 0) {
                // If the word isn't in the map, add it to the map with a frequency of 1.
                word_counts.insert_or_assign(contents, 1);
            } else {
                // If the word is in the map, increment its frequency value.
                word_counts.at(contents)++;
            }
        }
    }

    // Display the statistics for the user.
    std::cout << "Word counts:" << std::endl;
    // Loop through each key and value in the map.
    for (const auto & [ key, value ] : word_counts) {
        // Format them in a pretty way for human readability.
        std::cout << key << ": " << value << std::endl;
    }

    // Close the file we were reading from to free up system resources.
    file.close();
    std::cout << "File closed." << std::endl;

    // Tell the user we are writing to the save file.
    std::cout << "Writing to save file...";

    // Write the statistics to the save file.
    // Loop through each key and value in the map.
    for (const auto & [ key, value ] : word_counts) {
        // Format them in a pretty way for human readability.
        save_file << key << ": " << value << std::endl;
    }

    // Close the file to which we were writing to free up system resources.
    save_file.close();
    // Inform the user that the save file writing has completed.
    std::cout << "complete!" << std::endl;

    // Exit with return code 0 to signify a normal program termination.
    return 0;
}
