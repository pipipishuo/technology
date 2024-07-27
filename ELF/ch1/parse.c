#include<fstream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include"commType.h"

#define EI_NIDENT 16
#define ELF64_R_SYM(i)    ((i)>>32)
typedef struct {
        unsigned char   e_ident[EI_NIDENT];
        Elf64_Half      e_type;
        Elf64_Half      e_machine;
        Elf64_Word      e_version;
        Elf64_Addr      e_entry;
        Elf64_Off       e_phoff;
        Elf64_Off       e_shoff;
        Elf64_Word      e_flags;
        Elf64_Half      e_ehsize;
        Elf64_Half      e_phentsize;
        Elf64_Half      e_phnum;
        Elf64_Half      e_shentsize;
        Elf64_Half      e_shnum;
        Elf64_Half      e_shtrndx;
} Elf64_Ehdr;

typedef struct {
	Elf64_Word	sh_name;
	Elf64_Word	sh_type;
	Elf64_Xword	sh_flags;
	Elf64_Addr	sh_addr;
	Elf64_Off	sh_offset;
	Elf64_Xword	sh_size;
	Elf64_Word	sh_link;
	Elf64_Word	sh_info;
	Elf64_Xword	sh_addralign;
	Elf64_Xword	sh_entsize;
} Elf64_Shdr;
enum sh_type{
    SHT_NULL=0,
    SHT_PROGBITS,
    SHT_SYMTAB,
    SHT_STRTAB,
    SHT_RELA,
    SHT_HASH,
    SHT_DYNAMIC,
    SHT_NOTE,
    SHT_NOBITS,
    SHT_REL,
    SHT_SHLIB,
    SHT_DYNSYM,
    SHT_INIT_ARRAY=14,
    SHT_FINI_ARRAr=15,
    SHT_PREINIT_ARRAY=16,
    SHT_GROUP=17,
    SHT_SYMTAB_SHNDX=18,
    SHT_LOOS=0x60000000,
    SHT_HIOS=0x6fffffff,
    SHT_LOPROC=0x70000000,
    SHT_HIPROC=0x7fffffff,
    SHT_LOUSER=0x80000000,
    SHT_HIUSER=0xffffffff
};
typedef struct {
	Elf64_Addr	r_offset;
	Elf64_Xword	r_info;
} Elf64_Rel;

typedef struct {
	Elf64_Addr	r_offset;
	Elf64_Xword	r_info;
	Elf64_Sxword	r_addend;
} Elf64_Rela;
typedef struct {
	Elf64_Word	st_name;
	unsigned char	st_info;
	unsigned char	st_other;
	Elf64_Half	st_shndx;
	Elf64_Addr	st_value;
	Elf64_Xword	st_size;
} Elf64_Sym;
void printELFHeaderIdent(unsigned char e_ident[]);
void printELFHeaderType(Elf32_Half type);
void printELFHeaderMachine(Elf32_Half e_machine);
Elf64_Shdr findSection(char* buf, Elf64_Ehdr hdr,sh_type type);
Elf64_Shdr findSectionByIndex(char* buf, Elf64_Ehdr hdr,int index);
void printfSection(char*buf,Elf64_Shdr section);
void printfSymbolTable(char* buf, Elf64_Shdr shdr,int offset);
void printfRelaTable(char*buf,Elf64_Shdr section,Elf64_Shdr symSe,int offset);
char* printfRelaEntryName(char* buf,Elf64_Shdr shdr,int offset,int index);
int main(){
    std::string mystring;
    FILE* myfile;
    if((myfile=fopen("sample.out","rb"))!=NULL){
        Elf64_Ehdr hdr;
        printf("size:%ld\n",sizeof(Elf64_Ehdr));
        char buf[112256];
        memset(buf,0,sizeof(buf));
        fread(buf,sizeof(buf),1,myfile);
        memcpy(&hdr,buf,sizeof(Elf64_Ehdr));
        printELFHeaderIdent(hdr.e_ident);
        printELFHeaderType(hdr.e_type);
        printELFHeaderMachine(hdr.e_machine);
        printf("e_version:%d\n",hdr.e_version);
        printf("e_entry:%d\n",hdr.e_version);
        printf("e_phoff:%ld\n",hdr.e_phoff);
        printf("e_shoff:%ld\n",hdr.e_shoff);
        printf("e_flags:%d\n",hdr.e_flags);
        printf("e_ehsize:%d\n",hdr.e_ehsize);
        printf("e_phentsize:%d\n",hdr.e_phentsize);
        printf("e_phnum:%d\n",hdr.e_phnum);
        printf("e_shentsize:%d\n",hdr.e_shentsize);
        printf("e_shnum:%d\n",hdr.e_shnum);
        printf("e_shtrndx:%d\n",hdr.e_shtrndx);
      printf("%2s%40s%8s%8s%8s%8s%8s%8s%8s%13s%8s\n","num","name","type","flags","addr","offset","size","link","info","addralign","entsize");
      for(int i=0;i<hdr.e_shnum;i++){
            Elf64_Shdr section;
            memcpy(&section,buf+hdr.e_shoff+i*hdr.e_shentsize,sizeof(Elf64_Shdr));
            printf("%2d",i);
            printf("%40s",buf+0x3556+section.sh_name);
            printf("%8x",section.sh_type);
            printf("%8x",section.sh_flags);
            printf("%8x",section.sh_addr);
            printf("%8x",section.sh_offset);
            printf("%8x",section.sh_size);
            printf("%8x",section.sh_link);
            printf("%8x",section.sh_info);
            printf("%13x",section.sh_addralign);
            printf("%8x",section.sh_entsize);
            printf("\n");
        }
      printf("symbo table\n");
      Elf64_Shdr se=findSection(buf,hdr,SHT_DYNSYM);
      if(se.sh_type==SHT_NULL){
       printf("not found\n"); 
      }else{
       printfSection(buf,se); 
       printf("linkIndex:%d\n",se.sh_link);
       Elf64_Shdr strSe=findSectionByIndex(buf,hdr,se.sh_link);
       int offset=strSe.sh_offset;
       printf("strTableoffset:%x\n",offset);
       printfSymbolTable(buf,se,offset);
      }
      
      printf("rela table\n");
      Elf64_Shdr relaSe=findSection(buf,hdr,SHT_RELA);
      if(se.sh_type==SHT_NULL){
       printf("not found\n"); 
      }else{
       printfSection(buf,relaSe); 
       Elf64_Shdr se=findSection(buf,hdr,SHT_DYNSYM);
       Elf64_Shdr strSe=findSectionByIndex(buf,hdr,se.sh_link);
       int offset=strSe.sh_offset;
       printfRelaTable(buf,relaSe,se,offset);
      }
              
    }else{
        printf("error!\n");
    }
    return 0;
}
void printfRelaTable(char*buf,Elf64_Shdr section,Elf64_Shdr symSe,int offset){
    int entrySize=sizeof(Elf64_Rela);
    printf("%8s%8s%12s%30s%8s\n","num","offset","info","name","addend");
    for(int i=0;i<(section.sh_size/entrySize);i++){
        Elf64_Rela rela;
        memcpy(&rela,buf+section.sh_offset+i*entrySize,entrySize);
       
        printf("%8d",i);
        printf("%8x",rela.r_offset);
        printf("%12lx",rela.r_info);
        int index=ELF64_R_SYM(rela.r_info);
        printf("%8d",index);
      // char* namePtr=printfRelaEntryName(buf,symSe,offset,index);
        printf("%30s",rela.r_addend);
        printf("\n");
    }
}
char* printfRelaEntryName(char* buf,Elf64_Shdr shdr,int offset,int index){
        int entrySize=sizeof(Elf64_Sym);
      for(int i=0;i<shdr.sh_size/entrySize;i++){
        if(index==i){
             Elf64_Sym sym;
            memcpy(&sym,buf+shdr.sh_offset+i*entrySize,sizeof(Elf64_Sym));
           
            char*b=buf+offset+sym.st_name;
            return b;
        }
          
      }  
}
Elf64_Shdr* findSectionByIndex(char* buf, Elf64_Ehdr hdr,int index){
      Elf64_Shdr* section=NULL;
      section.sh_type=SHT_NULL;
      for(int i=0;i<hdr.e_shnum;i++){
            memcpy(&section,,sizeof(Elf64_Shdr));
            if(i==index){
                section=(Elf64_Shdr*)buf+hdr.e_shoff+i*hdr.e_shentsize;
            return section;
            }
       }
    return section;

}
void printfSymbolTable(char* buf, Elf64_Shdr shdr,int offset){
      printf("tableSize:%d\n",shdr.sh_size);
      int entrySize=sizeof(Elf64_Sym);
      printf("entrySize:%d\n",entrySize);
      printf("%4s%40s%8s%8s%8s%8s%8s\n","num","name","info","other","shndx","value","size");
      for(int i=0;i<shdr.sh_size/entrySize;i++){
            Elf64_Sym sym;
            memcpy(&sym,buf+shdr.sh_offset+i*entrySize,sizeof(Elf64_Sym));
            printf("%4d",i);
            printf("%40s",buf+offset+sym.st_name);
            printf("%8x",sym.st_info);
            printf("%8x",sym.st_other);
            printf("%8d",sym.st_shndx);
            printf("%8d",sym.st_value);
            printf("%8d",sym.st_size);
            printf("\n");
      }
}
Elf64_Shdr findSection(char* buf, Elf64_Ehdr  hdr,sh_type type){ 
      Elf64_Shdr section;
      section.sh_type=SHT_NULL;
      for(int i=0;i<hdr.e_shnum;i++){
            memcpy(&section,buf+hdr.e_shoff+i*hdr.e_shentsize,sizeof(Elf64_Shdr));
            if(section.sh_type==type){
            return section;
            }
       }
    return section;
}
void printfSection(char* buf,Elf64_Shdr section){
    printf("%2s%40s%8s%8s%8s%8s%8s%8s%8s%13s%8s\n","num","name","type","flags","addr","offset","size","link","info","addralign","entsize");
    printf("%40s",buf+0x3556+section.sh_name);
    printf("%8x",section.sh_type);
    printf("%8x",section.sh_flags);
    printf("%8x",section.sh_addr);
    printf("%8x",section.sh_offset);
    printf("%8x",section.sh_size);
    printf("%8x",section.sh_link);
    printf("%8x",section.sh_info);
    printf("%13x",section.sh_addralign);
    printf("%8x",section.sh_entsize);
    printf("\n");
}
void printELFHeaderMachine(Elf32_Half e_machine){
    switch(e_machine){
    case 0:
        printf("e_machine: No machine\n");
        break;
    case 1:
        printf("e_machine: AT&T WE 32100\n");
        break;
    case 2:
        printf("e_machine: SPARC\n");
        break;
    case 3:
        printf("e_machine: Intel Architecture\n");
        break;
    case 4:
        printf("e_machine: Motorola 68000\n");
        break;
    case 5:
        printf("e_machine: Motorola 88000\n");
        break;
    case 7:
        printf("e_machine: Intel 80860\n");
        break;
    case 8:
        printf("e_machine: MIPS RS3000 Big-Endian\n");
        break;
    case 10:
        printf("e_machine: MIPS RS4000 Big-Endian\n");
        break;
    default:
        printf("e_machine: error\n");
        break;
    }


}
void printELFHeaderType(Elf32_Half type){
    switch(type){
    case 0:
        printf("EI_Type: No file type\n");
        break;
    case 1:
        printf("EI_Type: Relocatable file\n");
        break;
    case 2:
        printf("EI_Type: Executable file\n");
        break;
    case 3:
        printf("EI_Type: Shared object file\n");
        break;
    case 4:
        printf("EI_Type: Core file\n");
        break;
    case 0xff00:
        printf("EI_Type: Processor-specific\n");
        break;
    case 0xffff:
        printf("EI_Type: Processor-specific\n");
        break;
    }

}
void printELFHeaderIdent(unsigned char e_ident[]){
    printf("EI_Ident:\n");
    printf("    EI_MAG0:0x%2x\n",e_ident[0]);
    printf("    EI_MAG1:%c\n",e_ident[1]);
    printf("    EI_MAG2:%c\n",e_ident[2]);
    printf("    EI_MAG3:%c\n",e_ident[3]);
    switch(e_ident[4]){
    case 0:
        printf("    EI_CLASS:Invalid class\n");
        break;
    case 1:
        printf("    EI_CLASS:32-bit objects\n");
        break;
    case 2:
        printf("    EI_CLASS:64-bit objects\n");
        break;
    }

    switch(e_ident[5]){
    case 0:
        printf("    EI_DATA:ELFDATANONE\n");
        break;
    case 1:
        printf("    EI_DATA:ELFDATA2LSB\n");
        break;
    case 2:
        printf("    EI_DATA:ELFDATA2MSB\n");
        break;
    }
    printf("    EI_VERSION:%d\n",e_ident[6]);
    printf("    EI_PAD:%d\n",e_ident[7]);
    printf("    EI_NIDENT:%d\n",e_ident[16]);
}

