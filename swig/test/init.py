import glooper_python_swig_wrap as glooper
import rng_python_swig_wrap as rng

M = glooper.Market()
U = rng.UniformGenerator()
P = rng.ParetoGenerator(10000,1.3)
N = rng.NormalGenerator()
G = glooper.ClassicAgentGenerator(U,N,P,U,U,M,10)
AP = glooper.AgentPopulation(U,G)
