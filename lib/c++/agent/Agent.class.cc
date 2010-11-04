// =====================================================================================
// 
//       Filename:  Agent.class.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  02/06/09 17:48:40
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Alexander Kabanov (aak), alexander.kabanov@gmail.com
//        Company:  
// 
// =====================================================================================

#include "agent/Agent.class.h"
#include "boost/make_shared.hpp"

using boost::make_shared;

using namespace GLOOPER_TEST;

unsigned long Agent::instance_counter(0);

Agent::Agent(double _belief, 
      const RNG::TypedRandomGenerator<double>& private_info_generator):
   SimulationObject(++instance_counter),
   belief(_belief),
   private_info_generator( 
	 boost::shared_ptr< RNG::TypedRandomGenerator<double> >( 
	    private_info_generator.clone() ) ),
   private_info_generator_string(
	 private_info_generator.xml_description().
	    string_format("qXML_line"))
{}

void Agent::receive_public_information(double xi)
{
   if(xi>=0 && xi <= 1.)
      public_information_record.push_back( process_public_information(xi) );
   else
      public_information_record.push_back(-1);
}

void Agent::update_belief()
{
   double private_xi = (*private_info_generator)();
   double public_xi = public_information_record.back();

   if(public_xi >= 0 && public_xi <= 1)
      belief = combine_public_private_information(public_xi , private_xi);
   else
      belief = private_xi;
}

Agent* Agent::clone() const
{
   return real_clone();
}

boost::shared_ptr<Market> Agent::get_market() const
{
   return spot_mkt;
}

Agent& Agent::set_market(const boost::shared_ptr<Market>& mkt)
{
   spot_mkt = mkt;
   return *this;
}

Agent& Agent::set_timer(const boost::shared_ptr<timer_signal>& _timer)
{
   timer = _timer;
   return *this;
}

Agent& Agent::set_ro_timer(const boost::shared_ptr<timer_signal>& _ro_timer)
{
   ro_timer = _ro_timer;
   return *this;
}

XmlField Agent::xml_description() const
{
   XmlField tmp("Agent");

   tmp("id") = id;

   tmp("type") = agent_type_str();
   
   tmp("belief") = belief;
   
   tmp("timer") = (*ro_timer)();

   tmp("private_info_generator") = private_info_generator_string;

   return tmp;
}
