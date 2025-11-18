#pragma once
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

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
	void displayHomes(int areaNumber) {
		int homesAvailable = 0;
		for (int i = 0; i < numberOfLines; i++) {
			if (area[i] == areaNumber) {
				homesAvailable++;
				cout << "Area: " << area[i] << endl;
				cout << "Size: " << size[i] << " Marla\n";
				cout << "Installments: " << installments[i] << endl;
				cout << "Price: " << price[i] << endl;
				cout << "Down Payment: " << downPayment[i] << endl;
				cout << endl;
			}
		}
		if (homesAvailable == 0) {
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
			cout << endl;
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
				cout << "------------------------\n";
			}
		}

		if (!found) {
			cout << "No cars found for " << targetMake << " to generate installment plan.\n";
		}
	}
};

class LoanSeeker {
private:
    bool validateAlphabetString(const string& str) {
        return all_of(str.begin(), str.end(), [](char c) { return isalpha(c) || isspace(c); });
    }

    bool validateNumeric(const string& str) {
        return all_of(str.begin(), str.end(), ::isdigit);
    }

    bool validateEmail(const string& str) {
        return str.find('@') != string::npos;
    }

    bool validateContactNumber(const string& str) {
        return (str.size() == 11 && str[0] == '0') || (str.size() == 13 && str.substr(0, 3) == "+92");
    }

    bool validateCnic(const string& str) {
        return str.size() == 13 && validateNumeric(str);
    }

    bool validateDate(const string& str) {
        int day, month, year;
        if (sscanf(str.c_str(), "%d %d %d", &day, &month, &year) == 3) {
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

public:
    string fullName, fatherName, postalAddress, contactNumber, email, cnic, cnicExpiryDate;
    string employmentStatus, maritalStatus, gender;
    int numberOfDependents;
    float annualIncome, avgElectricityBill, currentElectricityBill;

    void inputFullName() {
        while (true) {
            cout << "Enter Full Name: ";
            getline(cin, fullName);
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
        capitalizeWords(postalAddress);
    }

    void inputContactNumber() {
        while (true) {
            cout << "Enter Contact Number (11 or 13 digits): ";
            getline(cin, contactNumber);
            if (validateContactNumber(contactNumber)) break;
            cout << "Invalid Contact Number! Must be 11 digits starting with 0 or 13 digits starting with +92.\n";
        }
    }

    void inputEmail() {
        while (true) {
            cout << "Enter Email Address: ";
            getline(cin, email);
            transform(email.begin(), email.end(), email.begin(), ::tolower);
            if (validateEmail(email)) break;
            cout << "Invalid Email Address! Must contain @ symbol.\n";
        }
    }

    void inputCnic() {
        while (true) {
            cout << "Enter CNIC (13 digits, no dashes): ";
            getline(cin, cnic);
            if (validateCnic(cnic)) break;
            cout << "Invalid CNIC! Must be exactly 13 digits.\n";
        }
    }

    void inputCnicExpiryDate() {
        while (true) {
            cout << "Enter CNIC Expiry Date (dd mm yy): ";
            getline(cin, cnicExpiryDate);
            if (validateDate(cnicExpiryDate)) break;
            cout << "Invalid date! Format must be dd mm yy with valid day/month values.\n";
        }
    }

    void inputEmploymentStatus() {
        while (true) {
            cout << "Enter Employment Status (self, salaried, retired, unemployed): ";
            getline(cin, employmentStatus);
            transform(employmentStatus.begin(), employmentStatus.end(), employmentStatus.begin(), ::tolower);
            if (validateEmploymentStatus(employmentStatus)) break;
            cout << "Invalid Employment Status!\n";
        }
    }

    void inputMaritalStatus() {
        while (true) {
            cout << "Enter Marital Status (widowed, single, married, divorced): ";
            getline(cin, maritalStatus);
            transform(maritalStatus.begin(), maritalStatus.end(), maritalStatus.begin(), ::tolower);
            if (validateMaritalStatus(maritalStatus)) break;
            cout << "Invalid Marital Status!\n";
        }
    }

    void inputGender() {
        while (true) {
            cout << "Enter Gender (male, female, other): ";
            getline(cin, gender);
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
            if (validateNumeric(input)) {
                currentElectricityBill = stof(input);
                break;
            } else {
                cout << "Invalid input! Must contain only digits.\n";
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

	ScooterLoan scooterLoan;
	scooterLoan.initializeScooterLoan();

	CarLoan carLoan;
	carLoan.initializeCarLoan();

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
					homeLoan.displayHomes(stoi(area));
					cout << "\nWould you like an installment plan? Y/n  ";
					cin >> input;
					cin.ignore();
					if (input == "y" || input == "Y") {
						homeLoan.generateInstallmentPlan(stoi(area));
					}
				}
				else {
					cout << "Invalid area selected. Please try again.\n";
				}
			}
		}

		if (input == "S" || input == "s") {
			scooterLoan.displayScooters();
			cout << "\nWould you like an installment plan? Y/n  ";
			cin >> input;
			cin.ignore();
			if (input == "y" || input == "Y") {
				scooterLoan.generateInstallmentPlan();
			}
		}
		if (input == "C" || input == "c") {
			cout << "Select car make:\n";
			cout << "1. Make 1\n";
			cout << "2. Make 2\n";
			cout << "You: ";

			string choice;
			cin >> choice;

			if (choice == "1" || choice == "2") {
				int opt = stoi(choice);
				carLoan.displayCarsByMakeOption(opt);

				cout << "\nWould you like an installment plan? Y/n  ";
				cin >> input;
				if (input == "y" || input == "Y") {
					carLoan.generateInstallmentPlanForOption(opt);
				}
			}
			else {
				cout << "Invalid selection. Please choose 1 or 2.\n";
			}
		}
	}
}

