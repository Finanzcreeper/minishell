#include <unistd.h>
#include <stdio.h>

int main() {
    char *argv[] = {"/bin/ls", "-l", NULL};
    char *envp[] = {NULL};

    execve("/bin/ls", argv, envp);

    // This code is never reached after execve succeeds

    return 0;
}