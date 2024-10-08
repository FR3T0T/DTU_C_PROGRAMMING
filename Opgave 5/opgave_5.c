#include <stdio.h>
#include <string.h>

int countCharactersInSet(const char* text, const char* set) {
    int count = 0;
    while (*text != '\0') {
        const char* setPtr = set;
        while (*setPtr != '\0') {
            if (*text == *setPtr) {
                count++;
                break;
            }
            setPtr++;
        }
        text++;
    }
    return count;
}

int main() {
    const int maxlength = 100;  // define array size
    char mytext[maxlength];     // array to contain text
    const char* vowels = "aeiouAEIOU";

    printf("Indtast en tekst (max %d tegn): ", maxlength - 1);
    fgets(mytext, maxlength, stdin);  // read text from console

    // Remove newline character if present
    mytext[strcspn(mytext, "\n")] = 0;

    int vowelCount = countCharactersInSet(mytext, vowels);
    printf("Antal vokaler i \"%s\": %d\n", mytext, vowelCount);

    return 0;
}