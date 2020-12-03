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

static int vals_in_mem[] = {45,12,0,0,10,135,254,127,18,4,55,8,2,98,13,5,233,158, 167};

float load_register(float dest, int offset, int address)
{
    int counter = 0;
    if( offset + address <= 18)
    {
	dest = vals_in_mem[offset + address];
    }
    else if(offset + address > 18)
    {
	counter = offset+address -18;
	dest = vals_in_mem[counter - 1];
    }
    return dest;

}

void store_register(float dest, int offset, int address)
{

}

int main(int argc, char *argv[])
{

    FILE *fp;
    char buf[100];
    int bufsize = BUFSIZ;
    int index = 0;
    char* token;
    struct scoreboard my_scoreboard[bufsize];
    int counter = 0;
    //int vals_in_mem[] = {45,12,0,0,10,135,254,127,18,4,55,8,2,98,13,5,233,158,167};
    int other_vals[] = {45,12,0,0,10,135,254,127,18,4,55,8,2,98,13,5,233,158,167};
    int int_reg[32] = {0};
    float float_reg[32] = {0};


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
			token = strtok(NULL, " ");
			break;
		  case 1:
			strcpy(my_scoreboard[counter].dest_reg, token);
                        index++;
                        token = strtok(NULL, " ");
			break;
		  case 2:
			strcpy(my_scoreboard[counter].source_reg1, token);
                        index++;
                        token = strtok(NULL, " ");
                        break;
		  case 3:
			strcpy(my_scoreboard[counter].source_reg2, token);
                        index++;
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
        }
    }
        fclose(fp);
	int i = 0;
	for(i = 0; i < counter; i++)
	{
		printf("I: %s\n", my_scoreboard[i].instruction);
		printf("D: %s\n", my_scoreboard[i].dest_reg);
		printf("S1: %s\n", my_scoreboard[i].source_reg1);
		printf("S2: %s\n", my_scoreboard[i].source_reg2);
	}


	float_reg[0] = load_register(float_reg[0], 10, 13);
	printf("%f\n", float_reg[0]);
        return 0;
}
