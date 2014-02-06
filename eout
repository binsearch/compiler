Program:
   Procedure: main

      Basic_Block 2
         Asgn:
            LHS (Name : c)
            RHS (Num : 10)
         Asgn:
            LHS (Name : f)
            RHS (Num : 3)
         Asgn:
            LHS (Name : d)
            RHS (Num : 6)
         Asgn:
            LHS (Name : e)
            RHS (Num : 8)
         Asgn:
            LHS (Name : a)
            RHS (
            Condition: LT
               LHS (Name : f)
               RHS (Name : d))
         Asgn:
            LHS (Name : b)
            RHS (
            Condition: LE
               LHS (Name : d)
               RHS (Name : c))
         If_Else statement:
            Condition: NE
               LHS (
            Condition: LE
               LHS (
            Condition: GT
               LHS (Name : c)
               RHS (Name : b))
               RHS (Name : a))
               RHS (Name : e)
            True Successor: 3
            False Successor: 4
      Basic_Block 3
         Asgn:
            LHS (Name : c)
            RHS (
            Condition: NE
               LHS (Name : e)
               RHS (Name : d))
         Asgn:
            LHS (Name : d)
            RHS (
            Condition: GT
               LHS (Name : f)
               RHS (Name : e))
         Goto statement:
            Successor: 7
      Basic_Block 4
         If_Else statement:
            Condition: NE
               LHS (
            Condition: NE
               LHS (
            Condition: LT
               LHS (Name : c)
               RHS (Name : e))
               RHS (
            Condition: GT
               LHS (Name : d)
               RHS (Num : 8)))
               RHS (Num : 0)
            True Successor: 5
            False Successor: 6
      Basic_Block 5
         Asgn:
            LHS (Name : a)
            RHS (
            Condition: EQ
               LHS (
            Condition: GE
               LHS (Name : d)
               RHS (Name : e))
               RHS (Name : f))
         Asgn:
            LHS (Name : b)
            RHS (
            Condition: GE
               LHS (Name : b)
               RHS (Name : a))
         Goto statement:
            Successor: 7
      Basic_Block 6
         Asgn:
            LHS (Name : a)
            RHS (Num : 4)
      Basic_Block 7
         Return <NOTHING>
