toolchain("i686-windows")
set_toolset("cc", "i686-w64-mingw32-gcc")
set_toolset("cxx", "i686-w64-mingw32-gcc", "i686-w64-mingw32-g++")
set_toolset("ld", "i686-w64-mingw32-g++", "i686-w64-mingw32-gcc")

-- 工程
target("pxlib")

-- 构建之后运行插件
after_build(function(target)
    -- 导入task模块
    import("core.base.task")
    -- 运行插件任务
    task.run("project", {kind = "compile_commands"})
end)

set_kind("static")
set_optimize("faster")
set_targetdir("$(buildir)/$(mode)")

add_includedirs(
    "/usr/local/Cellar/mingw-w64/8.0.0_3/toolchain-i686/i686-w64-mingw32/include")
set_toolchains("i686-windows")

add_files(pxlibdir .. "/core/*.c")
add_files(pxlibdir .. "/kernel/*.c")
add_files(pxlibdir .. "/arch/*.c")

add_ldflags("-static-libgcc")
