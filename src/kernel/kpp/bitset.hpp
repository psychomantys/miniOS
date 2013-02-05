#ifndef  bitset_INC
#define  bitset_INC

class bitset::reference {
	friend class bitset;
	reference();                                 // no public constructor
	public:
	~reference();
	operator bool () const;                      // convert to bool
	reference& operator= ( bool x );             // assign from bool
	reference& operator= ( const reference& x ); // assign from bit
	reference& flip();                           // flip bit value
	bool operator~() const;                      // return inverse value
}

#endif   /* ----- #ifndef bitset_INC  ----- */

