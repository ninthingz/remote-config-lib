mkdir deploy\Release_x64
mkdir deploy\Release_x64\lib
mkdir deploy\Release_x64\include

copy cmake-build-release\lib\*.lib deploy\Release_x64\lib
copy src\include\*.h deploy\Release_x64\include

mkdir deploy\Release_x86
mkdir deploy\Release_x86\lib
mkdir deploy\Release_x86\include

copy cmake-build-release-x86\lib\*.lib deploy\Release_x86\lib
copy src\include\*.h deploy\Release_x86\include