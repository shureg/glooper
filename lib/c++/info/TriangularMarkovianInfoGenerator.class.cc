// =====================================================================================
// 
//       Filename:  TriangularMarkovianInfoGenerator.class.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  03/09/10 14:22:07
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#include "info/TriangularMarkovianInfoGenerator.class.h"
#include "rng/generator/inverse_cdf/TriangularGenerator.class.h"
#include "boost/make_shared.hpp"

using namespace GLOOPER_TEST;

TriangularMarkovianInfoGenerator::TriangularMarkovianInfoGenerator(
      double generation_prob): 
   VariableIntermittentInfoGenerator(generation_prob,
	 boost::make_shared<RNG::TriangularGenerator>(0,1,0.5))
{}

bspTRGd TriangularMarkovianInfoGenerator::next_generator()
{
   if( actual_values.empty() )
      return boost::make_shared<RNG::TriangularGenerator>(
	    0,1,0.5);
   else
      return boost::make_shared<RNG::TriangularGenerator>(
	    0,1,actual_values.back());
}

InfoGenerator* TriangularMarkovianInfoGenerator::real_clone() const
{
   TriangularMarkovianInfoGenerator* tmp =
      new TriangularMarkovianInfoGenerator(*this);

   return (InfoGenerator*) tmp;
}
