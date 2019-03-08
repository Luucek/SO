#include <stdio.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <utmp.h>
#include <grp.h>
#include <pwd.h>
#include <getopt.h>

void displayUserGroups(struct utmp *n) {
    int j, ngroups, ngroups_temp;
    gid_t *groups;
    struct passwd *pw;
    struct group *gr;

    if(n->ut_type==USER_PROCESS) {
        ngroups = 0;
        pw = getpwnam(n->ut_user);
        if (getgrouplist(n->ut_user, pw->pw_gid, groups, &ngroups) == -1) {
            groups = malloc(ngroups * sizeof(gid_t));
            getgrouplist(n->ut_user, pw->pw_gid, groups, &ngroups);

        }
        printf("[ ");
        for (j = 0; j < ngroups; j++) {
            gr = getgrgid(groups[j]);
            if (gr != NULL)
                printf("%s ", gr->gr_name);
        }
        printf("]");
    }
}
int main(int argc, char *argv[]) {
    int opt;
    int flag1 = 0, flag2 = 0;
    struct utmp *n;
    setutent();
    n=getutent();
    
    if (argv[1] == NULL) {
        while(n) {
            if(n->ut_type==USER_PROCESS) {
                printf("%s ", n->ut_user);
                printf("\n");
            }
            n=getutent();
        }
    }
    else {
    while ((opt = getopt(argc, argv, "hg")) != -1) {
            switch (opt) {
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

        if (flag1 > 0 && flag2 == 0) {
            setutent();
            n=getutent();
            while(n) {
                if(n->ut_type==USER_PROCESS) {
                    printf("%s ", n->ut_user);
                    printf("(%s)", n->ut_host);
                    printf("\n");
                }
                n=getutent();
            }
        }
        else if (flag2 > 0 && flag1 == 0) {
            while(n) {
                if(n->ut_type==USER_PROCESS) {
                    printf("%s ", n->ut_user);
                    displayUserGroups(n);
                    printf("\n");
                }
                n=getutent();
            }
        }
        else if (flag1 > 0 && flag2 > 0) {
            while(n) {
                if(n->ut_type==USER_PROCESS) {
                    printf("%s ", n->ut_user);
                    printf("(%s) ", n->ut_host);
                    displayUserGroups(n);
                    printf("\n");
                }
                n=getutent();
            }
        }
    }
    return 0;
}
