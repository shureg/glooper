// =====================================================================================
// 
//       Filename:  InfoGenerator.class.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  17/03/10 11:36:52
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#ifndef  INFOGENERATOR_CLASS_INC
#define  INFOGENERATOR_CLASS_INC

#include "rng/generator/TypedRandomGenerator.class.hpp"
#include "core/SimulationObject.class.h"
#include "boost/shared_ptr.hpp"

#include <vector>

typedef RNG::TypedRandomGenerator<double> TRGd;
typedef boost::shared_ptr< TRGd > bspTRGd;

namespace GLOOPER_TEST
{
   class InfoGenerator: public SimulationObject
   {
   public:

      InfoGenerator(
	    const bspTRGd& value_generator);

      double get_info(unsigned long turn_timer);

      virtual void reset();

      XmlField xml_description() const;

      InfoGenerator* clone() const;

   protected:

      virtual const bool generate_now(unsigned long turn_timer) = 0;

      bspTRGd value_generator;

      virtual const char* info_generator_type() const = 0;

      std::vector<double> info_values;

      //! A container for valid info values \f$\in [0,1]$\f
      std::vector<double> actual_values;

      virtual InfoGenerator* real_clone() const = 0;

   private:

      static unsigned long instance_ctr;
   };
}

#endif   // ----- #ifndef INFOGENERATOR_CLASS_INC  -----
