#ifndef UTILS_MESSAGE_H
#define UTILS_MESSAGE_H

#include <memory>

#include <google/protobuf/message.h>

#define SCAN_KEY "UOS-COOPERATION"

#pragma pack(push, 1)
struct MessageHeader {
    uint64_t size;
};
#pragma pack(pop)

const ssize_t header_size = sizeof(MessageHeader);

namespace MessageHelper {

inline std::vector<char> genMessage(const google::protobuf::Message &msg) {
    MessageHeader header{msg.ByteSizeLong()};

    std::vector<char> buff(header_size + msg.ByteSizeLong());
    memcpy(buff.data(), &header, header_size);
    msg.SerializeToArray(buff.data() + header_size, msg.ByteSizeLong());

    return buff;
}

inline MessageHeader parseMessageHeader(const char *buffer) noexcept {
    MessageHeader header;
    memcpy(&header, buffer, header_size);
    return header;
}

template <typename T>
inline T parseMessageBody(const char *buffer, size_t size) noexcept {
    T msg;
    msg.ParseFromArray(buffer, size);

    return msg;
}

} // namespace MessageHelper

#endif // !UTILS_MESSAGE_H
