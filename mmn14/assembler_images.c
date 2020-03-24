#include "assembler.h"

/* images initialization */
image_entry * instruction_image = NULL;
image_entry * data_image = NULL;

/* add entry an image - return 0 if succeeded     */
int add_to_image(image_type type,  int address, short int code) 
{
	/* new image entry dynamic allocation */
	image_entry * new_entry = (image_entry *)malloc(sizeof(image_entry));

	if ((!new_entry))
	{
		_ERROR(1, ALLOCATION_ERROR);
		return -1;
	}

	/* initialization of the new_entry */
	new_entry->address = address;
	new_entry->code = code;
	new_entry->next = NULL;
	
	

	/* choosing the next based on file the table is empty or not */
	if (type == INSTRUCTION_TABLE_TYPE)
	{
		if (instruction_image == NULL)
			instruction_image = new_entry;
		else
		{	
			image_entry * p = instruction_image;
			while (p->next != NULL)
				p = p->next;
			p->next = new_entry;
		}
	} 
	else /* (type == DATA_TABLE_TYPE) */
	{
		if (data_image == NULL)
			data_image = new_entry;
		else
		{	
			image_entry * p = data_image;
			while (p->next != NULL)
				p = p->next;
			p->next = new_entry;
		}
	}
	return ZERO;
}

/* free the dynamically allocated image*/
void free_image(image_type type)
{
	image_entry * entry;

	if (type == INSTRUCTION_TABLE_TYPE)
	{
		while(instruction_image)
		{
			entry = instruction_image;
			instruction_image = instruction_image->next;
			free(entry);
		}
	}
	else /* (type == DATA_TABLE_TYPE) */
	{
		while(data_image)
		{
			entry = data_image;
			data_image = data_image->next;
			free(entry);
		}
	}
}

/* print the symbol table */
void print_image(image_type type)
{
       image_entry * entry;
	
       char buffer1[MAX_ADDRESS_SIZE]; 
	char buffer2[MAX_DATA_SIZE];
	unsigned short int mask=MAX_15BIT_VAL;

	if (type == INSTRUCTION_TABLE_TYPE)
	{
		entry = instruction_image;
        	_DEBUG(1, PRINT_INSTRUCTION_TABLE);

		while (entry != NULL)
		{
			sprintf(buffer1, "%d", entry->address);
			/*using mask to avoid using the 16-bit, using casting to unsigned var to avoiding using a negative number*/
			sprintf(buffer2, "%05o", ((unsigned short int) entry->code&mask));
			_DEBUG(2, buffer1, buffer2);
			_DEBUG(1, LINE);
			entry = entry->next;
		}
	}
	else /* (type == DATA_TABLE_TYPE) */
	{
		entry = data_image; 
		_DEBUG(1, PRINT_CODE_IMAGE);
		while (entry != NULL)
		{
			sprintf(buffer1, "%d", entry->address);
			/*using mask to avoid using the 16-bit, using casting to unsigned var to avoiding using a negative number*/
			sprintf(buffer2, "%05o", ((unsigned short int) entry->code&mask));
			_DEBUG(2, buffer1, buffer2);
			_DEBUG(1, LINE);
			entry = entry->next;
		}
	}

        _DEBUG(1, LINE);
	

}

/* adding the counter IC to every data address stored in the data image*/
void add_IC_to_data_image()
{
	image_entry * dim = data_image;

	while (dim != NULL) 
	{
		dim->address = dim->address + IC;
		dim = dim->next;
	}

}


/*BMC = Binary Machine Code*/

/*Builds BMC in case that both operands are Registers*/
void bmc_for_two_op_two_reg(short int opcode, char first_op[],short int first_add, char second_op[],short int second_add)
{
	short int mask=ZERO;
	short int operand_code=ZERO;
	
	/*moves 4 bits to the left - for the next addressing*/
	opcode = opcode << MOVE_BITS_OF_OP;
	
	/*switch case for the addressing of the first operand*/
	switch (first_add)
	{
		/*addressing number 2*/
		case 2:
			mask = MASK_FOR_ADD_2;
			/*adds addresing 2 to the opcode*/
			opcode = opcode | mask;
			/*moves 4 bits to the left - for the next addressing*/
			opcode = opcode << MOVE_BITS_OF_OP;
			/*Seperates the number from the Register name - (for exmple *r'7')  */
			mask = (short int)(first_op[2])-'0';
			/*adds the register number to the operand_code*/
			operand_code=operand_code|mask;
			/*moves 3 bits to the left - for the next register*/
			operand_code = operand_code << MOVE_BITS_OF_REG;
       	break;

		/*addressing number 3*/
		case 3:
			mask = MASK_FOR_ADD_3;
			/*adds addresing 3 to the opcode*/
			opcode = opcode | mask;
			/*moves 4 bits to the left - for the next addressing*/
			opcode = opcode << MOVE_BITS_OF_OP;
			/*Seperates the number from the Register name - (for exmple r'7') */
			mask = (short int)(first_op[1])-'0';
			/*adds the register number to the operand_code*/
			operand_code=operand_code|mask;
			/*moves 3 bits to the left - for the next register*/
			operand_code = operand_code << MOVE_BITS_OF_REG;
		break;
    		default:
		break;
	}
	
	/*switch case for the addressing of the second operand*/
	switch (second_add)
	{
		/*addressing number 2*/
		case 2:
			mask = MASK_FOR_ADD_2;
			/*adds addresing 2 to the opcode*/
			opcode = opcode | mask;
			/*moves 3 bits to the left - for the ARE bits*/
			opcode = opcode << MOVE_BITS_OF_ARE;
			/*Seperates the number from the Register name - (for exmple *r'7')  */
			mask = (short int)(second_op[2])-'0';
			/*adds the register number to the operand_code*/
			operand_code=operand_code|mask;
			/*moves 3 bits to the left - for the ARE bits*/
			operand_code = operand_code << MOVE_BITS_OF_ARE;
       	break;
		
		/*addressing number 3*/
		case 3:
			mask = MASK_FOR_ADD_3;
			/*adds addresing 3 to the opcode*/
			opcode = opcode | mask;
			/*moves 3 bits to the left - for the ARE bits*/
			opcode = opcode << MOVE_BITS_OF_ARE;
			/*Seperates the number from the Register name - (for exmple r'7')  */
			mask = (short int)(second_op[1])-'0';
			/*adds the register number to the operand_code*/
			operand_code=operand_code|mask;
			/*moves 3 bits to the left - for the ARE bits*/
			operand_code = operand_code << MOVE_BITS_OF_ARE;
		break;
    		default:
		break;
	}
	
	/*adds ARE bits to the opcode*/
	opcode = opcode | MASK_FOR_ARE_A;
	/*adds ARE bits to the operand_code*/
	operand_code = operand_code | MASK_FOR_ARE_A;
	
	/*Adding the operation BMC to Instruction Table*/
	add_to_image(INSTRUCTION_TABLE_TYPE , IC, opcode);
	/*Adding the Registers BMC to Instruction Table*/
	add_to_image(INSTRUCTION_TABLE_TYPE , ++IC, operand_code);
	IC++;
}

/*Builds BMC in case that one of the operands is a Register*/
void bmc_for_two_op_one_reg(short int opcode, char first_op[], short int address, char second_op[], int flag, int line_counter)
{

	short int mask=ZERO;
	short int reg_operand_code=ZERO;
	short int non_reg_operand_code=ZERO;

	/*moves 4 bits to the left - for the next addressing bits*/
	opcode = opcode << MOVE_BITS_OF_OP;

	/*checks if the Register is used as a Source operand*/
	if(flag==SRC_FLAG)
	{
		/*switch case for the addressing of the first operand*/
		switch (address)
		{


			/*addressing number 2*/
			case 2:
				mask = MASK_FOR_ADD_2;
				/*adds addresing 2 to the opcode*/
				opcode = opcode | mask;
				/*moves 4 bits to the left - for the next addressing*/
				opcode = opcode << MOVE_BITS_OF_OP;
				/*Seperates the number from the Register name*/
				mask=(short int)(first_op[2])-'0';
				/*adds the register number to the reg_operand_code*/
				reg_operand_code = reg_operand_code|mask;
				/*moves 3 bits to the left - for the next reg bits*/
				reg_operand_code = reg_operand_code << MOVE_BITS_OF_REG;	
			break;
			
			/*addressing number 3*/
			case 3:
				mask = MASK_FOR_ADD_3;
				/*adds addresing 3 to the opcode*/
				opcode = opcode | mask;
				/*moves 4 bits to the left - for the next addressing*/
				opcode = opcode << MOVE_BITS_OF_OP;
				/*Seperates the number from the Register name*/
				mask = (short int)(first_op[1])-'0';
				/*adds the register number to the reg_operand_code*/
				reg_operand_code = reg_operand_code|mask;
				/*moves 3 bits to the left - for the next reg bits*/
				reg_operand_code = reg_operand_code << MOVE_BITS_OF_REG;
			break;
	    		
			default:
			break;
		}
		
		/*checks if the second operand is a symbol*/
		if(is_a_symbol(second_op))
		{	
			
			mask = MASK_FOR_ADD_1;
			/*adds addresing 1 to the opcode*/
			opcode = opcode | mask;
			
			/*checks if the second operand is an external symbol*/
			if(is_symbol_external(second_op)==ZERO)
			{	
				/*if operand is external symbol, the representation is Zero + 001 (ARE)*/
				non_reg_operand_code=ONE;
				/*adding the external symbol to external table with the line of appearance*/
				add_to_external_table(second_op,IC+TWO);
			}
			
			else/*the second operand is a local symbol*/
			{
				/*if the symbol is local, then his value is needed*/
				non_reg_operand_code=value_of_symbol(second_op);
				/*moves 3 bits to the left - for the ARE bits*/
				non_reg_operand_code=non_reg_operand_code<<MOVE_BITS_OF_ARE;
				/*adds ARE bits to the non_reg_operand_code*/
				non_reg_operand_code = non_reg_operand_code | MASK_FOR_ARE_R;
			}
		}
		/*if second operand is a number*/
		else if (is_valid_number(second_op))
		{
			/*checks if the number is exceeding 12-bit number size*/
			if(is_num_in_limit(second_op))
			{
				mask = MASK_FOR_ADD_0;
				/*adds addresing 0 to the opcode*/
				opcode = opcode | mask;
				/*Seperates the number from the '#'*/
				non_reg_operand_code = (short int)atoi(strtok(second_op,"#"));
				/*moves 3 bits to the left - for the ARE bits*/
				non_reg_operand_code = non_reg_operand_code << MOVE_BITS_OF_ARE;
				/*adds ARE bits to the non_reg_operand_code*/
				non_reg_operand_code = non_reg_operand_code| MASK_FOR_ARE_A;
			}
			/*in case the number is excceding the 12-bit representation*/		
			else
			{
				sprintf(line_number_buffer, "%d", line_counter);
				_ERROR(5, NUMBER_REPRESENTATION_EXCEEDED_12BIN , "-", current_filename, ":", line_number_buffer );
				error_counter++;
			}
		}
		/*in case the second operand is unknown*/
		else
		{
			sprintf(line_number_buffer, "%d", line_counter);
			_ERROR(5, UNKNOWN_OPERAND , "-", current_filename, ":", line_number_buffer );
			error_counter++;
		}
		/*moves 3 bits to the left - for the ARE bits*/
		opcode = opcode << MOVE_BITS_OF_ARE;
		/*adds ARE bits to the opcode*/
		opcode = opcode | MASK_FOR_ARE_A;
		
		/*moves 3 bits to the left - for the ARE bits*/
		reg_operand_code = reg_operand_code << MOVE_BITS_OF_ARE;
		/*adds ARE bits to the reg_operand_code*/
		reg_operand_code = reg_operand_code | MASK_FOR_ARE_A;
		
		/*Adding the operation BMC to Instruction Table*/
		add_to_image(INSTRUCTION_TABLE_TYPE , IC, opcode);
		/*Adding the Register operand BMC to Instruction Table*/
		add_to_image(INSTRUCTION_TABLE_TYPE , ++IC, reg_operand_code);
		/*Adding the non Register operand BMC to Instruction Table*/
		add_to_image(INSTRUCTION_TABLE_TYPE , ++IC, non_reg_operand_code);
		IC++;
	}
	
	else/*the Register is used as a Destination operand*/
	{	
		/*checks if the first operand is a symbol*/
		if(is_a_symbol(first_op))
		{	
			mask = MASK_FOR_ADD_1;
			/*adds addresing 1 to the opcode*/
			opcode = opcode | mask;
			/*moves 4 bits to the left - for the next addressing bits*/
			opcode = opcode << MOVE_BITS_OF_OP;
			
			/*checks if the first operand is an external symbol*/
			if(is_symbol_external(first_op)==ZERO)
			{
				/*if operand is external symbol, the representation is Zero + 001 (ARE)*/
				non_reg_operand_code = ONE;
				/*adding the external symbol to external table with the line of appearance*/
				add_to_external_table(first_op,IC+ONE);
			}
			else /*the first operand is a local symbol*/
			{
				/*if the symbol is local, then his value is needed*/
				non_reg_operand_code = value_of_symbol(first_op);
				/*moves 3 bits to the left - for the ARE bits*/
				non_reg_operand_code = non_reg_operand_code<<MOVE_BITS_OF_ARE;
				/*adds ARE bits to the non_reg_operand_code*/
				non_reg_operand_code = non_reg_operand_code | MASK_FOR_ARE_R;
			}
		}
		
		/*if first operand is a number*/
		else if (is_valid_number(first_op))
		{
			/*checks if the number is exceeding 12-bit number size*/
			if(is_num_in_limit(first_op))
			{
				mask = MASK_FOR_ADD_0;
				/*adds addresing 0 to the opcode*/
				opcode = opcode | mask;
				/*moves 4 bits to the left - for the next addressing bits*/
				opcode = opcode << MOVE_BITS_OF_OP;

				/*Seperates the number from the '#'*/
				non_reg_operand_code = (short int)atoi(strtok(first_op,"#"));
				/*moves 3 bits to the left - for the ARE bits*/
				non_reg_operand_code = non_reg_operand_code << MOVE_BITS_OF_ARE;
				/*adds ARE bits to the non_reg_operand_code*/
				non_reg_operand_code = non_reg_operand_code| MASK_FOR_ARE_A;
			}

			/*in case the number is excceding the 12-bit representation*/		
			else
			{
				sprintf(line_number_buffer, "%d", line_counter);
				_ERROR(5, NUMBER_REPRESENTATION_EXCEEDED_12BIN , "-", current_filename, ":", line_number_buffer );
				error_counter++;
			}
		}
		/*in case the first operand is unknown*/
		else
		{
			sprintf(line_number_buffer, "%d", line_counter);
			_ERROR(5, UNKNOWN_OPERAND , "-", current_filename, ":", line_number_buffer );
			error_counter++;
		}
		
		/*switch case for the addressing of the second operand*/
		switch (address)
		{
			/*addressing number 2*/
			case 2:
				mask = MASK_FOR_ADD_2;
				/*adds addresing 2 to the opcode*/
				opcode = opcode | mask;
				/*moves 3 bits to the left - for the ARE bits*/
				opcode = opcode << MOVE_BITS_OF_ARE;
				/*Seperates the number from the Register name*/
				mask = (short int)(second_op[2])-'0';
				/*adds the register number to the reg_operand_code*/
				reg_operand_code = reg_operand_code|mask;
				/*moves 3 bits to the left - for the next reg bits*/
				reg_operand_code = reg_operand_code << MOVE_BITS_OF_ARE;
				
			break;
			
			/*addressing number 3*/
			case 3:
				mask = MASK_FOR_ADD_3;
				/*adds addresing 3 to the opcode*/
				opcode = opcode | mask;
				/*moves 3 bits to the left - for the ARE bits*/
				opcode = opcode << MOVE_BITS_OF_ARE;
				/*Seperates the number from the Register name*/
				mask = (short int)(second_op[1])-'0';
				/*adds the register number to the reg_operand_code*/
				reg_operand_code = reg_operand_code|mask;
				/*moves 3 bits to the left - for the next reg bits*/
				reg_operand_code = reg_operand_code << MOVE_BITS_OF_ARE;
				
			break;
	    		
			default:
			break;
		}

		/*adds ARE bits to the opcode*/
		opcode = opcode | MASK_FOR_ARE_A;
		/*adds ARE bits to the reg_operand_code*/
		reg_operand_code = reg_operand_code | MASK_FOR_ARE_A;
		
		/*Adding the operation BMC to Instruction Table*/
		add_to_image(INSTRUCTION_TABLE_TYPE , IC, opcode);
		/*Adding the non Register operand BMC to Instruction Table*/
		add_to_image(INSTRUCTION_TABLE_TYPE , ++IC, non_reg_operand_code);
		/*Adding the Register operand BMC to Instruction Table*/		
		add_to_image(INSTRUCTION_TABLE_TYPE , ++IC, reg_operand_code);
		IC++;
	}
}

/*Builds BMC in case that both operands aren't Registers*/
void bmc_for_two_op_non_reg(short int opcode, char first_op[], char second_op[], int line_counter)
{
	short int mask=ZERO;
	short int first_operand_code=ZERO;
	short int second_operand_code=ZERO;
	/*moves 4 bits to the left - for the next addressing bits*/
	opcode = opcode << MOVE_BITS_OF_OP;

	/*checks if the first operand is a symbol*/
	if(is_a_symbol(first_op))
	{	
		mask = MASK_FOR_ADD_1;
		/*adds addresing 1 to the opcode*/
		opcode = opcode | mask;
		/*moves 4 bits to the left - for the next addressing bits*/
		opcode = opcode << MOVE_BITS_OF_OP;

		/*checks if the first operand is an external symbol*/
		if(is_symbol_external(first_op)==ZERO)
		{
			/*if operand is external symbol, the representation is Zero + 001 (ARE)*/
			first_operand_code=ONE;
			/*adding the external symbol to external table with the line of appearance*/
			add_to_external_table(first_op,IC+ONE);
		}
		else/*the first operand is a local symbol*/
		{
			/*if the symbol is local, then his value is needed*/
			first_operand_code=value_of_symbol(first_op);
			/*moves 3 bits to the left - for the ARE bits*/
			first_operand_code=first_operand_code<<MOVE_BITS_OF_ARE;
			/*adds ARE bits to the first_operand_code*/
			first_operand_code = first_operand_code | MASK_FOR_ARE_R;
		}

	
	}
	/*if first operand is a number*/
	else if (is_valid_number(first_op))
	{
		/*checks if the number is exceeding 12-bit number size*/
		if(is_num_in_limit(first_op))
		{
			mask = MASK_FOR_ADD_0;
			/*adds addresing 0 to the opcode*/
			opcode = opcode | mask;
			/*moves 4 bits to the left - for the next addressing bits*/
			opcode = opcode << MOVE_BITS_OF_OP;
		
			/*Seperates the number from the '#'*/
			first_operand_code = (short int)atoi(strtok(first_op,"#"));
			/*moves 3 bits to the left - for the ARE bits*/
			first_operand_code = first_operand_code << MOVE_BITS_OF_ARE;
			/*adds ARE bits to the first_operand_code*/
			first_operand_code = first_operand_code| MASK_FOR_ARE_A;
		}


		/*in case the number is excceding the 12-bit representation*/		
		else
		{
			sprintf(line_number_buffer, "%d", line_counter);
			_ERROR(5, NUMBER_REPRESENTATION_EXCEEDED_12BIN , "-", current_filename, ":", line_number_buffer );
			error_counter++;
		}
	}
	/*in case the first operand is unknown*/
	else
	{
		sprintf(line_number_buffer, "%d", line_counter);
		_ERROR(5, UNKNOWN_OPERAND , "-", current_filename, ":", line_number_buffer );
		error_counter++;
	}

	
	/*checks if the second operand is a symbol*/
	if(is_a_symbol(second_op))
	{	
		mask = MASK_FOR_ADD_1;
		/*adds addresing 1 to the opcode*/
		opcode = opcode | mask;
		
		/*checks if the second operand is an external symbol*/
		if(is_symbol_external(second_op)==ZERO)
		{
			/*if operand is external symbol, the representation is Zero + 001 (ARE)*/
			second_operand_code=ONE;
			/*adding the external symbol to external table with the line of appearance*/
			add_to_external_table(first_op,IC+TWO);
		}
		else/*the second operand is a local symbol*/
		{
			/*if the symbol is local, then his value is needed*/
			second_operand_code = value_of_symbol(second_op);
			/*moves 3 bits to the left - for the ARE bits*/
			second_operand_code = second_operand_code<<MOVE_BITS_OF_ARE;
			/*adds ARE bits to the second_operand_code*/
			second_operand_code = second_operand_code | MASK_FOR_ARE_R;
		}

	}
	/*if second operand is a number*/
	else if (is_valid_number(second_op))
	{
		/*checks if the number is exceeding 12-bit number size*/
		if(is_num_in_limit(second_op))
		{
			mask = MASK_FOR_ADD_0;
			/*adds addresing 0 to the opcode*/
			opcode = opcode | mask;
		
			/*Seperates the number from the '#'*/
			second_operand_code = (short int)atoi(strtok(second_op,"#"));
			/*moves 3 bits to the left - for the ARE bits*/
			second_operand_code = second_operand_code << MOVE_BITS_OF_ARE;
			/*adds ARE bits to the second_operand_code*/
			second_operand_code = second_operand_code| MASK_FOR_ARE_A;
		}


		/*in case the number is excceding the 12-bit representation*/		
		else
		{
			sprintf(line_number_buffer, "%d", line_counter);
			_ERROR(5, NUMBER_REPRESENTATION_EXCEEDED_12BIN , "-", current_filename, ":", line_number_buffer );
			error_counter++;
		}
	}
	/*in case the second operand is unknown*/
	else
	{
		sprintf(line_number_buffer, "%d", line_counter);
		_ERROR(5, UNKNOWN_OPERAND , "-", current_filename, ":", line_number_buffer );
		error_counter++;
	}



	/*moves 3 bits to the left - for the ARE bits*/
	opcode = opcode << MOVE_BITS_OF_ARE;
	/*adds ARE bits to the opcode*/
	opcode = opcode | MASK_FOR_ARE_A;
	
	/*Adding the operation BMC to Instruction Table*/
	add_to_image(INSTRUCTION_TABLE_TYPE , IC, opcode);
	/*Adding the first operand BMC to Instruction Table*/
	add_to_image(INSTRUCTION_TABLE_TYPE , ++IC, first_operand_code);
	/*Adding the second operand BMC to Instruction Table*/
	add_to_image(INSTRUCTION_TABLE_TYPE , ++IC, second_operand_code);
	IC++;

}

/*Builds BMC in case that there is only one operand*/
void bmc_for_one_op(short int opcode, char first_op[], int address)
{
	short int mask = ZERO;
	short int operand_code=ZERO;
	
	/*moves 4 bits twice to the left - for the destination op bits*/
	opcode = opcode << MOVE_BITS_OF_OP;	
	opcode = opcode << MOVE_BITS_OF_OP;
	
	/*switch case for the addressing type of the operand*/
	switch (address)
		{
			/*in case the operand is a number*/
			case 0:
				mask = MASK_FOR_ADD_0;
				/*adds addresing 0 to the opcode*/
				opcode = opcode | mask;
				
				/*Seperates the number from the '#'*/
				operand_code = (short int)atoi(strtok(first_op,"#"));
				/*moves 3 bits to the left - for the ARE bits*/
				operand_code = operand_code << MOVE_BITS_OF_ARE;
				/*adds ARE bits to the operand_code*/
				operand_code = operand_code| MASK_FOR_ARE_A;
				
			break;
			
			/*in case the operand is a symbol*/
			case 1:
				
				mask = MASK_FOR_ADD_1;
				/*adds addresing 1 to the opcode*/
				opcode = opcode | mask;
				
				/*checks if the operand is an external symbol*/
				if(is_symbol_external(first_op)==ZERO)
				{
					/*if operand is external symbol, the representation is Zero + 001 (ARE)*/
					operand_code=ONE;
					/*adding the external symbol to external table with the line of appearance*/
					add_to_external_table(first_op,IC+ONE);
				}
				else /*the symbol a local symbol*/
				{
					/*if the symbol is local, then his value is needed*/
					operand_code=value_of_symbol(first_op);
					/*moves 3 bits to the left - for the ARE bits*/
					operand_code = operand_code << MOVE_BITS_OF_ARE;
					/*adds ARE bits to the operand_code*/
					operand_code = operand_code | MASK_FOR_ARE_R;
				}
				


			break;
			
			/*in case the operand is a *Register */
			case 2: 
				
				mask = MASK_FOR_ADD_2;
				/*adds addresing 2 to the opcode*/
				opcode = opcode | mask;
				/*Seperates the number from the Register name*/
				mask = (short int)(first_op[2])-'0';
				/*adds the register number to the operand_code*/
				operand_code = operand_code|mask;
				/*moves 3 bits to the left - for the ARE bits*/
				operand_code = operand_code << MOVE_BITS_OF_ARE;
				/*adds ARE bits to the operand_code*/
				operand_code = operand_code | MASK_FOR_ARE_A;
			
			break;

			/*in case the operand is a Register*/
			case 3:
				mask = MASK_FOR_ADD_3;
				/*adds addresing 3 to the opcode*/
				opcode = opcode | mask;
				/*Seperates the number from the Register name*/
				mask = (short int)(first_op[1])-'0';
				/*adds the register number to the operand_code*/
				operand_code=operand_code|mask;
				/*moves 3 bits to the left - for the ARE bits*/
				operand_code = operand_code << MOVE_BITS_OF_ARE;
				/*adds ARE bits to the operand_code*/
				operand_code = operand_code | MASK_FOR_ARE_A;
				
			break;
	    		
			default:
			break;
		}
				
	/*moves 3 bits to the left - for the ARE bits*/
	opcode = opcode << MOVE_BITS_OF_ARE;
	/*adds ARE bits to the opcode*/
	opcode = opcode | MASK_FOR_ARE_A;
	
	/*Adding the operation BMC to Instruction Table*/
	add_to_image(INSTRUCTION_TABLE_TYPE , IC, opcode);
	/*Adding the operand BMC to Instruction Table*/
	add_to_image(INSTRUCTION_TABLE_TYPE , ++IC, operand_code);
	IC++;
}

/*Builds BMC in case that there are no operands*/
void bmc_for_no_op(short int opcode)
{
	/*moves 11 bits to the left - for ARE bits */
	opcode = opcode << MOVE_BITS_OF_OP;
	opcode = opcode << MOVE_BITS_OF_OP;
	opcode = opcode << MOVE_BITS_OF_ARE;
	/*adds ARE bits to the opcode*/
	opcode = opcode | MASK_FOR_ARE_A;
	/*Adding the operation BMC to Instruction Table*/
	add_to_image(INSTRUCTION_TABLE_TYPE , IC, opcode);
	IC++;
}






						
							
