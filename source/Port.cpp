#include "Port.h"

const char *portgetenv(const char *str)
{
    static size_t buf_size = 1024;
    static std::unique_ptr<char> buf(new char[buf_size]);

    size_t size_req = 0;
    memset(buf.get(), 0, buf_size);
    auto err = getenv_s(&size_req, buf.get(), buf_size, str);
    if (err == ERANGE)
    {
        buf_size = size_req;
        buf.reset(new char[buf_size]);
        memset(buf.get(), 0, buf_size);
        err = getenv_s(&size_req, buf.get(), buf_size, str);
    }
    return err ? nullptr : buf.get();
}