cls &&^
nvcc -allow-unsupported-compiler -I "./incl" "./src/CudaTest.cu" -o "./out/testing.exe" &&^
cls &&^
"out/testing.exe"
