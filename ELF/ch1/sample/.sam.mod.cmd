savedcmd_/home/pipishuo/Desktop/knowledge/technology/ELF/ch1/sample/sam.mod := printf '%s\n'   obj.o sample.o | awk '!x[$$0]++ { print("/home/pipishuo/Desktop/knowledge/technology/ELF/ch1/sample/"$$0) }' > /home/pipishuo/Desktop/knowledge/technology/ELF/ch1/sample/sam.mod