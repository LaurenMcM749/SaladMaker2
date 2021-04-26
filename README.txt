OS Project 3: SaladMaker
Lauren McMillen

The core of the saladmaker program is the synchronization of the chef and the saladmakers. The synchronization requires the use of six semaphores, one for each saladmaker and one for each vegetable. The saladmakers are named for the vegetables they require from the chef. For example, the saladmaker that need tomato and onion is opened by semaphore to. All semaphores are: to, op, tp, t, o, p. 

The chef determines which 2 vegetables it chooses by a mod 3 operation on a random number. Then, after it determines whether it will be giving to the to, op, or tp saladmaker, it unlocks that saladmaker. Then, it places each vegetable needed on the bench and unlocks the vegetable semaphores for it. After the specified saladmaker takes its vegetable off the workbench, it locks each vegetable semaphore, and when it does not need anymore vegetables, it tells the chef “I am done” by locking its own semaphore (to, tp, or op).

The generation of random numbers is based off the function printRandomNumbers, cited in the comments. After each salad is made, the shared variable c is incremented. All shared variables are kept in a struct called Veggies for ease of use within the program.

Finally, the saladmaker append its log statements to the file “outfile”. I overcome the Readers-Writer’s Problem by unlocking and locking the semaphore “out” around each fprintf which append to the outfile.
