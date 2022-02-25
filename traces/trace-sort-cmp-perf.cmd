# Test performance of sort between q_sort and list_sort
option fail 0
option malloc 0
new
ih dolphin 1000000
it gerbil 1000000
reverse
time sort
reverse
time linux_sort
free
new
ih RAND 100000
time sort
free
new
ih RAND 100000
time linux_sort
free
new
ih RAND 100000
time sort
free
new
ih RAND 100000
time linux_sort
free
new
ih RAND 100000
time sort
free
new
ih RAND 100000
time linux_sort
free
new
ih RAND 100000
time sort
free
new
ih RAND 100000
time linux_sort
free
new
ih RAND 100000
time sort
free
new
ih RAND 100000
time linux_sort
free
