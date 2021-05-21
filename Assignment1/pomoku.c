#include "pomoku.h"

char play_board[20][20];   
size_t black_score;
size_t white_score;
size_t row_length;
size_t col_length;
const size_t START_LENGTH = 15;

void init_game(void)
{
    size_t i;
    size_t j;
    black_score = 0;
    white_score = 0; 
    row_length = sizeof(play_board) / sizeof(play_board[0]);
    col_length = row_length / sizeof(play_board[0][0]);

    /* '0' invalid board, '1' valid board */
    for (i = 0; i < row_length; i++) {
        for (j = 0; j < col_length; j++) {
            if (i > 14 && j > 14) {
                play_board[i][j] = '0';
            } else {
                play_board[i][j] = '1';
            }
        }
    }
    row_length = START_LENGTH;
    col_length = START_LENGTH;
}

size_t get_row_count(void)
{
    return row_length;
}

size_t get_column_count(void)
{
    return col_length;
}

int get_score(const color_t color)
{
    if (color == COLOR_BLACK) {
        return (int)black_score;
    } else if (color == COLOR_WHITE) {
        return (int)white_score;
    } else {
        return -1;
    }
}

int get_color(const size_t row, const size_t col)
{
    char color_placed = play_board[row][col];
    
    if (color_placed == 'B') {
        return 0;
    } else if (color_placed == 'W') {
        return 1;
    } else {
        return -1;
    }
}

int is_placeable(const size_t row, const size_t col)
{
    char color_placed = play_board[row][col];
  
    if (row >= row_length || col >= col_length) {
        return FALSE;
    }

    if (color_placed == 'B' || color_placed == 'W') {
        return FALSE;
    } else {
        return TRUE;
    }
}

int place_stone(const color_t color, const size_t row, const size_t col)
{
    if (!is_placeable(row, col)) {
        return FALSE;
    }

    if (color == COLOR_BLACK) {
        play_board[row][col] = 'B';
    } else if (color == COLOR_WHITE) {
        play_board[row][col] = 'W';
    } else {
        return FALSE;
    }
    
    update_score(color, row, col);
    return TRUE;    
}

void update_score(const color_t color, const size_t row, const size_t col)
{
    int i;
    size_t same_color_count;
    char placed_color = color == COLOR_BLACK ? 'B' : 'W';

    /* update_score_row */
    {
        same_color_count = 1;

        for (i = col - 1; i >= 0; i--) {
            if (play_board[row][i] == placed_color) {
                same_color_count++;
            } else { 
                break;
            }
        }
        for (i = col + 1; i < (int)col_length; i++) {
            if (play_board[row][i] == placed_color) {
                same_color_count++;
            } else { 
                break;
            }
        }
        if (same_color_count >= 5) {
            placed_color == 'B' ? (black_score += same_color_count - 4) : (white_score += same_color_count - 4);
        }
    }
    /* update_score_col */ 
    {
        same_color_count = 1;

        for (i = row - 1; i >= 0; i--) {
            if (play_board[i][col] == placed_color) {
                same_color_count++;
            } else { 
                break;
            }
        }
        for (i = row + 1; i < (int)row_length; i++) {
            if (play_board[i][col] == placed_color) {
                same_color_count++;
            } else { 
                break;
            }
        }

        if (same_color_count >= 5) {
            placed_color == 'B' ? (black_score += same_color_count - 4) : (white_score += same_color_count - 4);
        }
    }
    /* update_score_top_left */
    {
        same_color_count = 1;
        while ((int)(row - i) >= 0 && (int)(col - i) >= 0) {
            if (play_board[row - i][col - i] == placed_color) {
                same_color_count++;
            } else {
                break;
            }
            i++;
        }

        if (same_color_count >= 5) {
            placed_color == 'B' ? (black_score += same_color_count - 4) : (white_score += same_color_count - 4);
        }
    }
    /* update_score_top_right */ 
    {
        same_color_count = 0;
        while (row + i <= row_length && col + i <= col_length) {
            if (play_board[row + i][col + i] == placed_color) {
                same_color_count++;
            } else {
                break;
            }
            i++;
        }

        if (same_color_count >= 5) {
            placed_color == 'B' ? (black_score += same_color_count - 4) : (white_score += same_color_count - 4);
        }
    }
}

/*special moves */
int insert_row(const color_t color, const size_t row)
{
    size_t i;
    size_t j;
    
    if (row_length >= 20 || row > row_length) {
        return FALSE;
    }

    if (color == COLOR_BLACK) {
        if (black_score < 3) {
            return FALSE;
        } else {
            black_score -= 3;
        }
    } else {
        if (white_score < 3) {
            return FALSE;
        } else {
            white_score -= 3;
        }
    }
    for (i = row_length; i >= row; i--) {
        for (j = 0; j < col_length; j++) {
            if (i == row) {
                play_board[i][j] = 0;
            } else {
                play_board[i][j] = play_board[i - 1][j];
            }
        }
    }

    row_length++;    
    return TRUE;
}

int insert_column(const color_t color, const size_t col)
{
    size_t i;
    size_t j;
    
    if (col_length >= 20 || col > col_length) {
        return FALSE;
    }

    if (color == COLOR_BLACK) {
        if (black_score < 3) {
            return FALSE;
        } else {
            black_score -= 3;
        }
    } else {
        if (white_score < 3) {
            return FALSE;
        } else {
            white_score -= 3;
        }
    }
    for (i = 0; i < row_length; i++) {
        for (j = col_length; j >= col; j--) {
            if (j == col) {
                play_board[i][j] = 0;
            } else {
                play_board[i][j] = play_board[i][j - 1];
            }
        }
    }

    col_length++;    
    return TRUE;
}

int remove_row(const color_t color, const size_t row)
{
    size_t i;
    size_t j;
    
    if (row_length <= 10 || row >= row_length) {
        return FALSE;
    }

    if (color == COLOR_BLACK) {
        if (black_score < 3) {
            return FALSE;
        } else {
            black_score -= 3;
        }
    } else {
        if (white_score < 3) {
            return FALSE;
        } else {
            white_score -= 3;
        }
    }
    for (i = row; i < row_length; i++) {
        for (j = 0; j < col_length; j++) {
            if (i == row_length - 1) {
                play_board[i][j] = 0;
            } else {
                play_board[i][j] = play_board[i + 1][j];
            }
        }
    }

    row_length--;    
    return TRUE;    
}

int remove_column(const color_t color, const size_t col)
{
    size_t i;
    size_t j;
    
    if (col_length <= 10 || col >= col_length) {
        return FALSE;
    }

    if (color == COLOR_BLACK) {
        if (black_score < 3) {
            return FALSE;
        } else {
            black_score -= 3;
        }
    } else {
        if (white_score < 3) {
            return FALSE;
        } else {
            white_score -= 3;
        }
    }
    for (i = 0; i < row_length; i++) {
        for (j = col; j < col_length; j++) {
            if (j == col_length - 1) {
                play_board[i][j] = 0;
            } else {
                play_board[i][j] = play_board[i][j + 1];
            }
        }
    }

    col_length--;    
    return TRUE;
}

int swap_rows(const color_t color, const size_t row0, const size_t row1)
{
    size_t j;
    char temp_char;

    if (row0 >= row_length || row1 >= row_length) {
        return FALSE;
    }

    if (color == COLOR_BLACK) {
        if (black_score < 2) {
            return FALSE;
        } else {
            black_score -= 2;
        }
    } else {
        if (white_score < 2) {
            return FALSE;
        } else {
            white_score -= 2;
        }
    }

    for (j = 0; j < col_length; j++) {
        temp_char = play_board[row0][j];
        play_board[row0][j] = play_board[row1][j];
        play_board[row1][j] = temp_char;
    }

    return TRUE;        
}    

int swap_columns(const color_t color, const size_t col0, const size_t col1)
{
    size_t i;
    char temp_char;
    
    if (col0 >= col_length || col1 >= col_length) {
        return FALSE;
    }

    if (color == COLOR_BLACK) {
        if (black_score < 2) {
            return FALSE;
        } else {
            black_score -= 2;
        }
    } else {
        if (white_score < 2) {
            return FALSE;
        } else {
            white_score -= 2;
        }
    }

    for (i = 0; i < row_length; i++) {
        temp_char = play_board[i][col0];
        play_board[i][col0] = play_board[i][col1];
        play_board[i][col1] = temp_char;
    }

    return TRUE;        
}

int copy_row(const color_t color, const size_t src, const size_t dst)
{
    size_t j;

    if (src >= row_length || dst >= row_length) {
        return FALSE;
    }

    if (color == COLOR_BLACK) {
        if (black_score < 4) {
            return FALSE;
        } else {
            black_score -= 4;
        }
    } else {
        if (white_score < 4) {
            return FALSE;
        } else {
            white_score -= 4;
        }
    }

    for (j = 0; j < col_length; j++) {
        play_board[dst][j] = play_board[src][j];
    }

    return TRUE;            
}    

int copy_column(const color_t color, const size_t src, const size_t dst)
{
    size_t i;

    if (src >= col_length || dst >= col_length) {
        return FALSE;
    }

    if (color == COLOR_BLACK) {
        if (black_score < 4) {
            return FALSE;
        } else {
            black_score -= 4;
        }
    } else {
        if (white_score < 4) {
            return FALSE;
        } else {
            white_score -= 4;
        }
    }

    for (i = 0; i < row_length; i++) {
        play_board[i][dst] = play_board[i][src];
    }

    return TRUE;            
}    
