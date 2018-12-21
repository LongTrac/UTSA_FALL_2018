;;; removeNILTop
;;;    Parameters:
;;;       L - a list of items that can contains embeded list
;;;    Purpose:
;;;       Returns a list that is completely free of NIL 
;;;		  on the top level only - if the list has embeded list,
;;;		  everthing else stays the same

(defun removeNILTop (L)
	( COND 	( (NULL L) NIL )
			( (NULL (CAR L)) (removeNILTop(CDR L)) )
			( T (CONS (CAR L) (removeNILTop(CDR L))) )	
	)
)
(print (removeNILTop '(NIL X NIL NIL Y  NIL Z)) )
(print (removeNILTop '(X NIL Y NIL Z NIL)) )
(print (removeNILTop '(NIL (X NIL Y) (NIL NIL))) )

;;; removeNILMost
;;;    Parameters:
;;;       L - a list of items that can contain embeded list
;;;    Purpose:
;;;       Returns a list that has NIL at any level removed
;;;	   Note:  
;;;		  If the result of removing NIL gives a NIL,
;;;       it is unnecessary to remove that resulting NIL.  

(defun removeNILMost (L)
	( COND	( (NULL  L)	NIL )
			( (NULL (CAR L)) (removeNILMost (CDR L)) )
			( (ATOM (CAR L)) (CONS (CAR L) (removeNILMost (CDR L))) )
			( T  (CONS (removeNILMost (CAR L)) (removeNILMost (CDR L))) )	
	)
)
(print (removeNILMost '(NIL X NIL NIL Y  NIL Z)) )
(print (removeNILMost '(X NIL (Y NIL Z) NIL)) )
(print (removeNILMost '(NIL (NIL) (X NIL Y) (NIL NIL) Z)) )
(print (removeNILMost '(NIL ( (((((NIL) NIL))))))) )

;;; reverseTop
;;;    Parameters:
;;;       L - a list of items that can contain embeded list
;;;    Purpose:
;;;       Returns a list that has the top level elements reversed   

(defun  reverseTop (L)
	( COND 	( (NULL L) NIL )
			( T (APPEND (reverseTop (CDR L)) (LIST (CAR L)) ) ) 
	)
)
(print (reverseTop '(X Y Z)) )
(print (reverseTop '(X (Y Z (A)) (W))) )

;;; reverseAll
;;;    Parameters:
;;;       L - a list of items that can contain embeded list
;;;    Purpose:
;;;       Returns a list that is reversed at all levels
;;;	   Note:  
;;;		  This function will reverse embeded lists first before 
;;;       reverse the outer levels

(defun reverseAll (L)
	( COND 	( (NULL L) NIL )
			( (ATOM (CAR L)) (APPEND (reverseAll(CDR L))(LIST (CAR L))) )
			( T (APPEND (reverseAll(CDR L))(LIST(reverseAll(CAR L)))) )
	)

)
(reverseAll '(X Y Z))
(reverseAll '(X (Y Z (A)) (W)))

;;; palindrome
;;;    Parameters:
;;;       L - a list of items that may or maynot have embeded lists
;;;    Purpose:
;;;       Returns T if the list is actually a palindrome
;;;       Returns NIL otherwise.
;;;    Note:
;;;		  Only need to work with the top level list

(defun palindrome (L)
	( COND 	( (EQUAL L (reverseTop L )) T ) 
			( T NIL )
	)
)
(palindrome '(R A C E C A R))
(palindrome '(W A S I T A C A R O R A C A T I S A W))
(palindrome '(N I X O N))

;;; removeNILAll
;;;    Parameters:
;;;       L - a list of items that can contain embeded list
;;;    Purpose:
;;;       Returns a list that has NIL at any level removed
;;;	   Note:  
;;;		  Unlike removeNilMost, This one actually remove all the NIL 
;;;       including the result of removing NIL gives a NIL.

(defun removeNILAll (L)
	( COND	( (NULL  L ) NIL )
			( (NULL (CAR L)) (removeNILAll (CDR L)) )
			( (ATOM (CAR L)) (CONS (CAR L) (removeNILAll (CDR L))) )
			( (NULL (CDR L)) (removeNILAll (CAR L)) )
			( (AND (NULL (CAAR L)) (NULL (CADR L))) (removeNILAll (CDR l)) )
			( (OR (NULL (CAAR L)) (NULL (CADR L))) (removeNILAll (CDR l)) )
			( (NULL (CDAR L)) (APPEND (removeNILAll (CAR L))(removeNILAll (CDR L))) )
			( T  (CONS (removeNILAll (CAR L)) (removeNILAll (CDR L))) )	
	)
)
(removeNILAll '(NIL (NIL) (X NIL Y) (NIL NIL) Z))
(removeNILAll '(NIL ( (((((NIL) NIL)))))))
(removeNILAll '(NIL (X (NIL) Y) ((NIL)) ))
(removeNILAll '(NIL (((X ((((((((((NIL)))))))))) Y) Z) W) (((NIL))) ))