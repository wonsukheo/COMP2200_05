#include "pomoku.h"

static char s_play_board[20][20];   
static size_t s_black_score;
static size_t s_white_score;
static size_t s_row_length;
static size_t s_col_length;
const size_t START_LENGTH = 15;

void init_game(void)
{
    size_t i;
    size_t j;
    s_black_score = 0;
    s_white_score = 0; 
    s_row_length = sizeof(s_play_board) / sizeof(s_play_board[0]);
    s_col_length = s_row_length / sizeof(s_play_board[0][0]);

    /* '0' invalid board, '1' valid board */
    for (i = 0; i < s_row_length; i++) {
        for (j = 0; j < s_col_length; j++) {
            if (i > 14 && j > 14) {
                s_play_board[i][j] = '0';
            } else {
                s_play_board[i][j] = '1';
            }
        }
    }
    s_row_length = START_LENGTH;
    s_col_length = START_LENGTH;
}

size_t get_row_count(void)
{
    return s_row_length;
}

size_t get_column_count(void)
{
    return s_col_length;
}

int get_score(const color_t color)
{
    if (color == COLOR_BLACK) {
        return s_black_score;
    } else if (color == COLOR_WHITE) {
        return s_white_score;
    } else {
        return -1;
    }
}

int get_color(const size_t row, const size_t col)
{
    char color_placed = s_play_board[row][col];
    
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
    char color_placed;

    if (row >= s_row_length || col >= s_col_length) {
        return FALSE;
    }
    
    color_placed = s_play_board[row][col];
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
        s_play_board[row][col] = 'B';
    } else if (color == COLOR_WHITE) {
        s_play_board[row][col] = 'W';
    } else {
        return FALSE;
    }
    
    update_score(color, row, col);
    return TRUE;    
}

void update_score(const color_t color, const size_t row, const size_t col)
{
    int i = 0;
    size_t same_color_count;
    char placed_color = color == COLOR_BLACK ? 'B' : 'W';

    /* update_score_row */
    {
        same_color_count = 1;

        for (i = (int)col - 1; i >= 0; i--) {
            if (s_play_board[row][i] == placed_color) {
                same_color_count++;
            } else { 
                break;
            }
        }
        for (i = (int)col + 1; i < (int)s_col_length; i++) {
            if (s_play_board[row][i] == placed_color) {
                same_color_count++;
            } else { 
                break;
            }
        }
        if (same_color_count >= 5) {
            placed_color == 'B' ? (s_black_score += same_color_count - 4) : (s_white_score += same_color_count - 4);
        }
    }
    /* update_score_col */ 
    {
        same_color_count = 1;

        for (i = (int)row - 1; i >= 0; i--) {
            if (s_play_board[i][col] == placed_color) {
                same_color_count++;
            } else { 
                break;
            }
        }
        for (i = (int)row + 1; i < (int)s_row_length; i++) {
            if (s_play_board[i][col] == placed_color) {
                same_color_count++;
            } else { 
                break;
            }
        }

        if (same_color_count >= 5) {
            placed_color == 'B' ? (s_black_score += same_color_count - 4) : (s_white_score += same_color_count - 4);
        }
    }
    /* update_score_top_left */
    {
        same_color_count = 1;
        i = 0;
        while ((int)(row - i) >= 0 && (int)(col - i) >= 0) {
            if (s_play_board[row - i][col - i] == placed_color) {
                same_color_count++;
            } else {
                break;
            }
            i++;
        }
        i = 0;
        while ((row + i) < s_row_length && (col + i) < s_col_length) {
            if (s_play_board[row + i][col + i] == placed_color) {
                same_color_count++;
            } else {
                break;
            }
            i++;
        }

        if (same_color_count >= 5) {
            placed_color == 'B' ? (s_black_score += same_color_count - 4) : (s_white_score += same_color_count - 4);
        }
    }
    /* update_score_top_right */ 
    {
        same_color_count = 0;
        i = 0;
        while ((int)(row - i) >= 0 && col + i < s_col_length) {
            if (s_play_board[row - i][col + i] == placed_color) {
                same_color_count++;
            } else {
                break;
            }
            i++;
        }
        i = 0;
        while (row + i < s_row_length && (int)(col - i) >= 0) {
            if (s_play_board[row + i][col - i] == placed_color) {
                same_color_count++;
            } else {
                break;
            }
            i++;
        }

        if (same_color_count >= 5) {
            placed_color == 'B' ? (s_black_score += same_color_count - 4) : (s_white_score += same_color_count - 4);
        }
    }
}

/*special moves */
int insert_row(const color_t color, const size_t row)
{
    int i;
    size_t j;
    
    if (s_row_length >= 20 || row > s_row_length) {
        return FALSE;
    }

    if (color == COLOR_BLACK) {
        if (s_black_score < 3) {
            return FALSE;
        } else {
            s_black_score -= 3;
        }
    } else {
        if (s_white_score < 3) {
            return FALSE;
        } else {
            s_white_score -= 3;
        }
    }
    for (i = (int)s_row_length; i >= (int)row; i--) {
        for (j = 0; j < s_col_length; j++) {
            if (i == row) {
                s_play_board[i][j] = '1';
            } else {
                s_play_board[i][j] = s_play_board[i - 1][j];
            }
        }
    }

    s_row_length++;    
    return TRUE;
}

int insert_column(const color_t color, const size_t col)
{
    size_t i;
    int j;
    
    if (s_col_length >= 20 || col > s_col_length) {
        return FALSE;
    }

    if (color == COLOR_BLACK) {
        if (s_black_score < 3) {
            return FALSE;
        } else {
            s_black_score -= 3;
        }
    } else {
        if (s_white_score < 3) {
            return FALSE;
        } else {
            s_white_score -= 3;
        }
    }
    for (i = 0; i < s_row_length; i++) {
        for (j = (int)s_col_length; j >= (int)col; j--) {
            if (j == col) {
                s_play_board[i][j] = '1';
            } else {
                s_play_board[i][j] = s_play_board[i][j - 1];
            }
        }
    }

    s_col_length++;    
    return TRUE;
}

int remove_row(const color_t color, const size_t row)
{
    size_t i;
    size_t j;
    
    if (s_row_length <= 10 || row >= s_row_length) {
        return FALSE;
    }

    if (color == COLOR_BLACK) {
        if (s_black_score < 3) {
            return FALSE;
        } else {
            s_black_score -= 3;
        }
    } else {
        if (s_white_score < 3) {
            return FALSE;
        } else {
            s_white_score -= 3;
        }
    }
    for (i = row; i < s_row_length; i++) {
        for (j = 0; j < s_col_length; j++) {
            if (i == s_row_length - 1) {
                s_play_board[i][j] = '0';
            } else {
                s_play_board[i][j] = s_play_board[i + 1][j];
            }
        }
    }

    s_row_length--;    
    return TRUE;    
}

int remove_column(const color_t color, const size_t col)
{
    size_t i;
    size_t j;
    
    if (s_col_length <= 10 || col >= s_col_length) {
        return FALSE;
    }

    if (color == COLOR_BLACK) {
        if (s_black_score < 3) {
            return FALSE;
        } else {
            s_black_score -= 3;
        }
    } else {
        if (s_white_score < 3) {
            return FALSE;
        } else {
            s_white_score -= 3;
        }
    }
    for (i = 0; i < s_row_length; i++) {
        for (j = col; j < s_col_length; j++) {
            if (j == s_col_length - 1) {
                s_play_board[i][j] = '0';
            } else {
                s_play_board[i][j] = s_play_board[i][j + 1];
            }
        }
    }

    s_col_length--;    
    return TRUE;
}

int swap_rows(const color_t color, const size_t row0, const size_t row1)
{
    size_t j;
    char temp_char;

    if (row0 >= s_row_length || row1 >= s_row_length) {
        return FALSE;
    }

    if (color == COLOR_BLACK) {
        if (s_black_score < 2) {
            return FALSE;
        } else {
            s_black_score -= 2;
        }
    } else {
        if (s_white_score < 2) {
            return FALSE;
        } else {
            s_white_score -= 2;
        }
    }

    for (j = 0; j < s_col_length; j++) {
        temp_char = s_play_board[row0][j];
        s_play_board[row0][j] = s_play_board[row1][j];
        s_play_board[row1][j] = temp_char;
    }

    return TRUE;        
}    

int swap_columns(const color_t color, const size_t col0, const size_t col1)
{
    size_t i;
    char temp_char;
    
    if (col0 >= s_col_length || col1 >= s_col_length) {
        return FALSE;
    }

    if (color == COLOR_BLACK) {
        if (s_black_score < 2) {
            return FALSE;
        } else {
            s_black_score -= 2;
        }
    } else {
        if (s_white_score < 2) {
            return FALSE;
        } else {
            s_white_score -= 2;
        }
    }

    for (i = 0; i < s_row_length; i++) {
        temp_char = s_play_board[i][col0];
        s_play_board[i][col0] = s_play_board[i][col1];
        s_play_board[i][col1] = temp_char;
    }

    return TRUE;        
}

int copy_row(const color_t color, const size_t src, const size_t dst)
{
    size_t j;

    if (src >= s_row_length || dst >= s_row_length) {
        return FALSE;
    }

    if (color == COLOR_BLACK) {
        if (s_black_score < 4) {
            return FALSE;
        } else {
            s_black_score -= 4;
        }
    } else {
        if (s_white_score < 4) {
            return FALSE;
        } else {
            s_white_score -= 4;
        }
    }

    for (j = 0; j < s_col_length; j++) {
        s_play_board[dst][j] = s_play_board[src][j];
    }

    return TRUE;            
}    

int copy_column(const color_t color, const size_t src, const size_t dst)
{
    size_t i;

    if (src >= s_col_length || dst >= s_col_length) {
        return FALSE;
    }

    if (color == COLOR_BLACK) {
        if (s_black_score < 4) {
            return FALSE;
        } else {
            s_black_score -= 4;
        }
    } else {
        if (s_white_score < 4) {
            return FALSE;
        } else {
            s_white_score -= 4;
        }
    }

    for (i = 0; i < s_row_length; i++) {
        s_play_board[i][dst] = s_play_board[i][src];
    }

    return TRUE;            
}    
