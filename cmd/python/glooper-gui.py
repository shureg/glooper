#!/usr/bin/env python

from Tkinter import *
import tkFileDialog, tkMessageBox
import os.path

root = Tk()
root.title("glooper launcher")

#Frame creation

ControlFrame = Frame(root)
ListSuperFrame = Frame(root)
InputListFrame = Frame(ListSuperFrame)
ExchangeFrame = Frame(ListSuperFrame)
OutputListFrame = Frame(ListSuperFrame)
ManageFrame = Frame(ControlFrame)
LaunchFrame = Frame(ControlFrame)

#List creation

scrollBar0 = Scrollbar(InputListFrame, orient=VERTICAL)
inputFileList = Listbox(InputListFrame, yscrollcommand=scrollBar0.set, selectmode=EXTENDED)
scrollBar0.configure(command=inputFileList.yview)

scrollBar1 = Scrollbar(OutputListFrame, orient=VERTICAL)
outputFileList = Listbox(OutputListFrame, yscrollcommand=scrollBar1.set, selectmode=EXTENDED)
scrollBar1.configure(command=outputFileList.yview)

#Callback creation

def browse_for_cfg_files():
   filenames = tkFileDialog.askopenfilenames(initialdir="multibatch",filetypes=[('sim config files','*.sim.cfg.py')])
   for fn in filenames:
      f = open(fn,'r')
      fl = f.readlines()
      f.close()
      cl = filter(lambda x: x[0:11] == "sim_comment", fl)
      exec cl[0]
      code = os.path.basename(fn).split('_',1)[0]
      cfg_des = "%s   -   %s   -   %s" % (code, sim_comment, fn )
      inputFileList.insert(END,cfg_des)

def save_staged():
   staged_list = reduce(lambda x,y: x+'\n'+y, outputFileList.get(0,END))
   sf = tkFileDialog.asksaveasfile(initialdir="multibatch/saved_config_sets", filetypes = [('config set files', '*.cfg.set')])
   sf.write(staged_list + '\n')
   sf.close()

def load_cfg():
   lf = tkFileDialog.askopenfile(initialdir="multibatch/saved_config_sets", filetypes=[('config set files', '*.cfg.set')])
   loaded_list = map(lambda x: x.rstrip('\n'), lf.readlines() )
   lf.close()
   clear_input()
   clear_staged()
   for cf in loaded_list:
      outputFileList.insert(END,cf)

def stage_file():
   cfg_ix = map(int, inputFileList.curselection())
   cfg_N = inputFileList.size()

   new_input_list = []
   new_output_list = []

   for i in xrange(0,cfg_N):
      if i in cfg_ix:
	 new_output_list.append(inputFileList.get(i))
      else:
	 new_input_list.append(inputFileList.get(i))

   for oc in new_output_list:
      outputFileList.insert(END,oc)

   inputFileList.delete(0,END)

   for ic in new_input_list:
      inputFileList.insert(END,ic)

def unstage_file():
   cfg_ix = map(int, outputFileList.curselection())
   cfg_N = outputFileList.size()

   new_input_list = []
   new_output_list = []

   for i in xrange(0,cfg_N):
      if i in cfg_ix:
	 new_input_list.append(outputFileList.get(i))
      else:
	 new_output_list.append(outputFileList.get(i))

   for ic in new_input_list:
      inputFileList.insert(END,ic)

   outputFileList.delete(0,END)

   for oc in new_output_list:
      outputFileList.insert(END,oc)

def clear_input():
   inputFileList.delete(0,END)

def clear_staged():
   outputFileList.delete(0,END)

def confirm_window(cfgs):
   confirmWindow = Toplevel()
   confirmWindow.title("Launch Simulation")
   msg = "Glooper simulation will be launched with the following configuration files\n" + reduce(lambda x,y: x+'\n'+y, cfgs)
   confirmMessage = Message(confirmWindow, text = msg, width = 1000)
   
   buttonFrame = Frame(confirmWindow)
   
   confirmMessage.pack(side=TOP, fill = BOTH, expand = 1)
   buttonFrame.pack(side=BOTTOM, fill = X)

   def run_launch_script():
      confirmWindow.destroy()
      root.destroy()
      os.system("MALLOC_CHECK_=0 nohup ./g_looper.py %s &" % " ".join(cfgs) )

   okButton = Button(buttonFrame, text="Launch", command = run_launch_script)
   cancelButton = Button(buttonFrame, text="Cancel", command = confirmWindow.destroy)

   okButton.pack(side=LEFT)
   cancelButton.pack(side=RIGHT)

def launch_sim():
   cfgs = map(lambda x: x.split("-")[-1].lstrip(), outputFileList.get(0,END))
   confirm_window(cfgs)

#Button creation

configButton = Button(ManageFrame, text="Launch Config Manager", command = lambda : os.system("python glooper-cfg-manage.py &"))
browseButton = Button(ManageFrame, text="Browse For Config Files", command = browse_for_cfg_files)
saveButton = Button(ManageFrame, text="Save Currently Staged Set", command = save_staged)
loadButton = Button(ManageFrame, text="Load Configuration Set", command = load_cfg)

stageButton = Button(ExchangeFrame, text = "Stage Selected", command = stage_file)
unstageButton = Button(ExchangeFrame, text = "Unstage Selected", command = unstage_file)
clearInputButton = Button(ExchangeFrame, text = "Clear Input Files", command = clear_input)
clearStagedButton = Button(ExchangeFrame, text = "Clear Staged Files", command = clear_staged)

simulationLaunchButton = Button(LaunchFrame, text = "Launch Simulation", command = launch_sim)

#Object packing

inputFileList.pack(side=LEFT,fill=BOTH,expand=1)
scrollBar0.pack(side=RIGHT,fill=Y)

outputFileList.pack(side=LEFT,fill=BOTH,expand=1)
scrollBar1.pack(side=RIGHT,fill=Y)

stageButton.pack(side=LEFT)
unstageButton.pack(side=LEFT)
clearInputButton.pack(side=LEFT)
clearStagedButton.pack(side=LEFT)

configButton.grid(sticky = W+E)
browseButton.grid(sticky = W+E)
saveButton.grid(sticky = W+E)
loadButton.grid(sticky = W+E)
simulationLaunchButton.grid(sticky = W+E)

InputListFrame.pack(side=TOP,fill=BOTH,expand=1)
ExchangeFrame.pack(fill=X)
OutputListFrame.pack(side=BOTTOM,fill=BOTH,expand=1)

ManageFrame.pack(side=TOP, fill = Y)
LaunchFrame.pack(side=BOTTOM)

ListSuperFrame.pack(side=LEFT,fill=BOTH,expand=1)
ControlFrame.pack(side=RIGHT,fill=Y)

root.mainloop()
