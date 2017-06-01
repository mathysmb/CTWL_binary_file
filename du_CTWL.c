#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct TWN{
	float data;
	struct TWN *prev;
	struct TWN *next;
}TWN;

typedef struct{
	TWN *cur;
}CTWL;

TWN *ctwl_insert_left(CTWL* list, float val){
	TWN *new_left = malloc(sizeof(TWN *));
	TWN	*left = malloc(sizeof(TWN *));
	
	if (list->cur != NULL){
		new_left->data = val;
	
		left = list->cur->prev;
		
		new_left->prev = left;
		new_left->next = list->cur;
		
		left->next = new_left;	//TU!
		
		list->cur->prev = new_left;
	} else{
		new_left->data = val;
		new_left->next = new_left;
		new_left->prev = new_left;
		
	}
	
	list->cur = new_left;

	return list->cur;
}

TWN *ctwl_insert_right(CTWL* list, float val){
	TWN *new_right = malloc(sizeof(TWN *));
	TWN	*right = malloc(sizeof(TWN *));

	if (list->cur != NULL){
		new_right->data = val;

		right = list->cur->prev;

		new_right->prev = right;
		new_right->next = list->cur;

		right->next = new_right;

		list->cur->prev = new_right;
	} else{
		new_right->data = val;
		new_right->next = new_right;
		new_right->prev = new_right;
	}

	list->cur = new_right;

	return list->cur;
}

CTWL *ctwl_create_empty(void){
	CTWL *empty = malloc(sizeof(CTWL *));

	empty->cur = NULL;

	return empty;
}

char *ctwl_delete(CTWL* list){
	TWN *left;
	TWN *right;

	if (list->cur != NULL && list->cur->next != list->cur->prev){
		left = list->cur->prev;
		right = list->cur->next;
		
		left->next = right;
		right->prev = left;
	
		free(list->cur);
	
		list->cur = right;
	
		return "CTWL_OK";
	} else{
		if (list->cur == NULL){
        	return "CTWL_FAIL";
		} 
		else {
			free(list->cur);
            
            list = ctwl_create_empty();

			return "CTWL_OK";
		}
	}
}

void ctwl_cur_step_right(CTWL *list){
	list->cur = list->cur->next;
}

void ctwl_cur_step_left(CTWL *list){
	list->cur = list->cur->prev;
}

void ctwl_print(CTWL *list){
	TWN *start;
	
	if (list->cur != NULL){
		start = list->cur;
		
		do {
			if(start == list->cur) {
				printf("*");
			}
			
			printf("%f ",list->cur->data);
	
			ctwl_cur_step_left(list);
		} while (start != list->cur);
	} else {
		printf("List is empty\n");
	}
}

CTWL *ctwl_create_random(unsigned int size){
	CTWL *list;
	int i;
	float x;

	list = ctwl_create_empty();

	for(i=0; i<size; i++){
		x = rand() % 100;
        
		ctwl_insert_left(list,x);
	}

	return list;
}

void ctwl_destroy(CTWL* list){
	TWN *start;
	TWN *next;
	
	if (list->cur != NULL){
		start = list->cur;
		
		ctwl_cur_step_right(list);
		
		while (list->cur != start){
			next = list->cur->next;
			
			free(list->cur);
			
			list->cur = next;
		}	
	}
	
	free(list);
}

char *ctwl_save(CTWL *list, char *filename){
	int file;
	char text[5] = "CTWL";
	TWN *start;
	float x;
	
	if (list->cur == NULL) return "CTWL_FILE_PROBLEM";
	
	file = open(filename,O_RDWR|O_CREAT|O_BINARY,S_IWUSR);
	
	if (file < 0) return "CTWL_FILE_PROBLEM";	
	
	write(file,&text,sizeof(char[5]));
	
	start = list->cur;
		
	do {
		x = list->cur->data;
		write(file,&x,sizeof(float));

		ctwl_cur_step_left(list);
	} while (start != list->cur);
	
	close(file);
	
	return "CTWL_FILE_OK";
}

CTWL *ctwl_load(char *filename){
	int i=0,file,file_size;
	char text[5];
	char *output = "CTWL";
	float x;
	CTWL *list = malloc(sizeof(CTWL*));
	
	file = open(filename,O_RDONLY|O_BINARY,S_IWUSR);
	
	if (file < 0) return NULL;
	
	file_size = lseek(file, 0, SEEK_END);
	lseek(file,0,SEEK_SET);
	
	read(file,&text,sizeof(char[5]));
	
	if (strcmp(text,output) != 0) return NULL;
	
	list = ctwl_create_empty();
	
	i = sizeof(char[5]);
	while (i < file_size) {
		read(file,&x,sizeof(float));

		ctwl_insert_left(list,x);
		i += sizeof(float);
	}	
	ctwl_cur_step_left(list);
	close(file);
	
	return list;
}


void main(){
	CTWL *mojList;
	CTWL *copy;
	char *save_control;


	srand((unsigned int) time(NULL));

	mojList = ctwl_create_random(10);

	ctwl_print(mojList);

	save_control = ctwl_save(mojList,"nieco");
	
	printf("\n%s\n",save_control);
	
	copy = ctwl_load("nieco");
	
	ctwl_print(copy);
}
