This is an LD_PRELOAD hack to prevent GNUstep libraries from creating
``$HOME/GNUstep/Library`` directory.

It is meant to work around `Debian bug #646031`_, which affects ``lsar`` and
``unar`` binaries from theunarchiver_ package.

.. _theunarchiver: https://unarchiver.c3.cx/
.. _Debian bug #646031: https://bugs.debian.org/646031
