%module "glooper_lua_swig_wrap"

%include "glooper.headers.swg"
%{
#include "rng/rng.hpp"
%}

namespace GLOOPER_TEST
{
   %rename(_StandardComplexAgent) StandardComplexAgent;

   class StandardComplexAgent: public ComplexAgent
   {
   public:

      StandardComplexAgent(double belief,
	    const RNG::TypedRandomGenerator<double>& private_info_generator,
	    double wealth,
	    const RNG::TypedRandomGenerator<double>& spread_generator,
	    int mean_reversion,
	    unsigned long max_memory,
	    unsigned long significance_threshold,
	    double public_weight,
	    double private_weight);

      const std::string to_string() const;
   };

   //Emulating keyword argument constructor for StandardComplexAgent
   %luacode %{
      function glooper_lua_swig_wrap.StandardComplexAgent(kw)
	 return glooper_lua_swig_wrap._StandardComplexAgent(
	    kw.belief, kw.private_info_generator, kw.wealth, kw.spread_generator, kw.mean_reversion, kw.max_memory, kw.significance_threshold, kw.public_weight, kw.private_weight)
      end
   %}

}
