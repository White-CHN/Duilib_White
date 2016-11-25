del *.sdf
del Debug\win32\*.ilk
del Debug\win32\*.pdb
del Debug\win32\*.lib

rd /s /q Lib
rd /s /q Temp
rd /s /q Release
rd /s /q ipch
rd /s /q Debug\x64

rd /s /q DuiDemo\Debug
rd /s /q DuiDemo\Release
rd /s /q DuiDemo\x64