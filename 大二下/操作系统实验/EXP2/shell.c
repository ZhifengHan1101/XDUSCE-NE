#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 80
#define MAX_HISTORY 10

void parse_input(char *input, char **args, int *background) {
    char *token = strtok(input, " \n");
    int i = 0;
    while (token != NULL) {
        if (strcmp(token, "&") == 0) {
            *background = 1;
        } else {
            args[i++] = token;
        }
        token = strtok(NULL, " \n");
    }
    args[i] = NULL;
}

void add_history(char *input, char history[][MAX_LINE], int *history_count) {
    if (*history_count < MAX_HISTORY) {
        strcpy(history[*history_count], input);
        (*history_count)++;
    } else {
        for (int i = 1; i < MAX_HISTORY; i++) {
            strcpy(history[i - 1], history[i]);
        }
        strcpy(history[MAX_HISTORY - 1], input);
        (*history_count)++;
    }
}

int main() {
    char input[MAX_LINE];
    char *args[MAX_LINE / 2 + 1];
    int background;
    char history[MAX_HISTORY][MAX_LINE];
    int history_count = 0;

    while (1) {
        background = 0;
        printf("osh > ");
        fflush(stdout);
        fgets(input, MAX_LINE, stdin);

        if (strcmp(input, "exit\n") == 0) {
            break;
        }

        if (strcmp(input, "\n") == 0) {
            continue;
        }

        if (strcmp(input, "history\n") == 0) {
            if (history_count <= MAX_HISTORY)
                for (int i = 0; i < history_count; i++) {
                    printf("%d %s", i + 1, history[i]);
                }
            else
                for(int i = 0; i < MAX_HISTORY; i++){
                    printf("%d %s", history_count - MAX_HISTORY + i + 1, history[i]);
                }
                
            continue;
        }

        if (strcmp(input, "!!\n") == 0) {
            if (history_count == 0) {
                printf("No commands in history\n");
                continue;
            } else {
                strcpy(input, history[history_count - 1]);
            }
        } else if (input[0] == '!') {
            int index = atoi(input + 1);
            if (index > history_count || index < history_count - MAX_HISTORY + 1) {
                printf("No such command in history\n");
                continue;
            } else {
                strcpy(input, history[index - (history_count - MAX_HISTORY + 1)]);
            }
        } else {
            add_history(input, history, &history_count);
        }

        parse_input(input, args, &background);

        if (strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL) {
                fprintf(stderr, "osh: expected argument to \"cd\"\n");
            } else {
                if (chdir(args[1]) != 0) {
                    perror("osh");
                }
            }
            continue;
        }

        pid_t pid = fork();

        if (pid == 0) {
            if (execvp(args[0], args) == -1) {
                perror("execvp");
                exit(EXIT_FAILURE);
            }
        } else if (pid > 0) {
            if (!background) {
                waitpid(pid, NULL, 0);
            }
        } else {
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
