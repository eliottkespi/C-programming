#include "assembler.h"

/* images initialization */
image_entry * instruction_image = NULL;
image_entry * data_image = NULL;

/* add entry an image - return 0 if succeeded     */
int add_to_image(image_type type, int address, char * code)
{
	/* new image entry dynamic allocation */
	image_entry * new_entry = (image_entry *)malloc(sizeof(image_entry));

	_DEBUG("-->> add_to_image");

	if (!new_entry)
	{
		_ERROR(ALLOCATION_ERROR);
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

		_DEBUG("Added new image entry in instruction image");
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

		_DEBUG("Added new image entry in data image");
	}

	_DEBUG("<<-- add_to_image");
	return 0;
}

/* free the dynamically allocated image*/
void free_image(image_type type)
{
	image_entry * entry;

	_DEBUG("-->> free_image");

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
		while(instruction_image)
		{
			entry = data_image;
			data_image = data_image->next;
			free(entry);
		}
	}
	_DEBUG("<<-- free_symbol_table");
}
