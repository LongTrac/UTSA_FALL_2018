
;;; set_isa
;;;    Parameters:
;;;       pOS           - part of speech
;;;       word_list     - the list of word that passed in to identify as a "part of speech"
;;;
;;;    Purpose:
;;;       The macro will store the word as a part of speech in the hashtable 
;;;       for later determination of what part of speech the word is when parsing 
;;;       the request statement
;;;
;;;     Return:
;;;       when finish processing the list of word the macro will return T to indicate the task is done
;;;
;;;     Expansion Example:
;;;       Example: (set_isa article a an the)
;;;       Expansion:    T ;
;;;                     T
(defmacro set_isa (pOS &rest word_list)
    (do ( (word word_list (CDR word)) )
        ( (NULL word) T )
        ( putp (CAR word) word-dict pOS )
    )
)

;;; isa
;;;    Parameters:
;;;       word          - the word that need to be determine if it is a specific pOS
;;;       pOS           - part of speech in English 
;;;
;;;    Purpose:
;;;       The function will determine if the passed in word is the same pOS we are looking for
;;;
;;;     Return:
;;;       T if the word is the same POS we are looking for
;;;       NIL if it is not
;;;
(defun isa (word pOS)
    ( EQL pOS (getp word word-dict) )

)

;;; resetPartsOfSpeech
;;;    Parameters:
;;;       parse         - the parse-obj hash table
;;;       pOS           - all part of speeches of a request 
;;;
;;;    Purpose:
;;;       The function will reset the parse-obj hash table for each time we have a new request
;;;
;;;     Return:
;;;       NIL if we are done resetting the hash table
;;;
(defun resetPartsOfSpeech (parse &rest pOS )
    (do ( (speechPart pOS (CDR speechPart)) )
        ( (NULL speechPart ) NIL )
        ( putp (CAR speechPart) parse NIL )
    )
)

;;; resetPartsOfSpeech
;;;    Parameters:
;;;       parse         - The complete request that needed to be parse
;;;
;;;    Purpose:
;;;       The function will check each word from a request to make sure it is a valid request
;;;       depending on one of the two type we have
;;;
;;;     Return:
;;;       T if it is a valid request
;;;       NIL if invalid
;;;
(defun checkRequest (parse)
    (PROG () 
        ;verb
        (setf token (getToken parse) )
        (if (not (isa token 'verb)) (return NIL) )
        (putp 'verb parse-obj token )

        ;article or directObj (noun)
        (setf token (getToken parse) )
        (if (isa token 'article)
            (setf token (getToken parse)) )
        (if (not (isa token 'noun)) (return NIL) )
        (putp 'directObj parse-obj token )

        ;prep
        (setf token (getToken parse) )
        (if (not (isa token 'prep)) (return NIL) )
        (putp 'prep parse-obj token )
            
        ;article or indirectObj (noun)
        (setf token (getToken parse) )
        (if (isa token 'article)
            (setf token (getToken parse)) )
        (if (not (isa token 'noun)) (return NIL) )
        (putp 'indirectObj parse-obj token )
            
        ;qualIndirectPrep
        ;if the program can reach this point, for type 1 request it is done
        (setf token (getToken parse) )
        (if (EQL token NIL) (return T) )             
        (if (not (isa token 'prep)) (return NIL) )
        (putp 'QualIndirectPrep parse-obj token )

        ;article or qualIndirectObj (noun)
        (setf token (getToken parse) )
        (if (isa token 'article)
            (setf token (getToken parse)) )
        (if (not (isa token 'noun)) (return NIL) )
        (putp 'QualIndirectObj parse-obj token )

        ;optional NOT
        (setf token (getToken parse) )
        (if (EQL token 'not)
            (putp 'NotQual parse-obj token) )
        (if (EQL token 'not)
            (setf token (getToken parse)) )

        ;Comparator
        
        (if (not (isa token 'comparator)) (return NIL) )
        (putp 'comparator parse-obj token )

        ;QualPrep
        (setf token (getToken parse) )
        (if (isa token 'prep) 
            (putp 'QualPrep parse-obj token) )
        (if (isa token 'prep) 
            (setf token (getToken parse) ) )

        ;QualValue
        (putp 'QualValue parse-obj token )

        (return T)
    )
)