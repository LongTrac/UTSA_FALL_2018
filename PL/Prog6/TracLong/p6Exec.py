#!/usr/bin/env python3
import os
import sys
import re
import traceback
from p5Dict import declareVar
from VarNotDef import VarNotDefined
from TooFewOp import TooFewOperands
from LabNotDef import LabelNotDefined
from InvalidExp import InvalidExpression
from InvalidValType import InvalidValueType

#========================================================================================================
#   execute
#   Purpose:
#       To execute the BEEP source code
#   Parameters:
#       varTypeD    -   The dictionary for variable's type
#       varValueD   -   The dictionary for variable's value  
#       labelD      -   The dictionary for labels
#       lines       -   The BEEP source code that was read from the file
#       args        -   The arguments list from sys.args
#   Return: 
#       None
def execute (varTypeD, varValueD, labelD, lines, args):
    #   line_exe_counter    -   keep track of how many line is executing
    #   line_num            -   the real line number that correspond with each line of code
    #   i                   -   the index to access the line from the list "lines"
    #                           i = line - 1
    line_exe_counter = 0
    line_num = 0
    i = 0
    print("execution begins ...")

    #   for each line of the BEEP code: execute them
    #       token_list      -   the list of token in 1 line
    #   
    #   This while loop is used to execute line by line
    while i < len(lines):
        line_num = i+ 1
        line = lines[i].strip()
        token_list = lines[i].split()

        #   calculate LINE_EXE_COUNTER 
        #   if the line is not a comment or an empty line: line_exe_counter +1
        if not (len(token_list) == 0):
            line_exe_counter += 1

        #   Verbose option
        if args[-1] == '-v':
            print('executing line {}: {}'.format(line_num,line))


        #   if the line is blank: ignore during execution
        if len(token_list) == 0:
            i+=1
            continue
        #   if it start with VAR ignore those lines during execution
        elif token_list[0] == "VAR":
            i += 1
            continue

        #   if it start with PRINT or a label that is followed by a PRINT statement
        if token_list[0].upper() == 'PRINT':
            print_beep(token_list[1:],varTypeD, varValueD, labelD,line_num)
        if token_list[1].upper() == 'PRINT':
            print_beep(token_list[2:],varTypeD, varValueD, labelD,line_num)

        #   if it start with ASSIGN or a label that is followed by an ASSIGN statement
        if token_list[0].upper() == 'ASSIGN':
            assign_beep(token_list[1:],varTypeD, varValueD, labelD,line_num)
        if token_list[1].upper() == 'ASSIGN':
            assign_beep(token_list[2:],varTypeD, varValueD, labelD,line_num)

        #   if it start with IF or a label that is followed by an IF statement
        if token_list[0].upper() == 'IF' or token_list[1].upper() == 'IF':
            if token_list[0].upper() == 'IF':
                temp_line = if_beep(token_list[1:],varTypeD, varValueD, labelD,line_num)
            if token_list[1].upper() == 'IF':
                temp_line = if_beep(token_list[2:],varTypeD, varValueD, labelD,line_num)
            if isinstance(temp_line, int):
                line_num = temp_line-2
                i = temp_line-2

        #   if it start wit a GOTO or a label follwed by a GOTO:
        if token_list[0].upper() == 'GOTO' or token_list[1].upper() == 'GOTO':
            if token_list[0].upper() == 'GOTO':
                temp_line = goto_beep(token_list[1:],varTypeD,varValueD,labelD,line_num)
            if token_list[1].upper() == 'GOTO':
                temp_line = goto_beep(token_list[2:],varTypeD,varValueD,labelD,line_num)
            if isinstance(temp_line, int):
                line_num = temp_line-2
                i = temp_line-2   

        #   increment the index of accessing lines
        i += 1

        #   check if infinite loop then limit it to at most 5k lines
        if line_exe_counter >= 5000:
            break

    print('execution ends, {} lines executed'.format(str(line_exe_counter)))

#========================================================================================================
#   print_beep()
#   Purpose:
#       To execute the Print Statement of the BEEP code
#   Parameters:
#       p_format    -   The format of printing: The list of all things that came 
#                       after the PRINT first word. It include string format and
#                       variables
#       varTypeD    -   The dictionary for variable's type
#       varValueD   -   The dictionary for variable's value  
#       labelD      -   The dictionary for labels 
#       lineNum     -   line number of that beep code
#   Return: 
#       None
def print_beep(p_format,varTypeD, varValueD, labelD,lineNum):
    for token in p_format:
        #   if token start and end with "" print directly without the ""
        if token[0] == token[-1] =='"':
            print(token.strip('"'),end = ' ')
        #   if it is not a literal string from the BEEP code then it is a variable
        else:
            #   check if varible exist if not then out as execption
            if check_var_exist(token.upper(),varValueD,lineNum):
                print(varValueD[token.upper()],end = ' ')
    print()       
            
#========================================================================================================
#   assign_beep()
#   Purpose:
#       To execute the ASSIGN statement of the BEEP source code
#   Parameters:
#       arg_list    -   The list of arguments that go after assign
#            [0]    -       The variable that we want to assign 
#            [1]    -       Could be a var literal or an opration
#            [2][3] -       if [1] is an operation then [2] and [3] are operands 
#       varTypeD    -   The dictionary for variable's type
#       varValueD   -   The dictionary for variable's value  
#       labelD      -   The dictionary for labels 
#       lineNum     -   line number of that beep code
#   Return: 
#       None

def assign_beep(arg_list,varTypeD,varValueD,labelD,lineNum):
    #print("\nASSIGN BEEP "+str(lineNum))
    #print(arg_list)
    try:
        #   check to see if the first token is already exist
        #   if yes:
        if check_var_exist(arg_list[0],varValueD,lineNum):
            #   evaluate the rest of the expression:
            #   if len == 2 meaning that this is a varliteral assignment
            if len(arg_list)==2:
                #   take the return of var_literal, create formated list like tokenM
                #   in declareVar function to call declareVar to assign new value
                #   call declareVar to re assign the variable
                #   else meaning that the varliteral is an undefined variable: just return
                var_literal = check_varLiteral(arg_list[1],varValueD,varTypeD,lineNum)
                if var_literal != False:
                    tokenM = [varTypeD[arg_list[0].upper()],arg_list[0].upper(),str(var_literal)]
                    declareVar(tokenM,varTypeD,varValueD)
                else:
                    return
            #   if it is an expression + - * > >= &
            elif len(arg_list)==4:
                #   if it successfully evaluate: create a tokenM list like in declareVar and reassign
                #   else return
                new_value = evaluate(arg_list[1:],varValueD,varTypeD,lineNum)
                if new_value != None:
                    tokenM = [varTypeD[arg_list[0].upper()],arg_list[0].upper(),str(new_value)]
                    declareVar(tokenM,varTypeD,varValueD)
                else:
                    return
            #   if we have 3 operand: too few arg
            elif len(arg_list) == 3:
                raise TooFewOperands('Too few operand for this operation') 
            #   if we have more than 4 operand then we have to many
            elif len(arg_list) >= 4:
                raise InvalidExpression ('Too many operands for this operation')
        #   if the first token doesn't exist then it fail to assign
        else:
            return

    except (TooFewOperands,InvalidExpression) as e:
        print ("\n*** line %d error detected ***" % (lineNum))
        print("%-10s %d *** %s ***" % (" ", lineNum, str(e.args[1])))
        return     
    except Exception as e:
        print("\n*** line %d error detected ***" % (lineNum))
        print(e)
        return 
    except:
        print("\n*** line %d error detected ***" % (lineNum))
        traceback.print_exc()
        return 
    

#========================================================================================================
#   if_beep()
#   Purpose:
#       To execute the IF statement of the BEEP source code
#   Parameters:
#       arg_list    -   The list of arguments that go after IF 
#            [0]    -       an opration
#            [1][2] -       operands 
#            [3]    -       a label
#       varTypeD    -   The dictionary for variable's type
#       varValueD   -   The dictionary for variable's value  
#       labelD      -   The dictionary for labels 
#       lineNum     -   line number of that beep code
#   Return: 
#       a line number if the label exist in the labelD dictionary

def if_beep(arg_list, varTypeD, varValueD, labelD, lineNum):
    try:
        #   if we have too many expression : raise exception
        if len(arg_list) > 4:
            raise InvalidExpression ('Too many Tokens')
        
        #   if evaluate return a boolean and it is true
        condition = evaluate (arg_list[:3],varValueD,varTypeD,lineNum)
        if isinstance(condition,bool) and condition == True:
            #   check if the label exist or not: 
            #       if yes then return the line number store in the dictionary
            #       if not then raise exception
            if arg_list[3].upper() in labelD.keys():
                return int(labelD[arg_list[3].upper()])
            else:
                raise LabelNotDefined ("'%s' is not defined as a label" % str(arg_list[3]))
        else:
            return    
    except (LabelNotDefined,InvalidExpression) as e:
        print ("\n*** line %d error detected ***" % (lineNum))
        print("%-10s %d *** %s ***" % (" ", lineNum, str(e.args[1])))
        return     
    except Exception as e:
        print("\n*** line %d error detected ***" % (lineNum))
        print(e)
        return 
    except:
        print("\n*** line %d error detected ***" % (lineNum))
        traceback.print_exc()
        return 

#========================================================================================================
#   goto_beep()
#   Purpose:
#       To execute the GOTO statement of the BEEP source code
#   Parameters:
#       arg_list    -   The list that should only contain a label in [0]
#       varTypeD    -   The dictionary for variable's type
#       varValueD   -   The dictionary for variable's value  
#       labelD      -   The dictionary for labels 
#       lineNum     -   line number of that beep code
#   Return: 
#       a line number if the label exist in the labelD dictionary

def goto_beep (arg_list,varTypeD, varValueD, labelD, lineNum):
    try:
        #   check for too many token exception
        if len(arg_list) > 1:
            raise InvalidExpression('Too many Tokens')
        #   check if the label exist or not: 
        #       if yes then return the line number store in the dictionary
        #       if not then raise exception
        if arg_list[0].upper() in labelD.keys():
            return int(labelD[arg_list[0].upper()])
        else:
            raise LabelNotDefined ("'%s' is not defined as a label" % str(arg_list[0]))

    except (LabelNotDefined,InvalidExpression) as e:
        print ("\n*** line %d error detected ***" % (lineNum))
        print("%-10s %d *** %s ***" % (" ", lineNum, str(e.args[1])))
        return     
    except Exception as e:
        print("\n*** line %d error detected ***" % (lineNum))
        print(e)
        return 
    except:
        print("\n*** line %d error detected ***" % (lineNum))
        traceback.print_exc()
        return     


#========================================================================================================
#   check_var_exist()
#   Purpose:
#       To check if a variable has already been defined
#   Parameters:
#       var         -   The variable we need to check id exist
#       varValueD   -   The dictionary for variable's value  
#       lineNum     -   line number of that beep code
#   Return: 
#       True        -   if exist
#       False       -   otherwise
def check_var_exist (var,varValueD,lineNum):
    var = var.upper()
    try:
        #   if var not exist raiase a exception else return T
        if var not in varValueD.keys():
            raise VarNotDefined("'%s' is not defined" % (var))
        else:
            return True
    except VarNotDefined as e:
        print ("\n*** line %d error detected ***" % (lineNum))
        print("%-10s %d *** %s ***" % (" ", lineNum, str(e.args[1])))
        return False    
    except Exception as e:
        print("\n*** line %d error detected ***" % (lineNum))
        print(e)
        return False
    except:
        print("\n*** line %d error detected ***" % (lineNum))
        traceback.print_exc()
        return False

#========================================================================================================
#   check_varLiteral()
#   Purpose:
#       check what type of var_literal is that and return the corespond answers
#   Parameters:
#       token       -   The var literal we need to check
#       varValueD   -   The dictionary for variable's value  
#       lineNum     -   line number of that beep code
#   Return: 
#       String literal      -   if token is a string that enquoted ""
#       an integer          -   if the token is a number
#       a value             -   if the token is a variable that is defined
#       False               -   if the token is a var that was not defined 
def check_varLiteral (token, varValueD, varTypeD ,lineNum):
    try:
        if token.upper() in varValueD.keys():
            return varValueD[token.upper()]
        elif token.isdecimal():
            return int(token)
        elif token[0] == "\"":
            return token.replace("\"","")
        else:
            raise VarNotDefined("'%s' is not defined" % (token))
    except VarNotDefined as e:
        print ("\n*** line %d error detected ***" % (lineNum))
        print("%-10s %d *** %s ***" % (" ", lineNum, str(e.args[1])))
        return False    
    except Exception as e:
        print("\n*** line %d error detected ***" % (lineNum))
        print(e)
        return False
    except:
        print("\n*** line %d error detected ***" % (lineNum))
        traceback.print_exc()
        return False


#========================================================================================================
#   evaluate()
#   Purpose:
#       Evaluate the expression for the BEEP code
#   Parameters:
#       tokens      -   The list of operation and operands  
#           [0]     -       The operation
#           [1][2]  -       The operands 
#       varValueD   -   The dictionary for variable's value  
#       varTypeD    -   The dictionary for variable's type
#       lineNum     -   line number of that beep code
#   Return: 
#       a boolean   -   for the > and >= evaluation
#       an int      -   for + - evaluataion
#       a string    -   for * and & for replica and concatnation 
#       False       -   if exception

def evaluate(tokens, varValueD, varTypeD, lineNum):
    try:
        #   if it is the * then check var literal and var num ================
        if tokens[0] == '*':
            #   if op2 is in the dictionary then take the value if not then it could be a number directly
            #   then check if it can be cast to int
            if tokens[2].upper() in varValueD.keys():
                op2 = varValueD[tokens[2].upper()]
            else:
                op2 = tokens[2]
            try:
                op2 = int (op2)
            except:
                raise InvalidValueType ("'%s' is not numeric" % (tokens[2]))
            #   evaluate the var literal in operand 1
            op1 = check_varLiteral(tokens[1],varValueD,varTypeD,lineNum)
            if op1 == False:
                return
            #   return the string replica
            return str(op1) * op2

        #   if it is + check both for var num ================================  
        elif tokens[0] == '+':
            op1 = evalNum(tokens[1],varValueD,lineNum)
            op2 = evalNum(tokens[2],varValueD,lineNum)
            return int(op1) + int(op2)

        #   if it is - check both for var num =================================
        elif tokens[0] == '-':
            op1 = evalNum(tokens[1],varValueD,lineNum)
            op2 = evalNum(tokens[2],varValueD,lineNum)
            return int(op1) - int(op2)

        #   if it is > check both for var num =================================
        elif tokens[0] == '>':
            op1 = evalNum(tokens[1],varValueD,lineNum)
            op2 = evalNum(tokens[2],varValueD,lineNum)
            return int(op1) > int(op2)

        #   if it is > check both for var num =================================
        elif tokens[0] == '>=':
            #TODO: greater_eql fucntion
            op1 = evalNum(tokens[1],varValueD,lineNum)
            op2 = evalNum(tokens[2],varValueD,lineNum)
            return int(op1) >= int(op2)        

        #   if it is & check both for var num =================================
        elif tokens[0] == '&':
            op1 = check_varLiteral(tokens[1],varValueD,varTypeD,lineNum)
            op2 = check_varLiteral(tokens[2],varValueD,varTypeD,lineNum)
            if op1 == False or op2 == False:
                return
            return str(op1) + str(op2)
        else:
            raise InvalidExpression ("'%s' is an Unknown operator" % (str(tokens[0])))
    except (InvalidValueType, InvalidExpression) as e:
        print ("\n*** line %d error detected ***" % (lineNum))
        print("%-10s %d *** %s ***" % (" ", lineNum, str(e.args[1])))
        return     
    except Exception as e:
        print("\n*** line %d error detected ***" % (lineNum))
        print(e)
        return 
    except:
        print("\n*** line %d error detected ***" % (lineNum))
        traceback.print_exc()
        return 

#========================================================================================================
#   evalNum()
#   Purpose:
#       Evaluate an operand to see if it is numeric or not
#   Parameters:
#       token       -   The operand
#       varValueD   -   The dictionary for variable's value  
#       lineNum     -   line number of that beep code
#   Return: 
#       op          -   the operand that had been change to int
def evalNum (token,varValueD,lineNum):
    token = token.upper()
    if token in varValueD.keys():
        op = varValueD[token]
    else:
        op = token
    try:
        op = int(op)
    except:
        raise InvalidValueType ("'%s' is not numeric" % (token))
    return op
