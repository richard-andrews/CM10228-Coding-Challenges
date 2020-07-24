#include <stdio.h>
#include <stdlib.h>

//initialise used methods
void add(char *name);
int search(char *criteria);
void delete(char *name);
int hash(char *name);
void rehash(int newCapacity);
int getNameLength(const char *name);
void readNames();
void printTable();


//a data structure to hold the characters and length of a char array
struct name {
    char *data;
    int length;
};


//a data structure to hold the contents of the hash table as well as the maximum capacity, the number of entries in the table and the minimum length of a name
struct hashtable {
    char **table;
    int capacity;
    int used;
    int minNameLength;
};


//initialises the global variable of the hashtable data structure
struct hashtable names;


//a method to print the whole table
void printTable() {
    //loop through every element in array of strings
    for (int i = 0; i < names.capacity; i++){
        printf("%i ", i);
        //loop through every character in the strings
        for (int j = 0; j < getNameLength(names.table[i]); j++){
            printf("%c", names.table[i][j]);
        }
        printf(" \n");
    }
}

//a method to read the names from the text file
void readNames() {
    //create a new file object with the file path to be read only
    FILE *file;
    file = fopen("names.txt", "r");

    //assume the characters being looked at are not a name
    int lookingAtName = 0;

    //create the a string and character buffer to read into
    struct name tempName;
    tempName.length = 0;
    char tempChar;

    //if the file has been found and opened successfully begin reading it
    if(file != NULL) {
        //state that there are no entries in the hash table yet
        names.used = 0;

        //loop through characters in file until the end of the file
        while ((tempChar = (char) getc(file)) != EOF) {

            //check if the character is a speech mark, separating names
            if (tempChar == '\"'){
                //if the characters previously looked at were not part of a name, it is the beginning of a name
                if (lookingAtName == 0){
                    //allocate the memory for the string buffer
                    tempName.data = malloc(tempName.length * sizeof(char));

                    //state that the characters being looked at are now part of a name
                    lookingAtName = 1;

                    //increase the number of names in the hashtable
                    names.used ++;

                    //if the hashtable is a at 75% of capacity, double the capacity of the hashtable
                    if (names.used == (names.capacity * 0.75)){
                        rehash(names.capacity * 2);
                    }

                }
                    //otherwise, the speech marks signify the end of a name
                else{
                    //add the name in the string buffer to the hashtable
                    add(tempName.data);

                    //assert that the characters being looked at are no longer part of name
                    lookingAtName = 0;

                    //assert that the string buffer is empty and the free the memory
                    tempName.length = 0;
                    free(tempName.data);
                }
            }
            else{
                //if the characters are currently being looked at are part of a name, add them to the string buffer
                if (lookingAtName == 1){
                    tempName.length ++;
                    //reallocate the necessary memory for the string buffer
                    tempName.data = realloc(tempName.data, tempName.length * sizeof(char));
                    tempName.data[tempName.length - 1] = tempChar;
                }
            }
        }
    }
        //otherwise state that the file cannot be found or opened
    else {
        printf("Names.txt not found.");
    }
}

//a method that adds a string (character array) to the hashtable
void add(char *name) {
    //call a method to get the length of the string that has been passed
    int nameLength = getNameLength(name);

    //call a method to produce a key from the string that has been passed
    int key = hash(name);

    //probe the hashtable for the next available entry if the entry suggested by the hash is occupied
    while ((65 <= names.table[key][0]) && (names.table[key][0] <= 90)){
        //loop through the hashtable if the end is reached
        if (key == names.capacity -1){
            key = 0;
        }
        key ++;
    }

    //reallocate the necessary amount of memory to store the name in the hashtable
    names.table[key] = realloc(names.table[key], nameLength * sizeof(char));

    //store the characters of the name passed in the hashtable one by one
    for (int i = 0; i < nameLength; i++) {
        names.table[key][i] = name[i];
    }
}

//a method that reimplements library functions used to calculate the length of a passed string (character array)
int getNameLength(const char *name) {
    int i = 0;

    //pass through array while the character at index is a capital alphabetic letter
    while((65 <= name[i]) && (name[i] <= 90)){
        i++;
    }

    int nameLength = i ;

    //return the length of the string
    return nameLength;
}

//a method to create a hash value from a string using the polynomial rolling hash function
int hash(char *name) {
    //call a method to get the length of the string
    int nameLength = getNameLength(name);
    int key = 0;
    //assign an arbitrary value to produce the hash value
    int p = 42;
    //get the size of the array for the hash to go into
    int m = names.capacity;
    int pow = 1;

    //iteratively create the hash value according to the algorithm
    for (int i = 0; i < nameLength; i++){
        key = (key + (name[i] - 64) * pow) % m;
        pow = (pow * p) % m;
    }

    //return the produced hash value as the key
    return key;
}

//a method to resize and rehash the names from a smaller hashtable to a larger hashtable
void rehash(int newCapacity) {
    //assign a new pointer to point to the hashtable in memory
    char **copy = names.table;

    //allocate an area of memory according to the new capacity required of the hash table
    names.table = malloc(newCapacity * sizeof(char*));
    //allocate memory for each of the arrays at each entry in the hashtable
    for (int i = 0; i < newCapacity; i++){
        names.table[i] = malloc(names.minNameLength * sizeof(char));
    }

    //loop through old hashtable and add the names to the new hashtable
    for (int i = 0; i < names.capacity ; i++){
        //only add the entry if the first letter is a capital alphabetic letter (important for search and delete functions)
        if ((65 <= copy[i][0]) && (copy[i][0] <= 90)) {
            add(copy[i]);
        }
    }

    //change the capacity field of the hashtable object to match the new capacity
    names.capacity = newCapacity;
    //free the memory of the old, smaller, hash table to prevent memory leak
    free(copy);
}

//a method to search for a given name in the hashtable
int search(char *criteria){
    //hash the name using same algorithm as to add it to the table, this should suggest where the name should be in the table if no collisions have occurred
    int key = hash(criteria);

    //assume that the name has not been found
    int match = 0;

    //loop through the table while the characters being looked at are capital alphabetic letters, if it is not, exit the loop and return the result
    while ((65 <= names.table[key][0]) && (names.table[key][0] <= 90)){
        //assume that the string being looked at is not a match
        match = 0;

        //if the length of the entry and the length of the name being searched for match, check through each character
        if (getNameLength(names.table[key]) == getNameLength(criteria)){
            //assume it is a match
            match = 1;
            //check if each character matches, if one character does not match, assert a match has not been found and give up on that entry by breaking the for loop
            for (int i = 0; i < getNameLength(criteria); i++){
                if (criteria[i] != names.table[key][i]){
                    match = 0;
                    break;
                }
            }
        }

        //if a match has been found (the lengths match and all characters in both strings are the same, leave the while loop
        if (match == 1){
            break;
        }

        //loop through the entries in the table if the end of the table is reached, as with the add function
        if (key == names.capacity -1){
            key = 0;
        }
        key ++;
    }

    //if a match has been found return the index/key where the match was found
    if (match == 1){
        return key;
    }
        //otherwise return -1, this signifies that the string has not been found as it is not a valid index in the table
    else {
        return -1;
    }
}

//a method to remove an entry from the table
void delete(char *name){
    //call the search function to find the index of the name in the hashtable
    int key = search(name);

    //if the key is not negative and therefore has been found by the search function
    if (key != -1){
        //change the first character of the name to no longer be a capital alphabetic letter so it is no longer recognised by the rehashing function and is not added the table again
        names.table[key][0] = '/';
    }
    else{
        printf("\nName cannot be found and therefore cannot be deleted.\n");
    }

    //rehash the table to the same size so that the name is not copied to from the original table to the new table and therefore lost/deleted
    rehash(names.capacity);
}

int main() {
    //sets initial values for the length of a name and the maximum number of entries possible in the table
    names.minNameLength = 2;
    names.capacity = 1000;

    //allocate memory for the entries in the table
    names.table = malloc(names.capacity * sizeof(char*));

    //allocate memory for each entry/name in the table
    for (int i = 0; i < names.capacity; i++){
        names.table[i] = malloc(names.minNameLength * sizeof(char));
    }

    //call functions to read all the names into the table from the file and display the table
    readNames();
    printTable();

    //search for a name and either return it's position in the table or state it cannot be found.
    int result  = search("HARRY");
    if (result != -1){
        printf("The name \"HARRY\" is stored at %i", result);
    }
    else {
        printf("Name cannot be found");
    }

    //remove a name from the table and print the new table
    printf("\nDeleting \"HARRY\" from the table.\n");
    delete("HARRY");
    printTable();

    //search again for the deleted name and either print its position or state that it cannot be found
    result  = search("HARRY");
    if (result != -1){
        printf("The name \"HARRY\" is stored at %i", result);
    }
    else {
        printf("Name cannot be found");
    }

    return 0;
}