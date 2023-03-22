#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <ctype.h>
#include <errno.h>
#include <sysexits.h>
#include "document.h"

int is_blank_line(char *line);

/*Main function for user input*/
int main(int argc, char *argv[]){
   Document doc;
   FILE *file;
   char buffer[MAX_STR_SIZE + 1], line[MAX_STR_SIZE + 1];

   /*Initialises document*/
   init_document(&doc, "main_document");

   /*Standard input from user*/
   if(argc == 1){
       
       printf("> ");
       file = stdin;
   }

   /* Reading instructions from file*/
   else if (argc == 2){
       file = fopen(argv[1], "r");

       /* Provided file cannot be opened*/
       if(file == NULL){
           fprintf(stderr, "%s cannot be opened.\n", argv[1]);
           exit(EX_OSERR);
       }
   }

   /* Invalid arguments*/
   else{
      fprintf(stderr, "Usage: user_interface\n");
      fprintf(stderr, "Usage: user_interface <filename>\n");
      exit(EX_USAGE);
   }

   fgets(buffer, MAX_STR_SIZE + 1, file);

   /* Continues until file ends*/
   while(!feof(file)){
      
      /* Scans the line and removes the newline character*/
      buffer[strlen(buffer) - 1] = '\0';
      sscanf(buffer, "%s", line);

      /* If not comment or blank line: */
      if(*line != '#' && is_blank_line(buffer) == 1){
         
         int num_values;

         /* Adds a paragraph after specified*/  
         if(strcmp(line, "add_paragraph_after") == 0){
            char extra[MAX_STR_SIZE + 1]; 
            int paragraph_num;

            num_values = sscanf(buffer, "%s%d%s", line, &paragraph_num, extra);

            if(num_values == 2 && paragraph_num >= 0){

               if(add_paragraph_after(&doc, paragraph_num) == FAILURE){
                  printf("add_paragraph_after failed\n");
               }
            }
            else{
               printf("Invalid Command\n");
            }
         }   
        
         /*add_line_after*/
         else if(strcmp(line, "add_line_after") == 0){
            int paragraph_num, line_num;
            char line1[MAX_STR_SIZE + 1];
            char *new_line;

            num_values = sscanf(buffer, "%s%d%d%s", line, &paragraph_num, &line_num, line1);
            if(num_values == 4 && paragraph_num > 0 && line_num >= 0){

                /*If not missing * symbol */
                if(strchr(buffer, '*') != NULL){

                   new_line = strstr(buffer, line1);
                   new_line ++;

                   if(add_line_after(&doc, paragraph_num, line_num, new_line) == FAILURE){
                      printf("add_line_after failed\n");
                   }
                }
            }
            else{
                printf("Invalid Command\n");
            }
         }

         /* print_document */
         else if(strcmp(line, "print_document") == 0){
             char extra[MAX_STR_SIZE + 1];
             num_values = sscanf(buffer, "%s%s", line, extra);
             
             if(num_values == 1){
                if(print_document(&doc) == FAILURE){
                    printf("print_document failed\n");
                }
             }
             else{
                printf("Invalid Command\n");
             }
         }

         /* quit */
         else if(strcmp(line, "quit") == 0){
             char extra[MAX_STR_SIZE + 1];
             num_values = sscanf(buffer, "%s%s", line, extra);

             if(num_values == 1){
                 exit(EXIT_SUCCESS);
             }
             else{
                 printf("Invalid Command\n");
             }
         }

         /* exit */
         else if(strcmp(line, "exit") == 0){
             char extra[MAX_STR_SIZE + 1];
             num_values = sscanf(buffer, "%s%s", line, extra);

             if(num_values == 1){
                exit(EXIT_SUCCESS);
             }
             else{
                printf("Invalid Command\n");
             }
         }

         /* append_line */
         else if (strcmp(line, "append_line") == 0){
            int paragraph_num;
            char line1[MAX_STR_SIZE + 1];
            char *new_line;

            num_values = sscanf(buffer, "%s%d%s", line, &paragraph_num, line1);
            if(num_values == 3 && paragraph_num > 0){
                if(strchr(buffer, '*') != NULL){
                   new_line = strstr(buffer, line1);
                   new_line ++;
                   
                   if(append_line(&doc, paragraph_num, new_line) == FAILURE){
                       printf("append_line failed\n");
                   }
                }
            }
            else{
                printf("Invalid Command\n");
            }
         }

         /* remove_line */
         else if (strcmp(line, "remove_line") == 0){
            int paragraph_num, line_num;
            char extra[MAX_STR_SIZE + 1];

            num_values = sscanf(buffer, "%s%d%d%s", line, &paragraph_num, &line_num, extra);
            if(num_values == 3 && paragraph_num > 0 && line_num > 0){
                if(remove_line(&doc, paragraph_num, line_num) == FAILURE){
                    printf("remove_line failed\n");
                }
            }
            else{
                printf("Invalid Command\n");
            }
         }

         /* load_file */
         else if(strcmp(line, "load_file") == 0){
             char name[MAX_STR_SIZE + 1], extra[MAX_STR_SIZE + 1];
             num_values = sscanf(buffer, "%s%s%s", line, name, extra);

             if(num_values == 2){
                 if(load_file(&doc, name) == FAILURE){
                    printf("load_file failed\n");
                 }
             }
             else{
                 printf("Invalid Command \n");
             }
         }

         /* replace_text */
         else if(strcmp(line, "replace_text") == 0){
            char target[MAX_STR_SIZE + 1], replacement[MAX_STR_SIZE + 1];
            char t[MAX_STR_SIZE + 1], r[MAX_STR_SIZE + 1];
            char *q1, *q2, *q3, *q4;

            num_values = sscanf(buffer, "%s%s%s", line, target, replacement);
            q1 = strstr(buffer, "\"");

            /*Assign pointer values to the four quotation marks for removal*/
            if(num_values == 3 && q1 != NULL){
                q2 = strstr(q1 + 1, "\"");
            
                if(q2 != NULL){
                   q3 = strstr(q2 + 1, "\"");
 
                   if(q3 != NULL){
                       q4 = strstr(q3 + 1, "\"");
              
                       /* Use pointer arithmetic to remove quotation marks */
                       if(q4 != NULL){
                          strncpy(t, q1 + 1, q2 - q1);
                          t[q2 - (q1 + 1)] = '\0';

                          strncpy(r, q3 + 1, q4 - q3);
                          r[q4 - (q3 + 1)] = '\0';

                          if(replace_text(&doc, t, r) == FAILURE){
                              printf("remove_line failed\n");
                          }
                       }         
                   }
                }
            }
            else{
               printf("Invalid Command\n");
            }
         }

         /* highlight_text */
         else if(strcmp(line, "highlight_text") == 0){
            char t[MAX_STR_SIZE + 1], target[MAX_STR_SIZE + 1];
            char *q1, *q2;
            num_values = sscanf(buffer, "%s%s", line, t);

            /* Removes quotation marks using pointer arithmetic*/
            q1 = strstr(buffer, "\"");
            if(num_values == 2){
                if(q1 != NULL){
                   q2 = strstr(q1 + 1, "\"");
                   
                   if(q2 != NULL){
                      strncpy(target, q1 + 1, q2 - q1);
                      target[q2 - (q1 + 1)] = '\0';

                      highlight_text(&doc, target);
                   }
                }
            }
            else{
                printf("Invalid Command\n");
            }
         }

         /* remove_text */
         else if(strcmp(line, "remove_text") == 0){
             char t[MAX_STR_SIZE + 1], target[MAX_STR_SIZE + 1];
             char *q1, *q2;
             
             num_values = sscanf(buffer, "%s%s", line, t);
             q1 = strstr(buffer, "\"");

             if(num_values == 2){
                if(q1 != NULL){
                    q2 = strstr(q1 + 1, "\"");
                    
                    if(q2 != NULL){
                        strncpy(target, q1 + 1, q2 - q1);
                        target[q2 - (q1 + 1)] = '\0';
              
                        remove_text(&doc, target);
                    }
                }
             }
             else{
                printf("Invalid Command\n");
             }
         }

         /*save_document */
         else if(strcmp(line, "save_document") == 0){
             char name[MAX_STR_SIZE + 1], extra[MAX_STR_SIZE + 1];
             num_values = sscanf(buffer, "%s%s%s", line, name, extra);

             if(num_values == 2){
                if(save_document(&doc, name) == FAILURE){
                   printf("save_document failed\n");
                }
             }
             else{
                printf("Invalid Command\n");
             }
         }

         /*reset_document */
         else if(strcmp(line, "reset_document") == 0){
             char extra[MAX_STR_SIZE + 1];
             num_values = sscanf(buffer, "%s%s", line, extra);
            
             if(num_values == 1){
                reset_document(&doc);
             }
             else{
                printf("Invalid Command\n");
             }
         }
      }

      /*Prompts and gets new user input*/
      if(argc == 1){
         printf("> ");
      }
      fgets(buffer, MAX_STR_SIZE + 1, file);
   }

   fclose(file);
   exit(EXIT_SUCCESS);
}

/*Helper method to check if a line is blank:
  Returns 0 if is blank, returns 1 if not blank
*/
int is_blank_line(char *line){
   int i;

   for(i = 0; i < strlen(line); i ++){
      if(isspace(line[i]) == 0){
         return 1;
      }
   }
   return 0;
}
