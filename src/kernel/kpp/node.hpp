#ifndef MY_NODE_PSYCHO
#define MY_NODE_PSYCHO

#include	<stdlib.h>

template <typename data_t>
class node{
		typedef node<data_t> Node;

		Node *_next;
		data_t _data;
	public:
		node(){ }

		node( const data_t &in, Node *n=NULL ):
				_next(n),
				_data(in)
		{ }

		inline Node *next(){
			return _next;
		}

		inline void next(Node *in){
			_next=in;
		}

		inline data_t &data(){
			return _data;
		}

		inline void data( const data_t &in){
			_data=in;
		}
};

template <typename data_t>
class d_node{
	private:
		typedef d_node<data_t> Node;
		Node *_left;
		Node *_rigth;
		data_t _data;
	public:
		d_node(){ }

		d_node( const data_t &in, Node *l=NULL, Node *r=NULL ) :
				_left(l),
				_rigth(r),
				_data(in)
		{ }

		Node *rigth(){
			return _rigth;
		}

		Node *left(){
			return _left;
		}

		void rigth(Node *in){
			_rigth=in;
		}

		void left(Node *in){
			_left=in;
		}

		data_t &data(){
			return _data;
		}

		void data( const data_t &in){
			_data=in;
		}
};

#endif // ifdef MY_NODE_PSYCHO

