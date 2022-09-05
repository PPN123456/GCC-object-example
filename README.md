# GCC-object-example

String object example ,with private access to struct members.<br />
E.g.  <br />
printf("String length:%llu\n", string->length ); <br />
error: invalid use of incomplete typedef 'm1String' <br />
Protect ( or private access ) of  struct member 'length'.<br />
Access from 'set' 'get' functions <br />
printf("String length:%llu\n", m1String_getLength(stringName) )
