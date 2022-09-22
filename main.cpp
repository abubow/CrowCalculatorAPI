#include "./include/calc.h"
#include "crow/middlewares/cors.h"

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
        .origin("example.com")
      .prefix("/nocors")
        .ignore();
    // clang-format on

    CROW_ROUTE(app, "/")
    ([]() {
        return "Check Access-Control-Allow-Methods header";
    });

    CROW_ROUTE(app, "/cors")
    ([]() {
        return "Check Access-Control-Allow-Origin header";
    });

    CROW_ROUTE(app, "/calc/<string>")
    ([](const crow::request& req, std::string s) {
        Calc calc;
        double result = calc.evaluate(s);
        crow::json::wvalue x = {{"result", result}};
        return crow::response(x);
    });

    app.port(18080).run();

    return 0;
}