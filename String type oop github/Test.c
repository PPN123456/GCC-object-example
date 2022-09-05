#include <stdio.h>

#include "m1Types.h"

//Tested on gcc (tdm64-1) 10.3.0 
int main(void){
	
	m1String* str1 = m1String_newReadFromFile("testText.txt");
	//if 'direct' str->length
	//error: invalid use of incomplete typedef 'm1String' 
	//kind of protection( or private access )
	
	//printf("File size: %llu bytes\n", str1->length);
	//access from 'set' 'get'
	printf("File size: %llu bytes\n", m1String_getLength(str1) );
    printf("File content:\n%s", m1String_getValue(str1));
    
    m1String* first20 = m1String_new( m1String_getSlice(str1, 0, 20)) ;
    printf("\n\nFirst 20 characters:\n'%s'\n", m1String_getValue( first20 ) );
    
    m1String_toUpper(first20);
	printf("Uppercase:\n'%s'\n", m1String_getValue( first20 ) );
	
	m1String_toLower(first20);
	printf("Lowercase:\n'%s'\n",  m1String_getValue( first20 ) );
	
	m1String_mod(first20, '*', "3"); //first20 = first20 + first20 + first20 //"3"
	printf("String multiplier:\n'%s'\n",  m1String_getValue( first20 ) );
	
	m1String_saveToFile(first20, "outText.txt");
	m1String_free(&str1);
	m1String_free(&first20);
	system("pause");
	return 0;
}



// slice to much memory use need optimize algorithm
