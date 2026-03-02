#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

void assert_xclip(const FILE *xclip) {
    if (xclip != NULL) return;
    fprintf(stderr, "Failed to run xclip; is it installed?");
    exit(1);
}

void help(const char *self) {
    printf("Usage: %s [-p | --primary]\n", self);
    printf("If piped in, sets clipboard; always prints it. Requires xclip.\n");
    printf("\n");
    printf("  -p, --primary    Use primary instead of cliboard\n");
    printf("  -h, --help       Display help message and exit\n");
}

int main(int argc, char **argv) {
    bool uses_primary = false;

    for (int i = 1; i < argc; i++) {
        char *arg = argv[i];
        if (strcmp(arg, "--help") == 0 || strcmp(arg, "-h") == 0) {
            help(*argv);
            return 0;
        } else if (strcmp(arg, "--primary") == 0 || strcmp(arg, "-p") == 0) {
            uses_primary = true;
        }
    }

    char line[4096];
    char command[64];
    strcpy(command, uses_primary
        ? "xclip -selection primary"
        : "xclip -selection clipboard");

    if (!isatty(STDIN_FILENO)) {
        FILE *xclip = popen(command, "w");
        assert_xclip(xclip);

        while (fgets(line, sizeof(line), stdin) != NULL) {
            fprintf(xclip, "%s", line);
        }

        pclose(xclip);
    }

    strcat(command, " -o");
    FILE *xclip = popen(command, "r");
    assert_xclip(xclip);

    while (fgets(line, sizeof(line), xclip) != NULL) {
        printf("%s", line);
    }

    pclose(xclip);

    return 0;
}
