add_rules("mode.release", "mode.debug")

pxlibdir = "$(projectdir)/pxlib"

includes("project/windows/pxlib.lua")
includes("project/windows/demo.lua")
includes("project/windows/client_startup.lua")
includes("project/windows/client_game.lua")
includes("project/windows/server_startup.lua")
includes("project/windows/server_game.lua")
