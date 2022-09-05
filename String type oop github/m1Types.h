#ifndef __M1_TYPES_H__
#define __M1_TYPES_H__


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define PUBLIC
#define PRIVATE  
#define ULLONG unsigned long long 
typedef struct m1String m1String;


PUBLIC m1String*     m1String_new       	  ( const char *str);
PUBLIC m1String*     m1String_newReadFromFile ( const char *fileName);
PUBLIC void 	     m1String_free	    	  ( m1String** st);

// SET
PUBLIC void       m1String_setValue  ( m1String  *st, const char *str);
//GET
PUBLIC ULLONG        m1String_getLength  ( m1String *st);
PUBLIC const char*   m1String_getValue   ( m1String *st);
PUBLIC char*         m1String_getSlice   ( m1String *st,  int begin, int end);

PUBLIC void          m1String_saveToFile   ( m1String *st, const char *fileName);
PUBLIC void			 m1String_toUpper      ( m1String *st);
PUBLIC void			 m1String_toLower  	   ( m1String *st);
PUBLIC void          m1String_mod          ( m1String *st, char mod, const char* str);

#endif
