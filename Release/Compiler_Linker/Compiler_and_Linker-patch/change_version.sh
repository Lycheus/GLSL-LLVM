chmod 755 change_x86.sh change_arm.sh
grep -q CC=gcc Makefile && ./change_arm.sh || ./change_x86.sh
chmod 644 change_x86.sh change_arm.sh