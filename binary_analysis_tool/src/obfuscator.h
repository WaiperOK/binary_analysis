#ifndef OBFUSCATOR_H
#define OBFUSCATOR_H

#include <string>

class BinaryObfuscator {
public:
    BinaryObfuscator(const std::string& filepath);
    void obfuscate();

private:
    std::string filepath;
    void obfuscateCode();
    void addFakeInstructions();
    void encryptStrings();
};

#endif // OBFUSCATOR_H
