#include "sim.h"

simevent::simevent() {
	status = 0;
}

void simevent::set() {
	status = 1;
}

void simevent::clear() {
	status = 0;
}

void simevent::wait() {
	while(status != 1) {}
}