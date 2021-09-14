@echo off

echo Backing up file...
rename c4chess.exe c4chess_bak.exe 
del c4chess.exe

echo.
Echo make...
cd ./ && make && c4chess

echo.
echo Finish!
pause >NUL
