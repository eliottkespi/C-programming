#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <math.h>
#include "error.h"

#define MIN_NUMBER_OF_ARGS 2 /* includes the command itself */
#define MAX_MESSAGE_SIZE 50
#define NUM_OF_OPCODE 16
#define MAX_SYMBOL_LENGTH 31
#define MAX_LINE_LENGTH 80
#define MAX_ADDRESS_SIZE 10
#define MAX_DATA_SIZE 13
#define MAX_BMC_SIZE 13
#define IN_FILE_SUFFIX_SIZE 3	/* includes the dot */
#define OUT_OB_FILE_SUFFIX_SIZE 3	/* includes the dot */
#define OUT_EXT_FILE_SUFFIX_SIZE 4 /* includes the dot */
#define OUT_ENT_FILE_SUFFIX_SIZE 4 /* includes the dot */
#define IN_FILE_SUFFIX ".as"
#define OUT_OB_FILE_SUFFIX ".ob"
#define OUT_EXT_FILE_SUFFIX ".ext"
#define OUT_ENT_FILE_SUFFIX ".ent"
#define LINE_NUM_BUF_SIZE 6
#define ZERO 0
#define ONE 1
#define TWO 2
#define MAX_MEMORY_SIZE 4095
#define MAX_12BIT_NUM 2047
#define MIN_12BIT_NUM -2048
#define MAX_15BIT_VAL 32767
#define START_IC_VALUE 100
#define CODE_SIZE 2
#define MOVE_BITS_OF_OP 4
#define MOVE_BITS_OF_ARE 3
#define MOVE_BITS_OF_REG 3
#define ADDRESSING_0 0
#define ADDRESSING_1 1
#define ADDRESSING_2 2
#define ADDRESSING_3 3
#define MASK_FOR_ADD_0 1
#define MASK_FOR_ADD_1 2
#define MASK_FOR_ADD_2 4
#define MASK_FOR_ADD_3 8
#define MASK_FOR_ARE_A 4
#define MASK_FOR_ARE_R 2
#define MASK_FOR_ARE_E 1
#define SRC_FLAG 1
#define DES_FLAG 2
#define ERR_RETURN_VAL -1
#define DATA_DIRECTIVE_TEXT ".data"
#define STRING_DIRECTIVE_TEXT ".string"
#define ENTRY_DIRECTIVE_TEXT ".entry"
#define EXTERN_DIRECTIVE_TEXT ".extern"

#define LINE "--------------------------------------------------"

enum {YES, NO}; 
typedef enum {INSTRUCTION_TABLE_TYPE, DATA_TABLE_TYPE} image_type;
typedef enum {CODE, DATA, NONE=0} symbol_type;
typedef enum {ENTRY=2, EXTERNAL=1, UNKNOWN=0} symbol_location;


/* opcode struct definition */
typedef struct opcode {
	int value;
	char * name;
	int operand_number_required;
	} opcode;

/* symbol struct definition */
typedef struct symbol {
	char * name;
	int value;
	symbol_type type;
	symbol_location location;
	struct symbol * next;
	} symbol;

/* external struct definition */
typedef struct external {
	char * name;
	int line;
	struct external * next;
	} external;

/* image entry struct definition */
typedef struct image_entry
{
	int address;
	short int code;
	struct image_entry * next;
	} image_entry;


/* function prototype */
void _message(char [],char []);
void _WARNING(int, ...);
void _ERROR(int, ...);
void _DEBUG(int, ...);
int add_to_symbol_table(char *, int, symbol_type, symbol_location);
int add_to_external_table(char *, int);
void print_symbol_table();
void print_external_table();
void free_symbol_table();
void free_external_table();
void do_assembler(FILE *,char *);
int add_to_image(image_type, int, short int);
void free_image(image_type);
void do_first_run(FILE *);
void do_second_run(FILE *);
int is_data(char []);
int is_string(char []);
int is_entry(char []);
int is_extern(char []);
int is_symbol_external(char []);
int is_directive(char []);
int is_a_symbol(char []);
int is_a_number(char []);
int value_of_symbol(char []);
int symbol_to_entry(char []);
int is_label(char []);
int is_valid_label(char []);
int is_a_register(char []);
int is_an_operation_name(char []);
int validate_list_of_elements(char []);
void remove_spaces(char *);
int count_instruction_words(char []);
void print_image(image_type);
int is_blank(char []);
int is_comment(char []);
void add_IC_to_data_symbol();
char * remove_leading_spaces(char *);
int operation_to_bin(char *);
void bmc_for_two_op_two_reg(short int, char [], short int , char [], short int);
void bmc_for_two_op_one_reg(short int, char [], short int, char [], int, int);
void bmc_for_two_op_non_reg(short int, char [], char [], int);
void bmc_for_one_op(short int, char [], int);
void bmc_for_no_op(short int);
void add_IC_to_data_image();
void create_ob_file(char * );
void create_ext_file(char * );
void create_ent_file(char * );
int is_valid_number(char *);
int is_num_in_limit(char *);

/* opcodes_table declaration */
opcode opcodes_table[NUM_OF_OPCODE];

/* symbol table declaration */
symbol * symbol_table;

/* external table declaration */
external * external_table;

/* instruction image declaration */
image_entry * instruction_image;

/* data image declaration */
image_entry * data_image;

/* counters declaration */
int IC; /* instruction counter */
int DC; /* data counter */
int error_counter; /* Error counter */

/* FILE descriptor an filename of inputed files */
FILE * fd_input;
char * current_filename;

/* will be used as a buffer for line number translation from int to array */
char line_number_buffer[LINE_NUM_BUF_SIZE];
