#ifndef  bitset_INC
#define  bitset_INC

// Macros used in the bitset algorithms.
#define INDEX_FROM_BIT(a) ( a/(8*sizeof(a)) )
#define OFFSET_FROM_BIT(a) ( a%(8*sizeof(a)) )

#include	<string.h>
#include	<kernel/kmalloc.hpp>

class bitset {
	private:
		typedef uint32_t Base_type;
		Base_type *base;
		size_t _size;
	public:
		bitset( const int &x ){
			base=new Base_type( (x/sizeof(Base_type))+1 );
			_size=x;
			memset(base, 0, (x/sizeof(Base_type))+1 );
		}
		~bitset(){
			delete base;
		}

		const size_t &size(){
			return _size;
		}
		// Static function to test if a bit is set.
		inline bool operator[]( const size_t &pos ) const {
			//static u32int test_frame(u32int frame_addr)
			//{
//			uint32_t frame = frame_addr/0x1000;
			const uint32_t idx = INDEX_FROM_BIT(pos);
			const uint32_t off = OFFSET_FROM_BIT(pos);
			return (base[idx] & (0x1 << off));
			//}
		}
		inline void set( const size_t &pos ){
//			u32int frame = frame_addr/0x1000;
			const uint32_t idx = INDEX_FROM_BIT(pos);
			const uint32_t off = OFFSET_FROM_BIT(pos);
			base[idx] |= (0x1 << off);
		}
		inline void clear( const size_t &pos ){
//			uint32_t frame = frame_addr/0x1000;
			const uint32_t idx = INDEX_FROM_BIT(pos);
			const uint32_t off = OFFSET_FROM_BIT(pos);
			base[idx] &= ~(0x1 << off);
		}
		inline size_t first(){
			size_t i;
			for( i=0 ; i<size() ; ++i ){
				if( (*this)[i]==false ){
					return i;
				}
			}
			// Não tem mais nada....
			return i;
		}
};

#endif   /* ----- #ifndef bitset_INC  ----- */

