#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#define MAX_PROFILES 100
#define DATABASE_FILE "health_profiles_db.dat"
#ifdef _WIN32
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#else
#include <unistd.h>
#endif

// Constants for validation
#define MIN_HEIGHT 0.5
#define MAX_HEIGHT 2.5
#define MIN_WEIGHT 20.0
#define MAX_WEIGHT 300.0
#define MAX_NAME_LENGTH 50
#define MAX_WEIGHT_HISTORY 12  // Increased to store 12 months of history

// Constants for BMI categories
#define BMI_UNDERWEIGHT 18.5
#define BMI_NORMAL 24.9
#define BMI_OVERWEIGHT 29.9
#define BMI_OBESE 34.9
#define BMI_SEVERELY_OBESE 39.9

// Constants for heart rate calculations
#define MAX_HEART_RATE_FACTOR 220
#define HEART_RATE_ZONES 5  // Number of heart rate training zones

// Error handling
typedef enum {
    SUCCESS,
    INVALID_DATE,
    INVALID_HEIGHT,
    INVALID_WEIGHT,
    INVALID_NAME,
    FILE_ERROR
} ErrorCode;

// New structure for heart rate zones
struct HeartRateZones {
    char name[20];
    float minPercentage;
    float maxPercentage;
    int minRate;
    int maxRate;
};

struct HealthData {
    float bmi;
    int maxHeartRate;
    struct HeartRateZones zones[HEART_RATE_ZONES];
    time_t lastUpdated;
    float idealWeight;      // New field for ideal weight range
    float weightToLose;     // New field for weight management
};

struct HealthProfile {
    char firstName[MAX_NAME_LENGTH];
    char lastName[MAX_NAME_LENGTH];
    int birthday[3];
    float height;
    float weight;
    int age;
    char gender;           // New field for gender-specific calculations
    struct HealthData healthData;
    float weightHistory[MAX_WEIGHT_HISTORY];
    time_t weightDates[MAX_WEIGHT_HISTORY];  // New array to track dates of weight measurements
    int weightHistoryCount;
    bool isMetric;        // New field for unit system preference
};

struct ProfileDatabase {
    struct HealthProfile profiles[MAX_PROFILES];
    int count;
};

// Function prototypes
void initializeProfile(struct HealthProfile* profile);
ErrorCode validateInput(struct HealthProfile* profile);
void calculateHealthMetrics(struct HealthProfile* profile);
void setupHeartRateZones(struct HealthProfile* profile);
void saveProfileToFile(const struct HealthProfile* profile);
ErrorCode loadProfileFromFile(struct HealthProfile* profile);
void exportHealthReport(const struct HealthProfile* profile);
void showBMIGraph(float bmi);
void generateWeightReport(const struct HealthProfile* profile);
int showMenu(void);
void updateWeight(struct HealthProfile* profile);
void listProfiles(const struct ProfileDatabase* db);
void deleteProfile(struct ProfileDatabase* db, int index);
ErrorCode loadDatabase(struct ProfileDatabase* db);
void saveDatabase(const struct ProfileDatabase* db);
int selectProfile(const struct ProfileDatabase* db);
void manageProfiles(struct ProfileDatabase* db);

void showLoadingAnimation(const char* message) {
    const char animation[] = "⠋⠙⠹⠸⠼⠴⠦⠧⠇⠏";
    static const int animationDelay = 40;
    
    #ifdef _WIN32
        system("cls");
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(hConsole, &cursorInfo);
        cursorInfo.bVisible = false;
        SetConsoleCursorInfo(hConsole, &cursorInfo);
    #else
        system("clear");
        printf("\e[?25l"); // Hide cursor
    #endif
    
    printf("\n\n");
    printf("    \033[1m%s\033[0m\n", message);
    
    for (int i = 0; i <= 30; i++) {
        printf("\r    \033[34mProcessing\033[0m %c [", animation[i % 10]);
        
        for (int j = 0; j < 20; j++) {
            if (j < (i * 20/30)) {
                printf("\033[32m█\033[0m");
            } else {
                printf("\033[90m░\033[0m");
            }
        }
        
        printf("] \033[33m%d%%\033[0m", (i * 100) / 30);
        fflush(stdout);
        
        #ifdef _WIN32
            Sleep(animationDelay);
        #else
            usleep(animationDelay * 1000);
        #endif
    }
    
    #ifdef _WIN32
        cursorInfo.bVisible = true;
        SetConsoleCursorInfo(hConsole, &cursorInfo);
    #else
        printf("\e[?25h"); // Show cursor
    #endif
}

void showBMIGraph(float bmi) {
    printf("\n    BMI Scale:\n");
    printf("    ╔════════════════════════════════════════════════╗\n    ║");
    
    const int graphWidth = 40;
    int position = (int)((bmi - 15) * (graphWidth / 30));
    position = position < 0 ? 0 : position >= graphWidth ? graphWidth - 1 : position;
    
    for (int i = 0; i < graphWidth; i++) {
        if (i == position) {
            printf("\033[1;31m▼\033[0m");
        } else if (i < graphWidth/4) {
            printf("\033[33m─\033[0m");  // Underweight
        } else if (i < graphWidth/2) {
            printf("\033[32m─\033[0m");  // Normal
        } else if (i < 3*graphWidth/4) {
            printf("\033[33m─\033[0m");  // Overweight
        } else {
            printf("\033[31m─\033[0m");  // Obese
        }
    }
    
    printf("║\n    ╚════════════════════════════════════════════════╝\n");
    printf("     15   18.5     24.9     29.9     35      40+\n");
}

void generateWeightReport(const struct HealthProfile* profile) {
    if (profile->weightHistoryCount < 2) {
        printf("\n\033[33mNot enough weight history for detailed analysis.\033[0m\n");
        return;
    }

    float totalChange = profile->weight - profile->weightHistory[0];
    float avgChange = totalChange / (profile->weightHistoryCount - 1);
    
    printf("\n\033[1;36m► Weight Analysis Report\033[0m\n");
    printf("   Initial Weight: %.1f kg\n", profile->weightHistory[0]);
    printf("   Current Weight: %.1f kg\n", profile->weight);
    printf("   Total Change: %.1f kg (%s)\n", 
           fabs(totalChange),
           totalChange > 0 ? "\033[31m↑\033[0m" : "\033[32m↓\033[0m");
    printf("   Average Change: %.1f kg per measurement\n", avgChange);
    
    // Visual representation of weight changes
    printf("\n   Weight Trend:\n   ");
    for (int i = 0; i < profile->weightHistoryCount - 1; i++) {
        float change = profile->weightHistory[i + 1] - profile->weightHistory[i];
        if (change > 0)
            printf("\033[31m↗\033[0m");
        else if (change < 0)
            printf("\033[32m↘\033[0m");
        else
            printf("→");
    }
    printf("\n");
}

void showHealthProfile(const struct HealthProfile* profile) {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    // Header
    printf("\033[1;34m╔═══════════════════════════════════════╗\n");
    printf("║         HEALTH PROFILE REPORT         ║\n");
    printf("╚═══════════════════════════════════════╝\033[0m\n\n");

    // Personal Information
    printf("\033[1;36m► Personal Information\033[0m\n");
    printf("   Name: %s %s\n", profile->firstName, profile->lastName);
    printf("   Age: %d years | Gender: %c\n", profile->age, profile->gender);
    printf("   Height: %.2f m | Weight: %.1f kg\n\n", profile->height, profile->weight);

    // BMI Information
    printf("\033[1;36m► Body Mass Index (BMI)\033[0m\n");
    printf("   Current BMI: %.1f kg/m²\n", profile->healthData.bmi);
    showBMIGraph(profile->healthData.bmi);
    printf("   Ideal Weight Range: %.1f - %.1f kg\n\n", 
           profile->healthData.idealWeight - 2.5,
           profile->healthData.idealWeight + 2.5);

    // Heart Rate Zones
    printf("\033[1;36m► Heart Rate Training Zones\033[0m\n");
    printf("   Maximum Heart Rate: %d bpm\n", profile->healthData.maxHeartRate);
    for (int i = 0; i < HEART_RATE_ZONES; i++) {
        printf("   • %s (%d-%d bpm)\n", 
               profile->healthData.zones[i].name,
               profile->healthData.zones[i].minRate,
               profile->healthData.zones[i].maxRate);
    }

    // Weight History and Analysis
    generateWeightReport(profile);

    // Platform-specific time string generation
    char timeStr[26];
    #ifdef _WIN32
        ctime_s(timeStr, sizeof(timeStr), &profile->healthData.lastUpdated);
    #else
        ctime_r(&profile->healthData.lastUpdated, timeStr);
    #endif
    timeStr[24] = '\0';  // Remove newline
    printf("\n\033[90mLast updated: %s\033[0m\n", timeStr);
}

// ... Additional implementation of other functions ...
// Add these new function implementations before the main function
// Modify showMenu() to include the new option
int showMenu(void) {
    printf("\n\033[1;36mHealth Profile Menu:\033[0m\n");
    printf("1. Show Current Health Profile\n");
    printf("2. Update Weight\n");
    printf("3. Export Health Report\n");
    printf("4. Manage Profiles\n");  // New option
    printf("5. Exit\n");            // Changed from 4 to 5
    printf("\nEnter your choice (1-5): ");
    
    int choice;
    scanf("%d", &choice);
    while(getchar() != '\n');
    
    return choice;
}

void updateWeight(struct HealthProfile* profile) {
    float newWeight;
    printf("\nEnter new weight (in kg): ");
    scanf("%f", &newWeight);
    while(getchar() != '\n'); // Clear input buffer
    
    // Validate input
    if (newWeight < MIN_WEIGHT || newWeight > MAX_WEIGHT) {
        printf("\n\033[31mError: Weight must be between %.1f and %.1f kg\033[0m\n", 
               MIN_WEIGHT, MAX_WEIGHT);
        return;
    }
    
    // Shift weight history
    if (profile->weightHistoryCount < MAX_WEIGHT_HISTORY) {
        profile->weightHistoryCount++;
    } else {
        // Shift array to make room for new weight
        for (int i = 0; i < MAX_WEIGHT_HISTORY - 1; i++) {
            profile->weightHistory[i] = profile->weightHistory[i + 1];
            profile->weightDates[i] = profile->weightDates[i + 1];
        }
    }
    
    // Update weight and time
    profile->weight = newWeight;
    profile->weightHistory[profile->weightHistoryCount - 1] = newWeight;
    profile->weightDates[profile->weightHistoryCount - 1] = time(NULL);
    
    // Recalculate health metrics
    calculateHealthMetrics(profile);
    
    printf("\n\033[32mWeight updated successfully!\033[0m\n");
}

void initializeProfile(struct HealthProfile* profile) {
    printf("\n\033[1;36m► Create New Health Profile\033[0m\n\n");
    
    printf("Enter first name: ");
    scanf("%49s", profile->firstName);
    while(getchar() != '\n');
    
    printf("Enter last name: ");
    scanf("%49s", profile->lastName);
    while(getchar() != '\n');
    
    printf("Enter birth date (DD MM YYYY): ");
    scanf("%d %d %d", &profile->birthday[0], &profile->birthday[1], &profile->birthday[2]);
    while(getchar() != '\n');
    
    printf("Enter height (in meters): ");
    scanf("%f", &profile->height);
    while(getchar() != '\n');
    
    printf("Enter weight (in kg): ");
    scanf("%f", &profile->weight);
    while(getchar() != '\n');
    
    printf("Enter gender (M/F): ");
    scanf("%c", &profile->gender);
    while(getchar() != '\n');
    
    // Initialize other fields
    profile->weightHistoryCount = 1;
    profile->weightHistory[0] = profile->weight;
    profile->weightDates[0] = time(NULL);
    profile->isMetric = true;
    
    // Calculate age
    time_t now = time(NULL);
    struct tm* current_time = localtime(&now);
    profile->age = current_time->tm_year + 1900 - profile->birthday[2];
    if (current_time->tm_mon + 1 < profile->birthday[1] || 
        (current_time->tm_mon + 1 == profile->birthday[1] && 
         current_time->tm_mday < profile->birthday[0])) {
        profile->age--;
    }
    
    // Calculate health metrics
    calculateHealthMetrics(profile);
    
    printf("\n\033[32mProfile created successfully!\033[0m\n");
}

void calculateHealthMetrics(struct HealthProfile* profile) {
    // Calculate BMI
    profile->healthData.bmi = profile->weight / (profile->height * profile->height);
    
    // Calculate max heart rate
    profile->healthData.maxHeartRate = MAX_HEART_RATE_FACTOR - profile->age;
    
    // Calculate ideal weight (using Hamwi formula)
    float baseWeight;
    if (profile->gender == 'M' || profile->gender == 'm') {
        baseWeight = 48.0 + 2.7 * (profile->height * 100 - 152.4) / 2.54;
    } else {
        baseWeight = 45.5 + 2.2 * (profile->height * 100 - 152.4) / 2.54;
    }
    profile->healthData.idealWeight = baseWeight;
    
    // Calculate weight to lose/gain
    profile->healthData.weightToLose = profile->weight - profile->healthData.idealWeight;
    
    // Setup heart rate zones
    setupHeartRateZones(profile);
    
    // Update last updated timestamp
    profile->healthData.lastUpdated = time(NULL);
}

void setupHeartRateZones(struct HealthProfile* profile) {
    struct HeartRateZones zones[HEART_RATE_ZONES] = {
        {"Recovery", 0.50, 0.60},
        {"Fat Burn", 0.60, 0.70},
        {"Aerobic", 0.70, 0.80},
        {"Anaerobic", 0.80, 0.90},
        {"Maximum", 0.90, 1.00}
    };
    
    for (int i = 0; i < HEART_RATE_ZONES; i++) {
        strcpy(profile->healthData.zones[i].name, zones[i].name);
        profile->healthData.zones[i].minPercentage = zones[i].minPercentage;
        profile->healthData.zones[i].maxPercentage = zones[i].maxPercentage;
        profile->healthData.zones[i].minRate = 
            (int)(profile->healthData.maxHeartRate * zones[i].minPercentage);
        profile->healthData.zones[i].maxRate = 
            (int)(profile->healthData.maxHeartRate * zones[i].maxPercentage);
    }
}

ErrorCode validateInput(struct HealthProfile* profile) {
    // Validate height
    if (profile->height < MIN_HEIGHT || profile->height > MAX_HEIGHT) {
        return INVALID_HEIGHT;
    }
    
    // Validate weight
    if (profile->weight < MIN_WEIGHT || profile->weight > MAX_WEIGHT) {
        return INVALID_WEIGHT;
    }
    
    // Validate name lengths
    if (strlen(profile->firstName) == 0 || strlen(profile->lastName) == 0 ||
        strlen(profile->firstName) >= MAX_NAME_LENGTH || 
        strlen(profile->lastName) >= MAX_NAME_LENGTH) {
        return INVALID_NAME;
    }
    
    return SUCCESS;
}

void saveProfileToFile(const struct HealthProfile* profile) {
    FILE* file = fopen("health_profile.dat", "wb");
    if (file == NULL) {
        printf("\n\033[31mError: Could not save profile to file.\033[0m\n");
        return;
    }
    
    fwrite(profile, sizeof(struct HealthProfile), 1, file);
    fclose(file);
    
    showLoadingAnimation("Saving profile to file...");
    printf("\n\n\033[32mProfile saved successfully!\033[0m\n");
}

ErrorCode loadProfileFromFile(struct HealthProfile* profile) {
    FILE* file = fopen("health_profile.dat", "rb");
    if (file == NULL) {
        return FILE_ERROR;
    }
    
    showLoadingAnimation("Loading profile from file...");
    
    size_t read = fread(profile, sizeof(struct HealthProfile), 1, file);
    fclose(file);
    
    if (read != 1) {
        return FILE_ERROR;
    }
    
    return SUCCESS;
}

void exportHealthReport(const struct HealthProfile* profile) {
    FILE* file = fopen("health_report.txt", "w");
    if (file == NULL) {
        printf("\n\033[31mError: Could not create health report.\033[0m\n");
        return;
    }
    
    // Write header
    fprintf(file, "HEALTH PROFILE REPORT\n");
    fprintf(file, "====================\n\n");
    
    // Personal Information
    fprintf(file, "Personal Information\n");
    fprintf(file, "-------------------\n");
    fprintf(file, "Name: %s %s\n", profile->firstName, profile->lastName);
    fprintf(file, "Age: %d years\n", profile->age);
    fprintf(file, "Height: %.2f m\n", profile->height);
    fprintf(file, "Weight: %.1f kg\n\n", profile->weight);
    
    // BMI Information
    fprintf(file, "Body Mass Index (BMI)\n");
    fprintf(file, "-------------------\n");
    fprintf(file, "Current BMI: %.1f kg/m²\n", profile->healthData.bmi);
    fprintf(file, "BMI Category: ");
    
    if (profile->healthData.bmi < BMI_UNDERWEIGHT)
        fprintf(file, "Underweight\n");
    else if (profile->healthData.bmi < BMI_NORMAL)
        fprintf(file, "Normal weight\n");
    else if (profile->healthData.bmi < BMI_OVERWEIGHT)
        fprintf(file, "Overweight\n");
    else if (profile->healthData.bmi < BMI_OBESE)
        fprintf(file, "Obese\n");
    else
        fprintf(file, "Severely obese\n");
    
    fprintf(file, "Ideal Weight Range: %.1f - %.1f kg\n\n",
            profile->healthData.idealWeight - 2.5,
            profile->healthData.idealWeight + 2.5);
    
    // Heart Rate Information
    fprintf(file, "Heart Rate Training Zones\n");
    fprintf(file, "----------------------\n");
    fprintf(file, "Maximum Heart Rate: %d bpm\n\n", profile->healthData.maxHeartRate);
    
    for (int i = 0; i < HEART_RATE_ZONES; i++) {
        fprintf(file, "%s Zone: %d-%d bpm\n",
                profile->healthData.zones[i].name,
                profile->healthData.zones[i].minRate,
                profile->healthData.zones[i].maxRate);
    }
    
    // Weight History
    if (profile->weightHistoryCount >= 2) {
        float totalChange = profile->weight - profile->weightHistory[0];
        float avgChange = totalChange / (profile->weightHistoryCount - 1);
        
        fprintf(file, "\nWeight History Analysis\n");
        fprintf(file, "--------------------\n");
        fprintf(file, "Initial Weight: %.1f kg\n", profile->weightHistory[0]);
        fprintf(file, "Current Weight: %.1f kg\n", profile->weight);
        fprintf(file, "Total Change: %.1f kg\n", fabs(totalChange));
        fprintf(file, "Average Change: %.1f kg per measurement\n", avgChange);
    }
    
    // Add timestamp
    time_t now = time(NULL);
    fprintf(file, "\nReport generated: %s", ctime(&now));
    
    fclose(file);
    
    showLoadingAnimation("Generating health report...");
    printf("\n\n\033[32mHealth report exported successfully to 'health_report.txt'!\033[0m\n");
}

#ifdef _WIN32
void enableConsoleSupport() {
    // Enable UTF-8 output
    SetConsoleOutputCP(CP_UTF8);
    
    // Enable ANSI escape sequences
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD consoleMode;
    GetConsoleMode(hConsole, &consoleMode);
    SetConsoleMode(hConsole, consoleMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}
#endif







// New function implementations
void listProfiles(const struct ProfileDatabase* db) {
    printf("\n\033[1;36m► Available Health Profiles\033[0m\n\n");
    if (db->count == 0) {
        printf("No profiles found.\n");
        return;
    }
    
    printf("ID  Name                  Age  Gender  BMI\n");
    printf("───────────────────────────────────────────\n");
    for (int i = 0; i < db->count; i++) {
        printf("%-3d %-20s %-4d %-7c %.1f\n",
               i + 1,
               strcat(strcat(strcpy((char[MAX_NAME_LENGTH]){0}, 
               db->profiles[i].firstName), " "), 
               db->profiles[i].lastName),
               db->profiles[i].age,
               db->profiles[i].gender,
               db->profiles[i].healthData.bmi);
    }
    printf("\n");
}

void deleteProfile(struct ProfileDatabase* db, int index) {
    if (index < 0 || index >= db->count) {
        printf("\n\033[31mInvalid profile ID.\033[0m\n");
        return;
    }
    
    char name[MAX_NAME_LENGTH * 2];
    sprintf(name, "%s %s", db->profiles[index].firstName, db->profiles[index].lastName);
    
    printf("\nAre you sure you want to delete the profile for %s? (y/n): ", name);
    char confirm;
    scanf(" %c", &confirm);
    while(getchar() != '\n');
    
    if (confirm == 'y' || confirm == 'Y') {
        // Shift remaining profiles
        for (int i = index; i < db->count - 1; i++) {
            db->profiles[i] = db->profiles[i + 1];
        }
        db->count--;
        saveDatabase(db);
        printf("\n\033[32mProfile deleted successfully.\033[0m\n");
    }
}

ErrorCode loadDatabase(struct ProfileDatabase* db) {
    FILE* file = fopen(DATABASE_FILE, "rb");
    if (file == NULL) {
        db->count = 0;
        return FILE_ERROR;
    }
    
    fread(&db->count, sizeof(int), 1, file);
    fread(db->profiles, sizeof(struct HealthProfile), db->count, file);
    fclose(file);
    
    return SUCCESS;
}

void saveDatabase(const struct ProfileDatabase* db) {
    FILE* file = fopen(DATABASE_FILE, "wb");
    if (file == NULL) {
        printf("\n\033[31mError: Could not save database.\033[0m\n");
        return;
    }
    
    fwrite(&db->count, sizeof(int), 1, file);
    fwrite(db->profiles, sizeof(struct HealthProfile), db->count, file);
    fclose(file);
}

int selectProfile(const struct ProfileDatabase* db) {
    listProfiles(db);
    if (db->count == 0) return -1;
    
    int choice;
    printf("Enter profile ID (1-%d) or 0 to cancel: ", db->count);
    scanf("%d", &choice);
    while(getchar() != '\n');
    
    if (choice == 0) return -1;
    if (choice < 1 || choice > db->count) {
        printf("\n\033[31mInvalid profile ID.\033[0m\n");
        return -1;
    }
    
    return choice - 1;
}

void manageProfiles(struct ProfileDatabase* db) {
    int choice;
    do {
        printf("\n\033[1;36mProfile Management Menu:\033[0m\n");
        printf("1. List All Profiles\n");
        printf("2. View Profile Details\n");
        printf("3. Create New Profile\n");
        printf("4. Edit Profile\n");
        printf("5. Delete Profile\n");
        printf("6. Return to Main Menu\n");
        printf("\nEnter your choice (1-6): ");
        
        scanf("%d", &choice);
        while(getchar() != '\n');
        
        switch (choice) {
            case 1:
                listProfiles(db);
                break;
                
            case 2: {
                int index = selectProfile(db);
                if (index >= 0) {
                    showHealthProfile(&db->profiles[index]);
                }
                break;
            }
                
            case 3:
                if (db->count >= MAX_PROFILES) {
                    printf("\n\033[31mError: Maximum number of profiles reached.\033[0m\n");
                    break;
                }
                initializeProfile(&db->profiles[db->count]);
                db->count++;
                saveDatabase(db);
                break;
                
            case 4: {
                int index = selectProfile(db);
                if (index >= 0) {
                    printf("\nUpdating profile for %s %s\n",
                           db->profiles[index].firstName,
                           db->profiles[index].lastName);
                    updateWeight(&db->profiles[index]);
                    saveDatabase(db);
                }
                break;
            }
                
            case 5: {
                int index = selectProfile(db);
                if (index >= 0) {
                    deleteProfile(db, index);
                }
                break;
            }
                
            case 6:
                printf("\n\033[32mReturning to main menu...\033[0m\n");
                break;
                
            default:
                printf("\n\033[31mInvalid choice. Please try again.\033[0m\n");
        }
    } while (choice != 6);
}










// Modify main() to use the database
int main() {
    #ifdef _WIN32
    enableConsoleSupport();
    #endif
    
    struct ProfileDatabase db;
    int choice;
    int currentProfileIndex = -1;
    
    printf("\n\033[1;34m╔════════════════════════════════════════╗");
    printf("\n║  WELCOME TO HEALTH PROFILE SYSTEM     ║");
    printf("\n╚════════════════════════════════════════╝\033[0m\n");
    
    // Load the database
    if (loadDatabase(&db) == SUCCESS && db.count > 0) {
        printf("\n\033[32mExisting profiles loaded successfully!\033[0m\n");
    }
    
    do {
        if (currentProfileIndex == -1 && db.count > 0) {
            printf("\nSelect a profile to work with:\n");
            currentProfileIndex = selectProfile(&db);
            if (currentProfileIndex == -1 && db.count == 0) {
                printf("\nNo profiles found. Creating new profile...\n");
                initializeProfile(&db.profiles[0]);
                db.count = 1;
                currentProfileIndex = 0;
                saveDatabase(&db);
            }
        }
        
        choice = showMenu();
        
        switch (choice) {
            case 1:
                if (currentProfileIndex >= 0) {
                    showHealthProfile(&db.profiles[currentProfileIndex]);
                }
                break;
            case 2:
                if (currentProfileIndex >= 0) {
                    updateWeight(&db.profiles[currentProfileIndex]);
                    saveDatabase(&db);
                }
                break;
            case 3:
                if (currentProfileIndex >= 0) {
                    exportHealthReport(&db.profiles[currentProfileIndex]);
                }
                break;
            case 4:
                manageProfiles(&db);
                break;
            case 5:
                printf("\n\033[1;32mThank you for using the Health Profile System!\033[0m\n");
                break;
            default:
                printf("\n\033[31mInvalid choice. Please try again.\033[0m\n");
        }
    } while (choice != 5);
    
    return 0;
}