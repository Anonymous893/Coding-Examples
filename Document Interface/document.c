/* Jessica Shi 
UID: 117976373
UMD Directory ID: jshi2021
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "document.h"

/*Initialises the document*/
int init_document(Document *doc, const char *name){
   int name_len;
   
   /*Cases of failure*/
   if(doc == NULL){
      return FAILURE;
   }
   if(name == NULL){
      return FAILURE;
   }
   
   name_len = strlen(name);
   if(name_len > MAX_STR_SIZE){
      return FAILURE;
   }

   /* Creates the document*/
   doc->number_of_paragraphs = 0;
   strcpy(doc->name, name);
   return SUCCESS;
}

/*Resets the document by setting number of paragraphs to 0*/
int reset_document(Document *doc){
   if(doc == NULL){
      return FAILURE;
   }
   doc->number_of_paragraphs = 0;
   return SUCCESS;
}

/*Prints document's name, number of paragraphs, paragraph*/
int print_document(Document *doc){
   int i, j;

   /*Cases of failure*/
   if(doc == NULL){
      return FAILURE;
   }

   /*Prints the document name and number of paragraphs*/
   printf("Document name: \"%s\"\n", doc->name);
   printf("Number of Paragraphs: %d\n", doc->number_of_paragraphs);

   /*Iterates through each paragraph to print the paragraph*/
   for(i = 0; i < doc->number_of_paragraphs; i ++){
       for(j = 0; j < doc->paragraphs[i].number_of_lines; j ++){
            printf("%s\n", doc->paragraphs[i].lines[j]);
       }
       if(i < (doc->number_of_paragraphs - 1)){
            printf("\n");
       }
   }

   return SUCCESS;
}

/*Adds a paragraph after specified paragraph number*/
int add_paragraph_after(Document *doc, int paragraph_number){
   int i;
   Paragraph add;
   
   /*Cases of failure*/
   if(doc == NULL){
      return FAILURE;
   }
   if(doc->number_of_paragraphs == MAX_PARAGRAPHS){
      return FAILURE;
   }
   if(paragraph_number > MAX_PARAGRAPHS || doc->number_of_paragraphs < paragraph_number){
      return FAILURE;
   }

   add.number_of_lines = 0;

   /*Adds paragraph to the end of the document*/
   if(doc->number_of_paragraphs == paragraph_number){
      doc->paragraphs[paragraph_number] = add;
   }

   /*Adds paragraph in between other paragraphs*/
   else{
      /* Shifts paragraphs to right to make space for new paragraph */
      for(i = doc->number_of_paragraphs - 1; i >= paragraph_number; i--){
          doc->paragraphs[i + 1] = doc->paragraphs[i];
      }
      doc->paragraphs[paragraph_number] = add;
   }

   doc->number_of_paragraphs += 1;
   return SUCCESS;
}

/*Adds a new line after the line with the specified line number*/
int add_line_after(Document *doc, int paragraph_number, int line_number, const char *new_line){
   int index = paragraph_number - 1;  
   int i;

   /* Causes of failure*/
   if(doc == NULL){
       return FAILURE;
   }
   if(paragraph_number > doc->number_of_paragraphs){
      return FAILURE;
   }
   if(doc->paragraphs[index].number_of_lines == MAX_PARAGRAPH_LINES){
      return FAILURE;
   }
   if(new_line == NULL){
      return FAILURE;
   }
   if(doc->paragraphs[index].number_of_lines < line_number){
      return FAILURE;
   }

   /* Adds a new line to end of paragraph */
   if(doc->paragraphs[index].number_of_lines == line_number){
       strcpy(doc->paragraphs[index].lines[line_number], new_line);
   }

   /* Adds a new line in between lines of the paragraph */
   else{
      /*Shifts the lines to the right to make space for new line */
      for(i = doc->paragraphs[index].number_of_lines - 1; i >= line_number; i--){
         strcpy(doc->paragraphs[index].lines[i + 1], doc->paragraphs[index].lines[i]);
      }
      strcpy(doc->paragraphs[index].lines[line_number], new_line);
   }

   doc->paragraphs[index].number_of_lines += 1;
   return SUCCESS;
}

/*Returns the number of lines in a paragraph*/
int get_number_lines_paragraph(Document *doc, int paragraph_number, int *number_of_lines){
   if(doc == NULL || number_of_lines == NULL){
       return FAILURE;
   }
   if(paragraph_number > doc->number_of_paragraphs){
       return FAILURE;
   }
   
   *number_of_lines = doc->paragraphs[paragraph_number - 1].number_of_lines;
   return SUCCESS;
}

/* Appends a line to the specified paragraph */
int append_line(Document *doc, int paragraph_number, const char *new_line){
   int index = paragraph_number - 1;
   int num_lines;

   /* Causes of failure*/
   if(doc == NULL){
       return FAILURE;
   }
   if(paragraph_number > doc->number_of_paragraphs){
      return FAILURE;
   }
   if(doc->paragraphs[index].number_of_lines == MAX_PARAGRAPH_LINES){
      return FAILURE;
   }
   if(new_line == NULL){
      return FAILURE;
   }

   /*Adds the new line to the end of the last line of specified paragraph*/
   num_lines = doc->paragraphs[index].number_of_lines;
   strcpy(doc->paragraphs[index].lines[num_lines], new_line);
   doc->paragraphs[index].number_of_lines += 1;

   return SUCCESS;
}

/*Removes the specified line from paragraph*/
int remove_line(Document *doc, int paragraph_number, int line_number){
   int index = paragraph_number - 1;
   int i;

   if(doc == NULL){
      return FAILURE;
   }
   if(paragraph_number > doc->number_of_paragraphs){
      return FAILURE;
   }
   if(line_number > doc->paragraphs[index].number_of_lines){
      return FAILURE;
   }

   /* Removes a line from the paragraph */
   if(doc->paragraphs[index].number_of_lines > 1){
      /*Shift all lines left of line to remove*/
      for(i = line_number; i < doc->paragraphs[index].number_of_lines; i ++){
         strcpy(doc->paragraphs[index].lines[i - 1], doc->paragraphs[index].lines[i]); 
      }
   }
  
   /* Clears the paragraph by removing one line */
   else{
      *doc->paragraphs[index].lines[doc->paragraphs[index].number_of_lines - 1] = 0;
   }
   
   doc->paragraphs[index].number_of_lines -= 1;
   return SUCCESS; 
}

/* Adds the first data_lines num of lines from data array to the document */
int load_document(Document *doc, char data[][MAX_STR_SIZE + 1], int data_lines){
   int i;
   int num_paragraphs = doc->number_of_paragraphs;
   if(doc == NULL || data == NULL || data_lines == 0){
       return FAILURE;
   }

   /*Creates the first paragraph by default*/
   add_paragraph_after(doc, doc->number_of_paragraphs);
  
   for(i = 0; i < data_lines; i ++){
      
      /*Adds the line from data array to document*/
      if(strcmp(data[i], "") != 0){
          append_line(doc, num_paragraphs + 1, data[i]);
      }
      else{
   
         /*Empty string in array creates new paragraph*/
         num_paragraphs += 1;
         add_paragraph_after(doc, num_paragraphs);
      }
   }

   return SUCCESS;
}

/*Replaces the text target with the text replacement throughout doc*/
int replace_text(Document *doc, const char *target, const char *replacement){
   int i, j, position;   
   char temp[MAX_STR_SIZE + 1];
   char *target_pointer;

   if(doc == NULL || target == NULL || replacement == NULL){
      return FAILURE;
   }
   
   for(i = 0; i < doc->number_of_paragraphs; i ++){
      for(j = 0; j < doc->paragraphs[i].number_of_lines; j ++){
         strcpy(temp, doc->paragraphs[i].lines[j]);

         /*Gets the pointer location for the first target occurence */          
         target_pointer = strstr(temp, target);

         /* If the target is contained in a line */
         if(target_pointer != NULL){

            /*Finds the position of the occurence and truncates it */
            position = target_pointer - temp;
            doc->paragraphs[i].lines[j][position] = '\0';
         }

         /*Iterates through the occurences of the target*/
         while(target_pointer){
            
            /* Adds the replacement string to the temp string*/
            strcat(doc->paragraphs[i].lines[j], replacement);

            /* Moves pointer then adds the rest of the string*/ 
            target_pointer = target_pointer + strlen(target);
            strcat(doc->paragraphs[i].lines[j], target_pointer);

            /* Moves the pointer to check for more occurences*/
            target_pointer = strstr(target_pointer, target);            
            if(target_pointer != NULL){
               position = strlen(doc->paragraphs[i].lines[j]) - strlen(target_pointer);
               doc->paragraphs[i].lines[j][position] = '\0';
            }
         }
      }
   }
   return SUCCESS;
}

/*Highlights the text associated with target everywhere it appears*/
int highlight_text(Document *doc, const char *target){
   char temp[MAX_STR_SIZE + 1] = "";

   if(doc == NULL || target == NULL){
       return FAILURE;
   }
   
   /* Surrounds the target with the highlight strings */   
   strcat(temp, HIGHLIGHT_START_STR);
   strcat(temp, target);
   strcat(temp, HIGHLIGHT_END_STR);
   replace_text(doc, target, temp);
   
         
   return SUCCESS;
}

/*Removes the target text everywhere it appears*/
int remove_text(Document *doc, const char *target){
   if(doc == NULL || target == NULL){
       return FAILURE;
   }
   
   replace_text(doc, target, "");
   return SUCCESS;
}
 
/*Loads data from a file, adds new paragraph by default*/
int load_file(Document *doc, const char *filename){
   FILE *file = fopen(filename, "r");
   char line[MAX_STR_SIZE + 1], instructions[MAX_STR_SIZE + 1];
   char comment_symbol;
   int paragraphs = 0;
   int line_values;

   if(doc == NULL || filename == NULL || file == NULL){
      return FAILURE;
   }
   
   /*Adds paragraph if document is empty*/
   add_paragraph_after(doc, 0);
   
   /*Loads the rest of the file*/
   while(fgets(line, MAX_STR_SIZE + 1, file)){
      
      /*Retrieves data from file, separates it by new line*/
      line_values = sscanf(line, "%c%s", &comment_symbol, instructions);
      line[strlen(line) - 1] = '\0';

      /*Line is not a comment */
      if(comment_symbol != '#'){
         if(line_values >= 2){
            append_line(doc, paragraphs + 1, line);
         }
         
         else{
            if(doc->number_of_paragraphs < MAX_PARAGRAPHS){
                add_paragraph_after(doc, paragraphs + 1);
                paragraphs += 1;
            }
         } 
      }
   }
   fclose(file);
   return SUCCESS;
}

/*Prints the paragraphs associated with the specified document to the specified file*/
int save_document(Document *doc, const char *filename){
    FILE *file = fopen(filename, "w");
    int i, j;

    if(doc == NULL || filename == NULL || file == NULL){
        return FAILURE;
    }

    for(i = 0; i < doc->number_of_paragraphs; i ++){
        for(j = 0; j < doc->paragraphs[i].number_of_lines; j++){
            
            fputs(doc->paragraphs[i].lines[j], file);
            
            if(j <= doc->paragraphs[i].number_of_lines){
                fputs("\n", file);
            }
        }

        if((i + 1) <= doc->number_of_paragraphs){
            fputs("\n", file);
        }
    }
    fclose(file);
    return SUCCESS;
}
