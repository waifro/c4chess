@echo off

echo.
Echo make...
cd ./ && make && cd output && c4chess.exe

echo.
echo Finish!
pause >NUL
