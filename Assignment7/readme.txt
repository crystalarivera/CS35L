Name: Crystal Rivera
UID: 904481468
Email: crystalarivera@g.ucla.edu
Professor: Eggert
TA: Mushi Zhou

time ./srt 1-test.ppm >1-test.ppm.tmp
real    0m48.015s
user    0m48.011s
sys     0m0.000s

time ./srt 2-test.ppm >2-test.ppm.tmp
real    0m24.154s
user    0m48.231s
sys     0m0.001s

time ./srt 4-test.ppm >4-test.ppm.tmp
real    0m12.315s
user    0m48.666s
sys     0m0.003s

time ./srt 8-test.ppm >8-test.ppm.tmp
real    0m6.516s
user    0m50.518s
sys     0m0.002s

Multithreading allows the program to render much faster. We see that the 
relationship between the real run time and the number of threads is
the original single-threaded run time divided by the number of threads.
There's a significant improvement in the execution time :D

There is some overhead for thread switching, but not as much overhead as
it would be for context switching, and this explains how the real time 
is slightly longer than anticipated for each thread we add on.

I had issues in deciding what variables to make global and working with the 
threads. Note that pthread_create only takes void functions, and I couldn't
pass in any variables. I also added a 3D array in order to further control
the order of the printing.