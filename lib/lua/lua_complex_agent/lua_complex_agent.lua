local rng = require "rng_lua_swig_wrap"

LuaComplexAgent = {U = rng.UniformGenerator(), max_spread=0.01}

LuaComplexAgent.update_belief = function(xi)
   LuaComplexAgent.belief = LuaComplexAgent.belief + U()*(xi-LuaComplexAgent.belief)
end

LuaComplexAgent.adjust_belief = function(z)
   local codirected = (LuaComplexAgent.belief > 0.5 and LuaComplexAgent.last_move_up) or (LuaComplexAgent.belief < 0.5 and not LuaComplexAgent.last_move_up) 
   local K = -1
   
   if (LuaComplexAgent.mean_reverter and codirected ) or (not LuaComplexAgent.mean_reverter and not codirected) then
      K = 0.5
   else
      if LuaComplexAgent.belief > 0.5 then K = 1 else K = 0 end
   end

   LuaComplexAgent.belief = LuaComplexAgent.belief + U()*z*(K - LuaComplexAgent.belief)
end

LuaComplexAgent.spread_fraction = function()
   local min_spread = LuaComplexAgent.max_spread*0.1
   return U()*(LuaComplexAgent.max_spread - min_spread) + min_spread
end
