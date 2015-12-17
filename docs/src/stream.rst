
.. _stream:

:cpp:class:`uvxx::stream` --- Stream handle
===========================================

Stream handles provide an abstraction of a duplex communication channel.
:cpp:class:`uvxx::stream` is an abstract type, uvxx provides 3 stream implementations
in the for of :cpp:class:`uvxx::tcp`, :cpp:class:`uvxx::pipe` and :cpp:class:`uvxx::tty`.


Data types
----------

.. cpp:class:: uvxx::stream: public uvxx::handle

	Stream handle type.


Wrapped libuv structure
-----------------------

.. cpp:function:: uv_stream_t* uvxx::stream::uv_stream()

	Pointer to the `uv_stream_t`_ structure.

.. _uv_stream_t: http://docs.libuv.org/en/v1.x/stream.html


API
---

.. cpp:function:: xx::generated<std::string> uvxx::stream::read(xx::task&)

	Read data from an incoming stream. Yields each time new data is received.

	To stop reading, simply destroy the generator.

.. cpp:function:: void uvxx::stream::write(const std::string& data)

	Asynchronously write data to the stream.

.. cpp:function:: bool uvxx::stream::is_readable() const

	Returns true if the stream is readable, false otherwise.

.. cpp:function:: bool uvxx::stream::is_writable() const

	Returns true if the stream is writable, false otherwise.

.. seealso:: The :cpp:class:`uvxx::handle` API methods also apply.
