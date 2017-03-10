del *.sdf
del *.suo
del bin\*.ilk
del bin\*.manifest
del bin\*.pdb
del bin\*.lib
del bin\*.exe

del Demos\DuiDemo\*.aps
del Demos\BaoFeng\*.aps
del Demos\DKPManagement\*.aps

rd /s /q Lib
rd /s /q Temp
rd /s /q ipch

rd /s /q Demos\DuiDemo\localStorage

@echo off
set "a=%cd%\bin\wke.dll"
set "b=%cd%\bin\msado15.dll"
set "c=%cd%\bin\msadox.dll"
for /r %cd% %%d in (*.dll) do (
	if "%a%"=="%%d" (echo 123213) else if "%b%"=="%%d" () else if "%c%"=="%%d" () else (del "%%d")
)