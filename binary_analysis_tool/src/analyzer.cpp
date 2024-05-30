#include "analyzer.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <elf.h>

BinaryAnalyzer::BinaryAnalyzer(const std::string& filepath) : filepath(filepath) {}

void BinaryAnalyzer::analyze() {
    displayFileInfo();
    analyzeHeaders();
    detectHiddenCode();
    findStrings();
    logResults("Analysis completed.");
}

void BinaryAnalyzer::displayFileInfo() {
    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "Failed to open file." << std::endl;
        return;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::cout << "File size: " << size << " bytes" << std::endl;
    log.push_back("File size: " + std::to_string(size) + " bytes");

    // Display additional file info (e.g., magic number, headers)
    char magic[4];
    file.read(magic, 4);
    std::cout << "Magic number: " << std::hex << static_cast<int>(magic[0])
              << static_cast<int>(magic[1]) << static_cast<int>(magic[2])
              << static_cast<int>(magic[3]) << std::dec << std::endl;
    log.push_back("Magic number: " + std::to_string(static_cast<int>(magic[0])) +
                  std::to_string(static_cast<int>(magic[1])) +
                  std::to_string(static_cast<int>(magic[2])) +
                  std::to_string(static_cast<int>(magic[3])));

    file.close();
}

void BinaryAnalyzer::analyzeHeaders() {
    std::ifstream file(filepath, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file." << std::endl;
        return;
    }

    // Read ELF header
    Elf64_Ehdr elf_header;
    file.read(reinterpret_cast<char*>(&elf_header), sizeof(elf_header));

    // Check ELF magic number
    if (std::memcmp(elf_header.e_ident, ELFMAG, SELFMAG) != 0) {
        std::cerr << "Not a valid ELF file." << std::endl;
        log.push_back("Not a valid ELF file.");
        file.close();
        return;
    }

    std::cout << "ELF file detected." << std::endl;
    log.push_back("ELF file detected.");

    // Display ELF header information
    std::cout << "Entry point: " << std::hex << elf_header.e_entry << std::dec << std::endl;
    log.push_back("Entry point: " + std::to_string(elf_header.e_entry));

    file.close();
}

void BinaryAnalyzer::detectHiddenCode() {
    std::ifstream file(filepath, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file." << std::endl;
        return;
    }

    unsigned char byte;
    while (file.read(reinterpret_cast<char*>(&byte), 1)) {
        if (byte == static_cast<unsigned char>(0x90)) {
            std::cout << "Potential NOP sled detected." << std::endl;
            log.push_back("Potential NOP sled detected.");
            break;
        }
    }

    file.close();
}

void BinaryAnalyzer::findStrings() {
    std::ifstream file(filepath, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file." << std::endl;
        return;
    }

    std::string str;
    unsigned char byte;
    while (file.read(reinterpret_cast<char*>(&byte), 1)) {
        if (isprint(byte) || byte == 0) {
            str += byte;
        } else if (!str.empty()) {
            if (str.length() > 4) {
                std::cout << "String found: " << str << std::endl;
                log.push_back("String found: " + str);
            }
            str.clear();
        }
    }

    file.close();
}

void BinaryAnalyzer::logResults(const std::string& message) {
    std::ofstream logfile("analysis_log.txt", std::ios::app);
    if (!logfile) {
        std::cerr << "Failed to open log file." << std::endl;
        return;
    }

    for (const auto& entry : log) {
        logfile << entry << std::endl;
    }

    logfile << message << std::endl;
    logfile.close();
}
