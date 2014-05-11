//
//  main.c
//  cmsketch
//
//  Created by Chris Guiney on 4/16/14.
//  Copyright (c) 2014 Chris Guiney. All rights reserved.
//

#include <stdio.h>
#include "../include/sketch_t.h"
#include "../include/dbg.h"
int int_cmp_asc(void* a, void* b) {
	return (int) a - (int) b;
}

int main(int argc, const char * argv[])
{

	printf("Starting\n");
	sketch_t *s = sketch_create(0.5, 0.5, 100);
	
	if(s == NULL) {
		printf("Error creating sketch");
		return 1;
	}
	
	sketch_destroy(s);
	
	// insert code here...
	printf("Hello, World!\n");
	return 0;
}

