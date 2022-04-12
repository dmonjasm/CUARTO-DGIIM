set pagination off

b selfmod.cc:63
b selfmod.cc:64
b selfmod.cc:65

r < <(echo 2; echo 3)

disas f1
x/6bx f1
c
disas f1,+6
x/6bx f1

disas f2
x/6bx f2
c
disas f2,+6
x/6bx f2

q
