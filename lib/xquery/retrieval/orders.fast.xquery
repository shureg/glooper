(fn:string-join( ( "simulation_id","batch_id","run_id","step_id", "registration_timer", "order_id", "price", "quantity", "bid", "market", "order_time", "owner_id"), ','),
let $sim_id := $1
for $st in fn:doc("SimulationDB")/SimulationData/Simulation[@id=$sim_id]/Batch/Run/Step
   let $st_cid := $st/@context_id
   let $id_seq := fn:tokenize($st_cid,'\.')
   for $ord in $st/Order
      stable order by $st/xs:integer(@id) ascending
      return fn:string-join( ( data($id_seq), data($ord/@registration_timer), data($ord/@id), data($ord/*) ), ',' )
      )
