/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 14:57:42 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/06/15 10:05:32 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H
#include "../lib/raylib/include/raylib.h"
#include "../lib_vector/include/struct.h"

#define DARK_RED "\x1b[48;5;1m\x1b[30m"
#define DARK_REDF "\x1b[38;5;1m"
#define LIGHT_RED "\x1b[48;5;216m\x1b[30m"
#define DARK_GREENF "\x1b[38;2;0;128;0m"
#define LIGHT_BLUE "\x1b[48;5;39m\x1b[30m"
#define YELLOW_F "\x1b[38;2;255;215;0m"
#define YBLUE_F "\x1b[38;2;218;112;214m"
#define w_WHITE "\x1b[48;5;15m\x1b[30m"
#define DARK_BLUE "\x1b[48;5;18m\x1b[30m"
#define RESET_COLOR "\033[0m"

#define TABLE_SIZE 150000
#define ASCII_ORDERED_SIZE 150000
#define ORDERED_WORD_LIST_PATH "data/Data.txt"

#define C_WHITE (Color){ 200, 200, 200, 255 }
#define EMPTY_CELL_GRAY (Color) {.r = 210, .g = 210, .b = 210, .a = 255}

enum {
	DWORD = 1,
	DLETTER = 2,
	TWORD = 3,
	TLETTER = 4,
	SCRABBLE = 50,
};

enum {
	VERTICAL = 1,
	HORIZONTAL = 2,
	BOTH = 4,
};

enum {
	STARTS_THERE,
	ENDS_THERE,
};

enum {
	NORTH,
	SOUTH,
	EAST,
	WEST,
};

typedef struct MatchVector {
	struct Match *data;
	size_t size;
	size_t capacity;
}	MatchVector;

typedef struct FindMatch {
	char word[15];
	int start;
	int end;
	int dir;
	int score;
	int save_coord;
} FindMatch;

typedef struct AsciiOrderedTable {
	char *key;
	Vector value_vect;
	struct AsciiOrderedTable *next;
} AsciiOrderedTable;

typedef struct Point {
	int x;
	int y;
} Point;

typedef struct Neighbor {
	int	  dir[4];
	bool valid;
} Neighbor;

typedef struct word {
	char		*word;
	int			score;
	struct word	*next;
} word;

typedef struct TextVar {
	int text_size;
	int font_size;
	char *text;
} TextVar;

typedef struct Grid {
	int			grid[15][15];
    int         copy[15][15];
	int			tour_grid[15][15];
	int			modifier[15][15];
	Rectangle	grid_rect;
} Grid;

typedef struct list {
	char **content;
	int size;
	int capacity;
} list;

typedef struct Ruler {
	int value[7];
	Rectangle rect[7];
	Rectangle base_rect[7];
	bool	dragging[7];
	int		modifier[7];
	bool	access;
	struct Point	cell[7];
} Ruler;

typedef struct Purse {
	int purse[26];
	Vector purse_vect;
} Purse;

typedef struct Score {
	int total_score;
	Vector prev_scores;
}Score;

typedef struct Match {
	char	word[15];
	int	start;
	int	end;
	int	dir;
	int	save_coord;
	int score;
	bool  validated;
	Rectangle match_rect;
} Match;

typedef struct TourManager {
	bool tourIsDone;
	bool canValidate;
	int  tour_score;
	int  highest_x;
	int  highest_y;
	Rectangle validate_rect;
	Match word_list[30];
} TourManager;

typedef struct GameData {
	struct word **		hashTable;
	struct AsciiOrderedTable	**asciiTable;
	struct Grid			grid;
	struct Purse		purse;
	struct Ruler		ruler;
	struct TourManager	tour;
	struct Score		score;
	struct Rectangle	ruler_rect;
	bool				isMainMenu;
	bool				shouldBeClosed;
	int					iterator;
} GameData;

#include "struct.h"

#endif
