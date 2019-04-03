//SO2 IS1 210B LAB03
//Patryk Wszola
//wp41563@zut.edu.pl
//gcc 41563.so2.lab03.main.c -o lab03
//./lab03 abcde

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int isPower(int len);

int main(int argc, char *argv[])
{
    int status = 0;
    if (argv[1] == NULL)
    {
        printf("Brak argumentu!\n");
        return -1;
    }
    else
    {
        int length = strlen(argv[1]);
        char *arg = NULL;
        if (isPower(length) != 1)
        {
            int tempLength = 1, newLength = 1;
            while (tempLength < length)
            {
                newLength = tempLength;
                tempLength *= 2;
            }
            arg = malloc((newLength + 1) * sizeof(char));
            for (int i = 0; i < newLength; i++)
            {
                arg[i] = argv[1][i];
            }
            arg[newLength] = '\0';
        }
        if (!arg)
            arg = argv[1];

        char *left;
        char *right;
        char *sen;
        //alokacja pamieci na cala sciezke
        if (!argv[2])
            sen = malloc((strlen(arg) * strlen(arg)) * sizeof(char));
        else
            sen = argv[2];

        if (strlen(arg) > 1)
        {
            //alokacja pamieci na kolejne watki
            int mid = strlen(arg) / 2;
            left = malloc((mid + 1) * sizeof(char));
            right = malloc((mid + 1) * sizeof(char));

            for (int i = 0; i < mid; i++)
            {
                left[i] = arg[i];
                right[i] = arg[i + mid];
            }
            left[mid] = '\0';
            right[mid] = '\0';

            strcat(sen, arg);
            strcat(sen, " ");

            pid_t pid = fork();

            if (pid == 0)
                execl("./lab03", "./lab03", left, sen, (char *)NULL);

            pid_t pid2 = fork();

            if (pid2 == 0)
                execl("./lab03", "./lab03", right, sen, (char *)NULL);
        }
        else
        {
            strcat(sen, arg);
            printf("%d %s\n", getpid(), sen);
            exit(1);
        }

        if (wait(&status))
        {
            if (!WIFEXITED(status))
                printf("BLAD wait()");
            else
                ;
        }
        if (wait(&status))
        {
            if (!WIFEXITED(status))
                printf("BLAD wait()");
            else
                ;
        }

        printf("%d %s\n", getpid(), sen);
    }

    return 1;
}

int isPower(int len)
{
    while (len != 1)
    {
        if (len % 2 != 0)
            return 0;
        len = len / 2;
    }
    return 1;
}