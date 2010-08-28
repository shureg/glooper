--The headers field table can contain single values as well as {table_header,original_header} pairs

agt = 
   {
      table_id = "agt",
      field_name = "Agent",
      contexts = {
	 "mid_turn",
	 {container = "Initial.Agent.Population", label="init"},
	 {container = "Step.End.Agent.Population", label="end_turn"}
      },
      headers = {
      {'agent_id','id'},
      {'belief','belief'},
      {'timer','timer'},
      {'wealth','wealth'},
      {'position','position'},
      {'type','type'},
      {'des_inv_prop','desired_investment_proportion'},
      {'cur_inv_prop','current_investment_proportion'},
      {'inv_value','investment_value'},
      {'bankrupt','is_bankrupt'},
--ClassicAgent attributes
      {'min_rev_prob','minimum_revision_probability'},
      {'min_bel_adj_prop','minimum_belief_adjustment_proportion'},
      {'max_bid_ask','maximum_bid-ask_spread'},
--LuaComplexAgent attributes
      {'trend_mode','trend_mode'},
      {'max_memory','max_memory'},
      {'sig_threshold','significance_threshold'}
      }
   }

trd = 
   {
      table_id = "trd",
      field_name = "Trade",
      headers = {
	 {'trade_id','id'},
	 'quantity',
	 'price',
	 'active_id',
	 'passive_id'
      }
   }

ord = 
   {
      table_id = "ord",
      field_name = "Order",
      headers = {
	 {'order_id','id'},
	 'price',
	 'quantity',
	 'is_bid',
	 'is_market',
	 'order_time',
	 'owner_id'
      }
   }

inf = 
   {
      table_id = "inf",
      field_name = "Information",
      headers = {
	 {'information_value','value'}
      }
   }

lob =
   {
      table_id = "lob",
      field_name = "Order",
      contexts = {
	 {container = "Market", label="limit_order"}
      },
      headers = {
	 {'order_id','id'},
	 'is_bid',
	 'depth',
	 'price',
	 'quantity',
	 'is_market',
	 'order_time',
	 'owner_id'
      }
   }

tables = {agt,trd,ord,inf,lob}
