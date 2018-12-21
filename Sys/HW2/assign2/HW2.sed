        # print out comments first then delete them in the buffer so changes can not affect them    
/\/\*/,/\*\//p
/\/\*/,/\*\//d
/(^ +\/\/)/p
/(^ +\/\/)/d

        # Binary operators should always surrounded by a single space on either side (including assignment
        # and Boolean). Only the following operators must be accounted for: +, -, *, /, =, ==,
        # <=, >=, <, >.

/(^#include)/!s/((\+)|(-)|(\*)|(\/)|(<=)|(>=)|(==)|>|<|=)/ \1 /g

        # Conditions should not have whitespace immediately inside of the parentheses.
s:\( +:\(:g
s: +\):\):g

        # No more than one space between tokens.
s/[ \t]+/ /2g  
         #reduce to 1 whitepace per token and ignoring leading spaces and tab


        # No trailing whitespace after a line.
s/ +$//g                              
	 #delete trailing whitepace
