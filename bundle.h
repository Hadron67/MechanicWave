#ifndef BUNDLE_H
#define BUNDLE_H
typedef struct _bundle{
	int objectCount;
	void** objects;
}Bundle;
Bundle* bundle_new(int size);
void bundle_free(Bundle*);
#endif
