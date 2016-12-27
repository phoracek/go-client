#ifndef NETWORKING_H_
#define NETWORKING_H_


#include <string>


class Networking {
public:
    Networking(const std::string &api_address);
    ~Networking();

    std::string send(const std::string &method, const std::string &path,
                     const std::string &data = "");

    int getOutgoingRequests() const;

private:
    std::string api_address_;

    int outgoing_requests_;
};


#endif // NETWORKING_H_
