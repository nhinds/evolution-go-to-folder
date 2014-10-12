## Evolution "Go To Folder" plugin

An evolution plugin/module which allows you to jump to a folder

# Building

    ./autogen.sh
    make

# Installing

If you don't use a package manager, just run:

    sudo make install

If you use a package manager, try:

    sudo checkinstall --pkgname evolution-go-to-folder --pkgversion 0.1 --nodoc

# Usage

In evolution's mail view, hit `Ctrl+Shift+v`, type to select a folder, hit `Enter`.

Alternatively, use the `Folder -> Go To...` menu item.
