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

--Fixed number agent generator

agent_counter = 0

function fixed_number_agent_generator()
   local a,g = basic_agent_ctor_call()
   agent_counter = agent_counter + 1
   return a,g,agent_counter,(agent_counter >= N_agents)
end

--Mixed (composite) agent generator

mixed_generator = {agent_counter = 0}

function prepare_next_generator()
   
   mixed_generator.generator_counter = (mixed_generator.generator_counter or 0) + 1

   local next_generator_entry = mixed_generator.table[mixed_generator.generator_counter]

   if(next_generator_entry) then

      mixed_generator.current_generator = next_generator_entry.generator

      agent_counter = 0
      N_agents = next_generator_entry.N_agents
      agent_ctor = next_generator_entry.agent_ctor
      generator_args = next_generator_entry.generator_args
      non_generator_args = next_generator_entry.non_generator_args

   end

end

function mixed_agent_generator()

   if(not mixed_generator.generator_counter) then prepare_next_generator() end

   local a_,g_,ac_,stop_ = mixed_generator.current_generator()
   mixed_generator.agent_counter = mixed_generator.agent_counter + 1

   if(stop_) then prepare_next_generator() end

   return a_,g_,mixed_generator.agent_counter,(mixed_generator.generator_counter > #(mixed_generator.table))

end
