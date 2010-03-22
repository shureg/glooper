module(...,package.seeall)

require "rng_lua_swig_wrap"

rng = rng_lua_swig_wrap

U = rng.UniformGenerator()

update_belief = function(xi,belief)
   return belief + U()*(xi-belief)
end

adjust_belief = function(z,belief,mean_reverter,last_move_up)
   local codirected = (belief > 0.5 and last_move_up) or (belief < 0.5 and not last_move_up) 
   local K = -1
   
   if (mean_reverter and codirected ) or (not mean_reverter and not codirected) then
      K = 0.5
   else
      if belief > 0.5 then K = 1 else K = 0 end
   end

   return belief + (1-z)*(K - belief)
end

spread_fraction = function()
   local min_spread = max_spread*0.1
   return U()*(max_spread - min_spread) + min_spread
end
