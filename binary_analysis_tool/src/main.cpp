#include <iostream>
#include <unistd.h>
#include "analyzer.h"
#include "obfuscator.h"

void print_usage() {
    std::cout << "Usage: binary_analysis_tool -f <file_path> [-a] [-o]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -f <file_path>    Path to the binary file" << std::endl;
    std::cout << "  -a                Analyze the binary file" << std::endl;
    std::cout << "  -o                Obfuscate the binary file" << std::endl;
}

int main(int argc, char* argv[]) {
    std::string filepath;
    bool analyze = false;
    bool obfuscate = false;

    int opt;
    while ((opt = getopt(argc, argv, "f:ao")) != -1) {
        switch (opt) {
        case 'f':
            filepath = optarg;
            break;
        case 'a':
            analyze = true;
            break;
        case 'o':
            obfuscate = true;
            break;
        default:
            print_usage();
            return 1;
        }
    }

    if (filepath.empty()) {
        print_usage();
        return 1;
    }

    if (analyze) {
        BinaryAnalyzer analyzer(filepath);
        analyzer.analyze();
    }

    if (obfuscate) {
        BinaryObfuscator obfuscator(filepath);
        obfuscator.obfuscate();
    }

    return 0;
}
