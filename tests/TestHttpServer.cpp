#include "gtest/gtest.h"

#include <cpprest/http_msg.h>
#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <cpprest/uri.h>

#include "HttpServer.h"

#include <chrono>
#include <exception>
#include <future>
#include <iostream>
#include <string>


using namespace std::chrono_literals;

class TestHttpServerFixture : public ::testing::TestWithParam<std::string> {
protected:
    web::json::value makePostBodyWithDirection(const std::string& direction) const {
        web::json::value body;
        body["direction"] = web::json::value::string(U(direction));
        return body;
    }

    web::http::client::http_client client{web::uri{"http://0.0.0.0:8080/drive"}};
};

TEST_P(TestHttpServerFixture, directionAccepted) {
    HttpServer::Server server{"http://0.0.0.0:8080/drive"};
    auto serverTask = std::async([&server](auto duration){
        return server.run(duration);
    }, 1s);

    auto direction = GetParam();
    
    auto response = client.request(web::http::methods::POST, "", makePostBodyWithDirection(direction))
        .then([=](web::http::http_response response) {
            std::cout << "Received response status code: " << response.status_code() << '\n';
            return response;
        });
    
    ASSERT_EQ(200, response.get().status_code());
}

INSTANTIATE_TEST_SUITE_P(
        AcceptedDirections,
        TestHttpServerFixture,
        ::testing::Values(
                "right", "left", "forward", "backward"
        ));


class TestHttpServerNegaticeCaseFixture : public ::testing::Test {
protected:
    web::json::value makePostBodyWithDirection(const std::string& direction) const {
        web::json::value body;
        body["direction"] = web::json::value::string(U(direction));
        return body;
    }

    web::http::client::http_client client{web::uri{"http://0.0.0.0:8080/drive"}};
};

TEST_F(TestHttpServerNegaticeCaseFixture, directionNotAccepted) {
    HttpServer::Server server{"http://0.0.0.0:8080/drive"};
    auto serverTask = std::async([&server](auto duration){
        return server.run(duration);
    }, 2s);

    auto direction = "leftishright";
    
    auto response = client.request(web::http::methods::POST, "", makePostBodyWithDirection(direction))
        .then([=](web::http::http_response response) {
            std::cout << "Received response status code: " << response.status_code() << '\n';
            return response;
        });
    
    ASSERT_EQ(500, response.get().status_code());
}

TEST_F(TestHttpServerNegaticeCaseFixture, directionMissing) {
    HttpServer::Server server{"http://0.0.0.0:8080/drive"};
    auto serverTask = std::async([&server](auto duration){
        return server.run(duration);
    }, 1s);
    
    auto response = client.request(web::http::methods::POST, "")
        .then([=](web::http::http_response response) {
            std::cout << "Received response status code: " << response.status_code() << '\n';
            return response;
        });
    
    ASSERT_EQ(500, response.get().status_code());
}