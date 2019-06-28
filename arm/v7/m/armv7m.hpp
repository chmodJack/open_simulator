#pragma once

#include"open_simulator.hpp"

namespace arm
{

namespace armv7m
{
	class sp;
	class psr;
	class core;

	class reg_sp
	{
	public:
		reg<32> msp;
		reg<32> psp;
	public:
		sp(reg<32>& control):reg_control(control){}
		inline void write(const sc_uint<32>& dat)
		{
			if(0 == reg_control[1])
			{
				msp.write(dat);
			}
			else
			{
				psp.write(dat);
			}
		}
		inline const sc_uint<32>& read(void)
		{
			if(0 == reg_control[1])
			{
				return msp.read();
			}
			else
			{
				return psp.read();
			}
		}
	public:
		reg<32>& reg_control;
	};

	//program state register
	class psr:public reg<32>
	{
	public:
#if 0
		//apsr
		inline const sc_uint<32> read_apsr(void)const
		{
			return m_data & (0b11111 << 27);
		}
		inline void write_apsr(const sc_uint<32>& dat)
		{
			m_data = m_data & (~(0b11111 << 27));
			dat = (dat & (0b11111 << 27));

			m_data = m_data | dat;
		}
#endif
		//TODO epsr
		inline const sc_uint<32> read_epsr(void)const
		{
			return m_data & (0b11111 << 27);
		}
		inline void write_epsr(const sc_uint<32>& dat)
		{
		}

		//read single bit
#define OPEN_SIMULATOR_XPSR_BIT_DEFINE(name,index) \
		inline const sc_uint_bitref& read_##name(void){return m_data[index];}\
		inline void write_##name(const sc_uint<1>& dat){m_data[index]=dat;}
		OPEN_SIMULATOR_XPSR_BIT_DEFINE(N,31);
		OPEN_SIMULATOR_XPSR_BIT_DEFINE(Z,30);
		OPEN_SIMULATOR_XPSR_BIT_DEFINE(C,29);
		OPEN_SIMULATOR_XPSR_BIT_DEFINE(V,28);
		OPEN_SIMULATOR_XPSR_BIT_DEFINE(Q,27);
		OPEN_SIMULATOR_XPSR_BIT_DEFINE(T,24);
#undef OPEN_SIMULATOR_XPSR_BIT_DEFINE
		inline const sc_uint<8> read_isr(void)
		{
			sc_uint<8> temp;
			temp=m_data;
			return temp;
		}
		inline void write_isr(const sc_uint<8>& dat)
		{
			m_data=dat;
		}
	};

	//core registers
	class core:public sc_module
	{
	public:
		//special registers
		reg<32> r16;
		reg<32>& psr=r16;

		reg<32> primask;
		reg<32> faultmask;
		reg<32> basepri;
		reg<32> control;


		//common registers
		reg<32> r0;
		reg<32> r1;
		reg<32> r2;
		reg<32> r3;
		reg<32> r4;
		reg<32> r5;
		reg<32> r6;
		reg<32> r7;
		reg<32> r8;
		reg<32> r9;
		reg<32> r10;
		reg<32> r11;
		reg<32> r12;
		reg_sp  r13{control};
		reg<32> r14;
		reg<32> r15;
		//alias name
		reg<32>& a1=r0;
		reg<32>& a2=r1;
		reg<32>& a3=r2;
		reg<32>& a4=r3;
		reg<32>& v1=r4;
		reg<32>& v2=r5;
		reg<32>& v3=r6;
		reg<32>& v4=r7;
		reg<32>& v5=r8;
		reg<32>& v6=r9;
		reg<32>& v7=r10;
		reg<32>& v8=r11;
		reg<32>& sb=r9;
		reg<32>& sl=r10;
		reg<32>& fp=r11;
		reg<32>& ip=r12;
		reg_sp   sp=r13;
		reg<32>& lr=r14;
		reg<32>& pc=r15;
	};

	//vector float point processer
	class vfp:public sc_module
	{
	public:
		reg<64> d0;
		reg<64> d1;
		reg<64> d2;
		reg<64> d3;
		reg<64> d4;
		reg<64> d5;
		reg<64> d6;
		reg<64> d7;
		reg<64> d8;
		reg<64> d9;
		reg<64> d10;
		reg<64> d11;
		reg<64> d12;
		reg<64> d13;
		reg<64> d14;
		reg<64> d15;
		reg<64> d16;
		reg<64> d17;
		reg<64> d18;
		reg<64> d19;
		reg<64> d20;
		reg<64> d21;
		reg<64> d22;
		reg<64> d23;
		reg<64> d24;
		reg<64> d25;
		reg<64> d26;
		reg<64> d27;
		reg<64> d28;
		reg<64> d29;
		reg<64> d30;
		reg<64> d31;
		reg<32> fpsr;
	};

	//nested vector interrupt controller
	class nvic:public sc_module
	{
	public:
	};

	//wake interrupt controller
	class wic:public sc_module
	{
	public:
	};

	//embedden trace macrocell
	class etm:public sc_module
	{
	public:
	};

	//debug access port
	class dap:public sc_module
	{
	public:
	};

	//memory protection unit
	class mpu:public sc_module
	{
	public:
	};

	//float point uint
	class fpu:public sc_module
	{
	public:
	};

	//flash patch
	class fp:public sc_module
	{
	public:
	};

	//data watchpoints
	class dw:public sc_module
	{
	public:
	};

	//system control block
	class scb:public sc_module
	{
	public:
	};

	//instruction decoder
	class thumb2_decoder
	{
	public:
	};

	class cortex_m3
	{
	public:
		core  m_core;
		vfpv3 m_cp10;
		vfpv3 m_cp11;
		nvic  m_nvic;
		scb   m_scb;
		thumb2_decoder m_decoder;
	};

}

}

