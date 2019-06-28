/*************************************************************************
    > File Name       : gdb_stub.hpp
    > Author          : chmodJack
    > Mail            : 158754845@qq.com 
    > GitHub          : https://github.com/chmodJack 
    > Created Time    : Tue 11 Jun 2019 05:33:31 PM DST
    > Description     : 
*************************************************************************/
#ifndef __GDB_STUB_HPP__
#define __GDB_STUB_HPP__

class gdb_stub
{
public:
	//typedef SOCKET communication_handle;//win
	typedef int communication_handle;//linux
	communication_handle m_handle;
	int read_data(char* buffer,size_t size)
	{
		//linux
		return read(m_handle,buffer,size);
	}
	int write_data(char* buffer,size_t size)
	{
		//linux
		return write(m_handle,buffer,size);
	}
public:
	typedef int (gdb_stub::*handle_func)(char*);
	handle_func m_handle_func_arr[128]={0};
public:
	gdb_stub(void)
	{
		m_handle_func_arr['?']=&gdb_stub::func__;

#define GDB_STUB_GLOBAL_CMD_FUNCTION_ARRAY_INITIALIZE(ccc) m_handle_func_arr[#ccc[0]] = &gdb_stub::func_##ccc;

#include"rsp_commands_list.h"

#undef GDB_STUB_GLOBAL_CMD_FUNCTION_ARRAY_INITIALIZE

	}
	void set_communication_handle(communication_handle handle)
	{
		m_handle=handle;
	}
public:
	//just end with '#'
	int get_packet(char* buffer)
	{
		int end_flag=0;
		int end_count=0;

		for(int i=0;;i++)
		{
			int ret = read_data(&buffer[i],1);
			if(1 == ret)
			{
				end_count+=1;
				if(end_count > 4096)
				{
					printf("warnning: receive too long, maybe something error happend...\n");
					send_ack_error();
					return -1;
				}

				if(buffer[i] == '#')
				{
					end_flag=1;
					end_count=0;

					buffer[i+3]='\0';
				}
			}
			else if(ret == 0)
			{
				printf("read nothing\n");
				send_ack_error();
				return -1;
			}
			else if(ret == -1)
			{
				printf("read error\n");
				send_ack_error();
				return -1;
			}

			if(end_flag)
				if(end_count == 2)
					return 0;
		}
	}
	int process_cmd(char* buffer)
	{
		printf(">>>[%s]\n",buffer);
		
		//make pointer move to '$'
		buffer=move_to_dollar(buffer);
		if(nullptr == buffer)
		{
			printf("[%s]: move to dollar failed\n",buffer);
			send_ack_error();
			return -1;
		}

		if(0!=check_checksum(buffer))
		{
			printf("[%s]: checksum failed\n",buffer);
			send_ack_error();
			return -1;
		}

		if(0 != send_ack_ok())
		{
			printf("send ack error\n");
			return -1;
		}

		handle_func temp=m_handle_func_arr[buffer[1]];
		if(temp==nullptr)
		{
			printf("[%s]: not implement\n",buffer);
			reply("");
			return -1;
		}

		sharp_to_0(buffer);

		//skip '$' and type char
		int ret = (this->*temp)(buffer+2);

		if(ret != 0)
		{
			printf("[%s]: cmd process exit code %d\n",buffer,ret);
			return 1;
		}
		return 0;
	}
private:
	//send '+'
	int send_char(char chr)
	{
		switch(write_data(&chr,1))
		{
			case 1:
				{
					//printf("write ok!\n");
					return 0;
				}break;
			case 0:
				{
					printf("write error 0\n");
					return -1;
				}break;
			case -1:
				{
					printf("write error -1\n");
					return -1;
				}break;
		}
		return 0;
	}
	int send_ack_ok(void)
	{
		return send_char('+');
	}
	int send_ack_error(void)
	{
		return send_char('-');
	}
	void reply(const char* str)
	{
		size_t len=strlen(str);

		char arr[2]={0};
		sprintf(arr,"%02x",get_check_sum(str,len));

		write_data((char*)"+",1);
		write_data((char*)"$",1);
		write_data((char*)str,len);
		write_data((char*)"#",1);
		write_data(arr,2);

		printf("<<<[+$%s#%2s]\n",str,arr);
	}
private:
	uint8_t get_check_sum(const char* str, size_t count)
	{
		uint8_t sum=0;
		for(int i=0;i<count;i++)
		{
			//printf("%c",str[i]);
			sum+=str[i];
		}
		//printf("\n\n\n");
		return sum;
	}
	char* move_to_dollar(char* str)
	{
		for(int i=0;;i++)
		{
			if(*str == '$')
			{
				return str;
			}
			if(*str == '\0')
			{
				printf("packet error\n");
				return nullptr;
			}
			if(*str == '#')
			{
				printf("packet error\n");
				return nullptr;
			}
			if(i>4096)
			{
				printf("string too long, maybe something error happend.\n");
				return nullptr;
			}
			str+=1;
		}
	}
	char* move_to_sharp(char* str)
	{
		for(int i=0;;i++)
		{
			if(*str == '#')
			{
				return str;
			}
			if(*str == '\0')
			{
				printf("packet error\n");
				return nullptr;
			}
			if(i>4096)
			{
				printf("string too long, maybe something error happend.\n");
				return nullptr;
			}
			str+=1;
		}
	}
	int check_checksum(char* buffer)
	{
		//skip '$' and '#' and two check code
		uint8_t sum=get_check_sum(buffer+1,strlen(buffer+1)-3);
		char* check_code_str=move_to_sharp(buffer);
		if(nullptr == check_code_str)
		{
			return -1;
		}
		check_code_str+=1;
		uint8_t temp=0;
		sscanf(check_code_str,"%hhx",&temp);

		if(temp == sum)
		{
			//printf("sum: %x equal code: %x\n",sum,temp);
			return 0;
		}
		else
		{
			//printf("sum: %x not equal code: %x\n",sum,temp);
			return -1;
		}
	}
	void sharp_to_0(char* buffer)
	{
		for(int i=0;i<4096;i++)
		{
			if(buffer[i]=='#')
			{
				buffer[i]='\0';
				return;
			}
		}
	}
private:
	#include"rsp_commands_func.h"
};

#endif//__GDB_STUB_HPP__
