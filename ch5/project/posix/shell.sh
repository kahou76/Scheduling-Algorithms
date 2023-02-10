rm -rf *.o
rm -rf fcfs
rm -rf sjf
rm -rf rr
rm -rf priority
rm -rf priority_rr

make fcfs
make sjf
make priority
make rr
make priority_rr

./fcfs schedule.txt
./sjf schedule.txt
./priority schedule.txt
./rr schedule.txt
./priority_rr schedule.txt