Run make in the A2 directory. This will compile heap.c, preorder.c, postorder.c, inorder.c, and calculate.c into object files inside the obj directory, and then link them into executables in the bin directory.

Then run:
./bin/preorder "expression"
./bin/postorder "expression" 
./bin/inorder "expression"
./bin/calculate "expression"
./bin/heap

Issues or Surprises:
The expression tree parsing was hard to implement correctly.

##Disclosure
I searched up the process of parsing a tree