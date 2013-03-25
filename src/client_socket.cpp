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

#include <client_socket.h>

using namespace std;

Client_socket::Client_socket(std::string p_host, int p_port)
{
	port = p_port;
	host = p_host;
	sock = SOCKET_ERROR;
}

/*
	solve_DNS : Solves the provided DNS and returns it as a valid IP adress
*/
string Client_socket::solve_DNS(string domain)
{
	hostent * record = gethostbyname(domain.c_str());
	if(record == NULL)
	{
		cout << endl << "Host : " << domain << " is unaviable" << endl;
		return "";
	}

	in_addr * address = (in_addr * )record->h_addr;
	string ip_address = "";
	ip_address = inet_ntoa(* address);

	return ip_address;
}

/*
	connect_socket : connects the socket to the host:port specified to the constructor
*/
int Client_socket::connect_socket()
{
	SOCKADDR_IN sin;

	sock = socket(AF_INET, SOCK_STREAM, 0);

	// Configuration of the connexion
	string ip = solve_DNS(host);

	if(ip == "")
	{
		connected = false;
		return -1;
	}

	sin.sin_addr.s_addr = inet_addr(ip.c_str());
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);

	// If we can connect
	/*if(!ssl_socket)
	{*/
		//cout << "sock" << endl;
		if(connect(sock, (SOCKADDR*)&sin, sizeof(sin)) != SOCKET_ERROR)
		{
			//cout << "ok" << endl;
			connected = true;
			return 0;
		}
		else
		{
			perror("connect()");
			connected = false;
			return -1;
		}
		//cout << "ok" << endl;
	//}
	//else

	// Highly improbable case.
	return -1;

}

/*
	reconnect_socket : Closes the socket then reconnect it
*/
int Client_socket::reconnect_socket()
{
	close_socket();
	return connect_socket();
}

void Client_socket::close_socket()
{
	closesocket(sock);
}

/*
	bufferize : Checks if data is aviable in the socket and appends it
				to the buffer
*/
void Client_socket::bufferize()
{
		int recvd_len = 0;
		char incoming_buffer[4096];
		fd_set set;
    struct timeval delai;

    FD_ZERO(&set);
    FD_SET(sock, &set);
    delai.tv_sec = 0;
    delai.tv_usec = 100;

    string recu = "";

    int ret = select(FD_SETSIZE, &set, NULL, NULL, &delai);

    if(ret == 0)
    {

    }
    else if(ret == 1)
    {
			recvd_len = recv(sock, incoming_buffer, 4095, 0); // Si recv renvoie 0 la socket a été fermée
        if(recvd_len > 0)
        { // Des données ont été reçues
            incoming_buffer[recvd_len] = '\0';

			for(int i = 0; i < recvd_len; i++)
			{
				buff.push_back(incoming_buffer[i]);
			}

            recu = incoming_buffer;
			poke();
        }
        else if(recvd_len <= 0)
        {
			connected = false;
        }
    }
}

/*
	poke : updates the last_poke
*/

void Client_socket::poke()
{
	time(&last_poke);
}

/*
	ping : updates the last_ping
*/

void Client_socket::ping()
{
	time(&last_ping);
}

/*
	get_line : Returns the oldest line in the buffer and removes it from the
				buffer.
*/

string Client_socket::getline()
{
	string line = "";

	if(can_getline())
	{
		int n = 0;
		vector<char> tmp;
		while(buff.at(n) != '\n')
		{
			line += buff.at(n);
			n++;
		}

		for(unsigned int i = n+1; i < buff.size(); i++)
		{
			tmp.push_back(buff.at(i));
		}

		buff = tmp;
	}

	if(line.length() > 0 && line.at(line.length()-1) == '\r')
		line = line.substr(0, line.length()-1);

	return line;

}

/*
	getchars : Returns all the content of the buffer and cleans it.
*/

vector<char> Client_socket::getchars()
{
	vector<char> b = buff;
	buff.clear();
	return b;
}

/*
	getchars_as_string : Returns all the content of the buffer and cleans it.
*/

string Client_socket::getchars_as_string()
{
	string chars = "";
	for(unsigned int i = 0; i < buff.size(); i++)
	{
		chars += buff.at(i);
	}

	buff.clear();
	return chars;
}

/*
	getchar : Returns a char in the buffer.
*/

char Client_socket::getchar()
{
	char ch = buff.at(0);
	vector<char> tmp;

	for(unsigned int i = 1; i < buff.size(); i++)
	{
		tmp.push_back(buff.at(i));
	}

	buff = tmp;

	return ch;
}

/*
	can_getchar : returns true if a new char is aviable
*/

bool Client_socket::can_getchar()
{
	bufferize();
	if(buff.size() > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*
	write : write datas to socket
*/
int Client_socket::write(string data)
{
  if(!connected)
    return -1;

	unsigned int len = 0;

	len = send(sock, data.c_str(), strlen(data.c_str()), 0);

	if(len != data.length())
	{
		perror("send()");
		connected = false;
	}
	else
	{
		ping();
	}

	return len;
}

/*
	write : write datas to socket
*/
int Client_socket::write(char c)
{

  if(!connected)
    return -1;

	unsigned int len = 0;
	char l[1] = {c};
	len = send(sock, l, 1, 0);

	if(len != 1)
	{
		perror("send()");
		connected = false;
	}
	else
	{
		ping();
	}

	return len;
}

/*
	write_line : write a line
*/

int Client_socket::write_line(std::string data)
{
	return write(data + "\n");
}

/*
	is_connected : returns true if the socket is connected
*/
bool Client_socket::is_connected()
{
	return connected;
}

/*
	can_getline : returns true if a new line (matching) ".*\n.*" is aviable
*/

bool Client_socket::can_getline()
{
	bufferize();
	for(unsigned int i = 0; i < buff.size() ; i++)
	{
		if(buff.at(i) == '\n')
			return true;
	}

	return false;
}

/*
	can_getchars : returns true if a new chars are aviable
*/

bool Client_socket::can_getchars()
{
	bufferize();
	if(buff.size() == 0)
		return false;
	else
		return true;
}

/*
	get_last_poke : returns the timestamp of the last time data was read
*/

time_t Client_socket::get_last_poke()
{
	return last_poke;
}

/*
	get_last_ping : returns the timestamp of the last time data was sent
*/

time_t Client_socket::get_last_ping()
{
	return last_ping;
}

/*
	get_host : return the host of the socket
*/

string Client_socket::get_host()
{
	return host;
}

/*
	get_port : return the port of the socket
*/

int Client_socket::get_port()
{
	return port;
}

/*
	has_timed_out : return true if the socket has timedout
*/

bool Client_socket::has_timed_out(long sec)
{
	time_t now;
	time(&now);

	if(now - last_poke < sec)
	{
		return false;
	}

	return true;
}

/*
	has_timed_out : return true if the socket has pinged-out
					(if the number of seconds passed as a parammeter
					is under the number of seconds eleapsed since the
					last time data was written to the socket)
*/

bool Client_socket::has_pinged_out(long sec)
{
	time_t now;
	time(&now);

	if(now - last_ping < sec)
	{
		return false;
	}

	return true;
}

/*
	== : ==operator
*/

bool Client_socket::operator==(Client_socket const& b)
{
	if(host == b.host && port == b.port)
		return true;
	else
		return false;
}

/*
	!= : != operator
*/
bool Client_socket::operator!=(Client_socket const& b)
{
	if(host != b.host || port != b.port)
		return true;
	else
		return false;
}

Client_socket::~Client_socket()
{

}
