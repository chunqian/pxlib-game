toolchain("i686-windows")
set_toolset("cc", "i686-w64-mingw32-gcc")
set_toolset("cxx", "i686-w64-mingw32-gcc", "i686-w64-mingw32-g++")
set_toolset("ld", "i686-w64-mingw32-g++", "i686-w64-mingw32-gcc")
set_toolset("ar", "i686-w64-mingw32-ar")

toolchain("x86_64-windows")
set_toolset("cc", "x86_64-w64-mingw32-gcc")
set_toolset("cxx", "x86_64-w64-mingw32-gcc", "x86_64-w64-mingw32-g++")
set_toolset("ld", "x86_64-w64-mingw32-g++", "x86_64-w64-mingw32-gcc")
set_toolset("ar", "x86_64-w64-mingw32-ar")

-- 工程
target("demo")

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

-- add_includedirs(
--     "/usr/local/Cellar/mingw-w64/7.0.0_2/toolchain-i686/i686-w64-mingw32/include")
-- set_toolchains("i686-windows")

add_includedirs(
    "/usr/local/Cellar/mingw-w64/7.0.0_2/toolchain-x86_64/x86_64-w64-mingw32/include")
set_toolchains("x86_64-windows")

add_files(pxlibdir .. "/plat/windows/*.c")
add_files(pxlibdir .. "/plat/windows/*.cpp")
add_files(pxlibdir .. "/plat/PX_*.c")
add_files("$(projectdir)/pxlib/target/demo/*.c")

add_ldflags("-static-libgcc")
add_links("pxlib")
add_syslinks("winmm", "d2d1", "ws2_32", "dsound", "comdlg32")
