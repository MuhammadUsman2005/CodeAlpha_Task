#include <iostream>
#include <vector>
#include <iomanip>
#include <string>

using namespace std;

struct Course {
    string courseName;
    double gradePoints;
    int creditHours;
};

int main() {
    int numberOfCourses;
    vector<Course> courses;
    
    do {
        cout << "Enter the number of courses taken this semester: ";
        cin >> numberOfCourses;
        
        if (numberOfCourses <= 0) {
            cout << "Please enter a valid number of courses (greater than 0).\n";
        }
    } while (numberOfCourses <= 0);
    
    courses.resize(numberOfCourses);
    
    cout << "\nEnter course details:\n";
    cout << "Note: Grade should be on a 4.0 scale (e.g., 4.0 for A, 3.0 for B, etc.)\n\n";
    
    for (int courseIndex = 0; courseIndex < numberOfCourses; courseIndex++) {
        cout << "Course " << (courseIndex + 1) << ":\n";
        
        cin.ignore();
        
        cout << "  Course Name: ";
        getline(cin, courses[courseIndex].courseName);
        
        do {
            cout << "  Grade Points (0.0 - 4.0): ";
            cin >> courses[courseIndex].gradePoints;
            
            if (courses[courseIndex].gradePoints < 0.0 || courses[courseIndex].gradePoints > 4.0) {
                cout << "  Please enter a valid grade between 0.0 and 4.0.\n";
            }
        } while (courses[courseIndex].gradePoints < 0.0 || courses[courseIndex].gradePoints > 4.0);
        
        do {
            cout << "  Credit Hours: ";
            cin >> courses[courseIndex].creditHours;
            
            if (courses[courseIndex].creditHours <= 0) {
                cout << "  Please enter valid credit hours (greater than 0).\n";
            }
        } while (courses[courseIndex].creditHours <= 0);
        
        cout << endl;
    }
    
    // Here we calculate total credits and total grade points
    int totalCreditHours = 0;
    double totalGradePoints = 0.0;
    
    for (const auto& currentCourse : courses) {
        totalCreditHours += currentCourse.creditHours;
        totalGradePoints += (currentCourse.gradePoints * currentCourse.creditHours);
    }
    
    double currentSemesterGPA = (totalCreditHours > 0) ? (totalGradePoints / totalCreditHours) : 0.0;
    
    // Here we take input for previous academic record for CGPA calculation
    int previousTotalCreditHours = 0;
    double previousTotalGradePoints = 0.0;
    char hasPreviousRecord;
    
    cout << "Do you have previous academic record? (y/n): ";
    cin >> hasPreviousRecord;
    
    if (hasPreviousRecord == 'y' || hasPreviousRecord == 'Y') {
        do {
            cout << "Enter total credit hours from previous semesters: ";
            cin >> previousTotalCreditHours;
            
            if (previousTotalCreditHours < 0) {
                cout << "Please enter a valid number (0 or greater).\n";
            }
        } while (previousTotalCreditHours < 0);
        
        if (previousTotalCreditHours > 0) {
            double previousCGPA;
            do {
                cout << "Enter your previous CGPA (0.0 - 4.0): ";
                cin >> previousCGPA;
                
                if (previousCGPA < 0.0 || previousCGPA > 4.0) {
                    cout << "Please enter a valid CGPA between 0.0 and 4.0.\n";
                }
            } while (previousCGPA < 0.0 || previousCGPA > 4.0);
            
            previousTotalGradePoints = previousCGPA * previousTotalCreditHours;
        }
    }
    
    // Here we calculate overall CGPA
    int overallTotalCreditHours = totalCreditHours + previousTotalCreditHours;
    double overallTotalGradePoints = totalGradePoints + previousTotalGradePoints;
    double overallCGPA = (overallTotalCreditHours > 0) ? (overallTotalGradePoints / overallTotalCreditHours) : 0.0;
    
    // Here we display results
    cout << "\n" << string(60, '=') << "\n";
    cout << "                    ACADEMIC REPORT\n";
    cout << string(60, '=') << "\n\n";
    
    cout << "INDIVIDUAL COURSE GRADES:\n";
    cout << string(60, '-') << "\n";
    cout << left << setw(30) << "Course Name" 
         << setw(12) << "Grade" 
         << setw(15) << "Credit Hours" 
         << "Grade Points\n";
    cout << string(60, '-') << "\n";
    
    for (const auto& currentCourse : courses) {
        cout << left << setw(30) << currentCourse.courseName
             << setw(12) << fixed << setprecision(2) << currentCourse.gradePoints
             << setw(15) << currentCourse.creditHours
             << fixed << setprecision(2) << (currentCourse.gradePoints * currentCourse.creditHours) << "\n";
    }
    
    cout << string(60, '-') << "\n";
    cout << left << setw(30) << "TOTALS:"
         << setw(12) << " "
         << setw(15) << totalCreditHours
         << fixed << setprecision(2) << totalGradePoints << "\n\n";
    
    cout << "SEMESTER PERFORMANCE:\n";
    cout << string(30, '-') << "\n";
    cout << "Current Semester GPA: " << fixed << setprecision(3) << currentSemesterGPA << "\n";
    cout << "Total Credit Hours (This Semester): " << totalCreditHours << "\n\n";
    
    cout << "OVERALL ACADEMIC PERFORMANCE:\n";
    cout << string(35, '-') << "\n";
    cout << "Overall CGPA: " << fixed << setprecision(3) << overallCGPA << "\n";
    cout << "Total Credit Hours (Overall): " << overallTotalCreditHours << "\n\n";
    
    cout << "PERFORMANCE INTERPRETATION:\n";
    cout << string(30, '-') << "\n";
    if (overallCGPA >= 3.7) {
        cout << "Excellent Performance (A-)\n";
    } 
	else if (overallCGPA >= 3.3) {
        cout << "Very Good Performance (B+)\n";
    } 
	else if (overallCGPA >= 3.0) {
        cout << "Good Performance (B)\n";
    } 
	else if (overallCGPA >= 2.7) {
        cout << "Above Average Performance (B-)\n";
    } 
	else if (overallCGPA >= 2.0) {
        cout << "Average Performance (C)\n";
    } 
	else {
        cout << "Below Average Performance\n";
    }
    
    cout << "\n" << string(60, '=') << "\n";
    
    return 0;
}
