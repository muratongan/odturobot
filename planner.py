#!/usr/bin/python

import os
import subprocess
import select
from xml.dom.minidom import parse


class Planner:
    def __init__(self, filename):
        "Reads plan file and parses it, initializes planner object"
        self.behaviours = []
        self.states = []
        self.state = None
        dom = parse(filename)
        # Get State List:
        statelist = dom.getElementsByTagName("states")[0].getElementsByTagName("state")
        for state in statelist:
            self.states.append(State(state))
        # Get Start State:
        self.state = dom.documentElement.getAttribute("start")
    
    def printPlan(self):
        "Prints the details of the plan"
        print "Behaviours:"
        for beh in self.behaviours:
            print " " * 4 + beh
        print "States:"
        for state in self.states:
            state.printState(4)
    
    def getState(self, statename):
        "Returns a state object by state name"
        for state in self.states:
            if state.name == statename:
                return state
        raise Exception, "There is no start state like: '%s'" % statename
    
    def run(self):
        "Starts the machine"
        self.changeState(self.state)
    
    def changeState(self, statename):
        "Changes the active state of the machine"
        self.active = []
        self.signals = {}
        self.state = self.getState(statename)
        for beh in self.state.behaviours:
            command = ["./behaviours/" + beh["name"]]
            if beh.has_key("arguments"):
                command.append(beh["arguments"])
            print "command", command
            p = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE)
            self.active.append({"name":beh["name"], "popen":p, "stdin":p.stdin, "stdout":p.stdout, "pid":p.pid})
        self.state.printState()
        self.listen()
    
    def listen(self):
        while True:
            inputlist = []
            # Control active behaviours:
            for beh in self.active:
                inputlist.append(beh["stdout"])
            # If there is a signal, get it:
            ins, outs, errs = select.select(inputlist, [], [])
            for insock in ins:
                a = insock.read()
                name = ""
                for beh in self.active:
                    if beh["stdout"] == insock:
                        name = beh["name"]
                        if not beh["popen"].poll():
                            self.active.remove(beh)
                self.signals[name] = a
		print "Signal %s: %s" % (name, a)
            # Control Transitions:
            next = self.nextTransition()
            if next:
                for beh in self.active:
                    os.kill(beh["pid"], 3)
                self.changeState(next)
            # If there is no active behaviour, end function:
            if not self.active:
                return
            
    def nextTransition(self):
        for trans in self.state.transitions:
            key, value = trans["expression"].split("=")
            if self.signals.has_key(key):
                if self.signals[key] == value:
                    return trans["nextstate"]
        return None


class State:
    def __init__(self, statenode):
        "Parses xml state node and initializes state"
        self.behaviours = []
        self.transitions = []
        # Get Name:
        self.name = statenode.getAttribute("name")
        # Get Behaviour List:
        behlist = statenode.getElementsByTagName("behaviours")[0].getElementsByTagName("behaviour")
        for beh in behlist:
            behdict = {}
            behdict["name"] = beh.getAttribute("name")
            if beh.hasAttribute("arguments"):
                behdict["arguments"] = beh.getAttribute("arguments")
            self.behaviours.append(behdict)
        # Get Transition List:
        translist = statenode.getElementsByTagName("transitions")[0].getElementsByTagName("transition")
        for trans in translist:
            self.transitions.append({"expression":trans.getAttribute("expression"), "nextstate":trans.getAttribute("nextstate")})
   
    def printState(self, indent=0):
        print indent * " " + self.name
        print (indent + 4) * " " + "Behaviours:"
        for beh in self.behaviours:
            print (indent + 8) * " " + beh["name"]
        print (indent + 4) * " " + "Transitions:"
        for trans in self.transitions:
            print (indent + 8) * " ", trans
