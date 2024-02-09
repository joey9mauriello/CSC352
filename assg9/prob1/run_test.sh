touch F
./mymake2 -f newmake A >myOut1
echo $? >>myOut1
touch F
./exMymake2 -f newmake A >exOut1
echo $? >>exOut1