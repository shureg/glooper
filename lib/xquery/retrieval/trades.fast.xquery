(fn:string-join( ( "simulation_id","batch_id","run_id","step_id","registration_timer","trade_id","quantity","price","active_id","passive_id" ), ',' ),
let $sim_id := $1
for $st in fn:doc("SimulationDB")/SimulationData/Simulation[@id=$sim_id]/Batch/Run/Step
   let $st_cid := $st/@context_id
   let $id_seq := fn:tokenize($st_cid,'\.')
   for $trd in $st/Trade
      stable order by $st/xs:integer(@id) ascending
      return fn:string-join( ( data($id_seq), data($trd/@registration_timer), data($trd/@id), data($trd/*) ), ',' )
      )
