#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct s_zone
{
    int w;
    int h;
    char c;
    char *tab;
} t_zone;

typedef struct s_rec
{
    char type;
    float x;
    float y;
    float xd;
    float yd;
    char c;
} t_rec

int main(int ac, char **av)
{
    t_zone map;
    t_rec rect;
    FILE *file;

    if (ac != 2)
    {
        write(1,"Error: argument\n",16);
        return 1;
    }
    return 0;
}
