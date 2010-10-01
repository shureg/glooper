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

#include "core/SimulationObject.class.h"

#include <vector>

namespace GLOOPER_TEST
{
   class InfoGenerator: public SimulationObject
   {
   public:

      InfoGenerator();

      double get_info(unsigned long turn_timer);

      virtual void reset();

      XmlField xml_description() const;

      InfoGenerator* clone() const;

   protected:

      virtual const bool generate_now() = 0;

      virtual double value() const = 0;

      virtual void generation_config(){}

      virtual const char* info_generator_type() const = 0;

      std::vector<double> info_values;

      //! A container for valid info values \f$\in [0,1]$\f
      std::vector<double> actual_values;

      unsigned long current_turn_timer;

      virtual InfoGenerator* real_clone() const = 0;

   private:

      static unsigned long instance_ctr;
   };
}

#endif   // ----- #ifndef INFOGENERATOR_CLASS_INC  -----
