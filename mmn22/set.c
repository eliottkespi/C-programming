#include "set.h"

void read_numbers_to_set_name()
{
	char cmd_from_stdin[MAX_COMMAND_LENGTH];
	char* token;
	int i, to_be_inserted;

	if (scanf("%s", cmd_from_stdin) == 1)
	{
		/* looking for the index of the requested set */
		for ( i = 0 ; i< NUMBER_OF_SETS ;  i++ )
		{
			if (strcmp(cmd_from_stdin, sets[i].set_name) == 0)
				break;
		}
		
		/* set name not found */
		if ( i == NUMBER_OF_SETS )
			printf("ERROR - Undefined set name: %s\n", cmd_from_stdin);
		else /* set name found */
		{
			if (scanf(" %[^\n]s", cmd_from_stdin) == 1)
			{
				if (validate_list_of_elements(cmd_from_stdin) >= 0)
				{
					printf("!!!!!!!!!!! start inserting\n");
					/*backup_set_value = sets[i].set_values	*/

					/*token = strtok(cmd_from_stdin, ","); 
					while (token != NULL) 
					{ 
					        printf("%d\n", to_be_inserted); 
					        token = strtok(NULL, ","); 
					}
	*/
				}
			}
			else
				printf("ERROR - Can't read from stdin\n"); 
		}
	}
	else
		printf("ERROR - Can't read from stdin\n");
}

void print_set()
{
	printf("print_set");
}

void union_set()
{
	printf("union_set");
}

void intersection_set()
{
	printf("intersection_set");
}

void substract_from_set()
{
	printf("substract_from_set");
}

void symetric_difference_set()
{
	printf("symetric_difference_set");
}

/* stop the program */
void stop()
{
	printf("Exiting ...\n");
	exit(1);
}

/* print help */ 
void help()
{
	int i;

	printf("%sHelp%s\n", BAR, BAR);
	for ( i = 0 ; cmds[i].command_function != NULL ; i++ )
		printf("Command: %s\nDescription: %s\nUsage: %s\n\n", cmds[i].command_name, cmds[i].description, cmds[i].usage);
}
