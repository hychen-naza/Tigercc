grammar tiger;

program : MAIN LET declarationSegment IN BEGIN statSeq END ;    

declarationSegment : typeDeclarationList varDeclarationList functionDeclarationList ;

typeDeclarationList :   
					| typeDeclaration typeDeclarationList
					;

varDeclarationList : 
				   | varDeclaration varDeclarationList
				   ;

functionDeclarationList : 
				   		| functionDeclaration functionDeclarationList
				   		;

typeDeclaration : TYPE ID ASSIGN type SEMI ; 
type : typeId
	 | ARRAY LBRACK INTLIT RBRACK OF typeId	
	 | ID
	 ;		
typeId : INT | FLOAT ;

varDeclaration : VAR idList COLON type optionalInit SEMI ;
idList : ID
	   | ID COMMA idList
	   ;
optionalInit : 
			 | ASSIGN constRule
			 ;

functionDeclaration : FUNCTION ID LPAREN paramList RPAREN retType BEGIN statSeq END SEMI ;
paramList : 
		  | param paramListTail
		  ;
paramListTail : 
			  | COMMA param paramListTail
			  ;	  
retType : 
		| COLON type
		;
param : ID COLON type ;

statSeq : stat
		| stat statSeq
		;

stat : <assoc=right> lvalue ltail ASSIGN expr SEMI 
	 | scopeControlFlow
	 | optPrefix ID LPAREN exprList RPAREN SEMI
	 | BREAK SEMI 
	 | RETURN expr SEMI 
	 | LET declarationSegment IN statSeq END SEMI 
	 ;
scopeControlFlow : IF expr THEN statSeq statTail
	 			 | WHILE expr DO statSeq ENDDO SEMI
	 			 | FOR ID ASSIGN expr TO expr DO statSeq ENDDO SEMI
	 			 ;
statTail : ENDIF SEMI
	 	 | ELSE statSeq ENDIF SEMI
	 	 ;
ltail : ASSIGN lvalue ltail
	  |
	  ;
optPrefix : lvalue ASSIGN
		  |
		  ;

//按优先级排列
expr : LPAREN expr RPAREN 	  
	 | <assoc=right> expr EXP expr
	 | expr (MULT | DIV) expr
	 | expr (PLUS | MINUS) expr
	 | expr (EQ | NEQ | GREATER | LESSER | GREATEREQ | LESSEREQ) expr
	 | expr AND expr
	 | expr OR expr
	 | constRule 
	 | lvalue 
	 ;

constRule : INTLIT | FLOATLIT ;


exprList : 
		 | expr exprListTail
		 ;
exprListTail : COMMA expr exprListTail
			 |
			 ;		 
lvalue : ID lvalueTail;
lvalueTail : LBRACK expr RBRACK
		   |
		   ;


COMMENT : '/*' .*? '*/' -> skip ;
INTLIT : '0' | [1-9]([0-9])* ;
FLOATLIT : [0-9]+ '.' [0-9]* ;


COMMA : ',' ;
COLON : ':' ;
SEMI : ';' ;
LPAREN : '(' ;
RPAREN : ')' ;
LBRACK : '[';
RBRACK : ']';
LBRACE : '{';
RBRACE : '}';


PLUS : '+' ;
MINUS : '-' ;
MULT : '*' ;
DIV : '/' ;
EXP : '**' ;
EQ : '==' ;
NEQ : '!=' ;
LESSER : '<' ;
GREATER : '>' ;
LESSEREQ : '<=' ;
GREATEREQ : '>=' ;
AND : '&' ;
OR : '|' ;
ASSIGN : ':=' | '=' ;



MAIN : 'main' ;             
ARRAY : 'array' ; 
BREAK : 'break' ; 
DO : 'do' ;
ELSE : 'else' ;
FOR : 'for' ;
FUNCTION : 'function' ;
IF : 'if' ;
IN : 'in' ;  
LET : 'let' ;
OF : 'of' ;
THEN : 'then' ;
TO : 'to' ;
TYPE : 'type' ;  
VAR : 'var' ;  
WHILE : 'while' ;   
ENDIF : 'endif' ;
BEGIN : 'begin' ;
END : 'end' ;
ENDDO : 'enddo' ;
RETURN : 'return' ;
INT : 'int' ;
FLOAT : 'float' ;

ID : [a-zA-Z]( '_' | [a-zA-Z] | [0-9] )* ;
WS : [ \t\r\n]+ -> skip ; // skip spaces, tabs, newlines