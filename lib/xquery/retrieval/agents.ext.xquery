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
	       return <step>
		  <begin_step>
		  {
		  let $rt := $st/Agent_Population_before/registration_timer
		     for $agt in $st/Agent_Population_before/Agent
			return<Agent>
			<simulation_id>{ data($sim_id) }</simulation_id>
			<batch_id>{ data($bt_id) }</batch_id>
			<run_id>{ data($rn_id) }</run_id>
			<step_id>{ data($st_id) }</step_id>
			{ $agt/* }
			{ $rt }
			<snapshot_type>{ "start_turn" }</snapshot_type>
			</Agent>
		  }
		  </begin_step>
		  <mid_step>
		  {
		     for $agt in $st/Agent
			return<Agent>
			<simulation_id>{ data($sim_id) }</simulation_id>
			<batch_id>{ data($bt_id) }</batch_id>
			<run_id>{ data($rn_id) }</run_id>
			<step_id>{ data($st_id) }</step_id>
			{ $agt/* }
			<snapshot_type>{ "mid_turn" }</snapshot_type>
			</Agent>
		  }
		  </mid_step>
		  <end_step>
		  {
		  let $rt := $st/Agent_Population_after/registration_timer
		     for $agt in $st/Agent_Population_after/Agent
			return<Agent>
			<simulation_id>{ data($sim_id) }</simulation_id>
			<batch_id>{ data($bt_id) }</batch_id>
			<run_id>{ data($rn_id) }</run_id>
			<step_id>{ data($st_id) }</step_id>
			{ $agt/* }
			{ $rt }
			<snapshot_type>{ "end_turn" }</snapshot_type>
			</Agent>
		  }</end_step>
	       </step>
	 }</run>
   }</batch>
}
</simulation>
