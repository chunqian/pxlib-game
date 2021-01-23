add_rules("mode.debug", "mode.release")

pxlibdir = "$(projectdir)/pxlib"

includes("project/pxlib.lua")
includes("project/client_startup.lua")
-- includes("project/client_game.lua")
-- includes("project/server_startup.lua")
-- includes("project/server_game.lua")
