import xml.sax
import copy

class SAXExceptionWithData(xml.sax.SAXException):
   def __init__(self, data):
      self.data = copy.deepcopy(data)

class SAXFirstNodeHanlder(xml.sax.handler.ContentHandler):
   def __init__(self, TopNodeName):
      self.TopNodeName = TopNodeName
      self.TopCounter = 0
      self.BottomNameList = []

   def startElement(self,name,attrs):
      if name == self.TopNodeName:
	 self.TopCounter += 1
      else:
	 if self.TopCounter == 1:
	    self.BottomNameList.append(str(name))
   
   def endElement(self,name):
      if name == self.TopNodeName and self.TopCounter == 1:
	 raise SAXExceptionWithData(self.BottomNameList)
