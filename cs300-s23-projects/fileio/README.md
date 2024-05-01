fileio
======================

An Input/Output (I/O) library that supports operations such as reading data from files (io300_readc and io300_read), writing data to files (io300_writec, io300_write), or moving to a different offset within the file (io_seek).
The library provides a wrapper around POSIX compliant sys calls and implements a cache to speed up access to data and reduce the number of disk operations required.

For example, when an application wants to read from a file, it can call io300_read instead of the system call read. The arguments to io300_read are:

io300_read(struct io300_file* const f, char* const buff, size_t const sz)
f is the file to read from and sz is the number of bytes to read. buff is the application buffer – a region of memory (provided by the caller) where the read bytes should go. In other words, io300_read should write bytes from the data stored in a file (or the cache) into buff. That way, when the io300_read call finishes, the caller can read the bytes returned from buff.

The io300_write function has the same structure:

io300_write(struct io300_file* const f, const char* buff, size_t const sz)
In this case, the caller puts the bytes they want to write into the application buffer buff. They then call io300_write on buff, which writes the first sz bytes of buff to the file f.

