patch -R -p0 <x86_to_arm.patch
cp -rf llvm-obj-x86/* llvm-obj/
echo ""
echo "The version is X86 now"
echo ""