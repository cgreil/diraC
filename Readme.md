# About this project
diraC is a quantum computing simulator completely written in the C programming language and implemented from first principles of quantum mechanics. It features statevector based manipulation of quantum registers using qubit gates. However qubit gate actions are not created using a circuit definition, but are instead called as functions onto a created qubit register directly. 

# Testing 
Testcases are implemented using the **Check** Library


# Debugging
Use 
``export CK_FORK=no`` to prevent multithreaded test execution which would diminish debugging possibilities.

