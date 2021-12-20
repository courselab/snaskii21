#!/bin/bash

# System information

OS=`uname`
LINUX_DISTRO=`awk -F'=' '/^ID=/ {print $2}' /etc/*-release`

# Messages

USAGE_MESSAGE='Usage: ./install_all [options]\nOptions:\n\t--prefix\tChoose the absolute path where ttsnake will be installed'
DEPENDENCIES_INSTALLED_MESSAGE='All dependencies are installed'
DEPENDECIES_MISSING_MESSAGE='There are missing dependencies'
BREW_MISSING_MESSAGE='Error: homebrew package manager was not found on your system'
DEPENDENCIES_ERROR_MESSAGE='Error: failed to install dependencies'

# Check is the prefix option was used

if [[ !(-z "$1") ]]
then
    PREFIX_DEFINED=`echo $1 | awk -F'=' '/^--prefix=/ {print $1}'`
    if [[ !(-z "$PREFIX_DEFINED") ]]
    then
        PREFIX=`echo $1 | awk -F'=' '/^--prefix=/ {print $2}'`
    else
        echo -e $USAGE_MESSAGE
        exit 1
    fi
fi

# Check if dependencies are already installed

if [[ !(-z `command -v autoconf`) && !(-z `command -v automake`) && -f /usr/include/ncurses.h ]]
then
    echo -e $DEPENDENCIES_INSTALLED_MESSAGE
else
    echo -e $MISSING_DEPENDENCIES_MESSAGE

    # Install dependencies using the system package manager

    if [[ "$OS" = "Linux" ]]
    then
        if [[ !(-z `command -v apt`) ]]
        then
            sudo apt update
            sudo apt install automake autoconf libncurses5-dev
        elif [[ !(-z `command -v pacman`) ]]
        then
            sudo pacman -S automake autoconf ncurses
        fi
    elif [[ "$OS" = "Darwin" ]]
    then
        if [[ !(-z `command -v brew`) ]]
        then
            brew install autoconf automake ncurses
        else
            echo -e $BREW_MISSING_MESSAGE
            exit 1
        fi
    fi
fi

# Check if dependencies were successfully installed before proceeding to installation

if [[ !(-z `command -v autoconf`) && !(-z `command -v automake`) && -f /usr/include/ncurses.h ]]
then
    ./autogen.sh

    if [[ !(-z "$PREFIX_DEFINED") ]]
    then
        ./configure --prefix=$PREFIX
    else
        ./configure
    fi

    make
    make install
else
    echo -e $DEPENDENCIES_ERROR_MESSAGE
fi
