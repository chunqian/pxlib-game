#!/bin/sh

for d in PainterEngine/architecture \
    PainterEngine/core \
    PainterEngine/kernel \
    PainterEngine/platform/windows \
    PainterEngine Game/PainterEngine Game \
    PainterEngine Game/PainterEngine Game Client \
    PainterEngine Game/PainterEngine Game Server \
    PainterEngine Startup/PainterEngine NetWork \
    PainterEngine Startup/PainterEngine Startup \
    PainterEngine Startup/PainterEngine StartupServer; do
    clang-format -i $d/*.cpp
    clang-format -i $d/*.h
    clang-format -i $d/*.c
done
