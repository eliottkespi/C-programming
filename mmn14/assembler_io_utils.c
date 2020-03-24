#include "assembler.h"

/* is_data checks if the string given as parameter is from directive type data */
int is_data(char str[])
{
	if (strcmp(str, DATA_DIRECTIVE_TEXT))
		return 0;
	return 1;
}

/* is_string checks if the string given as parameter is from directive type string */
int is_string(char str[])
{
	if (strcmp(str, STRING_DIRECTIVE_TEXT))
		return 0;
	return 1;
}

/* is_entry checks if the string given as parameter is from directive type entry */
int is_entry(char str[])
{
	if (strcmp(str, ENTRY_DIRECTIVE_TEXT))
		return 0;
	return 1;
}

/* is_extern checks if the string given as parameter is from directive type external */
int is_extern(char str[])
{
	if (strcmp(str, EXTERN_DIRECTIVE_TEXT))
		return 0;
	return 1;
}

/* is_directive checks if the string given as parameter is from any directive type */
int is_directive(char str[])
{
	if ((is_data(str)) || (is_string(str)) || (is_entry(str)) || (is_extern(str)))
		return 1;
	return 0;
}

/* is_label checks if the string given as parameter have the label format */
int is_label(char str[])
{
	int len = 0;
	/* label start with a letter */
	if (!(isalpha(str[ZERO])))
		return 0;

	/* label is not more than MAX_SYMBOL_LENGTH long */
	if ((len = strlen(str)) >= (MAX_SYMBOL_LENGTH +2)) /* +2 for the ":" and the \0 end of string */
		return 0;

	/* label declaration finished by ":" */
	if (str[len -1] != ':') 
		return 0;
	
	return 1;
}

/* is_valid_label checks if the string given as a label is valid
	this function is triming the last character of the label declaration - ':'
*/
int is_valid_label(char str[])
{
	/* trunckating the last character (':') to keep the label itself - if any*/
	if ((str[strlen(str)-1] == ':') || (str[strlen(str)-1] == '\n'))
		str[strlen(str)-1] = '\0';

	/* valid label is not already initialized */
	if (is_a_symbol(str))
		return ZERO;

	/* valid label is not a register name */
	if (is_a_register(str))
		return ZERO;

	/* valid label is not an operation name */
	if (is_an_operation_name(str))
		return ZERO;
	
	return ONE;
}

/* check if the given string is a register name (r0, r1, ... , r7) */
int is_a_register(char str[])
{
	if (str != NULL)
		if (str[ZERO] == 'r')
			if ((str[1] == '0') ||
			(str[1] == '1') ||
			(str[1] == '2') ||
			(str[1] == '3') ||
			(str[1] == '4') ||
			(str[1] == '5') ||
			(str[1] == '6') ||
			(str[1] == '7'))
				if (str[2] == '\0')
					return ONE;
	return ZERO;
}

/* checks if the given string is a number (#2 or #87...) */
int is_a_number(char str[])
{
	if (str != NULL)
		if (str[ZERO] == '#')
			return ONE;
	return ZERO;
}

/* check if the given string is a valid list of elements means int values sperated by ','
	return the number of elements found 
	if error encountered - return -1
*/
int validate_list_of_elements(char str[])
{
	int elements_found = ZERO;
	int i = ZERO;
	int num_started = NO;
	
	/*iterate the whole string*/
	for(i = ZERO; str[i] != '\0'; i++)
	{
		/*if string start with '+' or '-'*/
		if((str[i] == '+') ||  (str[i] == '-'))
		{
			if (num_started == YES)
			{
				elements_found = -1;
				break;
			}
			num_started = YES;
		}
		
		/*if number only*/
		else if(isdigit(str[i]))
			num_started = YES;
		
		/*if new element*/
		else if (str[i] == ',')
		{
			if (num_started == NO) /* 2 ',' in a row case */
			{
				elements_found = -1;
				break;
			}

			/* new element found */
			elements_found++;
			num_started = NO;
		}
		
		/*invalid elements*/
		else
		{
			elements_found = -1;
			break;
		}
	}
	return ++elements_found; /* +1 for the last element between the last ',' and the '\0' */
}

/* remove spaces from the given string */
void remove_spaces(char *  str_trimmed)
{
	/* declare initialize and copy str_trimmed into str_untrimmed + ONE For '\0'*/
	char * str_untrimmed = (char *) malloc(strlen(str_trimmed) + ONE);
	char * str_head_pointer = str_untrimmed;
	strcpy(str_untrimmed, str_trimmed);

	while (*str_untrimmed != '\0')
	{
		if(!isspace(*str_untrimmed))
		{
			*str_trimmed = *str_untrimmed;
			str_trimmed++;
		}
		str_untrimmed++;
	}
	*str_trimmed = '\0';
	str_untrimmed = str_head_pointer;

	/* the backed up str is kept into sre_untrimmed
	   if need to be use in future implementation, need to remove the free */
	free(&*str_untrimmed);
}

/* validate_instruction check the validity of the instruction and retrun the number of word used for this instruction 
	return -1 if invalid instruction */
int count_instruction_words(char str[])
{
	int num_of_operand_expected = -1;
	int i;
	char * chunk_of_line;
	
	
	/* catch the first word of the instruction */	
	chunk_of_line = strtok(str, " ");

	for(i = ZERO; i < NUM_OF_OPCODE && num_of_operand_expected != ZERO; i++)
	{
		if (strcmp(opcodes_table[i].name, chunk_of_line) == ZERO)
		{
			num_of_operand_expected = opcodes_table[i].operand_number_required +1; /* +1 for the operation used */
			break;
		}
	}

	return num_of_operand_expected;
}

/* is_blank checks if the string given as parameter is composed by whitespaces only */
int is_blank(char str[])
{
	int i;

	if (str == NULL)
		return ONE;

	for(i = ZERO; i < strlen(str) ; i++)
		if ((str[i] != ' ') && (str[i] != '\t') && (str[i] != '\n'))
			return ZERO;
	return ONE;
}

/* is_comment checks if the string given as parameter starts by a ';' */
int is_comment(char str[])
{
	int i;

	for(i = ZERO; i < strlen(str) ; i++) 
		if ((str[i] == ' ') || (str[i] == '\t') || (str[i] == '\n'))
			; /* do nothing */
		else if (str[i] == ';')
			return ONE;
		else
			return ZERO;
	return ZERO;
}

/* remove the first spaces from a given string */
char * remove_leading_spaces(char * str)
{
	while(isspace(* str))
		str++;
	return str;
}

/* checks if the given string is a number */
int is_valid_number(char * str)
{
	int i=ZERO;
	/*checks if the number starts with '#'*/
	if(str[ZERO]=='#')
		i++;
	else
		return ZERO;
	/*if the number starts with '-', ignore it*/
	if(str[i]=='-')
			i++;
	
	/*for each character of the string, checks if it's a digit*/
	for (; i<strlen(str); i++)	
		if (!isdigit(str[i]))
			return ZERO;
	return ONE;
}

/* checks if the given number (as a string) is in the limits of 12bit number */
int is_num_in_limit(char * str)
{	
	short int num;
	/*Seperates the number from the '#'*/
	num = (short int)atoi(strtok(str,"#"));
	
	/*checks if the number in limits of 12 bin number*/
	if((num<=MAX_12BIT_NUM)&&(num>=MIN_12BIT_NUM))
		return ONE;
	return ZERO;
}
