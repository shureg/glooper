(string-join( ( "simulation_id","batch_id","run_id","step_id","registration_timer","depth","order_id","price","quantity","bid","market","order_time","owner_id" ), ',' ),
let $sim_id := $1
for $bt in doc("SimulationDB")/SimulationData/Simulation[id=$sim_id]/Batch
   let $bt_id := $bt/id
   for $rn in $bt/Run
      let $rn_id := $bt/id
      for $st in $rn/Step
	 let $st_id := $st/id
	 for $mkt in $st/Market
	    let $reg_t := $mkt/registration_timer
	    for $ord in $mkt//LimitOrder
	       return string-join( ( string($sim_id), data($bt_id), data($rn_id), data($st_id), data($reg_t), data($ord/depth), data($ord/Order/*)  ), ',')
	       )
