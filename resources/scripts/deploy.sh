#!/bin/bash

if [ ! "$TRAVIS_BRANCH" = "master" -a ! "$TRAVIS_BRANCH" = "travis-ci" ]; then
    echo "Nothing to deploy"
    exit 0
fi

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
curl --ftp-create-dirs --ftp-ssl -u $FTP_USER:$FTP_PASSWORD ftp://$FTP_SERVER/bin/ -T $PACKAGE || echo "Deploy failed"
