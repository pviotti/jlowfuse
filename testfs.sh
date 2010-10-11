#!/bin/sh
(
    cd dist
    echo "Mounting..."
    java -Djava.library.path=. -jar testfs.jar
    ret=$?
    echo
    echo "Return code: $ret"
    echo "... umounting"
    sudo umount /mnt1

)