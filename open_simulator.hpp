#pragma once

#include<systemc>
using namespace sc_core;
using namespace sc_dt;

namespace open_simulator
{
	template<uint64_t BITWIDTH>
	class reg
	{
	public:
		//data buffer, use systemc builtin data type
		sc_uint<BITWIDTH> m_data;
	public:
		//return signle bit ref, can read or write
		inline sc_uint_bitref& operator[](const int64_t& index)
		{
			return m_data[index];
		}
		//write to register
		inline void write(const sc_uint<BITWIDTH>& dat)
		{
			m_data=dat;
		}
		//read to register
		inline const sc_uint<BITWIDTH>& read(void)const
		{
			return m_data;
		}
	public:
		//get bit width
		inline const uint64_t get_width(void)const
		{
			return BITWIDTH;
		}
		//print every bit in register
		void print(void)
		{
			for(uint64_t i=0;i<get_width();i++)
				printf("%c",(m_data[get_width()-i-1])?'1':'0');
			printf("\n");
		}
	};
}
using namespace open_simulator;
