
;;; +=
;;;    Parameters:
;;;       numericVariable - a numeric value that get add in at the end
;;;       incrementValue - the increment value to add to the numeric value    
;;;    Purpose:
;;;       imitate the += function in other language that add up 2 number and 
;;;       use that to assign to the numeric value
;;;    Expansion Example:
;;;       Example:  (setf x 10 y 5)  
;;;                 (+= x 5)
;;;       Expansion:(SETF X (EVAL (+ X 5))) ;

(defmacro += (numericVariable incrementValue)
    `(setf ,numericVariable (+ ,numericVariable ,incrementValue) )
)

;;; iterate
;;;    Parameters:
;;;       index - the looping variable
;;;       body - contain:
;;;                 beginValueExpr - The beginning value to start the iteration (car body) 
;;;                 endValueExpr - The end value to stop the iteration (cadr body)
;;;                 incrExpr - The increment by value (caddr body)
;;;                 all the expressions that is in the body of the call (cdddr body)
;;;    Purpose:
;;;       imitate the iterate in other language that iterate through the index from
;;;       the start to the end and increase by a number that is specified by the user
;;;       iterate is passed a controlVariable which is used to count from beginValueExpr to endValueExpr (inclusive)
;;;       by the specified increment
;;;
;;;     Return:
;;;       The functional value of iterate will be T.  
;;;
;;;     Expansion Example:
;;;       Example:(iterate i 1 5 1
;;;                  (print (list 'one i))
;;;                )
;;;       Expansion: (PROG
;;;                      ((BEG (EVAL (CAR '(1 5 1 (PRINT (LIST 'ONE I)))))) 
;;;                      (END (EVAL (CADR '(1 5 1 (PRINT (LIST 'ONE I))))))
;;;                      (INCR (EVAL (CADDR '(1 5 1 (PRINT (LIST 'ONE I)))))))
;;;                      (DO ((I BEG (+= I INCR))) ((> I END) T) (PRINT (LIST 'ONE I))) 
;;;                      (RETURN T)) ;

    (defmacro iterate (index &rest body)
        `(PROG ( (beg (eval (car ' ,body)))
                 (end (eval (cadr ' ,body)))
                 (incr (eval (caddr ' ,body)))  )
            (do ( (,index beg (+ ,index incr)) )
                ( (> ,index end) T )
                ,@ (cdddr body)
            )
            (return T )
        )
    )


