add_rules("mode.release", "mode.debug")

pxlibdir = "$(projectdir)/pxlib"

includes("project/windows/pxlib.lua")
-- includes("project/windows/client_startup.lua")
-- includes("project/windows/client_game.lua")
-- includes("project/windows/server_startup.lua")
-- includes("project/windows/server_game.lua")
includes("project/windows/console.lua")
-- includes("project/windows/fontmodule.lua")
-- includes("project/windows/pngtotraw.lua")
-- includes("project/windows/2dxmaketool.lua")
