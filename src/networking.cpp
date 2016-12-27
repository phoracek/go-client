#include "networking.h"

#include <SDL_log.h>

extern "C" {
#include <curl/curl.h>
}


size_t write_callback(void *contents, size_t size, size_t number,
                      void *user_data) {
    ((std::string *) user_data)->append((char *) contents, size * number);
    return size * number;
}

Networking::Networking(const std::string &api_address)
        : api_address_(api_address),
          outgoing_requests_(0) {
    curl_global_init(CURL_GLOBAL_ALL);
}

Networking::~Networking() {}

std::string
Networking::send(const std::string &method, const std::string &path,
                 const std::string &data) {
    outgoing_requests_ += 1;
    CURL *curl;
    CURLcode res;
    std::string answer = "";

    curl = curl_easy_init();

    if (curl) {
        // address
        curl_easy_setopt(curl, CURLOPT_URL,
                         (api_address_ + path).c_str());


        // follow redirection
        // curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        // https verification
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

        // post data
        if (data != "") {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS,
                             data.c_str());
        }

        // method
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());

        // write func
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &answer);

        // send request
        res = curl_easy_perform(curl);

        // error check
        if (res != CURLE_OK)
            SDL_Log("curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        // cleanup
        curl_easy_cleanup(curl);
    }

    outgoing_requests_ -= 1;

    return answer;
}

int Networking::getOutgoingRequests() const {
    return outgoing_requests_;
}
