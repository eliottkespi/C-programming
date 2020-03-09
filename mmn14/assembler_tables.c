#include "assembler.h"

/* operation code table initialization */
opcode opcodes_table[NUM_OF_OPCODE] = {
	{0, "mov", 2},
	{1, "cmp", 2},
	{2, "add", 2},
	{3, "sub", 2},
	{4, "lea", 2},
	{5, "clr", 1},
	{6, "not", 1},
	{7, "inc", 1},
	{8, "dec", 1},
	{9, "jmp", 1},
	{10, "bne", 1},
	{11, "red", 1},
	{12, "prn", 1},
	{13, "jsr", 1},
	{14, "rts", 0},
	{15, "stop", 0}
	};

/* symbol table initialization */
symbol * symbol_table = NULL;


/* add entry the the symbol table
	return 0 if succeeded     */
int add_to_symbol_table(char * name, int value, symbol_type type, symbol_location location)
{
	/* new symbol dynamic allocation */
	symbol * new_symbol = (symbol *)malloc(sizeof(symbol));
	char * new_name = (char *)malloc(sizeof(char) * strlen(name));

	if ((!(new_symbol)) || (!(new_name))) 
	{
		_ERROR(1, ALLOCATION_ERROR);
		return -1;
	}
	
	/* checking that the no other symbol exists with same name */
	if (is_a_symbol(name))
		return -2;

	/* keeping name into a dynamicaly allocated array of char */
	strcpy(new_name, name);

	/* initialization of the new_symbol */
	new_symbol->name = new_name;
	new_symbol->value = value;
	new_symbol->type = type;
	new_symbol->location = location;
	new_symbol->next = NULL;

	/* choosing the next based on file the table is empty or not */
	if (symbol_table == NULL)
		symbol_table = new_symbol;
	else
	{
		new_symbol->next = symbol_table;
		symbol_table = new_symbol;
	}
	return 0;
}

/* print the symbol table */
void print_symbol_table()
{
	symbol * sym = symbol_table;
	char buffer[MAX_SYMBOL_LENGTH +1]; /* +1 for \0 - value of the end of array of char */

	_DEBUG(1, LINE);
	_DEBUG(1, PRINT_SYM_TABLE);
	_DEBUG(1, LINE);
	while (sym != NULL)
	{
		_DEBUG(2, "Name:\t\t", sym->name);

		sprintf(buffer, "%d", sym->value);
		_DEBUG(2, "Value:\t\t", buffer);

		sprintf(buffer, "%d", sym->type);
		_DEBUG(2, "Type:\t\t", buffer);

		sprintf(buffer, "%d", sym->location);
		_DEBUG(2, "Location:\t", buffer);
		_DEBUG(1, LINE);

		sym = sym->next;
	}
}

/* free the dynamically allocated symbol table */
void free_symbol_table()
{
	symbol * sym;

	while(symbol_table)
	{
		sym = symbol_table;
		symbol_table = symbol_table->next;
		free(sym->name);
		free(sym);
	}
}

/* check if the given string is configured as name of a symbol */
int is_a_symbol(char str[])
{
	symbol * sym = symbol_table;

	while (sym != NULL) 
	{
		if ((strcmp(sym->name, str)) == ZERO)
			return -1;
		sym = sym->next;
	}
	return 0;	
}

/* check if the given string is an operation name */
int is_an_operation_name(char str[])
{
	int i;
	
	for(i = 0;i < NUM_OF_OPCODE ; i++)
	{
		if(strcmp(opcodes_table[i].name, str) == ZERO)
			return -1;
	}
	return 0;
}

/* add_IC_to_data_symbol is the last step of the first run
it consist by adding the counter IC to every data address stored in the symbol table
*/
void add_IC_to_data_symbol()
{
	symbol * sym = symbol_table;

	while (sym != NULL) 
	{
		if (sym->type == DATA)
			sym->value += IC;
		sym = sym->next;
	}
}
