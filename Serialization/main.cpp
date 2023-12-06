#include "serialization.h"

bool checkFileName(bool ser, std::string inputFile, std::string outputFile){
    auto inputSize = inputFile.size();
    auto outputSize = outputFile.size();
    if(ser)
        return inputFile.substr(inputSize - 4, 4) == ".tsv" && outputFile.substr(outputSize - 4, 4) == ".bin";
    return inputFile.substr(inputSize - 4, 4) == ".bin" && outputFile.substr(outputSize - 4, 4) == ".tsv";
}


int main(int argc, char* argv[]){    
    std::vector<std::string> args;
    std::string inputFile, outputFile;
    int flag;
    bool ser;
    for(int i = 1; i < argc; ++i){
        auto arg = std::string(argv[i]);
        if(arg == "-d")
            ser = false;
        else if(arg == "-s")
            ser = true;
        else if(arg == "-i"){
            if(i < argc - 1)
                inputFile = std::string(argv[++i]);
            else{
                std::cout << "No input filename after -i flag" << std::endl;
                return 1;
            }
        }
        else if(arg == "-o"){
            if(i < argc - 1)
                outputFile = std::string(argv[++i]);
            else{
                std::cout << "No output filename after -o flag" << std::endl;
                return 1;
            }
        }
        else{
            std::cout << "Unknown flag" << std::endl;
            return 1;
        }
    }

    if(!checkFileName(ser, inputFile, outputFile)){
        std::cout << "Шncorrect file extensions" << std::endl;
        return 1;
    }

    if(ser){
        Serializer sr(inputFile, outputFile);
        sr.Serialization();
    }
    else{
        Deserializer dsr(inputFile, outputFile);
        dsr.Deserialization();
    }

    return 0;
}       