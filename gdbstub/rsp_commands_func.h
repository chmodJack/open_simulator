int func__(char* cmd)
{
	//stand for ?
	//`?'
	//	Indicate the reason the target halted. The reply is the same as for step and continue.
	//	Reply: See section D.3 Stop Reply Packets, for the reply specifications.
	reply("S05");
	return 0;
}

//寄存器操作
int func_g(char* cmd)
{
	//Read Registers
	//分别获取16个寄存器的值，以16进制字符格式依次排好发过去
	//例如
	//reply("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000ffffffff00000000");
	reply("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000ffffffff00000000");
	return 0;
}
int func_G(char* cmd)
{
	//Write Registers
	//写寄存器
	return 0;
}
int func_p(char* cmd)
{
	//p19
	//读19号寄存器
	return 0;
}
int func_P(char* cmd)
{
	return 0;
}

//内存操作
int func_m(char* cmd)
{
	//读内存
	//`m addr,length'
	//Read length bytes of memory starting at address addr. Note that addr may not be aligned to any particular boundary.
	//	The stub need not use any particular size or alignment when gathering data from memory for the response; even if addr is word-aligned and length is a multiple of the word size, the stub is free to use byte accesses, or not. For this reason, this packet may not be suitable for accessing memory-mapped I/O devices.

	//	Reply:

	//	`XX...'
	//	Memory contents; each byte is transmitted as a two-digit hexadecimal number. The reply may contain fewer bytes than requested if the server was able to read only part of the region of memory.
	//	`E NN'
	//	NN is errno

	//读内存，例如：
	//m0,4
	//起始地址是0，读4字节
	//注意：返回数据可以少于指定长度
	reply("0000FFFF");
	return 0;
}
int func_M(char* cmd)
{
	//写内存
	return 0;
}

//控制操作
int func_c(char* cmd)
{
	//`c [addr]'
	//	Continue. addr is address to resume. If addr is omitted, resume at current address.
	//	Reply: See section D.3 Stop Reply Packets, for the reply specifications.
	return 0;
}
int func_C(char* cmd)
{
	//`C sig[;addr]'
	//	Continue with signal sig (hex signal number). If `;addr' is omitted, resume at same address.
	//	Reply: See section D.3 Stop Reply Packets, for the reply specifications.
	return 0;
}

//断点操作
//`z type,addr,length'
//`Z type,addr,length'
//Insert (`Z') or remove (`z') a type breakpoint or watchpoint starting at address address and covering the next length bytes.
//Each breakpoint and watchpoint packet type is documented separately.
//
//Implementation notes: A remote target shall return an empty string for an unrecognized breakpoint or watchpoint packet type. A remote target shall support either both or neither of a given `Ztype...' and `ztype...' packet pair. To avoid potential problems with duplicate packets, the operations should be implemented in an idempotent way.
//
//`z0,addr,length'
//`Z0,addr,length'
//Insert (`Z0') or remove (`z0') a memory breakpoint at address addr of size length.
//A memory breakpoint is implemented by replacing the instruction at addr with a software breakpoint or trap instruction. The length is used by targets that indicates the size of the breakpoint (in bytes) that should be inserted (e.g., the ARM and MIPS can insert either a 2 or 4 byte breakpoint).
//
//Implementation note: It is possible for a target to copy or move code that contains memory breakpoints (e.g., when implementing overlays). The behavior of this packet, in the presence of such a target, is not defined.
//
//Reply:
//
//`OK'
//success
//`'
//not supported
//`E NN'
//for an error
//`z1,addr,length'
//`Z1,addr,length'
//Insert (`Z1') or remove (`z1') a hardware breakpoint at address addr of size length.
//A hardware breakpoint is implemented using a mechanism that is not dependant on being able to modify the target's memory.
//
//Implementation note: A hardware breakpoint is not affected by code movement.
//
//Reply:
//
//`OK'
//success
//`'
//not supported
//`E NN'
//for an error
//`z2,addr,length'
//`Z2,addr,length'
//Insert (`Z2') or remove (`z2') a write watchpoint.
//Reply:
//
//`OK'
//success
//`'
//not supported
//`E NN'
//for an error
//`z3,addr,length'
//`Z3,addr,length'
//Insert (`Z3') or remove (`z3') a read watchpoint.
//Reply:
//
//`OK'
//success
//`'
//not supported
//`E NN'
//for an error
//`z4,addr,length'
//`Z4,addr,length'
//Insert (`Z4') or remove (`z4') an access watchpoint.
//Reply:
//
//`OK'
//success
//`'
//not supported
//`E NN'
//for an error
int func_z(char* cmd)
{
	//插入断点
	return 0;
}
int func_Z(char* cmd)
{
	//删除断点
	return 0;
}

//查询操作
int func_q(char* ptr)
{
	if (0 == strncmp(ptr, "TStatus", 7))
	{
		reply("");
		return 0;
	}
	if (0 == strncmp(ptr, "Symbol", 6))
	{
		reply("OK");
		return 0;
	}
	if(0 == strncmp(ptr, "Xfer", 4))
	{
#define qXfer_file(ffff) if(0 == strncmp(ptr,"Xfer:features:read:"#ffff,strlen("Xfer:features:read:"#ffff))) \
		{ \
			FILE* f=fopen(#ffff,"r"); \
			if(f==nullptr) \
			{ \
				printf("open xml file: %s, error\n",#ffff); \
				return -1; \
			} \
			char arr[4096]={0}; \
			arr[0]='l'; \
			int count=fread(arr+1,1,4095,f); \
			reply(arr); \
			fclose(f); \
			return 0; \
		}

		qXfer_file(target.xml);
		qXfer_file(arm-m-profile.xml);
		qXfer_file(arm-vfpv3.xml);

		return 0;
#undef qXfer_file
	}
	if (0 == strncmp(ptr, "Supported", 9))
	{
		reply("PacketSize=1fff;multiprocess+;xmlRegisters+;qXfer:features:read+");
		return 0;
	}
	if (0 == strncmp(ptr, "Offset", 6))
	{
		//`qOffsets'
		//Get section offsets that the target used when relocating the downloaded image.
		//	Reply:

		//	`Text=xxx;Data=yyy[;Bss=zzz]'
		//	Relocate the Text section by xxx from its original address. Relocate the Data section by yyy from its original address. If the object file format provides segment information (e.g. ELF `PT_LOAD' program headers), GDB will relocate entire segments by the supplied offsets.
		//	Note: while a Bss offset may be included in the response, GDB ignores this and instead applies the Data offset to the Bss section.

		//	`TextSeg=xxx[;DataSeg=yyy]'
		//	Relocate the first segment of the object file, which conventionally contains program code, to a starting address of xxx. If `DataSeg' is specified, relocate the second segment, which conventionally contains modifiable data, to a starting address of yyy. GDB will report an error if the object file does not contain segment information, or does not contain at least as many segments as mentioned in the reply. Extra segments are kept at fixed offsets relative to the last relocated segment.
		reply("");
		return 0;
	}
	if(0 == strncmp(ptr, "fThreadInfo", 11))
	{
		//		`qfThreadInfo'
		//			`qsThreadInfo'
		//			Obtain a list of all active thread ids from the target (OS). Since there may be too many active threads to fit into one reply packet, this query works iteratively: it may require more than one query/reply sequence to obtain the entire list of threads. The first query of the sequence will be the `qfThreadInfo' query; subsequent queries in the sequence will be the `qsThreadInfo' query.
		//			NOTE: This packet replaces the `qL' query (see below).
		//
		//			Reply:
		//
		//			`m id'
		//			A single thread id
		//			`m id,id...'
		//			a comma-separated list of thread ids
		//			`l'
		//			(lower case letter `L') denotes end of list.
		//			In response to each query, the target will reply with a list of one or more thread ids, in big-endian unsigned hex, separated by commas. GDB will respond to each reply with a request for more thread ids (using the `qs' form of the query), until the target responds with `l' (lower-case el, for last).
		reply("T1");
		return 0;
	}
	if(0 == strncmp(ptr, "C", 1))
	{
		//Return the current thread id
		//Reply:
		//
		//		`QC pid'
		//			Where pid is an unsigned hexadecimal process id.
		//			`(anything else)'
		//			Any other reply implies the old pid.
		reply("QC -1");
		return 0;
	}

	reply("");
	return 0;
}
int func_D(char* cmd)
{
	return 0;
}
int func_X(char* cmd)
{
	return 0;
}
int func_s(char* cmd)
{
	return 0;
}
int func_H(char* ptr)
{
	reply("OK");
	return 0;
#if 0
	char type = *ptr++;
	int thread = strtoull(ptr,(char **)&ptr,16);
	if (thread <= 0)
	{
		reply("OK");
		return 0;
	}
	else if (thread >= 1)
	{
		reply("E22");
		return 0;
	}

	switch (type) 
	{
		case 'c':
		case 'g':
			reply("OK");
			return 0;
		default:
			reply("E22");
			return 0;
	}
	#endif
}
int func_T(char* cmd)
{
	return 0;
}
int func_b(char* cmd)
{
	return 0;
}
int func_k(char* cmd)
{
	return 0;
}
int func_v(char* cmd)
{
	return 0;
}
int func_u(char* cmd)
{
	return 0;
}
