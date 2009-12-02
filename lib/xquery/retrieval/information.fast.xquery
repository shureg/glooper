(string-join( ( "simulation_id","batch_id","run_id","step_id","information_value","registration_timer" ), ',' ),
let $sim_id := $1
for $bt in doc("SimulationDB")/SimulationData/Simulation[id=$sim_id]/Batch
   let $bt_id := $bt/id
   for $rn in $bt/Run
      let $rn_id := $rn/id
      for $st in $rn/Step
	 let $st_id := $st/id
	 for $inf in $st/Information
	    return string-join( ( string($sim_id), data($bt_id), data($rn_id), data($st_id), data($inf/*) ), ',' )
	    )
