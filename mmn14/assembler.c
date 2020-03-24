#include "assembler.h"

int main(int argc, char * argv[])
{
	int file_index;

	/* validate minimum number of arguments */
	if (argc < MIN_NUMBER_OF_ARGS)
	{
		_ERROR(1, INVALID_ARGS_NUM);
		return 1;
	}

	/* iterate over every argument */
	for (file_index = 1; file_index < argc; file_index++)
	{
		/* adding the ".as" suffix to the argument + ONE for '\0'*/
		current_filename = (char *)malloc((strlen(argv[file_index]) + IN_FILE_SUFFIX_SIZE+ONE) * sizeof(char)); 
		strcpy(current_filename, argv[file_index]);
		strcat(current_filename, IN_FILE_SUFFIX);

		/* open file */
		if (!(fd_input = fopen(current_filename, "r")))
		{
			_ERROR(3, FILE_OPEN, current_filename, strerror(errno));
			return 2;
		}
		
		/* start assembling this file */
		do_assembler(fd_input,argv[file_index]);

		/* free dynamically allocated curretn_filename and close file */
		free(current_filename);
		fclose(fd_input);
	}	

	return 0;
}
