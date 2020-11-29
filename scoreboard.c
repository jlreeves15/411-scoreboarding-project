#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

typedef struct scoreboard
{
    char instruction[10];
    char dest_reg[10];
    char source_reg1[10];
    char source_reg2[10];

} scoreboard_t;
 

int main(int argc, char *argv[])
{
    FILE *fp;
    int bufsize = BUFSIZ;
    int index = 0;
    char buf[100];
    char *tokens = malloc(sizeof(char)* bufsize);
    char* token;
    struct scoreboard my_scoreboard[bufsize];
    int counter = 0;
    //char buf[100];
    //char* token;
    //int i = 0;
    //fp = fopen(argv[1], "rb");
    fp = fopen("instructions.txt", "r");
    if(!fp)
    {
        printf("Unable to open file\n");
        return 1;
    } else
    {
        while(!feof(fp))
        {
            fgets(buf, sizeof(buf), fp);
            puts(buf);
            token = strtok(buf, " ");
            //temp = token;
            while(token != NULL)
            {
                //printf("%d\n",index);
                //printf("%s\n",token);
                //tokens[index] = token;
                strcpy(tokens[index],token);
                index++;
                puts(token);
            /*
            if(index >= bufsize){
            bufsize += BUFSIZ;
            tokens = realloc(tokens,bufsize * sizeof(buf));
            }
            */
            token = strtok(NULL, " ");
            //temp = token;
            }
            /*
        strcpy(my_scoreboard[counter].instruction,tokens[0]);
        strcpy(my_scoreboard[counter].dest_reg,tokens[1]);
        strcpy(my_scoreboard[counter].source_reg1,tokens[2]);
        strcpy(my_scoreboard[counter].source_reg2,tokens[3]);
        counter++;
        */
        }
        printf("%d", sizeof(tokens));
        /*
        for(int i = 0; i < sizeof(); i++)
        {

            //printf("%s\n", tokens[i]);
        }
        }
        */
        
    }
    
        fclose(fp);
        return 0;
}