#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <errno.h>
#include <direct.h>
#include <cctype>
#include <algorithm>
#include <sstream>
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
				cout << endl;
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

struct ExistingLoan {
    string loanPeriodStatus;
    string totalLoanAmount;
    string amountReturned;
    string loanAmountDue;
    string bankName;
    string loanCategory;
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
        if (sscanf_s(str.c_str(), "%d-%d-%d", &day, &month, &year) == 3) {
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

	bool createDirectory(const std::string& path) {
	    int result = _mkdir(path.c_str());
	    return result == 0 || errno == EEXIST;
	}

	string generateApplicationID() {
	    static int lastID = 1000;
	    lastID++;
	    return to_string(lastID);
	}

public:
    string fullName, fatherName, postalAddress, contactNumber, email, cnic, cnicExpiryDate;
    string employmentStatus, maritalStatus, gender;
    int numberOfDependents;
    float annualIncome, avgElectricityBill, currentElectricityBill;

	bool hasExistingLoan;
	ExistingLoan existingLoan;
	Referee referee1, referee2;
	ApplicantImages images;
	string applicationID;
	string applicationStatus; 
	int selectedArea;      
	int selectedHomeSize;


LoanSeeker() {
    hasExistingLoan = false;
    applicationID = "";
    annualIncome = 0.0f;
    avgElectricityBill = 0.0f;
    currentElectricityBill = 0.0f;
    numberOfDependents = 0;
	applicationStatus = "submitted";
	selectedArea = 0;
	selectedHomeSize = 0;
}

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
            cout << "Enter CNIC Expiry Date (DD-MM-YYYY): ";
            getline(cin, cnicExpiryDate);
            if (validateDate(cnicExpiryDate)) break;
            cout << "Invalid date! Format must be DD-MM-YYYY with valid day/month/year values.\n";
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

	void inputExistingLoanInfo() {
	    cout << "\n=== EXISTING LOAN INFORMATION ===\n";
	    string response;
	
	    while (true) {
	        cout << "Do you have any existing loans? (yes/no): ";
	        getline(cin, response);
	        toLowerCase(response);
	        if (validateYesNo(response)) break;
	        cout << "Invalid input! Please enter 'yes' or 'no'.\n";
	    }
	
	    if (response == "yes" || response == "y") {
	        hasExistingLoan = true;
	
	        while (true) {
	            cout << "Is the loan period Active or Inactive? (active/inactive): ";
	            getline(cin, existingLoan.loanPeriodStatus);
	            toLowerCase(existingLoan.loanPeriodStatus);
	            if (validateLoanStatus(existingLoan.loanPeriodStatus)) {
	                capitalizeWords(existingLoan.loanPeriodStatus);
	                break;
	            }
	            cout << "Invalid input! Please enter 'active' or 'inactive'.\n";
	        }
	
	        while (true) {
	            cout << "Enter Total Loan Amount (no commas): ";
	            getline(cin, existingLoan.totalLoanAmount);
	            if (validateNumeric(existingLoan.totalLoanAmount)) break;
	            cout << "Invalid input! Must contain only digits.\n";
	        }
	
	        while (true) {
	            cout << "Enter Amount Returned (no commas): ";
	            getline(cin, existingLoan.amountReturned);
	            if (validateNumeric(existingLoan.amountReturned)) break;
	            cout << "Invalid input! Must contain only digits.\n";
	        }
	
	        while (true) {
	            cout << "Enter Loan Amount Still Due (no commas): ";
	            getline(cin, existingLoan.loanAmountDue);
	            if (validateNumeric(existingLoan.loanAmountDue)) break;
	            cout << "Invalid input! Must contain only digits.\n";
	        }
	
	        while (true) {
	            cout << "Enter Bank Name: ";
	            getline(cin, existingLoan.bankName);
	            if (validateAlphabetString(existingLoan.bankName)) {
	                capitalizeWords(existingLoan.bankName);
	                break;
	            }
	            cout << "Bank Name should contain only alphabets and spaces!\n";
	        }
	
	        while (true) {
	            cout << "Enter Loan Category (car/home/bike): ";
	            getline(cin, existingLoan.loanCategory);
	            toLowerCase(existingLoan.loanCategory);
	            if (validateLoanCategory(existingLoan.loanCategory)) {
	                capitalizeWords(existingLoan.loanCategory);
	                break;
	            }
	            cout << "Invalid input! Please enter 'car', 'home', or 'bike'.\n";
	        }
	    }
	    else {
	        hasExistingLoan = false;
	        existingLoan.loanPeriodStatus = "N/A";
	        existingLoan.totalLoanAmount = "0";
	        existingLoan.amountReturned = "0";
	        existingLoan.loanAmountDue = "0";
	        existingLoan.bankName = "N/A";
	        existingLoan.loanCategory = "N/A";
	    }
	}

	void inputRefereeDetails() {
	    cout << "\n=== REFEREE DETAILS ===\n";
	
	    cout << "\n--- Referee 1 Information ---\n";
	
	    while (true) {
	        cout << "Enter Name of Referee 1: ";
	        getline(cin, referee1.name);
	        if (validateAlphabetString(referee1.name)) {
	            capitalizeWords(referee1.name);
	            break;
	        }
	        cout << "Name should contain only alphabets and spaces!\n";
	    }
	
	    while (true) {
	        cout << "Enter CNIC of Referee 1 (13 digits, no dashes): ";
	        getline(cin, referee1.cnic);
	        if (validateCnic(referee1.cnic)) break;
	        cout << "Invalid CNIC! Must be exactly 13 digits.\n";
	    }
	
	    while (true) {
	        cout << "Enter CNIC Issue Date of Referee 1 (DD-MM-YYYY): ";
	        getline(cin, referee1.cnicIssueDate);
	        if (validateDate(referee1.cnicIssueDate)) break;
	        cout << "Invalid date! Format must be DD-MM-YYYY with valid day/month/year values.\n";
	    }
	
	    while (true) {
	        cout << "Enter Phone Number of Referee 1 (11 or 13 digits): ";
	        getline(cin, referee1.phoneNumber);
	        if (validateContactNumber(referee1.phoneNumber)) break;
	        cout << "Invalid Phone Number! Must be 11 digits starting with 0 or 13 digits starting with +92.\n";
	    }
	
	    while (true) {
	        cout << "Enter Email Address of Referee 1: ";
	        getline(cin, referee1.emailAddress);
	        toLowerCase(referee1.emailAddress);
	        if (validateEmail(referee1.emailAddress)) break;
	        cout << "Invalid Email Address! Must contain @ symbol.\n";
	    }
	
	    cout << "\n--- Referee 2 Information ---\n";
	
	    while (true) {
	        cout << "Enter Name of Referee 2: ";
	        getline(cin, referee2.name);
	        if (validateAlphabetString(referee2.name)) {
	            capitalizeWords(referee2.name);
	            break;
	        }
	        cout << "Name should contain only alphabets and spaces!\n";
	    }
	
	    while (true) {
	        cout << "Enter CNIC of Referee 2 (13 digits, no dashes): ";
	        getline(cin, referee2.cnic);
	        if (validateCnic(referee2.cnic)) break;
	        cout << "Invalid CNIC! Must be exactly 13 digits.\n";
	    }
	
	    while (true) {
	        cout << "Enter CNIC Issue Date of Referee 2 (DD-MM-YYYY): ";
	        getline(cin, referee2.cnicIssueDate);
	        if (validateDate(referee2.cnicIssueDate)) break;
	        cout << "Invalid date! Format must be DD-MM-YYYY with valid day/month/year values.\n";
	    }
	
	    while (true) {
	        cout << "Enter Phone Number of Referee 2 (11 or 13 digits): ";
	        getline(cin, referee2.phoneNumber);
	        if (validateContactNumber(referee2.phoneNumber)) break;
	        cout << "Invalid Phone Number! Must be 11 digits starting with 0 or 13 digits starting with +92.\n";
	    }
	
	    while (true) {
	        cout << "Enter Email Address of Referee 2: ";
	        getline(cin, referee2.emailAddress);
	        toLowerCase(referee2.emailAddress);
	        if (validateEmail(referee2.emailAddress)) break;
	        cout << "Invalid Email Address! Must contain @ symbol.\n";
	    }
	}

	void inputImagePaths() {
	    cout << "\n=== DOCUMENT IMAGES ===\n";
	
	    applicationID = generateApplicationID();
	
	    string dataFolder = "./data";
	    string appFolder = dataFolder + "/" + applicationID;
	
	    createDirectory(dataFolder);
	    createDirectory(appFolder);
	
	    cout << "Your Application ID is: " << applicationID << endl;
	    cout << "All images will be saved in folder: " << appFolder << endl;
	    cout << "\nPlease provide the file names for the following documents:\n";
	    cout << "(Images should be placed in the " << appFolder << " folder)\n\n";
	
	    cout << "Enter filename for CNIC Front (e.g. cnic_front.jpg): ";
	    string filename;
	    getline(cin, filename);
	    images.cnicFrontPath = appFolder + "/" + filename;
	
	    cout << "Enter filename for CNIC Back (e.g., cnic_back.jpg): ";
	    getline(cin, filename);
	    images.cnicBackPath = appFolder + "/" + filename;
	
	    cout << "Enter filename for Recent Electricity Bill (e.g. electricity_bill.jpg): ";
	    getline(cin, filename);
	    images.electricityBillPath = appFolder + "/" + filename;
	
	    cout << "Enter filename for Salary Slip/Bank Statement (e.g. salary_slip.jpg): ";
	    getline(cin, filename);
	    images.salarySlipPath = appFolder + "/" + filename;
	
	    cout << "\nImage paths recorded successfully!\n";
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
		cout << "Selected Area: " << selectedArea << endl;
		cout << "Selected Home Size: " << selectedHomeSize << " Marla" << endl;
		cout << "\nExisting Loan: " << (hasExistingLoan ? "Yes" : "No") << endl;
		if (hasExistingLoan) {
			cout << "  Bank: " << existingLoan.bankName << endl;
			cout << "  Category: " << existingLoan.loanCategory << endl;
			cout << "  Status: " << existingLoan.loanPeriodStatus << endl;
		}
		cout << "\nReferee 1: " << referee1.name << " (" << referee1.cnic << ")" << endl;
		cout << "Referee 2: " << referee2.name << " (" << referee2.cnic << ")" << endl;
		cout << "\n====================================\n";
	}

	bool confirmSubmission() {
		string response;
		while (true) {
			cout << "\nDo you want to submit this application? (yes/no): ";
			getline(cin, response);
			toLowerCase(response);
			if (validateYesNo(response)) {
				return (response == "yes" || response == "y");
			}
			cout << "Invalid input! Please enter 'yes' or 'no'.\n";
		}
	}

	void saveToFile() {
	    ofstream file("applications.txt", ios::app);
	    if (!file) {
	        cout << "Error! Could not open applications.txt for writing!\n";
	        return;
	    }
	
	    file << applicationID << "#"
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
	        << existingLoan.loanPeriodStatus << "#"
	        << existingLoan.totalLoanAmount << "#"
	        << existingLoan.amountReturned << "#"
	        << existingLoan.loanAmountDue << "#"
	        << existingLoan.bankName << "#"
	        << existingLoan.loanCategory << "#"
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
			<< applicationStatus << "#"  << endl;
	
	    file.close();
	    cout << "\n====================================\n";
	    cout << "Application submitted successfully!\n";
	    cout << "Application ID: " << applicationID << endl;
		cout << "Status: " << applicationStatus << endl;
	    cout << "====================================\n";
		}
};

void checkApplicationsByCNIC(const string& cnic) {
	ifstream file("applications.txt");
	if (!file) {
		cout << "No applications file found.\n";
		return;
	}

	int submitted = 0, approved = 0, rejected = 0;
	string line;

	while (getline(file, line)) {
		stringstream ss(line);
		string token;
		int fieldCount = 0;
		string fileCnic, status;

		while (getline(ss, token, '#')) {
			fieldCount++;
			if (fieldCount == 7) {
				fileCnic = token; 
			}
			if (fieldCount == 36) {
				status = token; 
			}
		}

		if (fileCnic == cnic) {
			if (status == "submitted") submitted++;
			else if (status == "approved") approved++;
			else if (status == "rejected") rejected++;
			else {
				cout << "Unexpected status: " << status << endl;
			}
		}
	}

	file.close();

	cout << "\n====================================\n";
	cout << "   APPLICATION STATUS FOR CNIC\n";
	cout << "   " << cnic << "\n";
	cout << "====================================\n";
	cout << "Submitted Applications: " << submitted << endl;
	cout << "Approved Applications: " << approved << endl;
	cout << "Rejected Applications: " << rejected << endl;
	cout << "Total Applications: " << (submitted + approved + rejected) << endl;
	cout << "====================================\n\n";
}

void generateMonthlyPlan(const string& cnic, HomeLoan& homeLoan) {
	ifstream file("applications.txt");
	if (!file) {
		cout << "No applications file found.\n";
		return;
	}

	string line;
	bool foundApproved = false;

	while (getline(file, line)) {
		stringstream ss(line);
		string token;
		int fieldCount = 0;
		string fileCnic, status, appID;
		int area = 0, homeSize = 0;

		while (getline(ss, token, '#')) {
			fieldCount++;
			if (fieldCount == 1) appID = token;
			if (fieldCount == 7) fileCnic = token;
			if (fieldCount == 36) status = token;
			if (fieldCount == 37) area = stoi(token);
			if (fieldCount == 38) homeSize = stoi(token);
		}

		if (fileCnic == cnic && status == "approved") {
			foundApproved = true;

			int price = 0, downPayment = 0, installments = 0;
			for (int i = 0; i < homeLoan.numberOfLines; i++) {
				if (homeLoan.area[i] == area && homeLoan.size[i] == homeSize) {
					price = homeLoan.price[i];
					downPayment = homeLoan.downPayment[i];
					installments = homeLoan.installments[i];
					break;
				}
			}

			if (price == 0) {
				cout << "Error: Could not find matching home data.\n";
				continue;
			}

			cout << "\n====================================\n";
			cout << "   APPROVED APPLICATION FOUND\n";
			cout << "====================================\n";
			cout << "Application ID: " << appID << endl;
			cout << "Area: " << area << endl;
			cout << "Home Size: " << homeSize << " Marla" << endl;
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
				if (all_of(input.begin(), input.end(), ::isdigit)) {
					startMonth = stoi(input);
					if (startMonth >= 1 && startMonth <= 12) break;
				}
				cout << "Invalid month! Please enter a number between 1 and 12.\n";
			}

			int monthlyInstallment = (price - downPayment) / installments;
			int remainingAmount = price - downPayment;

			cout << "\n====================================\n";
			cout << "   MONTHLY PAYMENT PLAN\n";
			cout << "====================================\n";
			cout << "Monthly Installment: " << monthlyInstallment << endl;
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
		}
	}

	file.close();

	if (!foundApproved) {
		cout << "\nNo approved applications found for CNIC: " << cnic << endl;
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

	while (true) {
		bool showLoan = false;

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
						cout << "\nWould you like an installment plan? Y/n  ";
						cin >> input;
						cin.ignore();
						if (input == "y" || input == "Y") {
							homeLoan.generateInstallmentPlan(stoi(area));
							showLoan = true;
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
			cout << "\nWould you like an installment plan? Y/n  ";
			cin >> input;
			cin.ignore();
			if (input == "y" || input == "Y") {
				scooterLoan.generateInstallmentPlan();
				showLoan = true;
			}
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
				if (input == "y" || input == "Y") {
					carLoan.generateInstallmentPlanForOption(opt);
					showLoan = true;
				}
			}
			else {
				cout << "Invalid make selected. Please try again.\n\n";
			}
		}

		if (input == "B" || input == "b") {
			string cnic;
			cin.ignore();
			cout << "Enter CNIC (13 digits): ";
			getline(cin, cnic);
			if (all_of(cnic.begin(), cnic.end(), ::isdigit) && cnic.length() == 13) {
				checkApplicationsByCNIC(cnic);
				continue;
			}
			else {
				cout << "Invalid CNIC. Please try again.\n\n";
			}
		}

		if (showLoan) {
			cout << "Would you like to apply for a loan? Y/n  ";
			cin >> input;
			if (input == "Y" || input == "y") {
				cin.ignore();
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
				applicant.inputAnnualIncome();
				applicant.inputAvgElectricityBill();
				applicant.inputCurrentElectricityBill();
				applicant.inputExistingLoanInfo();
				applicant.inputRefereeDetails();
				applicant.inputImagePaths();

				applicant.displaySummary();

				if (applicant.confirmSubmission()) {
					applicant.saveToFile();
				}
				else {
					cout << "\nApplication cancelled. Data not saved.\n";
				}
			}
			else {
				continue;
			}
		}
	}
}

