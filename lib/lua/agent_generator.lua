package.cpath = glooper_path.."/swig/output/?.so;"..package.cpath

local glooper = require "glooper_lua_swig_wrap.so"
local rng = require "rng_lua_swig_wrap.so"

module(...)

function basic_agent_ctor_call()
   args = {}
   gens = {}
   for k,v in pairs(generator_args) do
      args[k] = v()
      gens[#gens+1] = {lbl=k,gen=v}
   end
   for k,v in pairs(non_generator_args) do args[k] = v end
   agt = agent_ctor(args)
   return agt,gens
end

agent_counter = 0

function fixed_number_agent_generator()
   a,g = basic_agent_ctor_call()
   agent_counter = agent_counter + 1
   return a,g,(agent_counter >= N_agents)
end
