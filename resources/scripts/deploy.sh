#!/bin/bash

PLATFORM=""

case $TRAVIS_OS_NAME in
    "linux")
        echo "Linux platform"
        PLATFORM="Linux"
        ;;

    "osx")
        echo "Mac OS X platform"
        PLATFORM="Darwin"
        ;;

    *)
        echo "Unknown OS"
        exit 1
        ;;
esac

PACKAGE=build/CeCe-$VERSION-$PLATFORM.tar.gz

echo "Package: $PACKAGE"
sshpass -p "$FTP_PASSWORD" scp $PACKAGE $FTP_USER@$FTP_SERVER:bin

