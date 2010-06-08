%include "python/boost_shared_ptr.i"

SWIG_SHARED_PTR(bsp_SimulationObject,GLOOPER_TEST::SimulationObject)

SWIG_SHARED_PTR_DERIVED(bspMarket,GLOOPER_TEST::SimulationObject,GLOOPER_TEST::Market)

SWIG_SHARED_PTR_DERIVED(bspAgentGenerator, GLOOPER_TEST::SimulationObject, GLOOPER_TEST::AgentGenerator)

SWIG_SHARED_PTR_DERIVED(bspLuaAgentGenerator, GLOOPER_TEST::AgentGenerator,
   GLOOPER_TEST::LuaAgentGenerator)

SWIG_SHARED_PTR(bspProcess, GLOOPER_TEST::Process)

SWIG_SHARED_PTR_DERIVED(bspAgentPopulation, GLOOPER_TEST::Process, 
      GLOOPER_TEST::AgentPopulation)

SWIG_SHARED_PTR_DERIVED(bspInfoGenerator, GLOOPER_TEST::SimulationObject, GLOOPER_TEST::InfoGenerator)

SWIG_SHARED_PTR_DERIVED(bspSimpleInfoGenerator, GLOOPER_TEST::InfoGenerator, GLOOPER_TEST::SimpleInfoGenerator)

SWIG_SHARED_PTR_DERIVED(bspRandomTimeInfoGenerator, GLOOPER_TEST::InfoGenerator, GLOOPER_TEST::RandomTimeInfoGenerator)
