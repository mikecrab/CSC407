/*-------------------------------------------------------------------------*
 *---									---*
 *---		Message.h						---*
 *---									---*
 *---	    This file declares and defines:				---*
 *---	'struct Node': a C struct that holds text and a next ptr.	---*
 *---	'class Message': a C++ class that uses 'Node' that manages	---*
 *---	       		 a linked list of Node instances.		---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1a		2020 March 15		Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/

struct		Node
{
  char		text_[BUFFER_LEN];
  struct Node*	nextPtr_;
};


class		Message
{
  //  I.  Member vars:
  //  PURPOSE:  To hold the address of the first Node in the list, or 'NULL'
  //	if the list is empty.
  Node*				firstPtr_;

  //  PURPOSE:  To hold the address of the last Node in the list, or 'NULL'
  //	if the list is empty.
  Node*				lastPtr_;

  //  PURPOSE:  To hold the file descriptor for talking with the client.
  int				clientFd_;

  //  II.  Disallowed auto-generated methods:
  //  No default constructor:
  Message			();

  //  No copy constructor:
  Message			(const Message&);

  //  No copy assignment op:
  Message&	operator=	(const Message&);

protected :
  //  III.  Protected methods:

public :
  //  IV.  Constructor(s), assignment op(s), factory(s) and destructor:
  //  PURPOSE:  To initialize '*this' to hold an empty message for the
  //	client reachable by socket file descriptor 'newClientFd'.  No
  //	return value.
  Message			(int		newClientFd
  				);

  //  PURPOSE:  To release the resources of '*this'.  No parameters.
  //	No return value.
  ~Message			();

  //  V.  Accessors:
  //  PURPOSE:  To return the file descriptor for talking with the client.
  int		getClientFd	()
				const
				{ return(clientFd_); }

  //  VI.  Mutators:

  //  VII.  Methods that do main and misc. work of class:
  //  PURPOSE:  To add the text 'textPtr' in a new Node at the end of '*this'
  //		list.  No return value.
  void		append		(const char*	textPtr
				);

  //  PURPOSE:  To send the contents of '*this' message list to the the client.
  //		Each Node of text is exactly 'BUFFER_LEN' characters long.
  //		No parameters.  No return value.
  void		send		()
				const;

};


struct		ForThreadInfo
{
  Message*	messagePtr_;
  int		threadNum_;
};
