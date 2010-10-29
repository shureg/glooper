%module "glooper_lua_swig_wrap"

%include "glooper.headers.swg"

%import "rng/rng.lua.swig_wrap.swg"

%inline %{
   typedef RNG::TypedRandomGenerator<double> TRG_d;
%}

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

   %rename(_LuaComplexAgent) LuaComplexAgent;
   
   class LuaComplexAgent: public ComplexAgent
   {
   public:
      LuaComplexAgent(double belief,
	    double wealth,
	    int mean_reversion,
	    unsigned long max_memory,
	    unsigned long significance_threshold,
	    const char* lua_cfg_filename);
   };

   //Emulating keyword argument constructor for LuaComplexAgent
   %luacode %{
      function glooper_lua_swig_wrap.LuaComplexAgent(kw)
	 return glooper_lua_swig_wrap._LuaComplexAgent(
	    kw.belief, kw.wealth, kw.mean_reversion, kw.max_memory, kw.significance_threshold, kw.lua_cfg_filename)
      end
   %}

   %rename(_HomogeneousAgent) HomogeneousAgent;

   class HomogeneousAgent: public Agent
   {
   public:
      HomogeneousAgent(
	    double belief, 
	    unsigned long order_size,
	    double min_spread,
	    double max_spread);   
   };

   //Emulating keyword argument constructor for HomogeneousAgent
   %luacode %{
      function glooper_lua_swig_wrap.HomogeneousAgent(kw)
	 return glooper_lua_swig_wrap._HomogeneousAgent(
	    kw.belief, kw.order_size, kw.min_spread, kw.max_spread)
      end
   %}

   %rename(_NoiseTrader) NoiseTrader;

   class NoiseTrader: public TradingAgent
   {
   public:
      NoiseTrader(double belief, double wealth, 
	    const TRG_d& belief_generator_, double max_bid_ask_sp);
   };

   //Emulating keyword argument constructor for NoiseTrader
   %luacode %{
      function glooper_lua_swig_wrap.NoiseTrader(kw)
	 return glooper_lua_swig_wrap._NoiseTrader(
	    kw.belief, kw.wealth, kw.belief_generator, kw.max_bid_ask_spread)
      end
   %}
}
