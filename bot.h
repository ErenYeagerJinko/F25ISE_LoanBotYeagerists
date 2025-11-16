#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <algorithm>
#include <errno.h>
#include <direct.h>

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

	LoanSeeker() {
		hasExistingLoan = false;
		applicationID = "";
		annualIncome = 0.0f;
		avgElectricityBill = 0.0f;
		currentElectricityBill = 0.0f;
		numberOfDependents = 0;
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
            cout << "Enter Average Electricity Bill for last 12 months (no commas): ";
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

		cout << "Enter filename for CNIC Front (e.g., cnic_front.jpg): ";
		string filename;
		getline(cin, filename);
		images.cnicFrontPath = appFolder + "/" + filename;

		cout << "Enter filename for CNIC Back (e.g., cnic_back.jpg): ";
		getline(cin, filename);
		images.cnicBackPath = appFolder + "/" + filename;

		cout << "Enter filename for Recent Electricity Bill (e.g., electricity_bill.jpg): ";
		getline(cin, filename);
		images.electricityBillPath = appFolder + "/" + filename;

		cout << "Enter filename for Salary Slip/Bank Statement (e.g., salary_slip.jpg): ";
		getline(cin, filename);
		images.salarySlipPath = appFolder + "/" + filename;

		cout << "\nImage paths recorded successfully!\n";
	}

	void saveToFile() {
		ofstream file("applications.txt", ios::app);
		if (!file) {
			cout << "Error! Could not open applications.txt for writing!\n";
			return;
		}

		file << fullName << "#"
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
			<< images.salarySlipPath << endl;

		file.close();
		cout << "\n====================================\n";
		cout << "Application submitted successfully!\n";
		cout << "Application ID: " << applicationID << endl;
		cout << "====================================\n";
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





