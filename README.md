# SIC
SIC算法复现

# Build
g++-10 /media/zhn/D/From_C/Documents/GraduationProject/IM-code/int_SIC/src/sic_main.cpp /media/zhn/D/From_C/Documents/GraduationProject/IM-code/int_SIC/src/sic.cpp /media/zhn/D/From_C/Documents/GraduationProject/IM-code/int_SIC/src/sievestream_recurse.cpp -lpthread -std=c++20 -o output/sic_recurse.out

# Usage
output/sic_loop.out -u 2000000 -beta 0.1 -k 50 -n 250000 -l 5000 -dbplace /media/zhn/D/From_C/Documents/GraduationProject/IM-code/int_SIC/database/higgs-activity_time.txt -logplace /media/zhn/D/From_C/Documents/GraduationProject/IM-code/int_SIC/log/loop