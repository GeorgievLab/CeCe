#!/bin/bash

if [ ! "$TRAVIS_BRANCH" = "master" -a ! "$TRAVIS_BRANCH" = "travis-ci" ]; then
    echo "Nothing to deploy"
    exit 0
fi

SUFFIX=""

# Add suffix for non-tag releases
if [ -z "$TRAVIS_TAG" ]; then
    SUFFIX="$TRAVIS_BRANCH"
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

if [ -n "$SUFFIX" ]; then
    PACKAGE_NEW=build/CeCe-$VERSION-$PLATFORM-$SUFFIX.tar.gz
    mv $PACKAGE $PACKAGE_NEW
    PACKAGE=$PACKAGE_NEW
fi

echo "Package: $PACKAGE"
curl --ftp-create-dirs --ftp-ssl -u $FTP_USER:$FTP_PASSWORD ftp://$FTP_SERVER/bin/ -T $PACKAGE || echo "Deploy failed"
