./disassembler final_bin_fragment |awk '{for (i=0;i<10;i++) if(i==4) t[i] = $i; for(i=0;i<10;i++) $i=t[i];print}' | tr -d " :"|sed 's/\(.*\)/\1%\1/'|sed 's/^%$//'|sed 's/^[0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f]//'|sed 's/[0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f]$//'|sed -n /^$/!p | tr '%' '\n' | sed 's/^/0x/' 