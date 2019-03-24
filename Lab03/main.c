#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int isPower(int len);

int main(int argc, char *argv[])
{
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
            arg[newLength + 1] = '\0';
        }
        if (!arg)
            arg = argv[1];

        int *status;

        char *left;
        char *right;

        if (strlen(arg) > 1)
        {
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
        }
        else
        {
            printf("%s ", arg);
            exit(1);
        }

        //printf("Ciag znakow: %s\n", arg); // do usuniecia

        pid_t pid = fork();

        if (pid == 0)
        {
            // child process
            execl("./lab03", "./lab03", left, (char *)NULL);
            execl("./lab03", "./lab03", right, (char *)NULL);
        }
        else if (pid > 0)
        {
            // parent process
            waitpid(pid, status, 0);
            //printf("%s ", arg);
        }
        else
        {
            // fork failed
            return -2;
        }
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
