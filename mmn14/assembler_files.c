#include "assembler.h"

/*creates .ob output file from the .as file given */
void create_ob_file(char * file_name)
{
	
	char buffer1[MAX_ADDRESS_SIZE]; 
	char buffer2[MAX_DATA_SIZE];
	FILE *fp ;
	image_entry * entry;
	unsigned short int mask=MAX_15BIT_VAL;
	char * output_filename;

	/* adding the ".ob" suffix to the argument + ONE for '\0'  */
	output_filename = (char *)malloc((strlen(file_name) + OUT_OB_FILE_SUFFIX_SIZE + ONE) * sizeof(char)); 
	strcpy(output_filename, file_name);
	strcat(output_filename, OUT_OB_FILE_SUFFIX);
	

	/* open file */
	if (!(fp = fopen(output_filename, "w")))
	{
		_ERROR(3, FILE_OPEN, output_filename, strerror(errno));
		return;
	}
	
	/*writing IC and DC at the top of the file*/
	fprintf(fp,"   %d %d\n",IC-START_IC_VALUE,DC);

	/*writing instruction image to ob file*/
	entry = instruction_image;
	
	/*prints each entry from the instruction image*/
	while (entry != NULL)
	{
		sprintf(buffer1, "%04d ", entry->address);
		/*using mask to avoid using the 16-bit, using casting to unsigned var to avoiding using a negative number*/
		sprintf(buffer2, "%05o\n", ((unsigned short int) entry->code&mask));
		strcat(buffer1,buffer2);
		fputs (buffer1,fp);
		entry = entry->next;
	}

	/*writing data image to ob file*/
	entry = data_image; 
	
	/*prints each entry from the data image*/
	while (entry != NULL)
	{
		sprintf(buffer1, "%04d ", entry->address);
		/*using mask to avoid using the 16-bit, using casting to unsigned var to avoiding using a negative number*/
		sprintf(buffer2, "%05o\n", ((unsigned short int) entry->code&mask));
		strcat(buffer1,buffer2);
		fputs (buffer1,fp);
		entry = entry->next;
	}
	free(output_filename);
	fclose(fp);
}

/*creates .ext output file from the .as file given */
void create_ext_file(char * file_name)
{

	char buffer1[MAX_ADDRESS_SIZE]; 
	char buffer2[MAX_DATA_SIZE];
	FILE *fp ;
	external * ext = external_table;
	char * output_filename;
	

	/* adding the ".ext" suffix to the argument + ONE for '\0' */
	output_filename = (char *)malloc((strlen(file_name) + OUT_EXT_FILE_SUFFIX_SIZE + ONE) * sizeof(char));
	strcpy(output_filename, file_name);
	strcat(output_filename, OUT_EXT_FILE_SUFFIX);

	/* open file */
	if (!(fp = fopen(output_filename, "w")))
	{
		_ERROR(3, FILE_OPEN, output_filename, strerror(errno));
		return;
	}
	

	/*writing external table to ext file*/
	
	while (ext != NULL)
	{
		sprintf(buffer1, "%s ", ext->name);
		sprintf(buffer2, "%d\n", ext->line);
		strcat(buffer1,buffer2);
		fputs (buffer1,fp);
		ext = ext->next;
	}
	free(output_filename);
	fclose(fp);
}


/*creates .ent output file from the .as file given */
void create_ent_file(char * file_name)
{
	symbol * sym = symbol_table;
	char buffer1[MAX_ADDRESS_SIZE]; 
	char buffer2[MAX_DATA_SIZE];
	FILE *fp ;
	char * output_filename;
	
	/* adding the ".ent" suffix to the argument + ONE for '\0' */
	output_filename = (char *)malloc((strlen(file_name) + OUT_ENT_FILE_SUFFIX_SIZE + ONE) * sizeof(char)); 
	strcpy(output_filename, file_name);
	strcat(output_filename, OUT_ENT_FILE_SUFFIX);

	/* open file */
	if (!(fp = fopen(output_filename, "w")))
	{
		_ERROR(3, FILE_OPEN, output_filename, strerror(errno));
		return;
	}

	/*writing entry symbols to ent file*/
	while (sym != NULL) 
	{
		/*checks if symbol location is entry*/
		if (sym->location==ENTRY)
		{
			sprintf(buffer1, "%s ", sym->name);
			sprintf(buffer2, "%d\n", sym->value);
			strcat(buffer1,buffer2);
			fputs (buffer1,fp);
		}
		sym=sym->next;
	}
	free(output_filename);
	fclose(fp);
}
