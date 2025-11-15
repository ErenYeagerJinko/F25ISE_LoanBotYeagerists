#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <algorithm>
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
	void respondToUser(string input) {
		bool foundResponse = false;
		for (char &c : input) {
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

class HomeLoan {
public:
	int numberOfLines, area1Homes, area2Homes, area3Homes, area4Homes;
	int* area, * size, * installments, * price, * downPayment;
	HomeLoan() {
		numberOfLines = numberOfLinesInFile("Home.txt") - 1;
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
		string line;
		getline(file, line); 

		int index = 0;
		while (getline(file, line) && index < numberOfLines) {
			int pos = 0;
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
	void displayHomes(int areaNumber) {
		int homesAvaiable = 0;
		for (int i = 0; i < numberOfLines; i++) {
			if (area[i] == areaNumber) {
				homesAvaiable++;
				cout << "Area: " << area[i] << endl;
				cout << "Size: " << size[i] << " Marla\n";
				cout << "Installments: " << installments[i] << endl;
				cout << "Price: " << price[i] << endl;
				cout << "Down Payement: " << downPayment[i] << endl;
				cout << endl;
			}
		}
		if (homesAvaiable == 0) {
			cout << "No Homes Available in This Area\n\n";
		}
		return;
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

class LoanSeeker {
private:
    bool isAlphabetOnly(const string& str) {
        return all_of(str.begin(), str.end(), [](char c) { return isalpha(c) || isspace(c); });
    }

    bool isNumeric(const string& str) {
        return all_of(str.begin(), str.end(), ::isdigit);
    }

    bool validateDate(const string& date) {
        int day, month, year;
        if (sscanf(date.c_str(), "%d %d %d", &day, &month, &year) == 3) {
            if (month < 1 || month > 12) return false;
            if (day < 1 || day > 31) return false;
            if (month == 2) {
                if (year % 4 == 0) {
                    if (year % 100 == 0 && year % 400 != 0) return false;
                    if (day > 29) return false;
                } else {
                    if (day > 28) return false;
                }
            }
            if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) return false;
            return true;
        }
        return false;
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

public:
    string fullName, fatherName, postalAddress, contactNumber, email, cnic, cnicExpiryDate;
    string employmentStatus, maritalStatus, gender;
    int numberOfDependents;
    float annualIncome, avgElectricityBill, currentElectricityBill;

    void inputFullName() {
        while (true) {
            cout << "Enter Full Name: ";
            getline(cin, fullName);
            if (isAlphabetOnly(fullName)) {
                capitalizeWords(fullName);
                break;
            } else {
                cout << "Full Name should contain only alphabets and spaces! Please try again.\n";
            }
        }
    }

    void inputFatherName() {
        while (true) {
            cout << "Enter Father's Name: ";
            getline(cin, fatherName);
            if (isAlphabetOnly(fatherName)) {
                capitalizeWords(fatherName);
                break;
            } else {
                cout << "Father's Name should contain only alphabets and spaces! Please try again.\n";
            }
        }
    }

    void inputPostalAddress() {
        cout << "Enter Postal Address: ";
        getline(cin, postalAddress);
        capitalizeWords(postalAddress);
    }

    void inputContactNumber() {
        while (true) {
            cout << "Enter Contact Number (11 or 13 digits): ";
            getline(cin, contactNumber);
            if (contactNumber.size() == 11 && contactNumber[0] == '0' || contactNumber.size() == 13 && contactNumber.substr(0, 3) == "+92") {
                break;
            } else {
                cout << "Invalid contact number! Please try again.\n";
            }
        }
    }

    void inputEmail() {
        while (true) {
            cout << "Enter Email Address: ";
            getline(cin, email);
            transform(email.begin(), email.end(), email.begin(), ::tolower);
            if (email.find('@') != string::npos) {
                break;
            } else {
                cout << "Invalid email address! Please try again.\n";
            }
        }
    }

    void inputCnic() {
        while (true) {
            cout << "Enter CNIC (Without dashes): ";
            getline(cin, cnic);
            if (cnic.size() == 13 && all_of(cnic.begin(), cnic.end(), ::isdigit)) {
                break;
            } else {
                cout << "Invalid CNIC! Please try again.\n";
            }
        }
    }

    void inputCnicExpiryDate() {
        while (true) {
            cout << "Enter CNIC Expiry Date (dd mm yy): ";
            getline(cin, cnicExpiryDate);
            if (validateDate(cnicExpiryDate)) {
                break;
            } else {
                cout << "Invalid date format or values! Please enter in the format dd mm yy.\n";
            }
        }
    }

    void inputEmploymentStatus() {
        while (true) {
            cout << "Enter Employment Status (self, salaried, retired, unemployed): ";
            getline(cin, employmentStatus);
            transform(employmentStatus.begin(), employmentStatus.end(), employmentStatus.begin(), ::tolower);
            if (employmentStatus == "self" || employmentStatus == "salaried" || employmentStatus == "retired" || employmentStatus == "unemployed") {
                break;
            } else {
                cout << "Invalid employment status! Please try again.\n";
            }
        }
    }

    void inputMaritalStatus() {
        while (true) {
            cout << "Enter Marital Status (widowed, single, married, divorced): ";
            getline(cin, maritalStatus);
            transform(maritalStatus.begin(), maritalStatus.end(), maritalStatus.begin(), ::tolower);
            if (maritalStatus == "widowed" || maritalStatus == "single" || maritalStatus == "married" || maritalStatus == "divorced") {
                break;
            } else {
                cout << "Invalid marital status! Please try again.\n";
            }
        }
    }

    void inputGender() {
        while (true) {
            cout << "Enter Gender (male, female, other): ";
            getline(cin, gender);
            transform(gender.begin(), gender.end(), gender.begin(), ::tolower);
            if (gender == "male" || gender == "female" || gender == "other") {
                break;
            } else {
                cout << "Invalid gender! Please try again.\n";
            }
        }
    }

    void inputNumberOfDependents() {
        cout << "Enter Number of Dependents: ";
        string input;
        getline(cin, input);
        numberOfDependents = stoi(input);
    }

    void inputAnnualIncome() {
        while (true) {
            cout << "Enter Annual Income (no commas): ";
            string input;
            getline(cin, input);
            if (isNumeric(input)) {
                annualIncome = stof(input);
                break;
            } else {
                cout << "Annual Income should contain only numbers! Please try again.\n";
            }
        }
    }

    void inputAvgElectricityBill() {
        while (true) {
            cout << "Enter Average Electricity Bill (no commas): ";
            string input;
            getline(cin, input);
            if (isNumeric(input)) {
                avgElectricityBill = stof(input);
                break;
            } else {
                cout << "Average Electricity Bill should contain only numbers! Please try again.\n";
            }
        }
    }

    void inputCurrentElectricityBill() {
        while (true) {
            cout << "Enter Current Electricity Bill (exact): ";
            string input;
            getline(cin, input);
            if (isNumeric(input)) {
                currentElectricityBill = stof(input);
                break;
            } else {
                cout << "Current Electricity Bill should contain only numbers! Please try again.\n";
            }
        }
    }
};

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

	while (true) {
		cout << "You: ";
		cin >> input;
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
					homeLoan.displayHomes(stoi(area));
					cout << "\nWould you like an installment plan? Y/n  ";
					cin >> input;
					if (input == "y" || input == "Y") {
						homeLoan.generateInstallmentPlan(stoi(area));
					}
				}
				else {
					cout << "Invalid area selected. Please try again.\n";
				}
			}
		}
	}
}



