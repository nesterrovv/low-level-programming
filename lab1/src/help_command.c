#include "help_command.h"

void print_help() {
    printf("++++++++++++++++++++++++++++++++++++++++\n");

    printf("'DATA PRINTING'\n");
    printf("PRINTING CURRENT DATA FROM ARRAY.\n\n");

    printf("'HEADER PRINTING'\n");
    printf("HEADER FIELDS PRINTING\n\n");

    printf("'syntax: add <parent_id> <key #1>=<value #1> <key #1>=<value #2> <key #3>=<value #3> etc...'\n");
    printf("ADDING TO THE TREE NEW NODE WITH PARAMETERS WHICH WERE WRITTEN JUST NOW...\n\n");

    printf("'syntax: update <id> <key #1>=<new_value #1> <key #2>=<new_value #2> <key #3>=<new_value #3> etc...'\n");
    printf("UPDATING OF SELECTED FIELD OF FOUND NODE...\n\n");

    printf("'syntax: delete <node_id>'\n");
    printf("DELETING THE NODE WITH GIVEN ID. CHILDREN OF THIS NODE WILL BE DELETED TOO...\n\n");

    printf("'syntax: select_by field <field name> <field value>'\n");
    printf("FINDING NODE VIA GIVEN CONDITIONS...\n\n");

    printf("'syntax: select_by id <id>'\n");
    printf("FINDING THE NODE BY ID...\n\n");

    printf("'select_by parent <parent id>'\n");
    printf("Finding children of written parent.\n\n");

    printf("'exit'\n");
    printf("Exit the program.\n\n");
    printf("++++++++++++++++++++++++++++++++++++++++\n");
}