#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // Для функции usleep
#include <ncurses.h> // для изменения скорости

#define HEIGHT 25
#define WIDTH 80
#define BASE_SPEED 100000  // Базовая задержка в микросекундах

void create_memory(int ***matrix);
void input_start_stars(int n);
int read_start_position(int *n);
void game(int **map, int n);
void print_field(int **map);
int count_living_cells(int **map, int x, int y);
int result_of_compare(int result);
void algorythm(int **map);
void input_stars(int **map);
void clean_dynamic(int **matrix);

int main() {
    int **map, n, flag = 1;

    if (read_start_position(&n)) {
        flag = 0;
    }

    create_memory(&map);

    if (flag) {
        game(map, n);
    }

    clean_dynamic(map);
}

void game(int **map, int n) {
    input_start_stars(n);
    freopen("./presets/1.txt", "r", stdin);
    input_stars(map);

    float speed_multiplier = 1.0;  // начальная скорость
    while (1) {
        print_field(map);
        algorythm(map);  // обновление состояния игры

        // изменение скорости игры через ввод (n)
        change_speed(&speed_multiplier);

        // задержка на основе множителя скорости
        usleep(BASE_SPEED / speed_multiplier);
    }
}

int read_start_position(int *n) {
    printf("Введите вариант стартовой позиции: \n");
    printf("1 - 1preset\n");
    printf("2 - 2preset\n");
    if (scanf("%d", n) != 1 || *n < 1 || *n > 5) {
        return -1;
    }
    return 0;
}

void create_memory(int ***matrix) {
    *matrix = malloc(HEIGHT * sizeof(int *));
    for (int i = 0; i < HEIGHT; i++) {
        (*matrix)[i] = malloc(WIDTH * sizeof(int));
    }
}

void input_start_stars(int n) {
    switch (n) {
        case 1:
            if (freopen("./presets/1.txt", "r", stdin) == NULL) {
                perror("ошибка открытия 1.txt");
                exit(EXIT_FAILURE);
            }
            break;
        case 2: 
            if (freopen("./presets/2.txt", "r", stdin) == NULL) {
                perror("ошибка открытия 2.txt");
                exit(EXIT_FAILURE);
            }
            break;
            // case 3: freopen("", "r", stdin); break;
            // case 4: freopen("", "r", stdin); break;
            // case 5: freopen("", "r", stdin); break;
    }
}

void input_stars(int **map) {
    char c;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            scanf(" %c", &c);
            if (c == '1')
                map[i][j] = 1;
            else
                map[i][j] = 0;
        }
    }
}

void print_field(int **map) {
    printf("\033c");
    printf("                            GAME OF LIFE\n");
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (map[i][j] == 1)
                printf("*");
            else
                printf(" ");
        }
        printf("\n");
    }
    printf("\n");
}

int count_living_cells(int **map, int x, int y) {
    int counter_lives = 0;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;
            int nx = (x + dx + HEIGHT) % HEIGHT;
            int ny = (y + dy + WIDTH) % WIDTH;
            counter_lives += map[nx][ny];
        }
    }
    return counter_lives;
}

int result_of_compare(int result) {
    if (result == 3) return 1;                 // новая жизнь
    if (result == 2 || result == 3) return 1;  // жизнь
    return 0;                                  // смерть
}

void algorythm(int **map) {
    int **buffer;
    create_memory(&buffer);

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int counter_lives = count_living_cells(map, i, j);
            buffer[i][j] = result_of_compare(counter_lives);
        }
    }

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            map[i][j] = buffer[i][j];
        }
    }

    clean_dynamic(buffer);
}

void clean_dynamic(int **matrix) {
    for (int i = 0; i < HEIGHT; i++) {
        free(matrix[i]);
    }
    free(matrix);
}
