#include "m1Types.h"
//**** TYPE PRIVATE ****************
typedef struct m1String{
	ULLONG   length; 
	char     *value; 
	char 	 *slice; // last slice function result
}m1String;

#define CHECK_PTR( ptr) if(ptr==NULL) { \
			fprintf(stderr,"%s","WARNING: Access to a non-existent (not allocated) value"); \
			fprintf(stderr," in  %s\n",__FUNCTION__); \
			exit(0); \
		} \

//last string position 	for slice function	
#define  STR_LAST '$'


//************ METHODS ****************

//*********** PRIVATE  ****************
//access only in 'm1Types'
//PRIVATE void m1String_resize( m1String *st, long newSize);


//******** PUBLIC *********************

PUBLIC m1String* m1String_new(const char * str){
	m1String *new = malloc(sizeof(m1String));
	if(str !=NULL){
		ULLONG len = strlen(str);
		new->value =malloc( len * sizeof(char) +1); // +1 for \0
		new->slice =malloc( len * sizeof(char) +1);
		memset(new->slice, 0, len);
		new->length = len;
		sprintf(new->value, "%s", str);
		//printf("new setValue: '%s'\n", new->value);
	}else{
		new->length =0;
		new->value =NULL;
		new->slice =NULL;
		//sprintf(new->value,"%s", "");
	}
	return new;
}

PUBLIC void m1String_free( m1String** st){
	CHECK_PTR(*st);
	if((*st)->value !=NULL){
		free((*st)->value);
		(*st)->value = NULL;
	}
	if((*st)->slice !=NULL){
		free((*st)->slice);
		(*st)->slice = NULL;
	}
	free((*st));
	(*st)=NULL;
}

//******** SET **************
//PUBLIC void m1String_setValue( m1String **st, const char *str){
	//CHECK_PTR((*st));
	//m1String_free(st);
	//(*st) = m1String_new(str);
//}

PUBLIC void m1String_setValue( m1String *st, const char *str){
	CHECK_PTR(st);
	m1String_free(&st);
	st = m1String_new(str);
}

//********* GET *************
PUBLIC unsigned long long  m1String_getLength( m1String *st){
	//CHECK_PTR(st);
	return st->length;
}

PUBLIC const char*  m1String_getValue  ( m1String *st){
	CHECK_PTR(st);
	return st->value;
}

// if 'end' = '$'   'end' =st->length
PUBLIC char* m1String_getSlice( m1String *st, int begin, int end){
	CHECK_PTR(st);
	CHECK_PTR(st->slice);
	if(end == STR_LAST)
		end = st->length;

	if(begin <0){
		fprintf(stderr,"%s:%d", "WARNING: wrong slice 'begin' value", begin);
		fprintf(stderr, " in  %s\n",__FUNCTION__); 
		exit(0);
	}
	if(end > st->length){
		fprintf(stderr,"%s:%d", "WARNING: wrong slice 'end' value", end);
		fprintf(stderr,  " in  %s\n",__FUNCTION__); 
		exit(0);
	}
	if(begin > end ){
		fprintf(stderr,"%s:%d>%d", "WARNING: wrong slice position 'begin' more then 'end'",begin, end);
		fprintf(stderr,  " in  %s\n",__FUNCTION__); 
		exit(0);
	}
	ULLONG subSize = end - begin;
	if(st->slice ==NULL){
		st->slice =malloc(st->length * sizeof(char));
	}else{
		//clear 'slice'
		memset(st->slice, 0, st->length);
	}
	ULLONG i=0;
	for(i=0; i<subSize; ++i){
		st->slice[i] = st->value[begin+i];
	}
	st->slice[i]='\0';
	return st->slice;
	 
}

PUBLIC void m1String_toUpper  ( m1String *st){
	CHECK_PTR(st);
	CHECK_PTR(st->value);
	strupr(st->value);
}	

PUBLIC void m1String_toLower  ( m1String *st){
	CHECK_PTR(st);
	CHECK_PTR(st->value);
	strlwr(st->value);
}

//'mod' -> modding
// 'mod': '+'   concat   'st' and  'str'
//		  '-'   subtract 'str' from 'st'
//		  '*'   multiple 'st' by 'str'  // 'str'   should contain the multiplier >1: "2" "3"  etc.
//	
PUBLIC void m1String_mod ( m1String *st, char mod, const char* str){
	CHECK_PTR(st);
	CHECK_PTR(st->value);
	switch(mod){
		//concat 'st->value' and 'str'
		case '+':{
			char outStr[st->length + strlen(str) ];
			sprintf(outStr,"%s", st->value);
			strcat(outStr, str);
			m1String_free(&st);
			st = m1String_new(outStr);
		}break;
		
		//subtract 'str' from 'st->value'
		case '-':{
			//search 'str' in 'st->value'
			char *strPos = strstr(  st->value, str);
			int pos=-1;
			//if 'str' found
			//printf("strPos:%p\n", strPos);
			if(strPos !=NULL){
				//get position index in 'st->value'
				pos = strPos - st->value;
				int lenStr = st->length - pos;
				char buf[ lenStr ];
				memset(buf, 0, lenStr);
				int b =0;
				//save part before 'str'
				for( b=0; b< pos; ++b){
					buf[b] = st->value[b];
				}
				//save part after 'str'
				for(int i=pos+ strlen(str); i<  st->length; ++i){
					buf[b] = st->value[i];
					b++;
				}
				buf[b]='\0';
				//clear old 'value'
				memset(st->value,0, st->length);
				//set new 'value'
				sprintf(st->value, "%s", buf);
				st->length = strlen(buf);
			}
		}break;
		
		//multiple 'st->value' by 'str'
		case '*':{
			int mult;
			sscanf(str,"%d", &mult);
			//printf("multipler: %d\n", mult);
			if(mult >1){
				char buf[st->length];//save current value
				strcpy(buf, st->value);
				//printf("buf:'%s'\n", buf);
				for(int i=0; i< mult-1; ++i){ //-1 skip last 'concat'
					m1String_mod(st, '+', buf);
					//printf("value: '%s'\n", m1String_getValue(st));
				}
			}
			
		}break;
	}
	
}

PUBLIC m1String* m1String_newReadFromFile( const char *fileName){
	FILE *inFile = fopen(fileName, "rb");
	if(inFile ==NULL){
		fprintf(stderr, "Canno open-read file : '%s'\n", fileName);
		exit(0);
	}
	//if file not exist -> continue , repeat, exit ??
	//get file size
	fseek(inFile, 0,SEEK_END);
	size_t fileSize = ftell(inFile);
	fseek(inFile,0, SEEK_SET);
	//read file 
	char *buf = malloc(sizeof(char) *fileSize);
	fread( buf, fileSize, 1, inFile);
	m1String *new = m1String_new( buf );
	fclose(inFile);
	free(buf);
	return new;
}

 
PUBLIC void m1String_saveToFile( m1String *st, const char *fileName){
	CHECK_PTR(st);
	CHECK_PTR(st->value);
	FILE *outFile = fopen(fileName, "wb");
	if(outFile ==NULL){
		fprintf(stderr, "Canno open-write file : '%s'\n", fileName);
		exit(0);
	}
	fwrite(st->value, st->length, 1, outFile);
	fclose(outFile);
}


//*********** PRIVATE ********************

//resize st->value  with clear value
 
