gcc -o show show.c fs.c
gcc -o create create.c fs.c
gcc -o help help.c fs.c
gcc -o init init.c fs.c 
gcc -o clearfs clearfs.c fs.c
gcc -o write write.c fs.c
gcc -o remove remove.c fs.c
gcc -o printfs printfs.c fs.c
gcc -o listfiles listfiles.c fs.c


init(){
    ./init
}

help(){
    ./help
}

create(){
    ./create $1 $2
}

remove(){
    ./remove $1
}

write(){
    ./write $1 $2
}

show(){
    ./show $1
}

printfs(){
    ./printfs
}

clearfs(){
    ./clearfs
}

listfiles(){
    ./listfiles
}



gcc -o showpath ./dir_extensions/showpath.c fs.c ./dir_extensions/fs_dirextension.c
gcc -o jumpinto ./dir_extensions/jumpinto.c fs.c ./dir_extensions/fs_dirextension.c
gcc -o createdir ./dir_extensions/createdir.c fs.c ./dir_extensions/fs_dirextension.c
gcc -o removedir ./dir_extensions/removedir.c fs.c ./dir_extensions/fs_dirextension.c

showpath(){
    ./showpath
}

jumpinto(){
    ./jumpinto $1
} 

createdir(){
    ./createdir $1
}

removedir(){
    ./removedir $1
}


./help