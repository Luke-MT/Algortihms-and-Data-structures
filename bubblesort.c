#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <upo/sort.h>

void upo_bubble_sort(void *base, size_t n, size_t size, int (*cmp)(void*,void*));
int int_cmp(void *a, void *b){
	int *aa = a, *bb = b;
	return(*aa > *bb) - (*bb > *aa);
}
int string_cmp(void *a, void *b) {
    char **aa = (char**) a;
    char **bb = (char**) b;
    return strcmp(*aa,*bb);
}
void swap(void *a, void *b, size_t size){
	unsigned char *aa = a, *bb = b, tmp;
	for (size_t i = 0; i < size; ++i){
		tmp = aa[i];
		aa[i] = bb[i];
		bb[i] = tmp;
	}
}
void copy(void *a, void *b, size_t size){
	memcpy(a,b,size);//forse
}
void print_array_int(int v[], int dim) {
    for(int i=0; i < dim; i++) {
        printf("%d ", v[i]);
    }
    printf("\n");
}
void print_array_string(char *s[], int dim) {
    for(int i=0; i < dim; i++) {
        printf("%s ", s[i]);
    }
    printf("\n");
}




int main(int argc, char const *argv[])
{
	int int_numbers[] = {12,1,0,17,21,-4,8,5};
	printf("[1] Testing bubble sort:\n");
    printf("[1-1] Original int array:\n");
    print_array_int(int_numbers, sizeof(int_numbers)/sizeof(int_numbers[0]));
    upo_bubble_sort(int_numbers, sizeof(int_numbers)/sizeof(int_numbers[0]), sizeof(int_numbers[0]), int_cmp);
    printf("[1-2] Sorted int array:\n");
    print_array_int(int_numbers, sizeof(int_numbers)/sizeof(int_numbers[0]));
    printf("\n");

    char *string_vector[] = {"this", "is", "a", "preparation", "exercise"};
    printf("[1-5] Original string array:\n");
    print_array_string(string_vector, sizeof(string_vector)/sizeof(string_vector[0]));
    upo_bubble_sort(string_vector, sizeof(string_vector)/sizeof(string_vector[0]), sizeof(string_vector[0]), string_cmp);
    printf("[1-6] Sorted string array:\n");
    print_array_string(string_vector, sizeof(string_vector)/sizeof(string_vector[0]));
    printf("\n");

	return 0;
}

void upo_bubble_sort(void *base, size_t n, size_t size, int (*cmp)(void*,void*)){
 	unsigned char *basePtr = base;
 	int flag = 1;
 	while(flag){
 		flag = 0;
 		for(size_t i = 1; i<n; i++){
 			if(cmp(basePtr + (i-1)*size, basePtr + i*size) > 0){
 				swap(basePtr + (i-1)*size, basePtr + i*size, size);
 				flag = 1;
 			}
 		}
 	}



}
