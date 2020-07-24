#include <stdio.h>
#include <stdlib.h>

//the data structure that will hold the plain text once read from the file
struct table {
    //the characters in a 2D array list
    char **data;
    //the order in which the columns should be rearranged into
    int *order;
    //the number of rows in the data structure
    int rows;
    //the number of columns in the data structure
    int columns;
    //the total number of letters read into the data structure from the file
    int entries;
};

//declaring the methods used
void sort();
void resize();
void printTable(char **table);
void readFile();
void padEnd();
void encrypt();


//declaring the instance of the data structure
struct table plain;


//declaring the 2D character array that will hold the encrypted text
char **cipher;


//a method to encrypt the plain text into cipher text by rearranging the columns
void encrypt() {
    //calls a method to order the key word
    sort();

    //allocates memory to the 2D character array that will hold the cipher text using the number of rows and columns in the plain text structure
    cipher = malloc(plain.rows * sizeof(char*));
    for (int i = 0; i <= plain.rows; i++){
        cipher[i] = malloc(plain.columns * sizeof(char));
    }

    //copies the characters from the plain text structure into the cipher text array in the order determined by the ordered key
    for (int i = 0; i < plain.rows; i ++){
        for (int j = 0; j < plain.columns; j ++){
            //allocates the characters in each row and column using ordered key word
            cipher[i][j] = plain.data[i][plain.order[j]];
        }
    }

}

//a method that pads the end of the last row to match the length of the other rows
void padEnd() {
    //loops through the length of the row
    for (int i = 0; i < plain.columns; i ++){
        //checks if any of the letters in the last line of the plain text structure are not capital letters
         if (!( (plain.data[plain.rows - 1][i] >= 'A') && (plain.data[plain.rows - 1][i] <= 'Z') )){
             //if they are not capital letters, the character from the key word in the same column is copied into this position
             plain.data[plain.rows - 1][i] = plain.data[0][i];
         }
     }
}

//a method that reads the characters into the plain text data structure from the file
void readFile() {
    //adds the key word to the first line of the data structure and updates the number of letters now in the data structure
    plain.data[0] = "LOVELACE";
    plain.entries = 8;

    //creates a file object of the file to be read
    FILE *file;
    file = fopen("text.txt", "r");

    //declares a character to hold the character that is currently being looked at by the program
    char tempChar;

    //declares the variables to position being written to in the data structure
    int currentRow, currentColumn;

    //checks that file is found and opened correctly
    if(file != NULL) {
        //loops through every character in the file until the end of the file
        while ((tempChar = (char) getc(file)) != EOF) {
            //converts any lower case letters to uppercase characters by shifting the ASCII code
            if ((tempChar >= 'a') && (tempChar <= 'z')){
                tempChar -= 32;
            }

            //if the character is capital alphabetic letter, the current position in the data structure is updated
            if ((tempChar >= 'A') && (tempChar <= 'Z')){
                //the current row is equal to the number of times 8, the length of a row will go into the number of entries
                currentRow = plain.entries / 8;
                //the current column is equal to the remainder of the above division
                currentColumn = plain.entries % 8;

                //if the current row being written to is equal to the number of rows in the plain text data structure, the structure is resized to increase the number of rows
                if (currentRow == plain.rows){
                    //calls a method to resize the data structure
                    resize();
                }

                //assigns the current letter to the current position in the data structure
                plain.data[currentRow][currentColumn] = tempChar;
                //updates the number of entries in the data structure
                plain.entries ++;
            }
        }

        //once the whole file has been read, a method is called to pad the last row of the structure
        padEnd();
    }
    //if the file is not opened or cannot be found, print out to the user
    else {
        printf("File not found. ");
    }
}

//a method that sorts an array of integers corresponding to first row of the plain text data structure (the key) into alphabetic order
void sort(){
    //allocates memory for the array of integers corresponding to the order in which the columns should be arranged
    plain.order = malloc(plain.columns * sizeof(int));

    //assigns initial values to the array, being the index of the characters of the key word as they are
    for (int i = 0; i < plain.columns; i++){
        plain.order[i] = i;
    }

    //a bubble sort to reorder the integers that correspond to the position of the characters in the key word
    //loop through the characters in the key word
    for (int i = 0; i < plain.columns - 1; i ++){
        //loop through the characters, comparing the current character against the next
        for (int j = 0; j < plain.columns - i - 1; j ++){

            //if the character in the position determined by teh array of indexes is greater than the next character then swap the indexes
            if (plain.data[0][plain.order[j]] > plain.data[0][plain.order[j + 1]]){
                int temp = plain.order[j+1];
                plain.order[j+1] = plain.order[j];
                plain.order[j] = temp;
            }
        }
    }
}

//a method to print the table, 2D array of characters as a body of text
void printTable(char **table) {
    //loop through each row in the data structure
    for (int i = 1; i < plain.rows; i++){
        //loop through each character in the row of the data structure
        for (int j = 0; j < plain.columns; j++){
            printf("%c", table[i][j]);
        }
    }
}

//a method that will increase the number of rows in the plain text 2D array of characters by one
void resize(){
    //declare a pointer the memory containing the structure
    char **copy = plain.data;

    //allocate memory for the new structure, which is one row more than the current
    plain.data = malloc((plain.rows + 1) * sizeof(char*));

    //allocate memory for each row of the new structure
    for (int i = 0; i < (plain.rows + 1); i++){
        plain.data [i] = malloc(plain.columns * sizeof(char));
    }

    //copy the characters in the previous structure to the new structure
    for (int i = 0; i < plain.rows; i++){
        for (int j = 0; j < plain.columns; j++){
            plain.data[i][j] = copy[i][j];
        }
    }

    //increase the number of rows in the current structure to match that of the size of the new structure
    plain.rows ++;
}


//the main method that is called on running the program
int main() {
    //setting the initial size of the plain text data structure
    plain.rows = 2;
    //since the key "LOVELACE" is 8 characters long, the data structure must have 8 columns
    plain.columns = 8;

    //allocating memory for the plain text data structure according to the initial size
    plain.data = malloc(plain.rows * sizeof(char*));
    for (int i = 0; i <= plain.rows; i++){
        plain.data[i] = malloc(plain.columns * sizeof(char));
    }

    //calls a method to read the data in from the file
    readFile();

    //prints the data as read in from the file, in the column structure
    printf("Plain text:\n");
    printTable(plain.data);

    //calls a method to encrypt the plain text as read in from the file using the key word.
    encrypt();

    //prints the encrypted data in the column structure
    printf("\nCipher text:\n");
    printTable(cipher);

    return 0;
}