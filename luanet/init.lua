-- luanet

-- Version 0.1
-- Author Thomas Maurice

luanet = {}
luanet.__index = luanet

-- Basic configuration
luanet.version = "0.1"
luanet.author  = "Thomas Maurice"

-- Loads all the sub-modules
require 'luanet.socket'
