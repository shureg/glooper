%module "glooper_lua_swig_wrap"

%include "glooper.headers.swg"

%import "rng/rng.python.swig_wrap.swg"

namespace GLOOPER_TEST
{
   %rename(_ClassicAgent) ClassicAgent;//Need this to emulate keyword argument construction

   class ClassicAgent: public TradingAgent
   {
   public:
      ClassicAgent(double belief,
	    double wealth,
	    double p_min, double f_min, double bas_max);
   };

   //Emulating keyword argument constructor for ClassicAgent
   %luacode %{
      function glooper_lua_swig_wrap.ClassicAgent(kw)
	 return glooper_lua_swig_wrap._ClassicAgent(kw.belief,kw.wealth,kw.p_min,kw.f_min,kw.bas_max)
      end
   %}
}
