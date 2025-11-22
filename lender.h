#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>
using namespace std;

struct Application {
    string applicationID;
    string fullName;
    string fatherName;
    string postalAddress;
    string contactNumber;
    string email;
    string cnic;
    string cnicExpiryDate;
    string employmentStatus;
    string maritalStatus;
    string gender;
    string numberOfDependents;
    string annualIncome;
    string avgElectricityBill;
    string currentElectricityBill;
    string referee1Name, referee1Cnic, referee1IssueDate, referee1Phone, referee1Email;
    string referee2Name, referee2Cnic, referee2IssueDate, referee2Phone, referee2Email;
    string cnicFrontPath, cnicBackPath, electricityBillPath, salarySlipPath;
    string applicationStatus;
    string loanType;
    string selectedMake;
    string selectedArea;
    string selectedHomeIndex;
    string selectedCarIndex;
    string selectedScooterIndex;
    string selectedHomeDetails;
    string selectedCarDetails;
    string selectedScooterDetails;
};

vector<Application> loadApplications() {
    vector<Application> apps;
    ifstream file("applications.txt");
    if (!file.is_open()) {
        cout << "Error: applications.txt not found.\n";
        return apps;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        Application app;
        string token;

        vector<string*> fields = {
            &app.applicationID, &app.fullName, &app.fatherName, &app.postalAddress,
            &app.contactNumber, &app.email, &app.cnic, &app.cnicExpiryDate,
            &app.employmentStatus, &app.maritalStatus, &app.gender, &app.numberOfDependents,
            &app.annualIncome, &app.avgElectricityBill, &app.currentElectricityBill,
            &app.referee1Name, &app.referee1Cnic, &app.referee1IssueDate,
            &app.referee1Phone, &app.referee1Email,
            &app.referee2Name, &app.referee2Cnic, &app.referee2IssueDate,
            &app.referee2Phone, &app.referee2Email,
            &app.cnicFrontPath, &app.cnicBackPath, &app.electricityBillPath, &app.salarySlipPath,
            &app.applicationStatus, &app.loanType, &app.selectedMake, &app.selectedArea,
            &app.selectedHomeIndex, &app.selectedCarIndex, &app.selectedScooterIndex,
            &app.selectedHomeDetails, &app.selectedCarDetails, &app.selectedScooterDetails
        };

        for (size_t i = 0; i < fields.size(); i++) {
            if (getline(ss, token, '#')) {
                *(fields[i]) = token;
            }
            else {
                *(fields[i]) = "";
            }
        }
        apps.push_back(app);
    }
    file.close();
    return apps;
}

void displayApplicationPlain(const Application& app) {
    system("cls");

    cout << "Name: " << app.fullName << "\n";
    cout << "Father's Name: " << app.fatherName << "\n";
    cout << "ID: " << app.applicationID << "\n";
    cout << "CNIC: " << app.cnic << "\n";
    cout << "Contact: " << app.contactNumber << "\n";
    cout << "Email: " << app.email << "\n";
    cout << "Address: " << app.postalAddress << "\n";
    cout << "Employment Status: " << app.employmentStatus << "\n";
    cout << "Marital Status: " << app.maritalStatus << "\n";
    cout << "Gender: " << app.gender << "\n";
    cout << "Dependents: " << app.numberOfDependents << "\n";
    cout << "Annual Income: " << app.annualIncome << " PKR\n";
    cout << "Avg Electricity Bill: " << app.avgElectricityBill << " PKR\n";
    cout << "Current Electricity Bill: " << app.currentElectricityBill << " PKR\n";

    cout << "\nLoan Type: " << (app.loanType.empty() ? "N/A" : app.loanType) << "\n";

    if (app.loanType == "home") {
        cout << "Selected Area: " << app.selectedArea << "\n";
        if (!app.selectedHomeDetails.empty()) {
            stringstream ss(app.selectedHomeDetails);
            string item;
            while (getline(ss, item, ',')) {
                size_t colonPos = item.find(':');
                if (colonPos != string::npos) {
                    string key = item.substr(0, colonPos);
                    string value = item.substr(colonPos + 1);
                    cout << key << ": " << value << "\n";
                }
            }
        }
        else {
            cout << "Home Details: N/A\n";
        }
    }
    else if (app.loanType == "car") {
        cout << "Selected Make: " << (app.selectedMake == "1" ? "Make 1" : "Make 2") << "\n";
        if (!app.selectedCarDetails.empty()) {
            stringstream ss(app.selectedCarDetails);
            string item;
            while (getline(ss, item, ',')) {
                size_t colonPos = item.find(':');
                if (colonPos != string::npos) {
                    string key = item.substr(0, colonPos);
                    string value = item.substr(colonPos + 1);
                    cout << key << ": " << value << "\n";
                }
            }
        }
        else {
            cout << "Car Details: N/A\n";
        }
    }
    else if (app.loanType == "scooter") {
        cout << "Selected Make: " << "Make 1" << "\n";
        if (!app.selectedScooterDetails.empty()) {
            stringstream ss(app.selectedScooterDetails);
            string item;
            while (getline(ss, item, ',')) {
                size_t colonPos = item.find(':');
                if (colonPos != string::npos) {
                    string key = item.substr(0, colonPos);
                    string value = item.substr(colonPos + 1);
                    cout << key << ": " << value << "\n";
                }
            }
        }
        else {
            cout << "Scooter Details: N/A\n";
        }
    }

    cout << "\nReferee 1 Name: " << app.referee1Name << "\n";
    cout << "Referee 1 CNIC: " << app.referee1Cnic << "\n";
    cout << "Referee 1 Phone: " << app.referee1Phone << "\n";
    cout << "Referee 1 Email: " << app.referee1Email << "\n";

    cout << "Referee 2 Name: " << app.referee2Name << "\n";
    cout << "Referee 2 CNIC: " << app.referee2Cnic << "\n";
    cout << "Referee 2 Phone: " << app.referee2Phone << "\n";
    cout << "Referee 2 Email: " << app.referee2Email << "\n";

    cout << "\nCNIC Front: " << app.cnicFrontPath << "\n";
    cout << "CNIC Back: " << app.cnicBackPath << "\n";
    cout << "Electricity Bill: " << app.electricityBillPath << "\n";
    cout << "Salary Slip: " << app.salarySlipPath << "\n";

    cout << "\nStatus: " << app.applicationStatus << "\n";
    cout << "\n====================================\n";
}

void updateApplicationStatus(const string& appID, const string& newStatus) {
    vector<Application> apps = loadApplications();
    bool found = false;

    for (auto& app : apps) {
        if (app.applicationID == appID) {
            app.applicationStatus = newStatus;
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Application with ID " << appID << " not found.\n";
        return;
    }

    ofstream file("applications.txt");
    if (!file.is_open()) {
        cout << "Error: Could not open file to write.\n";
        return;
    }

    for (const auto& app : apps) {
        vector<string> fields = {
            app.applicationID, app.fullName, app.fatherName, app.postalAddress, app.contactNumber,
            app.email, app.cnic, app.cnicExpiryDate, app.employmentStatus, app.maritalStatus, app.gender,
            app.numberOfDependents, app.annualIncome, app.avgElectricityBill, app.currentElectricityBill,
            app.referee1Name, app.referee1Cnic, app.referee1IssueDate, app.referee1Phone, app.referee1Email,
            app.referee2Name, app.referee2Cnic, app.referee2IssueDate, app.referee2Phone, app.referee2Email,
            app.cnicFrontPath, app.cnicBackPath, app.electricityBillPath, app.salarySlipPath,
            app.applicationStatus, app.loanType, app.selectedMake, app.selectedArea, app.selectedHomeIndex,
            app.selectedCarIndex, app.selectedScooterIndex, app.selectedHomeDetails, app.selectedCarDetails,
            app.selectedScooterDetails
        };

        for (size_t i = 0; i < fields.size(); i++) {
            file << fields[i];
            if (i < fields.size() - 1) {
                file << "#";
            }
        }
        file << "\n";
    }
    file.close();
    cout << "Application status updated successfully.\n";
}

void loginLender() {
    cout << "Loan Applications - Lender View\n";
    cout << "====================================\n";

    while (true) {
        vector<Application> apps = loadApplications();
        if (apps.empty()) {
            cout << "\nNo applications found.\n";
            return;
        }

        cout << "\nAll Applications:\n";
        cout << "=================\n";
        for (const auto& app : apps) {
            cout << "ID: " << app.applicationID << "\n";
            cout << "Name: " << app.fullName << "\n";
            cout << "CNIC: " << app.cnic << "\n";
            cout << "Loan Type: " << app.loanType << "\n";
            cout << "Status: " << app.applicationStatus << "\n";
            cout << "------------------------\n";
        }

        cout << "\nEnter Application ID to review (or Q to quit): ";
        string id;
        cin >> id;
        if (id == "Q" || id == "q") break;

        Application* selected = nullptr;
        for (auto& app : apps) {
            if (app.applicationID == id) {
                selected = &app;
                break;
            }
        }

        if (!selected) {
            cout << "\nInvalid Application ID.\n";
            continue;
        }

        displayApplicationPlain(*selected);

        string currentStatus = selected->applicationStatus;
        cout << "\nCurrent Status: " << currentStatus << "\n";

        if (currentStatus == "approved") {
            cout << "\nThis application is already approved.\n";
            cout << "You can only reject it now.\n";
            cout << "(R) Reject  (C) Cancel: ";
        }
        else if (currentStatus == "rejected") {
            cout << "\nThis application is already rejected.\n";
            cout << "You can only approve it now.\n";
            cout << "(A) Approve  (C) Cancel: ";
        }
        else {
            cout << "\n(A) Approve  (R) Reject  (C) Cancel: ";
        }

        string choice;
        cin >> choice;

        if (choice == "A" || choice == "a") {
            if (currentStatus == "approved") {
                cout << "\nApplication is already approved. No changes made.\n";
            }
            else {
                selected->applicationStatus = "approved";
                updateApplicationStatus(id, "approved");
                cout << "\nApplication " << id << " APPROVED.\n";
            }
        }
        else if (choice == "R" || choice == "r") {
            if (currentStatus == "rejected") {
                cout << "\nApplication is already rejected. No changes made.\n";
            }
            else {
                selected->applicationStatus = "rejected";
                if (currentStatus == "approved") {
                    updateApplicationStatus(id, "rejected");
                }

                cout << "\nApplication " << id << " REJECTED.\n";
            }
        }
        else {
            cout << "\nOperation cancelled.\n";
        }

        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
        system("cls");
    }
}
