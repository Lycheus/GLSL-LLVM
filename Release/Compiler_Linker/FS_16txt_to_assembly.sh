./FS_16txt_to_01txt.sh $1 > temp_01_txt
./txt_to_bin temp_01_txt temp_bin > hide_info
./disassembler temp_bin
rm temp_01_txt temp_bin hide_info