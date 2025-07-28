
@echo off
if not exist ..\build mkdir ..\build
pushd ..\build
del *.pdb > NUL 2> NUL
echo WAITING FOR PDB > LOCK.tmp

call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
cl  /I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\ucrt" -nologo -Zi -FC -DDEVELOPMENT ..\code\win32_platform.c /link user32.lib gdi32.lib  -incremental:no -opt:ref

del lock.tmp
del *.obj

popd
