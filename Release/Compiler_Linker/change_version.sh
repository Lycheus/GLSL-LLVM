chmod 755 change_x86.sh change_arm.sh
grep -q "#CCC=g++" Makefile && ./change_x86.sh || ./change_arm.sh
chmod 644 change_x86.sh change_arm.sh