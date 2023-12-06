#pragma once
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include "output.h"

using graph = std::unordered_map<uint32_t, std::vector<std::pair<uint32_t, uint8_t>>>;

class Serializer{
public:
    Serializer(std::string input, std::string output) : inputFile(input), outputFile(output) {};
    void Serialization();
private:
    std::string inputFile;
    std::string outputFile;
    graph gr;
    auto readData(uint64_t& max_deg);
    void writeData();
};

class Deserializer{
public:
    Deserializer(std::string input, std::string output) : inputFile(input), outputFile(output) {};
    void Deserialization();
private:
    std::string inputFile;
    std::string outputFile;
};