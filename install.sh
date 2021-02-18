#!/bin/bash

IM7PROJECT=${HOME}/imagemagick/v7/project
IM7INSTALL=${HOME}/imagemagick/v7/install

if [ -d  "$IM7PROJECT" ]; then
    echo "$IM7PROJECT already exists."

else

#=---=[   DOWNLOWD SOURCE   ]=--------------------------------=#

    REPOSITORY="https://github.com/ImageMagick/ImageMagick"

    mkdir -p $IM7PROJECT
    mkdir -p $IM7INSTALL

    git clone $REPOSITORY $IM7PROJECT

#=---=[   LINK MODULES   ]=-----------------------------------=#

    rm -r $IM7PROJECT/filters
    ln -s $PWD/filters $IM7PROJECT/filters

fi

#=---=[   BUILD IMAGEMAGICK   ]=------------------------------=#

cd $IM7PROJECT

aclocal
automake
autoconf

./configure --prefix=$IM7INSTALL                               \
            --with-quantum-depth=32                            \
            --enable-hdri                                      \
            --disable-docs                                     \
            --with-modules=yes

make install
