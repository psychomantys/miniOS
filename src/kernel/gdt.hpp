#ifndef  gdt_INC
#define  gdt_INC

#include	<stdint.h>
#include	<isa_specific_code.hpp>

/* Defines a GDT entry. We say packed, because it prevents the
 *  compiler from doing things that it thinks is best: Prevent
 *  compiler "optimization" by packing */
struct GDT_entry{
	uint16_t	limit_low;
	uint16_t	base_low;
	uint8_t	base_middle;
	uint8_t	access;
	uint8_t	granularity;
	uint8_t	base_high;
} __attribute__((packed));
/*typedef struct GDT_entry GDT_entry;

union packet_GDT_entry{
	struct GDT_entry p;
	uint64_t	s;
};
*/
/* Special pointer which includes the limit: The max bytes
 *  taken up by the GDT, minus 1. Again, this NEEDS to be packed */
struct GDT_ptr{
	uint16_t	limit;
	uint32_t	base;
} __attribute__((packed));
/*typedef struct GDT_ptr GDT_ptr;

union packet_GDT_ptr{
	struct GDT_ptr p;
	uint64_t	s;
};
*/
class GDT{
	public:
		GDT();
		void set_gate(const int &num, const uint32_t &base, const uint32_t &limit, const uint8_t &access, const uint8_t &gran);
		void install();
		void flush();
	private:
//		union packet_GDT_entry gdt[3];
//		union packet_GDT_ptr gp;
		static struct GDT_entry gdt[3];
		static struct GDT_ptr gp;
};

#endif   /* ----- #ifndef gdt_INC  ----- */

