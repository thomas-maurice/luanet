/*

Copyright (C) 2013 Thomas MAURICE

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#ifndef CLIENT_SOCKET
#define CLIENT_SOCKET

#include "general_net.h"

#include <cstring>
#include <iostream>
#include <string>
#include <ctime>
#include <vector>

class Client_socket
{
	public:
		// Class Constructor
		Client_socket(std::string p_host, int p_port);
		~Client_socket();
		
		// I/O Functions
		void close_socket();
		void bufferize();
		std::string solve_DNS(std::string domain);
		
		// Actions
		int connect_socket();
		int reconnect_socket();
		
		bool can_getline();
		bool can_getchars();
		bool can_getchar();
		
		std::string getline();
		std::vector<char> getchars();
		std::string getchars_as_string();
		char getchar();
		
		int write(std::string data);
		int write(char c);
		int write_line(std::string data);
		bool is_connected();
		
		// Time management functions
		// Last recieved datas
			void poke();
			time_t get_last_poke();
			bool has_timed_out(long sec);
		// Last sent datas
			void ping();
			time_t get_last_ping();
			bool has_pinged_out(long sec);
		
		//Data accessing functions
		std::string get_host();
		int get_port();
		
		// Comparison operators
		bool operator==(Client_socket const& b);
		bool operator!=(Client_socket const& b);
		
	protected:
		SOCKET sock;
		
		bool connected;
		std::string host;
		int port;

		std::vector<char> buff;
		time_t last_poke;
		time_t last_ping;
		
	private:
		
};

#endif
