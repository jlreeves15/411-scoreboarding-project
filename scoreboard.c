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
    char* token;
    struct scoreboard my_scoreboard[bufsize];
    int counter = 0;
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
	    while(token != NULL)
            {
		int num_mod = index % 4;
		switch(num_mod)
		{
		  case 0:
			strcpy(my_scoreboard[counter].instruction, token);
			index++;
			printf("I: %s\n", my_scoreboard[counter].instruction);
			token = strtok(NULL, " ");
			break;
		  case 1:
			strcpy(my_scoreboard[counter].dest_reg, token);
                        index++;
			printf("D: %s\n", my_scoreboard[counter].dest_reg);
                        token = strtok(NULL, " ");
			break;
		  case 2:
			strcpy(my_scoreboard[counter].source_reg1, token);
                        index++;
			printf("S1: %s\n", my_scoreboard[counter].source_reg1);
                        token = strtok(NULL, " ");
                        break;
		  case 3:
			strcpy(my_scoreboard[counter].source_reg2, token);
                        index++;
			printf("S2: %s\n", my_scoreboard[counter].source_reg2);
                        token = strtok(NULL, " ");
                        break;
		  default:
			break;
		}
            }
		if( index % 4 == 3)
		{
		  index++;
		}
		counter++;
		printf("How many instructions: %d\n", counter);
        }
    }
        fclose(fp);
        return 0;
}
