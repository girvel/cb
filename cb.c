#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

void assert_xclip(const FILE *xclip) {
    if (xclip != NULL) return;
    fprintf(stderr, "Failed to run xclip; is it installed?");
    exit(1);
}

int main() {
    char line[4096];

    if (!isatty(STDIN_FILENO)) {
        FILE *xclip = popen("xclip -selection clipboard", "w");
        assert_xclip(xclip);

        while (fgets(line, sizeof(line), stdin) != NULL) {
            fprintf(xclip, "%s", line);
        }

        pclose(xclip);
    }

    FILE *xclip = popen("xclip -o -selection clipboard", "r");
    assert_xclip(xclip);

    while (fgets(line, sizeof(line), xclip) != NULL) {
        printf("%s", line);
    }

    pclose(xclip);

    return 0;
}
