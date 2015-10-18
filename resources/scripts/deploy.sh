#!/bin/bash

PLATFORM=""

case $TRAVIS_OS_NAME in
    "linux")
        PLATFORM="Linux"
        ;;

    "osx")
        PLATFORM="Darwin"
        ;;

    *)
        echo "Unknown OS"
        exit 1
        ;;
esac

PACKAGE=build/CeCe-$VERSION-$PLATFORM.tar.gz

sshpass -p "$FTP_PASSWORD" scp $PACKAGE $FTP_USER@$FTP_SERVER:bin
