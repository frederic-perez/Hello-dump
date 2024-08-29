#ifndef HELLO_DUMP__AUX_RAW_HPP
#define HELLO_DUMP__AUX_RAW_HPP

// Unfortunately __func__ is not understood by vs12. This is a workaround:
#ifndef __func__
#  define __func__ __FUNCTION__
#endif

namespace aux {

} // namespace aux

#endif // HELLO_DUMP__AUX_RAW_HPP
