@echo off
chcp 65001 >nul
title VNB Bank Compiler
cls

echo.
echo ╔═══════════════════════════════════════════╗
echo ║      VNB Banking System Compiler          ║
echo ║      VELELO NATIONAL BANK                 ║
echo ╚═══════════════════════════════════════════╝
echo.

:: Check for compiler
where g++ >nul 2>nul
if errorlevel 1 (
    echo ERROR: g++ compiler not found!
    echo.
    echo Please install:
    echo 1. MinGW-w64 from https://mingw-w64.org/
    echo 2. Add to PATH environment variable
    echo.
    pause
    exit /b 1
)

echo Compiling VNB Banking System...
echo.

:: Clean previous builds
if exist VNBBank.exe del VNBBank.exe
if exist *.o del *.o

:: Compile all files
g++ -std=c++17 -O2 -Wall ^
    main.cpp ^
    VNBBank.cpp ^
    Account.cpp ^
    Transaction.cpp ^
    VNBUI.cpp ^
    -o VNBBank.exe

if errorlevel 1 (
    echo.
    echo ❌ Compilation failed!
    echo Please check for errors above.
    pause
    exit /b 1
)

echo.
echo ✅ Compilation successful!
echo.
echo ╔═══════════════════════════════════════════╗
echo ║      VNB Banking System Ready!           ║
echo ╚═══════════════════════════════════════════╝
echo.
echo To run: VNBBank.exe
echo.
timeout /t 2 >nul

:: Run the program
cls
VNBBank.exe

pause