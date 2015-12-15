
Welcome to uvxx's documentation!
================================

Overview
--------

uvxx is a coroutine-based multi-platform C++ networking library that uses `yieldxx`_ to provide a high-level synchronous API on top of the `libuv`_ event loop.

.. note::
	In case you find errors in this documentation you can help by sending
	`pull requests <https://github.com/LEW21/uvxx>`_!

.. _yieldxx: https://github.com/LEW21/yieldxx
.. _libuv: https://github.com/libuv/libuv


Features
--------

* Fast event loop based on `libuv`_.
* Lightweight execution units based on `yieldxx`_, a C++ implementation of coroutines.
* Designed for C++14.


Installation
------------

Installation instructions can be found on `the README <https://github.com/LEW21/uvxx/blob/master/README.md>`_.


Documentation
-------------

.. note::
	uvxx is a work in progress, descriptions of some methods may refer to types that do not exist yet.

.. toctree::
	:maxdepth: 1

	errors
	handle
	stream
	tcp
