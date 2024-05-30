#ifndef ANALYZER_H
#define ANALYZER_H

#include <string>
#include <vector>

class BinaryAnalyzer {
public:
    BinaryAnalyzer(const std::string& filepath);
    void analyze();

private:
    std::string filepath;
    void displayFileInfo();
    void detectHiddenCode();
    void findStrings();
    void analyzeHeaders();
    void logResults(const std::string& message);

    std::vector<std::string> log;
};

#endif // ANALYZER_H
