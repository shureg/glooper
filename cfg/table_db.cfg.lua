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
--Agent attributes
      {'agent_id','id'},
      {'belief','belief'},
      {'timer','timer'},
      {'type','type'},
      {'private_info_generator','private_info_generator'},
--TradingAgent attributes
      {'wealth','wealth'},
      {'position','position'},
      {'inv_value','investment_value'},
      {'total_inv_value','total_investment_value'},
      {'des_inv_prop','desired_investment_proportion'},
      {'cur_inv_prop','current_investment_proportion'},
      {'bankrupt','is_bankrupt'},
      {'overleveraged','is_overleveraged'},
      {'spread_generator','spread_generator'},
--ClassicAgent attributes
      {'min_rev_prob','minimum_revision_probability'},
      {'min_bel_adj_prop','minimum_belief_adjustment_proportion'},
      {'max_bid_ask','maximum_bid-ask_spread'},
--StandardComplexAgent attributes
      {'max_memory','max_memory'},
      {'sig_threshold','significance_threshold'},
      {'trend_mode','trend_mode'},
      {'public_proportion','public_proportion'},
      {'private_proportion','private_proportion'},
--NoiseTrader attributes
      {'belief_generator','belief_generator'}
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
	 'is_crossing',
	 'order_time',
	 'owner_id',
	 'bid0_price',
	 'bid0_quantity',
	 'ask0_price',
	 'ask0_quantity',
	 'bid1_price',
	 'bid1_quantity',
	 'ask1_price',
	 'ask1_quantity',
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
