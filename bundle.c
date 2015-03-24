#include<stdlib.h>
#include "bundle.h"
Bundle* bundle_new(int size){
	Bundle* output=(Bundle*)malloc(sizeof(Bundle));
	output->objects=(void**)malloc(sizeof(void*)*size);
	output->objectCount=size;
	return output;
}
void bundle_free(Bundle* this){
	free(this->objects);
	free(this);
}
