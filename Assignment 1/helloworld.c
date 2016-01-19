#include<stdio.h>
#include<stdlib.h>
//#include<conio.h>
#include<string.h>

//Buffer size to read the input file.
//The input file may not fit entirely in the In-Memory.
#define BUFFER_SIZE 10000

int match(char [], char []);

int main(int argc, char* argv[]){
	
	//Parse Input Arguments to get the file names.
	FILE* input_file = fopen(argv[1], "r");
	FILE* output_file = fopen(argv[3], "w");

	//Steps to get the size of the file.
	fseek(input_file, 0, SEEK_END); // seek to end of file 	
	int file_size = ftell(input_file)+1; // get current file pointer
	fseek(input_file, 0, SEEK_SET); // seek back to beginning of file
	
	//Parse String to be searched.
	int search_string_length = strlen(argv[2]);
	char search_string[search_string_length];
	strcpy(search_string, argv[2]);
	//printf("search string = %s\n", search_string);
	//Declare buffer 
	char buffer[BUFFER_SIZE];
	
	int count = 0; //variable to store number of string matches.
	int iteration = 0; //To track number of times buffer is refreshed.
	char* s[search_string_length];//strncpy(substr, buff+10, 4);
	//reading the file and counting the number of string matches.
	while (!feof(input_file)){
		if (iteration == 0){
			if(fgets(buffer, BUFFER_SIZE, input_file) == NULL){
				break;
			}
			printf("string read = %s\n", buffer);
			iteration++;
		}else{
			//fseek(input_file,-1*(search_string_length-1), SEEK_CUR);
			//strncpy(s, buffer, search_string_length-1);
			//s[search_string_length-1] = '\0';
			printf("string offset = %s\t", s);
			if(fgets(buffer, BUFFER_SIZE, input_file) == NULL)
				break;
			printf("string read = %s\n", buffer);
		}
		count  = count + match(buffer,search_string);
		//printf("input file = %s and count = %d\n", buffer,count);  //debug line.
	}
	
	//count  = match(buf,search_string);  //debug line.
	//printing number of string matches.
	printf("count = %d\n", count);
	
	if (output_file != NULL){
		printf("output file ready. \n");
		fprintf(output_file,"Input File Name : %s . \n",argv[1]);
		fprintf(output_file,"Input File Size : %d Bytes .\n",file_size);
		fprintf(output_file,"Number of matches of the search string - '%s' in input file : %d . \n",search_string,count);
		fclose(output_file);
	}
	return 0;
}

int match(char input_string[], char search_string[]){
	int i, j, k, text_length, search_string_length, position = -1;
	int count = 0;
	text_length = strlen(input_string);
	search_string_length = strlen(search_string);
	
	for(i = 0; i <= text_length - search_string_length; i++){
		position = k = i;
		for(j = 0; j < search_string_length; j++){
			if(search_string[j] == input_string[k]) k++; 
			else break; //break out of loop if a character doesnt match.
		}
		if(j == search_string_length) count++; //increment count if string matches.
	}
	return count;
}