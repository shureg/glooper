#!/usr/bin/env python

numWidth = 4
textWidth = 40
indentSize = 3
workDir = 'multibatch/new_cfg'

AgentTypeDict = {'StandardComplexAgent': 
      ['belief', 'private_info_generator', 'wealth', 'spread_generator','mean_reversion','max_memory','significance_threshold','public_weight','private_weight']
      }

InfoTypeDict = {
      'SimpleInfoGenerator': ['value_generator'],
      'SimpleIntermittentInfoGenerator': ['value_generator','generation_prob'],
      'TriangularMarkovianInfoGenerator': ['generation_prob']
      }

from Tkinter import *
import tkFileDialog, tkMessageBox, tkSimpleDialog
import os.path
import os
import tempfile
import hashlib
import pickle

root = Tk()
root.title("glooper config manager")

#A class holding a labelled section
class LabelledSection:

   def __init__(self, parent, label):
      self.parent = parent
      self.label_frame = Frame(parent)
      self.body_frame = Frame(parent)
      self.label = Label(self.label_frame, text = label)

   def pack(self, **kwargs):
      self.label.pack(side=TOP)
      self.label_frame.pack(side=TOP)
      self.body_frame.pack(side=BOTTOM, **kwargs)

#A class holding a labelled entry box
class LabelledEntry:

   def __init__ (self, parent, label, width):
      self.parent = parent
      self.holding_frame = parent #Frame(parent)
      self.label = Label(self.holding_frame, text = label)
      self.entry = Entry(self.holding_frame, width = width)

   def grid(self, row, start_col, **kwargs):
      self.label.grid(row=row, column = start_col, sticky = E)
      self.entry.grid(row=row, column = start_col+1, sticky = W, **kwargs)
      
#   def __del__(self):
#      self.label.destroy()
#      self.entry.destroy()
      #self.holding_frame.destroy()

   def get(self):
      return self.entry.get()

   def set(self, new_text):
      self.entry.delete(0,END)
      self.entry.insert(END,new_text)
      return self

#A class holding a labelled entry box and "generator" tickbox (for agent parameters)
class AgentEntry(LabelledEntry):

   def __init__(self, parent, root, label):
      LabelledEntry.__init__(self, parent, label, textWidth)
      self.is_generator = IntVar(root)
      self.generator_tick = Checkbutton(self.holding_frame, variable = self.is_generator, takefocus=0)

   def grid(self, row):
      LabelledEntry.grid(self, row, 0)
      self.generator_tick.grid(row=row, column = 2)

#   def __del__(self):
#      self.generator_tick.destroy()
      #self.fill_frame.destroy()
#      LabelledEntry.__del__(self)

#A class holding a listbox with a scrollbar
class ScrollBox:

   def __init__(self, parent, selectmode, height = -1):
      self.parent = parent
      self.scrollbar = Scrollbar(parent, orient=VERTICAL)
      
      if (height < 0):
	 self.listbox = Listbox(parent, yscrollcommand=self.scrollbar.set, selectmode=selectmode)
      else:
	 self.listbox = Listbox(parent, yscrollcommand=self.scrollbar.set, selectmode=selectmode, height=height)

      self.scrollbar.configure(command=self.listbox.yview)

   def pack(self, **kwargs):
     self.listbox.pack(side=LEFT,fill=BOTH,expand=1)
     self.scrollbar.pack(side=RIGHT,fill=Y)


#A class emulating a named combobox
class ComboBox:

   def __init__(self, parent, root, title, default, *args):
      self.var = StringVar(root)
      self.var.set(default)
      self.parent = parent
      self.holding_frame = parent
      self.label = Label(self.holding_frame, text=title)
      self.combo_box = OptionMenu(self.holding_frame,self.var, *args)

   def grid(self, row, start_col):
      self.label.grid(row = row, column = start_col)
      self.combo_box.grid(row = row, column = start_col + 1)

#A class implementing a field update signal
class UpdateSignal:

   def __init__(self, **dep_fields):
      self.slots = []
      self.dep_fields = dep_fields
      for v in self.dep_fields.itervalues():
	 v.entry.bind('<Return>', self.update)

   def add_slot(self, update_fun):
      self.slots.append( update_fun )

   def update(self, event):
      field_values = dict.fromkeys(self.dep_fields)
      for k in field_values.iterkeys():
	 field_values[k] = self.dep_fields[k].get()
      for s in self.slots:
	 s(**field_values)


#A class implementing a generated non-editable entry
class GeneratedEntry(LabelledEntry):

   def __init__(self, parent, label, width, update_fun, signal_object):
      LabelledEntry.__init__(self, parent, label, width)
      self.entry.config(state="readonly")
      self.update_fun = update_fun
      signal_object.add_slot(self.update)

   def update(self, **kwargs):
      var = self.update_fun(**kwargs)
      self.entry.config(state=NORMAL)
      self.set(var)
      self.entry.config(state="readonly")
      return self

#Button commands

agt_entries = dict()

#validate agent type and populate attribute entry fields accordingly
def validate_agent_type():
   agt_type_str = AgentTypeEntry.var.get()
   if not agt_type_str in AgentTypeDict:
      tkMessageBox.showerror("Agent Type Lookup", "Agent type %s is unknown" % agt_type_str )
   else:
      global agt_entries
      global root
      if(len(agt_entries) > 0):
	 agt_entries.clear()

      agt_type_attrs = AgentTypeDict[agt_type_str]
      agt_entries = dict()
      row_ctr = 1
      for a in agt_type_attrs:
	 agt_entries[a] = AgentEntry(AgentVarFrame,root,a)
	 agt_entries[a].grid(row=row_ctr)
	 row_ctr = row_ctr + 1

staged_populations = dict()

pop_ctr = 0

def get_population_name(default_name):
   pop_name = tkSimpleDialog.askstring("Population Name","Please enter the new populaiton name", initialvalue = default_name)
   return pop_name

#add an agent population to the staged populations area
def add_population(pname = None):

   global agt_entries
   global staged_populations
   global pop_ctr

   if len(agt_entries) == 0:
      tkMessageBox.showerror("Agent Population Add", "Agent attributes have not been populated")

   pop_name = None

   if(pname == None):
   
      pop_name0 = "Population %d" % pop_ctr

      pop_name = get_population_name(pop_name0)
   else:
      pop_name = pname

   
   if pop_name != None:

      agt_attr_dict = dict.fromkeys(agt_entries.iterkeys())

      for a in agt_attr_dict.iterkeys():
	 agt_attr_dict[a] = (agt_entries[a].entry.get(), agt_entries[a].is_generator.get())

      pop_ctr = pop_ctr + 1

      staged_populations[pop_name] = {'type': AgentTypeEntry.var.get()  ,'N': NAgentEntry.entry.get(), 'attrs': agt_attr_dict}

      AgentStagedPops.listbox.insert(END,pop_name)

#Update the populaiton list based on the entires in the currently selected population
def update_selected_population():

   global agt_entries
   global staged_populations

   if len(agt_entries) == 0:
      tkMessageBox.showerror("Agent Population Add", "Agent attributes have not been populated")

   agt_attr_dict = dict.fromkeys(agt_entries.iterkeys())

   for a in agt_attr_dict.iterkeys():
      agt_attr_dict[a] = (agt_entries[a].get(), agt_entries[a].is_generator.get())

   #pop_ix = int(AgentStagedPops.listbox.curselection()[0])

   pop_name = AgentStagedPops.listbox.get(ACTIVE)

   staged_populations[pop_name] = {'type': AgentTypeEntry.var.get()  ,'N': NAgentEntry.entry.get(), 'attrs': agt_attr_dict}

#change the population label
def rename_population():

   global staged_populations

   pop_ix = int(AgentStagedPops.listbox.curselection()[0])

   pop_name0 = AgentStagedPops.listbox.get(pop_ix)

   pop_name = get_population_name(pop_name0)

   if pop_name != None:
      remove_population(pop_ix)
      add_population(pop_name)


#remove an agent population from the staged populations area
def remove_population(index = -1):

   global agt_entries
   global staged_populations

   pop_ix = None

   if (index >= 0):
      pop_ix = index
   else:
      pop_ix = int(AgentStagedPops.listbox.curselection()[0])

   pop_name = AgentStagedPops.listbox.get(pop_ix)

   AgentStagedPops.listbox.delete(pop_ix)

   staged_populations.pop(pop_name)

#convert a staged population data structure to GUI representation
def population_to_entries(pop):

   global root
   global NAgentEntry
   global AgentTypeEntry

   new_agt = pop

   NAgentEntry.entry.delete(0,END)
   NAgentEntry.entry.insert(END,new_agt["N"])

   AgentTypeEntry.var.set(new_agt["type"])

   attrs = new_agt["attrs"]

   agt_entries.clear()

   row_ctr = 1

   for a in AgentTypeDict[new_agt["type"]] :
      agt_entries[a] = AgentEntry(AgentVarFrame, root, a)
      agt_entries[a].entry.insert(END, attrs[a][0])
      agt_entries[a].is_generator.set(attrs[a][1])
      agt_entries[a].grid(row=row_ctr)
      row_ctr = row_ctr + 1

#update the agent attributes based on the currently selected staged population
def update_population(event, index = -1):

   if(AgentStagedPops.listbox.size() > 0):

      pop_ix = None

      if index >= 0:
	 pop_ix = index
      else:
	 pop_ix = int(AgentStagedPops.listbox.curselection()[0])

      pop_name = AgentStagedPops.listbox.get(pop_ix)

      population_to_entries(staged_populations[pop_name])


info_entries = {}

#Validating the info generator type
def validate_info_type():
   info_type_str = InfoGenType.var.get()
   if not info_type_str in InfoTypeDict:
      tkMessageBox.showerror("Info Generator Type Lookup", "Type %s is unknown" % info_type_str )
   else:
      global info_entries
      if(len(info_entries) > 0):
	 info_entries.clear()

      info_type_attrs = InfoTypeDict[info_type_str]
      info_entries = dict(type = info_type_str, attrs = dict())
      row_ctr = 1
      for a in info_type_attrs:
	 info_entries['attrs'][a] = LabelledEntry(InfoVarFrame,a,textWidth)
	 info_entries['attrs'][a].grid(row_ctr,0)
	 row_ctr = row_ctr + 1

#converting info entries into info data structure
def info_entries_to_data_structure(info):
   info_type_attrs = InfoTypeDict[info['type']]
   info_ds = dict(type = info['type'], attrs = dict())
   for a in info_type_attrs:
      info_ds['attrs'][a] = info['attrs'][a].entry.get()

   return info_ds

#convert info data structure to GUI representation
def info_data_structure_to_entries(info_ds):
   global InfoGenType
   global info_entries
   
   InfoGenType.var.set(info_ds['type'])

   info_type_attrs = InfoTypeDict[info_ds['type']]
   info_entries = dict(type = info_ds['type'], attrs = dict())
   row_ctr = 1
   for a in info_type_attrs:
      info_entries['attrs'][a] = LabelledEntry(InfoVarFrame,a,textWidth)
      info_entries['attrs'][a].grid(row_ctr,0)
      info_entries['attrs'][a].set( info_ds['attrs'][a] )
      row_ctr = row_ctr + 1


#Browsing for the output path
def output_path_browse():
   outdir = tkFileDialog.askdirectory(initialdir = workDir)
   OutputPath.entry.delete(0,END)
   OutputPath.entry.insert(END,outdir)


#Updating generated filenames based on the filed values supplied
def generate_fname(outdir, case, fstem, ext):
   if outdir != "" and case != "" and fstem != "" and ext != "":
      return os.path.abspath(os.path.join(outdir, "%s_%s.sim.cfg.%s" % (case, fstem, ext) ) )
   else:
      return ""

current_ds = None

#validating the data-holding structure
def validate_data_structure(ds):

   missing_values = lambda x: any( map(lambda y: y == None or y == "", x.itervalues()) ) 

   fields_empty = missing_values(ds) or missing_values(ds["agent_population"]) or missing_values(ds["info_generator"])

   if fields_empty:
      tkMessageBox.showerror("Invalid Data","All fields are compulsory")
      raise Exception

#creating the data-holding structure
def create_data_structure():
   global current_ds
   CfgInputUpdater.update(None)
   data_structure = dict(
	 N_threads = NThreadEntry.get(),
	 N_batches = NBatchEntry.get(),
	 N_runs = NRunEntry.get(),
	 N_steps = NStepEntry.get(),
	 sim_comment = CommentBox.get(),
	 agent_population = staged_populations,
	 info_generator = info_entries_to_data_structure(info_entries),
	 case_prefix = CasePrefix.get(),
	 fname_stem = FileNameStem.get(),
	 outpath = OutputPath.get(),
	 sim_cfg = SimCfgDisplay.get(),
	 agt_cfg = AgtCfgDisplay.get(),
	 bin_cfg = BinCfgDisplay.get(),
	 )
   current_ds = data_structure
   return data_structure

def load_data_structure(ds):

   global staged_populations
   global AgentStagedPops

   NThreadEntry.set( ds["N_threads"] )
   NBatchEntry.set( ds["N_batches"] )
   NRunEntry.set( ds["N_runs"] )
   NStepEntry.set( ds["N_steps"] )
   CommentBox.delete(0, END)
   CommentBox.insert(END, ds["sim_comment"] )
   
   staged_populations = ds["agent_population"]
   AgentStagedPops.listbox.delete(0,END)
   AgentStagedPops.listbox.insert(END, *sorted(staged_populations.iterkeys()))
   update_population(None,0)
   
   info_data_structure_to_entries( ds["info_generator"] )
   
   CasePrefix.set( ds["case_prefix"] )
   FileNameStem.set( ds["fname_stem"] )
   OutputPath.set( ds["outpath"] )

   CfgInputUpdater.update(None)


#Create an agent generator table entry for the agent cfg file
def agent_generator_entry(agt_dict):
   ind_str = ' '*indentSize

   indent = lambda x,n: map(lambda y: ind_str*n + y, x)

   agt_type_str = agt_dict["type"]

   agt_type_attrs = AgentTypeDict[agt_type_str]

   gen_attrs = filter(lambda x: agt_dict["attrs"][x][1], agt_type_attrs)
   non_gen_attrs = filter(lambda x: not (agt_dict["attrs"][x][1]), agt_type_attrs)

   gen_str_list = map(lambda x: "%s = %s," % (x, agt_dict["attrs"][x][0]), gen_attrs)
   non_gen_str_list = map(lambda x: "%s = %s," % (x, agt_dict["attrs"][x][0]), non_gen_attrs)

   gen_str_list[-1] = gen_str_list[-1].rstrip(',')
   non_gen_str_list[-1] = non_gen_str_list[-1].rstrip(',')

   gen_str_list = indent(gen_str_list,1)
   non_gen_str_list = indent(non_gen_str_list,1)

   entry_str_list = ([
	 'generator = agt_gen.fixed_number_agent_generator,',
	 'N_agents = %s,' % agt_dict["N"],
	 'agent_ctor = glooper.%s,' % agt_dict["type"],
	 'generator_args = {'
	 ] + 
   gen_str_list + [
	 '},',
	 'non_generator_args = {'
	 ] +
   non_gen_str_list + [
	 '}'
	 ])

   entry_str_list = indent(entry_str_list,1)

   full_list = ['{'] + entry_str_list +['}']

   full_list = indent(full_list,1)

   return '\n'.join(full_list)

#Generating the config files
def generate_files():
   ds = create_data_structure()

   info_type_str = ds["info_generator"]["type"]
   info_type_attrs = InfoTypeDict[info_type_str]
   info_gen_arg_str = ', '.join( map(lambda x: ds["info_generator"]["attrs"][x], info_type_attrs) ) 

   sim_str_list = [
	 "N_threads = %s" % ds["N_threads"],
	 "N_batches = %s" % ds["N_batches"],
	 "N_runs = %s" % ds["N_runs"],
	 "N_steps = %s" % ds["N_steps"],
	 "",
	 "sim_comment = \"%s\"" % ds["sim_comment"],
	 "",
	 "info = glooper.%s( %s )" % (info_type_str, info_gen_arg_str),
	 "",
	 "agt_gen_cfg_file = \"%s\"" % ds["agt_cfg"]
	 ]

   agt_hdr_str = '\n'.join([
	 r'require "rng_lua_swig_wrap"',
	 r'',
	 r'glooper_path = "/simulation_data/glooper"',
	 r'',
	 r'package.path = glooper_path.."/lib/lua/?.lua;"..package.path',
	 r'package.cpath = glooper_path.."/swig/output/?.so;"..package.cpath',
	 r'',
	 r'require "glooper_lua_swig_wrap"',
	 r'require "agent_generator"',
	 r'',
	 r'glooper = glooper_lua_swig_wrap',
	 r'rng = rng_lua_swig_wrap',
	 r'agt_gen = agent_generator',
	 r''])

   agt_pop = ds["agent_population"]
   
   agt_gen_str = ',\n'.join( map(lambda x: agent_generator_entry( agt_pop[x] ), agt_pop.iterkeys() ) )

   agt_str_list = [
	 agt_hdr_str,
	 'agt_gen.mixed_generator.table = {',
	 agt_gen_str,
	 '}',
	 '',
	 'generator = agt_gen.mixed_agent_generator'
	 ]

   sim_str = '\n'.join(sim_str_list)
   agt_str = '\n'.join(agt_str_list)

   ds["sim_tmp"] = tempfile.mkstemp()
   ds["agt_tmp"] = tempfile.mkstemp()

   sim_tmp = open(ds["sim_tmp"][1],'w')
   agt_tmp = open(ds["agt_tmp"][1],'w')
   
   sim_tmp.write(sim_str)
   agt_tmp.write(agt_str)

   sim_tmp.close()
   agt_tmp.close()

   return ds

#Preview generated files
def preview_generated():
   ds = generate_files()
   os.system("gview -o %s %s" % (ds["sim_tmp"][1], ds["agt_tmp"][1]) )

#View config files
def view_config():
   ds = current_ds
   os.system("gview -o %s %s" % (ds["sim_cfg"], ds["agt_cfg"]))

#Save generated files
def save_cfg():
   ds = generate_files()

   sim_tmp = open(ds["sim_tmp"][1],'r')
   agt_tmp = open(ds["agt_tmp"][1],'r')

   sim_cont = sim_tmp.read()
   agt_cont = agt_tmp.read()
   
   sim_tmp.close()
   agt_tmp.close()

   os.remove(ds["sim_tmp"][1])
   os.remove(ds["agt_tmp"][1])

   ds["sim_hash"] = hashlib.sha512(sim_cont).digest()
   ds["agt_hash"] = hashlib.sha512(agt_cont).digest()
   
   sim_name = ds["sim_cfg"]
   agt_name = ds["agt_cfg"]
   bin_name = ds["bin_cfg"]

   case_label = ds["case_prefix"] +  '_' + ds["fname_stem"]

   proceed = True

   if os.path.exists(sim_name) or os.path.exists(agt_name) or os.path.exists(bin_name):
       proceed = tkMessageBox.askyesno("File Exists","Config files for case %s already exist, overwrite?" % case_label,default=tkMessageBox.NO)

   if proceed:
      sim_file = open(sim_name,'w')
      agt_file = open(agt_name,'w')

      sim_file.write(sim_cont)
      agt_file.write(agt_cont)

      sim_file.close()
      agt_file.close()

      del ds["sim_tmp"]
      del ds["agt_tmp"]

      bin_file = open(ds["bin_cfg"],'w')

      pickle.dump(ds,bin_file)

      bin_file.close()

      tkMessageBox.showinfo("Save Files","Case %s files saved to %s" % (case_label,ds["outpath"]) )

#Load config files
def load_cfg():

   global current_ds

   bf = tkFileDialog.askopenfile(initialdir=workDir, filetypes=[('glooper config holders', '*.sim.cfg.bin')])

   if bf != None:

      ds = pickle.load(bf)


      bf.close()

      failed_load = False

      sim_check = open(ds["sim_cfg"],'r')
      sim_cont = sim_check.read()
      sim_check.close()

      if hashlib.sha512(sim_cont).digest() != ds["sim_hash"]:
	 tkMessageBox.showerror('Load Failed','Top-level config file %s has been changed outside the config manager' % ds["sim_cfg"])
	 failed_load = True
      
      agt_check = open(ds["agt_cfg"],'r')
      agt_cont = agt_check.read()
      agt_check.close()

      if hashlib.sha512(agt_cont).digest() != ds["agt_hash"]:
	 tkMessageBox.showerror('Load Failed','Agent config file %s has been changed outside the config manager' % ds["agt_cfg"])
	 failed_load = True
      
      if not failed_load:
	 load_data_structure(ds)
	 current_ds = ds

#clone current configuration
def clone_cfg():

   def clone_ok():
      CommentBox.delete(0,END)
      CommentBox.insert(END, CQComment.get() )
      CasePrefix.set( CQCaseCode.get() )
      FileNameStem.set( CQFNStem.get() )
      CfgInputUpdater.update(None)
      CloneQuery.destroy()

   CloneQuery = Toplevel(root)
   
   CQTitle = Label(CloneQuery, text = "Clone Config Parameters")
   CQCaseCode = LabelledEntry(CloneQuery, "Case Code", 5)
   CQFNStem = LabelledEntry(CloneQuery, "FileName Stem", 15)
   CQComment = LabelledEntry(CloneQuery, "Simulation Comment", 50)

   CQOK = Button(CloneQuery, text = "OK", command = clone_ok)
   CQCancel = Button(CloneQuery, text = "Cancel", command = CloneQuery.destroy)

   CQTitle.grid(row=0,column=2,columnspan=3)
   CQCaseCode.grid(1,0)
   CQFNStem.grid(1,5,columnspan=3)
   CQComment.grid(2,0,columnspan=8)
   CQOK.grid(row=3,column=1,columnspan=2)
   CQCancel.grid(row=3,column=4,columnspan=2)

#Frame creation
ControlFrame = Frame(root)
CfgFrame = Frame(root)

SimFrame = Frame(CfgFrame, bd = 2)
CommentFrame = Frame(CfgFrame, bd =2)
AgentFrame = Frame(CfgFrame, bd = 2)
InfoFrame = Frame(CfgFrame, bd = 2)
FileFrame = Frame(CfgFrame, bd = 2)

SimSection = LabelledSection(SimFrame, "Simulation Parameters")
CommentSection = LabelledSection(CommentFrame, "Simulation Comment")
AgentSection = LabelledSection(AgentFrame, "Agent Parameters")
InfoSection = LabelledSection(InfoFrame, "Info Parameters")
FileSection = LabelledSection(FileFrame, "Config Files Setup")

AgentFixedFrame = Frame(AgentSection.body_frame)

AgentVarFrame = Frame(AgentSection.body_frame)

AgentStageFrame = Frame(AgentSection.body_frame)
AgentStageListFrame = Frame(AgentStageFrame)
AgentStageControlFrame = Frame(AgentStageFrame)

InfoFixedFrame = Frame(InfoSection.body_frame)
InfoVarFrame = Frame(InfoSection.body_frame)

#Simulation parameter entry boxes
NThreadEntry = LabelledEntry(SimSection.body_frame,"Threads",numWidth)
NBatchEntry = LabelledEntry(SimSection.body_frame,"Batches",numWidth)
NRunEntry = LabelledEntry(SimSection.body_frame, "Runs",numWidth)
NStepEntry = LabelledEntry(SimSection.body_frame, "Steps",numWidth)

#Simulation comment box
CommentBox = Entry(CommentSection.body_frame, width = 50)

#Agent parameter variable frame setup
AgentAttributeLabel = Label(AgentVarFrame, text="attribute")
AgentValueLabel = Label(AgentVarFrame, text="value")
AgentGeneratorLabel = Label(AgentVarFrame, text="generator?")
#AgentFillFrame = Frame(AgentVarHeaderFrame)

#Agent staged populations setup
AgentStagedPops = ScrollBox(AgentStageListFrame, SINGLE, 3)
AgentStagedPops.listbox.bind('<ButtonRelease-1>', update_population)
AddAgentPop = Button(AgentStageControlFrame, text = "Add Population", command = add_population)
UpdateAgentPop = Button(AgentStageControlFrame, text = "Update Population", command = update_selected_population)
RenameAgentPop = Button(AgentStageControlFrame, text = "Rename Population", command = rename_population)
RemoveAgentPop = Button(AgentStageControlFrame, text = "Remove Population", command = remove_population)

#Agent parameter entry boxes
NAgentEntry = LabelledEntry(AgentFixedFrame, "Agents", numWidth)
AgentTypeEntry = ComboBox(AgentFixedFrame, root, "Agent Type", 'StandardComplexAgent', *(AgentTypeDict.keys()) )
AgentTypeButton = Button(AgentFixedFrame, text="Validate Type", command = validate_agent_type)

#Info parameter variable frame setup
InfoAttributeLabel = Label(InfoVarFrame, text="attribute")
InfoValueLabel = Label(InfoVarFrame, text="value")

#Info generator items
InfoGenType = ComboBox(InfoFixedFrame, root, "Info Generator Type", 'SimpleInfoGenerator', *(InfoTypeDict.keys()))
InfoGenTypeButton = Button(InfoFixedFrame, text="Validate Type", command = validate_info_type)

#Cfg file items
CasePrefix = LabelledEntry(FileSection.body_frame, "Case Prefix", 5)
FileNameStem = LabelledEntry(FileSection.body_frame, "Filename Stem", 15)
OutputPath = LabelledEntry(FileSection.body_frame, "Output Path", 40)
BrowseButton = Button(FileSection.body_frame, text = "Browse", command = output_path_browse)

CfgInputUpdater = UpdateSignal(case = CasePrefix, fstem = FileNameStem, outdir = OutputPath)
SimCfgDisplay = GeneratedEntry(FileSection.body_frame, "Top-level config file", 70, 
      lambda case,fstem,outdir: generate_fname(case=case,fstem=fstem,outdir=outdir,ext='py'), CfgInputUpdater)
AgtCfgDisplay = GeneratedEntry(FileSection.body_frame, "Agent config file", 70, 
      lambda case,fstem,outdir: generate_fname(case=case,fstem=fstem,outdir=outdir,ext='agent.lua'), CfgInputUpdater)
BinCfgDisplay = GeneratedEntry(FileSection.body_frame, "Binary config holder", 70, 
      lambda case,fstem,outdir: generate_fname(case=case,fstem=fstem,outdir=outdir,ext='bin'), CfgInputUpdater)

#Main control buttons
PreviewButton = Button(ControlFrame, text = "Preview Generated Files", command = preview_generated)
ViewButton = Button(ControlFrame, text = "View Config Files", command = view_config)
SaveButton = Button(ControlFrame, text = "Save", command = save_cfg)
LoadButton = Button(ControlFrame, text = "Load", command = load_cfg)
CloneButton = Button(ControlFrame, text = "Clone", command = clone_cfg)

#Packing simulation items
NThreadEntry.grid(0,0)
NBatchEntry.grid(0,2)
NRunEntry.grid(0,4)
NStepEntry.grid(0,6)

CommentBox.pack()

#Packing agent items
NAgentEntry.grid(0,0)
AgentTypeEntry.grid(0,2)
AgentTypeButton.grid(row=0, column = 4)

AgentAttributeLabel.grid(row=0,column=0)
AgentValueLabel.grid(row=0,column=1)
AgentGeneratorLabel.grid(row=0,column=2)

AgentStagedPops.pack(side = LEFT, fill = BOTH, expand = 1)
AddAgentPop.grid(sticky=W+E)
UpdateAgentPop.grid(sticky=W+E)
RenameAgentPop.grid(sticky=W+E)
RemoveAgentPop.grid(sticky=W+E)

#Packing info items
InfoAttributeLabel.grid(row=0,column =0)
InfoValueLabel.grid(row=0,column=1)

InfoGenType.grid(0,0)
InfoGenTypeButton.grid(row=0,column=2)

#Packing cfg file items
CasePrefix.grid(0,0)
FileNameStem.grid(0,1)
OutputPath.grid(1,0)
BrowseButton.grid(row=1,column=2)
SimCfgDisplay.grid(2,0, columnspan = 3)
AgtCfgDisplay.grid(3,0, columnspan = 3)
BinCfgDisplay.grid(4,0, columnspan = 3)

#Packing main controls
PreviewButton.grid(row=0, column = 0, columnspan=3, sticky=W+E)
ViewButton.grid(row=0, column = 3, columnspan = 3, sticky=W+E)
SaveButton.grid(row=1,column=0, columnspan = 2, sticky=W+E)
LoadButton.grid(row=1, column=2, columnspan = 2, sticky=W+E)
CloneButton.grid(row=1, column=4, columnspan = 2, sticky=W+E)


#Packing sections and frames
AgentStageListFrame.pack(side=LEFT, fill=BOTH, expand =1)
AgentStageControlFrame.pack(side=RIGHT, fill = Y, expand =1)

AgentFixedFrame.pack(side=TOP, fill = X, expand = 1)
AgentVarFrame.pack(side=TOP, fill=BOTH, expand = 1)
AgentStageFrame.pack(side=TOP, fill=BOTH, expand = 1)

InfoFixedFrame.pack(side=TOP, fill=X, expand=1)
InfoVarFrame.pack(side=TOP, fill=X, expand=1)

SimSection.pack()
CommentSection.pack()
AgentSection.pack()
InfoSection.pack()
FileSection.pack()

SimFrame.pack(side=TOP)
CommentFrame.pack(side=TOP)
AgentFrame.pack(side=TOP)
InfoFrame.pack(side=TOP)
FileFrame.pack(side=TOP)

ControlFrame.pack(side=BOTTOM)
CfgFrame.pack(side=TOP)

root.mainloop()
