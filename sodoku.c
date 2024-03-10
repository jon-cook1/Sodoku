/*****
 * Project 02: Grid game
 * COSC 208, Introduction to Computer Systems, Fall 2023
 *****/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>


//Scrapped Features (for time) auto candidate, ac add/del, file parsing, timer, quit command, pause/resume timer command
    //left code commented to be completed in the future (for fun)


//Holds the variables of the board, allowing for fewer parameters in functions
struct Game{
    //time_t start_time;
    char **board;
    char **immutable;  //n array of length 2 char array. Holds the coordinates for unremovable numbers (default on board)
    //int gamemode;
    int number; // # of filled in squares
    int immutable_count;
};

//free the struct and the teo 2d arrays inside it
void free_memory(struct Game *sudoku){
    for (int i=0;i<sudoku->immutable_count;i++)
        free(sudoku->immutable[i]);
    for (int i=0;i<9;i++)
        free(sudoku->board[i]);

    free(sudoku->immutable);
    free(sudoku->board);
    free(sudoku);
}

//print the list of user commands and their use
void print_help(){
    printf("\n*************************\nAvailable Commands:\n\nadd -> Add a number\ndel -> Remove a number\nprint -> Print the board\nac -> (DISABLED) Toggle auto candidate mode on/off\nhelp -> List this menu\nNote: All commands are case insensitive but require strict formatting\nPress Control + z to quit at any point\n\n*************************\n");
}

//helper for checking win, ensures no repeats in row
int has_unique_numbers(char *segment) {
        int seen[10] = {0}; // Index 0 is unused for simplicity
        for (int i = 0; i < 9; ++i) {
            if (segment[i] == ' ') { // Skip empty cells.
                continue;
            }
            int num = segment[i] - '0'; // Convert char to corresponding integer.
            if (num < 1 || num > 9 || seen[num]) { // Invalid number or duplicate.
                return 0;
            }
            seen[num] = 1; // Mark this number as seen.
        }
        return 1; // All nums unique
    }

//evaluate board to check for win. checks row, column, box for repeat nums
//must also be full (81) vals
int check_win(struct Game *maingame){
    if (maingame->number != 81)
        return 0;

    // Check all rows and columns.
    for (int i = 0; i < 9; ++i) {
        char row[9], column[9];
        for (int j = 0; j < 9; ++j) {
            row[j] = maingame->board[i][j];
            column[j] = maingame->board[j][i];
        }
        if (!has_unique_numbers(row) || !has_unique_numbers(column)) {
            return 0; // Duplicate found
        }
    }

    // Hardcoded indices for the 3x3 boxes.
    int box_indices[9][2] = {
        {0, 0}, {0, 3}, {0, 6},
        {3, 0}, {3, 3}, {3, 6},
        {6, 0}, {6, 3}, {6, 6}
    };

    // Check all 3x3 boxes. //Chat GPT assistance used for checking boxes
    for (int i = 0; i < 9; ++i) {
        char box[9];
        int start_row = box_indices[i][0];
        int start_col = box_indices[i][1];

        for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < 3; ++k) {
                box[j * 3 + k] = maingame->board[start_row + j][start_col + k];
            }
        }

        if (!has_unique_numbers(box)) {
            return 0; // Duplicate found in a 3x3 box.
        }
    }

    // If all checks pass, the board is correctly filled.
    return 1;
}

//give index (x,y) checks row,box,column to return list of all possible vals for that square
char *possible_vals(char x, char y, struct Game *maingame){
    char *possible = (char *)malloc(sizeof(char) * 10);  // 0 through 9
    memset(possible, 0, 10);  // Initialize array to zeros

    // Check column
    for (int i = 0; i < 9; i++){
        char val = maingame->board[x-'0'][i];
        //printf("LOOKINGAT: %c",val);
        if (val != ' '){
            //printf("PASS");
            if (val != ' ' && val >= '0' && val <= '9'){
                possible[val-'0'] = val; 
            }
        }
    }

    // Check row
    for (int i = 0; i < 9; i++){
        char val = maingame->board[i][y-'0'];
        //printf("LOOKINGAT2: %c",val);
        if (val != ' '){
            //printf("PASS");
            if (val != ' ' && val >= '0' && val <= '9'){
                possible[val-'0'] = val; 
            }
        }
    }

    // Check 3x3 box
    int xbox = ((x - '0')/3) * 3;
    int ybox = ((y - '0')/3) * 3;
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            char val = maingame->board[xbox+i][ybox+j];
            //printf("LOOKINGAT3: %c",val);
            if (val != ' '){
                //printf("PASS");
            }
            if (val != ' ' && val >= '0' && val <= '9'){
                possible[val-'0'] = val;  
            }
        }
    }
    //printf("\nPOSSIBLES: ");
    //for (int i=0;i<10;i++){
        //printf("%d: %c ",i,possible[i]);
    //}
    return possible;
}

//void print_acBoard(struct Game *maingame){} // print with auto candidate values

/*char *input_board(){
    for (int i=0;i<9;i++){
        printf("\nPlease enter row #%d.\nEnter the nine values as one line of text, with zeros in place of empty squares.\n",i+1); 

    }
}

void update_ac(char x, char y, char num, char **board){} //when adding/removing number update ac values in r,b,c

int add_ac(char x, char y, char number, struct Game *maingame){return 0;}
//add and sub ac are available whether or not ac mode is on
int del_ac(char x, char y, char number, struct Game *maingame){return 0;}

*/

//prints the board
void print_board(struct Game *maingame) {

    //for ac use 3 temp 3x3 arrays

  printf("\n\n     0   1   2   3   4   5   6   7   8\n");
  printf("    — — — — — — — — — — — — — — — — — —\n");
  for (int i = 0; i < 9; i++) {
    printf("%d  ", i); // Print the row number with an extra space
    for (int j = 0; j < 9; j++) {
      if (j % 3 == 0) {
        printf("|");
      } else {
        printf(" ");
      }
      if (maingame->board[i][j]) {
        printf(" %c ", maingame->board[j][i]);
      } else {
        printf("   ");
      }
    }
    printf("|\n");
    if ((i + 1) % 3 == 0 && i != 8) {
      printf("    — — — — — — — — — — — — — — — — — —\n");
    } else if (i != 8) {
      printf("   |           |           |           |\n");
    }
  }
  printf("    — — — — — — — — — — — — — — — — — —\n");
}

void print_rules(){
    printf("\nWelcome to Sudoku!\n\nRules:\n");
    printf("1. Each row must contain distinct integers from 1 to 9.\n");
    printf("2. Each column must contain distinct integers from 1 to 9.\n");
    printf("3. Each of the nine 3x3 sub-grids must contain distinct integers from 1 to 9.\n");
    printf("4. The objective of the game is to fill the board entirely with integers from 1 to 9, following these rules.\n");
    printf("5. Type 'Help' in order to see a list of user available commands.\n\n Press enter/return to begin...\n\n\n\n");
    getchar();
}

//creates string of 81 char to be turned into board
char *parse_board(){
    /*char *boardData = malloc(sizeof(char)*82); //9x9 = 81 char + null terminator for strlen
    char filename[] = {"boardata.csv"};

    FILE *file = fopen(filename, "r");  //stackoverflow assistance
    int inde = 9;
    int tempchar = 0;
    
    while ((tempchar = fgetc(file)) != EOF){
        if (tempchar != '\n'){
            boardData[inde] = tempchar;
            inde++; 
        }
    }
    return boardData;*/
    //This is getting scrapped for time purposes. Changing to hardcoded 7 boards
    
    // Each string should be exactly 81 characters long, followed by a null terminator
    
    static char strings[7][82] = {  //change to [8][82] for testing
        "96241537818576342937492856149615783221839674575328419682754961353167298464983125 ",     //uncomment for testing
        "  241     8576   937 9     4   5  3  1  96  5   2841    75    3   6       9  12 7",
        "    6   742 19 86 7 83  19   7685    1    9  8    9     2 51      4      349 67  ",
        "     34     687  5  12 4    3716    5 6  913 9  5  7  3  89  56 5    3      4   1",
        " 58 642    2598   7   3     8 2 64 32 6  98 1     1 2 1  4 7  8      3    4    9 ",
        "2 685    58    2 1   9 28  6     4  8 14    9 74 283      4    493185       9  34",
        "    1 9  83  9 2 7  9 7 613 57 6     962        7     9 8 4 56 7    84   15  6   ",
        "96 4 13   7 9  56    7   2 4 61       8 6        481 65  6 79        2 782    6 3"
    };

    srand(time(NULL));
    int randomIndex = rand() % 7; //comment for testing

    return strings[randomIndex]; //comment for testing
    //return strings[0]; //uncomment for testing
} 

//turn 81 char string into 9x9 board
void create_board(char* boardData, struct Game *maingame){
    //printf("board function caled\n");
    
    maingame->number = 0;
    for (int i=0;i<81;i++){
        if (boardData[i] != ' ')
            maingame->number++;
    }
    //printf("\nNumber: %d\n",maingame->number);
    maingame->immutable = malloc(sizeof(char*)*maingame->number);
        for (int i=0;i<maingame->number;i++)
            maingame->immutable[i] = malloc(sizeof(char)*2);

    maingame->board = malloc(sizeof(char*)*9);
    for (int i=0;i<9;i++)
        maingame->board[i] = malloc(sizeof(char)*10);

    int row = 0;
    int col = 0;
    int immut_count = 0;
    for(int i=0;i<strlen(boardData);i++){
        if (i%9 == 0 && i!=0){
            col=0;
            row++;
        }
        maingame->board[col][row] = boardData[i];
        if (boardData[i] != ' '){
            maingame->immutable[immut_count][0] = col + '0';
            maingame->immutable[immut_count][1] = row + '0';
            immut_count++;
        }
        col++;
    }
    maingame->immutable_count = immut_count;
    //printf("Done board function");
}

//adds a num to the board if given coords for an empty square
int add_num(char x, char y, char number, struct Game *maingame){
    //no overewrites to prevent mis types of coords by user
    //printf("\\nnTEST INDEX: %d  %d\n\n",x-'0',y-'0');
    char *existing_val = &maingame->board[x-'0'][y-'0'];
    //printf("\nExisting Val: '%c'\n\n",*existing_val);
    if (*existing_val != ' '){  
        printf("\nThere is already an existing number there (%c), please remove it first\n",*existing_val);
        return 0;
    }
    int repeat = 0;
    char *vals = possible_vals(x,y,maingame);
    printf("\n");
    for (int i=0;i<10;i++){
        //printf("\nTAKEN CHAR: %c",vals[i]);
        if (vals[i] == number)
            repeat=1;
    }
    printf("\n");
    free(vals);
    if (repeat){
        printf("\nThere is already a %c in that row/column/box\n",number);
        return 0;
    }
    *existing_val = number;
    maingame->number++;
    return 1;
}

//removes number from the board at given coords. Can only remove player inputted numbers
int del_num(char x, char y, struct Game *maingame){ //return∈{0,1}  automatically interpreted as false,true-> TO BE USED FOR DETERMING WHETHER TO PRINT THE BOARD BEFORE REPROMPT!!!
    //printf("\n\nImmutable Count: %d\n\n",maingame->immutable_count);
    
    for (int i = 0; i<maingame->immutable_count; i++){
        //printf("\n\n#%d IMMUT0CHAR: %c",i,maingame->immutable[i][0]);
        //printf("IMMUT1CHAR: %c\n\n",maingame->immutable[i][0]);

        if (maingame->immutable[i][0] == x && maingame->immutable[i][1] == y){
            printf("\nThat number can't be deleted\n");
            return 0;
        }
    }   
    maingame->board[x-'0'][y-'0'] = ' ';
    maingame->number--;
    return 1;
}

//make sure player input matches accepted inputs in target arr
int validate_input(char* input, char** target, int targetLen){   //return∈{0,1}  automatically interpreted as false,true
    //target is if a specific word is desired. Passed as array containing all allowable inputs
    // !!! target values are hard coded. *****ALL HAVE TO BE LOWERCASE*****
    if (input==NULL)
        return 0;

    if (target == NULL || (strlen(input) == 1 && input[0] == 'Q')) {
        return 1;
    }
    // Convert the input string to lowercase
    for (int i = 0; input[i]; i++) {
        if (isalpha((unsigned char)input[i])) { // Ensure the character is alphabetic
            input[i] = tolower((unsigned char)input[i]);
        }
    }
    // Compare the input with each string in the target array
    for (int i = 0; i < targetLen; i++) {
        int comp  = strcmp(input, target[i]);
        
        if (!comp) {
            return 1; // Input matches one of the target strings
        }
    }
    return 0; // No match found, invalid
}

//gets an input from the player then validates it
char* get_input(char **options, int optionslen) {
    int valid = 0;
    char *input = malloc(sizeof(char) * 10);
    int first = 1;

    while (!valid) {
        if (!first) {
            printf("\nInvalid Input");
            if (options){ 
                printf("\nOptions: [");
                for (int i = 0; i < optionslen; i++) { 
                    printf("%s", options[i]);
                    if (i != optionslen - 1) {
                        printf(", "); 
                    }
                }
                printf("]\n\nEnter Command:\n> "); 
            }
        }
        first = 0;
        fgets(input, 10, stdin);

        size_t length = strlen(input); //from chatGPT on removing new line char
        if (length > 0 && input[length - 1] == '\n') {
            input[length - 1] = '\0';
        }
        valid = validate_input(input, options, optionslen);
    }
    return input;
}

//get and validate user input of coords/number to be added there 
int add_helper(struct Game *maingame){
    printf("\nEnter coordinates and number: [Three characters- format: XY*number*]\n> ");
    int attempt = 0;
    int valid = 0;
    int first = 1;
    char *input;

    while (attempt < 2 && !valid){
        if (!first)
            printf("\nInvalid Input- One Attempt Remaining\n");

        input = get_input(NULL, 0); // Assuming get_input allocates memory

        if (strlen(input) == 3){
            // Check if the first two characters are between '0' and '8'
            if ((input[0] >= '0' && input[0] <= '8') && (input[1] >= '0' && input[1] <= '8')){
                // Check if the third character is between '1' and '9'
                if (input[2] >= '1' && input[2] <= '9')
                    valid = 1;
            }
        }
        attempt++;
        first = 0;

        if (!valid) {
            free(input); // Prevent memory leak if input is invalid
        }
    }

    if (valid){
        int success;
        // Convert characters to their integer values
        success = add_num(input[0], input[1], input[2], maingame);
        free(input);
        return success;
    }
    return 0;
    //printf("\nDone with helper\n");
}

//get and validate coords for number to be deleted
int del_helper(struct Game *maingame){
    printf("\nEnter coordinates and number: [Two characters- format: XY]\n> ");
    int attempt = 0;
    int valid = 0;
    int first = 1;
    char *input;

    while (attempt < 2 && !valid){
        if (!first)
            printf("\nInvalid Input- One Attempt Remaining\n");

        input = get_input(NULL, 0); // Assuming get_input allocates memory

        if (strlen(input) == 2){
            // Check if the first two characters are between '0' and '8'
            if ((input[0] >= '0' && input[0] <= '8') && (input[1] >= '0' && input[1] <= '8'))
                valid = 1;
            
        }
        attempt++;
        first = 0;

        if (!valid) {
            free(input); // Prevent memory leak if input is invalid
        }
    }

    if (valid){
        int success;
        // Convert characters to their integer values
        success = del_num(input[0], input[1], maingame);
        free(input);
        return success;
    }
    return 0;
    //printf("\nDone with helper\n");
}

//conditional branch to call functions for their respective user command
int delegate_input(char *input, struct Game *maingame){
    int valid = 0;
    //if (*input == 'q' ){
        //printf("\nThank you for playing !\n");
        //free(input);
        //atexit(free_memory);
        //exit(EXIT_SUCCESS);
        //return; //unreachable, should help w/ valgrind though
    //}
    /*if (*input == 't'){
        //display time
    }*/
    if (*input == 'a'){
        if (*(input+1) == 'd'){
            valid = add_helper(maingame);
        }
        //else
            //maingame->gamemode = !(maingame->gamemode);   //ac toggle
    }
    else if (*input == 'p'){
        if (*(input+1) == 'r'){
            print_board(maingame);
        }
        /*else{
            //placeholder for pause function
        }*/
    }
    else if (*input == 'h'){
        print_help();
    }
    else if (*input == 'd'){
        valid = del_helper(maingame);
    }
    free(input);
    return valid;
}

//calls input and delegates it until player wins 
    int finished = 0;
    char *options[] = {"add","del","print","help"};
    int print = 1;
    
    while(!finished){
        if (print)
            print_board(maingame);
        print = 1;
        printf("\nEnter Command: \n> ");
        char *input = get_input(options,4);
        print = delegate_input(input,maingame);
        if (check_win(maingame))
            finished++;
    }
    printf("*****\n Congratulations, you win!\n*****");
}

//create main struct, call play_game loop, and free memory. Then prompts user to play again
void init_game(){
    char *input;
    char again = 'y';
    while (again == 'y'){
        struct Game *sudoku = malloc(sizeof(struct Game));
        //sudoku->gamemode = 0;
        create_board(parse_board(),sudoku);
        play_game(sudoku);
        free_memory(sudoku);

        char *options[] = {"yes", "no"};    //no malloc needed as it's a static allocation- not done during runtime
        printf("\nWould you like to play again? [Yes, No]\n");
        input = get_input(options,2);
        again = *input;
        free(input);
    }
    printf("\nThanks for playing!\n");
}

int main(){
    print_rules();
    init_game();
}