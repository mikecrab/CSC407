/*--------------------------------------------------------------------------*
 *----									----*
 *----	    This file defines a recursive-descent parser for a simple	----*
 *----	grammar on strings:						----*
 *----									----*
 *----		(1) S -> E						----*
 *----		(2) S -> E '+' S	// String concatenation		----*
 *----		(3) E -> <string>	// String constant		----*
 *----		(4) E -> <int> '*' E	// String repetition		----*
 *----		(5) E -> '(' S ')'					----*
 *----									----*
 *----	----	----	----	----	----	----	----	----	----*
 *----									----*
 *----	Version 1.0		2016 April 2		Joseph Phillips	----*
 *----									----*
 *----		Compile with:						----*
 *----		$ g++ stringLanguage.cpp -o stringLanguage		----*
 *----									----*
 *--------------------------------------------------------------------------*/

#include	<cstdlib>
#include	<iostream>
#include	<string>
#include	<unistd.h>


//  PURPOSE:  To represent the different lexemes in the language.
typedef		enum
		{
		  END_OF_INPUT_SYM,
		  STRING_CONST_SYM,
		  INTEGER_CONST_SYM,
		  BEGIN_PAREN_SYM,
		  END_PAREN_SYM,
		  CONCAT_SYM,
		  REPEAT_SYM
		}
		symbol_t;


//  PURPOSE:  To represent the different types of ground Node classes.
typedef		enum
		{
		  STRING_CONST_NODE,
		  CONCAT_NODE,
		  REPEAT_NODE
		}
		node_t;

//  PURPOSE:  To represent the double quote character.
const char	QUOTE_CHAR	= (char)0x22;


//  PURPOSE:  To serve as the base class of all Node classes.
class		Node
{
  //  I.  Member vars:

  //  II.  Disallowed auto-generated methods:
  //  No copy assignment operator:
  Node&		operator=
		(const Node&	rhs
		);

protected : 
  //  III.  Protected methods:

public :
  //  IV.  Constructors:
  //  PURPOSE:  To initiailize '*this'.  No parameters.  No return value.
  Node				()
				{ }

  //  PURPOSE:  To make '*this' a copy of 'rhs'.  No return value.
  Node				(const Node&	rhs
				)
				{ }

  //  PURPOSE:  To release the resources of '*this'.  No parameters.  No return
  //	value.
  virtual
  ~Node				()
				{ }

  //  V.  Accessors:
  //  PURPOSE:  To return a 'node_t' representing the type of '*this'.  No
  //	parameters.
  virtual
  node_t	getType		()
				const
				= 0;

  //  VI.  Mutators:

  //  VII.  Methods that do main and misc work of class:
  virtual
  std::string	getValue	()
				const
				{ return(std::string("")); }

};


//  PURPOSE:  To serve as the base class of all constant-storing Node classes.
class		StringNode : public Node
{
  //  I.  Member vars:
  //  PURPOSE:  To hold the string constant associated with '*this' Node.
  std::string			value_;

  //  II.  Disallowed auto-generated methods:
  //  No default constructor:
  StringNode			();

  //  No copy assignment operator:
  StringNode	operator=	(const StringNode&	rhs
				);

protected : 
  //  III.  Protected methods:

public :
  //  IV.  Constructors:
  //  PURPOSE:  To initialize '*this' to hold 'newString'.  No return value.
  StringNode			(const std::string&	newString
				) :
				Node(),
				value_(newString)
				{ }

  //  PURPOSE:  To make '*this' a copy of 'rhs'.  No return value.
  StringNode			(const StringNode&	rhs
				) :
				Node(*this),
				value_(rhs.value_)
				{ }

  //  PURPOSE:  To release the resources of '*this'.  No parameters.  No return
  //	value.
  ~StringNode			()
				{ }

  //  V.  Accessors:
  //  PURPOSE:  To return a 'node_t' representing the type of '*this'.  No
  //	parameters.
  node_t	getType		()
				const
				{ return(STRING_CONST_NODE); }

  //  VI.  Mutators:

  //  VII.  Methods that do main and misc work of class:
  std::string	getValue	()
				const
				{ return(value_); }

};


//  PURPOSE:  To represent the concatenation operation Node.
class		ConcatNode : public Node
{
  //  I.  Member vars:
  //  PURPOSE:  To point to the Node on the left hand side of
  //	'*this' concatenation.
  Node*				lhsPtr_;

  //  PURPOSE:  To point to the Node on the right hand side of
  //	'*this' concatenation.
  Node*				rhsPtr_;

  //  II.  Disallowed auto-generated methods:
  //  No default constructor:
  ConcatNode			();

  //  No copy constructor:
  ConcatNode			(const ConcatNode&	rhs
				);

  //  No copy assignment operator:
  ConcatNode	operator=	(const ConcatNode&	rhs
				);

protected : 
  //  III.  Protected methods:

public :
  //  IV.  Constructors:
  //  PURPOSE:  To initialize '*this' to have node '*newLhsPtr' on the left
  //	hand side and node '*newRhsPtr' on the right hand side.  No return
  //	value.
  ConcatNode			(Node*		newLhsPtr,
				 Node*		newRhsPtr
				) :
				Node(),
				lhsPtr_(newLhsPtr),
				rhsPtr_(newRhsPtr)
				{ }

  //  PURPOSE:  To release the resources of '*this'.  No parameters.  No return
  //	value.
  ~ConcatNode			()
				{
				  delete(rhsPtr_);
				  delete(lhsPtr_);
				}

  //  V.  Accessors:
  //  PURPOSE:  To return a 'node_t' representing the type of '*this'.  No
  //	parameters.
  node_t	getType		()
				const
				{ return(CONCAT_NODE); }

  //  VI.  Mutators:

  //  VII.  Methods that do main and misc work of class:
  std::string	getValue	()
				const
				{
				  return(lhsPtr_->getValue() +
					 rhsPtr_->getValue()
					);
				}

};


//  PURPOSE:  To represent the repetition operation Node.
class		RepeatNode : public Node
{
  //  I.  Member vars:
  //  PURPOSE:  To hold the integer constant telling how many times to repeat
  //	the string.
  //	'*this' concatenation.
  unsigned int			totalCount_;

  //  PURPOSE:  To point to the Node telling the string to repeat.
  Node*				nodePtr_;

  //  II.  Disallowed auto-generated methods:
  //  No default constructor:
  RepeatNode			();

  //  No copy constructor:
  RepeatNode			(const RepeatNode&	rhs
				);

  //  No copy assignment operator:
  RepeatNode	operator=	(const RepeatNode&	rhs
				);

protected : 
  //  III.  Protected methods:

public :
  //  IV.  Constructors:
  //  PURPOSE:  To initialize '*this' to note that '*newNodePtr' ought to be
  //	repeated 'newTotalCount' times.  No return value.
  RepeatNode			(unsigned int	newTotalCount,
				 Node*		newNodePtr
				) :
				Node(),
				totalCount_(newTotalCount),
				nodePtr_(newNodePtr)
				{ }

  //  PURPOSE:  To release the resources of '*this'.  No parameters.  No return
  //	value.
  ~RepeatNode			()
				{
				  delete(nodePtr_);
				}

  //  V.  Accessors:
  //  PURPOSE:  To return a 'node_t' representing the type of '*this'.  No
  //	parameters.
  node_t	getType		()
				const
				{ return(REPEAT_NODE); }

  //  VI.  Mutators:

  //  VII.  Methods that do main and misc work of class:
  std::string	getValue	()
				const
				{
				  std::string	toReturn("");

				  if  (totalCount_ == 0)
				    return(toReturn);

				  std::string	addend	= nodePtr_->getValue();
				  unsigned int	count	= 0;

				  do
				  {
				    toReturn += addend;
				    count++;
				  }
				  while  (count < totalCount_);

				  return(toReturn);
				}

};


/*  PURPOSE:  To implement an interface that manages the character source.
 */
class	InputCharStream
{
  //  I.  Member vars:
  //  PURPOSE:  To hold the input.
  std::string		input_;

  //  PURPOSE:  To hold where the cursor is.
  int			index_;

  //  II.  Disallowed auto-generated methods:
  
  //  III.  Protected methods:
protected :

public:
  //  IV.  Constructor(s), assignment op(s), factory(s) and destructor:
  //  PURPOSE:  To
  InputCharStream	(std::string&	newInput
  			) :
			input_(newInput),
			index_(0)
			{ }

  //  V.  Accessors:

  //  VI.  Mutators:

  //  VII.  Methods that do main and misc work of class:
  //  PURPOSE:  To return the current char, or '\0' if there are no more.
  //	No parameters.
  char		peek	()
  			const
			throw()
			{ return
			  ( (index_ >= input_.length())
			    ? '\0' : input_[index_]
			  );
			}

  //  PURPOSE:  To return 'true' if at eof-of-input, or 'false' otherwise.
  bool		isAtEnd	()
  			const
			throw()
			{ return(index_ >= input_.length()); }

  //  PURPOSE:  To advance to the next char (if not already at end).  No
  //	parameters.  No return value.
  void		advance	()
  			throw()
			{
			  if  (index_ < input_.length())  index_++;
			}

};


//  PURPOSE:  To represent a parsed symbol, and any associated data.
class		Symbol
{
  //  I.  Member vars:
  //  PURPOSE:  To tell the type of symbol that '*this' represents.
  symbol_t			symbol_;

  //  PURPOSE:  To point to the string associated with '*this' symbol (if there
  //	is one).
  std::string*			stringPtr_;

  //  PURPOSE:  To hold the integer associated with '*this' symbol (if there
  //	is one).
  unsigned int			integer_;

  //  II.  Disallowed auto-generated methods:
  //  No default constructor:
  Symbol			();

  //  No copy constructor:
  Symbol			(const Symbol&
				);

  //  No copy assignment op:
  Symbol&	operator=	(const Symbol&
				);

protected :
  //  III.  Protected methods:

public :
  //  IV.  Constructor(s), assignment op(s), factory(s) and destructor:
  //  PURPOSE:  To initialize '*this' to hold symbol 'newSymbol'.  No return
  //	value.
  Symbol			(symbol_t	newSymbol
				) :
				symbol_(newSymbol),
				stringPtr_(NULL),
				integer_(0)
				{ }

  //  PURPOSE:  To initialize '*this' to hold string 'newString'.  No return
  //	value.
  Symbol			(const std::string&	newString
				) :
				symbol_(STRING_CONST_SYM),
				stringPtr_(new std::string(newString)),
				integer_(0)
				{ }

  //  PURPOSE:  To initialize '*this' to hold integer 'newInteger'.  No return
  //	value.
  Symbol			(unsigned int	newInteger
				) :
				symbol_(INTEGER_CONST_SYM),
				integer_(newInteger),
				stringPtr_(NULL)
				{ }

  //  V.  Accessors:
  //  PURPOSE:  To return the type associated with '*this' Symbol.  No
  //  	parameters.
  symbol_t	getType		()
				const
				{ return(symbol_); }

  //  PURPOSE:  To return the integer stored at '*this' Symbol, or '0' if
  //	'*this' does not represent an integer.
  unsigned int	getInteger	()
				const
				{ return(integer_); }

  //  PURPOSE:  To return a reference to the string stored at '*this' Symbol,
  //	or the empty string if '*this' does not represent a string.
  const std::string&
		getString	()
				const
				{
				  static std::string	empty("");

				  return( (stringPtr_ == NULL)
				  	  ? empty
					  : *stringPtr_
					);
				}

};


//  PURPOSE:  To represent the end of input.
Symbol				endSymbol(END_OF_INPUT_SYM);


//  PURPOSE:  To implement an interface that gathers characters into lexemes.
class	TokenStream
{
  //  I.  Member vars:
  //  PURPOSE:  To hold the source of the character input.
  InputCharStream&	inputCharStream_;

  //  PURPOSE:  To hold the lastest lexeme parsed.
  Symbol*	   	lastParsedPtr_;

  //  II.  Disallowed auto-generated methods:
  //  No default constructor:
  TokenStream		();

  //  No copy constructor:
  TokenStream		(const TokenStream&
			);

  //  No copy assignment op:
  TokenStream&		operator=
			(const TokenStream&
			);

protected :
  //  III.  Protected methods:
  //  PURPOSE:  To return a pointer representing a scanned string constant.
  //	No parameters.
  Symbol*	scanString	()
				{
				  std::string	lex("");
				  bool		haveFoundEnd	= false;

				  //  Advance past first quote:
				  inputCharStream_.advance();

				  while  ( !inputCharStream_.isAtEnd() )
				  {
				    char	c = inputCharStream_.peek();

				    inputCharStream_.advance();

				    if  (c == QUOTE_CHAR)
				    {
				      haveFoundEnd	= true;
				      break;
				    }

				    lex	+= c;
				  }

				  if  (!haveFoundEnd)
				    throw "Non-terminated string constant";

				  return(new Symbol(lex));
				}

  //  PURPOSE:  To return a pointer representing a scanned integer.  No
  //	parameters.
  Symbol*   	scanDigits	()
				throw(const char*)
				{
				  std::string	lex("");

				  while  ( isdigit(inputCharStream_.peek()) )
				  {
				    lex += inputCharStream_.peek();
				    inputCharStream_.advance();
				  }

				  return(new Symbol(atoi(lex.c_str())));
				}


  //  PURPOSE:  To return a pointer representing a scanned Symbol, or to return
  //	'&endSymbol' if the '*this' is at the end-of-input.  No parameters.
  Symbol*	scanner		()
				throw(const char*)
  				{
				  while  ( isspace(inputCharStream_.peek()) )
				    inputCharStream_.advance();

				  if  ( inputCharStream_.isAtEnd() )
				    return( &endSymbol );

				  if  ( inputCharStream_.peek() == QUOTE_CHAR )
				    return( scanString() );

				  if  ( isdigit(inputCharStream_.peek()) )
				    return( scanDigits() );

				  char	  ch	    = inputCharStream_.peek();
				  Symbol* symbolPtr = NULL;

				  inputCharStream_.advance();

				  switch  (ch)
				  {
				  case '(' :
				    symbolPtr	= new Symbol(BEGIN_PAREN_SYM);
				    break;

				  case ')' :
				    symbolPtr	= new Symbol(END_PAREN_SYM);
				    break;

				  case '+' :
				    symbolPtr	= new Symbol(CONCAT_SYM);
				    break;

				  case '*' :
				    symbolPtr	= new Symbol(REPEAT_SYM);
				    break;

				  default :
				    throw "Unexpected character in input";
				  }

				  return(symbolPtr);
				}

public :
  //  IV.  Constructor(s), assignment op(s), factory(s) and destructor:
  //  PURPOSE:  To initialize '*this' to tokenize given characters read from
  //  	'newInputCharStream'.  No parameters.
  TokenStream			(InputCharStream&	newInputCharStream
				)
				throw(const char*) :
				inputCharStream_(newInputCharStream),
				lastParsedPtr_(NULL)
				{
				  advance();
				}

  //  PURPOSE:  To release the resources of '*this'.  No parameters.  No return
  //	value.
  ~TokenStream			()
				{ }

  //  V.  Accessors:

  //  VI.  Mutators:

  //  VII.  Methods that do main and misc work of class:
  //  PURPOSE:  To return the 'symbol_t' of the 'Symbol' instance that is next
  //	in the symbol stream.  No parameters.
  symbol_t   	peek		()
				throw(const char*)
				{
				  if  (lastParsedPtr_ == NULL)
				    lastParsedPtr_	= scanner();

				  return(lastParsedPtr_->getType());
				}

  //  PURPOSE:  To return the pointer to the old Symbol at that was at the
  //	front of the symbol stream, and then to internally advance to the next
  //	Symbol instance (if not already at the end).  No parameters.
  Symbol*	advance		()
	  			throw(const char*)
				{
				  Symbol* toReturn	= lastParsedPtr_;

				  lastParsedPtr_	= scanner();
				  return(toReturn);
				}

};


//  PURPOSE:  To return a pointer to a heap-allocated Node instance
//	representing a sentence (non-terminal 'S') in String language.
Node*		parseSentence	(TokenStream&	tokenizer
				)
				throw(const char*);


//  PURPOSE:  To return a pointer to a heap-allocated Node instance
//	representing an expression (non-terminal 'E') in String language.
Node*		parseExpression	(TokenStream&	tokenizer
				)
				throw(const char*)
{
  //  I.  Application validity check:

  //  II.  Attempt to parse expression:
  Symbol*	symbolPtr	= tokenizer.advance();
  Node*		nodePtr		= NULL;

  switch  (symbolPtr->getType())
  {
  case STRING_CONST_SYM :
    nodePtr	= new StringNode(symbolPtr->getString());
    break;

  case INTEGER_CONST_SYM :
    {
      unsigned int	count	= symbolPtr->getInteger();

      delete(symbolPtr);
      symbolPtr			= tokenizer.advance();

      if  (symbolPtr->getType() != REPEAT_SYM)
        throw "Expected * after integer";

      nodePtr	= new RepeatNode(count,parseExpression(tokenizer));
    }
    break;

  case BEGIN_PAREN_SYM :
    delete(symbolPtr);
    nodePtr	= parseSentence(tokenizer);

    if  (tokenizer.peek() != END_PAREN_SYM)
      throw "Expected end parenthesis";

    symbolPtr	= tokenizer.advance();
    break;

  default :
    if  (symbolPtr->getType() != END_OF_INPUT_SYM)
      delete(symbolPtr);

    throw "Expected integer, string constant, or begin parenthesis";
  }

  //  III.  Finished:
  if  (symbolPtr->getType() != END_OF_INPUT_SYM)
    delete(symbolPtr);

  return(nodePtr);
}


//  PURPOSE:  To return a pointer to a heap-allocated Node instance
//	representing a sentence (non-terminal 'S') in String language.
Node*		parseSentence	(TokenStream&	tokenizer
				)
				throw(const char*)
{
  //  I.  Application validity check:

  //  II.  Attempt to parse sentence:
  Node*	lhsPtr	= parseExpression(tokenizer);

  if  ( (tokenizer.peek() == END_OF_INPUT_SYM)  ||
	(tokenizer.peek() == END_PAREN_SYM)
      )
    return(lhsPtr);

  if  (tokenizer.peek() == CONCAT_SYM)
  {
    Node*	rhsPtr;

    delete(tokenizer.advance());
    rhsPtr	= parseSentence(tokenizer);
    return(new ConcatNode(lhsPtr,rhsPtr));
  }

  throw "Expected concatenation, end parenthesis or end of input";
}


//  PURPOSE:  To return a string with the expression to compute, either from
//	the command line or the keyboard.  'argc' tells how many arguments
//	were on the command line and 'argv[]' points to those arguments.
std::string	getInput	(int		argc,
				 char*		argv[]
				)
{
  //  I.  Application validity check:

  //  II.  Get input:
  //  II.A.  Handle command line input:
  if  (argc > 1)
  {
    return(std::string(argv[1]));
  }

  //  II.B.  Handle keyboard input:
  std::string	input;

  std::getline(std::cin,input);

  //  III.  Finished:
  return(input);
}


//  PURPOSE:  To get, and attempt to compute, the expression.  The expression
//	may either come from the command line or the keyboard.  'argc' tells
//	how many arguments were on the command line and 'argv[]' points to
//	those arguments.  Returns 'EXIT_SUCCESS' if the expression was
//	successfully parsed and computed or 'EXIT_FAILURE' otherwise.
int		main		(int		argc,
				 char*		argv[]
				)
{
  std::string		input(getInput(argc,argv));
  InputCharStream	charStream(input);
  int			status	= EXIT_SUCCESS;

  try
  {
    TokenStream		tokenizer(charStream);
    Node*		nodePtr	= parseSentence(tokenizer);

    std::cout << nodePtr->getValue() << std::endl;
    delete(nodePtr);
  }
  catch  (const char*  messageCPtr
	 )
  {
    std::cout << messageCPtr << std::endl;
    status	= EXIT_FAILURE;
  }

  return(status);
}
