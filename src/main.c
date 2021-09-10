#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <argp.h>
#include "atmega328/atmega328.h"
#include "atmega328/debug.h"

#include <stdio.h>
#include <argp.h>

const char *argp_program_version =
    "atmegaEmu 0.0.1";

const char *argp_program_bug_address =
    "<andreas.kipping@mailbox.org>";

/* This structure is used by main to communicate with parse_opt. */
struct arguments
{
    char *args[1];           /* ARG1 and ARG2 */
    int verbose;             /* The -v flag */
    char *outfile;           /* Argument for -o */
    char *string1, *string2; /* Arguments for -a and -b */
};

/*
   OPTIONS.  Field 1 in ARGP.
   Order of fields: {NAME, KEY, ARG, FLAGS, DOC}.
*/
static struct argp_option options[] =
    {
        {"verbose", 'v', 0, 0, "Produce verbose output"},
        {"clock", 'c', "CLOCKSPEED", 0,
         "Do something with STRING1 related to the letter A"},
        {"bravo", 'b', "STRING2", 0,
         "Do something with STRING2 related to the letter B"},
        {"output", 'o', "OUTFILE", 0,
         "Output to OUTFILE instead of to standard output"},
        {0}};

/*
   PARSER. Field 2 in ARGP.
   Order of parameters: KEY, ARG, STATE.
*/
static error_t
parse_opt(int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = state->input;

    switch (key)
    {
    case 'v':
        arguments->verbose = 1;
        break;
    case 'c':
        arguments->string1 = arg;
        g_clockSpeed = atoi(arguments->string1);
        break;
    case 'b':
        arguments->string2 = arg;
        break;
    case 'o':
        arguments->outfile = arg;
        break;
    case ARGP_KEY_ARG:
        if (state->arg_num >= 1)
        {
            argp_usage(state);
        }
        arguments->args[state->arg_num] = arg;
        break;
    case ARGP_KEY_END:
        if (state->arg_num < 1)
        {
            argp_usage(state);
        }
        break;
    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

/*
   ARGS_DOC. Field 3 in ARGP.
   A description of the non-option command-line arguments
     that we accept.
*/
static char args_doc[] = "FILEPATH";

/*
  DOC.  Field 4 in ARGP.
  Program documentation.
*/
static char doc[] =
    "argex -- A program to demonstrate how to code command-line options and arguments.\vFrom the GNU C Tutorial.";

/*
   The ARGP structure itself.
*/
static struct argp argp = {options, parse_opt, args_doc, doc};

/*
   The main function.
   Notice how now the only function call needed to process
   all command-line options and arguments nicely
   is argp_parse.
*/
int main(int argc, char **argv)
{
    struct arguments arguments;
    FILE *outstream;

    /* Set argument defaults */
    arguments.outfile = NULL;
    arguments.string1 = "";
    arguments.string2 = "";
    arguments.verbose = 0;

    /* Where the magic happens */
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    /* Where do we send output? */
    if (arguments.outfile)
        outstream = fopen(arguments.outfile, "w");
    else
        outstream = stdout;

    ///* Print argument values */
    //fprintf(outstream, "clockspeed = %s\nbravo = %s\n\n",
    //        arguments.string1, arguments.string2);
    //fprintf(outstream, "FILEPATH = %s\n\n",
    //        arguments.args[0],
    //        arguments.args[1]);


    atm_loadProgram(arguments.args[0]);

    /* If in verbose mode, print song stanza */
    if (arguments.verbose) {
        atm_startVerbose();
    } else {
        atm_start();
    }

    bool is_running = true;

    while(is_running) {
        char character = getchar();
    
        switch (character) {
            case 'e': is_running = false; break;
            case 'l': atm_loadProgram(argv[1]); break;
            case 's': atm_start(); break;
            case 'h': atm_stop(); break;
            case 'c': is_running = false; break;
            case 'm': deb_triggerSpeedTest(); break;
            case 'd': atm_disassemble(); break;
            case '1': atm_setPin5high(); break;
            case '0': atm_setPin5low(); break;
            default: ; break;
    
        }
    }

    return 0;
}