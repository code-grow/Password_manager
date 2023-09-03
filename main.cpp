#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>

std::map<std::string, std::string> passwords;

std::string encryptPassword(const std::string &password) {// Function to encrypt the password (a basic example)
    std::string encrypted = password;
    for (char &c : encrypted) {
        c = (c + 1) % 128; // Simple Caesar cipher, not secure
    }
    return encrypted;
}

std::string decryptPassword(const std::string &encryptedPassword) {// Function to decrypt the password (matching the encryption above)
    std::string decrypted = encryptedPassword;
    for (char &c : decrypted) {
        c = (c - 1 + 128) % 128;
    }
    return decrypted;
}

void savePasswordsToFile(const std::string &filename) { // Save passwords to a file
    std::ofstream outFile(filename);
    if (outFile) {
        for (const auto &entry : passwords) {
            outFile << entry.first << ":" << entry.second << std::endl;
        }
        outFile.close();
    } else {
        std::cerr << "Failed to save passwords to file." << std::endl;
    }
}

void loadPasswordsFromFile(const std::string &filename) {   // Load passwords from a file
    std::ifstream inFile(filename);
    if (inFile) {
        passwords.clear();
        std::string line;
        while (std::getline(inFile, line)) {
            std::istringstream iss(line);
            std::string service, encryptedPassword;
            std::getline(iss, service, ':');
            std::getline(iss, encryptedPassword);
            passwords[service] = encryptedPassword;
        }
        inFile.close();
    } else {
        std::cerr << "Failed to load passwords from file." << std::endl;
    }
}

int main() {
    const std::string passwordFile = "passwords.txt";

    loadPasswordsFromFile(passwordFile);

    while (true) {
        std::cout << "1. Add password\n2. Retrieve password\n3. Exit\n";
        int choice;
        std::cin >> choice;

        if (choice == 1) {
            std::string service, password;
            std::cout << "Service: ";
            std::cin >> service;
            std::cout << "Password: ";
            std::cin >> password;
            passwords[service] = encryptPassword(password);
            savePasswordsToFile(passwordFile);
        } else if (choice == 2) {
            std::string service;
            std::cout << "Service: ";
            std::cin >> service;
            if (passwords.find(service) != passwords.end()) {
                std::string decryptedPassword = decryptPassword(passwords[service]);
                std::cout << "Password for " << service << ": " << decryptedPassword << std::endl;
            } else {
                std::cout << "Password not found for " << service << std::endl;
            }
        } else if (choice == 3) {
            break;
          } else {
            std::cout << "Invalid choice." << std::endl;
            break;//in case the choice is not a character other than a number (bug-fixed)
        }
    }

    return 0;
}
