#include <stdio.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <utmp.h>
#include <grp.h>
#include <pwd.h>
#include <getopt.h>

#include <dlfcn.h>

int main(int argc, char *argv[])
{
    int opt;
    int flag1 = 0, flag2 = 0, flag3 = 0;
    struct utmp *n;
    setutent();
    n = getutent();

    char **groupNames;
    void *handle;
    char *error;

    char *(*displayUserGroups)(char *name);
    char *groupsList;

    handle = dlopen("./libdisplayGroups.so", RTLD_LAZY);

    if (!handle)
    {
        flag3++;
        fprintf(stderr, "%s\n", dlerror());
        printf("Uruchamiam program bez argumentow:\n\n");
    }
    else
    {
        displayUserGroups = dlsym(handle, "displayUserGroups");
    }

    dlerror();

    error = dlerror();
    if (error != NULL)
    {
        fprintf(stderr, "%s\n", error);
        //exit(EXIT_FAILURE);
    }

    if (argv[1] == NULL || flag3 > 0)
    {
        while (n)
        {
            if (n->ut_type == USER_PROCESS)
            {
                printf("%s ", n->ut_user);
                printf("\n");
            }
            n = getutent();
        }
    }
    else
    {
        while ((opt = getopt(argc, argv, "hg")) != -1)
        {
            switch (opt)
            {
            case 'h':
                flag1++;
                break;
            case 'g':
                flag2++;
                break;
            default:
                exit(EXIT_FAILURE);
            }
        }
        if (flag1 > 0 && flag2 == 0)
        {
            setutent();
            n = getutent();
            while (n)
            {
                if (n->ut_type == USER_PROCESS)
                {
                    printf("%s ", n->ut_user);
                    printf("(%s)", n->ut_host);
                    printf("\n");
                }
                n = getutent();
            }
        }
        else if (flag2 > 0 && flag1 == 0)
        {
            while (n)
            {
                if (n->ut_type == USER_PROCESS)
                {
                    printf("%s ", n->ut_user);
                    groupsList = displayUserGroups(n->ut_user);
                    printf("%s", groupsList);
                    free(groupsList);
                    printf("\n");
                }
                n = getutent();
            }
        }
        else if (flag1 > 0 && flag2 > 0)
        {
            while (n)
            {
                if (n->ut_type == USER_PROCESS)
                {
                    printf("%s ", n->ut_user);
                    printf("(%s) ", n->ut_host);
                    groupsList = displayUserGroups(n->ut_user);
                    printf("%s", groupsList);
                    free(groupsList);
                    printf("\n");
                }
                n = getutent();
            }
        }
    }
    if (handle)
        dlclose(handle);

    return 0;
}
