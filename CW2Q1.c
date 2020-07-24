#include <stdio.h>
#include <time.h>


//a method that completes the middle square method to create random numbers from a given seed replacing existing PRNG library functions
int middleSquare(int seed) {
    int square = seed * seed;

    //retrieves the middle numbers
    seed = ((square)/100) % 10000;

    return seed;
}


//a method to print the array passed to it
void printArray(int cocktail[100]){
    printf("(");
    for (int i = 0; i<100;i++){
        printf("%i ", cocktail[i]);
    }

    printf(")\n\n");
}


//a method that sorts the array passed to it according to the "cocktail sort" method
void cocktailShake(int cocktail[100]){
    int swapped;
    int first, second;

    do{
        //assumes a swap will not occur
        swapped = 0;

        //runs through all elements in the array to be sorted from index 0 to index 99.
        for (int i = 0; i < 99; i++){

            //tests if two adjacent elements are in the wrong order. If they are in the wrong order, they are swapped.
            if (cocktail[i] > cocktail[i+1]) {
                first = cocktail[i];
                second = cocktail[i + 1];

                cocktail[i] = second;
                cocktail[i + 1] = first;

                //the swapped variable is changed to the TRUE value to show that a swap has occurred.
                swapped = 1;
            }
        }

        //checks if any swaps were made forwards, if not then the algorithm is complete and the array is sorted.
        if (swapped == 0){
            break;
        }

        //resets the swapped variable to determine if a swap is made going back through the array
        swapped = 0;

        //runs through all elements in the array to be sorted from index 99 to index 0.
        for (int i = 99 ; i > 0; i--){
            if (cocktail[i] < cocktail[i-1]){
                first = cocktail[i];
                second = cocktail[i - 1];
                cocktail[i] = second;
                cocktail[i - 1] = first;

                swapped = 1;
            }
        }
        
        //calls the print array method to print the final sorted array
        printArray(cocktail);

    } while (swapped == 1);
}

//the initial method that is called when the program is run
int main() {
    //initialises an array of 100 integers
    int cocktail[100];

    //sets the initial seed to be the last 4 digits of the time since Epoch
    int seed = (int) ((time(NULL)) % 10000);

    //fills the array with pseudo random numbers
    for (int i = 0; i < 100; i ++){
        seed = middleSquare(seed);
        cocktail[i] = seed;
    }

    //prints the unsorted array
    printArray(cocktail);

    //calls the sort method to sort and print the array
    cocktailShake(cocktail);

    return 0;
}
