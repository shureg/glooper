(fn:string-join( ( "simulation_id","batch_id","run_id","step_id", "registration_timer",
   "agent_id", "belief", "timer", "type", "wealth", "position", "des_inv_prop", "min_rev_prob", "min_bel_adj_prop", "max_bid_ask", 
   "bankrupt", "snapshot_type" ), ',' ),
let $sim_id := $1
for $st in fn:doc("SimulationDB")/SimulationData/Simulation[@id=$sim_id]/Batch/Run/Step
   let $st_cid := $st/@context_id
   let $id_seq := fn:tokenize($st_cid,'\.')
   let $rt_0 := $st/Agent_Population_before/@registration_timer
   let $rt_1 := $st/Agent_Population_after/@registration_timer
   stable order by $st/xs:integer(@id) ascending
   return (
   for $agt in $st/Agent_Population_before/Agent
      return fn:string-join( ( data($id_seq), data($rt_0), data($agt/@id), data($agt/*), "start_turn" ), ',' ),
   for $agt in $st/Agent
      return fn:string-join( ( data($id_seq), data($agt/@registration_timer), data($agt/@id), data($agt/*), "mid_turn" ), ',' ),
   for $agt in $st/Agent_Population_after/Agent
      return fn:string-join( ( data($id_seq), data($rt_1), data($agt/@id), data($agt/*), "end_turn" ), ',' )
      )
   )
