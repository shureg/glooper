%import "rng/rng.python.swig_wrap.swg"

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

namespace GLOOPER_TEST
{
   class SednaDBInterface
   {
   public:
      SednaDBInterface(const std::string&, 
	    const std::string&, const std::string&,
	    const std::string& = "",
	    const bool = true);
   };

   class Market: public SimulationObject
   {
   public:

      Market();
   };

   class ClassicAgentGenerator: public AgentGenerator
   {
   public:

      ClassicAgentGenerator(
	    const bsp_TRGd&, const bsp_TRGd&,
	    const bsp_TRGd&, const bsp_TRGd&, const bsp_TRGd&,
	    const boost::shared_ptr<Market>&, unsigned long);

   };

   class AgentPopulation: public Process
   {
   public:

      AgentPopulation(const bsp_TRGd&,
			const boost::shared_ptr<GLOOPER_TEST::AgentGenerator>&);
   };

   class StandardFSSimulation: public StandardSimulation
   {
   public:

      StandardFSSimulation(const boost::shared_ptr<GLOOPER_TEST::Process>&,
	    const std::string&, 
	    const SednaDBInterface&, const char*,
	    unsigned long, unsigned long,
	    unsigned long);

      StandardFSSimulation(const boost::shared_ptr<GLOOPER_TEST::Process>&,
	    const std::string&, 
	    const SednaDBInterface&, unsigned long,
	    unsigned long, unsigned long,
	    unsigned long);
      
      void simulate();

      const std::vector<unsigned long>& get_batch_run_structure() const;

   };

}
