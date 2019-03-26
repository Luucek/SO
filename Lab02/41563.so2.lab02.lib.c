#include <stdio.h>
#include <stdlib.h>
#include <utmp.h>
#include <grp.h>
#include <pwd.h>

#include <string.h>
char *groupString;

char *displayUserGroups(char *name)
{
    int ngroups;
    gid_t *groups;
    struct passwd *pw;
    struct group *gr;

    ngroups = 0;
    pw = getpwnam(name);
    if (getgrouplist(name, pw->pw_gid, groups, &ngroups) == -1)
    {
        groups = malloc(ngroups * sizeof(gid_t));
        getgrouplist(name, pw->pw_gid, groups, &ngroups);
    }

    //alokacja pamieci na tablice tablic charow
    char **groupNames = (char **)malloc(ngroups * sizeof(char *));

    //wypelnianie tablic z nazwami grup
    for (int i = 0; i < ngroups; i++)
    {
        gr = getgrgid(groups[i]);
        if (gr != NULL)
        {
            groupNames[i] = (char *)malloc(strlen(gr->gr_name) * sizeof(char));
            strcpy(groupNames[i], gr->gr_name);
        }
    }

    int size = 0;

    for (int i = 0; i < ngroups; i++)
    {
        size += strlen(groupNames[i]);
    }

    groupString = malloc(size + ngroups + 4);

    int index = 0;
    groupString[index] = '[';
    index++;
    groupString[index] = ' ';
    index++;
    for (int i = 0; i < ngroups; i++)
    {
        for (int j = 0; j < strlen(groupNames[i]); j++)
        {
            groupString[index] = groupNames[i][j];
            index++;
        }
        groupString[index] = ' ';
        index++;
    }
    groupString[index] = ']';
    index++;
    groupString[index] = '\0';
    //printf("%s", groupString);

    free(groups);
    for (int i = 0; i < ngroups; i++)
    {
        free(groupNames[i]);
    }
    free(groupNames);

    return groupString;
}
