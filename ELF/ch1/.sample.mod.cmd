savedcmd_/home/pipishuo/Desktop/technology/ELF/ch1/sample.mod := printf '%s\n'   sample.o | awk '!x[$$0]++ { print("/home/pipishuo/Desktop/technology/ELF/ch1/"$$0) }' > /home/pipishuo/Desktop/technology/ELF/ch1/sample.mod