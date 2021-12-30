s/^inp\s\([^ ]*\)/\n\tv.w=w; v.x=x; v.y=y; v.z=z;\n\treturn v;\n}\n\nTVars step(TVars v, int i) {\n\tlong int w=v.w, x=v.x, y=v.y, z=v.z;\n\t\1=i;/
s/^mul\s\([^ ]*\)\s\([^ ]*\)/\t\1*=\2;/
s/^add\s\([^ ]*\)\s\([^ ]*\)/\t\1+=\2;/
s/^div\s\([^ ]*\)\s\([^ ]*\)/\t\1=\1\/\2;/
s/^mod\s\([^ ]*\)\s\([^ ]*\)/\t\1=\1%\2;/
s/^eql\s\([^ ]*\)\s\([^ ]*\)/\t\1=(\1==\2)?1:0;/
