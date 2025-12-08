/* 
 * BigInt.java
 * A recreation of the BigInteger class in java using arrays.
 * Programmed by: Connor Spannuth
 * Date: 10/1/2024
 */

public class BigInt  {
    // the maximum number of digits in a BigInt -- and thus the length
    // of the digits array
    private static final int MAX_SIZE = 20;
    
    // the array of digits for this BigInt object
    private int[] digits;
    
    // the number of significant digits in this BigInt object
    private int numSigDigits;

    /*
     * Default, no-argument constructor -- creates a BigInt that 
     * represents the number 0.
     */
    public BigInt() {
        digits = new int[MAX_SIZE];
        numSigDigits = 1;  // 0 has one sig digit
    }

    /* A constructor that takes in an argument for an int[] array
     *  Creates a big int from that array, and updates the statistics
     * Throws an error if any part of the array passed is invalid.
     */
    public BigInt(int[] arr) {
        if(arr.length > 0 && arr.length <= MAX_SIZE && isDigitValid(arr)) {
            numSigDigits = findNumSigDigits(arr);
            digits = new int[MAX_SIZE];
            int j = findLeadZeros(arr);
            for(int i = digits.length - numSigDigits; i < digits.length; i++) {
                digits[i] = arr[j];
                j++;
            }
        } else throw new IllegalArgumentException();
    }

    /* A constructor that takes in an argument for an integer
     *  Creates a big int from that integer, and updates the statistics
     * Throws an error if the integer is negative.
     */
    public BigInt(int n) {
        if(n < 0) {
            throw new IllegalArgumentException();
        } else if(n == 0) {
            numSigDigits = 1;
            digits = new int[MAX_SIZE];
        } 
        else {
            numSigDigits = findNumSigDigitsInt(n);
            digits = new int[MAX_SIZE];
            int unitsDigit = n % 10;
            for(int i = digits.length - 1; i > digits.length - numSigDigits - 1; i--) {
                unitsDigit = n % 10;
                n /= 10;
                digits[i] = unitsDigit;
            }
        }
    }

    /* Accessor method that returns the sigDigits variable in the object
     * Inputs: None, Return: int
     */
    public int getNumSigDigits() {
        return numSigDigits;
    }

    /* A method that overrides the toString to help print out the bigInt
     * Inputs: none, Return: String
     */
    public String toString() {
        String s  = "";
        for(int i = digits.length - numSigDigits; i < digits.length; i++) {
            s += digits[i];
        }
        return s;
    } 

    /* A method that compares BigInt objects. It returns -1 if this < other, 
     * 0 if this == other, and 1 if this > other.
     * Inputs: BigInt other
     * Returns: int
     */ 
    public int compareTo(BigInt other) {
        if(other == null) {
            throw new IllegalArgumentException();
        } else if(this.numSigDigits > other.numSigDigits) {
            return 1;
        } else if(this.numSigDigits < other.numSigDigits) {
            return -1;
        } else {
            for(int i = 0; i < this.digits.length; i++) {
                if(this.digits[i] > other.digits[i]) {
                    return 1;
                } else if(this.digits[i] < other.digits[i]) {
                    return -1;
                }
            }
        }
        return 0;
    }

    /* A method that adds together two BigInt objects, and returns a newly created
     * BigInt object with the result saved to its internal digits array.
     * Input: BigInt other
     * Return: BigInt
     */

    public BigInt add(BigInt other) {
        if(other == null) throw new IllegalArgumentException();
        BigInt retObject = new BigInt();
        int carryBit = 0;
        for(int i = this.digits.length - 1; i >= 0; i--) {
            if((this.digits[i] + other.digits[i] + carryBit) < 10) {
                retObject.digits[i] = (this.digits[i] + other.digits[i] + carryBit);
                carryBit = 0;
            } else {
                retObject.digits[i] = ((this.digits[i] + other.digits[i] + carryBit) - 10);
                carryBit = 1;
            }
        }
        if(carryBit == 1) {
            throw new ArithmeticException();
        }
        retObject.numSigDigits = findNumSigDigits(retObject.digits);
        return retObject;
    }


    /*A method that multiplies together two BigInt objects, and returns a newly created
     * BigInt object with the result saved to its internal digits array.
     * Input: BigInt other
     * Return: BigInt
     */
    public BigInt mul(BigInt other) {
        if(other == null) throw new IllegalArgumentException();
        int result;
        int resultSum;
        int[] productArray = new int[MAX_SIZE*2];
        for(int i = this.digits.length - 1; i >= 0; i--) {
            for(int j = other.digits.length - 1; j >= 0; j--) {
                result = this.digits[i] * other.digits[j];
                resultSum = result + productArray[i+j+1];
                productArray[i+j+1] = resultSum % 10;
                productArray[i+j] += resultSum / 10;
            }
        }
        int[] finalArray = new int[findNumSigDigits(productArray)];
        if(finalArray.length > 20) throw new ArithmeticException();
        int j = productArray.length - 1;
        for(int i = finalArray.length - 1; i >= 0; i--) {
            finalArray[i] = productArray[j];
            j--;
        }
        BigInt retObject = new BigInt(finalArray);

        retObject.numSigDigits = findNumSigDigits(retObject.digits);
        return retObject;
    }


    /* Private helper method that validates if the array is valid. returns
     * false if invalid, and a compiler error if a non int value is passed in the array
     * Inputs: int[] Return: boolean
     */
    private static boolean isDigitValid(int[] passArr) {
        int[] arr = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        boolean isValid = false;
        for(int i = 0; i < passArr.length; i++) {
            isValid = false;
            for(int j = 0; j < arr.length; j++) {
                if(arr[j] == passArr[i]) {
                    isValid = true;
                    break;
                }
            }
            if(isValid == false) {
                return false;
            }
        }
        return isValid;
    }

    /* A private helper method that finds the number of sigDigits in an integer.
     * Input: Integer
     * Return: Integer
     */
    private static int findNumSigDigitsInt(int num) {
        int count = 0;
        while(num > 0) {
            num /= 10;
            num = (int)num;
            count++;
        }
        return count;
    }

    /* private helper method to find the number of sig digits in an array
     * inputs: int[]
     * return: int
     */
    private static int findNumSigDigits(int[] arr) {
        boolean leadZero = true;
        int count = 0;
        for(int i = 0; i < arr.length; i++) {
            if(arr[i] != 0) {
                leadZero = false;
            }
            if(!leadZero) {
                count++;
            }
        }
        if(count == 0) {
            count = 1;
        }
        return count;
    }

    /* private helper method that finds the amount of leading zeros before the number begins
     * inputs: int[]
     * return: int
     */
    private static int findLeadZeros(int[] arr) {
        int count = 0;
        for(int i = 0; i < arr.length; i++) {
            if(arr[i] == 0) {
                count++;
            } else {
                return count;
            }
        }
        if(count == 20) {
            count--;
        }
        return count;
    }
    
    public static void main(String[] args) {
        System.out.println("Unit tests for the BigInt class.");
        System.out.println();
	
        System.out.println("Test 1: result should be 7");
        int[] a1 = { 1,2,3,4,5,6,7 };
        BigInt b1 = new BigInt(a1);
        System.out.println(b1.getNumSigDigits());
        System.out.println();
        
        System.out.println("Test 2: result should be 1234567");
        b1 = new BigInt(a1);
        System.out.println(b1);
        System.out.println();
        
        System.out.println("Test 3: result should be 0");
        int[] a2 = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
        BigInt b2 = new BigInt(a2);
        System.out.println(b2);
        System.out.println();
        
        System.out.println("Test 4: should throw an IllegalArgumentException");
        try {
            int[] a3 = { 0,0,0,0,23,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
            BigInt b3 = new BigInt(a3);
            System.out.println("Test failed.");
        } catch (IllegalArgumentException e) {
            System.out.println("Test passed.");
        } catch (Exception e) {
            System.out.println("Test failed: threw wrong type of exception.");
        }
        System.out.println();
        
        System.out.println("Test 5: result should be \n1234567");
        b1 = new BigInt(1234567);
        System.out.println(b1);
        System.out.println();
        
        System.out.println("Test 6: result should be 0");
        b2 = new BigInt(0);
        System.out.println(b2);
        System.out.println();
        
        System.out.println("Test 7: should throw an IllegalArgumentException");
        try {
            BigInt b3 = new BigInt(-4);
            System.out.println("Test failed.");
        } catch (IllegalArgumentException e) {
            System.out.println("Test passed.");
        } catch (Exception e) {
            System.out.println("Test failed: threw wrong type of exception.");
        }
        System.out.println();
        
        System.out.println("Test 8: result should be 0");
        b1 = new BigInt(12375);
        b2 = new BigInt(12375);
        System.out.println(b1.compareTo(b2));
        System.out.println();

        System.out.println("Test 9: result should be -1");
        b2 = new BigInt(12378);
        System.out.println(b1.compareTo(b2));
        System.out.println();

        System.out.println("Test 10: result should be 1");
        System.out.println(b2.compareTo(b1));
        System.out.println();

        System.out.println("Test 11: result should be 0");
        b1 = new BigInt(0);
        b2 = new BigInt(0);
        System.out.println(b1.compareTo(b2));
        System.out.println();
        
        System.out.println("Test 12: result should be\n123456789123456789");
        int[] a4 = { 3,6,1,8,2,7,3,6,0,3,6,1,8,2,7,3,6 };
        int[] a5 = { 8,7,2,7,4,0,5,3,0,8,7,2,7,4,0,5,3 };
        BigInt b4 = new BigInt(a4);
        BigInt b5 = new BigInt(a5);
        BigInt sum = b4.add(b5);
        System.out.println(sum);
        System.out.println();

        System.out.println("Test 13: result should be\n123456789123456789");
        System.out.println(b5.add(b4));
        System.out.println();

        System.out.println("Test 14: result should be\n3141592653598");
        b1 = new BigInt(0);
        int[] a6 = { 3,1,4,1,5,9,2,6,5,3,5,9,8 };
        b2 = new BigInt(a6);
        System.out.println(b1.add(b2));
        System.out.println();

        System.out.println("Test 15: result should be\n10000000000000000000");
        int[] a19 = { 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9 };    // 19 nines!
        b1 = new BigInt(a19);
        b2 = new BigInt(1);
        System.out.println(b1.add(b2));
        System.out.println();

        System.out.println("Test 16: result should be Arithmetic Error");
        try {
            int[] a20 = { 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9 };    // 20 nines!
            b1 = new BigInt(a20);
            b2 = new BigInt(1);
            System.out.println(b1.add(b2));
            System.out.println("Test failed.");
        } catch (ArithmeticException e) {
            System.out.println("Test passed.");
        } catch (Exception e) {
            System.out.println("Test failed: threw wrong type of exception.");
        }
        System.out.println();

        System.out.println("Test 17: result should print\n255553");
        BigInt val1 = new BigInt(11111);
        BigInt val2 = new BigInt(23);
        BigInt product = val1.mul(val2);
        System.out.println(product);
        System.out.println();

        System.out.println("Test 18: result should print\n118215");
        val1 = new BigInt(555);
        val2 = new BigInt(213);
        product = val1.mul(val2);
        System.out.println(product);
        
    }
}