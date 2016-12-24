#include "networking.h"

extern "C" {
#include <curl/curl.h>
}

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string *) userp)->append((char *) contents, size * nmemb);
    return size * nmemb;
}

Networking::Networking(std::string api_address)
        : api_address_(api_address),
          outgoing_requests_(0) {
    curl_global_init(CURL_GLOBAL_ALL);
}

Networking::~Networking() {}


#include <iostream>

std::string
Networking::send(std::string method, std::string path, std::string data) {
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
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        // cleanup
        curl_easy_cleanup(curl);
    }

    outgoing_requests_ -= 1;

    return answer;
}

std::string
Networking::sendThreaded(std::string method, std::string path,
                         std::string data) {
    // auto answer = std::async(send, method, path, data);

    return ""; // answer.get();
}

int Networking::outgoingRequests() {
    return outgoing_requests_;
}