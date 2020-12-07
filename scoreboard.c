#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

typedef struct scoreboard
{
    char inst[10];
    char dest_reg[6];
    char source_reg1[6];
    char source_reg2[6];
    int dest_num;
    int source_num1;
    int source_num2;
    int issue;
    int read;
    int execute;
    int write;
} scoreboard_t;


const int bufsize = BUFSIZ;
static int vals_in_mem[] = {45,12,0,0,10,135,254,127,18,4,55,8,2,98,13,5,233,158, 167};
static int int_reg[32] = {0};
static float float_reg[32] = {0};
static int fp_adder = 0;
static int fp_mul = 0;
static int fp_div = 0;
static int int_calc = 0;

void load_register(int dest, int offset, int address, int reg)
{
    int counter = 0;
    if( offset + address <= 18)
    {
	if(reg == 1) float_reg[dest] = vals_in_mem[offset + address];
	else int_reg[dest] = vals_in_mem[offset + address];
    }
    else if(offset + address > 18)
    {
	counter = offset+address -18;
	if(reg == 1) float_reg[dest] = vals_in_mem[counter - 1];
	else int_reg[dest] = vals_in_mem[counter - 1];
    }
}

void store_register(float dest, int offset, int address)
{
    int counter = 0;
    if( offset + address <= 18)
    {
        vals_in_mem[offset + address] = (int) dest;
    }
    else if(offset + address > 18)
    {
        counter = offset+address -18;
        vals_in_mem[counter - 1] = (int) dest;
    }
}

void add_integer( int dest, int reg1, int reg2)
{
    int_reg[dest] = reg1 + reg2;
}

void add_float( int dest, float reg1, float reg2)
{
    float_reg[dest] = reg1 + reg2;
}

void sub_integer( int dest, int reg1, int reg2)
{
    int_reg[dest] = reg1 - reg2;
}

void sub_float( int dest, float reg1, float reg2)
{
    float_reg[dest] = reg1 - reg2;
}

void mul_float( int dest, float reg1, float reg2)
{
    float_reg[dest] = reg1 * reg2;
}

void div_float( int dest, float reg1, float reg2)
{
    float_reg[dest] = reg1 / reg2;
}

void remove_char(char *string, char garbage)
{
    char *source, * dest;
    for(source = dest = string; *source != '\0'; source++)
    {
	*dest = *source;
	if(*dest != garbage)
	{
		dest++;
	}
    }
    *dest = '\0';
}

void delete_comma(scoreboard_t instructions[], int counter)
{
    int i;
    for( i = 0; i < counter - 1; i++)
    {
       	remove_char(instructions[i].dest_reg, ',');
	remove_char(instructions[i].source_reg1, ',');
    }
}

int calculate( scoreboard_t *instructions)
{
    if(strcmp(instructions->inst, "L.D") == 0)
    {
	int reg;
	if(instructions->dest_reg[0] == 'F')
	{
	    reg = 1;
	}
	else
	{
	    reg = 2;
	}
	char dest[2];
	int i = 1;
	int j = 0;
	char* token;
	while(instructions->dest_reg[i] >= 48 &&
                instructions->dest_reg[i] <= 57)
	{
	    dest[j] = instructions->dest_reg[i];
	    i++;
	    j++;
	}
	token = strtok(instructions->source_reg1, "(");
	char offset[2];
	char address[2];
	strcpy(offset, token);
	token = strtok(NULL, ")");
	strcpy(address, token);
	instructions->dest_num = atoi(dest);
	load_register(atoi(dest), atoi(offset), atoi(address), reg);
	return 0;
    }
    else if(strcmp(instructions->inst, "S.D") == 0)
    {
        int reg;
        if(instructions->dest_reg[0] == 'F')
        {
            reg = 1;
        }
        else
        {
            reg = 2;
        }
	char dest[2];
        int i = 1;
        int j = 0;
        char* token;
        while(instructions->dest_reg[i] >= 48 &&
                instructions->dest_reg[i] <= 57)
        {
            dest[j] = instructions->dest_reg[i];
            i++;
            j++;
        }
        token = strtok(instructions->source_reg1, "(");
        char offset[2];
        char address[2];
        strcpy(offset, token);
        token = strtok(NULL, ")");
        strcpy(address, token);
	instructions->dest_num = atoi(dest);
	if (reg == 1) store_register(float_reg[atoi(dest)],
		atoi(offset), atoi(address));
	else store_register(int_reg[atoi(dest)],
                atoi(offset), atoi(address));
        return 0;
    }
    else if(strcmp(instructions->inst, "ADD") == 0)
    {
	char dest[2];
        int i = 1;
        int j = 0;
        while(instructions->dest_reg[i] >= 48 &&
                instructions->dest_reg[i] <= 57)
        {
            dest[j] = instructions->dest_reg[i];
            i++;
            j++;
        }
	instructions->dest_num = atoi(dest);

	char reg1[2];
        i = 1;
        j = 0;
        while(instructions->source_reg1[i] >= 48 &&
                instructions->source_reg1[i] <= 57)
        {
            reg1[j] = instructions->source_reg1[i];
            i++;
            j++;
        }
        instructions->source_num1 = atoi(reg1);

	char reg2[2];
        i = 1;
        j = 0;
        while(instructions->source_reg2[i] >= 48 &&
		instructions->source_reg2[i] <= 57)
        {
            reg2[j] = instructions->source_reg2[i];
            i++;
            j++;
        }
	instructions->source_num2 = atoi(reg2);

	add_integer(atoi(dest), int_reg[atoi(reg1)], int_reg[atoi(reg2)]);
	return 0;
    }
    else if(strcmp(instructions->inst, "ADDI") == 0)
    {

        char dest[2];
        int i = 1;
        int j = 0;
        while(instructions->dest_reg[i] >= 48 &&
                instructions->dest_reg[i] <= 57)
        {
            dest[j] = instructions->dest_reg[i];
            i++;
            j++;
        }
	instructions->dest_num = atoi(dest);

        char reg1[2];
        i = 1;
        j = 0;
        while(instructions->source_reg1[i] >= 48 &&
                instructions->source_reg1[i] <= 57)
        {
            reg1[j] = instructions->source_reg1[i];
            i++;
            j++;
        }
	instructions->source_num1 = atoi(reg1);

	add_integer(atoi(dest), int_reg[atoi(reg1)], atoi(instructions->source_reg2));
	return 0;
    }
    else if(strcmp(instructions->inst, "ADD.D") == 0)
    {
	char dest[2];
        int i = 1;
        int j = 0;
        while(instructions->dest_reg[i] >= 48 &&
                instructions->dest_reg[i] <= 57)
        {
            dest[j] = instructions->dest_reg[i];
            i++;
            j++;
        }
	instructions->dest_num = atoi(dest);

        char reg1[2];
        i = 1;
        j = 0;
        while(instructions->source_reg1[i] >= 48 &&
                instructions->source_reg1[i] <= 57)
        {
            reg1[j] = instructions->source_reg1[i];
            i++;
            j++;
        }
	instructions->source_num1 = atoi(reg1);

        char reg2[2];
        i = 1;
        j = 0;
        while(instructions->source_reg2[i] >= 48 &&
		instructions->source_reg2[i] <= 57)
        {
            reg2[j] = instructions->source_reg2[i];
            i++;
            j++;
        }
	instructions->source_num2 = atoi(reg2);

        add_float(atoi(dest), float_reg[atoi(reg1)], float_reg[atoi(reg2)]);
	return 0;
    }
    else if(strcmp(instructions->inst, "SUB.D") == 0)
    {
	char dest[2];
        int i = 1;
        int j = 0;
        while(instructions->dest_reg[i] >= 48 &&
                instructions->dest_reg[i] <= 57)
        {
            dest[j] = instructions->dest_reg[i];
            i++;
            j++;
        }
	instructions->dest_num = atoi(dest);

        char reg1[2];
        i = 1;
        j = 0;
        while(instructions->source_reg1[i] >= 48 &&
                instructions->source_reg1[i] <= 57)
        {
            reg1[j] = instructions->source_reg1[i];
            i++;
            j++;
        }
	instructions->source_num1 = atoi(reg1);

       char reg2[2];
        i = 1;
        j = 0;
        while(instructions->source_reg2[i] >= 48 &&
                instructions->source_reg2[i] <= 57)
        {
            reg2[j] = instructions->source_reg2[i];
            i++;
            j++;
        }
	instructions->source_num2 = atoi(reg2);

       	sub_float(atoi(dest), float_reg[atoi(reg1)], float_reg[atoi(reg2)]);
	return 0;
    }
    else if(strcmp(instructions->inst, "SUB") == 0)
    {
        char dest[2];
        int i = 1;
        int j = 0;
        while(instructions->dest_reg[i] >= 48 &&
                instructions->dest_reg[i] <= 57)
        {
            dest[j] = instructions->dest_reg[i];
            i++;
            j++;
        }
	instructions->dest_num = atoi(dest);

        char reg1[2];
        i = 1;
        j = 0;
        while(instructions->source_reg1[i] >= 48 &&
                instructions->source_reg1[i] <= 57)
        {
            reg1[j] = instructions->source_reg1[i];
            i++;
            j++;
        }
	instructions->source_num1 = atoi(reg1);

       char reg2[2];
        i = 1;
        j = 0;
        while(instructions->source_reg2[i] >= 48 &&
                instructions->source_reg2[i] <= 57)
        {
            reg2[j] = instructions->source_reg2[i];
            i++;
            j++;
        }
	instructions->source_num2 = atoi(reg2);

        sub_integer(atoi(dest), int_reg[atoi(reg1)], int_reg[atoi(reg2)]);
	return 0;
    }
    else if(strcmp(instructions->inst, "MUL.D") == 0)
    {
        char dest[2];
        int i = 1;
        int j = 0;
        while(instructions->dest_reg[i] >= 48 &&
                instructions->dest_reg[i] <= 57)
        {
            dest[j] = instructions->dest_reg[i];
            i++;
            j++;
        }
	instructions->dest_num = atoi(dest);

        char reg1[2];
        i = 1;
        j = 0;
        while(instructions->source_reg1[i] >= 48 &&
                instructions->source_reg1[i] <= 57)
        {
            reg1[j] = instructions->source_reg1[i];
            i++;
            j++;
        }
	instructions->source_num1 = atoi(reg1);

       char reg2[2];
        i = 1;
        j = 0;
        while(instructions->source_reg2[i] >= 48 &&
                instructions->source_reg2[i] <= 57)
        {
            reg2[j] = instructions->source_reg2[i];
            i++;
            j++;
        }
	instructions->source_num2 = atoi(reg2);

        mul_float(atoi(dest), float_reg[atoi(reg1)], float_reg[atoi(reg2)]);
	return 0;
    }
    else if(strcmp(instructions->inst, "DIV.D") == 0)
    {
        char dest[2];
        int i = 1;
        int j = 0;
        while(instructions->dest_reg[i] >= 48 &&
                instructions->dest_reg[i] <= 57)
        {
            dest[j] = instructions->dest_reg[i];
            i++;
            j++;
        }
	instructions->dest_num = atoi(dest);

        char reg1[2];
        i = 1;
        j = 0;
        while(instructions->source_reg1[i] >= 48 &&
                instructions->source_reg1[i] <= 57)
        {
            reg1[j] = instructions->source_reg1[i];
            i++;
            j++;
        }
	instructions->source_num1 = atoi(reg1);

       char reg2[2];
        i = 1;
        j = 0;
        while(instructions->source_reg2[i] >= 48 &&
                instructions->source_reg2[i] <= 57)
        {
            reg2[j] = instructions->source_reg2[i];
            i++;
            j++;
        }
	instructions->source_num2 = atoi(reg2);

        div_float(atoi(dest), float_reg[atoi(reg1)], float_reg[atoi(reg2)]);
	return 0;
    }
    else {
    return 1;
    }
}

int scoreboarding(scoreboard_t instructions[], int used_reg[],
	int counter, int score, int cur_inst)
{
    printf("Counter: %d\n",counter);
    printf("Cur inst: %d\n",cur_inst);
    int i;
    int available = 0;

	if ((strcmp(instructions[cur_inst].inst, "L.D") == 0 ||
		strcmp(instructions[cur_inst].inst, "S.D") == 0)
		&& int_calc == 0)
	{
	    printf("On LOad Store\n");
	    int_calc = 1;

	    if(instructions[cur_inst].issue == 0)
            	instructions[cur_inst].issue = score;

	    for( i = 0; i < 100; i++)
	    {
		if(instructions[cur_inst].dest_num == used_reg[i])
		{
		    printf("No on cmp%d: %d %d \n",i, instructions[cur_inst].dest_num, used_reg[i]);
		    return 1;
		}
		i++;
	    }

	    for( i = 0; i < 100; i++)
            {
                if(used_reg[i] == -1)
                {
                    used_reg[i] = instructions[cur_inst].dest_num;
		    printf("On: %d\n",used_reg[i]);
		    break;
                }
                i++;
            }

	    if(counter - 1 > 0)
	    {
		printf("Before loop\n");
		available = scoreboarding(instructions, used_reg, counter - 1,
			score, cur_inst + 1);
	    }

	    instructions[cur_inst].read = ++score;
	    instructions[cur_inst].execute = ++score;
	    instructions[cur_inst].write = ++score;
	    int_calc = 0;
	    score++;

	    for( i = 0; i < 100; i++)
            {
                if(instructions[cur_inst].dest_num == used_reg[i])
                {
                    used_reg[i] = -1;
		    break;
                }
		i++;
            }

	    if(available == 1 && counter - 1 > 0)
	    {
		printf("Yes\n");
		scoreboarding(instructions, used_reg, counter - 1,
			score, cur_inst + 1);
	    }
	}
	else if((strcmp(instructions[cur_inst].inst, "ADD") == 0 ||
                	strcmp(instructions[cur_inst].inst, "SUB") == 0 ||
			strcmp(instructions[cur_inst].inst, "ADDI") == 0)
                	&& int_calc == 0)
	{
	    printf("Counter: %d\n",counter);
	    int_calc = 1;
            instructions[cur_inst].issue = score;

	    if(counter - 1 > 0)
	    {
		available = scoreboarding(instructions, used_reg, counter - 1,
			score, cur_inst + 1);
	    }
	    instructions[cur_inst].read = ++score;
	    instructions[cur_inst].execute = ++score;
	    instructions[cur_inst].write = ++score;
	    int_calc = 0;
	    score++;

	    if(available == 1 && counter - 1 > 0)
	    {
		printf("Yes\n");
		scoreboarding(instructions, used_reg, counter - 1,
			score, cur_inst + 1);
	    }
	}
	else if((strcmp(instructions[cur_inst].inst, "ADD.D") == 0 ||
			strcmp(instructions[cur_inst].inst, "SUB.D") == 0)
                        && fp_adder == 0)
	{
	    printf("Counter on Add.d: %d\n",counter);
	    fp_adder = 1;

	    if(instructions[cur_inst].issue == 0)
            	instructions[cur_inst].issue = ++score;

	    for( i = 0; i < 100; i++)
	    {
		if(instructions[cur_inst].dest_num == used_reg[i])
		{
		    printf("No on dest_reg %d: %d %d\n", i, instructions[cur_inst].dest_num, used_reg[i]);
		    fp_adder = 0;
		    return 1;
		}
		if(instructions[cur_inst].source_num1 == used_reg[i])
                {
                    printf("No on sc1: %d\n", used_reg[i]);
		    fp_adder = 0;
                    return 1;
                }
                if(instructions[cur_inst].source_num2 == used_reg[i])
                {
                    printf("No on sc2: %d %d\n",instructions[cur_inst].source_num2, used_reg[i]);
		    fp_adder = 0;
                    return 1;
                }
		i++;
	    }
	    int count = 0;
	    for( i = 0; i < 100; i++)
            {
                if(used_reg[i] == -1)
                {
                    if( count == 0)
			used_reg[i] = instructions[cur_inst].dest_num;
		    else if( count == 1)
                        used_reg[i] = instructions[cur_inst].source_num1;
		    else if( count == 2)
                        used_reg[i] = instructions[cur_inst].source_num2;
		    else
			break;
		    printf("On: %d\n",used_reg[i]);
                }
                i++;
		count++;
            }

	    if(counter - 1 > 0)
	    {
		printf("Before loop!\n");
		available = scoreboarding(instructions, used_reg, counter - 1,
			score, cur_inst + 1);
	    }

	    instructions[cur_inst].read = score;
	    score+=2;
	    instructions[cur_inst].execute = score;
	    instructions[cur_inst].write = ++score;
	    fp_adder = 0;
	    score++;

	    for( i = 0; i < 100; i++)
            {
                if(instructions[cur_inst].dest_num == used_reg[i])
                {
                    used_reg[i] = -1;
		    printf("No: %d\n",used_reg[i]);
                }
		else if(instructions[cur_inst].source_num1 == used_reg[i])
                {
                    used_reg[i] = -1;
                    printf("No: %d\n",used_reg[i]);
                }
		else if(instructions[cur_inst].source_num2 == used_reg[i])
                {
                    used_reg[i] = -1;
                    printf("No: %d\n",used_reg[i]);
                }
		i++;
            }

	    if(available == 1 && counter - 1 > 0)
	    {
	    	printf("Yes\n");
		scoreboarding(instructions, used_reg, counter - 1,
			score, cur_inst + 1);
	    }
	}
	else if(strcmp(instructions[cur_inst].inst, "MUL.D") == 0
                        && fp_mul == 0)
	{
	    printf("Counter on Mul.d: %d\n",counter);
	    fp_mul = 1;

	    if(instructions[cur_inst].issue == 0)
            	instructions[cur_inst].issue = ++score;

	    for( i = 0; i < 100; i++)
	    {
		if(instructions[cur_inst].dest_num == used_reg[i])
		{
		    printf("No on dest_reg %d: %d %d\n", i, instructions[cur_inst].dest_num, used_reg[i]);
		    fp_mul = 0;
		    return 1;
		}
		if(instructions[cur_inst].source_num1 == used_reg[i])
                {
                    printf("No on sc1: %d\n", used_reg[i]);
		    fp_mul = 0;
                    return 1;
                }
                if(instructions[cur_inst].source_num2 == used_reg[i])
                {
                    printf("No on sc2: %d %d\n",instructions[cur_inst].source_num2, used_reg[i]);
		    fp_mul = 0;
                    return 1;
                }
		i++;
	    }

	    int count = 0;
	    for( i = 0; i < 100; i++)
            {
                if(used_reg[i] == -1)
                {
                    if( count == 0)
			used_reg[i] = instructions[cur_inst].dest_num;
		    else if( count == 1)
                        used_reg[i] = instructions[cur_inst].source_num1;
		    else if( count == 2)
                        used_reg[i] = instructions[cur_inst].source_num2;
		    else
			break;
		    printf("On: %d\n",used_reg[i]);
                }
                i++;
		count++;
            }

	    if(counter - 1 > 0)
	    {
		printf("Before loop!\n");
		available = scoreboarding(instructions, used_reg, counter - 1,
			score, cur_inst + 1);
	    }

	    instructions[cur_inst].read = score;
	    score+=10;
	    instructions[cur_inst].execute = score;
	    instructions[cur_inst].write = ++score;
	    fp_mul = 0;
	    score++;

	    for( i = 0; i < 100; i++)
            {
                if(instructions[cur_inst].dest_num == used_reg[i])
                {
                    used_reg[i] = -1;
		    printf("No: %d\n",used_reg[i]);
                }
		else if(instructions[cur_inst].source_num1 == used_reg[i])
                {
                    used_reg[i] = -1;
                    printf("No: %d\n",used_reg[i]);
                }
		else if(instructions[cur_inst].source_num2 == used_reg[i])
                {
                    used_reg[i] = -1;
                    printf("No: %d\n",used_reg[i]);
                }
		i++;
            }

	    if(available == 1 && counter - 1 > 0)
	    {
	    	printf("Yes\n");
		scoreboarding(instructions, used_reg, counter - 1,
			score, cur_inst + 1);
	    }
	}
	else if(strcmp(instructions[cur_inst].inst, "DIV.D") == 0
                        && fp_div == 0)
	{
	    printf("Counter on div.d: %d\n",counter);
	    fp_div = 1;

	    if(instructions[cur_inst].issue == 0)
            	instructions[cur_inst].issue = ++score;

	    for( i = 0; i < 100; i++)
	    {
		if(instructions[cur_inst].dest_num == used_reg[i])
		{
		    printf("No on dest_reg %d: %d %d\n", i, instructions[cur_inst].dest_num, used_reg[i]);
		    fp_div = 0;
		    return 1;
		}
		if(instructions[cur_inst].source_num1 == used_reg[i])
                {
                    printf("No on sc1: %d\n", used_reg[i]);
		    fp_div = 0;
                    return 1;
                }
                if(instructions[cur_inst].source_num2 == used_reg[i])
                {
                    printf("No on sc2: %d %d\n",instructions[cur_inst].source_num2, used_reg[i]);
		    fp_div = 0;
                    return 1;
                }
		i++;
	    }
	    int count = 0;
	    for( i = 0; i < 100; i++)
            {
                if(used_reg[i] == -1)
                {
                    if( count == 0)
			used_reg[i] = instructions[cur_inst].dest_num;
		    else if( count == 1)
                        used_reg[i] = instructions[cur_inst].source_num1;
		    else if( count == 2)
                        used_reg[i] = instructions[cur_inst].source_num2;
		    else
			break;
		    printf("On: %d\n",used_reg[i]);
                }
                i++;
		count++;
            }

	    if(counter - 1 > 0)
	    {
		printf("Before loop!\n");
		available = scoreboarding(instructions, used_reg, counter - 1,
			score, cur_inst + 1);
	    }

	    instructions[cur_inst].read = score;
	    score+=40;
	    instructions[cur_inst].execute = score;
	    instructions[cur_inst].write = ++score;
	    fp_div = 0;
	    score++;

	    for( i = 0; i < 100; i++)
            {
                if(instructions[cur_inst].dest_num == used_reg[i])
                {
                    used_reg[i] = -1;
		    printf("No: %d\n",used_reg[i]);
                }
		else if(instructions[cur_inst].source_num1 == used_reg[i])
                {
                    used_reg[i] = -1;
                    printf("No: %d\n",used_reg[i]);
                }
		else if(instructions[cur_inst].source_num2 == used_reg[i])
                {
                    used_reg[i] = -1;
                    printf("No: %d\n",used_reg[i]);
                }
		i++;
            }

	    if(available == 1 && counter - 1 > 0)
	    {
	    	printf("Yes\n");
		scoreboarding(instructions, used_reg, counter - 1,
			score, cur_inst + 1);
	    }
	}
	else {
	    printf("Invalid\n");
	    return 1; }
    return 0;
    printf("Goes here\n");
}

int main()
{

    FILE *fp;
    char buf[100];
    int index = 0;
    char* token;
    int counter = 0;
    scoreboard_t my_scoreboard[bufsize];
    //int other_vals[] = {45,12,0,0,10,135,254,127,18,4,55,8,2,98,13,5,233,158,167};

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
            token = strtok(buf, " ");
	    while(token != NULL)
            {
		int num_mod = index % 4;
		switch(num_mod)
		{
		  case 0:
			strcpy(my_scoreboard[counter].inst, token);
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

    delete_comma(my_scoreboard, counter);

           int p;
        for(p = 0; p < counter - 1; p++)
        {
                calculate(&my_scoreboard[p]);
        }
    int score = 1;
    int cur_inst = 0;
    int used_reg[100];
    for(p = 0; p < 100; p++)
    {
        used_reg[p] = -1;
    }
    scoreboarding(my_scoreboard, used_reg, counter - 1, score, cur_inst);

	int i = 0;
	for(i = 0; i < counter - 1; i++)
	{
		printf("I: %s\n", my_scoreboard[i].inst);
		printf("D: %s\n", my_scoreboard[i].dest_reg);
		printf("S1: %s\n", my_scoreboard[i].source_reg1);
		printf("S2: %s\n", my_scoreboard[i].source_reg2);
		printf("DNUM: %d\n", my_scoreboard[i].dest_num);
                printf("S1NUM: %d\n", my_scoreboard[i].source_num1);
                printf("S2NUM: %d\n", my_scoreboard[i].source_num2);

	}

        printf("Float\n");
        for(p = 0; p < 32; p++)
        {
                printf("%f\n", float_reg[p]);
        }

        for(i = 0; i < counter - 1; i++)
        {
              printf("I: %d\n", my_scoreboard[i].issue);
              printf("R: %d\n", my_scoreboard[i].read);
              printf("E: %d\n", my_scoreboard[i].execute);
              printf("W: %d\n\n", my_scoreboard[i].write);
        }

        //printf("Int\n");
        //for(p = 0; p < 32; p++)
        //{
        //        printf("%d\n", int_reg[p]);
       // }


	return 0;
}
