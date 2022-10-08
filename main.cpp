#include "./include/calc.h"
#include "crow/middlewares/cors.h"
#include "./include/json.hpp"

int main()
{
    // Enable CORS
    crow::App<crow::CORSHandler> app;

    // Customize CORS
    auto& cors = app.get_middleware<crow::CORSHandler>();

    // clang-format off
    cors
      .global()
        .headers("X-Custom-Header", "Upgrade-Insecure-Requests")
        .methods("POST"_method, "GET"_method)
      .prefix("/cors")
        .origin("*")
      .prefix("/nocors")
        .origin("*")
        .methods("GET"_method)
        .max_age(3600);

    CROW_ROUTE(app, "/")
    ([]() {
        return "Check Access-Control-Allow-Methods header";
    });

    CROW_ROUTE(app, "/cors")
    ([]() {
        return "Check Access-Control-Allow-Origin header";
    });

    CROW_ROUTE(app, "/calc/<string>")
    ([](const crow::request& req, crow::response &res, std::string s) {
        Calc calc;
        double result = calc.evaluate(s);
        nlohmann::json j;
        j["result"] = result;
        res.write(j.dump());
        res.end();
    });

    app.loglevel(crow::LogLevel::Warning);
    app.port(18080).multithreaded().run();
}
