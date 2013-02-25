/* brainf**k interpreter in ANSI C
 * by mladjo2505
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Memory limit, number of registers */
#define LIMIT 30000
/* Number of loops inside loops */
#define BR_LIMIT 20

char array[LIMIT];          /* Memory registers */
char code[LIMIT];           /* brainf**k code to interpret */
int jumps[LIMIT];           /* Loop jumps, stores a index of the instruction to jump to if necessary */
int brackets[BR_LIMIT];     /* Brackets, used to calculate jumps for easier interpretation */
int pointer = 0;            /* Pointer to current position in memory array, data pointer */
int instruction = 0;        /* Pointer to current instruction in the code, instruction pointer */
int ins_number = 0;         /* Number of instructions in the read code */
int br_number = 0;          /* Current number of open brackets while reading the code */

char flag_interpret = 1;    /* Flag that indicates if we interpret the bf code */
char flag_bfoutput = 0;     /* Flag that indicates if we output clean bf code */
char flag_coutput = 0;      /* Flag that indicates if we output ANSI C equivalent of read bf code */

FILE* input = NULL;         /* File containing bf code */
char in_c;                  /* Input char for reading from input file */

/* Program usage, put in a sting for easier calling (and cleaner code) */
char usage[] =  "brainfuckk - brainf**k code interpreter by mladjo2505\n\n"
                "Usage:\n\tbrainfuck [-i|-n] [-w|-c] file.bf\n"
                "\n\t-i\tinterpret code, default action\n\t-n\tdo not interpret code"
                "\n\t-w\twrite pure bf code to output\n\t-c\twrite the ANSI C output code equivalent for given bf code"
                "\n\nfile.bf is a simple text file containing clean or commented bf code\n";

char code_header[] = "/* ANSI C code for brainf**k code given below.\n * Generated with brainf**k interpreter by mladjo2505.\n */\n\n";

/* Display an error message and exit */
void err_exit(const char* message);

int main(int argc, char* argv[]) {
    int i, j;

    /* If no command line arguments are passed, print out info and usage */
    if (argc == 1 || argc > 4) {
        /* If more than 3 command line arguments are passed, exit */
        if (argc > 4) printf("Too many command line parameters!\n\n");
        err_exit(usage);
    }

    /* If there are some (2 or 3) command line arguments, parse them */
    for (i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-i")) flag_interpret = 1;     /* Interpret bf code */
        if (!strcmp(argv[i], "-n")) flag_interpret = 0;     /* Do not interpret bf code */
        if (!strcmp(argv[i], "-w")) flag_bfoutput = 1;      /* Output clean bf code */
        if (!strcmp(argv[i], "-c")) flag_coutput = 1;       /* Output equivalent ANSI C code for read bf code */
    }

    /* Read code from input file */
    input = fopen(argv[argc-1], "rt");
    if (input == NULL) err_exit("Error opening input file.\n");
    while ((in_c = getc(input)) != EOF) {
        /* We ignore all chars but bf instuction ones */
        if (in_c == '>' || in_c == '<' || in_c == '+' || in_c == '-' || in_c == '.' || in_c == ',' || in_c == '[' || in_c == ']') {
            code[ins_number] = in_c;
            if (in_c == '[') {
                /* We make note where the loop begins */
                brackets[br_number] = ins_number;
                if (br_number >= BR_LIMIT) err_exit("Too many open brackets! (too many loops inside loops)\n");
                else br_number++;
            }
            if (in_c == ']') {
                if (br_number <= 0) err_exit("Too many closed brackets! (no matching open bracket)\n");
                /* Set jumps for this pair of matching brackets */
                else {
                    jumps[ins_number] = brackets[br_number - 1] + 1;
                    jumps[brackets[br_number - 1]] = ins_number + 1;
                    br_number--;
                }
            }
            ins_number++;
        }
    }

    /* Check if there were some unmatched brackets */
    if (br_number) err_exit("There are unmatched brackets in the code!\n");

    /* Output clean bf code, 70 chars in each line */
    if (flag_bfoutput) {
        for (i = 0; i < ins_number; i++) {
            if ((i % 70) == 0) printf("\n");
            putchar(code[i]);
        }
        printf("\n");
    }

    /* Output equivalent ANSI C code */
    if (flag_coutput) {
        printf(code_header);
        printf("/*\n");
        for (i = 0; i < ins_number; i++) {
            if ((i % 70) == 0) printf("\n");
            putchar(code[i]);
        }
        printf("\n*/\n");
        printf("#include <stdio.h>\nchar a[30000];\nint main() {\nchar *p=a;\n");
        for (i = 0; i < ins_number; i++) {
            switch (code[i]) {
                /* Increment the data pointer (to point to the next cell to the right) */
                case '>':
                    printf("++p;\n");
                    break;
                /* Decrement the data pointer (to point to the next cell to the left) */
                case '<':
                    printf("--p;\n");
                    break;
                /* Increment (increase by one) the byte at the data pointer */
                case '+':
                    printf("++*p;\n");
                    break;
                /* Decrement (decrease by one) the byte at the data pointer */
                case '-':
                    printf("--*p;\n");
                    break;
                /* Output the byte at the data pointer */
                case '.':
                    printf("putchar(*p);\n");
                    break;
                /* Accept one byte of input, storing its value in the byte at the data pointer */
                case ',':
                    printf("*p = getchar();\n");
                    break;
                /* If the byte at the data pointer is zero, then instead of moving the instruction
                 * pointer forward to the next command, jump it forward to the command after the
                 * matching ] command */
                case '[':
                    printf("while (*p) {\n");
                    break;
                /* if the byte at the data pointer is nonzero, then instead of moving the instruction
                 * pointer forward to the next command, jump it back to the command after the
                 * matching [ command */
                case ']':
                    printf("}\n");
                    break;
            }
        }
        printf("return 0;\n}\n");
    }

    /* Interpret bf code */
    if (flag_interpret) {
        instruction = 0;
        while (instruction < ins_number) {
            switch (code[instruction]) {
                /* Increment the data pointer (to point to the next cell to the right) */
                case '>':
                    if (pointer >= LIMIT) err_exit("Error in BF code: data pointer out of bounds (limit is 30000 chars)\n");
                    else pointer++;
                    instruction++;
                    break;
                /* Decrement the data pointer (to point to the next cell to the left) */
                case '<':
                    if (pointer <= 0) err_exit("Error in BF code: data pointer out of bounds (index of data pointer becomes negative)\n");
                    else pointer--;
                    instruction++;
                    break;
                /* Increment (increase by one) the byte at the data pointer */
                case '+':
                    array[pointer]++;
                    instruction++;
                    break;
                /* Decrement (decrease by one) the byte at the data pointer */
                case '-':
                    array[pointer]--;
                    instruction++;
                    break;
                /* Output the byte at the data pointer */
                case '.':
                    putchar(array[pointer]);
                    instruction++;
                    break;
                /* Accept one byte of input, storing its value in the byte at the data pointer */
                case ',':
                    array[pointer] = getchar();
                    instruction++;
                    break;
                /* If the byte at the data pointer is zero, then instead of moving the instruction
                 * pointer forward to the next command, jump it forward to the command after the
                 * matching ] command */
                case '[':
                    if (array[pointer] == (char)0) {
                        instruction = jumps[instruction];       /* Jump after matching closed bracket */
                    }
                    else instruction++;
                    break;
                /* If the byte at the data pointer is nonzero, then instead of moving the instruction
                 * pointer forward to the next command, jump it back to the command after the
                 * matching [ command */
                case ']':
                    if (array[pointer] != (char)0) {
                        instruction = jumps[instruction];       /* Jump after matching open bracket */
                    }
                    else instruction++;
                    break;
            }
        }
    }

    return 0;
}

void err_exit(const char* message) {
    fprintf(stderr, message);
    exit(EXIT_FAILURE);
}
