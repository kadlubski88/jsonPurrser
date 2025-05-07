# jsonPurrser
A simple json parser library in c.

Short jp. 

## usage
The json string is given trough a pipe.
The key is given as a argument.
For example:
~~~bash
$ echo '{"key_1":{"key_11":"value_11"}}' | ./jp key_1.key_11
value_11
~~~
If the key doesn't exists, the string "NULL" is returned:
~~~bash
$ echo '{"key_1":"value_1"}' | ./jp key_2
NULL
~~~
## compilling
To compile the application use the make command

## todo
- array
- bool
- null


