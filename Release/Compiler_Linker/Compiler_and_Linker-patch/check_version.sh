chmod 755 change_x86.sh change_arm.sh
echo
echo -n "The version of this program is: "
grep -q CC=gcc Makefile && (echo "x86";./change_x86.sh) ||(echo "ARM";./change_arm.sh)
echo ""