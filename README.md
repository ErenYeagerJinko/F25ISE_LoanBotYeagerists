                                                                                    ISE Project Documentation
                                                                                        Team: Yeagerist++

  Members:
    Abdul Ahad (24L-3029)
    M. Anas (24L-3004)
    M. Babar Shahzad (24L-3047)
    Abdul Rehman (24L-3059)


Project Features:
The Loan Processing System we built had two main user roles:
Applicants – can apply for loans (home, car, scooter, personal), save/continue applications, upload documents, and check loan status.
Lenders – can review submitted applications, approve/reject them, and manage loan records.
Additional features:
Chabot interface for loan inquiries, application status checks, and general conversation.
Checkpoint system – applicants can save and resume incomplete applications.
Payment plan generator for approved loans.
Document/image handling – upload and storage of CNIC, bills, salary slips.
Loan catalog browsing – view available homes, cars, scooters, and personal loans with installment plans.
Application tracking by CNIC – view submitted, approved, and rejected applications.

You're absolutely right. Let me correct that approach. Only classes and standalone functions (not belonging to any class) are modules. Functions inside classes are part of their class module.


Module List: 
  1. Application Module (LoanSeeker class)
  Responsibility: Complete loan application handling for applicants.
  - Contains all validation functions (validateEmail(), validateCnic(), etc.)
  - Contains all input functions (inputFullName(), inputContactNumber(), etc.)
  - Contains loan selection functions (selectHomeLoan(), selectCarLoan(), etc.)
  - Contains file operations for saving/loading applications

2. Home Loan Module (HomeLoan class)
  Responsibility: Home/real estate loan product management.
  - Contains initializeHomeLoan(), displayHomes(), generateInstallmentPlan()
  - Manages home data arrays and filtering logic

3. Car Loan Module (CarLoan class)
  Responsibility: Automobile loan product management.
  - Contains initializeCarLoan(), displayCarsByMakeOption(), generateInstallmentPlanForOption()
  - Manages car data arrays and make-based filtering

4. Scooter Loan Module (ScooterLoan class)
  Responsibility: Electric scooter loan product management.
  - Contains initializeScooterLoan(), displayScooters(), generateInstallmentPlan()
  - Manages scooter data and display logic

5. Personal Loan Module (PersonalLoan class)
  Responsibility: Personal/cash loan product management.
  - Contains initializePersonalLoan(), displayPersonalLoans(), generateInstallmentPlan()
  - Manages personal loan categories and amounts

6. Chat Responder Module (Responder class)
  Responsibility: Predefined Q&A chatbot responses.
  - Contains initializeResponder(), respondToUser()
  - Manages utterance-response matching system

7. Conversation Module (Conversation class)
  Responsibility: General conversational AI.
  - Contains loadConversationFromFile(), getBestResponse(), calculateIoU()
  - Implements IoU-based response matching

8. Lender Interface Module (lender.h)
  Responsibility: Lender/admin application review system.
  - loadApplications() - loads all applications from file
  - displayApplicationPlain() - shows full application details
  - updateApplicationStatus() - changes application status
  - loginLender() - main lender interface function

9. Bot Controller Module (startBot())
  Responsibility: Main chatbot interface controller.
  - Handles user input routing to other modules
  - Manages the main menu system
  - Coordinates loan application flow
  - Controls mode switching

10. Application Loader Module (loadIncompleteApplication())
  Responsibility: Loads saved/incomplete applications.
  - Reads application data by ID and CNIC
  - Restores application state from file
  - Handles checkpoint resuming logic

11. Status Check Module (checkApplicationsByCNIC())
  Responsibility: Checks application status by CNIC.
  - Reads application file to find matching CNIC
  - Counts submitted/approved/rejected applications
  - Displays status summary
    
12. Payment Plan Generator Module (generateMonthlyPlan())
  Responsibility: Creates payment plans for approved loans.
  - Generates monthly installment schedules
  - Saves plans to approved.txt
  - Calculates payment timelines

13. Loan Details Display Module (displayAllLoanDetailsByCNIC())
  Responsibility: Shows all loan details for a CNIC.
  - Reads from approved.txt
  - Displays complete loan information and payment progress
  - Shows payment timeline

14. Utility Functions Module (Various helper functions)
  Responsibility: General utility operations.
  - numberOfLinesInFile() - counts lines in a file
  - trim() - trims whitespace from strings
  - Supporting functions



UML Diagram in Pictorial Form (Kindly Zoom-In for Clarity):
                   <link here if possible>

Work Breakdown Structure in Pictorial Form (Kindly Zoom-In for Clarity):
                      <link here if possible>

Known Faults in Project:
We can confirm that all project deliverables have been finalized after thorough testing and debugging, all faults occurring during demos with the course instructor were duly rectified and our final review has found no issues or faults requiring further attention.

Tools Used During Completion of the Project:

Visual Studio:       Integrated Development Environment (IDE) used for writing, compiling, and debugging the core application code.
Github:              Version control platform for source code repository management, team collaboration, and change tracking.
Jira:                Agile project management tool for task allocation, sprint planning, progress tracking, and time management.
Microsoft Word:      Documentation tool for reporting project characteristics and user manual.
Microsoft Excel:     Documentation and analysis tool for recording project timeline.
Hangzhou DeepSeek:   AI tool to create UML diagram of the project and work breakdown structure for the project to into a pictorial form.


User Manual:
  ChatBot: 
  
Command	      Function
     H		                  View available homes by area (1-4)
     S		                  View available scooters
     P	                    View personal loan options
     C	                    View available cars by make (1 or 2)
     L	                    Start a new loan application
     B	                    Check application status by CNIC
     D	                    Generate payment plan for approved loans
     E	                    Display all loan details by CNIC
     G	                    Enter general conversation mode
     X	                    Exit the chatbot

  Lender:
1.	System shows all submitted applications
2.	Enter an Application ID to review details

Command		 Function
    A		           Accept Application
		R              Reject Application
		C			         Return to Main Menu


    
