/********************************
* Lab Assignment 1.
* -----------------
* SearchString.c
* 
* This code helps to count the number of ocuurances of a given String in the given input text file.
* We are reading the input file into buffer of fixed size. 
* Hence we can even run this code for larger input file, without worrying about in memory storage.
*
*
* @author : Rakshith Kunchum
* @version : 1.0.0
* @email : kunchum.1@osu.edu
*/

#include<stdio.h>
#include<stdlib.h>
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
	int file_size = ftell(input_file)+1; // get current file pointer. +1 to account for end-of-file character.
	fseek(input_file, 0, SEEK_SET); // seek back to beginning of file
	
	//Parse String to be searched.
	int search_string_length = strlen(argv[2]);
	char search_string[search_string_length];
	strcpy(search_string, argv[2]);
	//printf("search string = %s, %d\n", search_string, strlen(search_string)); //debug line.
	//verify if the file size is less than the length of the search string.

	if((file_size-1) < search_string_length){
		if(file_size == 0){
			printf("Input file is empty.");
		}else{
			printf("The input file is very small to search for the given string.\nWe can conclude search string doesnt occur in the text file.");
		}
		fprintf(output_file,"Input File Name : %s . \n",argv[1]);
		fprintf(output_file,"Input File Size : %d Bytes .\n",file_size);
		fprintf(output_file,"Number of matches of the search string - '%s' in input file : %d . \n",search_string,0);
		fclose(output_file);
		return 0;
	}
	//Declare buffer 
	char buffer[BUFFER_SIZE]="";
	int count = 0; //variable to store number of string matches.

	while (fgets(buffer, BUFFER_SIZE, input_file) != NULL){
		count  = count + match(buffer,search_string);
		if(strlen(buffer) == BUFFER_SIZE-1){
			fseek(input_file,-1*(search_string_length-1),SEEK_CUR);
		}
	}
	
	//count  = match(buf,search_string);  //debug line.
	//printing number of string matches.
	printf("Search String : %s\n", search_string);
	printf("Number of Search-String matches in the given file = %d\n", count);
	
	if (output_file != NULL){
		printf("output file ready. \n");
		fprintf(output_file,"Input File Name : %s . \n",argv[1]);
		fprintf(output_file,"Input File Size : %d Bytes .\n",file_size);
		fprintf(output_file,"Number of matches of the search string - '%s' in input file : %d . \n",search_string,count);
		fclose(output_file);
	}
	return 0;
}

/**************************
* int match(char input_string[], char search_string[])
* input_string : String from the input text file.
* search_string : the string that we are trying to serach in the input file.
* The code traverses through the input string in a serial fashion and searches for the given string.
* Every time a match is found the count is incremented. In the end count is returned.
*/
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
