let $sim_id := doc("tempsimdb")/Simulation/id
return
<simulation>
{
for $bt in doc("tempsimdb")/Simulation/Batch
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
		  for $ord in $st/Order
		     return<Order>
		     <simulation_id>{ data($sim_id) }</simulation_id>
		     <batch_id>{ data($bt_id) }</batch_id>
		     <run_id>{ data($rn_id) }</run_id>
		     <step_id>{ data($st_id) }</step_id>
		     { $ord/* }
		     </Order>
	       }</step>
	 }</run>
   }</batch>
}
</simulation>
