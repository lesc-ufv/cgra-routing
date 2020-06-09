# CGRA Routing FSM

## Pseudo-Code

    while(NextEdge!=0,0->0,0):
        Source, Destination = GetNextEdge()
        modified = false
        while(Source!=Destination):
            while(AdvanceXisPossible(Source, Destination)):
                AdvanceX(Source)
                modified = true
            while(AdvanceYisPossible(Source, Destination)):
                AdvanceY(Source)
                modified = True
            if(modified = False):
                EraseEdge()
                Break
