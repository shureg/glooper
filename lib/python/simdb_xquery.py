inf_label_dict = {}

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

trd_label_dict = {}

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

ord_label_dict = {}

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

lob_label_dict = {}

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

agt_labels = [
      ('belief','@belief'),
      ('timer','@timer'),
      ('wealth','@wealth'),
      ('position','@position'),
      ('type','@type'),
      ('des_inv_prop','@desired_investment_proportion'),
      ('cur_inv_prop','@current_investment_proportion'),
      ('inv_value','@investment_value'),
      ('bankrupt','@is_bankrupt'),
#ClassicAgent attributes
      ('min_rev_prob','@minimum_revision_probability'),
      ('min_bel_adj_prop','@minimum_belief_adjustment_proportion'),
      ('max_bid_ask','@maximum_bid-ask_spread'),
#LuaComplexAgent attributes
      ('trend_mode','@trend_mode'),
      ('max_memory','@max_memory'),
      ('sig_threshold','@significance_threshold')
      ]

agt_csv_labels = ', '.join( map( lambda x: '"%s"' % x[0],agt_labels ) )
agt_xpath_labels = ', '.join( map( lambda x: "string(data($agt/%s))" % x[1],agt_labels ) )

agt_label_dict = {'agt_csv': agt_csv_labels, 'agt_xpath': agt_xpath_labels}

agt_xq = (
"""
(fn:string-join( ( "simulation_id", "batch_id", "run_id", "step_id", "registration_timer", 
                    "agent_id", %(agt_csv)s, "snapshot_type" ), ',' ),
for $st in %(xpath_root)s/Simulation[@id=%(sim_id)d]/Batch[@id=%(bat_id)d]/Run[@id=%(run_id)d]/Step
   let $st_cid := $st/@context_id
   let $id_seq := fn:tokenize($st_cid,'\.')
   let $rt_1 := $st/Step.End.Agent.Population/@registration_timer
   stable order by $st/xs:integer(@id) ascending
   return (
   for $agt in $st/Agent
      return fn:string-join( ( data($id_seq), data($agt/@registration_timer), data($agt/@id), %(agt_xpath)s, "mid_turn" ), ',' ),
   for $agt in $st/Step.End.Agent.Population/Agent
      return fn:string-join( ( data($id_seq), data($rt_1), data($agt/@id), %(agt_xpath)s, "end_turn" ), ',' )
      )
   )
"""
)

def get_xquery(ext_dict):
   ext_dict.update( eval("%(xq_label)s_label_dict" % ext_dict) )
   return eval("%(xq_label)s_xq" % ext_dict) % ext_dict
