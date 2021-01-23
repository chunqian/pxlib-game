add_rules("mode.debug", "mode.release")

toolchain("i686-windows")
set_toolset("cc", "i686-w64-mingw32-gcc")
set_toolset("cxx", "i686-w64-mingw32-gcc", "i686-w64-mingw32-g++")
set_toolset("ld", "i686-w64-mingw32-g++", "i686-w64-mingw32-gcc")
set_toolset("ar", "i686-w64-mingw32-ar")

pxlibdir = "$(projectdir)/pxlib"

includes("pxlib/xmake.lua")

-- 工程
-- target("startup_client")

-- -- 构建之后运行插件
-- del_files("compile_commands.json")
-- after_build(function(target)
--     -- 导入task模块
--     import("core.base.task")
--     -- 运行插件任务
--     task.run("project", {kind = "compile_commands"})
-- end)

-- set_kind("binary")

-- set_plat("windows")
-- set_arch("i686")

-- set_optimize("faster")

-- add_includedirs(
--     "/usr/local/Cellar/mingw-w64/7.0.0_2/toolchain-i686/i686-w64-mingw32/include")
-- -- add_includedirs("$(projectdir)/startup/network")
-- -- add_includedirs(pxlibdir .. "/core")
-- -- add_includedirs(pxlibdir .. "/kernel")
-- -- add_includedirs(pxlibdir .. "/arch")
-- -- add_includedirs(pxlibdir .. "/platform/windows")

-- add_files(pxlibdir .. "/platform/windows/*.c")
-- add_files(pxlibdir .. "/platform/windows/*.cpp|Platform_Windows_D3D.cpp")
-- add_files("$(projectdir)/startup/client/*.c")

-- set_toolchains("i686-windows")

-- add_ldflags("-static-libgcc")

-- -- add_cxflags("-fexec-charset=GBK")

-- add_links("pxlib")
-- add_linkdirs("$(buildir)/windows/i686/release/")

-- add_syslinks("winmm", "d2d1", "ws2_32", "dsound", "comdlg32")

-- 工程
target("game_client")

-- 构建之后运行插件
del_files("compile_commands.json")
after_build(function(target)
    -- 导入task模块
    import("core.base.task")
    -- 运行插件任务
    task.run("project", {kind = "compile_commands"})
end)

set_kind("binary")

set_plat("windows")
set_arch("i686")

set_optimize("faster")

add_includedirs(
    "/usr/local/Cellar/mingw-w64/7.0.0_2/toolchain-i686/i686-w64-mingw32/include")

add_files(pxlibdir .. "/platform/windows/*.c")
add_files(pxlibdir .. "/platform/windows/*.cpp|Platform_Windows_D3D.cpp")
add_files("$(projectdir)/game/common/*.c")
add_files("$(projectdir)/game/client/*.c")

set_toolchains("i686-windows")

add_ldflags("-static-libgcc")

add_links("pxlib")
add_linkdirs("$(buildir)/windows/i686/release/")

add_syslinks("winmm", "d2d1", "ws2_32", "dsound", "comdlg32")

-- 工程
-- target("startup_server")

-- -- 构建之后运行插件
-- del_files("compile_commands.json")
-- after_build(function(target)
--     -- 导入task模块
--     import("core.base.task")
--     -- 运行插件任务
--     task.run("project", {kind = "compile_commands"})
-- end)

-- set_kind("binary")

-- set_plat("windows")
-- set_arch("i686")

-- set_optimize("faster")

-- add_includedirs(
--     "/usr/local/Cellar/mingw-w64/7.0.0_2/toolchain-i686/i686-w64-mingw32/include")

-- add_files(pxlibdir .. "/platform/windows/*.c|Platform_Windows_SerialPort.c")
-- add_files(pxlibdir ..
--               "/platform/windows/*.cpp|Platform_Windows_D3D.cpp|Platform_Windows_DSound.cpp")
-- add_files("$(projectdir)/startup/server/*.c")

-- set_toolchains("i686-windows")

-- add_ldflags("-static-libgcc")

-- add_links("pxlib")
-- add_linkdirs("$(buildir)/windows/i686/release/")

-- add_syslinks("winmm", "d2d1", "ws2_32", "dsound", "comdlg32")

-- -- 工程
-- target("game_server")

-- -- 构建之后运行插件
-- del_files("compile_commands.json")
-- after_build(function(target)
--     -- 导入task模块
--     import("core.base.task")
--     -- 运行插件任务
--     task.run("project", {kind = "compile_commands"})
-- end)

-- set_kind("binary")

-- set_plat("windows")
-- set_arch("i686")

-- set_optimize("faster")

-- add_includedirs(
--     "/usr/local/Cellar/mingw-w64/7.0.0_2/toolchain-i686/i686-w64-mingw32/include")

-- add_files(pxlibdir .. "/platform/windows/*.c")
-- add_files(pxlibdir .. "/platform/windows/*.cpp|Platform_Windows_D3D.cpp")
-- add_files("$(projectdir)/game/common/*.c")
-- add_files("$(projectdir)/game/server/*.c")

-- set_toolchains("i686-windows")

-- add_ldflags("-static-libgcc")

-- add_links("pxlib")
-- add_linkdirs("$(buildir)/windows/i686/release/")

-- add_syslinks("winmm", "d2d1", "ws2_32", "dsound", "comdlg32")
