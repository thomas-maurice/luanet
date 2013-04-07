-- luanet.irc
-- A simple wrapper for an IRC server
--
-- Author : Thomas Maurice

require "luanet.socket"
require "luanet.sleep"

-- luanet.irc
luanet.irc = {}
luanet.irc.__index = luanet.irc

-- luanet.irc.server
-- For the nice OO part
luanet.irc.server = {}
luanet.irc.server.__index = luanet.irc.server

-- luanet.irc functions
-- a new server
function luanet.irc.newServer(host, port)
  local t = {}
	
  t.socket = luanet.newSocket(host, port)
  t.nick = "LuaBot"
  t.user = "luabot"
  t.realname = "A bot written in Lua :)"
  
  t.buffer = ""
  
  t.waittime = 100 -- milliseconds
  
  t.connected = false
	
  setmetatable(t,luanet.irc.server)
  return t
end

-- decompose a nick!user~host into 3 variables
function luanet.irc.splituser(u)
  _,_, nick, user, host = string.find(u, "(%w*)!(%w*)@(.*)")
  return nick, user, host
end

function luanet.irc.splitUser(u)
  _,_, nick, user, host = string.find(u, "(%w*)!(%w*)@(.*)")
  return nick, user, host
end

-- luanet.irc.server
-- connects the socket
function luanet.irc.server:connectServer()
  self.socket:connectSocket()
end

-- are we connected
function luanet.irc.server:isConnected()
  if self.socket ~= nil then return self.socket:isConnected()
  else return false
  end
end

-- get a line
function luanet.irc.server:getLine()
	if self.socket == nil then return nil end
  if self.socket:canGetLine() then
    return self.socket:getLine()
  else
    return nil
  end
end

function luanet.irc.server:getBufferedLine()
  if string.find(self.buffer, "\n") == nil then
    local res = self.buffer
    self.buffer = ""
    return res
  else
    local _,_,p= string.find(self.buffer, "(.[^\n]*)")
    
    self.buffer = string.sub(self.buffer, #p+2, #(self.buffer))
    return res
  end
end

-- parts a chan
function luanet.irc.server:part(chan, message)
  if message == nil then message = "" end
  self:send("PART " .. chan .. " " .. message)
end

-- kicks someone off a channel
function luanet.irc.server:kick(chan, target, reason)
  if reason == nil then reason = "" end
  self:send("KICK " .. chan .. " " .. target .. " " .. reason)
end

-- quits the server
function luanet.irc.server:quit(message)
  if message == nil then message = "" end
  self:send("QUIT " .. message)
end

-- sends a message
function luanet.irc.server:privmsg(target, message)
  self:send("PRIVMSG " .. target .. " " .. message)
end

-- sends a notice
function luanet.irc.server:notice(target, message)
  self:send("NOTICE " .. target .. " " .. message)
end

-- join a chan
function luanet.irc.server:join(chan)
	self:send("JOIN "..chan)
end

-- send line
function luanet.irc.server:send(str)
  if self.socket ~= nil then
    self.socket:writeLine(str)
  end
end

-- change modes
function luanet.irc.server:mode(chan, modes, targets)
  if targets == nil then target = "" end
  self.socket:writeLine("MODE " .. chan .. " " .. modes .. " " .. targets)
end

-- Send the ident commands
function luanet.irc.server:identify()
  self:send("NICK "..self.nick)
  self:send("USER " .. self.user .. " - - :" .. self.realname)
end

-- Whois a person
function luanet.irc.server:whois(nick)
  self:send("WHOIS " .. nick)
  local end_whois = false
  local user = {}
  
  while not end_whois do
    local str = self:getLine()
    
    if str ~= nil then
      local author
      local command
      local param
      
      _, _, author, command, param = string.find(str, ":([%w@!-~.]+) ([%w.]+) :*(.+)")
      
      if command == "318" then
        end_whois = true
      elseif command == "401" then return nil
      elseif command == "311" then
        local _,_,_,n,u,h,_,r = string.find(param, "([%w_@!-~.]+) ([%w_@!-~.]+) ([%w_@!-~.]+) ([%w_@!-~.]+) (.[^:]+):(.+)")
        user.nick = n
        user.user = u
        user.host = h
        user.realname = r
      else
        self.buffer = self.buffer .. "\n" .. str
      end
    end
    luanet.sleep(50)
  end
  
  return user
  
end

-- proceed the events
function luanet.irc.server:proceed()
  local str = self:getBufferedLine()
  if str == "" then str = self:getLine() end
  
  -- if no string is retrieved
  if str == nil then return nil end
  
  -- if it's a ping
  if string.find(str, "PING") ~= nil and string.find(str, "PING") == 1 then
    if self.on_ping ~= nil then
      self:on_ping(str)
      return nil
    end
  end
  
  -- anyway
  local author
  local command
  local param
  
  _, _, author, command, param = string.find(str, ":([%w_@!-~.]+) ([%w.]+) :*(.+)")
  
  -- Acknowledge the fact that we are connected
  if command == "376" then
  	if self.on_connect ~= nil then
  		self:on_connect()
  	end
  -- Any numeric command
  elseif string.find(command, "^%d+$") == 1 then
    	if self.on_numeric ~= nil then
		self:on_numeric(author, command, param)
	end
  -- A privmsg
  elseif command == "PRIVMSG" then
  	_, _, chan, message = string.find(param, "([%w#*]*) :(.*)") 
  	if self.on_privmsg ~= nil then
  		self:on_privmsg(author, chan, message)
  	end
  -- A notice
  elseif command == "NOTICE" then
  	_, _, target, message = string.find(param, "([%w#*]*) :(.*)") 
  	if self.on_notice ~= nil then
  		self:on_notice(author, target, message)
  	end
  -- A join
  elseif command == "JOIN" then
  	_, _, chan = string.find(param, "(.*)") 
  	if self.on_join ~= nil then
  		self:on_join(author, chan)
  	end
  -- A part
  elseif command == "PART" then
  	_, _, chan = string.find(param, "(#[%w]*)")
  	-- if no part message is provided, it's just a ""
  	_, _, _, message = string.find(param, "(#[%w]*) :*(.+)")
  	if message == nil then message = "" end
  	if self.on_part ~= nil then
  		self:on_part(author, chan, message)
  	end
  elseif command == "KICK" then
	_, _, chan, target = string.find(param, "(#%w+) (%w+)")
	_, _, _, _, message = string.find(param, ":(.+)")
	if message == nil then message = "" end
	if self.on_kick ~= nil then
		self:on_kick(author, chan, target, message)
	end
  elseif command == "QUIT" then
  	_, _, message = string.find(param, ":*(.+)")
  	if message == nil then message = "" end
  	if self.on_quit ~= nil then
  		self:on_quit(author, message)
  	end
  end
  
  -- anyway, let's send it to the raw parser
  if self.on_raw ~= nil then
  	self:on_raw(str)
  end

  luanet.sleep(self.waittime)
end
