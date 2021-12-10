cls &&^
nvcc -allow-unsupported-compiler -I "./incl" "./src/CudaTest.cu" -o "./outcu/testing.exe" &&^
cls &&^
"./outcu/testing.exe"
