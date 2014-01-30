Root Less ArchLinux
===================

Try to use Archlinux official package repository to make a root less linux distribution.

For now, it is just a test, nothing else, don't expect too much.

Development
-----------
As you can see, there is only one file. Why, for now it's really simple, I just want
that someone just get this file and execute it. Maybe in a future we may create a "real"
python module/program and deploy using pip. However, in each case we need to have a solution
to pack everything and give only one file to deploy a Root Less ArchLinux in hostile environnement.

Usage
-----
Hum hum, very limited features and not fully implemented ! ;)

To bootstrap the environment you could define the RLARCH_PREFIX

	$ export RLARCH_PREFIX=<your path to install the RLAL>

Once this variable is defined you have to create the env:
    
    $ rlarch --create-env

Then you could install load the ArchLinux referential using:

	$ rlarch -Sy

Finally, you could install package:

	$ rlarch -S gcc

Note
----
* You have to be sure to have xz tools available on your PATH
* You have to be sure to have patchelf available on your PATH

Next Step
---------
* write the installed package into the db/local directory.  
  we do not keep what packages are installed :(
* change the workflow, we should prefer: download all packages, verify checksums then install packages.

Requirements:
-------------
* xz utils
* patchelf from NixOs project (http://nixos.org/patchelf.html)
* Python version 2.7 (do not work for now wyth Python 3.3)

Packages
--------
* valgrind: you have to export  VALGRIND_LIB=$RLARCH_PREFIX/usr/lib/valgrind
