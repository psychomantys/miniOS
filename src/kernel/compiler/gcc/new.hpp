#ifndef	_NEW_HPP_
#define	_NEW_HPP_

void *operator new[](unsigned int s);
void *operator new(unsigned int s);

void operator delete(void*);
void operator delete[](void*);

#endif	/* _NEW_HPP_ */

