#!/usr/bin/env python3
#   p5Driver.py
#   Purpose:    
#       To execute the BEEP source code and store all variables and labels 
#           to their respective dictionary
#   Input:      
#       A file with BEEP source code 
#   Output:
#       The source code itself with line number on the left hand side
#       A list of variables that includes the variable's name, type and initial value (alphabetically)
#       A list of labels that includes the label's name, line number (alphabetically)

import os
import sys
import re
from p5Dict import declareVar, declareLabel, printVariables, printLabels

#   Empty dictionary for:
#   varTypeD    -   The data type for the variable
#   varValueD   -   The initial value for the variable 
#   labelD      -   The line number for labels
varTypeD = {}
varValueD = {}
labelD ={}

#   extracting the arguments
args= list(x for x in sys.argv[:])

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
    comment_pattern = r'^\s*#.*$'
    label_pattern   = r'^\s*\S.+:\s*((ASSIGN)|(IF)|(PRINT)|(GOTO)|(assign)|(if)|(print)|(goto)).*$'
    stat_pattern    = r'^\s*((ASSIGN)|(IF)|(PRINT)|(GOTO)|(assign)|(if)|(print)|(goto)).*$'

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
