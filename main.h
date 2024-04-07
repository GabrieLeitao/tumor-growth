#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h> // For usleep function

// Foreground Colors
#define ANSI_COLOR_RESET         "\x1b[0m"
#define ANSI_COLOR_RED           "\x1b[31m"
#define ANSI_COLOR_GREEN         "\x1b[32m"
#define ANSI_COLOR_YELLOW        "\x1b[33m"
#define ANSI_COLOR_BLUE          "\x1b[34m"
#define ANSI_COLOR_MAGENTA       "\x1b[35m"
#define ANSI_COLOR_CYAN          "\x1b[36m"
#define ANSI_COLOR_LIGHT_GRAY    "\x1b[37m"
#define ANSI_COLOR_DARK_GRAY     "\x1b[90m"
#define ANSI_COLOR_LIGHT_RED     "\x1b[91m"
#define ANSI_COLOR_LIGHT_GREEN   "\x1b[92m"
#define ANSI_COLOR_LIGHT_YELLOW  "\x1b[93m"
#define ANSI_COLOR_LIGHT_BLUE    "\x1b[94m"
#define ANSI_COLOR_LIGHT_MAGENTA "\x1b[95m"
#define ANSI_COLOR_LIGHT_CYAN    "\x1b[96m"
#define ANSI_COLOR_WHITE         "\x1b[97m"
// Background Colors
#define ANSI_BG_RED              "\x1b[41m"
#define ANSI_BG_GREEN            "\x1b[42m"
#define ANSI_BG_YELLOW           "\x1b[43m"
#define ANSI_BG_BLUE             "\x1b[44m"
#define ANSI_BG_MAGENTA          "\x1b[45m"
#define ANSI_BG_CYAN             "\x1b[46m"
#define ANSI_BG_LIGHT_GRAY       "\x1b[47m"
#define ANSI_BG_DARK_GRAY        "\x1b[100m"
#define ANSI_BG_LIGHT_RED        "\x1b[101m"
#define ANSI_BG_LIGHT_GREEN      "\x1b[102m"
#define ANSI_BG_LIGHT_YELLOW     "\x1b[103m"
#define ANSI_BG_LIGHT_BLUE       "\x1b[104m"
#define ANSI_BG_LIGHT_MAGENTA    "\x1b[105m"
#define ANSI_BG_LIGHT_CYAN       "\x1b[106m"
#define ANSI_BG_WHITE            "\x1b[107m"
// Other Attributes
#define ANSI_BOLD               "\x1b[1m"
#define ANSI_UNDERLINE          "\x1b[4m"
#define ANSI_INVERSE            "\x1b[7m"
#define ANSI_RESET_ALL          "\x1b[0m"

#define GRID_SIZE 40

// Perlin Noise
#define OCTAVES 4
#define PERSISTENCE 0.5
#define FREQUENCY 0.1

#define VEIN_THRESHOLD 0.2

#define SIMULATION_DELAY 500000 // Delay between simulation steps in microseconds

#define HEALTHY 'o'
#define TUMOR '#'
#define NECROTIC '.'
#define VEIN '+'

#define X 0
#define Y 1

typedef enum {
    TISSUE_CELL,
    WHITE_CELL,
    RED_CELL,
    VEIN_CELL
} CELL_TYPE;

typedef struct tissuecell
{
    int state;
    float growth_rate;
} TissueCell;

typedef struct whitecell
{
    int num_phagossomes;
} WhiteCell;

typedef struct erythrocyte
{
    float hemoglobin;
} Erythrocyte;

typedef struct veincell
{
    float hemoglobin;
} VeinCell;

typedef struct cell 
{
    int x, y, size;
    int age;
    float nutrient_level;
    float oxygen_level;
    float atp;
    CELL_TYPE type;
    union {
        TissueCell tissue_cell;
        WhiteCell white_cell;
        Erythrocyte red_cell;
        VeinCell vein_cell;
    } cell_data;
} CELL;

typedef struct vector
{
    float x, y;
} Vector2;

float lerp(float a0, float a1, float w);
float dotGridGradient(int ix, int iy, float x, float y, Vector2 gradients[][GRID_SIZE]);
float perlinNoise(float x, float y, Vector2 gradients[][GRID_SIZE]);

void seedInitialVeinPoints(CELL grid[][GRID_SIZE], int numPoints);
void growVeins(CELL grid[][GRID_SIZE], int x, int y, float noise);
Vector2 findCLosestVeinPoint(CELL grid[][GRID_SIZE], int x, int y);

void initializeTissue(CELL grid[][GRID_SIZE]);
void generateTissue(CELL grid[][GRID_SIZE]);
void displayGrid(CELL grid[][GRID_SIZE]);