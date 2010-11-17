%import "rng/rng.python.swig_wrap.swg"
%import "callback_log/callback_log.python.swig_wrap.swg"

%include "cstring.i"
%include "std_string.i"
%include "std_vector.i"

%template(vector_ul) std::vector<unsigned long>;

%exception
{
   try
   {
      $action
   }
   catch(const char* c)
   {
      PyErr_SetString(PyExc_Exception,c);
      return NULL;
   }
}

%inline
%{
   typedef boost::shared_ptr< RNG::TypedRandomGenerator<double> > bsp_TRGd;
%}

%inline
%{
   typedef boost::shared_ptr< RNG::TypedRandomGenerator<double> > bspTRGd;
%}

namespace GLOOPER_TEST
{
   class TableDBInterface
   {
   public:

      TableDBInterface(const char* lua_cfg_file, const char* outdir);
   };

   class Market: public SimulationObject
   {
   public:

      Market();
   };

   class SimpleInfoGenerator: public RNGInfoGenerator
   {
   public:

      SimpleInfoGenerator(
	    const bspTRGd& value_generator);
   };

   class SimpleIntermittentInfoGenerator:
      public RNGInfoGenerator, public BasicOccasionalInfoGenerator
   {
   public:

      SimpleIntermittentInfoGenerator(
	    const bspTRGd& value_generator, 
	    double generation_prob);
   };

   class TriangularMarkovianInfoGenerator: 
      public VariableIntermittentInfoGenerator
   {
   public:

      TriangularMarkovianInfoGenerator(
	    double generation_prob);
   };

   class AgentPopulation: public Process
   {
   public:

      AgentPopulation(const boost::shared_ptr<GLOOPER_TEST::InfoGenerator>&,
			const boost::shared_ptr<GLOOPER_TEST::AgentGenerator>&,
			const boost::shared_ptr<Market>&);

      const std::string agt_gen_description(const std::string& fmt) const;

      const std::string info_description(const std::string& fmt) const;

      const std::string mkt_description(const std::string& fmt) const;
   };

   class LuaAgentGenerator: public AgentGenerator
   {
   public:

      LuaAgentGenerator(const char* cfg_filename);

      size_t population_size() const;

   };

   class StandardFSSimulation: public StandardSimulation
   {
   public:

      StandardFSSimulation(const boost::shared_ptr<GLOOPER_TEST::Process>&,
	    const std::string&, 
	    const char*, const char*,
	    const boost::shared_ptr<GLOOPER_TEST::TableDBInterface>&,
	    unsigned long, unsigned long,
	    unsigned long);

      StandardFSSimulation(const boost::shared_ptr<GLOOPER_TEST::Process>&,
	    const std::string&, 
	    const char*, unsigned long,
	    const boost::shared_ptr<GLOOPER_TEST::TableDBInterface>&,
	    unsigned long, unsigned long,
	    unsigned long);
      
      void simulate();

      void set_N_threads(const unsigned long);

      void set_log_ptr(const boost::shared_ptr<CALLBACK_LOG::CallbackLog>&);

      const std::vector<unsigned long>& get_batch_run_structure() const;

      unsigned long get_total_step_count() const;

   };
   
}
