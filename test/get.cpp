#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do
                           // this in one cpp file
#include <catch.hpp>
#include <json.hpp>
#include <restless.hpp>
using json = nlohmann::json;

TEST_CASE("Simple Get Request", "[get]") {
        auto result =
            asoni::Handle().get("http://httpbin.org/robots.txt").exec();
        REQUIRE(result.code == 200);
        std::string actual = "User-agent: *\nDisallow: /deny\n";
        REQUIRE(result.body == actual);
}

TEST_CASE("Simple Get Request: HTTPS", "[get]") {
        auto result =
            asoni::Handle().get("https://httpbin.org/robots.txt").exec();
        REQUIRE(result.code == 200);
        std::string actual = "User-agent: *\nDisallow: /deny\n";
        REQUIRE(result.body == actual);
}

TEST_CASE("Get Request with basic auth and headers", "[get]") {
        auto result = asoni::Handle()
                          .get("http://httpbin.org/get", "password")
                          .header({{"Hello", "This is a header"},
                                   {"Second", "Another header"}})
                          .exec();
        REQUIRE(result.code == 200);
        auto response = json::parse(result.body);
        std::string header_hello = response["headers"]["Hello"];
        std::string header_second = response["headers"]["Second"];
        REQUIRE(header_hello == "This is a header");
        REQUIRE(header_second == "Another header");
}

TEST_CASE("Invalid Get Request", "[get]") {
        auto result = asoni::Handle().get("http://xyz").exec();
        REQUIRE(result.code == -1);
        REQUIRE(result.body == "Failed to fetch response.");
}
