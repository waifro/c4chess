@echo off

echo.
Echo make...
cd ./ && make && cd output && c4chess.exe && cd ..

echo.
echo Finish!
pause >NUL
