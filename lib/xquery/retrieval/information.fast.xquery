(fn:string-join( ( "simulation_id","batch_id","run_id","step_id","registration_timer","information_value" ), ',' ),
let $sim_id := $1
for $st in fn:doc("SimulationDB")/SimulationData/Simulation[@id=$sim_id]/Batch/Run/Step
   let $st_cid := $st/@context_id
   let $id_seq := fn:tokenize($st_cid,'\.')
   for $inf in $st/Information
      stable order by $st/xs:integer(@id) ascending
      return fn:string-join( ( data($id_seq), data($inf/@registration_timer), data($inf/@value) ), ',' )
      )
