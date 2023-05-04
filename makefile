VCVARSALL_PATH="D:\Visual Studio 2022\Visual Studio\VC\Auxiliary\Build\vcvarsall.bat"
SOURCES=2sem8lab.cpp Dictionary.cpp checks.cpp excel.cpp
EXECUTABLE=main.exe

compile:
	%comspec% /c "${VCVARSALL_PATH} x64 && cl.exe /W3 ${SOURCES} /link /out:$(EXECUTABLE)"