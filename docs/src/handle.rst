
.. _handle:

:cpp:class:`uvxx::handle` --- Base handle
=========================================

:cpp:class:`uvxx::handle` is the base type for all uvxx handle types.

All the handles are movable and default-constructible. Default-construction does not initialize the handle, i.e. does not allocate the uv_foo_t structure.


Data types
----------

.. cpp:class:: uvxx::handle

	The base uvxx handle type.


Wrapped libuv structure
-----------------------

.. cpp:function:: uv_handle_t* uvxx::handle::uv_handle()

	Pointer to the `uv_handle_t`_ structure.

.. _uv_handle_t: http://docs.libuv.org/en/v1.x/handle.html


API
---

.. cpp:function:: uvxx::handle::operator bool() const

	Returns true if the handle is not empty.

	Default-constructed, closed and moved-from handles are empty.

	.. note::
		Not yet implemented.

.. cpp:function:: bool uvxx::handle::is_active() const

	Returns true if the handle is active, false if it's inactive (or empty). What
	"active" means depends on the type of handle:

	- A :cpp:class:`uvxx::async` handle is always active and cannot be deactivated, except
	  by closing it with :cpp:func:`uvxx::handle::close()`.

	- A :cpp:class:`uvxx::pipe`, :cpp:class:`uvxx::tcp`, :cpp:class:`uvxx::udp`, etc. handle - basically any handle that
	  deals with i/o - is active when it is doing something that involves i/o,
	  like reading, writing, connecting, accepting new connections, etc.

	- A :cpp:class:`uvxx::check`, :cpp:class:`uvxx::idle`, :cpp:class:`uvxx::timer`, etc. handle is active when it has
	  been started with a call to :cpp:func:`uvxx::check::start()`, :cpp:func:`uvxx::idle::start()`, etc.

	Rule of thumb: if a handle of type :cpp:class:`uvxx::foo` has a :cpp:func:`uvxx::foo::start()`
	function, then it's active from the moment that function is called.
	Likewise, :cpp:func:`uvxx::foo::stop()` deactivates the handle again.

	.. note::
		Not yet implemented.

.. cpp:function:: void uvxx::handle::close(xx::task&)

	Synchronically close the handle.

	Call this method only if you want your task to wait until the handle becomes closed. In other cases, the handle's destructor will automatically close the handle asynchronically.
