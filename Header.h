#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

class Base64 {
protected:
	std::string alphabet;
	std::string input;
	std::string coded;
	std::string output;
	std::ifstream inputFile;
	std::ofstream outputFile;


public:

	Base64();

};

class Encoder:Base64 {

private:
	std::string comments;
public:
	
	void EncodeTriplet(const std::string& triplet);
	void EncodeDuplet(const std::string& duplet);
	void EncodeSymbol(const char& symbol);
	void EncodeFile(const std::string& FilePath);
	void DataToFile(const std::string& FilePath);
	void addCommentary(const std::string& com);
	void ClearCoded();
};


class Decoder :Base64 {


public:

	int DecodeTriplet(const std::string& triplet);
	int DecodeDuplet(const std::string& duplet);
	int DecodeSymbol(const std::string& symbol);
	int getPos(const char& symbol);
	int DecodeFile(const std::string& FilePath);
	void DataToFile(const std::string& FilePath);
	void ClearOutput();

};