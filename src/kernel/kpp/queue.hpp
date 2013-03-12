#ifndef MY_QUEUE_PSYCHO
#define MY_QUEUE_PSYCHO

#include	<kernel/kpp/node.hpp>
#include	<stdlib.h>
#include	<kernel/kmalloc.hpp>
#include	<kernel/debug.hpp>

template <typename data_t>
class DQueue {
	private:
		typedef node<data_t> Node;

		Node *begin;
		Node *end;
		unsigned int size;

	public:
		DQueue() :
			begin(NULL),
			end(NULL),
			size(0)
		{ }

		inline bool is_empty(){
			return size==0;
		}

		inline data_t pop(){
			if( not this->is_empty() ){
				data_t ret = end->data();
				Node *n=end->next();
				delete end;
				end=n;
				--size;
				return ret;
			}else{
				kprintf("cabou lista\n");
				exit(-1);
			}
		}

		inline void push( const data_t &in ){
			Node *n( new Node(in) );
			if( not this->is_empty() ){
				begin->next(n);
			}else{
				end=n;
			}
			begin=n;
			++size;
		}

		~DQueue(){
			Node *aux=end;
			while( aux ){
				end=aux->next();
				delete aux;
				aux=end;
			}
		}
};

template <typename data_t>
class SQueue {
	private:
		unsigned int begin;
		unsigned int end;
		unsigned int size, max_size;

	public:
		data_t *base;
		SQueue() :
			begin(0),
			end(0),
			size(0),
			max_size(0),
			base(NULL)
		{ }

		SQueue( unsigned int s ) :
			begin(0),
			end(0),
			size(0),
			max_size(s)
		{
			  base=new data_t[s];
		}

		inline bool is_empty(){
			return size==0;
		}
		inline bool is_full(){
			return size==max_size;
		}

		inline data_t pop(){
//			if( not this->is_empty() ){
				data_t ret=base[end];
				--size;
				++end;
				if( max_size==end ){
					end=0;
				}
				return ret;
//			}else{
//				kprintf("cabou lista\n");
//				exit(-1);
//			}
		}

		inline void push( const data_t &in ){
//			if( not this->is_full() ){
				base[begin]=in;
				++begin;
				++size;
				if( max_size==begin ){
					begin=0;
				}
//			}else{
////				if( reserve(max_size+5) ){
////					push(in);
////				}else{
//					kprintf("Não foi possivel inserir mais!!\n");
//					exit(1);
////				}
//			}
		}

		~SQueue(){
			int a;
			kdebug(8,"~SQueue %p %p\n",&a, base);
//			free(base);
			kdebug(8,"~SQueue %p %p\n",&a, base);
		}
};

#endif // ifdef MY_QUEUE_PSYCHO

