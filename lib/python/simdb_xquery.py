inf_xq = (
"""
(fn:string-join( ( "simulation_id","batch_id","run_id","step_id","registration_timer","information_value" ), ',' ),
for $st in %(xpath_root)s/Simulation[@id=%(sim_id)d]/Batch[@id=%(bat_id)d]/Run[@id=%(run_id)d]/Step
   let $st_cid := $st/@context_id
   let $id_seq := fn:tokenize($st_cid,'\.')
   for $inf in $st/Information
      stable order by $st/xs:integer(@id) ascending
      return fn:string-join( ( data($id_seq), data($inf/@registration_timer), data($inf/@value) ), ',' )
      )
""")

trd_xq = (
"""
(fn:string-join( ( "simulation_id","batch_id","run_id","step_id","registration_timer","trade_id","quantity","price","active_id","passive_id" ), ',' ),
for $st in %(xpath_root)s/Simulation[@id=%(sim_id)d]/Batch[@id=%(bat_id)d]/Run[@id=%(run_id)d]/Step
   let $st_cid := $st/@context_id
   let $id_seq := fn:tokenize($st_cid,'\.')
   for $trd in $st/Trade
      stable order by $st/xs:integer(@id) ascending
      return fn:string-join( ( data($id_seq), data($trd/@registration_timer), data($trd/@id), 
                               data($trd/@quantity), data($trd/@price), data($trd/@active_id), data($trd/@passive_id) ), ',' )
      )
"""
)

ord_xq = (
"""
(fn:string-join( ( "simulation_id","batch_id","run_id","step_id", "registration_timer", "order_id", 
		     "price", "quantity", "bid", "market", "order_time", "owner_id"), ','),
for $st in %(xpath_root)s/Simulation[@id=%(sim_id)d]/Batch[@id=%(bat_id)d]/Run[@id=%(run_id)d]/Step
   let $st_cid := $st/@context_id
   let $id_seq := fn:tokenize($st_cid,'\.')
   for $ord in $st/Order
      stable order by $st/xs:integer(@id) ascending
      return fn:string-join( ( data($id_seq), data($ord/@registration_timer), data($ord/@id), 
				 data( ($ord/@price, $ord/@quantity, $ord/@is_bid, $ord/@is_market, $ord/@order_time, $ord/@owner_id ) ) ), ',' )
      )
"""
)

lob_xq = (
"""
(fn:string-join( ( "simulation_id","batch_id","run_id","step_id","registration_timer","depth","order_id","price","quantity","bid","order_time","owner_id" ), ',' ),
for $st in %(xpath_root)s/Simulation[@id=%(sim_id)d]/Batch[@id=%(bat_id)d]/Run[@id=%(run_id)d]/Step
   let $st_cid := $st/@context_id
   let $id_seq := fn:tokenize($st_cid,'\.')
   for $mkt in $st/Market
      let $reg_t := $mkt/@registration_timer
      for $ord in $mkt/Order
	 stable order by $st/xs:integer(@id)
	 return fn:string-join( (data($id_seq), data($reg_t), data($ord/@depth), data($ord/@id), 
				    data( ($ord/@price, $ord/@quantity, $ord/@is_bid, $ord/@order_time, $ord/@owner_id ) )  ), ',')
	 )
"""
)

agt_xq = (
"""
(fn:string-join( ( "simulation_id","batch_id","run_id","step_id", "registration_timer",
   "agent_id", "belief", "timer", "type", "wealth", "position", "des_inv_prop", "min_rev_prob", "min_bel_adj_prop", "max_bid_ask", 
   "bankrupt", "snapshot_type" ), ',' ),
for $st in %(xpath_root)s/Simulation[@id=%(sim_id)d]/Batch[@id=%(bat_id)d]/Run[@id=%(run_id)d]/Step
   let $st_cid := $st/@context_id
   let $id_seq := fn:tokenize($st_cid,'\.')
   let $rt_1 := $st/Agent_Population_after/@registration_timer
   stable order by $st/xs:integer(@id) ascending
   return (
   for $agt in $st/Agent
      return fn:string-join( ( data($id_seq), data($agt/@registration_timer), data($agt/@id), data($agt/*), "mid_turn" ), ',' ),
   for $agt in $st/Agent_Population_after/Agent
      return fn:string-join( ( data($id_seq), data($rt_1), data($agt/@id), data($agt/*), "end_turn" ), ',' )
      )
   )
"""
)

