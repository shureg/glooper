(string-join( ( "simulation_id","batch_id","run_id","step_id",
   "agent_id", "belief", "timer", "type", "wealth", "position", "des_inv_prop", "min_rev_prob", "min_bel_adj_prop", "max_bid_ask", 
   "bankrupt", "registration_timer", "snapshot_type" ), ',' ),
let $sim_id := $1
for $bt in doc("SimulationDB")/SimulationData/Simulation[id=$sim_id]/Batch
   let $bt_id := $bt/id
   for $rn in $bt/Run
      let $rn_id := $rn/id
      for $st in $rn/Step
	 let $st_id := $st/id
	 let $rt_0 := $st/Agent_Population_before/registration_timer
	 let $rt_1 := $st/Agent_Population_after/registration_timer
	 return (
	 for $agt in $st/Agent_Population_before/Agent
	    return string-join( ( string($sim_id), data($bt_id), data($rn_id), data($st_id), data($agt/*), data($rt_0), "start_turn" ), ',' ),
	 for $agt in $st/Agent
	    return string-join( ( string($sim_id), data($bt_id), data($rn_id), data($st_id), data($agt/*), "mid_turn" ), ',' ),
	 for $agt in $st/Agent_Population_after/Agent
	    return string-join( ( string($sim_id), data($bt_id), data($rn_id), data($st_id), data($agt/*), data($rt_1), "end_turn" ), ',' )
	    )
	 )
