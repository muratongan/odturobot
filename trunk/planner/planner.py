#!/usr/bin/python

from xml.dom.minidom import parse


class Planner:
    def __init__(self, filename):
        "Reads plan file and parses it, initializes planner object"
        self.behaviours = []
        self.states = []
        self.start = None
        dom = parse(filename)
        # Get Behaviour List:
        behlist = dom.getElementsByTagName("behaviours")[0].getElementsByTagName("behaviour")
        for beh in behlist:
            self.behaviours.append(beh.getAttribute("name"))
        # Get State List:
        statelist = dom.getElementsByTagName("states")[0].getElementsByTagName("state")
        for state in statelist:
            self.states.append(State(state))
        # Get Start State:
        self.start = dom.documentElement.getAttribute("start")
    
    def printPlan(self):
        "Prints the details of the plan"
        print "Behaviours:", self.behaviours
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
        self.changeState(self.start)
    
    def changeState(self, statename):
        "Changes the active state of the machine"
        state = self.getState(statename)
        state.printState()


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
            self.behaviours.append(beh.getAttribute("name"))
        # Get Transition List:
        translist = statenode.getElementsByTagName("transitions")[0].getElementsByTagName("transition")
        for trans in translist:
            self.transitions.append({"expression":trans.getAttribute("expression"), "nextstate":trans.getAttribute("nextstate")})
   
    def printState(self, indent=0):
        print indent * " " + "State Name:", self.name
        print indent * " " + "Behaviours:", self.behaviours
        print indent * " " + "Transitions:"
        for trans in self.transitions:
            print (indent + 4) * " ", trans


if __name__ == "__main__":
    plan = Planner("plan.xml")
    #plan.printPlan()
    plan.run()
