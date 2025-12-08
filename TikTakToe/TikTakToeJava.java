// Tik Tak Toe Java Edition
// Programmed by: Connor Spannuth
// Date: 12/11/2024

import java.util.*;
public class TikTakToeJava {
    private int length;
    private int width;
    private int[][] slots;
    private int markCount;
    private String difficulty;
    private boolean isWinner;

    public TikTakToeJava(String difficulty) {
        this.difficulty = difficulty;
        this.length = 3;
        this.width = 3;
        this.slots = new int[3][3];
        this.markCount = 0;
        isWinner = false;

        //0 == x, 1 == O, 9 == Empty
        for(int i = 0; i < length; i++) {
            for(int j = 0; j < width; j++) {
                this.slots[i][j] = 9;
            }
        }

    }

    public void printSlots() {
        for(int i = 0; i < this.length; i++) {
            System.out.print("\n" + i + " ");
            for(int j = 0; j < this.length; j++) {
                System.out.print("|");
                if(this.slots[i][j] == 0) System.out.print("X");
                else if(this.slots[i][j] == 1) System.out.print("O");
                else System.out.print(" ");

            }
            System.out.print("|");
        } 
        System.out.print("\n  -------\n   0 1 2");
    }

    private boolean isFull() {
        return markCount == 9;
    }

    private boolean canAdd(int row, int col) {
        if(row > 3 || col > 3 || row < 0 || col < 0) return false;
        if(this.slots[row][col] == 9) return true;
        else return false;
    }

    private boolean addMark(int mark, int row, int col) {
        if (this.isFull()) return false;
        else if (!this.canAdd(row, col)) return false;
        else {
            this.slots[row][col] = mark;
            markCount++;
            return true;
        }
    }

    private void playerMove(Scanner scan) {
            do {
            System.out.println("\nWhere would you like to go");
            int playerMark = scan.nextInt();
            int row = playerMark / 10;
            int col = playerMark % 10;
            if(this.canAdd(row, col)) {
                this.addMark(0, row, col);
                break;
            } else if (row > 2 || col > 2 || row < 0 || col < 0) {
                System.out.println("Error: Invalid inputs");
            } else {
                System.out.println("Error: Spot already filled");
            }
        } while (true);
    }

    private void robotMove(String robotSelect) {
        if(robotSelect.equals("e")) {
            do {
                int row1 = (int)(Math.random() * 3);
                int col1 = (int)(Math.random() * 3);
                if(this.canAdd(row1, col1)) {
                    this.addMark(1, row1, col1);
                    break;
                }
            } while (true);

        } else if(robotSelect.equals("m")) {
            if(this.win1Turn(1) != 9) {
                double row = this.win1Turn(1);
                double col = (row*10) % 10;
                this.addMark(1, (int)row, (int)col);
            } else if(this.win1Turn(0) != 9) {
                double row = this.win1Turn(0);
                double col = (row*10) % 10;
                this.addMark(1, (int)row, (int)col);
            } else {
                robotMove("e");
            }

        } else {
            if(this.canAdd(1, 1)) {
                this.addMark(1, 1, 1);
            }else if(this.win1Turn(1) != 9) {
                double row = this.win1Turn(1);
                double col = (row*10) % 10;
                this.addMark(1, (int)row, (int)col);
            } else if(this.win1Turn(0) != 9) {
                double row = this.win1Turn(0);
                double col = (row*10) % 10;
                this.addMark(1, (int)row, (int)col);
            } else if(this.win2Turn(1) != 9) {
                double row = this.win2Turn(0);
                double col = (row*10) % 10;
                this.addMark(1, (int)row, (int)col);
            } else {
                robotMove("e");
            }
        }
    }

    private boolean dWin() {
        if(this.slots[1][1] == 9) return false;
        return (this.slots[0][0] == this.slots[1][1] && this.slots[1][1] == this.slots[2][2]) || (this.slots[2][0] == this.slots[1][1] && this.slots[1][1] == this.slots[0][2]);
    }
    private boolean vWin() {
        for(int i = 0; i < 3; i++) {
            if(this.slots[i][0] != 9 && this.slots[i][0] == this.slots[i][1] && this.slots[i][1] == this.slots[i][2]) return true;
        }
        return false;
    }
    private boolean hWin() {
        for(int i = 0; i < 3; i++) {
            if(this.slots[0][i] != 9 && this.slots[0][i] == this.slots[1][i] && this.slots[1][i] == this.slots[2][i]) return true;
        }
        return false;
    }

    private boolean isWin() {
        return dWin() || vWin() || hWin();
    }

    public void runGame(Scanner scan) {
        do {
            playerMove(scan);
            if(isWin()) {
                System.out.println("\nCongratulations, you win!");
                isWinner = true;
                this.printSlots();
                break;
            }
            if(this.isFull()) break;
            robotMove(difficulty);
            if(isWin()) {
                System.out.println("\nSorry, you lose.");
                isWinner = true;
                this.printSlots();
                break;
            }
            this.printSlots();
        } while(!this.isFull());
        if(!isWinner) {
            System.out.println("\nLooks like its a draw!");
            this.printSlots();
        }
        
    }

    private double win1Turn(int mark) {
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {
                if(this.canAdd(i, j)) {
                    addMark(mark, i, j);
                    if(isWin()) {
                        removeMark(i, j);
                        double order = (i + (j / 10.0));
                        return order;
                    } else removeMark(i, j);
                }
            }
        }
        return 9;
    }

    private double win2Turn(int mark) {
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {
                if(this.canAdd(i, j)) {
                    this.addMark(mark, i, j);
                    if(this.win1Turn(mark) != 9) {
                        this.removeMark(i, j);
                        double order = (i + (j / 10.0));
                        return order;
                    } else this.removeMark(i, j);
                }
            }
        }
        return 9;
    }

    private void removeMark(int row, int col) {
        this.slots[row][col] = 9;
        markCount--;
    }


    public static void main(String[]args) {
        Scanner scan = new Scanner(System.in);
        System.out.println("Welcome to Tik Tak Toe! Type 'e/m/h' for easy, medium, or hard difficulty.");
        String difficulty = scan.nextLine();
        TikTakToeJava board = new TikTakToeJava(difficulty);
        board.printSlots();
        System.out.println("\n");
        board.runGame(scan);
    }


}
