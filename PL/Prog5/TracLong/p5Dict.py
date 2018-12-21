#!/usr/bin/env python3
import os
import sys
import re

#========================================================================================================
#   declareVar
#   Purpose:
#       Function declareVar puts the tokens of a line into correct dictionaries for 
#           dataType and value using the variable's name as a key everytime a VAR statement is called 
#   Parameters:
#       tokenM      -   The VAR statement that contain anything else beside 'VAR'     
#       varTypeD    -   The dictionary for variable's type
#       varValued   -   The dictionary for variable's value 
#   Return: 
#       None
def declareVar (tokenM, varTypeD, varValueD ):
    varTypeD[tokenM[1].upper()] = tokenM[0]
    #   if the token's length is not exactly 3 meaning that 
    #   it doesn't have the initial value
    if(len(tokenM)==3):
        varValueD[tokenM[1].upper()] = tokenM[2].strip('"')
    else:
        varValueD[tokenM[1].upper()] = ""

#========================================================================================================
#   declareLabel
#   Purpose:
#       Function declareVar puts the line number of a label statement into 
#           labelD dictionary using the label's name as the key
#   Parameters:
#       tokenM      -   The label statement  
#       line_number -   The line number of that label
#       labelD      -   The dictionary that holds all labels' line number
#   Return: 
#       None

def declareLabel (tokenM, line_number,labelD):
    #   split the label base on ":"
    label_name = tokenM.split(":")[0].strip().upper()
    error_msg = "***Error: label '{}' appears on multiple lines: {} and {}"

    if label_name in labelD.keys():
        print(error_msg.format(label_name,labelD[label_name],line_number))
    else:
        labelD[label_name] = line_number
                                                                            
#========================================================================================================
#   printVariables
#   Purpose:
#       To report the list of variables alphabetically bby using 3 different columns:
#           variable's name, data type and value
#   Parameters:
#       varTypeD    -   The dictionary for variable's type
#       varValued   -   The dictionary for variable's value 
#   Return: 
#       None
def printVariables(varTypeD,varValueD):
    print('Variables:')
    print('\t{:15}{:10}{:10}'.format('Variable', 'Type', 'Value'))
    for key in sorted(varTypeD):
        print('\t{:<15}{:<10}{:<10}'.format(key, varTypeD[key], varValueD[key]))

#========================================================================================================
#   printVariables
#   Purpose:
#       To report the list of label alphabetically bby using 2 different columns:
#           labels's name and line number 
#   Parameters:
#       labelD      -   The dictionary that holds all labels' line number
#   Return: 
#       None
def printLabels (labelD):
    print('Labels:')
    print('\t{:15}{:10}'.format('Label', 'Statement'))
    for key in sorted(labelD):
        print('\t{:<15}{:<10}'.format(key,labelD[key]))