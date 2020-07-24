import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.net.URL;
import java.util.ArrayList;

public class CW2Q6{
    //initialise the array list that will hold the words to be redacted
    private static ArrayList<String> wordsToRedact;
    private static String prevLineLastWord;
    private static boolean sentenceOpen;

    //a method that reads the given redactions from the file
    private static String readRedactionsFromFile(){
        //create the file object
        URL path = CW2Q6.class.getResource("redact.txt");
        File textFile = new File(path.getFile());

        //initialise and declare a string that holds all of the redactions
        String redactionsAsString = "";

        //initialise a string that will hold the line that has just been read in
        String line;

        //read all the lines in from the redact file and add them to a string separated by a space
        try {
            BufferedReader br = new BufferedReader(new FileReader(textFile));
            while((line = br.readLine()) != null){
                redactionsAsString += (line + " ");
            }
            br.close();
        }
        catch (Exception ignored){
            System.out.println("Redact file could not be read.");
            System.exit(0);
        }

        //returns all the redactions in one string ending with a $ to signify the end of the string
        return (redactionsAsString + "$");
    }

    //a method that reimplements the split function to split the words in a string by and non letter character
    private static ArrayList<String> separateRedactions(String line) {
        int i = 0;
        int wordStart = 0;
        int wordEnd;
        ArrayList<String> words = new ArrayList<>();

        //loops through all characters in the passed string until the end
        while (line.charAt(i) != '$'){

            //if the character is not a letter, retrieve the word that has just been passed over
            if (!isLetter(line.charAt(i))){
                wordEnd = i;

                words.add(subStr(line, wordStart, wordEnd));

                wordStart = i + 1;
            }

            i ++;
        }
        return words;
    }


    //a method that reimplements the substring function and retrieves a set amount of character within a string
    private static String subStr(String superStr, int start, int end){
        String result = "";

        //loops through the given super string between an upper and lower bound, adding the character to the string
        for (int i = start; i < end; i ++){
            result += superStr.charAt(i);
        }

        return result;
    }


    //a method that reads the war and peace file and calls methods to redact proper nouns
    private static void readBookFile(){
        //creates the file object
        URL path = CW2Q6.class.getResource("warandpeace.txt");
        File textFile = new File(path.getFile());

        //initialise the string buffer for the line that will be read in
        String line;
        ArrayList<String> lineAsList;

        prevLineLastWord = " ";
        sentenceOpen = true;

        try{
            BufferedReader br = new BufferedReader(new FileReader(textFile));

            //reads the file line by line until the end
            while ((line = br.readLine()) != null){
                //calls a method that separates the string into words
                lineAsList = separateWordsInLine(line);

                //calls a method that retracts the proper nouns
                lineAsList = redactProperNouns(lineAsList);

                //calls a method that prints the line with the proper nouns retracted
                printArray(lineAsList);
            }
        }
        catch (Exception e){
            System.out.println("War and Peace could not be read.");
        }
    }


    //a method that redacts the proper nouns in a string passed to it
    private static ArrayList<String> redactProperNouns(ArrayList<String> lineAsList) {
        //initialise variables
        boolean match;
        String currentWord, prevWord, replacement;

        //loops through every word in the array list
        for (int i = 1; i <lineAsList.size(); i++){
            match = false;
            replacement = "";
            currentWord = lineAsList.get(i);
            prevWord = lineAsList.get(i-1);

            //checks if the word contains punctuation that means it closes the sentence
            if (isEndOfSentence(prevWord)){
                sentenceOpen = false;
            }

            //checks if the first letter of the word is capitalised and no others
            if (initialOnlyCapitalised(currentWord)){

                //compares against all the words to redact already found, if it matches, the word is redacted
                for (String redactable : wordsToRedact) {
                    if (currentWord.equals(redactable)) {
                        match = true;
                        break;
                    }
                }

                //if a match has not yet been found, the sentence has not been 'closed' by the previous word and the word is not a common word that is not a proper noun then it is a proper noun and is added to the list of proper nouns to redact so that it is picked up next time
                if ((!match) && (sentenceOpen) && !(isCommonWord(currentWord))){
                        wordsToRedact.add(currentWord);
                        match = true;
                }

                if (isLetter(currentWord.charAt(0))){
                    sentenceOpen = true;
                }

            }

            //if it is a match generates a string of * of the same length and replaces the word
            if (match){
                for (int j = 0; j < currentWord.length(); j++){
                    replacement += "*";
                }
                lineAsList.set(i, replacement);
            }
        }

        return lineAsList;
    }


    //a method that checks that the word passed to it is not a common word that is not a proper noun
    private static boolean isCommonWord(String currentWord) {
        boolean result = false;
        switch (currentWord){
            case "And":
            case "Soon":
            case "All":
            case "The":
            case "Don":
            case "Well":
            case "Having":
            case "Now":
            case "That":
            case "Who":
            case "While":
            case "How":
            case "Why":
            case "This":
            case "With":
            case "It":
            case "So":
            case "Is":
            case "We":
            case "Will":
            case "What":
            case "If":
            case "Both":
            case "Just":
            case "Before":
            case "In":
            case "Did":
            case "As":
            case "Only":
            case "To":
            case "For":
            case "But":
            case "From":
                result = true;
                break;
        }
        return result;
    }

    //a method that determines if the previous word contains punctuation meaning that it is a new sentence
    private static boolean isEndOfSentence(String prevWord) {
        return (prevWord.contains(":")) || (prevWord.contains(".")) || (prevWord.contains("!")) || (prevWord.contains("?") || (prevWord.contains(")")));
    }


    //a method that checks if only the first letter in a word is capitalised and no others
    private static boolean initialOnlyCapitalised(String currentWord) {
        boolean result = false;
        char first, second;
        if (currentWord.length() >= 2){
            first  = currentWord.charAt(0);
            second = currentWord.charAt(1);

            if(charIsCapital(first) && !charIsCapital(second)){
                result = true;
            }
        }

        return result;
    }


    //a method that checks if a character is an uppercase alphabetic letter
    private static boolean charIsCapital(char letter) {
        return ((letter >= 65) && (letter <= 90));
    }


    //a method that reimplements the split function by separating a string into elements in an array list
    private static ArrayList<String> separateWordsInLine(String line) {
        //initialise the array list for the words in the string ot be put into
        ArrayList<String> wordList = new ArrayList<>();
        boolean lookingAtWord = false;
        int wordStart = 0;
        int wordEnd;

        //adds the word from the previous line to
        wordList.add(prevLineLastWord);

        //loops through every character in the string and separates the words
        int i;
        for (i = 0; i < line.length(); i++){
            //character is a letter and it was not previously a letter, add the previous characters between the start and end bounds to the array list as a word of non letter characters
            if (isLetter(line.charAt(i))){
                if (!lookingAtWord){
                    lookingAtWord = true;
                    wordEnd = i;
                    wordList.add(subStr(line, wordStart, wordEnd));
                    wordStart = i;
                }
            }
            //if the character is not a letter and it was preivously a letter, add the previous characters between the start and end bounds to the array list as a word of letter characters
            else{
                if (lookingAtWord){
                    lookingAtWord = false;
                    wordEnd = i;
                    wordList.add(subStr(line, wordStart, wordEnd));
                    wordStart = i;
                }
            }
        }
        wordEnd = i;
        wordList.add(subStr(line, wordStart, wordEnd));


        String lastWord = wordList.get(wordList.size()-1);
        if (lastWord.length() ==0){
            prevLineLastWord = prevLineLastWord;
        }
        else{
            prevLineLastWord = lastWord;
        }

        return wordList;
    }


    //a method reimplements the isAlphabetic method determining whether or not a character is a alphabetic letter
    private static boolean isLetter(char chr) {
        return ((chr >= 'A') && (chr <= 'Z')) || ((chr >= 'a') && (chr <= 'z'));
    }


    //a method that reimplements the print array list function printing each element in the array list
    private static void printArray(ArrayList<String> line){
        for(int i = 1; i < line.size(); i++) {
            System.out.print(line.get(i));

        }
        System.out.println();
    }


    public static void main (String[] args){
        //reads the redactions in from the file and separates the into elements in an array list
        wordsToRedact = separateRedactions(readRedactionsFromFile());

        //reads war and peace and makes the appropriate redactions
        readBookFile();
        System.out.print("\nThe following have been redacted:\n" + wordsToRedact);
    }

}
