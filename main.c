#include "main.h"

int main() 
{
    srand(time(NULL));
    CELL grid[GRID_SIZE][GRID_SIZE]; // Grid to represent the terminal display

    int pos[2] = {0, 0};
    int direction[2] = {1, 1};

    initializeTissue(grid);
    seedInitialVeinPoints(grid, 25);
    generateTissue(grid);

    // Display the grid
    displayGrid(grid);

    return 0;
}

void initializeTissue(CELL grid[][GRID_SIZE])
{
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
           grid[i][j].type = TISSUE_CELL;
           grid[i][j].cell_data.tissue_cell.state = 0;
        }
    }
}

// Initialize grid with empty spaces
void generateTissue(CELL grid[][GRID_SIZE]) 
{
    // srand(42);
    Vector2 gradients[GRID_SIZE][GRID_SIZE];
    // int visited[GRID_SIZE][GRID_SIZE] = {0};
    float total, angle, frequency, amplitude, maxNoiseValue, noise;
    // Generate random gradients
    for (int x = 0; x < GRID_SIZE; x++)
    {
        for (int y = 0; y < GRID_SIZE; y++)
        {

            angle = ((float)rand() / RAND_MAX) * 2 * M_PI;
            gradients[x][y].x = cos(angle);
            gradients[x][y].y = sin(angle);

            total = 0;
            frequency = FREQUENCY;
            amplitude = 1;
            maxNoiseValue = 0;
            for (int i = 0; i < OCTAVES; i++)
            {
                total += perlinNoise(x * frequency, y * frequency, gradients) * amplitude;
                maxNoiseValue += amplitude;
                amplitude *= PERSISTENCE;
                frequency *= 2;
            }
            noise = total / maxNoiseValue;

            if (grid[x][y].type = VEIN) 
            {
                // Start vein growth
                growVeins(grid, x, y, noise);
            }
        }
    }
}

float lerp(float a0, float a1, float w)
{
    return (1.0 - w) * a0 + w * a1;
}

float dotGridGradient(int ix, int iy, float x, float y, Vector2 gradients[][GRID_SIZE])
{
    Vector2 gradient = gradients[ix][iy];
    float dx = x - (float)ix;
    float dy = y - (float)iy;
    return dx * gradient.x + dy * gradient.y;
}

float perlinNoise(float x, float y, Vector2 gradients[][GRID_SIZE])
{
    int x0 = (int)x;
    int x1 = x0 + 1;
    int y0 = (int)y;
    int y1 = y0 + 1;

    float sx = x - (float)x0;
    float sy = y - (float)y0;

    float n0, n1, ix0, ix1;

    n0 = dotGridGradient(x0, y0, x, y, gradients);
    n1 = dotGridGradient(x1, y0, x, y, gradients);
    ix0 = lerp(n0, n1, sx);

    n0 = dotGridGradient(x0, y1, x, y, gradients);
    n1 = dotGridGradient(x1, y1, x, y, gradients);
    ix1 = lerp(n0, n1, sx);

    return lerp(ix0, ix1, sy);
}

void growVeins(CELL grid[][GRID_SIZE], int x, int y, float noise)
{
    // Base case: if the current point is out of bounds or already visited
    if (x < 0 || x >= GRID_SIZE || y < 0 || y >= GRID_SIZE || noise > VEIN_THRESHOLD) {
        return;
    }

    if (grid[x][y].type == TISSUE_CELL)
    {
        // Convert tissue cell to vein cell
        grid[x][y].type = VEIN_CELL;

        Vector2 vein_point = findCLosestVeinPoint(grid, x, y);
        Vector2 direction = {floor(vein_point.x - x), floor(vein_point.y - y)};

        if (abs(direction.x) > 1)
            direction.x /= abs(direction.x);
        if (abs(direction.y) > 1)
            direction.y /= abs(direction.y);

        printf("Direction: (%f, %f)\n", direction.x, direction.y);

        // Recursively grow veins in neighboring cells
        growVeins(grid, x + direction.x, y + direction.y, noise);
        // growVeins(grid, x + 1, y, veinThreshold, noise);
        // growVeins(grid, x - 1, y, veinThreshold, noise);
        // growVeins(grid, x, y + 1, veinThreshold, noise);
        // growVeins(grid, x, y - 1, veinThreshold, noise);
    }
}

Vector2 findCLosestVeinPoint(CELL grid[][GRID_SIZE], int x, int y)
{
    float min_distance = RAND_MAX, distance;
    Vector2 minpoint;
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            distance = sqrt(pow(i - x, 2) + pow(j - y, 2));
            if (distance < min_distance)
            {
                min_distance = distance;
                minpoint.x = i;
                minpoint.y = j;
            }
        }
    }
    return minpoint;
}

void seedInitialVeinPoints(CELL grid[][GRID_SIZE], int numPoints)
{
    grid[0][0].type = VEIN_CELL;
    for (int i = 0; i < numPoints - 1; i++) {
        int x = rand() % GRID_SIZE;
        int y = rand() % GRID_SIZE;
        grid[x][y].type = VEIN_CELL;
        // You may also want to initialize other properties of the vein cells here
    }
}

void displayGrid(CELL grid[][GRID_SIZE]) 
{
    // Clear terminal
    // printf("\033[H\033[J");
    // Display the grid
    for (int i = 0; i < GRID_SIZE; i++) 
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            if (grid[i][j].type == TISSUE_CELL)
            {
                int state = grid[i][j].cell_data.tissue_cell.state;
                if (state == 0)
                    printf(ANSI_COLOR_LIGHT_GREEN "%c " ANSI_COLOR_RESET, HEALTHY);
                else if (state == -1)
                    printf(ANSI_COLOR_DARK_GRAY "%c " ANSI_COLOR_RESET, NECROTIC);
                else if (state == 1)
                    printf(ANSI_COLOR_MAGENTA "%c " ANSI_COLOR_RESET, TUMOR);
            }
            else if (grid[i][j].type == VEIN_CELL)
                printf(ANSI_COLOR_RED "%c " ANSI_COLOR_RESET, HEALTHY);
            else
                printf("  ");
        }
        printf("\n");
    }
}