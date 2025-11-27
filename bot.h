#pragma once
#include <iostream>
#include <limits>
#include <fstream>
#include <string>
#include <errno.h>
#include <direct.h>
#include <cctype>
#include <algorithm>
#include <sstream>
#include <vector>
#include <set>
#include <sys/stat.h>
#include <cstring>
#include <cstdlib>
#include <ctime>
using namespace std;

void initializeRandomSeed() {
	srand(time(0));
}

int numberOfLinesInFile(string fileName) {
	ifstream file(fileName);
	if (!file) {
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

string trim(string& str) {
	auto start = find_if_not(str.begin(), str.end(), [](unsigned char ch) {
		return isspace(ch);
		});

	auto end = find_if_not(str.rbegin(), str.rend(), [](unsigned char ch) {
		return isspace(ch);
		}).base();

	return (start < end) ? string(start, end) : "";
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
	void respondToUser(string input) {
		bool foundResponse = false;
		for (char& c : input) {
			if (c == '\0') {
				break;
			}
			c = tolower(c);
		}
		for (int i = 0; i < numberOfLines; i++) {
			if (input == userInput[i]) {
				cout << systemResponse[i] << endl;
				foundResponse = true;
			}
		}
		if (!foundResponse) {
			foundResponse = true;
			respondToUser("*");
		}
		return;
	}
};

struct ConversationPair {
	string question;
	string response;
};

class Conversation {
public:
	string* userInput, * systemResponse;
	int numberOfLines;

	Conversation() {
		numberOfLines = numberOfLinesInFile("human_chat_corpus.txt") / 2;
		userInput = new string[numberOfLines];
		systemResponse = new string[numberOfLines];
		for (int i = 0; i < numberOfLines; i++) {
			userInput[i] = systemResponse[i] = "";
		}
	}

	~Conversation() {
		delete[] userInput;
		delete[] systemResponse;
		userInput = systemResponse = nullptr;
	}

	vector<string> tokenize(const string& text) {
		vector<string> tokens;
		string word = "";

		for (int i = 0; i < text.length(); i++) {
			char c = tolower(text[i]);

			if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) {
				word += c;
			}
			else if (c == ' ' || c == '\t' || c == '\n') {
				if (!word.empty()) {
					tokens.push_back(word);
					word = "";
				}
			}
		}

		if (!word.empty()) {
			tokens.push_back(word);
		}

		return tokens;
	}


	double calculateIoU(const string& userInput, const string& corpusLine) {
		vector<string> tokensUser = tokenize(userInput);
		vector<string> tokensCorpus = tokenize(corpusLine);

		if (tokensUser.empty() || tokensCorpus.empty()) {
			return 0.0;
		}

		int intersection = 0;
		for (int i = 0; i < tokensUser.size(); i++) {
			for (int j = 0; j < tokensCorpus.size(); j++) {
				if (tokensUser[i] == tokensCorpus[j]) {
					intersection++;
					break;
				}
			}
		}

		vector<string> unionTokens;

		for (int i = 0; i < tokensUser.size(); i++) {
			unionTokens.push_back(tokensUser[i]);
		}

		for (int i = 0; i < tokensCorpus.size(); i++) {
			bool alreadyExists = false;
			for (int j = 0; j < unionTokens.size(); j++) {
				if (tokensCorpus[i] == unionTokens[j]) {
					alreadyExists = true;
					break;
				}
			}
			if (!alreadyExists) {
				unionTokens.push_back(tokensCorpus[i]);
			}
		}

		int unionSize = unionTokens.size();

		if (unionSize == 0) {
			return 0.0;
		}

		return (double)intersection / (double)unionSize;
	}

	vector<ConversationPair> loadConversationFromFile(const string& filename) {
		vector<ConversationPair> corpus;
		ifstream file(filename);

		if (!file.is_open()) {
			cerr << "Error: Cannot open " << filename << endl;
			return corpus;
		}

		string line;
		string currentHuman1 = "";

		while (getline(file, line)) {
			if (line.find("Human 1:") == 0) {
				currentHuman1 = line.substr(9);
			}
			else if (line.find("Human 2:") == 0 && !currentHuman1.empty()) {
				ConversationPair pair;
				pair.question = currentHuman1;
				pair.response = line.substr(9);

				corpus.push_back(pair);

				currentHuman1 = "";
			}
		}

		file.close();
		return corpus;
	}

	string getBestResponse(const string& userInput) {
		vector<ConversationPair> corpus = loadConversationFromFile("human_chat_corpus.txt");
		if (corpus.empty()) {
			return "Conversation database not loaded.";
		}

		struct Match {
			string response;
			string question;
			double iou;
			int index;
		};

		vector<Match> allMatches;

		for (int i = 0; i < corpus.size(); i++) {
			double iou = calculateIoU(userInput, corpus[i].question);

			if (iou > 0.1) {
				Match m;
				m.response = corpus[i].response;
				m.question = corpus[i].question;
				m.iou = iou;
				m.index = i;
				allMatches.push_back(m);
			}
		}

		if (allMatches.empty()) {
			return "I'm not sure how to respond to that.";
		}

		double maxIoU = 0.0;
		for (int i = 0; i < allMatches.size(); i++) {
			if (allMatches[i].iou > maxIoU) {
				maxIoU = allMatches[i].iou;
			}
		}

		vector<Match> bestMatches;
		for (int i = 0; i < allMatches.size(); i++) {
			if (allMatches[i].iou == maxIoU) {
				bestMatches.push_back(allMatches[i]);
			}
		}

		int randomIndex = rand() % bestMatches.size();
		return bestMatches[randomIndex].response;
	}
};

class HomeLoan {
public:
	int numberOfLines, area1Homes, area2Homes, area3Homes, area4Homes;
	int* area, * size, * installments, * price, * downPayment;
	HomeLoan() {
		numberOfLines = numberOfLinesInFile("Home.txt");
		if (numberOfLines > 0) numberOfLines -= 1;
		area = new int[numberOfLines];
		size = new int[numberOfLines];
		installments = new int[numberOfLines];
		price = new int[numberOfLines];
		downPayment = new int[numberOfLines];
		area1Homes = area2Homes = area3Homes = area4Homes = 0;
	}
	~HomeLoan() {
		delete[] area;
		delete[] size;
		delete[] installments;
		delete[] price;
		delete[] downPayment;
		area = size = installments = price = downPayment = nullptr;
	}
	void initializeHomeLoan() {
		ifstream file("Home.txt");
		if (!file) {
			cout << "Error: Home.txt not found!\n";
			return;
		}
		string line;
		getline(file, line);

		int index = 0;
		while (getline(file, line) && index < numberOfLines) {
			size_t pos = 0;
			string token;

			pos = line.find('#');
			token = line.substr(0, pos);
			area[index] = extractInteger(token);
			if (area[index] == 1) {
				area1Homes++;
			}
			else if (area[index] == 2) {
				area2Homes++;
			}
			else if (area[index] == 3) {
				area3Homes++;
			}
			else if (area[index] == 4) {
				area4Homes++;
			}

			line.erase(0, pos + 1);

			pos = line.find('#');
			token = line.substr(0, pos);
			size[index] = extractInteger(token);
			line.erase(0, pos + 1);

			pos = line.find('#');
			token = line.substr(0, pos);
			installments[index] = extractInteger(token);
			line.erase(0, pos + 1);

			pos = line.find('#');
			token = line.substr(0, pos);
			price[index] = extractInteger(token);
			line.erase(0, pos + 1);

			downPayment[index] = extractInteger(line);

			++index;
		}
		file.close();
	}
	int extractInteger(string& str) {
		int value = 0;
		int start = 0;
		while (start < str.length() && !isdigit(str[start])) {
			start++;
		}
		for (int i = start; i < str.length(); i++) {
			if (isdigit(str[i])) {
				value = value * 10 + (str[i] - '0');
			}
		}
		return value;
	}
	int displayHomes(int areaNumber) {
		int homesAvailable = 0;
		for (int i = 0; i < numberOfLines; i++) {
			if (area[i] == areaNumber) {
				homesAvailable++;
				cout << "Area: " << area[i] << endl;
				cout << "Size: " << size[i] << " Marla\n";
				cout << "Installments: " << installments[i] << endl;
				cout << "Price: " << price[i] << endl;
				cout << "Down Payment: " << downPayment[i] << endl;
				cout << "------------------------\n" << endl;
			}
		}
		if (homesAvailable == 0) {
			cout << "No Homes Available in This Area\n\n";
		}
		return homesAvailable;
	}
	void generateInstallmentPlan(int areaNumber) {
		for (int i = 0; i < numberOfLines; i++) {
			if (area[i] == areaNumber) {
				int monthlyInstallment = (price[i] - downPayment[i]) / installments[i];
				cout << "\nFor home size " << size[i] << " Marla in Area " << area[i] << ":\n";
				cout << "Price: " << price[i] << "\n";
				cout << "Down Payment: " << downPayment[i] << "\n";
				cout << "Installments: " << installments[i] << " months\n";
				cout << "Monthly Installment: " << monthlyInstallment << endl;
				cout << "------------------------\n\n";
			}
		}
	}
};

class ScooterLoan {
public:
	int numberOfLines;
	string* make;
	string* model;
	int* distancePerCharge;
	int* chargingTime;
	int* maxSpeed;
	int* installments;
	int* price;
	int* downPayment;

	ScooterLoan() {
		numberOfLines = numberOfLinesInFile("Scooter.txt");
		if (numberOfLines > 0) numberOfLines -= 1;

		make = new string[numberOfLines];
		model = new string[numberOfLines];
		distancePerCharge = new int[numberOfLines];
		chargingTime = new int[numberOfLines];
		maxSpeed = new int[numberOfLines];
		installments = new int[numberOfLines];
		price = new int[numberOfLines];
		downPayment = new int[numberOfLines];
	}

	~ScooterLoan() {
		delete[] make;
		delete[] model;
		delete[] distancePerCharge;
		delete[] chargingTime;
		delete[] maxSpeed;
		delete[] installments;
		delete[] price;
		delete[] downPayment;
		make = model = nullptr;
		distancePerCharge = chargingTime = maxSpeed = installments = price = downPayment = nullptr;
	}

	void initializeScooterLoan() {
		ifstream file("Scooter.txt");
		if (!file) {
			cout << "Error: Scooter.txt not found!\n";
			return;
		}
		string line;
		getline(file, line); 

		int index = 0;
		while (getline(file, line) && index < numberOfLines) {
			int pos = 0;
			string token;

			pos = line.find('#');
			make[index] = line.substr(0, pos);
			line.erase(0, pos + 1);

			pos = line.find('#');
			model[index] = line.substr(0, pos);
			line.erase(0, pos + 1);

			pos = line.find('#');
			token = line.substr(0, pos);
			distancePerCharge[index] = extractInteger(token);
			line.erase(0, pos + 1);

			pos = line.find('#');
			token = line.substr(0, pos);
			chargingTime[index] = extractInteger(token);
			line.erase(0, pos + 1);

			pos = line.find('#');
			token = line.substr(0, pos);
			maxSpeed[index] = extractInteger(token);
			line.erase(0, pos + 1);

			pos = line.find('#');
			token = line.substr(0, pos);
			installments[index] = extractInteger(token);
			line.erase(0, pos + 1);

			pos = line.find('#');
			token = line.substr(0, pos);
			price[index] = extractInteger(token);
			line.erase(0, pos + 1);

			downPayment[index] = extractInteger(line);

			++index;
		}
		file.close();
	}

	int extractInteger(string& str) {
		int value = 0;
		int start = 0;
		while (start < str.length() && !isdigit(str[start])) {
			start++;
		}
		for (int i = start; i < str.length(); i++) {
			if (isdigit(str[i])) {
				value = value * 10 + (str[i] - '0');
			}
		}
		return value;
	}

	void displayScooters() {
		if (numberOfLines == 0) {
			cout << "No Scooters Available\n\n";
			return;
		}
		for (int i = 0; i < numberOfLines; i++) {
			cout << "Make: " << make[i] << endl;
			cout << "Model: " << model[i] << endl;
			cout << "Distance Per Charge: " << distancePerCharge[i] << " KM\n";
			cout << "Charging Time: " << chargingTime[i] << " Hrs\n";
			cout << "Max Speed: " << maxSpeed[i] << " KM/H\n";
			cout << "Installments: " << installments[i] << " months\n";
			cout << "Price: " << price[i] << endl;
			cout << "Down Payment: " << downPayment[i] << endl;
			cout << "------------------------\n" << endl;
		}
	}

	void generateInstallmentPlan() {
		for (int i = 0; i < numberOfLines; i++) {
			int monthlyInstallment = (price[i] - downPayment[i]) / installments[i];
			cout << "\nFor " << make[i] << " " << model[i] << ":\n";
			cout << "Price: " << price[i] << "\n";
			cout << "Down Payment: " << downPayment[i] << "\n";
			cout << "Installments: " << installments[i] << " months\n";
			cout << "Monthly Installment: " << monthlyInstallment << endl;
			cout << "------------------------\n\n";
		}
	}
};

class CarLoan {
public:
	int numberOfLines;
	string* make;
	string* model;
	int* engine;       
	string* used;       
	int* year;
	int* installments;
	int* price;
	int* downPayment;
	
	CarLoan() {
		numberOfLines = numberOfLinesInFile("Car.txt");
		if (numberOfLines > 0) numberOfLines -= 1;

		make = new string[numberOfLines];
		model = new string[numberOfLines];
		engine = new int[numberOfLines];   
		used = new string[numberOfLines];   
		year = new int[numberOfLines];
		installments = new int[numberOfLines];
		price = new int[numberOfLines];
		downPayment = new int[numberOfLines];
		
	}

	~CarLoan() {
		delete[] make;
		delete[] model;
		delete[] engine;    
		delete[] used;      
		delete[] year;
		delete[] installments;
		delete[] price;
		delete[] downPayment;
		
		make = model = used = nullptr;
		year = engine = installments = price = downPayment = nullptr;
	}

	void initializeCarLoan() {
		ifstream file("Car.txt");
		if (!file) {
			cout << "Error: Car.txt not found!\n";
			return;
		}
		string line;
		getline(file, line); 

		int index = 0;
		while (getline(file, line) && index < numberOfLines) {
			int pos = 0;
			string token;

			
			pos = line.find('#');
			make[index] = line.substr(0, pos);
			line.erase(0, pos + 1);

			
			pos = line.find('#');
			model[index] = line.substr(0, pos);
			line.erase(0, pos + 1);

			
			pos = line.find('#');
			token = line.substr(0, pos);
			engine[index] = extractInteger(token);
			line.erase(0, pos + 1);

			
			pos = line.find('#');
			used[index] = line.substr(0, pos);
			line.erase(0, pos + 1);

			
			pos = line.find('#');
			token = line.substr(0, pos);
			year[index] = extractInteger(token);
			line.erase(0, pos + 1);

		
			pos = line.find('#');
			token = line.substr(0, pos);
			installments[index] = extractInteger(token);
			line.erase(0, pos + 1);

		
			pos = line.find('#');
			token = line.substr(0, pos);
			price[index] = extractInteger(token); 
			line.erase(0, pos + 1);

			
			downPayment[index] = extractInteger(line);

			++index;
		}
		file.close();
	}

	int extractInteger(string& str) {
		int value = 0;
		int start = 0;
		while (start < str.length() && !isdigit(str[start])) {
			start++;
		}
		for (int i = start; i < str.length(); i++) {
			if (isdigit(str[i])) {
				value = value * 10 + (str[i] - '0');
			}
		}
		return value;
	}

	void displayCarsByMakeOption(int option) {
		string targetMake = (option == 1) ? "Make 1" : "Make 2";

		bool found = false;
		for (int i = 0; i < numberOfLines; i++) {
			if (make[i] == targetMake) {
				found = true;
				cout << "Make: " << make[i] << endl;
				cout << "Model: " << model[i] << endl;
				cout << "Engine: " << engine[i] << " cc" << endl;
				cout << "Used: " << used[i] << endl;
				cout << "Manufacturing Year: " << year[i] << endl;
				cout << "Installments: " << installments[i] << " months" << endl;
				cout << "Price: " << price[i] << endl;
				cout << "Down Payment: " << downPayment[i] << endl;
				cout << "------------------------" << endl;
			}
		}

		if (!found) {
			cout << "No cars available for " << targetMake << "." << endl;
		}
	}

	void generateInstallmentPlanForOption(int option) {
		string targetMake = (option == 1) ? "Make 1" : "Make 2";

		bool found = false;
		for (int i = 0; i < numberOfLines; i++) {
			if (make[i] == targetMake) {
				found = true;
				int monthlyInstallment = (price[i] - downPayment[i]) / installments[i];
				cout << "\nFor " << make[i] << " " << model[i] << " (" << year[i] << "):\n";
				cout << "Price: " << price[i] << "\n";
				cout << "Down Payment: " << downPayment[i] << "\n";
				cout << "Installments: " << installments[i] << " months\n";
				cout << "Monthly Installment: " << monthlyInstallment << endl;
				cout << "------------------------\n" << endl;
			}
		}

		if (!found) {
			cout << "No cars found for " << targetMake << " to generate installment plan.\n";
		}
	}
};

class PersonalLoan {
public:
	int numberOfLines;
	string* category;
	int* installments;
	int* amount;
	int* downPayment;

	PersonalLoan() {
		numberOfLines = numberOfLinesInFile("Personal.txt");
		if (numberOfLines > 0) numberOfLines -= 1;
		category = (numberOfLines > 0) ? new string[numberOfLines] : nullptr;
		installments = (numberOfLines > 0) ? new int[numberOfLines] : nullptr;
		amount = (numberOfLines > 0) ? new int[numberOfLines] : nullptr;
		downPayment = (numberOfLines > 0) ? new int[numberOfLines] : nullptr;
	}

	~PersonalLoan() {
		delete[] category;
		delete[] installments;
		delete[] amount;
		delete[] downPayment;
	}

	void initializePersonalLoan() {
		ifstream file("Personal.txt");
		if (!file) {
			cout << "Error: Personal.txt not found!\n";
			return;
		}
		string line;
		getline(file, line); 
		int index = 0;
		while (getline(file, line) && index < numberOfLines) {
			size_t pos = 0;
			string token;

			pos = line.find('#');
			category[index] = line.substr(0, pos);
			line.erase(0, pos + 1);

			pos = line.find('#');
			token = line.substr(0, pos);
			installments[index] = extractInteger(token);
			line.erase(0, pos + 1);

			pos = line.find('#');
			token = line.substr(0, pos);
			amount[index] = extractInteger(token);
			line.erase(0, pos + 1);

			downPayment[index] = extractInteger(line);
			++index;
		}
		file.close();
	}

	int extractInteger(string& str) {
		int value = 0;
		int start = 0;
		while (start < str.length() && !isdigit(str[start])) {
			start++;
		}
		for (int i = start; i < str.length(); i++) {
			if (isdigit(str[i])) {
				value = value * 10 + (str[i] - '0');
			}
		}
		return value;
	}

	void displayPersonalLoans() {
		if (numberOfLines == 0) {
			cout << "No personal loans available.\n";
			return;
		}
		for (int i = 0; i < numberOfLines; i++) {
			cout << "Category: " << category[i] << endl;
			cout << "Amount: " << amount[i] << endl;
			cout << "Down Payment: " << downPayment[i] << endl;
			cout << "Installments: " << installments[i] << endl;
			cout << "------------------------\n" << endl;
		}
	}

	void generateInstallmentPlan() {
		for (int i = 0; i < numberOfLines; i++) {
			int monthlyInstallment = (amount[i] - downPayment[i]) / installments[i];
			cout << "\nFor " << category[i] << " Loan:\n";
			cout << "Total Amount: " << amount[i] << "\n";
			cout << "Down Payment: " << downPayment[i] << "\n";
			cout << "Installments: " << installments[i] << " months\n";
			cout << "Monthly Installment: " << monthlyInstallment << endl;
			cout << "------------------------\n";
		}
	}
};

struct Referee {
    string name;
    string cnic;
    string cnicIssueDate;
    string phoneNumber;
    string emailAddress;
};

struct ApplicantImages {
    string cnicFrontPath;
    string cnicBackPath;
    string electricityBillPath;
    string salarySlipPath;
};

class LoanSeeker {
private:
    bool validateAlphabetString(const string& str) {
        return all_of(str.begin(), str.end(), [](char c) { return isalpha(c) || isspace(c); });
    }

    bool validateNumeric(const string& str) {
        return all_of(str.begin(), str.end(), ::isdigit);
    }

	bool validateEmail(const std::string& str) {
		size_t atPos = str.find('@');
		if (atPos == std::string::npos) {
			return false; 
		}
		if (atPos == 0 || atPos == str.length() - 1) {
			return false;
		}

		size_t dotPos = str.find('.', atPos);
		if (dotPos == std::string::npos || dotPos == str.length() - 1) {
			return false; 
		}

		if (dotPos == atPos + 1) {
			return false;
		}
		if (dotPos == str.length() - 2) {
			return false;
		}
		return true;
	}

    bool validateContactNumber(const string& str) {
        return (str.size() == 11 && str[0] == '0') || (str.size() == 13 && str.substr(0, 3) == "+92");
    }

    bool validateCnic(const string& str) {
        return str.size() == 13 && validateNumeric(str);
    }

	bool validateDate(const std::string& str) {
		int day, month, year;
		if (sscanf_s(str.c_str(), "%d-%d-%d", &day, &month, &year) == 3) {
			if (year < 1900 || year > 2100) return false;
			if (month < 1 || month > 12) return false;
			if (day < 1 || day > 31) return false;
			if (month == 2) {
				if (year % 4 == 0) {
					if (year % 100 == 0 && year % 400 != 0) return false;
					if (day > 29) return false;
				}
				else {
					if (day > 28) return false;
				}
			}
			if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) return false;
			return true;
		}
		return false;
	}

    bool validateEmploymentStatus(const string& str) {
        return str == "self" || str == "salaried" || str == "retired" || str == "unemployed";
    }

    bool validateMaritalStatus(const string& str) {
        return str == "widowed" || str == "single" || str == "married" || str == "divorced";
    }

    bool validateGender(const string& str) {
        return str == "male" || str == "female" || str == "other";
    }

    void capitalizeWords(string& str) {
        bool newWord = true;
        for (char& c : str) {
            if (newWord && isalpha(c)) {
                c = toupper(c);
                newWord = false;
            } else if (isspace(c)) {
                newWord = true;
            } else {
                c = tolower(c);
            }
        }
    }

	bool validateYesNo(const string& str) {
		return str == "yes" || str == "y" || str == "no" || str == "n";
	}

	bool validateLoanStatus(const string& str) {
		return str == "active" || str == "inactive";
	}

	bool validateLoanCategory(const string& str) {
		return str == "car" || str == "home" || str == "bike";
	}

	void toLowerCase(string& str) {
		transform(str.begin(), str.end(), str.begin(), ::tolower);
	}

	vector<string> getExistingIDs(const string& filename) {
		vector<string> ids;
		ifstream file(filename);

		if (file.is_open()) {
			string line;
			while (getline(file, line)) {
				stringstream ss(line);
				string id;
				getline(ss, id, '#');
				ids.push_back(id); 
			}
			file.close();
		}
		else {
			cout << "Could not open the file " << filename << endl;
		}
		return ids;
	}

	string generateApplicationID() {
		srand(time(0));
		vector<string> existingIDs = getExistingIDs("applications.txt");

		string newID;
		bool isUnique = false;

		while (!isUnique) {
			int randomID = rand() % 9000 + 1000;
			newID = to_string(randomID);

			isUnique = true;
			for (const string& id : existingIDs) {
				if (id == newID) {
					isUnique = false; 
					break;
				}
			}
		}

		return newID;
	}

	void selectHomeLoan() {
		HomeLoan homeLoan;
		homeLoan.initializeHomeLoan();

		while (true) {
			cout << "\nSelect area (1 or 2): ";
			string areaInput;
			getline(cin, areaInput);
			areaInput = trim(areaInput);
			if (validateArea(areaInput)) {
				selectedArea = stoi(areaInput);
				break;
			}
			cout << "Invalid area! Please enter 1 or 2.\n";
		}

		cout << "\nAvailable homes in Area " << selectedArea << ":\n";
		cout << "====================================\n";

		int availableHomes = 0;
		int* homeIndices = new int[homeLoan.numberOfLines];

		for (int i = 0; i < homeLoan.numberOfLines; i++) {
			if (homeLoan.area[i] == selectedArea) {
				availableHomes++;
				homeIndices[availableHomes - 1] = i;
				int monthlyInstallment = (homeLoan.price[i] - homeLoan.downPayment[i]) / homeLoan.installments[i];
				cout << availableHomes << ". ";
				cout << "Size: " << homeLoan.size[i] << " Marla | ";
				cout << "Price: " << homeLoan.price[i] << " | ";
				cout << "Down Payment: " << homeLoan.downPayment[i] << " | ";
				cout << "Installments: " << homeLoan.installments[i] << " months | ";
				cout << "Monthly Installment: " << monthlyInstallment << endl;
			}
		}

		if (availableHomes == 0) {
			cout << "No homes available in this area!\n";
			delete[] homeIndices;
			return;
		}

		while (true) {
			cout << "\nSelect a home (1-" << availableHomes << "): ";
			string choiceInput;
			getline(cin, choiceInput);
			choiceInput = trim(choiceInput);
			if (validateChoice(choiceInput, availableHomes)) {
				int choice = stoi(choiceInput);
				selectedHomeIndex = homeIndices[choice - 1];

				selectedHomeDetails = "Area:" + to_string(selectedArea) +
					",Size:" + to_string(homeLoan.size[selectedHomeIndex]) +
					",Price:" + to_string(homeLoan.price[selectedHomeIndex]);

				cout << "\nSelected: " << homeLoan.size[selectedHomeIndex]
					<< " Marla home in Area " << selectedArea
					<< " for " << homeLoan.price[selectedHomeIndex] << endl;
				break;
			}
			cout << "Invalid selection! Please enter a number between 1 and " << availableHomes << ".\n";
		}

		delete[] homeIndices;
	}

	void selectCarLoan() {
		CarLoan carLoan;
		carLoan.initializeCarLoan();

		while (true) {
			cout << "\nSelect car make:\n";
			cout << "1. Make 1\n";
			cout << "2. Make 2\n";
			cout << "Enter choice (1-2): ";
			getline(cin, selectedMake);
			selectedMake = trim(selectedMake);
			if (validateMake(selectedMake)) break;
			cout << "Invalid make! Please enter 1 or 2.\n";
		}

		string targetMake = (selectedMake == "1") ? "Make 1" : "Make 2";

		cout << "\nAvailable " << targetMake << " cars:\n";
		cout << "====================================\n";

		int availableCars = 0;
		int* carIndices = new int[carLoan.numberOfLines];

		for (int i = 0; i < carLoan.numberOfLines; i++) {
			if (carLoan.make[i] == targetMake) {
				availableCars++;
				carIndices[availableCars - 1] = i;
				int monthlyInstallment = (carLoan.price[i] - carLoan.downPayment[i]) / carLoan.installments[i];
				cout << availableCars << ". ";
				cout << carLoan.make[i] << " " << carLoan.model[i] << " | ";
				cout << "Engine: " << carLoan.engine[i] << "cc | ";
				cout << "Year: " << carLoan.year[i] << " | ";
				cout << "Price: " << carLoan.price[i] << " | ";
				cout << "Installments: " << carLoan.installments[i] << " months | ";
				cout << "Monthly Installment: " << monthlyInstallment << endl;
			}
		}

		if (availableCars == 0) {
			cout << "No cars available for " << targetMake << "!\n";
			delete[] carIndices;
			return;
		}

		while (true) {
			cout << "\nSelect a car (1-" << availableCars << "): ";
			string choiceInput;
			getline(cin, choiceInput);
			choiceInput = trim(choiceInput);
			if (validateChoice(choiceInput, availableCars)) {
				int choice = stoi(choiceInput);
				selectedCarIndex = carIndices[choice - 1];

				selectedCarDetails = "Make:" + carLoan.make[selectedCarIndex] +
					",Model:" + carLoan.model[selectedCarIndex] +
					",Year:" + to_string(carLoan.year[selectedCarIndex]) +
					",Price:" + to_string(carLoan.price[selectedCarIndex]);

				cout << "\nSelected: " << carLoan.make[selectedCarIndex] << " "
					<< carLoan.model[selectedCarIndex] << " ("
					<< carLoan.year[selectedCarIndex] << ") for "
					<< carLoan.price[selectedCarIndex] << endl;
				break;
			}
			cout << "Invalid selection! Please enter a number between 1 and " << availableCars << ".\n";
		}

		delete[] carIndices;
	}

	void selectScooterLoan() {
		ScooterLoan scooterLoan;
		scooterLoan.initializeScooterLoan();

		while (true) {
			selectedMake = "1";
			if (validateMake(selectedMake)) break;
		}

		cout << "\nAvailable scooters:\n";
		cout << "====================================\n";

		int availableScooters = scooterLoan.numberOfLines;
		if (availableScooters == 0) {
			cout << "No scooters available!\n";
			return;
		}

		for (int i = 0; i < availableScooters; i++) {
			int monthlyInstallment = (scooterLoan.price[i] - scooterLoan.downPayment[i]) / scooterLoan.installments[i];
			cout << (i + 1) << ". ";
			cout << scooterLoan.make[i] << " " << scooterLoan.model[i] << " | ";
			cout << "Range: " << scooterLoan.distancePerCharge[i] << "km | ";
			cout << "Speed: " << scooterLoan.maxSpeed[i] << "km/h | ";
			cout << "Price: " << scooterLoan.price[i] << " | ";
			cout << "Installments: " << scooterLoan.installments[i] << " months | ";
			cout << "Monthly Installment: " << monthlyInstallment << endl;
		}

		while (true) {
			cout << "\nSelect a scooter (1-" << availableScooters << "): ";
			string choiceInput;
			getline(cin, choiceInput);
			choiceInput = trim(choiceInput);
			if (validateChoice(choiceInput, availableScooters)) {
				int choice = stoi(choiceInput);
				selectedScooterIndex = choice - 1;

				selectedScooterDetails = "Make:" + scooterLoan.make[selectedScooterIndex] +
					",Model:" + scooterLoan.model[selectedScooterIndex] +
					",Price:" + to_string(scooterLoan.price[selectedScooterIndex]);

				cout << "\nSelected: " << scooterLoan.make[selectedScooterIndex] << " "
					<< scooterLoan.model[selectedScooterIndex] << " for "
					<< scooterLoan.price[selectedScooterIndex] << endl;
				break;
			}
			cout << "Invalid selection! Please enter a number between 1 and " << availableScooters << ".\n";
		}
	}

	void selectPersonalLoan() {
		PersonalLoan personalLoan;
		personalLoan.initializePersonalLoan();
		if (personalLoan.numberOfLines == 0) {
			cout << "No personal loans available!\n";
			loanType = "";
			return;
		}

		cout << "\nAvailable Personal Loans:\n";
		cout << "====================================\n";
		for (int i = 0; i < personalLoan.numberOfLines; i++) {
			int monthlyInstallment = (personalLoan.amount[i] - personalLoan.downPayment[i]) / personalLoan.installments[i];
			cout << (i + 1) << ". ";
			cout << "Category: " << personalLoan.category[i] << " | ";
			cout << "Amount: " << personalLoan.amount[i] << " PKR | ";
			cout << "Down Payment: " << personalLoan.downPayment[i] << " PKR | ";
			cout << "Installments: " << personalLoan.installments[i] << " months | ";
			cout << "Monthly Installment: " << monthlyInstallment << endl;
		}

		while (true) {
			cout << "\nSelect a loan (1-" << personalLoan.numberOfLines << "): ";
			string choiceInput;
			getline(cin, choiceInput);
			choiceInput = trim(choiceInput);
			if (validateChoice(choiceInput, personalLoan.numberOfLines)) {
				int choice = stoi(choiceInput);
				selectedPersonalIndex = choice - 1;
				selectedPersonalDetails = "Category:" + personalLoan.category[selectedPersonalIndex] +
					",Amount:" + to_string(personalLoan.amount[selectedPersonalIndex]) +
					",DownPayment:" + to_string(personalLoan.downPayment[selectedPersonalIndex]);
				cout << "\nSelected: " << personalLoan.category[selectedPersonalIndex]
					<< " Loan for " << personalLoan.amount[selectedPersonalIndex] << " PKR\n";
				break;
			}
			cout << "Invalid selection! Please enter a number between 1 and "
				<< personalLoan.numberOfLines << ".\n";
		}
	}

	bool validateLoanType(const string& str) {
		return str == "home" || str == "car" || str == "scooter" || str == "personal";
	}

	bool validateArea(const string& str) {
		return str == "1" || str == "2";
	}

	bool validateMake(const string& str) {
		return str == "1" || str == "2";
	}

	bool validateChoice(const string& str, int maxChoice) {
		if (!all_of(str.begin(), str.end(), ::isdigit)) return false;
		int choice = stoi(str);
		return choice >= 1 && choice <= maxChoice;
	}

	bool verifyImageFileExistence(const string& filePath) {
		ifstream file(filePath);
		return file.is_open();
	}

	bool isImageFile(const string& filePath) {
		string extensions[] = { ".jpg", ".jpeg", ".png", ".bmp", ".gif" };
		for (const auto& ext : extensions) {
			if (filePath.size() >= ext.size() && filePath.compare(filePath.size() - ext.size(), ext.size(), ext) == 0) {
				return true;
			}
		}
		return false;
	}

	bool createDirectory(const string& dirPath) {
		struct stat info;
		if (stat(dirPath.c_str(), &info) != 0) {  
			if (_mkdir(dirPath.c_str()) != 0) {
				cout << "Error creating directory: " << dirPath << endl;
				return false;
			}
		}
		return true;
	}

	bool copyImage(const string& source, const string& destination) {
		ifstream src(source, ios::binary);
		ofstream dest(destination, ios::binary);

		if (!src || !dest) {
			cout << "Error opening files for copying!" << endl;
			return false;
		}

		dest << src.rdbuf();  
		return true;
	}

	string buildRecordLine() const {
		stringstream ss;
		ss << applicationID << "#"
			<< fullName << "#"
			<< fatherName << "#"
			<< postalAddress << "#"
			<< contactNumber << "#"
			<< email << "#"
			<< cnic << "#"
			<< cnicExpiryDate << "#"
			<< employmentStatus << "#"
			<< maritalStatus << "#"
			<< gender << "#"
			<< numberOfDependents << "#"
			<< annualIncome << "#"
			<< avgElectricityBill << "#"
			<< currentElectricityBill << "#"
			<< referee1.name << "#"
			<< referee1.cnic << "#"
			<< referee1.cnicIssueDate << "#"
			<< referee1.phoneNumber << "#"
			<< referee1.emailAddress << "#"
			<< referee2.name << "#"
			<< referee2.cnic << "#"
			<< referee2.cnicIssueDate << "#"
			<< referee2.phoneNumber << "#"
			<< referee2.emailAddress << "#"
			<< images.cnicFrontPath << "#"
			<< images.cnicBackPath << "#"
			<< images.electricityBillPath << "#"
			<< images.salarySlipPath << "#"
			<< applicationStatus << "#"
			<< loanType << "#"
			<< selectedMake << "#"
			<< selectedArea << "#"
			<< selectedHomeIndex << "#"
			<< selectedCarIndex << "#"
			<< selectedScooterIndex << "#"
			<< selectedHomeDetails << "#"
			<< selectedCarDetails << "#"
			<< selectedScooterDetails << "#"
			<< selectedPersonalIndex << "#"
			<< selectedPersonalDetails << "#";
		return ss.str();
	}

	void writeApplicationRecord(bool isFinalSubmission) {
		if (applicationID.empty()) {
			cout << "Error: Application ID is empty. Cannot save application.\n";
			return;
		}

		vector<string> lines;
		bool updated = false;

		ifstream in("applications.txt");
		if (in) {
			string line;
			while (getline(in, line)) {
				if (line.empty()) continue;
				stringstream ss(line);
				string existingId;
				getline(ss, existingId, '#');
				if (existingId == applicationID) {
					lines.push_back(buildRecordLine());
					updated = true;
				}
				else {
					lines.push_back(line);
				}
			}
			in.close();
		}

		if (!updated) {
			lines.push_back(buildRecordLine());
		}

		ofstream out("applications.txt");
		if (!out) {
			cout << "Error! Could not open applications.txt for writing!\n";
			return;
		}

		for (const string& l : lines) {
			out << l << endl;
		}
		out.close();

		if (isFinalSubmission) {
			cout << "\n====================================\n";
			cout << "Application submitted successfully!\n";
			cout << "Application ID: " << applicationID << endl;
			cout << "Status: " << applicationStatus << endl;
			cout << "====================================\n";
		}
	}

public:
    string fullName, fatherName, postalAddress, contactNumber, email, cnic, cnicExpiryDate;
    string employmentStatus, maritalStatus, gender;
    int numberOfDependents;
    float annualIncome, avgElectricityBill, currentElectricityBill;

	bool hasExistingLoan;
	Referee referee1, referee2;
	ApplicantImages images;
	string applicationID;
	string applicationStatus; 
	int selectedArea;      

	string loanType;
	string selectedMake;      
	int selectedHomeIndex;    
	int selectedCarIndex;     
	int selectedScooterIndex; 
	string selectedHomeDetails; 
	string selectedCarDetails;  
	string selectedScooterDetails; 
	int selectedPersonalIndex;
	string selectedPersonalDetails;

	LoanSeeker() {
		selectedPersonalIndex = -1;
		selectedPersonalDetails = "";
		hasExistingLoan = false;
		applicationID = "";
		annualIncome = 0.0f;
		avgElectricityBill = 0.0f;
		currentElectricityBill = 0.0f;
		numberOfDependents = 0;
		applicationStatus = "submitted";
		selectedArea = 0;
		loanType = "";
		selectedMake = "";
		selectedArea = 0;
		selectedHomeIndex = -1;
		selectedCarIndex = -1;
		selectedScooterIndex = -1;
		selectedHomeDetails = "";
		selectedCarDetails = "";
		selectedScooterDetails = "";
	}

    void inputFullName() {
        while (true) {
            cout << "Enter Full Name: ";
            getline(cin, fullName);
			fullName = trim(fullName);
            if (validateAlphabetString(fullName)) {
                capitalizeWords(fullName);
                break;
            } else {
                cout << "Full Name should contain only alphabets and spaces!\n";
            }
        }
    }

    void inputFatherName() {
        while (true) {
            cout << "Enter Father's Name: ";
            getline(cin, fatherName);
			fatherName = trim(fatherName);
            if (validateAlphabetString(fatherName)) {
                capitalizeWords(fatherName);
                break;
            } else {
                cout << "Father's Name should contain only alphabets and spaces!\n";
            }
        }
    }

    void inputPostalAddress() {
        cout << "Enter Postal Address: ";
        getline(cin, postalAddress);
		postalAddress = trim(postalAddress);
        capitalizeWords(postalAddress);
    }

    void inputContactNumber() {
        while (true) {
            cout << "Enter Contact Number (11 or 13 digits): ";
            getline(cin, contactNumber);
			contactNumber = trim(contactNumber);
            if (validateContactNumber(contactNumber)) break;
            cout << "Invalid Contact Number! Must be 11 digits starting with 0 or 13 digits starting with +92.\n";
        }
    }

    void inputEmail() {
        while (true) {
            cout << "Enter Email Address: ";
            getline(cin, email);
			email = trim(email);
            transform(email.begin(), email.end(), email.begin(), ::tolower);
            if (validateEmail(email)) break;
            cout << "Invalid Email Address! Example email: erenyeager@yeagerists.corps\n";
        }
    }

    void inputCnic() {
        while (true) {
            cout << "Enter CNIC (13 digits, no dashes): ";
            getline(cin, cnic);
			cnic = trim(cnic);
            if (validateCnic(cnic)) break;
            cout << "Invalid CNIC! Must be exactly 13 digits.\n";
        }
    }

    void inputCnicExpiryDate() {
        while (true) {
            cout << "Enter CNIC Expiry Date (DD-MM-YYYY): ";
            getline(cin, cnicExpiryDate);
			cnicExpiryDate = trim(cnicExpiryDate);
            if (validateDate(cnicExpiryDate)) break;
            cout << "Invalid date! Format must be DD-MM-YYYY with valid day/month/year values.\n";
        }
    }

    void inputEmploymentStatus() {
        while (true) {
            cout << "Enter Employment Status (self, salaried, retired, unemployed): ";
            getline(cin, employmentStatus);
			employmentStatus = trim(employmentStatus);
            transform(employmentStatus.begin(), employmentStatus.end(), employmentStatus.begin(), ::tolower);
            if (validateEmploymentStatus(employmentStatus)) break;
            cout << "Invalid Employment Status!\n";
        }
    }

    void inputMaritalStatus() {
        while (true) {
            cout << "Enter Marital Status (widowed, single, married, divorced): ";
            getline(cin, maritalStatus);
			maritalStatus = trim(maritalStatus);
            transform(maritalStatus.begin(), maritalStatus.end(), maritalStatus.begin(), ::tolower);
            if (validateMaritalStatus(maritalStatus)) break;
            cout << "Invalid Marital Status!\n";
        }
    }

    void inputGender() {
        while (true) {
            cout << "Enter Gender (male, female, other): ";
            getline(cin, gender);
			gender = trim(gender);
            transform(gender.begin(), gender.end(), gender.begin(), ::tolower);
            if (validateGender(gender)) break;
            cout << "Invalid Gender!\n";
        }
    }

    void inputNumberOfDependents() {
        string input;
        while (true) {
            cout << "Enter Number of Dependents: ";
            getline(cin, input);
			input = trim(input);
            if (validateNumeric(input)) {
                numberOfDependents = stoi(input);
                break;
            } else {
                cout << "Invalid number! Must contain only digits.\n";
            }
        }
    }

    void inputAnnualIncome() {
        while (true) {
            cout << "Enter Annual Income (no commas): ";
            string input;
            getline(cin, input);
			input = trim(input);
            if (validateNumeric(input)) {
                annualIncome = stof(input);
                break;
            } else {
                cout << "Invalid input! Must contain only digits.\n";
            }
        }
    }

    void inputAvgElectricityBill() {
        while (true) {
            cout << "Enter Average Electricity Bill (no commas): ";
            string input;
            getline(cin, input);
			input = trim(input);
            if (validateNumeric(input)) {
                avgElectricityBill = stof(input);
                break;
            } else {
                cout << "Invalid input! Must contain only digits.\n";
            }
        }
    }

    void inputCurrentElectricityBill() {
        while (true) {
            cout << "Enter Current Electricity Bill (exact): ";
            string input;
            getline(cin, input);
			input = trim(input);
            if (validateNumeric(input)) {
                currentElectricityBill = stof(input);
                break;
            } else {
                cout << "Invalid input! Must contain only digits.\n";
            }
        }
    }

	void inputRefereeDetails() {
		cout << "\n=== REFEREE DETAILS ===\n";

		cout << "\n--- Referee 1 Information ---\n";

		while (true) {
			cout << "Enter Name of Referee 1: ";
			getline(cin, referee1.name);
			referee1.name = trim(referee1.name);
			if (validateAlphabetString(referee1.name)) {
				capitalizeWords(referee1.name);
				break;
			}
			cout << "Name should contain only alphabets and spaces!\n";
		}

		while (true) {
			cout << "Enter CNIC of Referee 1 (13 digits, no dashes): ";
			getline(cin, referee1.cnic);
			referee1.cnic = trim(referee1.cnic);
			if (referee1.cnic == cnic) {
				cout << "Referee CNIC cannot be same as applicant CNIC.\n";
				continue;
			}
			trim(referee1.cnic);
			if (validateCnic(referee1.cnic)) break;
			cout << "Invalid CNIC! Must be exactly 13 digits.\n";
		}

		while (true) {
			cout << "Enter CNIC Issue Date of Referee 1 (DD-MM-YYYY): ";
			getline(cin, referee1.cnicIssueDate);
			referee1.cnicIssueDate = trim(referee1.cnicIssueDate);
			if (validateDate(referee1.cnicIssueDate)) break;
			cout << "Invalid date! Format must be DD-MM-YYYY with valid day/month/year values.\n";
		}

		while (true) {
			cout << "Enter Phone Number of Referee 1 (11 or 13 digits): ";
			getline(cin, referee1.phoneNumber);
			referee1.phoneNumber = trim(referee1.phoneNumber);
			if (referee1.phoneNumber == contactNumber) {
				cout << "Referee contact number cannot be same as application contact number.\n";
				continue;
			}
			if (validateContactNumber(referee1.phoneNumber)) break;
			cout << "Invalid Phone Number! Must be 11 digits starting with 0 or 13 digits starting with +92.\n";
		}

		while (true) {
			cout << "Enter Email Address of Referee 1: ";
			getline(cin, referee1.emailAddress);
			referee1.emailAddress = trim(referee1.emailAddress);
			toLowerCase(referee1.emailAddress);
			if (referee1.emailAddress == email) {
				cout << "Referee email cannot be same as applicant email.\n";
				continue;
			}
			if (validateEmail(referee1.emailAddress)) break;
			cout << "Invalid Email Address! Example email: erenyeager@yeagerists.corps\n";
		}

		cout << "\n--- Referee 2 Information ---\n";

		while (true) {
			cout << "Enter Name of Referee 2: ";
			getline(cin, referee2.name);
			referee2.name = trim(referee2.name);
			if (validateAlphabetString(referee2.name)) {
				capitalizeWords(referee2.name);
				break;
			}
			cout << "Name should contain only alphabets and spaces!\n";
		}

		while (true) {
			cout << "Enter CNIC of Referee 2 (13 digits, no dashes): ";
			getline(cin, referee2.cnic);
			referee2.cnic = trim(referee2.cnic);
			if (referee2.cnic == cnic) {
				cout << "Referee CNIC cannot be same as applicant CNIC.\n";
				continue;
			}
			if (referee1.cnic == referee2.cnic) {
				cout << "Referees cannot have the same CNIC.\n";
				continue;
			}
			if (validateCnic(referee2.cnic)) break;
			cout << "Invalid CNIC! Must be exactly 13 digits.\n";
		}

		while (true) {
			cout << "Enter CNIC Issue Date of Referee 2 (DD-MM-YYYY): ";
			getline(cin, referee2.cnicIssueDate);
			referee2.cnicIssueDate = trim(referee2.cnicIssueDate);
			if (validateDate(referee2.cnicIssueDate)) break;
			cout << "Invalid date! Format must be DD-MM-YYYY with valid day/month/year values.\n";
		}

		while (true) {
			cout << "Enter Phone Number of Referee 2 (11 or 13 digits): ";
			getline(cin, referee2.phoneNumber);
			referee2.phoneNumber = trim(referee2.phoneNumber);
			if (referee2.phoneNumber == contactNumber) {
				cout << "Referee contact number cannot be same as applicant contact number.\n";
				continue;
			}
			if (referee1.phoneNumber == referee2.phoneNumber) {
				cout << "Referees cannot have the same contact number.\n";
				continue;
			}
			if (validateContactNumber(referee2.phoneNumber)) break;
			cout << "Invalid Phone Number! Must be 11 digits starting with 0 or 13 digits starting with +92.\n";
		}

		while (true) {
			cout << "Enter Email Address of Referee 2: ";
			getline(cin, referee2.emailAddress);
			referee2.phoneNumber = trim(referee2.phoneNumber);
			toLowerCase(referee2.emailAddress);
			if (referee1.emailAddress == email) {
				cout << "Referee email cannot be same as applicant email.\n";
				continue;
			}
			if (referee1.emailAddress == referee2.emailAddress) {
				cout << "Referees cannot have the same email.\n";
				continue;
			}
			if (validateEmail(referee2.emailAddress)) break;
			cout << "Invalid Email Address! Example email: erenyeager@yeagerists.corps\n";
		}
	}

	void inputImagePaths(string applicationID) {
		cout << "\n=== DOCUMENT IMAGES ===\n";


		string dataFolder = "./data";
		string appFolder = dataFolder + "/" + applicationID;

		createDirectory(dataFolder);
		createDirectory(appFolder);

		cout << "Your Application ID is: " << applicationID << endl;
		cout << "All images will be saved in folder: " << appFolder << endl;
		cout << "\nPlease provide the FULL FILE PATHS for the following documents:\n";
		cout << "(They will be copied automatically into: " << appFolder << ")\n\n";

		string filename;
		string sourcePath;
		string destPath;

		while (true) {
			cout << "Enter file path for CNIC Front (e.g. C:/docs/cnic_front.jpg): ";
			getline(cin, sourcePath);
			sourcePath = trim(sourcePath);

			if (!verifyImageFileExistence(sourcePath)) {
				cout << "File does not exist. Try again.\n";
				continue;
			}
			if (!isImageFile(sourcePath)) {
				cout << "Not a valid image file. Try again.\n";
				continue;
			}

			filename = sourcePath.substr(sourcePath.find_last_of("/\\") + 1);
			destPath = appFolder + "/" + filename;

			copyImage(sourcePath, destPath);
			images.cnicFrontPath = destPath;
			break;
		}

		while (true) {
			cout << "Enter file path for CNIC Back: ";
			getline(cin, sourcePath);
			sourcePath = trim(sourcePath);

			if (!verifyImageFileExistence(sourcePath)) {
				cout << "File does not exist. Try again.\n";
				continue;
			}
			if (!isImageFile(sourcePath)) {
				cout << "Not a valid image file. Try again.\n";
				continue;
			}

			filename = sourcePath.substr(sourcePath.find_last_of("/\\") + 1);
			destPath = appFolder + "/" + filename;

			copyImage(sourcePath, destPath);
			images.cnicBackPath = destPath;
			break;
		}

		while (true) {
			cout << "Enter file path for Recent Electricity Bill: ";
			getline(cin, sourcePath);
			sourcePath = trim(sourcePath);

			if (!verifyImageFileExistence(sourcePath)) {
				cout << "File does not exist. Try again.\n";
				continue;
			}
			if (!isImageFile(sourcePath)) {
				cout << "Not a valid image file. Try again.\n";
				continue;
			}

			filename = sourcePath.substr(sourcePath.find_last_of("/\\") + 1);
			destPath = appFolder + "/" + filename;

			copyImage(sourcePath, destPath);
			images.electricityBillPath = destPath;
			break;
		}

		while (true) {
			cout << "Enter file path for Salary Slip/Bank Statement: ";
			getline(cin, sourcePath);
			sourcePath = trim(sourcePath);

			if (!verifyImageFileExistence(sourcePath)) {
				cout << "File does not exist. Try again.\n";
				continue;
			}
			if (!isImageFile(sourcePath)) {
				cout << "Not a valid image file. Try again.\n";
				continue;
			}

			filename = sourcePath.substr(sourcePath.find_last_of("/\\") + 1);
			destPath = appFolder + "/" + filename;

			copyImage(sourcePath, destPath);
			images.salarySlipPath = destPath;
			break;
		}

		cout << "\nImage paths validated and copied successfully!\n";
	}

	void inputLoanTypeAndSelection() {
		cout << "\n=== LOAN TYPE SELECTION ===\n";

		while (true) {
			cout << "Select loan type (home/car/scooter/personal): ";
			getline(cin, loanType);
			loanType = trim(loanType);
			transform(loanType.begin(), loanType.end(), loanType.begin(), ::tolower);
			if (validateLoanType(loanType)) break;
			cout << "Invalid loan type! Please enter 'home', 'car', 'scooter' or 'personal'.\n";
		}

		if (loanType == "home") {
			selectHomeLoan();
		}
		else if (loanType == "car") {
			selectCarLoan();
		}
		else if (loanType == "scooter") {
			selectScooterLoan();
		}
		else if (loanType == "personal") {
			selectPersonalLoan();
		}
	}

	void displaySummary() {
		cout << "\n====================================\n";
		cout << "   APPLICATION SUMMARY\n";
		cout << "====================================\n\n";
		cout << "Application ID: " << applicationID << endl;
		cout << "Full Name: " << fullName << endl;
		cout << "Father's Name: " << fatherName << endl;
		cout << "CNIC: " << cnic << endl;
		cout << "Contact: " << contactNumber << endl;
		cout << "Email: " << email << endl;
		cout << "Employment: " << employmentStatus << endl;
		cout << "Annual Income: " << annualIncome << endl;

		cout << "\n--- LOAN INFORMATION ---\n";
		cout << "Loan Type: " << (loanType.empty() ? "Not Selected" : loanType) << endl;

		if (loanType == "home") {
			cout << "Selected Area: " << selectedArea << endl;
			if (!selectedHomeDetails.empty()) {
				stringstream ss(selectedHomeDetails);
				string token;
				while (getline(ss, token, ',')) {
					size_t colonPos = token.find(':');
					if (colonPos != string::npos) {
						string key = token.substr(0, colonPos);
						string value = token.substr(colonPos + 1);
						if (!key.empty()) {
							key[0] = toupper(key[0]);
						}
						cout << key << ": " << value << endl;
					}
					else {
						cout << token << endl;
					}
				}
			}
			else {
				cout << "Home Details: Not specified\n";
			}
		}
		else if (loanType == "car") {
			if (!selectedCarDetails.empty()) {
				stringstream ss(selectedCarDetails);
				string token;
				while (getline(ss, token, ',')) {
					size_t colonPos = token.find(':');
					if (colonPos != string::npos) {
						string key = token.substr(0, colonPos);
						string value = token.substr(colonPos + 1);
						if (!key.empty()) {
							key[0] = toupper(key[0]);
						}
						cout << key << ": " << value << endl;
					}
					else {
						cout << token << endl;
					}
				}
			}
			else {
				cout << "Car Details: Not specified\n";
			}
			if (!selectedMake.empty()) {
				cout << "Selected Make: " << "Make 1" << endl;
			}
		}
		else if (loanType == "scooter") {
			if (!selectedScooterDetails.empty()) {
				stringstream ss(selectedScooterDetails);
				string token;
				while (getline(ss, token, ',')) {
					size_t colonPos = token.find(':');
					if (colonPos != string::npos) {
						string key = token.substr(0, colonPos);
						string value = token.substr(colonPos + 1);
						if (!key.empty()) {
							key[0] = toupper(key[0]);
						}
						cout << key << ": " << value << endl;
					}
					else {
						cout << token << endl;
					}
				}
			}
			else {
				cout << "Scooter Details: Not specified\n";
			}
			if (!selectedMake.empty()) {
				cout << "Selected Make: " << (selectedMake == "1" ? "Make 1" : "Make 2") << endl;
			}
		}
		else if (loanType == "personal") {
			if (!selectedPersonalDetails.empty()) {
				stringstream ss(selectedPersonalDetails);
				string token;
				while (getline(ss, token, ',')) {
					size_t colonPos = token.find(':');
					if (colonPos != string::npos) {
						string key = token.substr(0, colonPos);
						string value = token.substr(colonPos + 1);
						if (!key.empty()) {
							key[0] = toupper(key[0]);
						}
						cout << key << ": " << value << endl;
					}
				}
			}
			else {
				cout << "Personal Loan Details: Not specified\n";
			}
		}
		else {
			cout << "No loan type selected\n";
		}

		cout << "\n--- REFEREE INFORMATION ---\n";
		cout << "Referee 1: " << referee1.name << " (" << referee1.cnic << ")" << endl;
		cout << "Referee 2: " << referee2.name << " (" << referee2.cnic << ")" << endl;

		cout << "\n--- DOCUMENT INFORMATION ---\n";
		cout << "CNIC Front: " << images.cnicFrontPath << endl;
		cout << "CNIC Back: " << images.cnicBackPath << endl;
		cout << "Electricity Bill: " << images.electricityBillPath << endl;
		cout << "Salary Slip: " << images.salarySlipPath << endl;

		cout << "\n====================================\n";
	}

	bool confirmSubmission() {
		string response;
		while (true) {
			cout << "\nDo you want to submit this application? (yes/no): ";
			getline(cin, response);
			toLowerCase(response);
			response = trim(response);
			if (validateYesNo(response)) {
				return (response == "yes" || response == "y");
			}
			cout << "Invalid input! Please enter 'yes' or 'no'.\n";
		}
	}

	void initializeApplicationID() {
		if (applicationID.empty()) {
			applicationID = generateApplicationID();
		}
	}

	void saveCheckpoint(const string& status) {
		applicationStatus = status;   
		writeApplicationRecord(false);
	}

	void saveToFile() {
		applicationStatus = "submitted";
		writeApplicationRecord(true);
	}

	void removeFromFile(const string& filename, const string& numberToRemove) {
		ifstream inFile(filename);

		vector<string> lines;
		string line;

		while (getline(inFile, line)) {
			if (line.substr(0, line.find('#')) != numberToRemove) {
				lines.push_back(line);
			}
		}
		inFile.close();

		if (lines.size() == 0) {
			return;
		}

		ofstream outFile(filename);

		for (const auto& line : lines) {
			outFile << line << endl;
		}

		outFile.close();
	}
};

bool loadIncompleteApplication(const string& appID, const string& cnic, LoanSeeker& applicant, string& statusOut) {
	ifstream file("applications.txt");
	if (!file) {
		cout << "No applications file found.\n";
		return false;
	}

	string line;
	while (getline(file, line)) {
		if (line.empty()) continue;

		stringstream ss(line);
		vector<string> tokens;
		string token;

		while (getline(ss, token, '#')) {
			tokens.push_back(token);
		}

		if (tokens.size() < 30) {
			continue;
		}

		string fileAppID = tokens[0];
		string fileCNIC = tokens[6];
		string status = tokens[29];

		if (fileAppID == appID && fileCNIC == cnic) {
			if (status != "C1" && status != "C2" && status != "C3") {
				cout << "Application " << appID << " is already completed or not eligible for update.\n";
				file.close();
				return false;
			}

			statusOut = status;

			applicant.applicationID = tokens[0];
			applicant.fullName = tokens[1];
			applicant.fatherName = tokens[2];
			applicant.postalAddress = tokens[3];
			applicant.contactNumber = tokens[4];
			applicant.email = tokens[5];
			applicant.cnic = tokens[6];
			applicant.cnicExpiryDate = tokens[7];
			applicant.employmentStatus = tokens[8];
			applicant.maritalStatus = tokens[9];
			applicant.gender = tokens[10];

			applicant.numberOfDependents = tokens[11].empty() ? 0 : stoi(tokens[11]);
			applicant.annualIncome = tokens[12].empty() ? 0.0f : stof(tokens[12]);
			applicant.avgElectricityBill = tokens[13].empty() ? 0.0f : stof(tokens[13]);
			applicant.currentElectricityBill = tokens[14].empty() ? 0.0f : stof(tokens[14]);

			applicant.referee1.name = tokens[15];
			applicant.referee1.cnic = tokens[16];
			applicant.referee1.cnicIssueDate = tokens[17];
			applicant.referee1.phoneNumber = tokens[18];
			applicant.referee1.emailAddress = tokens[19];

			applicant.referee2.name = tokens[20];
			applicant.referee2.cnic = tokens[21];
			applicant.referee2.cnicIssueDate = tokens[22];
			applicant.referee2.phoneNumber = tokens[23];
			applicant.referee2.emailAddress = tokens[24];

			applicant.images.cnicFrontPath = tokens[25];
			applicant.images.cnicBackPath = tokens[26];
			applicant.images.electricityBillPath = tokens[27];
			applicant.images.salarySlipPath = tokens[28];

			applicant.applicationStatus = status;

			if (tokens.size() > 30) applicant.loanType = tokens[30];
			if (tokens.size() > 31) applicant.selectedMake = tokens[31];
			applicant.selectedArea = (tokens.size() > 32 && !tokens[32].empty()) ? stoi(tokens[32]) : 0;
			applicant.selectedHomeIndex = (tokens.size() > 33 && !tokens[33].empty()) ? stoi(tokens[33]) : -1;
			applicant.selectedCarIndex = (tokens.size() > 34 && !tokens[34].empty()) ? stoi(tokens[34]) : -1;
			applicant.selectedScooterIndex = (tokens.size() > 35 && !tokens[35].empty()) ? stoi(tokens[35]) : -1;
			if (tokens.size() > 36) applicant.selectedHomeDetails = tokens[36];
			if (tokens.size() > 37) applicant.selectedCarDetails = tokens[37];
			if (tokens.size() > 38) applicant.selectedScooterDetails = tokens[38];
			applicant.selectedPersonalIndex = (tokens.size() > 39 && !tokens[39].empty()) ? stoi(tokens[39]) : -1;
			if (tokens.size() > 40) applicant.selectedPersonalDetails = tokens[40];

			file.close();
			return true;
		}
	}

	cout << "No matching incomplete application found for the given ID and CNIC.\n";
	file.close();
	return false;
}

void checkApplicationsByCNIC(const string& cnic) {
	ifstream file("applications.txt");
	if (!file) {
		cout << "No applications file found.\n";
		return;
	}

	int submitted = 0, approved = 0, rejected = 0;
	string line;
	vector<string> approvedAppIDs;
	while (getline(file, line)) {
		stringstream ss(line);
		string token;
		int fieldCount = 0;
		string fileCnic, status, appID, loanType;

		while (getline(ss, token, '#')) {
			fieldCount++;
			if (fieldCount == 1) appID = token;
			if (fieldCount == 7) fileCnic = token;
			if (fieldCount == 30) status = token;
			if (fieldCount == 31) loanType = token;
		}

		if (fileCnic == cnic) {
			if (status == "submitted") submitted++;
			else if (status == "approved") {
				approved++;
				approvedAppIDs.push_back(appID);
			}
			else if (status == "rejected") rejected++;
			else {
				cout << "Incomplete Application!\n"<< endl;
				return;
			}
		}
	}
	file.close();

	cout << "\n=============================================\n";
	cout << "   APPLICATION STATUS FOR CNIC " << cnic << "\n";
	cout << "=============================================\n";
	cout << "Submitted Applications: " << submitted << endl;
	cout << "Approved Applications: " << approved << endl;
	cout << "Rejected Applications: " << rejected << endl;
	cout << "Total Applications: " << (submitted + approved + rejected) << endl;
	if (approved > 0) {
		cout << "\n--- APPROVED APPLICATION DETAILS ---\n";

		ifstream approvedFile("approved.txt");
		if (approvedFile) {
			bool foundPaymentPlans = false;

			while (getline(approvedFile, line)) {
				stringstream ss(line);
				string token;
				int fieldCount = 0;
				string approvedAppID, approvedCNIC, loanType, description;
				int price, downPayment, installments, monthlyInstallment, totalAmount, startMonth;
				string paymentStatus;
				int remainingAmount, paidAmount;

				while (getline(ss, token, '#')) {
					fieldCount++;
					switch (fieldCount) {
					case 1: approvedAppID = token; break;
					case 2: approvedCNIC = token; break;
					case 3: loanType = token; break;
					case 4: description = token; break;
					case 5: price = stoi(token); break;
					case 6: downPayment = stoi(token); break;
					case 7: installments = stoi(token); break;
					case 8: monthlyInstallment = stoi(token); break;
					case 9: totalAmount = stoi(token); break;
					case 10: startMonth = stoi(token); break;
					case 11: paymentStatus = token; break;
					case 12: remainingAmount = stoi(token); break;
					case 13: paidAmount = stoi(token); break;
					}
				}

				if (approvedCNIC == cnic) {
					foundPaymentPlans = true;
					cout << "\nApplication ID: " << approvedAppID << endl;
					cout << "Loan Type: " << loanType << endl;
					cout << "Description: " << description << endl;
					cout << "Total Price: " << price << endl;
					cout << "Down Payment: " << downPayment << endl;
					cout << "Monthly Installment: " << monthlyInstallment << endl;
					cout << "Total Loan Amount: " << totalAmount << endl;
					cout << "Installment Period: " << installments << " months" << endl;
					cout << "Starting Month: " << startMonth << endl;
					cout << "Payment Status: " << paymentStatus << endl;
					cout << "Amount Paid: " << paidAmount << endl;
					cout << "Remaining Amount: " << remainingAmount << endl;

					double progress = (totalAmount > 0) ? ((double)paidAmount / totalAmount) * 100 : 0;
					cout << "Payment Progress: " << progress << "%" << endl;
					cout << "------------------------" << endl;
				}
			}
			approvedFile.close();

			if (!foundPaymentPlans) {
				cout << "No payment plans generated for approved applications.\n";
			}
		}
		else {
			cout << "No payment plans found for approved applications.\n";
		}
	}
	cout << "====================================\n\n";
}

void generateMonthlyPlan(const string& cnic) {
	ifstream file("applications.txt");
	if (!file) {
		cout << "No applications file found.\n";
		return;
	}

	string line;
	bool foundApproved = false;

	set<string> applicationsWithPlans;
	ifstream approvedFile("approved.txt");
	if (approvedFile) {
		string approvedLine;
		while (getline(approvedFile, approvedLine)) {
			stringstream ss(approvedLine);
			string appID;
			getline(ss, appID, '#');
			applicationsWithPlans.insert(appID);
		}
		approvedFile.close();
	}

	HomeLoan homeLoan;
	homeLoan.initializeHomeLoan();

	ScooterLoan scooterLoan;
	scooterLoan.initializeScooterLoan();

	CarLoan carLoan;
	carLoan.initializeCarLoan();

	PersonalLoan personalLoan;
	personalLoan.initializePersonalLoan();

	while (getline(file, line)) {
		stringstream ss(line);
		string token;
		int fieldCount = 0;
		string fileCnic, status, appID, loanType, selectedMake;
		int selectedArea = 0, selectedHomeIndex = -1, selectedCarIndex = -1, selectedScooterIndex = -1;
		string selectedHomeDetails, selectedCarDetails, selectedScooterDetails;
		int selectedPersonalIndex = -1;
		string selectedPersonalDetails = "";

		while (getline(ss, token, '#')) {
			fieldCount++;
			switch (fieldCount) {
				case 1: appID = token; break;
				case 7: fileCnic = token; break;
				case 30: status = token; break;
				case 31: loanType = token; break;
				case 32: selectedMake = token; break;
				case 33: if (!token.empty()) selectedArea = stoi(token); break;
				case 34: if (!token.empty()) selectedHomeIndex = stoi(token); break;
				case 35: if (!token.empty()) selectedCarIndex = stoi(token); break;
				case 36: if (!token.empty()) selectedScooterIndex = stoi(token); break;
				case 37: selectedHomeDetails = token; break;
				case 38: selectedCarDetails = token; break;
				case 39: selectedScooterDetails = token; break;
				case 40: if (!token.empty()) selectedPersonalIndex = stoi(token); break;
				case 41: selectedPersonalDetails = token; break;
			}
		}

			if (fileCnic == cnic && status == "approved" &&
				!loanType.empty() && loanType != "0" && loanType != "default" &&
				applicationsWithPlans.find(appID) == applicationsWithPlans.end()) {

				foundApproved = true;

				int price = 0, downPayment = 0, installments = 0;
				string description = "";
				bool loanFound = false;

				if (loanType == "home" && selectedHomeIndex != -1) {
					for (int i = 0; i < homeLoan.numberOfLines; i++) {
						if (i == selectedHomeIndex) {
							price = homeLoan.price[i];
							downPayment = homeLoan.downPayment[i];
							installments = homeLoan.installments[i];
							description = "Home in Area " + to_string(homeLoan.area[i]) +
								", Size: " + to_string(homeLoan.size[i]) + " Marla";
							loanFound = true;
							break;
						}
					}
				}
				else if (loanType == "car" && selectedCarIndex != -1) {
					for (int i = 0; i < carLoan.numberOfLines; i++) {
						if (i == selectedCarIndex) {
							price = carLoan.price[i];
							downPayment = carLoan.downPayment[i];
							installments = carLoan.installments[i];
							description = carLoan.make[i] + " " + carLoan.model[i] +
								" (" + to_string(carLoan.year[i]) + ")";
							loanFound = true;
							break;
						}
					}
				}
				else if (loanType == "scooter" && selectedScooterIndex != -1) {
					for (int i = 0; i < scooterLoan.numberOfLines; i++) {
						if (i == selectedScooterIndex) {
							price = scooterLoan.price[i];
							downPayment = scooterLoan.downPayment[i];
							installments = scooterLoan.installments[i];
							description = scooterLoan.make[i] + " " + scooterLoan.model[i] + " Scooter";
							loanFound = true;
							break;
						}
					}
				}
				else if (loanType == "personal" && selectedPersonalIndex != -1) {
					for (int i = 0; i < personalLoan.numberOfLines; i++) {
						if (i == selectedPersonalIndex) {
							description = personalLoan.category[i];
							downPayment = personalLoan.downPayment[i];
							installments = personalLoan.installments[i];
							price = personalLoan.amount[i];
							loanFound = true;
							break;
						}
					}
				}

				if (!loanFound || price == 0) {
					cout << "Error: Could not find matching loan data for application " << appID << ".\n";
					cout << "Loan Type: " << loanType << "\nIndex: ";
					if (loanType == "home") cout << selectedHomeIndex;
					else if (loanType == "car") cout << selectedCarIndex;
					else if (loanType == "scooter") cout << selectedScooterIndex;
					else if (loanType == "personal") cout << selectedPersonalIndex;
					cout << endl;
					continue;
				}

			cout << "\n====================================\n";
			cout << "   GENERATING PAYMENT PLAN\n";
			cout << "====================================\n";
			cout << "Application ID: " << appID << endl;
			cout << "Loan Type: " << loanType << endl;
			cout << "Description: " << description << endl;
			cout << "Total Price: " << price << endl;
			cout << "Down Payment: " << downPayment << endl;
			cout << "Installment Period: " << installments << " months" << endl;
			cout << "====================================\n\n";

			string months[] = { "January", "February", "March", "April", "May", "June",
							   "July", "August", "September", "October", "November", "December" };
			int startMonth;
			while (true) {
				cout << "Enter starting month (1-12): ";
				string input;
				getline(cin, input);
				input = trim(input);
				if (all_of(input.begin(), input.end(), ::isdigit)) {
					startMonth = stoi(input);
					if (startMonth >= 1 && startMonth <= 12) break;
				}
				cout << "Invalid month! Please enter a number between 1 and 12.\n";
			}

			int monthlyInstallment = (price - downPayment) / installments;
			int remainingAmount = price - downPayment;
			int totalAmountToPay = price - downPayment;

			cout << "\n====================================\n";
			cout << "   MONTHLY PAYMENT PLAN\n";
			cout << "====================================\n";
			cout << "Monthly Installment: " << monthlyInstallment << endl;
			cout << "Total Amount to Pay: " << totalAmountToPay << endl;
			cout << "====================================\n\n";

			int currentMonth = startMonth - 1;
			for (int i = 1; i <= installments; i++) {
				cout << "Month " << i << " (" << months[currentMonth] << "): " << monthlyInstallment << endl;
				remainingAmount -= monthlyInstallment;
				currentMonth = (currentMonth + 1) % 12;

				if (i % 6 == 0 && i != installments) {
					cout << "  --> Remaining Balance: " << remainingAmount << endl << endl;
				}
			}

			cout << "\nFinal Balance: 0" << endl;
			cout << "====================================\n\n";

			ofstream approvedFileOut("approved.txt", ios::app);
			if (approvedFileOut) {
				approvedFileOut << appID << "#"
					<< cnic << "#"
					<< loanType << "#"
					<< description << "#"
					<< price << "#"
					<< downPayment << "#"
					<< installments << "#"
					<< monthlyInstallment << "#"
					<< totalAmountToPay << "#"
					<< startMonth << "#"
					<< "active" << "#"  
					<< totalAmountToPay << "#" 
					<< "0" << "#" 
					<< endl;

				approvedFileOut.close();
				cout << "Payment plan saved successfully for Application ID: " << appID << endl;
			}
			else {
				cout << "Warning: Could not save payment plan.\n";
			}
		}
	}

	file.close();

	if (!foundApproved) {
		cout << "\nNo approved applications found for CNIC: " << cnic << " that need payment plans.\n";
		cout << "Note: Payment plans are only generated for approved applications that don't already have plans.\n";
	}
}

void displayAllLoanDetailsByCNIC(const string& cnic) {
	ifstream approvedFile("approved.txt");
	if (!approvedFile) {
		cout << "No approved loans file found.\n";
		return;
	}

	string line;
	bool foundLoans = false;
	int loanCount = 0;

	cout << "\n=============================================\n";
	cout << "   ALL LOAN DETAILS FOR CNIC: " << cnic << "\n";
	cout << "=============================================\n";

	while (getline(approvedFile, line)) {
		stringstream ss(line);
		string token;
		int fieldCount = 0;

		string approvedAppID, approvedCNIC, loanType, description, paymentStatus;
		int price = 0, downPayment = 0, installments = 0, monthlyInstallment = 0;
		int totalAmount = 0, startMonth = 0, remainingAmount = 0, paidAmount = 0;

		while (getline(ss, token, '#')) {
			fieldCount++;
			switch (fieldCount) {
			case 1: approvedAppID = token; break;
			case 2: approvedCNIC = token; break;
			case 3: loanType = token; break;
			case 4: description = token; break;
			case 5: price = stoi(token); break;
			case 6: downPayment = stoi(token); break;
			case 7: installments = stoi(token); break;
			case 8: monthlyInstallment = stoi(token); break;
			case 9: totalAmount = stoi(token); break;
			case 10: startMonth = stoi(token); break;
			case 11: paymentStatus = token; break;
			case 12: remainingAmount = stoi(token); break;
			case 13: paidAmount = stoi(token); break;
			}
		}

		if (approvedCNIC == cnic) {
			foundLoans = true;
			loanCount++;

			cout << "\nLoan #" << loanCount << ":\n";
			cout << "====================\n";
			cout << "Application ID: " << approvedAppID << endl;
			cout << "Loan Type: " << loanType << endl;
			cout << "Description: " << description << endl;
			cout << "Total Price: " << price << endl;
			cout << "Down Payment: " << downPayment << endl;
			cout << "Monthly Installment: " << monthlyInstallment << endl;
			cout << "Total Loan Amount: " << totalAmount << endl;
			cout << "Installment Period: " << installments << " months" << endl;
			cout << "Starting Month: " << startMonth << endl;
			cout << "Payment Status: " << paymentStatus << endl;
			cout << "Amount Paid: " << paidAmount << endl;
			cout << "Remaining Amount: " << remainingAmount << endl;

			double progress = (totalAmount > 0) ? ((double)paidAmount / totalAmount) * 100 : 0;

			int wholePart = (int)progress;
			int decimalPart = (int)((progress - wholePart) * 100);
			cout << "Payment Progress: " << wholePart << "." << (decimalPart < 10 ? "0" : "") << decimalPart << "%" << endl;

			if (paymentStatus == "active" && installments > 0) {
				cout << "\nPayment Timeline:\n";
				string months[] = { "January", "February", "March", "April", "May", "June",
									"July", "August", "September", "October", "November", "December" };

				int completedInstallments = paidAmount / monthlyInstallment;
				int currentMonth = startMonth - 1;

				for (int i = 1; i <= installments; i++) {
					string status = (i <= completedInstallments) ? "[PAID]" : "[PENDING]";
					cout << "Month " << i << " (" << months[currentMonth] << "): "
						<< monthlyInstallment << " " << status << endl;
					currentMonth = (currentMonth + 1) % 12;
				}
			}
			cout << "------------------------" << endl;
		}
	}

	approvedFile.close();

	if (!foundLoans) {
		cout << "No loans found for CNIC: " << cnic << endl;
		cout << "This CNIC either has no approved loans or payment plans haven't been generated yet.\n";
	}
	else {
		cout << "\n=============================================\n";
		cout << "Total loans found: " << loanCount << endl;
		cout << "=============================================\n\n";
	}
}

void startBot() {
	Responder responderForUtterances;
	responderForUtterances.initializeResponder("Utterances.txt");
	cout << "Loading chatbot resources...\n";
	cout << "Chatbot initialized successfully!\n";
	cout << "====================================\n";
	cout << "   LOAN PROCESSING CHATBOT\n";
	cout << "====================================\n\n";
	cout << "Welcome! How can I assist you today?\n\n";

	string input = "";
	HomeLoan homeLoan;
	homeLoan.initializeHomeLoan();

	ScooterLoan scooterLoan;
	scooterLoan.initializeScooterLoan();

	CarLoan carLoan;
	carLoan.initializeCarLoan();
	PersonalLoan personalLoan;
	personalLoan.initializePersonalLoan();

	while (true) {
		cout << "You: ";
		cin>>input;
		cout << endl;

		if (input == "X" || input == "x") {
			cout << "Exiting...\n";
			break;
		}

		responderForUtterances.respondToUser(input);
		cout << endl;
		if (input == "H" || input == "h") {
			string area = "";
			while (area[0] < '1' || area[0] > '4') {
				cout << "You: ";
				cin >> area;
				cout << endl;
				if (area[0] >= '1' && area[0] <= '4') {
					int homes = homeLoan.displayHomes(stoi(area));
					if (homes > 0) {
						cout << "\nWould you like to see installment plans? Y/n  ";
						cin >> input;
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						if (input == "y" || input == "Y") {
							homeLoan.generateInstallmentPlan(stoi(area));
						}
					}
				}
				else {
					cout << "Invalid area selected. Please try again.\n\n";
				}
			}
		}

		if (input == "S" || input == "s") {
			scooterLoan.displayScooters();
			cout << "\nWould you like to see installment plans? Y/n  ";
			cin >> input;
			cin.ignore();
			if (input == "y" || input == "Y") {
				scooterLoan.generateInstallmentPlan();
			}
		}

		if (input == "P" || input == "p") {
			personalLoan.displayPersonalLoans();
			cout << "\nWould you like to see installment plans? Y/n  ";
			cin >> input;
			if (input == "y" || input == "Y") {
				personalLoan.generateInstallmentPlan();
			}
			cin.ignore();
		}

		if (input == "C" || input == "c") {
			cout << "You: ";

			string choice;
			cin >> choice;

			if (choice == "1" || choice == "2") {
				int opt = stoi(choice);
				carLoan.displayCarsByMakeOption(opt);

				cout << "\nWould you like an installment plan? Y/n  ";
				cin >> input;
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				if (input == "y" || input == "Y") {
				carLoan.generateInstallmentPlanForOption(opt);
				}
			}
			else {
				cout << "Invalid make selected. Please try again.\n\n";
			}
		}

		if (input == "B" || input == "b") {
			string cnic;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Enter CNIC (13 digits): ";
			getline(cin, cnic);
			cnic = trim(cnic);
			if (all_of(cnic.begin(), cnic.end(), ::isdigit) && cnic.length() == 13) {
				checkApplicationsByCNIC(cnic);
				continue;
			}
			else {
				cout << "Invalid CNIC. Please try again.\n\n";
			}
		}

		if (input == "D" || input == "d") {
			string cnic;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Enter CNIC (13 digits): ";
			getline(cin, cnic);
			cnic = trim(cnic);
			if (all_of(cnic.begin(), cnic.end(), ::isdigit) && cnic.length() == 13) {
				generateMonthlyPlan(cnic);
				continue;
			}
			else {
				cout << "Invalid CNIC. Please try again.\n\n";
			}
		}

		if (input == "E" || input == "e") {
			string cnic;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Enter CNIC (13 digits): ";
			getline(cin, cnic);
			cnic = trim(cnic);
			if (all_of(cnic.begin(), cnic.end(), ::isdigit) && cnic.length() == 13) {
				displayAllLoanDetailsByCNIC(cnic);
				continue;
			}
			else {
				cout << "Invalid CNIC. Please try again.\n\n";
			}
		}

		if (input == "L" || input == "l") {
			cout << "Would you like to apply for a loan? Y/n  ";
			cin >> input;
			if (input == "Y" || input == "y") {
				cin.ignore(numeric_limits<streamsize>::max(), '\n');

				cout << "\nPress 1 to start a NEW application.\n";
				cout << "Press 2 to CONTINUE an existing incomplete application.\n";
				cout << "\nYour choice: ";
				string appChoice;
				getline(cin, appChoice);
				appChoice = trim(appChoice);
				if (appChoice == "2") {
					string appID, cnic;
					cout << "Enter your previous Application ID: ";
					getline(cin, appID);
					appID = trim(appID);
					if (!all_of(appID.begin(), appID.end(), ::isdigit) || appID.length() != 4) {
						cout << "Invalid ID. Please try again.\n\n";
						while (all_of(appID.begin(), appID.end(), ::isdigit) || appID.length() != 4) {
							cout << "Enter your previous Application ID: ";
							getline(cin, appID);
							appID = trim(appID);
						}
					}
					cout << "Enter your CNIC (13 digits, no dashes): ";
					getline(cin, cnic);
					cnic = trim(cnic);
					if (!all_of(cnic.begin(), cnic.end(), ::isdigit) || cnic.length() != 13) {
						cout << "Invalid CNIC. Please try again.\n\n";
						while (all_of(cnic.begin(), cnic.end(), ::isdigit) || cnic.length() != 13) {
							cout << "Enter your CNIC (13 digits, no dashes): ";
							getline(cin, cnic);
							cnic = trim(cnic);
						}
					}

					LoanSeeker applicant;
					string status;
					if (!loadIncompleteApplication(appID, cnic, applicant, status)) {
						continue;
					}

					cout << "\nResuming your application (ID: " << applicant.applicationID
						<< ") from checkpoint " << status << ".\n\n";

					if (status == "C1") {
						applicant.inputAnnualIncome();
						applicant.inputAvgElectricityBill();
						applicant.inputCurrentElectricityBill();
						applicant.saveCheckpoint("C2");
						string proceed;
						cout << "Checkpoint 2: Saved!\nDo you want to continue? (Y/n)  ";
						getline(cin, proceed);
						proceed = trim(proceed);
						if (proceed != "Y" && proceed != "y") {
							continue;
						}

						applicant.inputRefereeDetails();
						applicant.saveCheckpoint("C3");
						cout << "Checkpoint 3: Saved!\nDo you want to continue? Y/n  ";
						getline(cin, proceed);
						proceed = trim(proceed);
						if (proceed != "Y" && proceed != "y") {
							continue;
						}

						applicant.inputImagePaths(applicant.applicationID);
						applicant.inputLoanTypeAndSelection();
					}
					else if (status == "C2") {
						applicant.inputRefereeDetails();
						applicant.saveCheckpoint("C3");
						string proceed;
						cout << "Checkpoint 3: Saved!\nDo you want to continue? Y/n  ";
						getline(cin, proceed);
						proceed = trim(proceed);
						if (proceed != "Y" && proceed != "y") {
							continue;
						}

						applicant.inputImagePaths(applicant.applicationID);
						applicant.inputLoanTypeAndSelection();
					}
					else if (status == "C3") {
						applicant.inputImagePaths(applicant.applicationID);
						applicant.inputLoanTypeAndSelection();
					}

					applicant.displaySummary();

					if (applicant.confirmSubmission()) {
						applicant.saveToFile();
					}
					else {
						cout << "\nApplication not submitted.\n";
						applicant.removeFromFile(applicant.applicationID, "applications.txt");
					}
				}
				else if (appChoice == "1") {
					LoanSeeker applicant;

					cout << "\n=== LOAN APPLICATION FORM ===\n\n";

					applicant.inputFullName();
					applicant.inputFatherName();
					applicant.inputPostalAddress();
					applicant.inputContactNumber();
					applicant.inputEmail();
					applicant.inputCnic();
					applicant.inputCnicExpiryDate();
					applicant.inputEmploymentStatus();
					applicant.inputMaritalStatus();
					applicant.inputGender();
					applicant.inputNumberOfDependents();

					applicant.initializeApplicationID();
					cout << "\nYour Application ID is: " << applicant.applicationID << endl;
					cout << "Please keep it safe to continue your application later if needed.\n\n";

					applicant.saveCheckpoint("C1");
					cout << "Checkpoint 1: Saved!\nDo you want to continue? (Y/n)  ";
					string proceed;
					getline(cin, proceed);
					proceed = trim(proceed);
					if (proceed != "Y" && proceed != "y") {
						continue;
					}

					applicant.inputAnnualIncome();
					applicant.inputAvgElectricityBill();
					applicant.inputCurrentElectricityBill();
					applicant.saveCheckpoint("C2");
					cout << "Checkpoint 2: Saved!\nDo you want to continue? (Y/n)  ";
					getline(cin, proceed);
					proceed = trim(proceed);
					if (proceed != "Y" && proceed != "y") {
						continue;
					}

					applicant.inputRefereeDetails();
					applicant.saveCheckpoint("C3");
					cout << "Checkpoint 3: Saved!\nDo you want to continue? Y/n  ";
					getline(cin, proceed);
					proceed = trim(proceed);
					if (proceed != "Y" && proceed != "y") {
						continue;
					}

					applicant.inputImagePaths(applicant.applicationID);
					applicant.inputLoanTypeAndSelection();
					applicant.displaySummary();

					if (applicant.confirmSubmission()) {
						applicant.saveToFile();
					}
					else {
						cout << "\nApplication not submitted.\n";
						applicant.removeFromFile(applicant.applicationID, "applications.txt");
					}
				}
				else {
					cout << "Invalid Choice!\n";
					continue;
				}
			}
			else {
				continue;
			}
		}
		if (input == "G" || input == "g") {
			system("cls");
			cout << "Entering General Conversation Mode...\n";
			cout << "Chatbot initialized successfully!\n";
			cout << "====================================\n";
			cout << "   CONVERSATIONAL CHATBOT\n";
			cout << "====================================\n\n";
			Conversation generalConversation;
			generalConversation.loadConversationFromFile("human_chat_corpus.txt");
			string input = "";
			cout << "Entered General Conversation Mode. Press X to return.\n";
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			while (true) {
				cout << "\nYou: ";
				getline(cin, input);
				input = trim(input);
				if (input == "X" || input == "x") {
					system("cls");
					cout << "Exiting General Conversation Mode...\n";
					cout << "Loading chatbot resources...\n";
					cout << "Chatbot initialized successfully!\n";
					cout << "====================================\n";
					cout << "   LOAN PROCESSING CHATBOT\n";
					cout << "====================================\n\n";
					cout << "Welcome! How can I assist you today?\n\n";
					break;
				}
				transform(input.begin(), input.end(), input.begin(), ::tolower);
				string response = generalConversation.getBestResponse(input);
				cout << endl << response << endl;
			}
		}
	}
}

