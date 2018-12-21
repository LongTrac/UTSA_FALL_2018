#!/usr/bin/env python3

#----------------------------------------------------------------------------------
#	    p6Driver.py by Long Trac
#   Purpose:
#	    - This program reads a file that contains many BEEP source code text lines. 
#	        Then it will read the text lines and place them in a homogeneous array 
#	        (i.e., list). It will populate associative arrays (dictionaries) with 
#	        information based on the BEEP source code.
#	    - after reading the BEEP source code, it should print the variables and
#	        their data types (and values if provided). Additionally, it should print 
#	        the labels and their locations.
#   Command Parameters:
#	    python3 p5Driver.py inputFile [-v]
#
#	    The input file should contains commands. There are commands which
#	        specify declaring a variable, and a label, assigning value to a variable,
#	        printing each of the specified variables or literals, branching to the
#	        specified label, returning True or False from a numerical comparison
#
#	VAR dataType variableName initialValue
#		This declares the specified variableName to be of the specified data 
#		type. If specified, the initialValue is the initial value for 
#		the variable variableName.
#	
#	# comment
#		This is a comment helps explain a BEEP program
#
#	label: statement
#		The subscript for this line of code is stored in the labelD dictionary.
#
#	statement is one of
#
#		- PRINT varLiteral1 varLiteral2
#			print each of the specified variables or literals
#
#		- ASSIGN variableName expression
#			assign the value of the expression to the specified
#			which must have been declared
#
#		- IF expression label
#			if the expression is True, branch to the specified label.
#			Otherwise, continue with the next statement.
#
#		- GOTO Label
#			branch to the specified label.
#
#	expression is one of:
#		varLiteral						(1)
#		*   varLiteral  varNumber		(2)
#		+   varNumber1  varNumber2		(3)
#		-   varNumber1  varNumber2		(4)
#		>   varNumber1  varNumber2		(5)
#		>=  varNumber1  varNumber2		(6)
#		&   varLiteral1 varLiteral2		(7)
#
#   Results:
#	    - All output is written to stdout.
#	    - If the same label is encountered at the beginning of two or more statements,
#	        show an error message. The message shows its first line number and other one.
#   Notes:
#	    This program will call functions from p5Dict.py and p6Exec.py
#----------------------------------------------------------------------------------

import os
import sys
import re
import traceback
from p5Dict import declareVar, declareLabel, printVariables, printLabels
from p6Exec import execute

#   Empty dictionary for:
#   varTypeD    -   The data type for the variable
#   varValueD   -   The initial value for the variable 
#   labelD      -   The line number for labels
varTypeD = {}
varValueD = {}
labelD ={}

#   extracting the arguments
args= list(x for x in sys.argv[:])
#args = ['p6Driver.py','p6InputL.txt']
#   assign the beep file
beep_file_name = args[1]

#   open for reading
try:
    with open(beep_file_name,'r', encoding='latin-1') as beep_file: 
        #   parse the items
        lines = beep_file.readlines()
except IOError:  
    print('ERROR OPENING A FILE')
except:
    print('SOMETHING UNEXPECTED HAPPENDED!!!')

#   Print the header output,    
#   For loop to output the file line by line start with line number relative to 1
#       Regex testing to call the appropriate function
#   
#   line_num        -   line number relative to 1

line_num = 0
print('BEEP source code in {}'.format(args[1]))
for line in lines:
    line_num += 1
    line = line.rstrip('\n')
    print('{}. {}'.format(line_num,line) )
    
    #   Regex testing for specific pattern
    var_pattern     = r'^\s*VAR.*$'
    label_pattern   = r'^\s*\S.+:\s*((ASSIGN)|(IF)|(PRINT)|(GOTO)|(assign)|(if)|(print)|(goto)).*$'

    #   searching for specific pattern (for prog 5, only need variable and label)
    #   VAR
    match = re.search(var_pattern, line)
    if match:
        #   split the line into a list of string 
        temp = line.split()[1:]
        declareVar (temp,varTypeD,varValueD)

    #   labels
    match = re.search(label_pattern, line)
    if match:
        declareLabel(line, line_num, labelD)

#   Print the variables and labels alphabetically
printVariables(varTypeD,varValueD)
printLabels(labelD)

#   Calling execution from p6Exect
execute(varTypeD, varValueD, labelD, lines, args)
