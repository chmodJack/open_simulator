#include<systemc>
#include<iostream>
using namespace std;
using namespace sc_core;
using namespace sc_dt;

#include"open_simulator.hpp"
#include"armv7m.hpp"

int sc_main(int argc,char* argv[])
{
	arm::armv7m::psr x;
	x.write_N(0);
	x.write_Z(1);
	x.write_C(1);
	x.write_V(1);
	x.write_Q(1);
	x.print();

	sc_uint<4> a;
	sc_uint<32> b;

	b=0xF2;
	a=b<<1;
	cout<<a;

    return 0;
}
