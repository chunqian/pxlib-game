toolchain("i686-windows")
set_toolset("cc", "i686-w64-mingw32-gcc")
set_toolset("cxx", "i686-w64-mingw32-gcc", "i686-w64-mingw32-g++")
set_toolset("ld", "i686-w64-mingw32-g++", "i686-w64-mingw32-gcc")
set_toolset("ar", "i686-w64-mingw32-ar")

-- 工程
target("2dxmaketool")

-- 依赖
add_deps("pxlib")

-- 构建之后运行插件
after_build(function(target)
    -- 导入task模块
    import("core.base.task")
    -- 运行插件任务
    task.run("project", {kind = "compile_commands"})
end)

set_kind("binary")
set_optimize("faster")
set_targetdir("$(buildir)/$(mode)")
add_linkdirs("$(buildir)/$(mode)")

add_includedirs(
    "/usr/local/Cellar/mingw-w64/8.0.0_3/toolchain-i686/i686-w64-mingw32/include")
set_toolchains("i686-windows")

add_files(pxlibdir .. "/plat/windows/*.c|PX_File.c")
add_files(pxlibdir .. "/plat/PX_*.c")
add_files("$(projectdir)/pxtool/2dxmaketool/*.c")
-- add_files("$(projectdir)/pxtool/2dxmaketool/*.cpp")

add_ldflags("-static-libgcc")
add_links("pxlib")
add_syslinks("winmm", "ws2_32", "comdlg32")
