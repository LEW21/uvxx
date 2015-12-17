
.. _tcp:

:cpp:class:`uvxx::tcp` --- TCP handle
=====================================

TCP handles are used to represent both TCP streams and servers.


Data types
----------

.. cpp:class:: uvxx::tcp: public uvxx::stream

	TCP handle type.


Wrapped libuv structure
-----------------------

.. cpp:function:: uv_tcp_t* uvxx::tcp::uv_tcp()

	Pointer to the `uv_tcp_t`_ structure.

.. _uv_tcp_t: http://docs.libuv.org/en/v1.x/tcp.html


API
---

.. cpp:function:: uvxx::tcp(uv_loop_t*)

	Construct the handle. No socket is created as of yet.

.. cpp:function:: void uvxx::tcp::bind(const std::string& address, int port)

	Bind the handle to an address and port.

	Both IPv4 and IPv6 addresses are supported.

	When the port is already taken, you can expect to see an ``UV_EADDRINUSE``
	error from either :cpp:func:`uvxx::tcp::bind`, :cpp:func:`uvxx::stream::listen` or
	:cpp:func:`uvxx::tcp::connect`. That is, a successful call to this function does
	not guarantee that the call to :cpp:func:`uvxx::stream::listen` or :cpp:func:`uvxx::tcp::connect`
	will succeed as well.

.. cpp:function:: xx::generated<uvxx::tcp> uvxx::tcp::listen(xx::task&, int backlog)

	Listens for incomming connection. `backlog` indicates the number of
	connections the kernel might queue, same as `listen(2)`.

	Accepts and yields each connection received.

	.. note::
		It performs both uv_stream_listen and uv_stream_accept.

.. cpp:function:: void uvxx::tcp::connect(xx::task&, const std::string& address, int port)

	Establish an IPv4 or IPv6 TCP connection.

	Both IPv4 and IPv6 addresses are supported.

	.. note::
		Not yet implemented.

.. cpp:function:: std::string uvxx::tcp::getsockname()

	Get the current address to which the handle is bound.

	.. note::
		This method's signature may get changed in the future.

.. cpp:function:: std::string uvxx::tcp::getpeername()

	Get the address of the peer connected to the handle.

	.. note::
		This method's signature may get changed in the future.

.. cpp:function:: int uvxx::tcp::open(uv_os_sock_t sock)

	Open an existing file descriptor or SOCKET as a TCP handle.

	The file descriptor is set to non-blocking mode.

	.. note::
		The passed file descriptor or SOCKET is not checked for its type, but
		it's required that it represents a valid stream socket.

	.. note::
		Not yet implemented.

.. cpp:function:: void uvxx::tcp::nodelay(bool enable)

	Enable / disable Nagle's algorithm.

	.. note::
		This method's signature may get changed in the future.

.. cpp:function:: void uvxx::tcp::keepalive(bool enable, uint delay)

	Enable / disable TCP keep-alive. `delay` is the initial delay in seconds,
	ignored when `enable` is zero.

	.. note::
		This method's signature may get changed in the future.

.. cpp:function:: void uvxx::tcp::simultaneous_accepts(bool enable)

	Enable / disable simultaneous asynchronous accept requests that are
	queued by the operating system when listening for new TCP connections.

	This setting is used to tune a TCP server for the desired performance.
	Having simultaneous accepts can significantly improve the rate of accepting
	connections (which is why it is enabled by default) but may lead to uneven
	load distribution in multi-process setups.

	.. note::
		This method's signature may get changed in the future.

.. seealso:: The :cpp:class:`stream` API methods also apply.
