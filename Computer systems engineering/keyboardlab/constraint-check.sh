#!/bin/sh

for pattern in w W '\\'
do
	echo [CHECKING] ${pattern} in problem1.c
	grep "${pattern}" problem1.c | grep -v "^\s*\*" | grep -v "/\*.*${pattern}" | grep -v "//.*${pattern}"
	grep -e ".*${pattern}.*/\*" -e ".*${pattern}.*//" -e "\*/.*${pattern}" problem1.c
done

for pattern in w W '\\' 1 2 3 4 5 6 8 ! @ '\#' '\$' % '\^' '\*' s S f F
do
	if [ $pattern == f ] || [ $pattern == F ]
	then
		echo [CHECKING] ${pattern} in problem2.c \(up to one if-statement per function allowed\)
	else
		echo [CHECKING] ${pattern} in problem2.c
	fi
	if [ $pattern == '\*' ]
	then
		grep "${pattern}" problem2.c | grep -v "^\s*\*" | grep -v "/\*.*${pattern}" | grep -v "//.*${pattern}" | grep -v "/\*"
	else
		grep "${pattern}" problem2.c | grep -v "^\s*\*" | grep -v "/\*.*${pattern}" | grep -v "//.*${pattern}"
	fi
	grep -e ".*${pattern}.*/\*" -e ".*${pattern}.*//" -e "\*/.*${pattern}" problem2.c
done

for pattern in + - / 5 %
do
	echo [CHECKING] ${pattern} in problem3.c
	if [ $pattern == / ]
	then
		grep "${pattern}" problem3.c | grep -v "^\s*\*" | grep -v "/\*.*${pattern}" | grep -v "//.*${pattern}" | grep -v "sprintf(value_text, \"%d\", value)" | grep -v "/\*" | grep -v "//"
	else
		grep "${pattern}" problem3.c | grep -v "^\s*\*" | grep -v "/\*.*${pattern}" | grep -v "//.*${pattern}" | grep -v "sprintf(value_text, \"%d\", value)"
	fi
	grep -e ".*${pattern}.*/\*" -e ".*${pattern}.*//" -e "\*/.*${pattern}" problem3.c
	grep -e ".*${pattern}.*sprintf(value_text, \"%d\", value)" -e "sprintf(value_text, \"%d\", value).*${pattern}" problem3.c
done

