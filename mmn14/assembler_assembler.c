#include "assembler.h"

/* do_assembler is assembling the assembly file parameter and is creating 3 output files
	*.ob
	*.int
	*.ext
*/
void do_assembler(FILE * fd_input)
{
	int error_counter = 0;

	/* images/table (re)initialization */
	instruction_image = NULL;
	data_image = NULL;
	symbol_table = NULL;

	/* counters (re)initialization */
	IC = 100; /* instruction counter */
	DC = 0; /* data counter */

	/* do run 1 run 2 */
	error_counter += do_first_run(fd_input);

	print_symbol_table();

	/* print into files */
	
	/* free images/table */
	free_image(INSTRUCTION_TABLE_TYPE);
	free_image(DATA_TABLE_TYPE);
	free_symbol_table();
}


/* first run is building the symbol table and calculate how many memory will be required
	the number of error found will be returned
	if no error - return 0 
*/
int do_first_run(FILE * fd_input)
{
	int error_counter = 0, i = 0;
	int in_error;
	int instruction_words = 0;
	int current_line_number = 0;
	char line[MAX_LINE_LENGTH];
	char * chunk_of_line;

	/* read the file line by line entil EOF */
	while(fgets(line, MAX_LINE_LENGTH, fd_input))
	{
		/* remove last character if it is new line */
		while(line[i] != '\0')
		{
			if (line[i] == '\n')
			{
				line[i] = '\0';
				break;
			}
			i++;
		}
 
		/* in_error allow us to know if doing anything with line or not */
		in_error = NO;
		
		/* new line - increasing line number by 1 */			
		current_line_number++;

		/* the first word of the line in chunk_of_line */
		chunk_of_line = strtok(line, " ");

		/* label case */
		if (is_label(chunk_of_line)) /* in this case, chunk_of_line is the first word */
		{
			char * label_name;
			
			/* validate label validity or go to next line */
			if(!is_valid_label(chunk_of_line))
			{
				sprintf(line_number_buffer, "%d", current_line_number);
				_ERROR(5, INVALID_LABEL_NAME, "-", current_filename, ":", line_number_buffer );
				error_counter++;
				in_error = YES;
			}

			/* keep the label name in label_name */
			label_name = chunk_of_line;

			/* the second word of the line in chunk_of_line */
			chunk_of_line = strtok(NULL, " ");

			/* label directive case */
			if (is_directive(chunk_of_line))
			{
				/* data directive label case */
				if (is_data(chunk_of_line))
				{
					int number_of_elements = 0;

					/* the rest of the line in chunk_of_line */
					chunk_of_line = strtok(NULL, "\0");
					
					/* remove spaces from the list */
					remove_spaces(chunk_of_line);

					if((number_of_elements = validate_list_of_elements(chunk_of_line)) <= ZERO)
					{	
						sprintf(line_number_buffer, "%d", current_line_number);
						_ERROR(5, INVALID_LIST_DATA, "-", current_filename, ":", line_number_buffer );
						error_counter++;
						in_error = YES;
					}
					/* checking if error encountered - if yes, no need to move forward with symbol table and image table additions */
					if(in_error == NO)
					{
						if (add_to_image(DATA_TABLE_TYPE, DC, "code") == ZERO) /* NEED TO ADD SUPPORT FOR MULTIPLE DATA'S */
						{
							if (add_to_symbol_table(label_name, DC, NONE, UNKNOWN) == ZERO)
							{
								_DEBUG(2, "New data symbol registred", label_name);
								DC += number_of_elements;
							}
							else
							{
								sprintf(line_number_buffer, "%d", current_line_number);
								_ERROR(5, CANT_ADD_TO_SYMTABLE, "-", current_filename, ":", line_number_buffer );
								error_counter++;
							}
						}
						else
						{
							sprintf(line_number_buffer, "%d", current_line_number);
							_ERROR(5, CANT_ADD_TO_DATA_IMAGE, "-", current_filename, ":", line_number_buffer );
							error_counter++;
						}
					}
				}
				/* string directive label case */
				else if (is_string(chunk_of_line))
				{
					int i;
					char * start_of_string = NULL, * end_of_string = NULL;

					/* the rest of the line in chunk_of_line */
					chunk_of_line = strtok(NULL, "\0");

					if (chunk_of_line[strlen(chunk_of_line) -1] == '\n')
						chunk_of_line[strlen(chunk_of_line) -1] = '\0';

					/* iterating over every character */
					for(i = 0 ; i < strlen(chunk_of_line) ; i++)
					{
						/* defining beginning and end of the string */
						if (chunk_of_line[i] == '"')
						{
							if (!start_of_string)
								start_of_string = &chunk_of_line[i];
							else
								end_of_string = &chunk_of_line[i];
						}
						
						/* what comes after the last double quote will be ignored */
						else if ((start_of_string) && (end_of_string))
						{
							sprintf(line_number_buffer, "%d", current_line_number);
							_WARNING(5, TEXT_AFTER_END_OF_STR_NOT_RELEVANT, "-", current_filename, ":", line_number_buffer);
							break;
						}	
					}
					/* invalid line - cant find a string between double quotes */
					if ((!(start_of_string)) || (!(end_of_string))) 
					{
						sprintf(line_number_buffer, "%d", current_line_number);
						_ERROR(5, CANT_FIND_STRING, "-", current_filename, ":", line_number_buffer );
						error_counter++;
						in_error = YES;
					}

					/* checking if error encountered - if yes, no need to move forward with symbol table and image table additions */
					if(in_error == NO)
					{
						if (add_to_image(DATA_TABLE_TYPE, DC, "code") == ZERO) /* NEED TO ADD SUPPORT FOR MULTIPLE DATA'S */
						{
							if (add_to_symbol_table(label_name, DC, NONE, UNKNOWN) == ZERO)
							{
								DC += (end_of_string - start_of_string +1 -2); /* +1 for the '\0' to be added AND -2 for the the 2 '"'*/
								_DEBUG(2, "New data symbol registred", label_name);
							}
							else
							{
								sprintf(line_number_buffer, "%d", current_line_number);
								_ERROR(5, CANT_ADD_TO_SYMTABLE, "-", current_filename, ":", line_number_buffer );
								error_counter++;
							}
						}
						else
						{
							sprintf(line_number_buffer, "%d", current_line_number);
							_ERROR(5, CANT_ADD_TO_DATA_IMAGE, "-", current_filename, ":", line_number_buffer );
							error_counter++;
						}
					}
				}
				/* entry directive label case - need to warn the user (in second run) about not taking the label into consideration */
				else if (is_entry(chunk_of_line))
				{
					; /* nothing to do for first run, but need to take care of it in second run */
				}
				/* extern directive label case - need to warn the user (in second run) about not taking the label into consideration */
				else if (is_extern(chunk_of_line))
				{
					/* next word is label name */
					chunk_of_line = strtok(NULL, " ");

					if ((!(is_a_symbol(chunk_of_line))) && (is_valid_label(chunk_of_line)))
					{
						if (add_to_symbol_table(chunk_of_line, ZERO, NONE, EXTERNAL) == ZERO)
						{
							_DEBUG(2, "New external symbol registered", chunk_of_line);
						}
					}
					else
					{
						sprintf(line_number_buffer, "%d", current_line_number);
						_ERROR(5, ALREADY_INITIALIZED_LABEL, "-", current_filename, ":", line_number_buffer );
						error_counter++;
					}
				}
				/* invalid syntax */
				else
				{
					sprintf(line_number_buffer, "%d", current_line_number);
					 _ERROR(5, INVALID_LABEL_DIRECTIVE, "-", current_filename, ":", line_number_buffer );
					error_counter++;
				}
			}
			else /* label instruction case */
			{
				if((instruction_words = count_instruction_words(chunk_of_line)) < ZERO)
				{
					sprintf(line_number_buffer, "%d", current_line_number);
					 _ERROR(5, INVALID_INSTRUCTION, "-", current_filename, ":", line_number_buffer );
					error_counter++;
					in_error = YES;
				}
				/* checking if error encountered - if yes, no need to move forward with symbol table and image table additions */
				if(in_error == NO)
				{
					if (add_to_symbol_table(label_name, IC, CODE, UNKNOWN) == ZERO)
					{
						IC += instruction_words;
						_DEBUG(2, "New data symbol registred", label_name);
					}
					else
					{
						sprintf(line_number_buffer, "%d", current_line_number);
						 _ERROR(5, CANT_ADD_TO_SYMTABLE, "-", current_filename, ":", line_number_buffer );
						error_counter++;
					}
				}
				
			}
		}
		else if (is_extern(chunk_of_line)) /* not a label - need to check if extern */
		{
			/* next word is label name */
			chunk_of_line = strtok(NULL, " ");

			if ((!(is_a_symbol(chunk_of_line))) && (is_valid_label(chunk_of_line)))
			{
				if (add_to_symbol_table(chunk_of_line, ZERO, NONE, EXTERNAL) == ZERO)
				{
					_DEBUG(2, "New external symbol registered", chunk_of_line);
				}
			}
			else
			{
				sprintf(line_number_buffer, "%d", current_line_number);
				 _ERROR(5, ALREADY_INITIALIZED_LABEL, "-", current_filename, ":", line_number_buffer );
				error_counter++;
			}
		}
		else if ((instruction_words = count_instruction_words(chunk_of_line)) >= ZERO)
		{
			IC += instruction_words;
		}
		else /* not a valid start of line */
		{
			sprintf(line_number_buffer, "%d", current_line_number);
			 _ERROR(5, LINE_NOT_UNDERSTANDABLE, "-", current_filename, ":", line_number_buffer );
			error_counter++;
		}
	} /* end of while loop - line */
	return error_counter;
}
