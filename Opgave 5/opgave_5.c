#include <stdio.h>

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
    const char* text = "Denmark is a democratic country";
    const char* vowels = "aeiouAEIOU";
    int vowelCount = countCharactersInSet(text, vowels);
    printf("Antal vokaler i \"%s\": %d\n", text, vowelCount);
    return 0;
}