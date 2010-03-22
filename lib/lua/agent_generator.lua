module(...,package.seeall)

function basic_agent_ctor_call()
   local args = {}
   local gens = {}
   for k,v in pairs(generator_args) do
      args[k] = v()
      gens[#gens+1] = {lbl=k,gen=v}
   end
   for k,v in pairs(non_generator_args) do args[k] = v end
   local agt = agent_ctor(args)
   return agt,gens
end

agent_counter = 0

function fixed_number_agent_generator()
   local a,g = basic_agent_ctor_call()
   agent_counter = agent_counter + 1
   return a,g,agent_counter,(agent_counter >= N_agents)
end
