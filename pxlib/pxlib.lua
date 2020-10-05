toolchain("i686-windows")
    set_toolset("cc", "i686-w64-mingw32-gcc")
    set_toolset("cxx", "i686-w64-mingw32-gcc", "i686-w64-mingw32-g++")
    set_toolset("ld", "i686-w64-mingw32-g++", "i686-w64-mingw32-gcc")

target("pxlib")

    -- 构建之后运行插件
    after_build(function (target)
        -- 导入task模块
        import("core.base.task")
        -- 运行插件任务
        task.run("project", {kind="compile_commands"})
    end)

    set_kind("static")

    set_plat("windows")
    set_arch("i686")

    set_optimize("faster")

    add_includedirs("/usr/local/Cellar/mingw-w64/7.0.0_2/toolchain-i686/i686-w64-mingw32/include")
    add_includedirs(pxlibdir)
    add_includedirs(pxlibdir .. "/core")
    add_includedirs(pxlibdir .. "/kernel")
    add_includedirs(pxlibdir .. "/arch")
    add_includedirs(pxlibdir .. "/platform/windows")

    add_files(pxlibdir .. "/core/*.c")
    add_files(pxlibdir .. "/kernel/*.c")
    add_files(pxlibdir .. "/arch/*.c")

    set_toolchains("i686-windows")

    add_ldflags("-static-libgcc")

--
-- If you want to known more usage about xmake, please see https://xmake.io
--
-- ## FAQ
-- 
-- You can enter the project directory firstly before building project.
--
--   $ cd projectdir
--
-- 1. How to build project?
--
--   $ xmake
--
-- 2. How to configure project?
--
--   $ xmake f -p [macosx|linux|iphoneos ..] -a [x86_64|i386|arm64 ..] -m [debug|release]
--
-- 3. Where is the build output directory?
--
--   The default output directory is `./build` and you can configure the output directory.
--
--   $ xmake f -o outputdir
--   $ xmake
--
-- 4. How to run and debug target after building project?
--
--   $ xmake run [targetname]
--   $ xmake run -d [targetname]
--
-- 5. How to install target to the system directory or other output directory?
--
--   $ xmake install
--   $ xmake install -o installdir
--
-- 6. Add some frequently-used compilation flags in xmake.lua
--
-- @code
--    -- add debug and release modes
--    add_rules("mode.debug", "mode.release")
--
--    -- add macro defination
--    add_defines("NDEBUG", "_GNU_SOURCE=1")
--
--    -- set warning all as error
--    set_warnings("all", "error")
--
--    -- set language: c99, c++11
--    set_languages("c99", "c++11")
--
--    -- set optimization: none, faster, fastest, smallest
--    set_optimize("fastest")
--
--    -- add include search directories
--    add_includedirs("/usr/include", "/usr/local/include")
--
--    -- add link libraries and search directories
--    add_links("tbox")
--    add_linkdirs("/usr/local/lib", "/usr/lib")
--
--    -- add system link libraries
--    add_syslinks("z", "pthread")
--
--    -- add compilation and link flags
--    add_cxflags("-stdnolib", "-fno-strict-aliasing")
--    add_ldflags("-L/usr/local/lib", "-lpthread", {force = true})
--
-- @endcode
--
