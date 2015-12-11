cp ../../../3D_src/Compiler_Linker/Compiler_and_Linker-patch/OGLCompilersDLL/Makefile.x86 ../../../3D_src/Compiler_Linker/Compiler_and_Linker-patch/OGLCompilersDLL/Makefile 
cp ../../../3D_src/Compiler_Linker/Compiler_and_Linker-patch/glslang/MachineIndependent/Makefile.x86 ../../../3D_src/Compiler_Linker/Compiler_and_Linker-patch/glslang/MachineIndependent/Makefile 
cp ../../../3D_src/Compiler_Linker/Compiler_and_Linker-patch/glslang/MachineIndependent/preprocessor/Makefile.x86 ../../../3D_src/Compiler_Linker/Compiler_and_Linker-patch/glslang/MachineIndependent/preprocessor/Makefile 
cp ../../../3D_src/Compiler_Linker/Compiler_and_Linker-patch/glslang/GenericCodeGen/Makefile.x86 ../../../3D_src/Compiler_Linker/Compiler_and_Linker-patch/glslang/GenericCodeGen/Makefile
cp ../../../3D_src/Compiler_Linker/Compiler_and_Linker-patch/glslang/OSDependent/Linux/Makefile.x86 ../../../3D_src/Compiler_Linker/Compiler_and_Linker-patch/glslang/OSDependent/Linux/Makefile
cp Makefile.x86 Makefile

#cp -rf llvm-obj-x86/* llvm-obj/

rm -f llvm-obj
ln -s llvm-obj-x86 llvm-obj

echo ""
echo "The version is X86 now"
echo ""