(fn:string-join( ( "simulation_id","batch_id","run_id","step_id","registration_timer","depth","order_id","price","quantity","bid","market","order_time","owner_id" ), ',' ),
let $sim_id := $1
for $st in fn:doc("SimulationDB")/SimulationData/Simulation[@id=$sim_id]/Batch/Run/Step
   let $st_cid := $st/@context_id
   let $id_seq := fn:tokenize($st_cid,'\.')
   for $mkt in $st/Market
      let $reg_t := $mkt/@registration_timer
      for $ord in $mkt//LimitOrder
	 stable order by $st/xs:integer(@id)
	 return fn:string-join( (data($id_seq), data($reg_t), data($ord/depth), data($ord/Order/@id), data($ord/Order/*)  ), ',')
	 )
