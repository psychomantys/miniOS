
#include	<cpp_ctor_dtor_impl.hpp>

#include	<isa_specific_code.hpp>
#include	<plataform_specific_code.hpp>
#include	<compiler_specific_code.hpp>

extern "C" {
	void _at_global_begin(){
		__cxa_atbegin();
	}

	void _at_global_end(){
		__cxa_finalize();
	}
}

