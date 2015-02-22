all: sim

sim: sim.cpp
	g++ -pthread -lrt sim.cpp asim.cpp simevent.cpp facility.cpp calendar.cpp -o sim
	
clean:
	rm -f sim
