#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <limits> // Required for std::numeric_limits

#define MAX_PROJECTS 10
#define MAX_TEXT_LENGTH 256
#define FILENAME "portfolio_data.txt"

// Structure to hold portfolio data
struct Portfolio {
    std::string about;
    std::string projects[MAX_PROJECTS];
    int project_count;
    std::string phone;
    std::string linkedin;
    std::string gmail;
};

Portfolio portfolio;

bool contact_initialized = false; // Flag to check if contact details have been initialized
bool about_initialized = false; // Flag to check if initial details have been added

// Function to initialize portfolio data
void init_portfolio() {
    portfolio.about = "No information added.";
    portfolio.project_count = 0;
    portfolio.phone = "No phone number added.";
    portfolio.linkedin = "No LinkedIn profile added.";
    portfolio.gmail = "No Gmail address added.";
}

// Function to load portfolio from a file
void load_portfolio() {
    std::ifstream file(FILENAME);
    if (!file.is_open()) {
        std::cout << "No saved portfolio found. Starting a new portfolio.\n";
        return;
    }
    file.read(reinterpret_cast<char*>(&portfolio), sizeof(Portfolio));
    file.close();
    std::cout << "Portfolio loaded successfully.\n";
}

// Function to save portfolio to a file
void save_portfolio() {
    std::ofstream file(FILENAME, std::ios::app | std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << FILENAME << "\n";
        return;
    }
    file.write(reinterpret_cast<const char*>(&portfolio), sizeof(Portfolio));
    if (!file.good()) {
        std::cerr << "Error writing to file.\n";
    } else {
        std::cout << "Portfolio saved successfully.\n";
    }
    file.close();
}

// Function to display the main menu
void show_menu() {
    std::cout << "\n==============================\n";
    std::cout << " Portfolio Creation CLI\n";
    std::cout << "==============================\n";
    std::cout << "Commands:\n";
    std::cout << "  edit_about     - Edit About Me section\n";
    std::cout << "  add_project    - Add a new project\n";
    std::cout << "  view_projects  - View all projects\n";
    std::cout << "  edit_project   - Edit a specific project\n";
    std::cout << "  delete_project - Delete a specific project\n";
    std::cout << "  edit_contact   - Edit contact information\n";
    std::cout << "  display        - Display entire portfolio\n";
    std::cout << "  save           - Save portfolio to file\n";
    std::cout << "  load           - Load portfolio from file\n";
    std::cout << "  exit           - Exit the CLI\n";
    std::cout << "Enter a command: ";
}

// Function to edit the About Me section
void edit_about() {
    std::cout << "\n--- Edit About Me ---\n";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer

    if (!about_initialized || portfolio.about == "No information added.") {
        // First time setup
        std::string name, skills, experience, education, achievements;

        std::cout << "Enter your name: ";
        std::getline(std::cin, name);

        std::cout << "Enter your skills: ";
        std::getline(std::cin, skills);

        std::cout << "Enter your experience: ";
        std::getline(std::cin, experience);

        std::cout << "Enter your education: ";
        std::getline(std::cin, education);

        std::cout << "Enter your achievements (optional, press Enter to skip): ";
        std::getline(std::cin, achievements);

        portfolio.about = "Name: " + name + "\nSkills: " + skills + "\nExperience: " + experience +
                          "\nEducation: " + education + "\nAchievements: " + (achievements.empty() ? "None" : achievements);

        about_initialized = true;
        std::cout << "About Me section initialized successfully.\n";
    } else {
        // Append new information to the existing sections
        std::cout << "Existing About Me:\n" << portfolio.about << "\n\n";

        std::string additional_info;
        std::cout << "Enter additional information to append to the 'About Me' section: ";
        std::getline(std::cin, additional_info);

        if (portfolio.about.length() + additional_info.length() + 2 < MAX_TEXT_LENGTH) {
            portfolio.about += "\n" + additional_info;
            std::cout << "Information appended to the About Me section.\n";
        } else {
            std::cout << "Error: Not enough space to add additional information.\n";
        }
    }
}

// Function to add a project
void add_project() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer
    if (portfolio.project_count < MAX_PROJECTS) {
        std::cout << "\n--- Add a Project ---\n";
        std::cout << "Enter the project description: ";
        std::getline(std::cin, portfolio.projects[portfolio.project_count]);
        portfolio.project_count++;
        std::cout << "Project added successfully.\n";
    } else {
        std::cout << "Maximum number of projects reached.\n";
    }
}

// Function to view all projects
void view_projects() {
    std::cout << "\n--- Projects ---\n";
    for (int i = 0; i < portfolio.project_count; i++) {
        std::cout << "  " << i + 1 << ". " << portfolio.projects[i] << "\n";
    }
}

// Function to edit a specific project
void edit_project() {
    int index;
    std::string new_description;
    char choice;

    view_projects();
    std::cout << "Enter the project number to edit: ";
    
    // Check for a valid integer and clear the stream if it fails
    if (!(std::cin >> index)) {
        std::cout << "Invalid input. Please enter a valid project number.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (index > 0 && index <= portfolio.project_count) {
        std::cout << "Would you like to (O)verwrite or (A)ppend to this project? (O/A): ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Enter new description for project " << index << ": ";
        std::getline(std::cin, new_description);

        if (choice == 'O' || choice == 'o') {
            portfolio.projects[index - 1] = new_description;
            std::cout << "Project " << index << " description overwritten successfully.\n";
        } else if (choice == 'A' || choice == 'a') {
            if (portfolio.projects[index - 1].length() + new_description.length() + 1 < MAX_TEXT_LENGTH) {
                portfolio.projects[index - 1] += " " + new_description;
                std::cout << "Project " << index << " description appended successfully.\n";
            } else {
                std::cout << "Error: Not enough space to add additional information.\n";
            }
        } else {
            std::cout << "Invalid choice. Please enter 'O' to overwrite or 'A' to append.\n";
        }
    } else {
        std::cout << "Invalid project number.\n";
    }
}

// Function to delete a specific project
void delete_project() {
    int index;
    if (portfolio.project_count == 0) {
        std::cout << "No projects to delete.\n";
        return;
    }

    view_projects();
    std::cout << "Enter the project number to delete: ";
    if (!(std::cin >> index)) {
        std::cout << "Invalid input. Please enter a valid project number.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (index > 0 && index <= portfolio.project_count) {
        for (int i = index - 1; i < portfolio.project_count - 1; i++) {
            portfolio.projects[i] = portfolio.projects[i + 1];
        }
        portfolio.project_count--;
        std::cout << "Project deleted successfully.\n";
    } else {
        std::cout << "Invalid project number.\n";
    }
}

// Function to check if a string represents a valid phone number
bool is_valid_phone_number(const std::string& phone) {
    if (phone.length() != 10) return false;
    for (char c : phone) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}

// Function to edit contact information
void edit_contact() {
    std::cout << "\n--- Edit Contact Information ---\n";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer

    if (!contact_initialized) {
        // First-time setup for contact details
        std::string phone, gmail, linkedin;

        while (true) {
            std::cout << "Enter your phone number (10 digits only): ";
            std::getline(std::cin, phone);
            if (is_valid_phone_number(phone)) {
                portfolio.phone = phone;
                break;
            } else {
                std::cout << "Error: Please enter a valid phone number (exactly 10 digits).\n";
            }
        }

        std::cout << "Enter your Gmail: ";
        std::getline(std::cin, gmail);
        portfolio.gmail = gmail;

        std::cout << "Enter your LinkedIn profile URL: ";
        std::getline(std::cin, linkedin);
        portfolio.linkedin = linkedin;

        contact_initialized = true;
        std::cout << "Contact information initialized successfully.\n";
    } else {
        // Display current contact details
        std::cout << "Existing Contact Information:\n";
        std::cout << "Phone: " << portfolio.phone << "\n";
        std::cout << "Gmail: " << portfolio.gmail << "\n";
        std::cout << "LinkedIn: " << portfolio.linkedin << "\n\n";

        char choice;
        std::cout << "Do you want to update any of the contact information? (Y/N): ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 'Y' || choice == 'y') {
            std::string phone, gmail, linkedin;

            std::cout << "Enter new phone number (leave blank to keep existing): ";
            std::getline(std::cin, phone);
            if (!phone.empty()) {
                if (is_valid_phone_number(phone)) {
                    portfolio.phone = phone;
                } else {
                    std::cout << "Error: Please enter a valid phone number (exactly 10 digits).\n";
                    return;
                }
            }

            std::cout << "Enter new Gmail (leave blank to keep existing): ";
            std::getline(std::cin, gmail);
            if (!gmail.empty()) {
                portfolio.gmail = gmail;
            }

            std::cout << "Enter new LinkedIn profile (leave blank to keep existing): ";
            std::getline(std::cin, linkedin);
            if (!linkedin.empty()) {
                portfolio.linkedin = linkedin;
            }

            std::cout << "Contact information updated successfully.\n";
        }
    }
}

// Function to display the portfolio
void display_portfolio() {
    std::cout << "\n--- Portfolio ---\n";
    std::cout << "About Me: " << portfolio.about << "\n";
    std::cout << "Projects:\n";
    if (portfolio.project_count == 0) {
        std::cout << "  No projects added.\n";
    } else {
        for (int i = 0; i < portfolio.project_count; i++) {
            std::cout << "  " << i + 1 << ". " << portfolio.projects[i] << "\n";
        }
    }
    std::cout << "Contact Information:\n";
    std::cout << "  Phone: " << portfolio.phone << "\n";
    std::cout << "  Gmail: " << portfolio.gmail << "\n";
    std::cout << "  LinkedIn: " << portfolio.linkedin << "\n\n";
}

int main() {
    std::string command;
    init_portfolio();
    load_portfolio();

    while (true) {
        show_menu();
        std::getline(std::cin, command);

        if (command == "edit_about") {
            edit_about();
        } else if (command == "add_project") {
            add_project();
        } else if (command == "view_projects") {
            view_projects();
        } else if (command == "edit_project") {
            edit_project();
        } else if (command == "delete_project") {
            delete_project();
        } else if (command == "edit_contact") {
            edit_contact();
        } else if (command == "display") {
            display_portfolio();
        } else if (command == "save") {
            save_portfolio();
        } else if (command == "load") {
            load_portfolio();
        } else if (command == "exit") {
            save_portfolio();
            std::cout << "Exiting the portfolio CLI. Goodbye!\n";
            break;
        } else {
            std::cout << "Unknown command. Please enter a valid command.\n";
        }
    }
    return 0;
}
