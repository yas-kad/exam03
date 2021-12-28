#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_zone
{
    int w;
    int h;
    char c;
    char *tab;
} t_zone;

typedef struct s_rect
{
    char type;
    float x;
    float y;
    float xd;
    float yd;
    char c;
} t_rect;

void filled_rect(t_zone *map, t_rect rect)
{
    int i = 0;
    int j;

    while (i < map->h)
    {
        j = 0;
        while (j < map->w)
        {
            if (j >= rect.x && j <= rect.xd + rect.x && i >= rect.y && i <= rect.y + rect.yd)
            {
                map->tab[j + i*map->w] = rect.c;   
             }
             j++;
        }
        i++;
    }
}

void empty_rect(t_zone *map, t_rect rect)
{
    int i = 0;
    int j;

    while (i < map->h)
    {
        j = 0;
        while (j < map->w)
        {
            if (j >= rect.x && j <= rect.xd + rect.x && i >= rect.y && i <= rect.y + rect.yd)
            {
                if (j - rect.x < 1 || i - rect.y < 1 || rect.x + rect.xd - j < 1 || rect.y + rect.yd - i < 1)
                {
                    map->tab[j + i*map->w] = rect.c;  
                } 
            }
             j++;
        }
        i++;
    }
}

int main(int ac, char **av)
{
    t_zone map;
    t_rect rect;
    FILE *file;

    if (ac != 2)
    {
        write(1, "Error: argument\n", 16);
        return 1;
    }
    file = fopen(av[1], "r");
    if (!file)
    {
        write(1, "Error: Operation file corrupted\n", 32);
        return 1;
    }
    int m = fscanf(file, "%d %d %c \n", &map.w, &map.h, &map.c);
    if (m != 3)
    {
        write(1, "Error: Operation file corrupted\n", 32);
        return 1;
    }
    if (map.w > 300 || map.h > 300 || map.w <= 0 || map.h <= 0)
    {
        write(1, "Error: Operation file corrupted\n", 32);
        return 1;
    }
    map.tab = (char *)malloc(map.h * map.w);
    memset(map.tab, map.c, map.h * map.w);

    int r = fscanf(file, "%c %f %f %f %f %c \n", &rect.type, &rect.x, &rect.y, &rect.xd, &rect.yd, &rect.c);
    while (r == 6)
    {
        if (rect.xd <= 0 || rect.yd <= 0)
        {
            write(1, "Error: Operation file corrupted\n", 32);
            return 1;
        }
        if (rect.type == 'R')
        {
            filled_rect(&map,rect);
        }
        else if (rect.type == 'r')
        {
            empty_rect(&map, rect);
        }
        else
        {
           write(1, "Error: Operation file corrupted\n", 32);
            return 1; 
        }
        r = fscanf(file, "%c %f %f %f %f %c \n", &rect.type, &rect.x, &rect.y, &rect.xd, &rect.yd, &rect.c);
    }
    if (r != 6 && r != -1 && rect.c != '\n')
    {
        write(1, "Error: Operation file corrupted\n", 32);
        return 1;
    }
    int i = 0;
    while (i < map.h * map.w)
    {
        write(1, map.tab + i, map.w);
        write(1, "\n", 1);
        i += map.w;
    }
    return 0;
}