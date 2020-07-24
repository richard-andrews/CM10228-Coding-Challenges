public class CW2Q8 {
    public static void main(String[] args){
        //the number of days in each month of a common year respectively
        int[] monthLengths = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

        //the current day, month and year in that cycle of the loops
        int day, month, year;

        //an integer representing the day of the week, Monday = 1, Tuesday = 2, Wednesday = 3 etc
        int weekDay = 1;

        //the cumulative number of Tuesdays that occur on the first of a month since 01/01/1901 to that date
        int firstTuesdays = 0;

        //cycles through the years beginning at 1900 using the given data
        for (year = 1900; year <= 2000; year++){
            //cycles through the 12 months in a year
            for (month = 1; month <= 12; month++){

                //checks if it is a leap year (divisible by 4 and not divisible by 100 or divisible by 400) and it is February.
                if((month == 2) && (divisibleBy(year, 4) && (!divisibleBy(year, 100) || divisibleBy(year, 400)))){
                    //cycles through the days in the February of a leap year
                    for ( day = 1; day <= 29; day++) {

                        //call method to check if there is a first Tuesday
                        firstTuesdays = countFirstTuesdays(day, month, year, weekDay, firstTuesdays);

                        //call method to increase and loop the weekday
                        weekDay = weekDayLoop(weekDay);
                    }

                }
                //if it is not a leap year, use the common year array.
                else{
                    //loop through the days in that month
                    for (day = 1; day <= monthLengths[month-1]; day++) {

                        //call method to check if there is a first Tuesday
                        firstTuesdays = countFirstTuesdays(day, month, year, weekDay, firstTuesdays);

                        //call method to increase and loop the weekday
                        weekDay = weekDayLoop(weekDay);
                    }
                }
            }
        }

        System.out.print("There were " + firstTuesdays + " Tuesdays on the 1st of a month between 01/01/1901 and 31/12/2000");
    }

    private static int countFirstTuesdays(int day, int month, int year, int weekDay, int firstTuesdays) {
        //checks if counting in the 20th Century
        if (year != 1900) {
            //checks if it is the first day of the month and the second weekday (Tuesday)
            if ((day == 1) && (weekDay == 2)) {

                //increase counter and print date
                firstTuesdays ++;
                System.out.println(firstTuesdays + " - " + day + "/" + month + "/" + year);
            }
        }
        //return the cumulative number of Tuesdays
        return firstTuesdays;
    }

    private static int weekDayLoop(int weekDay) {
        //if it is a 'Sunday' the next day is Monday so loop to the first weekday
        if (weekDay == 7){
            return 1;
        }
        //otherwise return the next weekday
        else{
            return (weekDay+1);
        }
    }


    //a method that reimplements the modulo operation and determines if a number is wholy divisible by another number
    private static boolean divisibleBy (int input, int divisor){
        int multiples = input / divisor;
        int remainder = input - (multiples * divisor);

        if (remainder == 0){
            return true;
        }
        else {
            return false;
        }
    }
}
