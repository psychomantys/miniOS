/*
 * =====================================================================================
 *
 *       Filename:  ordered_array.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/21/2013 11:47:22 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include	<kernel/kpp/ordered_array.hpp>
/*
template<class data_t>
Ordered_array<data_t>::Ordered_array( const size_t &max_size ){
	base=new data_t(max_size);
	memset( base, 0, max_size*sizeof(base));
	size(0);
	this->max_size(max_size);
}

template<class data_t>
Ordered_array<data_t>::Ordered_array( const void* addr, const size_t &max_size ){
	base=(data_t*)(addr);
	memset( base, 0, max_size*sizeof(base));
	size(0);
	this->max_size(max_size);
}

template<class data_t>
const data_t &Ordered_array<data_t>::operator[]( const size_t &pos ){
	ASSERT( pos<size() );
	return base[pos];
}

template<class data_t>
void Ordered_array<data_t>::remove( size_t index ){
	while( index<size() ){
		base[index] = base[index+1];
		++index;
	}
	size(size()-1);
}

template<class data_t>
bool Ordered_array<data_t>::less_than( const data_t &l, const data_t &r){
	return l<r;
}

template<class data_t>
void Ordered_array<data_t>::insert(const data_t &data){
//	ASSERT(array->less_than);
	size_t iterator = 0;
	while (iterator < size() && less_than( base[iterator], data) )
		iterator++;
	if( iterator==size() ){ // just add at the end of the array.
		base[size] = data;
		size( size()+1 );
	}else{
		data_t tmp=base[iterator];
		base[iterator] = data;
		while( iterator<size() ){
			++iterator;
			data_t tmp2=base[iterator];
			base[iterator]=tmp;
			tmp=tmp2;
		}
		size(size()+1);
	}
}

template<class data_t>
Ordered_array<data_t>::~Ordered_array(){
	delete base;
}
*/

