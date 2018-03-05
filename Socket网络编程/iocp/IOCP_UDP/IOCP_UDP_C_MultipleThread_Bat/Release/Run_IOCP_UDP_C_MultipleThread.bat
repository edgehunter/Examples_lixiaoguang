@echo off
for /l %%i in (1,1,10) do start IOCP_UDP_C_MultipleThread.exe
pause