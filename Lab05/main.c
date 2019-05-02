#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>

int filesCounter = 0, foldersCounter = 0;
int flag1 = 0, flag2 = 0, flag3 = 0, flag4 = 0;

void listDirectory(char *arg, int indent)
{
    struct dirent **namelist;
    int n = 0; //ilosc wszystkich plikow
    char **folders, **foldersNames;
    folders = malloc(30 * sizeof(char *));
    for (int i = 0; i < 30; i++)
        folders[i] = malloc(FILENAME_MAX);
    foldersNames = malloc(30 * sizeof(char *));
    for (int i = 0; i < 30; i++)
        foldersNames[i] = malloc(FILENAME_MAX);
    int f = 0; //ilosc folderow
    struct stat path_stat;
    char *path;
    path = malloc(FILENAME_MAX);

    n = scandir(arg, &namelist, 0, alphasort);
    if (n < 0)
        perror("scandir");
    else
    {
        while (n--)
        {
            strcpy(path, arg);
            strcat(path, namelist[n]->d_name);
            path = realpath(path, NULL);
            stat(path, &path_stat);
            if (S_ISREG(path_stat.st_mode) != 0)
            {
                if (namelist[n]->d_name[0] != '.')
                {

                    if (flag1 == 0)
                    {
                        for (int i = 0; i < indent; i++)
                            printf("   ");
                        if (flag2 == 0)
                            printf("|> %s\n", namelist[n]->d_name);
                        else
                            printf("|> %s\n", path);

                        filesCounter++;
                    }
                }
                free(namelist[n]);
            }
            else
            {
                if (namelist[n]->d_name[0] != '.')
                {
                    foldersCounter++;
                    strcpy(foldersNames[f], namelist[n]->d_name);
                    strcpy(folders[f], path);
                    f++;
                }
                free(namelist[n]);
            }
        }
        free(namelist);
        while (f--)
        {
            for (int i = 0; i < indent; i++)
                printf("   ");
            if (flag2 == 0)
                printf("|# %s\n", foldersNames[f]);
            else
                printf("|# %s\n", folders[f]);

            strcat(folders[f], "/");
            if (indent + 1 < flag4 || flag4 == 0)
            {
                listDirectory(folders[f], ++indent);
                indent--;
            }
            free(folders[f]);
        }
        free(folders);
    }
}

int main(int argc, char *artv[])
{
    int opt;
    while ((opt = getopt(argc, artv, "dflL:")) != -1)
    {
        switch (opt)
        {
        case 'd':
            flag1++;
            break;
        case 'f':
            flag2++;
            break;
        case 'l':
            flag3++;
            break;
        case 'L':
            flag4 = atoi(optarg);
            if (flag4 < 0 || flag4 > 10)
            {
                printf("BLAD: argument tylko z zakresu od 0 do 10\n");
                exit(EXIT_FAILURE);
            }
            break;
        default:
            printf("Uzycie: ./program [-dfl] [-L poziom] [sciezka]\n");
            exit(EXIT_FAILURE);
        }
    }
    if (argc == 1)
        listDirectory("./", 0);
    else
    {
        if (artv[argc - 1][0] != '-')
            listDirectory(artv[argc - 1], 0);
        else
            listDirectory("./", 0);
    }

    if (foldersCounter > 0 && filesCounter > 0)
        printf("\n%d folderow, %d plikow\n", foldersCounter, filesCounter);
    else if (foldersCounter > 0 && filesCounter == 0)
        printf("\n%d folderow\n", foldersCounter);
    else
        printf("\n%d plikow\n", filesCounter);
}