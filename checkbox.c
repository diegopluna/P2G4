// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <dialog.h>

// int main() {
//     // Initialize the dialog library
//     dialog_vars.dialog_version = DIALOG_VERSION;
//     dialog_vars.input_result = malloc(sizeof(char) * MAX_LEN);
//     dialog_vars.output_result = malloc(sizeof(char) * MAX_LEN);
//     dialog_vars.input_result[0] = '\0';
//     dialog_vars.output_result[0] = '\0';

//     // Create the checklist items
//     char *items[] = {
//         "Assiduidade", 
//         "Pontualidade", 
//         "Iniciativa", 
//         "Postura ético-profissional", 
//         "Relacionamento em equipe", 
//         "Espírito crítico", 
//         "Comunicação", 
//         "Habilidades e competências específicas", 
//         "Participação nas atividades teóricas",
//         NULL
//     };

//     // Create the checklist
//     dialog_vars.input_result = dialog_checklist(
//         "Employee Evaluation",
//         "Please select the following attributes:",
//         10, 50, 10,
//         0,
//         items,
//         NULL
//     );

//     // Print the selected options
//     printf("Selected options: %s\n", dialog_vars.input_result);

//     // Cleanup and exit
//     free(dialog_vars.input_result);
//     free(dialog_vars.output_result);
//     end_dialog();
//     return 0;
// }

#include <dialog.h>

int main(void)
{
    int status;
    init_dialog(stdin, stdout);
    //   status = dialog_yesno(
    //     "Hello, in dialog-format",
    //     "Hello World!",
    //     0, 0);
    char *opt[] = {"opcao 1", "", "status 1", "opcao 2", "", "status 2", "opcao 3", "", "status 3", NULL};
    status = dialog_checklist("Titulo", "Selecione o teste", 0, 0, 0, 3, opt, FLAG_CHECK);
    end_dialog();
    printf("%d", status);
    return 0;
}

// extern int dialog_checklist(const char * /*title*/, const char * /*cprompt*/, int /*height*/, 
// int /*width*/, int /*list_height*/, int /*item_no*/, char ** /*items*/, int /*flag*/);