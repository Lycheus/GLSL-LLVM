SOURCE_DIR=../../3D_src/Compiler_Linker/Compiler_and_Linker-patch
WORKING_DIR=Compiler_and_Linker-patch
#CCC=arm-none-linux-gnueabi-g++
CCC=g++
LEVEL=.
TARGET_FILE=$(LEVEL)/target.testbench
TEST_VAR=

include $(LEVEL)/Makefile.config

all: BuildLinux.sh $(WORKING_DIR)/modify_type $(WORKING_DIR)/add_metadata fixer $(WORKING_DIR)/FixTheNOPs format backend_modifier
	@cd $(WORKING_DIR);make;cp GLCompiler.o linker.o compiler.o inlineFunction.o disassembler output_txt ../
	$(CCC) $(DEFINE_FLAG) -g GLCompiler.o linker.o compiler.o inlineFunction.o -o test_Compiler
	rm $(WORKING_DIR)/GLCompiler.o

$(WORKING_DIR)/FixTheNOPs: $(SOURCE_DIR)/FixTheNOPs
	cp $^ $@

assembler: $(SOURCE_DIR)/linker.c $(SOURCE_DIR)/inlineFunction.cpp
	$(CCC) -Wall -g -fno-stack-protector -D'assembler_standalone' $(DEFINE_FLAG) $(SOURCE_DIR)/linker.c $(SOURCE_DIR)/inlineFunction.cpp -o assembler

red_cube:
	#make DEFINE_FLAG="$(DEFINE_FLAG) -D'RED_CUBE' -D'unified_shader_register' "
	make DEFINE_FLAG="$(DEFINE_FLAG) -D'RED_CUBE' "
	make check_version

texture_cube:
	make DEFINE_FLAG="$(DEFINE_FLAG) -D'TEXTURE_CUBE'"
	make check_version

lighting_cube:
	make DEFINE_FLAG="$(DEFINE_FLAG) -D'LIGHTING_CUBE'"
	make check_version

lighting_cube2:
	make DEFINE_FLAG="$(DEFINE_FLAG) -D'LIGHTING_CUBE2'"
	make check_version

lighting_cube_new:
	make DEFINE_FLAG="$(DEFINE_FLAG) -D'NEW_LIGHTING_CUBE'"
	make check_version

ball_VS:
	make DEFINE_FLAG="$(DEFINE_FLAG) -D'MORPHING_BALL_IN_VS'"
	make check_version

ocean:
	make DEFINE_FLAG="$(DEFINE_FLAG) -D'OCEAN_SIMULATION'"
	make check_version

default_testbench:
	make DEFINE_FLAG="$(DEFINE_FLAG) -D'DEFAULT_TESTBENCH'"
	make check_version


BuildLinux.sh:	$(SOURCE_DIR)/BuildLinux.sh
	cp $(SOURCE_DIR)/BuildLinux.sh .

$(WORKING_DIR)/modify_type: $(SOURCE_DIR)/modify_type
	cp $(SOURCE_DIR)/modify_type $(WORKING_DIR)/

$(WORKING_DIR)/add_metadata: $(SOURCE_DIR)/add_metadata
	cp $(SOURCE_DIR)/add_metadata $(WORKING_DIR)/

fixer: $(SOURCE_DIR)/fixer.cpp
	$(CCC) $(SOURCE_DIR)/fixer.cpp -o fixer; mv fixer $(WORKING_DIR)/StandAlone/

backend_modifier: $(SOURCE_DIR)/backend_modifier.cpp
	$(CCC) $(DEFINE_FLAG) $(SOURCE_DIR)/backend_modifier.cpp -o backend_modifier; 

format: $(SOURCE_DIR)/format
	cp $(SOURCE_DIR)/format $(WORKING_DIR)

test_inline: $(SOURCE_DIR)/inlineFunction.cpp $(SOURCE_DIR)/linker.c
	$(CCC) $(DEFINE_FLAG) $^ -D'inline_standalone' -o $@

set_llvm_path:
	./set_llvm_path.sh

help:
	@echo ""
	@echo "  flag                   meaning"
	@echo "***********************************************************"
	@echo "  all:                   Build all without llvm."
	@echo "  llvm_x86:              Build llvm for x86."
	@echo "  llvm_arm:              Build llvm for arm."
	@echo "  llvm:                  Build llvms for x86 and arm."
	@echo "  remake-llvm:           Remake llvms for x86 and arm."
	@echo "  clean_llvm_x86:        Clean x86 llvm."
	@echo "  clean_llvm_arm:        Clean x86 llvm."
	@echo "  clean_llvm:            Clean x86 and arm llvms."
	@echo "  check_version:         Check the version of llvm now."
	@echo "  change_version:        Change the version to another."
	@echo "  change_version:        Change the version to another."
	@echo "  test_inline:           Test inline function."
	@echo "  assembler:             Build simple assembler."
	@echo ""
	@echo "  The followings must be chosen one at most:"
	@echo "  red_cube:              Compile red cube testbench."
	@echo "  texture_cube:          Compile texture cube testbench."
	@echo "  lighting_cube:         Compile lighting cube testbench."
	@echo "  ball_VS:               Compile ball_in_VS testbench."
	@echo "  ocean:                 Compile ocean simulation testbench."
	@echo "  default_testbench:     Compile test testbench."
	@echo ""

llvm:
	@cd $(WORKING_DIR);make compile_llvm

remake-llvm:
	@cd $(WORKING_DIR)/llvm-obj-x86;make
	@cd $(WORKING_DIR)/llvm-obj-arm;make
	make check_version

llvm_x86:
	@cd $(WORKING_DIR);make compile_llvm_x86

llvm_arm:
	@cd $(WORKING_DIR);make compile_llvm_arm

clean_llvm:
	@cd $(WORKING_DIR);make clean_llvm

clean_llvm_x86:
	@cd $(WORKING_DIR);make clean_llvm_x86

clean_llvm_arm:
	@cd $(WORKING_DIR);make clean_llvm_arm

change_version:
	@./change_version.sh
	@cd $(WORKING_DIR);./change_version.sh

check_version:
	@cd $(WORKING_DIR);./check_version.sh

clean:
	rm BuildLinux.sh GLCompiler.o linker.o compiler.o inlineFunction.o peep assembler test_Compiler test_inline backend_modifier -f
	@cd $(WORKING_DIR);make clean
	rm -f $(WORKING_DIR)/llvm-obj
