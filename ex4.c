/******************
Name:tomer michels
ID:322440892
Assignment:4
*******************/
#include <stdio.h>
#define MAX_DIMM 20
#define CHAR_SET 127
#define pl_one 1
#define zero 0
#define MAX_GRID 30
#define MAX_SLOTS 100
#define MAX_WORD_LENGTH 15
#define PYRAMID_ROWS 5
#define PYRAMID_COLUMNS 5
#define half_cheer 0.5

//case 1
int robot(int x, int y) {
    if (x == zero && y == zero) {//when we reach the destination
        return 1;
    }

    if (x < zero || y < zero) {//when we didnt reach the destination
        return 0;
    }

    return robot(x - pl_one, y) + robot(x, y - pl_one);//call the recurse with one time mooving dowm and one lrft
}

//case 2
//get weight from user
float getWeightInput() {
    float weight;
    scanf("%f", &weight);
    return weight;
}


float calculatePyramidWeight(int row, int col, float pyramidWeights[PYRAMID_ROWS][PYRAMID_COLUMNS]) {
    if (row < zero || col < zero || col > row)//check if position valid
        return 0;

    if (row == zero && col == zero)//for person at top
        return pyramidWeights[zero][zero];

    return pyramidWeights[row][col] +//calaculate total weight=each weight with half of each above with recursive
        half_cheer * calculatePyramidWeight(row - pl_one, col - pl_one, pyramidWeights) +
        half_cheer * calculatePyramidWeight(row - pl_one, col, pyramidWeights);
}

//case 3
char findcloser(char opener) {
    char closer = '\0'; // searching for the right closer
    if (opener == '(') {
        closer = ')';
    }
    if (opener == '[') {
        closer = ']';
    }
    if (opener == '{') {
        closer = '}';
    }
    if (opener == '<') {
        closer = '>';
    }
    return closer;
}

int recurse(char close) {
    char spec_ch;
    scanf("%c", &spec_ch); //reading the char

    if (spec_ch == '\n') {
        // Check if at the end of input
        return close == '\n';
    }

    if (spec_ch == '(' || spec_ch == '[' || spec_ch == '{' || spec_ch == '<') {
        // when we found an  opener
        char spec_closer = findcloser(spec_ch); // call func to get the closer
        if (!recurse(spec_closer)) { // call recurse with the neew closer
            return 0;
        }
    } else if (spec_ch == ')' || spec_ch == ']' || spec_ch == '}' || spec_ch == '>') {
        // when we found closer
        if (spec_ch != close) {
            return 0; // when there is ni match bet close and opener
        }
        return 1; // the match is good
    }

    //
    return recurse(close);//if we didnt fint the right closr
}

//case 4
// check if a queen can be placed at given position by checking all 8 surrounding squares
int checkValidArea(int boardSize, char board[][MAX_DIMM], char state[][MAX_DIMM], int charMap[], int row, int col) {//
    (void)board;
    (void)charMap;

    int isValid = pl_one;


  // if there is q in any of these position false
    if(row + pl_one < boardSize && state[row+pl_one][col] == 'X')
        isValid = 0;
    if(row - pl_one >= zero && state[row-pl_one][col] == 'X')
        isValid = 0;
    if(col + pl_one < boardSize && state[row][col+pl_one] == 'X')
        isValid = 0;
    if(col - pl_one >= zero && state[row][col-pl_one] == 'X')
        isValid = 0;
    if(row + pl_one < boardSize && col + pl_one < boardSize && state[row+pl_one][col+pl_one] == 'X')
        isValid = 0;
    if(row - pl_one >= zero && col + pl_one < boardSize && state[row-pl_one][col+pl_one] == 'X')
        isValid = 0;
    if(row - pl_one >= zero && col - pl_one >= zero && state[row-pl_one][col-pl_one] == 'X')
        isValid = 0;
    if(row + pl_one < boardSize && col - pl_one >= zero && state[row+pl_one][col-pl_one] == 'X')
        isValid = 0;

    return isValid;
}

// place a queen on the board at given position
void placeQueen(char board[][MAX_DIMM], char state[][MAX_DIMM], int charMap[], int row, int col) {
    state[row][col] = 'X';
    charMap[(unsigned char)board[row][col]] = pl_one;
}

//remove queen from board
void removeQueen(char board[][MAX_DIMM], char state[][MAX_DIMM], int charMap[], int row, int col) {
    state[row][col] = '*';
    charMap[(unsigned char)board[row][col]] = zero;
}


void displayState(int size, char state[][MAX_DIMM]) {// print current board
    printf("Solution:\n");
    for (int i = zero; i < size; i++) {
        for (int j = zero; j < size; j++) {
            printf("%c ", state[i][j]);
        }
        printf("\n");
    }
}

int validateRow(char board[][MAX_DIMM], char state[][MAX_DIMM], int charMap[], int row, int col, int idx) {//check for queens at culum
    if (idx < row) {
        if (state[idx][col] == 'X')
            return 0;
        return validateRow(board, state, charMap, row, col, idx + 1);
    }
    return 1;
}


int checkAvailability(int boardSize, char board[][MAX_DIMM], char state[][MAX_DIMM], int charMap[], int row, int col) {
    int isValid = pl_one;

    if (!validateRow(board, state, charMap, row, col, zero))
        isValid = 0;
    if (!checkValidArea(boardSize, board, state, charMap, row, col))
        isValid = 0;
    if (charMap[(unsigned char)board[row][col]] == pl_one)
        isValid = 0;

    return isValid;
}


int solveQueens(int boardSize, char board[][MAX_DIMM], char state[][MAX_DIMM], int charMap[], int row, int col) {//recursive func to solve puzzle
    if (col >= boardSize)
        return 0;

    if (row >= boardSize) {
        displayState(boardSize, state);
        return 1;
    }

    if (checkAvailability(boardSize, board, state, charMap, row, col)) {
        placeQueen(board, state, charMap, row, col);

        if (solveQueens(boardSize, board, state, charMap, row + pl_one, zero))
            return 1;

        removeQueen(board, state, charMap, row, col);
    }

    return solveQueens(boardSize, board, state, charMap, row, col + pl_one);
}

//case 5

typedef struct {
    int row;
    int col;
    int length;
    char direction;
} Slot;

char grid[MAX_GRID][MAX_GRID];
int grid_size;
Slot slots[MAX_SLOTS];
char dictionary[MAX_SLOTS][MAX_WORD_LENGTH + pl_one];
int num_slots;
int num_words;
int usedWords[MAX_SLOTS] = {zero};


void initializeGrid() {//fill grid with#
    for (int i = zero; i < grid_size; i++) {
        for (int j = zero; j < grid_size; j++) {
            grid[i][j] = '#';
        }
    }
}

// display the current state of the crossword grid
void printGrid() {
    for (int i = zero; i < grid_size; i++) {
        printf("| ");
        for (int j = zero; j < grid_size; j++) {
            printf("%c | ", grid[i][j]);
        }
        printf("\n");
    }
}


int can_fit(Slot slot, char word[MAX_WORD_LENGTH]) {//check if word can fit in place
    for (int i = zero; i < slot.length; i++) {
        int r = slot.row + (slot.direction == 'V' ? i : zero);
        int c = slot.col + (slot.direction == 'H' ? i : zero);

        // check if position is valid and either empty or matches the letter
        if (r >= grid_size || c >= grid_size || (grid[r][c] != '#' && grid[r][c] != word[i])) {
            return 0;
        }
    }
    return 1;
}

// place or remove a word from the grid
void place_word(Slot slot, char word[MAX_WORD_LENGTH], int is_placing) {
    for (int i = zero; i < slot.length; i++) {
        int r = slot.row + (slot.direction == 'V' ? i : zero);
        int c = slot.col + (slot.direction == 'H' ? i : zero);

        grid[r][c] = is_placing ? word[i] : '#';
    }
}

// calculate  length of  word
int word_length(char word[MAX_WORD_LENGTH]) {
    int length = zero;
    while (length < MAX_WORD_LENGTH && word[length] != '\0') {
        length++;
    }
    return length;
}

//recursive func to solve the crossword puzzle
int task5CrosswordGenerator(int slot_index) {

    if (slot_index == num_slots) {    // if slots filled, puzzle solve

        return 1;
    }

    Slot slot = slots[slot_index];

    // try each word from dictionary
    for (int i = zero; i < num_words; i++) {
        if (usedWords[i]) {
            continue;
        }

        // if word fits try placing it
        if (word_length(dictionary[i]) == slot.length && can_fit(slot, dictionary[i])) {
            place_word(slot, dictionary[i], pl_one);
            usedWords[i] = pl_one;

            // try  solve rest of  puzzle
            if (task5CrosswordGenerator(slot_index + pl_one)) {
                return 1;
            }

            // if not successful, remove word and try next one
            place_word(slot, dictionary[i], zero);
            usedWords[i] = zero;
        }
    }

    return 0;
}


int main()
{
    int task = -1;
    do
    {
        printf("Choose an option:\n"
               "1. Robot Paths\n"
               "2. The Human Pyramid\n"
               "3. Parenthesis Validation\n"
               "4. Queens Battle\n"
               "5. Crossword Generator\n"
               "6. Exit\n");

        if (scanf("%d", &task))
        {
            switch (task)
            {
            case 6:
                printf("Goodbye!\n");
                break;
            case 1:
                int x, y;
                printf("Please enter the coordinates of the robot (column, row):\n");
                scanf("%d %d", &x, &y);

                int allpaths = robot(x, y);
                printf("The total number of paths the robot can take to reach home is: %d\n", allpaths);


                break;
            case 2:
                float pyramidWeights[PYRAMID_ROWS][PYRAMID_COLUMNS] = {0};
                int hasNegativeWeight = 0;

                printf("Please enter the weights of the cheerleaders:\n");

                for (int level = zero; level < PYRAMID_ROWS; level++) {
                    for (int position = zero; position <= level; position++) {
                        pyramidWeights[level][position] = getWeightInput();
                        if (pyramidWeights[level][position] < zero) {
                            printf("Negative weights are not supported.\n");
                            hasNegativeWeight = pl_one;
                            break;
                        }
                    }
                    if (hasNegativeWeight) break;
                }

                if (!hasNegativeWeight) {
                    printf("The total weight on each cheerleader is:\n");
                    for (int level = zero; level < PYRAMID_ROWS; level++) {
                        for (int position = zero; position <= level; position++) {
                            printf("%.2f ", calculatePyramidWeight(level, position, pyramidWeights));
                        }
                        printf("\n");
                    }
                }
                break;
            case 3:
                printf("Please enter a term for validation:\n");
                char spec_ch;//be sure not ruin
                scanf("%c", &spec_ch);//be sure not ruin
                int result = recurse('\n');
                if (result == pl_one) {//if its true
                    printf("The parentheses are balanced correctly.\n");
                } else {
                    printf("The parentheses are not balanced correctly.\n");
                }
                break;
            case 4:
                int charMap[CHAR_SET] = {zero};
                char board[MAX_DIMM][MAX_DIMM];
                char state[MAX_DIMM][MAX_DIMM];
                int boardSize;

                printf("Please enter the board dimensions:\n");
                scanf("%d", &boardSize);

                printf("Please enter a %d*%d puzzle board:\n", boardSize, boardSize);
                for(int i = zero; i < boardSize; i++)
                    for(int j = zero; j < boardSize; j++)
                        scanf(" %c", &board[i][j]);

                for(int i = zero; i < boardSize; i++)
                    for(int j = zero; j < boardSize; j++)
                        state[i][j] = '*';

                if(!solveQueens(boardSize, board, state, charMap, zero, zero))
                    printf("This puzzle cannot be solved.\n");
                break;
            case 5:
                printf("Please enter the dimensions of the crossword grid:\n");
                scanf("%d", &grid_size);
                initializeGrid();

                printf("Please enter the number of slots in the crossword:\n");
                scanf("%d", &num_slots);

                printf("Please enter the details for each slot (Row, Column, Length, Direction):\n");
                for (int i = zero; i < num_slots; i++) {
                    scanf("%d %d %d %c", &slots[i].row, &slots[i].col, &slots[i].length, &slots[i].direction);
                }

                printf("Please enter the number of words in the dictionary:\n");
                do {
                    scanf("%d", &num_words);
                    if (num_words < num_slots) {
                        printf("The dictionary must contain at least %d words. Please enter a valid dictionary size:\n", num_slots);
                    }
                } while (num_words < num_slots);

                printf("Please enter the words for the dictionary:\n");
                for (int i = zero; i < num_words; i++) {
                    scanf("%s", dictionary[i]);
                }

                if (task5CrosswordGenerator(zero)) {
                    printGrid();
                } else {
                    printf("This crossword cannot be solved.\n");
                }


                break;
            default:
                printf("Please choose a task number from the list.\n");
                break;
            }
        }
        else
        {
            scanf("%*s");
        }

    } while (task != 6);
}


