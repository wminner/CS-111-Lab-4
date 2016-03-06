#include "SortedList.h"
#include <string.h>
#include <stdlib.h>

SortedList_t* SortedList_new(void) {
	SortedList_t *new_list = (SortedList_t*) malloc(sizeof(SortedList_t));
	new_list->prev = new_list;
	new_list->next = new_list;
	new_list->key = NULL;
	return new_list;
}

void SortedList_insert(SortedList_t *list, SortedListElement_t *element) {
	SortedListElement_t *ele_prev = list;
	SortedListElement_t *ele_next = list->next;
	while ( ele_next != list ) {
		// Compare keys and break if found target location (sorted ascending order)
		if ( strcmp(element->key, ele_next->key) <= 0 )
			break;
		// Go to next element in list
		ele_prev = ele_next;
		ele_next = ele_next->next;
	}
	// Found location, hook up pointers between current elements
	element->next = ele_next;
	element->prev = ele_prev;
	ele_next->prev = element;
	ele_prev->next = element;
}

int SortedList_delete( SortedListElement_t *element) {
	SortedListElement_t *ele_prev = element->prev;
	SortedListElement_t *ele_next = element->next;
	// Check pointers are valid
	if ( ele_next->prev != element )
		return 1;
	if ( ele_prev->next != element )
		return 1;
	// Rearrange pointers and set element pointers to NULL
	ele_next->prev = element->prev;
	ele_prev->next = element->next;
	element->prev = NULL;
	element->next = NULL;
	return 0;
}

SortedListElement_t *SortedList_lookup(SortedList_t *list, const char *key) {
	SortedListElement_t *element = list->next;
	while ( element && element != list ) {
		// Check if element has key
		if ( element->key == key )
			return element;
		element = element->next;
	}
	// Didn't find key
	return NULL;
}

int SortedList_length(SortedList_t *list) {
	SortedListElement_t *element = list;
	int count = 0;
	while ( element->next != list ) {
		// Check for valid pointers
		if (!element->next || !element->prev)
			return -1;
		// Increment count
		count++;
		element = element->next;
	}
	return count;
}