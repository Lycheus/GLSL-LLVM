這是一個用SystemC寫成的模擬器
針對目前3D 2.0的VS與FS做模擬
如果你做出任何修改後commit至SVN
請一定要留下message 越詳細越好


目錄檔案說明 :

show_frame.exe				把dump的framebuff繪製出來, win執行檔

testbnech.h				決定模擬器要跑哪個testbench, 修改後需重新編譯

FS_input/
   ./testbench_name/txt_bin		FS 指令
   ./testbench_name/Uniform.txt		FS Uniform
註:FS輸入的viewport由VS提供

VS_input/
   ./testbench_name/txt_bin		VS指令
   ./testbench_name/Uniform.txt		VS Uniform
   ./testbench_name/Vertex_Data.txt	VS Attribute

output/
   ./framebuff.txt			FS dump的framebuff
   ./log.VS_port.txt			VS input dump 用來驗證檔案讀取是否正確
   ./log.FS.txt				FS input dump 用來驗證檔案讀取是否正確
   ./VS_VOB.txt				VS輸出的VOB(經viewport轉換)

VS主要程式 :
src_VS6/SC_VS.cpp

FS主要程式 :
src_FS5/FS.cpp

執行 :
./test_shader

執行(輸出到檔案,模擬速度較快) :
./test_shader > 檔名

執行(輸出到檔案,模擬速度最快) :
./test_shader > /dev/null