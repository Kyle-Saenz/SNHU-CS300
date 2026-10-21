// ============================================================================
// Name        : CS 300 Project Two.cpp
// Author      : Kyle Saenz
// Course      : CS-300: DSA Analysis and Design
// Project     : Project Two - Advising Assistance Program
// Description : ABCU course advising program using a Binary Search Tree (BST)
// ============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Define a structure to hold individual course details
struct Course {
    string courseNumber;
    string title;
    vector<string> prerequisites;
};

// Internal node structure for the Binary Search Tree
struct Node {
    Course course;
    Node* left;
    Node* right;

    // Default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // Initialize node with a course object
    Node(Course aCourse) {
        course = aCourse;
        left = nullptr;
        right = nullptr;
    }
};

// Binary Search Tree class for managing course records
class BinarySearchTree {
private:
    Node* root;

    // Helper method to recursively clear nodes and free heap memory
    void clearTree(Node* node) {
        if (node != nullptr) {
            clearTree(node->left);
            clearTree(node->right);
            delete node;
        }
    }

    // Helper method to recursively insert a course into the BST based on courseNumber
    Node* insertNode(Node* node, Course course) {
        if (node == nullptr) {
            return new Node(course);
        }

        // Smaller course numbers go left
        if (course.courseNumber < node->course.courseNumber) {
            if (node->left == nullptr) {
                node->left = new Node(course);
            }
            else {
                node->left = insertNode(node->left, course);
            }
        }
        // Larger or duplicate course numbers go right
        else {
            if (node->right == nullptr) {
                node->right = new Node(course);
            }
            else {
                node->right = insertNode(node->right, course);
            }
        }

        return node;
    }

    // Helper method for recursive in-order traversal (prints alphanumerically)
    void inOrder(Node* node) {
        if (node != nullptr) {
            inOrder(node->left);
            cout << node->course.courseNumber << ", " << node->course.title << endl;
            inOrder(node->right);
        }
    }

public:
    // Constructor
    BinarySearchTree() {
        root = nullptr;
    }

    // Destructor to prevent memory leaks
    virtual ~BinarySearchTree() {
        clear();
    }

    // Clear the entire tree
    void clear() {
        clearTree(root);
        root = nullptr;
    }

    // Insert a course into the tree
    void insert(Course course) {
        root = insertNode(root, course);
    }

    // Print all courses in alphanumeric order
    void printAllInOrder() {
        inOrder(root);
    }

    // Search for a course by its course number (case-insensitive)
    Course* search(string targetCourseNumber) {
        Node* current = root;

        // Convert target search key to uppercase for robust matching
        string searchKey = targetCourseNumber;
        transform(searchKey.begin(), searchKey.end(), searchKey.begin(), ::toupper);

        while (current != nullptr) {
            string currentKey = current->course.courseNumber;
            transform(currentKey.begin(), currentKey.end(), currentKey.begin(), ::toupper);

            if (currentKey == searchKey) {
                return &(current->course);
            }
            else if (searchKey < currentKey) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }

        return nullptr; // Course not found
    }
};

// Utility function to trim leading and trailing whitespace from a string
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

// Function to load, parse, and validate course data from a CSV file
bool loadCourses(string fileName, BinarySearchTree& bst, bool& dataLoaded) {
    // Clear any existing data in the tree and reset loaded status
    bst.clear();
    dataLoaded = false;

    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "Error: Unable to open file '" << fileName << "'." << endl;
        cout << "Tip: Please check the file name, verify spelling, and ensure the correct extension (e.g., .csv or .txt) is included." << endl;
        return false;
    }

    int lineNumber = 0;
    vector<string> validCourseNumbers;
    vector<Course> loadedCourses;

    string line;
    while (getline(file, line)) {
        lineNumber++;
        line = trim(line);

        // Skip blank lines
        if (line.empty()) {
            continue;
        }

        stringstream ss(line);
        string token;
        vector<string> tokens;

        // Split line by comma delimiter
        while (getline(ss, token, ',')) {
            tokens.push_back(trim(token));
        }

        // Validation Check 1: Minimum parameter requirement (Course Number & Title)
        if (tokens.size() < 2) {
            cout << "Error on line " << lineNumber << ": Invalid format. Minimum 2 parameters required." << endl;
            file.close();
            return false;
        }

        Course currentCourse;
        currentCourse.courseNumber = tokens[0];
        // Convert course number to uppercase for case-insensitive consistency
        transform(currentCourse.courseNumber.begin(), currentCourse.courseNumber.end(), currentCourse.courseNumber.begin(), ::toupper);
        currentCourse.title = tokens[1];

        // Validation Check 2: Ensure course number and title are not blank
        if (currentCourse.courseNumber.empty() || currentCourse.title.empty()) {
            cout << "Error on line " << lineNumber << ": Course number and title cannot be blank." << endl;
            file.close();
            return false;
        }

        // Validation Check 3: Prevent duplicate course entries in file
        auto it = find(validCourseNumbers.begin(), validCourseNumbers.end(), currentCourse.courseNumber);
        if (it != validCourseNumbers.end()) {
            cout << "Error on line " << lineNumber << ": Duplicate course number '" << currentCourse.courseNumber << "' detected." << endl;
            file.close();
            return false;
        }

        // Parse optional prerequisites (parameters from index 2 onwards)
        for (size_t i = 2; i < tokens.size(); i++) {
            string prereq = tokens[i];
            if (!prereq.empty()) {
                // NORMALIZE: Convert prerequisite to uppercase as well
                transform(prereq.begin(), prereq.end(), prereq.begin(), ::toupper);
                currentCourse.prerequisites.push_back(prereq);
            }
        }

        loadedCourses.push_back(currentCourse);
        validCourseNumbers.push_back(currentCourse.courseNumber);
    }

    file.close();

    // STEP 2: Validate Prerequisite Existence (Ensures prerequisites listed exist in file)
    for (const auto& course : loadedCourses) {
        for (const auto& prereq : course.prerequisites) {
            if (find(validCourseNumbers.begin(), validCourseNumbers.end(), prereq) == validCourseNumbers.end()) {
                cout << "Error: Prerequisite '" << prereq << "' required by " << course.courseNumber << " does not exist in the file." << endl;
                return false;
            }
        }
    }

    // STEP 3: Populate Binary Search Tree
    for (const auto& course : loadedCourses) {
        bst.insert(course);
    }

    dataLoaded = true;
    cout << "Data loaded successfully. Total courses: " << loadedCourses.size() << endl;
    return true;
}

// Function to print a single course's details and prerequisites with their titles
void printCourseInformation(BinarySearchTree& bst, string targetCourseNumber) {
    Course* course = bst.search(targetCourseNumber);

    if (course != nullptr) {
        cout << course->courseNumber << ", " << course->title << endl;
        if (!course->prerequisites.empty()) {
            cout << "Prerequisites: ";
            for (size_t i = 0; i < course->prerequisites.size(); ++i) {
                string prereqNum = course->prerequisites[i];
                Course* prereqCourse = bst.search(prereqNum);

                if (prereqCourse != nullptr) {
                    cout << prereqNum << " (" << prereqCourse->title << ")";
                }
                else {
                    cout << prereqNum; // Fallback if title lookup fails
                }

                if (i < course->prerequisites.size() - 1) {
                    cout << ", ";
                }
            }
            cout << endl;
        }
        else {
            cout << "Prerequisites: None" << endl;
        }
    }
    else {
        cout << "Course '" << targetCourseNumber << "' not found." << endl;
    }
}

// Function to print the complete course list in alphanumeric order
void printCourseList(BinarySearchTree& bst) {
    cout << "Course List:" << endl;
    bst.printAllInOrder();
}

// Main interactive menu loop
int main() {
    BinarySearchTree courseTree;
    bool dataLoaded = false;
    string fileName = "";
    string choiceInput = "";

    cout << "Welcome to the course advising planner." << endl;

    do {
        cout << "\nMenu:" << endl;
        cout << "  1. Load Data Structure." << endl;
        cout << "  2. Print Course List." << endl;
        cout << "  3. Print Course." << endl;
        cout << "  9. Exit." << endl;
        cout << "What would you like to do? ";

        // Read selection as string to completely prevent stream input failures
        getline(cin, choiceInput);
        choiceInput = trim(choiceInput);

        if (choiceInput == "1") {
            cout << "Enter the file name containing the course data (e.g., ABCU_Advising_Program_Input): ";
            getline(cin, fileName);
            fileName = trim(fileName);

            if (fileName.empty()) {
                fileName = "CS 300 ABCU_Advising_Program_Input.csv"; // Default fallback
            }
            else {
                // Automatically append .csv if no extension/dot is provided
                if (fileName.find('.') == string::npos) {
                    fileName += ".csv";
                }
            }

            loadCourses(fileName, courseTree, dataLoaded);
        }
        else if (choiceInput == "2") {
            if (!dataLoaded) {
                cout << "Error: Please load data structure first (Option 1)." << endl;
            }
            else {
                printCourseList(courseTree);
            }
        }
        else if (choiceInput == "3") {
            if (!dataLoaded) {
                cout << "Error: Please load data structure first (Option 1)." << endl;
            }
            else {
                string courseInput;
                cout << "What course do you want to know about? ";
                getline(cin, courseInput);
                printCourseInformation(courseTree, trim(courseInput));
            }
        }
        else if (choiceInput == "9") {
            cout << "Thank you for using the course advising planner!" << endl;
        }
        else {
            cout << "'" << choiceInput << "' is not a valid option. Please choose 1, 2, 3, or 9." << endl;
        }
    } while (choiceInput != "9");

    return 0;
}