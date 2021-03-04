# pxtools

## build

```bash
conan create recipes/libxml2/all libxml2/2.9.10@ -pr:h prh -pr:b prb --build missing
conan install . -pr:h prh -pr:b prb --build missing
xmake f -p mingw -a i386
xmake clean
xmake
```
