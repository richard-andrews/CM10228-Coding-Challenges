//necessary imported libraries to print and read from file
#include <stdio.h>
#include <stdlib.h>


//a structure that holds a string (array of characters) and the length of that string
struct word{
    int length;
    char *data;
};


//initialise methods
char **readRedact();
int wordsMatch(struct word word1, const char *word2);
void printRedactables(char *const *redactables, int numberOfRedactions, int maximumLengthOfRedaction);
void printWord(struct word toPrint);
void readDebate(char *const *redactables, int numberOfRedactions);


//a method that reads from the text file and prints either the word or the redaction
void readDebate(char *const *redactables, int numberOfRedactions) {
    //declare variable to open the text file
    FILE *file;
    file = fopen("debate.txt", "r");

    //only runs if the file has been found and opened successfully
    if(file != NULL) {

        //initialise current word buffer
        struct word current;
        current.length = 1;
        current.data = malloc(current.length * sizeof(char));

        //initialise current character buffer
        char temp;

        //loop through all characters in the file while not at the end of the file
        while ((temp = (char) getc(file)) != EOF) {

            //if the current character is not separating a word, add the character to the word buffer
            if ((((temp) >= 'A') && (temp <= 'Z')) || ((temp >= 'a') && (temp <= 'z'))){
                //add the character buffer to the word buffer and increase the memory allocated to the word buffer
                current.data[current.length - 1] = temp;
                current.length++;
                current.data = realloc(current.data, current.length * sizeof(char));
            }
                //if the current character is separating a word, check if the word is to be redacted and print the word or its redaction
            else{
                //assume that a match has not been found, acts as a boolean where 0 = false and 1 = true
                int match = 0;

                //loop through the redactables not including the lines dedicated to metadata
                for(int i = 2; i< numberOfRedactions;i++){
                    //calls a method that returns a integer acting as a boolean according to if two words match or not
                    match = wordsMatch(current, redactables[i]);

                    //if 1 was returned, a match was found and the loop does not need to continue
                    if (match == 1){
                        break;
                    }
                }

                //if a match was found, replace the word in the word buffer with *s
                if (match == 1) {
                    for(int i = 0; i < current.length-1; i++){
                        current.data[i] = '*';
                    }
                }

                //print the word in the word buffer followed by the character separating the words
                printWord(current);
                printf("%c", temp);

                //reallocate the memory in the word buffer to the reset
                free(current.data);
                current.length = 1;
                current.data = malloc(current.length * sizeof(char));
            }
        }
        fclose(file);
    }
        //if the file could not be found or opened
    else{
        printf("Debate file not opened.");
    }

}


//a method to print the current word in the word buffer
void printWord(struct word toPrint) {
    //loops through the characters in word and prints them out
    for (int i = 0; i < toPrint.length-1; i++){
        if ((toPrint.data[i] > 0) && (toPrint.data[0] < 'z')) {
            printf("%c", toPrint.data[i]);
        }
    }
}


//a method that prints the list of words to be redacted in the text
void printRedactables(char *const *redactables, int numberOfRedactions, int maximumLengthOfRedaction) {
    //loops through the words to redact excluding the first two lines that contain metadata
    for (int i = 2; i < numberOfRedactions - 1; i++){
        //loops through the characters in each word and prints the alphabetic letters
        for (int j = 0; j < maximumLengthOfRedaction; j++){
            if (((redactables[i][j] >= 'A') && (redactables[i][j] <= 'Z')) || ((redactables[i][j] >= 'a') && (redactables[i][j] <= 'z')))
                printf("%c", redactables[i][j]);
        }
        printf("\n");
    }
}


//a method that compares two words to determine if they match exactly or not replacing the string library function
int wordsMatch(struct word word1, const char *word2) {
    //printf("%s %s \n", word1.data, word2);

    //if the word does not begin with a letter, it does not match any of the words in the list of words to redact
    if(!((word1.data[0] >= 'A' && word1.data[0] <= 'Z') || (word1.data[0] >= 'a' && word1.data[0] <= 'z'))){
        return 0;
    }

    //calculates the length of the two words to compare
    int length1 = word1.length;

    int length2 = 1;
    for (int i = 0; i < 100; i++) {
        if (!(((word2[i] >= 'A') && (word2[i] <= 'Z')) || ((word2[i] >= 'a') && (word2[i] <= 'z')))) {
            break;
        } else {
            length2++;
        }
    }

    //only compares the lengths if the lengths of two words match
    if(length1 == length2) {
        //loop through every letter and compare the two letters at the same index, if they do not match, return 0 (false) meaning the words do not match
        for (int i = 0; i < length1 - 2; i ++) {
            if ((word1.data[i] != word2[i])){
                return 0;
            }
        }
    }
        //if the lengths do not match, return 0 (false) meaning the words do not match
    else {
        return 0;
    }

    //if a integer has not been returned yet, return 1 (true) meaning the words match
    return 1;
}


//a method that reads the words to be redacted from the file into the
char **readRedact() {
    //initialise and 2D character array to hold words to be redacted 'redactables'
    char **redactables;
    int numberOfRedactables = 3;
    int maxRedactableLength = 1;

    //allocate initial memory of 2D array
    redactables = malloc(numberOfRedactables * sizeof(char*));
    for (int i = 0; i < numberOfRedactables; i++){
        redactables[i] = malloc((maxRedactableLength + 1) * sizeof(char));
    }

    //create the file object
    FILE *file;
    file = fopen("redact.txt", "r");

    //if the file has been opened successfully run through each character
    if(file != NULL) {
        //initialise variable to hold character currently being read
        char temp;
        int currentWordLength = 0;

        //loop through all characters in the file until the end of file character
        while ((temp = (char) getc(file)) != EOF) {

            //if the character signals the end of a word, increase the number of redactables and resize the redactables array
            if ((temp == '\n') || temp == ' '){
                numberOfRedactables ++;
                redactables = realloc(redactables, numberOfRedactables * sizeof(char*));
                redactables[numberOfRedactables - 1] = malloc((maxRedactableLength + 1) * sizeof(char));

                currentWordLength = 0;
            }
                //otherwise resize the word length in the redactables array if necessary and add the character to the array
            else{
                currentWordLength ++;

                if (currentWordLength > maxRedactableLength){
                    maxRedactableLength = currentWordLength;

                    for (int i = 0; i < numberOfRedactables; i++){
                        redactables[i] = realloc(redactables[i], (maxRedactableLength + 1) * sizeof(char));
                    }
                }

                redactables[numberOfRedactables-1][currentWordLength-1] = temp;
            }
        }
        fclose(file);
    }
    else{
        printf("Redact file could not be found.");
    }



    //add the metadata to the redactables array so that it can be displayed correctly
    redactables[0][0] = (char) numberOfRedactables;
    redactables[1][0] = (char) maxRedactableLength;

    return redactables;
}


//the method that is called when the program is run
int main( ) {
    //initialises char array of arrays and calls method to read words to be redacted
    char **redactables = readRedact();

    //reads an element from the 2D array that represents the number of words to be redacted
    int numberOfRedactions = (int) redactables[0][0];

    //reads an element from the 2D array that represents the maximum length of a word from the words to be redacted
    int maximumLengthOfRedaction = (int) redactables[1][0];

    //prints the words that will be redacted from the text
    printf("Words to redact:\n");
    printRedactables(redactables, numberOfRedactions, maximumLengthOfRedaction);

    //prints the text with the specified words redacted
    printf("\nRedacted debate:\n");
    readDebate(redactables, numberOfRedactions);

    return 0;
}