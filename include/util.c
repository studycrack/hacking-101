#include "util.h"
#include <libgen.h>
#include <limits.h>
#include <time.h>

static char guess[] = "Who am I???\0";
static char filename[] = "image.txt\0";

void get_dir_path(char *path) {
        // Get absolute path of current executable
        int ret = readlink("/proc/self/exe", path, UTIL_MAX_LEN);
        if (ret < 0) {
                perror("readlink error");
                path = NULL;
                return;
        }

        path = dirname(path);
        // printf("Directory is %s\n", path);
}

void print_color(char * str, char * color, char * bgcolor)
{
        char fmt[UTIL_MAX_LEN + 0x10] = {0};

        if (strlen(str) <= UTIL_MAX_LEN) {
                if (bgcolor != NULL)
                        sprintf(fmt, "%s%s%s %s %s", LOG_BOLD, bgcolor, color, str, LOG_RESET);
                else
                        sprintf(fmt, "%s%s %s %s", LOG_BOLD, color, str, LOG_RESET);
                printf("%s", fmt);
                return;
        }
}

void print_str_color(char * str, char * color, char * bgcolor)
{
        const int cnt = 68;
        int i, padding, remain = 0;
        char upper_str[UTIL_MAX_LEN] = {0};

        // Convert to uppercase
        for (int i = 0; str[i] != '\0'; i++) {
                upper_str[i] = toupper(str[i]);
                if (i == UTIL_MAX_LEN) {
                        break;
                }
        }

        padding = (cnt - 2 - strlen(str)) / 2;
        remain = (cnt - 2 - strlen(str)) % 2;

        // Left padding
        for (i = 0; i < padding; i++) {
                printf("=");
        }

        // Print string
        print_color(upper_str, color, bgcolor);

        // Right padding
        for (i = 0; i < padding; i++) {
                printf("=");
        }
        if (remain) {
                printf("=");
        }
        printf("\n");
}

static inline void print_time()
{
	time_t timep;
	struct tm *p;
	time(&timep);
	p = gmtime(&timep);
	printf("%d-%02d-%02d %02d:%02d:%02d", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, 8 + p->tm_hour, p->tm_min, p->tm_sec);
}

int get_abs_path(char * filename, char *abs_path)
{
        get_dir_path(abs_path);
        // Get full file path
        if (abs_path == NULL) {
                return -1;
        }
        strcat(abs_path, "/");
        strcat(abs_path, filename);
        return 0;
}

int print_image(char * pokemon_name, char * filename)
{
	char read_string[UTIL_MAX_LEN], file_path[UTIL_MAX_LEN];
	FILE * fptr = NULL;
        int flag = 0;

        if (get_abs_path(filename, file_path) < 0) {
                fprintf(stderr, "error get absolute path %s\n", file_path);
                return -1;
        }
        
        // Open Pokémon ASCII art file
	if((fptr = fopen(file_path, "r")) == NULL) {
		fprintf(stderr, "error opening %s\n", file_path);
		return -1;
	}

        // Print title/guess string
        print_str_color(guess, LOG_BLUE, NULL);

        // Print Pokémon art line by line
	while(fgets(read_string, sizeof(read_string), fptr) != NULL) {
                if (flag == 0) {
                        flag++;
                        continue;
                }
                printf("%s", read_string);
                usleep(100000);
        }

	printf("\n");
        sleep(2);
        
        // Print Pokémon name
        strcat(pokemon_name, "!");
        print_str_color(pokemon_name, LOG_CYAN, NULL);
        pokemon_name[strlen(pokemon_name) - 1] = '\0';

	fclose(fptr);
	return 0;
}

void print_desc_name(char * pokemon_name)
{
        printf("###\n");
        printf("### Before officially challenging ");
        print_color(pokemon_name, LOG_CYAN, NULL);
        printf(", please carefully read and learn the battle tips in the description.\n");
	printf("### If it's too difficult, try defeating lower-level Pokémon first.\n");
	printf("### Good luck :)\n");
	printf("###\n");
}

void get_pokemon_name(char * filename, char * name, int num)
{
        char fmt[UTIL_MAX_LEN] = {0}, file_path[UTIL_MAX_LEN];
	FILE * fptr = NULL;

        // Open Pokémon name file
        get_abs_path(filename, file_path);
	if((fptr = fopen(file_path, "r")) == NULL) {
		fprintf(stderr, "error opening %s\n", file_path);
                return;
	}
        
        // Read up to the nth line
        sprintf(fmt, "%s%d%s", "%", num, "s[^\n]");
        fscanf(fptr, fmt, name);
        fclose(fptr);
}

void print_exit()
{
        int name_len = 32;
        char name[name_len];

        // Get Pokémon name
        get_pokemon_name(filename, name, name_len);

        printf("\n### ");
	print_time();
        printf(" Battle log: ");
        print_color(name, LOG_RED, NULL);
        printf(" won!\n");
	printf("### Come back and challenge again after leveling up!\n");
        fflush(stdout);
}

void print_desc()
{
        int name_len = 32;
        char name[name_len];

        // Get Pokémon name
        get_pokemon_name(filename, name, name_len);

        // Print Pokémon ASCII art
        print_image(name, filename);

        // Print description message
	print_desc_name(name);
        fflush(stdout);
}
