call "C:\Program Files\Microsoft Visual Studio\2022\Preview\VC\Auxiliary\Build\vcvarsall.bat" x86_amd64
cd build
cmake ..
msbuild ALL_BUILD.vcxproj