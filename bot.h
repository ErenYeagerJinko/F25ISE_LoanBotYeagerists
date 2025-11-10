#pragma once
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int numberOfLinesInFile(string fileName) {
	ifstream file(fileName);
	if (!file) {
		cout << "Error! Could Not Open File!\n";
		return 0;
	}
	int numberOfLines = 0;
	string line;
	while (getline(file, line)) {
		numberOfLines++;
	}
	file.close();
	return numberOfLines;
}

class Responder {
public:
	string* userInput, * systemResponse; 
	int numberOfLines;
	Responder() {
		numberOfLines = numberOfLinesInFile("Utterances.txt");
		userInput = new string[numberOfLines];
		systemResponse = new string[numberOfLines];
		for (int i = 0; i < numberOfLines; i++) {
			userInput[i] = systemResponse[i] = "";
		}
	}
	~Responder() {
		delete[] userInput;
		delete[] systemResponse;
		userInput = systemResponse = nullptr;
	}
	void initializeResponder(string fileName) {
		ifstream file(fileName);
		if (!file) {
			cout << "Error! Could Not Open File!\n";
			return;
		}
		string line = "";
		for (int i = 0; i < numberOfLines; i++) {
			getline(file, line);
			int index = 0;
			while (line[index] != '#') {
				userInput[i] += tolower(line[index]);
				index++;
			}
			index++;
			while (line[index] != '\0') {
				systemResponse[i] += line[index];
				index++;
			}
		}
		file.close();
		return;
	}
};