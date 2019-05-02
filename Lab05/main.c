#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int filesCounter = 0, foldersCounter = 0;

void listDirectory(char *arg, int indent)
{
    struct dirent **namelist;
    int n = 0; //ilosc wszystkich plikow
    char **folders, **foldersNames;
    folders = malloc(30 * sizeof(int *));
    for (int i = 0; i < 30; i++)
        folders[i] = malloc(FILENAME_MAX);
    foldersNames = malloc(30 * sizeof(int *));
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
                    for (int i = 0; i < indent; i++)
                        printf("   ");

                    printf("|> %s\n", namelist[n]->d_name);
                    filesCounter++;
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

            printf("|# %s\n", foldersNames[f]);
            strcat(folders[f], "/");
            listDirectory(folders[f], ++indent);
            indent--;
            free(folders[f]);
        }
        free(folders);
    }
}

int main(int argc, char *artv[])
{
    if (!artv[1])
        listDirectory("./", 0);
    else
        listDirectory(artv[1], 0);

    printf("\n%d folderow, %d plikow\n", foldersCounter, filesCounter);
}