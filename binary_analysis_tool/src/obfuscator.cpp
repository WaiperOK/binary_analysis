#include "obfuscator.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

BinaryObfuscator::BinaryObfuscator(const std::string& filepath) : filepath(filepath) {}

void BinaryObfuscator::obfuscate() {
    obfuscateCode();
    addFakeInstructions();
    encryptStrings();
}

void BinaryObfuscator::obfuscateCode() {
    std::fstream file(filepath, std::ios::binary | std::ios::in | std::ios::out);
    if (!file) {
        std::cerr << "Failed to open file." << std::endl;
        return;
    }

    unsigned char byte;
    while (file.read(reinterpret_cast<char*>(&byte), 1)) {
        if (byte == static_cast<unsigned char>(0x90)) {
            file.seekp(-1, std::ios::cur); 
            file.put(static_cast<char>(0x31)); 
            file.put(static_cast<char>(0xC0));
        }
    }

    file.close();
    std::cout << "Code obfuscation completed." << std::endl;
}

void BinaryObfuscator::addFakeInstructions() {
    std::fstream file(filepath, std::ios::binary | std::ios::in | std::ios::out | std::ios::app);
    if (!file) {
        std::cerr << "Failed to open file." << std::endl;
        return;
    }

   
    const unsigned char fake_instructions[] = {0x90, 0x90, 0x90, 0x31, 0xC0};
    file.write(reinterpret_cast<const char*>(fake_instructions), sizeof(fake_instructions));

    file.close();
    std::cout << "Fake instructions added." << std::endl;
}

void BinaryObfuscator::encryptStrings() {
    std::fstream file(filepath, std::ios::binary | std::ios::in | std::ios::out);
    if (!file) {
        std::cerr << "Failed to open file." << std::endl;
        return;
    }

    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.clear();
    file.seekp(0, std::ios::beg);

    std::string key = "encryption_key";
    for (size_t i = 0; i < buffer.size(); ++i) {
        buffer[i] ^= key[i % key.size()];
    }

    file.write(buffer.data(), buffer.size());
    file.close();
    std::cout << "Strings encrypted." << std::endl;
}
