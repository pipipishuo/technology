savedcmd_/home/pipishuo/Desktop/technology/ELF/ch1/sample/sample.mod := printf '%s\n'   sample.o | awk '!x[$$0]++ { print("/home/pipishuo/Desktop/technology/ELF/ch1/sample/"$$0) }' > /home/pipishuo/Desktop/technology/ELF/ch1/sample/sample.mod