#!/usr/bin/lua

--[[
--               testirc.lua
--                 -------
-- Some example script to demonstrate the
-- capacities of the irc part of the luanet
-- module. We just connect to a server,
-- respond to it's pings and output all the
-- raw text, commands and so on that it
-- sends us
--]]

-- Author: Thomas Maurice

-- Let's load the 'luanet' library
require "luanet.irc"

-- Ping-handling function
function on_ping(server, str)
   server:send(string.gsub(str, "PING", "PONG"))
end

-- Connect-handling function
function on_connect(server)
	print("Connected, joining channels")
	server:join("#testthomas")
end

-- Privmsg-handling function
function on_privmsg(server, author, target, message)
	nick,user,host = luanet.irc.splituser(author)
	
	print("Message : " .. luanet.irc.splituser(author) .. " to " .. target .. " : " .. message)
	if string.find(message, "!echo (.+)") == 1 then
		_, _, echo = string.find(message, "!echo (.+)")
		if echo ~= nil then
			server:privmsg(target, echo)
		end
	elseif string.find(message, "!kick") == 1 then
		server:kick(chan, nick, "Hahar")
	end
end

-- Kick-handling function
function on_kick(server, author, chan, target, reason)
	-- Autorejoin after a kick
	if server.nick == target then
		server:join(chan)
	end
end

-- Notice-handling function
function on_notice(server, author, target, message)
	print("Notice  : " .. author .. " to " .. target .. " : " .. message)
end

-- Join-handling function
function on_join(server, author, target)
	print("Join     : " .. author .. " joined " .. target)
end

-- Part-handling function
function on_part(server, author, target, message)
	print("Part     : " .. author .. " parted " .. target .. " (" .. message .. ")")
end

-- Quit-handling function
function on_quit(server, author, message)
	print("Part     : " .. author .. " quitted : " .. message)
end

-- Raw-handling function
function on_raw(server, str)
	print(str)
end

-- We create a new server with the appropriate
-- host and port.
serv = luanet.irc.newServer("localhost", 6667)

-- Some parameters
serv.nick = "LuaBotTest"
serv.waittime = 10

-- We add the handling functions
serv.on_ping = on_ping
serv.on_raw = on_raw
serv.on_connect = on_connect
serv.on_privmsg = on_privmsg
serv.on_notice = on_notice
serv.on_join = on_join
serv.on_part = on_part
serv.on_quit = on_quit
serv.on_kick = on_kick

print("Connecting to the server")

-- We connect
serv:connectServer()

-- We send the ident commands
serv:identify()

-- We process until we got disconnected
while serv:isConnected() do
	serv:proceed() -- The function used to parse server output
end
