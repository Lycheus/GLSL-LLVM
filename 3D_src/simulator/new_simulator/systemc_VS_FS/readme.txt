�o�O�@�ӥ�SystemC�g����������
�w��ثe3D 2.0��VS�PFS������
�p�G�A���X����ק��commit��SVN
�Ф@�w�n�d�Umessage �V�ԲӶV�n


�ؿ��ɮ׻��� :

show_frame.exe				��dump��framebuffø�s�X��, win������

testbnech.h				�M�w�������n�]����testbench, �ק��ݭ��s�sĶ

FS_input/
   ./testbench_name/txt_bin		FS ���O
   ./testbench_name/Uniform.txt		FS Uniform
��:FS��J��viewport��VS����

VS_input/
   ./testbench_name/txt_bin		VS���O
   ./testbench_name/Uniform.txt		VS Uniform
   ./testbench_name/Vertex_Data.txt	VS Attribute

output/
   ./framebuff.txt			FS dump��framebuff
   ./log.VS_port.txt			VS input dump �Ψ������ɮ�Ū���O�_���T
   ./log.FS.txt				FS input dump �Ψ������ɮ�Ū���O�_���T
   ./VS_VOB.txt				VS��X��VOB(�gviewport�ഫ)

VS�D�n�{�� :
src_VS6/SC_VS.cpp

FS�D�n�{�� :
src_FS5/FS.cpp

���� :
./test_shader

����(��X���ɮ�,�����t�׸���) :
./test_shader > �ɦW

����(��X���ɮ�,�����t�׳̧�) :
./test_shader > /dev/null