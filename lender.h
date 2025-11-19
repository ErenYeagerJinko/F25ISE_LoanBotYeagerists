//// LenderInterface.cpp
//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include <vector>
//#include <string>
//using namespace std;
//
//struct Application {
//    string applicationID;
//    string fullName;
//    string fatherName;
//    string postalAddress;
//    string contactNumber;
//    string email;
//    string cnic;
//    string cnicExpiryDate;
//    string employmentStatus;
//    string maritalStatus;
//    string gender;
//    string numberOfDependents;
//    string annualIncome;
//    string avgElectricityBill;
//    string currentElectricityBill;
//    string loanPeriodStatus;
//    string totalLoanAmount;
//    string amountReturned;
//    string loanAmountDue;
//    string bankName;
//    string loanCategory;
//    string referee1Name, referee1Cnic, referee1IssueDate, referee1Phone, referee1Email;
//    string referee2Name, referee2Cnic, referee2IssueDate, referee2Phone, referee2Email;
//    string cnicFrontPath, cnicBackPath, electricityBillPath, salarySlipPath;
//    string applicationStatus;
//    string loanType;
//    string selectedMake;
//    string selectedArea;
//    string selectedHomeIndex;
//    string selectedCarIndex;
//    string selectedScooterIndex;
//    string selectedHomeDetails;
//    string selectedCarDetails;
//    string selectedScooterDetails;
//};
//
//vector<Application> loadApplications() {
//    vector<Application> apps;
//    ifstream file("applications.txt");
//    if (!file.is_open()) {
//        cout << "Error: applications.txt not found.\n";
//        return apps;
//    }
//
//    string line;
//    while (getline(file, line)) {
//        if (line.empty()) continue;
//        stringstream ss(line);
//        Application app;
//        string token;
//
//        // Read all 45 fields
//        vector<string*> fields = {
//            &app.applicationID, &app.fullName, &app.fatherName, &app.postalAddress,
//            &app.contactNumber, &app.email, &app.cnic, &app.cnicExpiryDate,
//            &app.employmentStatus, &app.maritalStatus, &app.gender, &app.numberOfDependents,
//            &app.annualIncome, &app.avgElectricityBill, &app.currentElectricityBill,
//            &app.loanPeriodStatus, &app.totalLoanAmount, &app.amountReturned,
//            &app.loanAmountDue, &app.bankName, &app.loanCategory,
//            &app.referee1Name, &app.referee1Cnic, &app.referee1IssueDate,
//            &app.referee1Phone, &app.referee1Email,
//            &app.referee2Name, &app.referee2Cnic, &app.referee2IssueDate,
//            &app.referee2Phone, &app.referee2Email,
//            &app.cnicFrontPath, &app.cnicBackPath, &app.electricityBillPath, &app.salarySlipPath,
//            &app.applicationStatus, &app.loanType, &app.selectedMake, &app.selectedArea,
//            &app.selectedHomeIndex, &app.selectedCarIndex, &app.selectedScooterIndex,
//            &app.selectedHomeDetails, &app.selectedCarDetails, &app.selectedScooterDetails
//        };
//
//        for (size_t i = 0; i < fields.size(); i++) {
//            if (getline(ss, token, '#')) {
//                *(fields[i]) = token;
//            }
//            else {
//                *(fields[i]) = "";
//            }
//        }
//        apps.push_back(app);
//    }
//    file.close();
//    return apps;
//}
//
//void saveApplications(const vector<Application>& apps) {
//    ofstream file("applications.txt");
//    if (!file.is_open()) {
//        cout << "Error: Could not write to applications.txt!\n";
//        return;
//    }
//
//    for (const auto& app : apps) {
//        file << app.applicationID << "#"
//            << app.fullName << "#" << app.fatherName << "#" << app.postalAddress << "#"
//            << app.contactNumber << "#" << app.email << "#" << app.cnic << "#"
//            << app.cnicExpiryDate << "#" << app.employmentStatus << "#"
//            << app.maritalStatus << "#" << app.gender << "#"
//            << app.numberOfDependents << "#" << app.annualIncome << "#"
//            << app.avgElectricityBill << "#" << app.currentElectricityBill << "#"
//            << app.loanPeriodStatus << "#" << app.totalLoanAmount << "#"
//            << app.amountReturned << "#" << app.loanAmountDue << "#"
//            << app.bankName << "#" << app.loanCategory << "#"
//            << app.referee1Name << "#" << app.referee1Cnic << "#"
//            << app.referee1IssueDate << "#" << app.referee1Phone << "#"
//            << app.referee1Email << "#"
//            << app.referee2Name << "#" << app.referee2Cnic << "#"
//            << app.referee2IssueDate << "#" << app.referee2Phone << "#"
//            << app.referee2Email << "#"
//            << app.cnicFrontPath << "#" << app.cnicBackPath << "#"
//            << app.electricityBillPath << "#" << app.salarySlipPath << "#"
//            << app.applicationStatus << "#" << app.loanType << "#"
//            << app.selectedMake << "#" << app.selectedArea << "#"
//            << app.selectedHomeIndex << "#" << app.selectedCarIndex << "#"
//            << app.selectedScooterIndex << "#"
//            << app.selectedHomeDetails << "#" << app.selectedCarDetails << "#"
//            << app.selectedScooterDetails << "#\n";
//    }
//    file.close();
//}
//
//// ✅ THIS IS THE KEY FUNCTION: DISPLAY IN "Label: Value" FORMAT ONLY
//void displayApplicationPlain(const Application& app) {
//    system("cls"); // Use "clear" on Linux/Mac
//
//    cout << "Name: " << app.fullName << "\n";
//    cout << "Father's Name: " << app.fatherName << "\n";
//    cout << "ID: " << app.applicationID << "\n";
//    cout << "CNIC: " << app.cnic << "\n";
//    cout << "Contact: " << app.contactNumber << "\n";
//    cout << "Email: " << app.email << "\n";
//    cout << "Address: " << app.postalAddress << "\n";
//    cout << "Employment Status: " << app.employmentStatus << "\n";
//    cout << "Marital Status: " << app.maritalStatus << "\n";
//    cout << "Gender: " << app.gender << "\n";
//    cout << "Dependents: " << app.numberOfDependents << "\n";
//    cout << "Annual Income: " << app.annualIncome << " PKR\n";
//    cout << "Avg Electricity Bill: " << app.avgElectricityBill << " PKR\n";
//    cout << "Current Electricity Bill: " << app.currentElectricityBill << " PKR\n";
//
//    cout << "\nLoan Type: " << (app.loanType.empty() ? "N/A" : app.loanType) << "\n";
//    if (app.loanType == "home") {
//        cout << "Selected Area: " << app.selectedArea << "\n";
//        cout << "Home Details: " << (app.selectedHomeDetails.empty() ? "N/A" : app.selectedHomeDetails) << "\n";
//    }
//    else if (app.loanType == "car") {
//        cout << "Selected Make: " << (app.selectedMake == "1" ? "Make 1" : "Make 2") << "\n";
//        cout << "Car Details: " << (app.selectedCarDetails.empty() ? "N/A" : app.selectedCarDetails) << "\n";
//    }
//    else if (app.loanType == "scooter") {
//        cout << "Selected Make: " << (app.selectedMake == "1" ? "Make 1" : "Make 2") << "\n";
//        cout << "Scooter Details: " << (app.selectedScooterDetails.empty() ? "N/A" : app.selectedScooterDetails) << "\n";
//    }
//
//    if (app.loanPeriodStatus != "N/A") {
//        cout << "\nExisting Loan: Yes\n";
//        cout << "Loan Status: " << app.loanPeriodStatus << "\n";
//        cout << "Bank: " << app.bankName << "\n";
//        cout << "Loan Category: " << app.loanCategory << "\n";
//        cout << "Total Amount: " << app.totalLoanAmount << " PKR\n";
//        cout << "Amount Returned: " << app.amountReturned << " PKR\n";
//        cout << "Amount Due: " << app.loanAmountDue << " PKR\n";
//    }
//    else {
//        cout << "\nExisting Loan: None\n";
//    }
//
//    cout << "\nReferee 1 Name: " << app.referee1Name << "\n";
//    cout << "Referee 1 CNIC: " << app.referee1Cnic << "\n";
//    cout << "Referee 1 Phone: " << app.referee1Phone << "\n";
//    cout << "Referee 1 Email: " << app.referee1Email << "\n";
//
//    cout << "Referee 2 Name: " << app.referee2Name << "\n";
//    cout << "Referee 2 CNIC: " << app.referee2Cnic << "\n";
//    cout << "Referee 2 Phone: " << app.referee2Phone << "\n";
//    cout << "Referee 2 Email: " << app.referee2Email << "\n";
//
//    cout << "\nCNIC Front: " << app.cnicFrontPath << "\n";
//    cout << "CNIC Back: " << app.cnicBackPath << "\n";
//    cout << "Electricity Bill: " << app.electricityBillPath << "\n";
//    cout << "Salary Slip: " << app.salarySlipPath << "\n";
//
//    cout << "\nStatus: " << app.applicationStatus << "\n";
//    cout << "\n====================================\n";
//}
//
//int main() {
//    cout << "Loan Applications - Lender View\n";
//    cout << "====================================\n";
//
//    while (true) {
//        vector<Application> apps = loadApplications();
//        if (apps.empty()) {
//            cout << "\nNo applications found.\n";
//            return 0;
//        }
//
//        // Show only pending applications (submitted)
//        vector<Application> pending;
//        for (const auto& app : apps) {
//            if (app.applicationStatus == "submitted") {
//                pending.push_back(app);
//            }
//        }
//
//        if (pending.empty()) {
//            cout << "\nNo pending applications.\n";
//            return 0;
//        }
//
//        // ✅ NO TABLE — just list IDs with names in plain format
//        cout << "\nPending Applications:\n";
//        for (const auto& app : pending) {
//            cout << "ID: " << app.applicationID << "\n";
//            cout << "Name: " << app.fullName << "\n";
//            cout << "CNIC: " << app.cnic << "\n";
//            cout << "Loan Type: " << app.loanType << "\n";
//            cout << "------------------------\n";
//        }
//
//        cout << "\nEnter Application ID to review (or Q to quit): ";
//        string id;
//        cin >> id;
//        if (id == "Q" || id == "q") break;
//
//        Application* selected = nullptr;
//        for (auto& app : apps) {
//            if (app.applicationID == id && app.applicationStatus == "submitted") {
//                selected = &app;
//                break;
//            }
//        }
//
//        if (!selected) {
//            cout << "\nInvalid or already processed ID.\n";
//            continue;
//        }
//
//        // ✅ DISPLAY FULL DETAILS IN PLAIN "Label: Value" FORMAT
//        displayApplicationPlain(*selected);
//
//        cout << "\n(A) Approve  (R) Reject  (C) Cancel: ";
//        string choice;
//        cin >> choice;
//
//        if (choice == "A" || choice == "a") {
//            selected->applicationStatus = "approved";
//            saveApplications(apps);
//            cout << "\n✅ Application " << id << " APPROVED.\n";
//        }
//        else if (choice == "R" || choice == "r") {
//            selected->applicationStatus = "rejected";
//            saveApplications(apps);
//            cout << "\n❌ Application " << id << " REJECTED.\n";
//        }
//
//        cout << "\nPress Enter to continue...";
//        cin.ignore();
//        cin.get();
//    }
//
//    return 0;
//}