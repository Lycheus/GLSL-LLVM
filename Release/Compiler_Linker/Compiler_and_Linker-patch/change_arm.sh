cp ../../../3D_src/Compiler_Linker/Compiler_and_Linker-patch/OGLCompilersDLL/Makefile.arm ../../../3D_src/Compiler_Linker/Compiler_and_Linker-patch/OGLCompilersDLL/Makefile 
cp ../../../3D_src/Compiler_Linker/Compiler_and_Linker-patch/glslang/MachineIndependent/Makefile.arm ../../../3D_src/Compiler_Linker/Compiler_and_Linker-patch/glslang/MachineIndependent/Makefile 
cp ../../../3D_src/Compiler_Linker/Compiler_and_Linker-patch/glslang/MachineIndependent/preprocessor/Makefile.arm ../../../3D_src/Compiler_Linker/Compiler_and_Linker-patch/glslang/MachineIndependent/preprocessor/Makefile 
cp ../../../3D_src/Compiler_Linker/Compiler_and_Linker-patch/glslang/GenericCodeGen/Makefile.arm ../../../3D_src/Compiler_Linker/Compiler_and_Linker-patch/glslang/GenericCodeGen/Makefile
cp ../../../3D_src/Compiler_Linker/Compiler_and_Linker-patch/glslang/OSDependent/Linux/Makefile.arm ../../../3D_src/Compiler_Linker/Compiler_and_Linker-patch/glslang/OSDependent/Linux/Makefile
cp Makefile.arm Makefile

#cp -rf llvm-obj-arm/* llvm-obj/

rm -f llvm-obj
ln -s llvm-obj-arm llvm-obj

echo ""
echo "The version is ARM now"
echo ""