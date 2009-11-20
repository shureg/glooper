<simulation>
{
for $bt in doc("SimulationDB")/SimulationData/Simulation[id=46]/Batch
   let $bt_id := $bt/id
   return <batch>
   {
      for $rn in $bt/Run
	 let $rn_id := $rn/id
	 return <run>
	 {
	    for $st in $rn/Step
	       let $st_id := $st/id
	       return <step>{
		  for $mkt in $st//Market
		     let $reg_t := $mkt/registration_timer
		     return<CrossSection>
		     {
			for $ord in $mkt//LimitOrder
			   return <LimitOrder>
			   <simulation_id>{ 46 }</simulation_id>
			   <batch_id>{ data($bt_id) }</batch_id>
			   <run_id>{ data($rn_id) }</run_id>
			   <step_id>{ data($st_id) }</step_id>
			   { $reg_t }
			   { $ord/depth }
			   { $ord/Order/* }
			   </LimitOrder>
		     }
		     </CrossSection>
	       }</step>
	 }</run>
   }</batch>
}
</simulation>
