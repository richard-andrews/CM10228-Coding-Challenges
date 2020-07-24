import java.io.*;
import java.net.URL;
import java.util.ArrayList;

public class CW2Q2 {
    //a method that reads the names from the file and returns them as an array list
    private static ArrayList<String> readFile(ArrayList<String> names) {
        //creates a file object of the desired file
        URL path = CW2Q2.class.getResource("names.txt");
        File textFile = new File(path.getFile());

        //initialises the string that will hold the input from the file
        String allNames = "";

        try {
            //reads the one line of the file into a string and appends it with an ! that will mark the end of the string
            BufferedReader br = new BufferedReader(new FileReader(textFile));
            allNames = br.readLine();
            allNames += "!";
        }
        catch (Exception e) {
            System.out.print("Names could not be read.");
            System.exit(0);
        }

        //index of characters through string
        int i = 0;
        //determines whether the characters currently being looked at part of a name
        boolean currentName = false;
        //holds the start and end index of the name currently being looked at
        int nameStart = 0;
        int nameEnd = 0;

        //while the final character marker has not been reached, add all names to the array list
        while (allNames.charAt(i) != '!'){

            //if the current character is " the current name variable is flipped
            if (allNames.charAt(i) == '"'){
                currentName = !currentName;

                //if characters following are part of a name, update the start index of the next name
                if (currentName){
                    nameStart = i+1;
                }
                //if the characters following are not part of name, update the end index and retrieve the name substring
                else{
                    nameEnd = i;
                    names.add(allNames.substring(nameStart,nameEnd));
                }
            }
            i++;
        }

        //returns the array list now full of names
        return names;
    }


    //a recursive method that creates increasingly small partitions to be sorted
    private static void quickSort(ArrayList<String> names, int low, int high){
        if (low < high){
            int part = partition(names, low, high);

            quickSort(names, low, part - 1);
            quickSort(names,part + 1, high);
        }
    }


    //a method that sorts and swaps names given a lower and upper bound
    private static int partition(ArrayList<String> names, int low, int high){
        String pivot = names.get(high);

        int i = low - 1;

        //compares name to the upper bound and swaps if the name being looked at is greater than the pivot
        for (int j = low; j <= high - 1; j ++){
            if ((compareNames(names.get(j), pivot))<0){
                i++;

                names = swapNames(names, i, j);
            }
        }

        names = swapNames(names,i+1,high);

        return i+1;
    }


    //a method that reimplements the comparison of two strings
    //returns 0 if the names are the same
    //returns a positive integer if name 1 is greater than name 2
    //returns a negative integer if name 2 is greater than name 1
    private static int compareNames(String name1, String name2){
        //determines the shortest name of the two passed to it
        int name1Length = nameLength(name1);
        int name2Length = nameLength(name2);
        int lengthMin = lengthMin(name1Length, name2Length);

        //for the number of characters in the shortest name, compare the characters at each index
        for (int i = 0; i < lengthMin; i ++){
            int name1Char = (int)name1.charAt(i);
            int name2Char = (int)name2.charAt(i);

            //if the characters are not equal, return the difference of the characters at that index
            if (name1Char != name2Char){
                return (name1Char - name2Char);
            }
        }

        //if the character of the two names up until the index of the shortest name are equal, return the difference in length
        if (name1Length != name2Length){
            return (name1Length - name2Length);
        }
        //otherwise they are equal or the same
        else {
            return 0;
        }
    }


    //a method to return the array list passed to it with the only difference being that two names have been swapped
    public static ArrayList<String> swapNames (ArrayList<String> names, int index1, int index2) {
        String first = names.get(index1);
        String second = names.get(index2);
        names.set(index1, second);
        names.set(index2, first);

        return names;
    }


    //reimplementation of string.length function to find the number of characters in a string
    private static int nameLength(String name) {
        int count = 0;

        //increases count for each character in the string
        for(char c: name.toCharArray()){
            count ++;
        }

        return count;
    }


    //reimplementation of the math.min function to find the minimum of two integers
    private static int lengthMin(int length1, int length2) {
        if (length1 > length2) {
            return length2;
        }
        //if length2 is greater or equal to length 1, length 2 is returned as it does not matter if they are equal
        else {
            return length1;
        }
    }


    //the initial method that is called when the program is run
    public static void main (String[] args) {
        //initialises ArrayList to hold the names
        ArrayList<String> names = new ArrayList<>();

        //calls method to read names from file into array list
        names = readFile(names);
        System.out.println(names);

        //calls method to sort the array list of names with the initial lower and upper bounds of the sort
        quickSort(names, 0, names.size() - 1);
        System.out.println(names);
    }
}
