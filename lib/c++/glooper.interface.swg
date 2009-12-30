%import "rng/rng.python.swig_wrap.swg"

%include "std_string.i"

namespace GLOOPER_TEST
{
   class SimulationObject;

   class Market: public SimulationObject
   {
   public:
      Market();
   };

   class AgentGenerator: public SimulationObject
   {
   public:

      AgentGenerator(const bsp_TRGd&, const bspTRGd&);
   };

   class ClassicAgentGenerator: public AgentGenerator
   {
   public:
      
      ClassicAgentGenerator(
	    const bsp_TRGd&, const bsp_TRGd&,
	    const bsp_TRGd&, const bsp_TRGd&, const bsp_TRGd&,
	    const boost::shared_ptr<Market>&, unsigned long);
   };

   class Process;

   class AgentPopulation: public Process
   {
   public:
      AgentPopulation(
	    const boost::shared_ptr< TypedRandomGenerator<double> >&,
	    const boost::shared_ptr< AgentGenerator >&);
   };

   %ignore xml_description;
   %ignore end_;

   %include "simulation/Simulation.class.h"
   %include "simulation/StandardSimulation.class.h"
   %include "simulation/StandardFSSimulation.class.h"

   class SednaDBInterface
   {
   public:
      SednaDBInterface(const std::string&, 
	    const std::string&, const std::string&,
	    const std::string& = "",
	    const bool = true);
   };
}
