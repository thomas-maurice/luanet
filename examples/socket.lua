#!/usr/bin/lua

--[[
--            test.lua
--              ----
-- A simple Lua script to make an
-- overview of the little 'luanet'
-- module, here we connect to a IRC
-- server, and keep connected to it
--    by answering it's pings
--]]

-- Author: Thomas Maurice

-- Loads the library
require 'luanet'

-- We create a new socket with the host and the port
s = luanet.newSocket("irc.epiknet.fr", 6667)
-- Let's connect it
s:connectSocket()

-- Use luanet.socket.writeLine(string) to send
-- all our commands
s:writeLine("NICK LuaBotV1")
s:writeLine("USER luabot - - :A bot written in Lua O_o")

-- While we are connected
while(s:isConnected() == true) do
  -- If a new line is availiable in the buffer
  if s:canGetLine() then
    -- We read it and print it
    line = s:getLine()
    print(line)
    -- Finally we replace all the PING in the
    -- PING :somestring
    -- requests by PONG to answer the server
    if string.find(line, "PING") then
       s:writeLine(string.gsub(line, "PING", "PONG"))
    end
  end
end
