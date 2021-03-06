//
//  main.c
//  lsh
//  This is an example shell being created by me for all sorts of experiments.
//  Created by Vishwanath Seshagiri on 20/02/17.
//  Copyright (c) 2017 DarthCoders. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#define LSH_LINE_BUFFER 1024
#define EXIT_SUCCESS 1
#define EXIT_FAILURE 0
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\n\a\r"


char *lsh_read_line()
{
    int bufferSize = LSH_LINE_BUFFER;
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufferSize);
    int c; // C needs be an integer, since EOF is an integer, and there's no escaping that! :)
    
    if(!buffer)
    {
        fprintf(stderr, "Error in allocating the memory ; LSH \n");
        exit(EXIT_FAILURE);
    }
    
    while (1) {
        c = getchar();
        if (c == EOF || c == '\n') {
            buffer[position] = '\0';
            return buffer;
        }
        else
        {
            buffer[position++] = c;
        }
        
        //If the buffer size is exceded, then the memory space is re-allocated.
        
        if (position > bufferSize) {
            bufferSize += LSH_LINE_BUFFER;
            buffer = realloc(buffer, bufferSize);
            
            if (!buffer) {
                fprintf(stderr, "Buffer Re-Allocation Error. Try again.");
                return EXIT_FAILURE;
                
            }
            
        }
    }
}

char **lsh_split_line(char *line)
{
    int bufsize = LSH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;
    
    if(!token)
    {
        fprintf(stderr, "LSH: Allocation Error\n");
        exit(EXIT_FAILURE);
        
    }
    
    token = strtok(line, LSH_TOK_DELIM);
    while (token != NULL) {
        tokens[position] = token;
        position++;
        
        if (position >= bufsize) {
            bufsize += LSH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "LSH: Allocation Error\n");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, LSH_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}
void lsh_loop(void){
    char* line;
    char** args;
    int status;
    
    do{
        printf(">");
        line = lsh_read_line();
        args = lsh_split_line(line);
        status = lsh_execute(args);
        
        free(line);
        free(args);
        
    }while (status);
    
}

int lsh_launch(char **args)
{
    pid_t pid, wpid;
    int status;
    pid = fork();
    if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("LSH ERROR\n");
        }
        exit(EXIT_FAILURE);
    }else if (pid < 0){
        perror("ERROR IN FORKING\n");
    }else{
        do{
            wpid = waitpid(pid, &status, WUNTRACED);
        }while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    
    return 1;
}

int main(int argc, const char * argv[]) {
    
    
    lsh_loop();
    
    return EXIT_SUCCESS;
}